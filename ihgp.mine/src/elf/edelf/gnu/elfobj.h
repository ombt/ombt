#ifndef __ELFOBJ_H
#define __ELFOBJ_H
// class for accessing an ELF object file.

// headers 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#if EXPTOOLS==1 || GNUVERSION==1
#include "stab.h"
#else
#include <stab.h>
#endif
#include <elf.h>
#include <link.h>

// local headers
#include "returns.h"
#include "debug.h"

// swab macros
#define EOSWABIT(_eo, _data) \
	_eo.eoswabit((unsigned char *)(&_data), sizeof((_data)))

#define SWABIT(_data) { \
	(_data) = swabit((unsigned char *)(&_data), sizeof((_data))); \
}

// ELF object file class
class ElfObject {
public:
	// helper data
	struct offsets {
		long start;
		long end;
		long offset;
	};

	// constructors and destructor
	ElfObject(const char *fnm):
		_fname(NULL), _efd(NOTOK), _lastoper(NOTOK),
		_pehdr(NULL), _pshdrs(NULL), _pphdrs(NULL),
		_pshdrdata(NULL), _pverdefnms(NULL), _poffsets(NULL),
		_hasehdr(0), _hasrelocs(0), _hassymbols(0), _hasstabs(0), 
		_hasphdrs(0), _hasshdrs(0), _hashash(0), _hasstrings(0),
		_hasverdef(0), _hasverneed(0), _hasversym(0), _hasdynamic(0),
		showsec(-1), showstart(65565), showend(-1), showoffset(0),
		_swabdata(-1)
	{
		MustBeTrue(fnm != NULL && *fnm != '\0');
		_fname = new char [strlen(fnm)+1];
		MustBeTrue(_fname != NULL);
		*_fname = '\0';
		strcpy(_fname, fnm);
	}
	~ElfObject() {
		_lastoper = NOTOK;
		_hasehdr = 0;
		_hasrelocs = 0;
		_hassymbols = 0;
		_hasstabs = 0;
		_hasphdrs = 0;
		_hasshdrs = 0;
		_hashash = 0;
		_hasstrings = 0;
		_hasverdef = 0;
		_hasverneed = 0;
		_hasversym = 0;
		_hasdynamic = 0;
		_swabdata = -1;
		showsec = -1;
		showstart = 65565;
		showend = -1;
		showoffset = 0;
		if (_efd != NOTOK)
		{
			close(_efd);
			_efd = NOTOK;
		}
		if (_fname != NULL)
		{
			delete [] _fname;
			_fname = NULL;
		}
		if (_pverdefnms != NULL)
		{
			for (int i=0; _pverdefnms[i] != NULL; i++)
			{
				delete [] _pverdefnms[i];
			}
			delete [] _pverdefnms;
			_pverdefnms = NULL;
		}
		if (_pphdrs != NULL)
		{
			delete [] _pphdrs;
			_pphdrs = NULL;
		}
		if (_poffsets != NULL)
		{
			for (int s=0; s<_pehdr->e_shnum; s++)
			{
				if (_poffsets[s] != NULL)
				{
					delete [] _poffsets[s];
					_poffsets[s] = NULL;
				}
			}
			delete [] _poffsets;
			_poffsets = NULL;
		}
		if (_pshdrdata != NULL)
		{
			for (int s=0; s<_pehdr->e_shnum; s++)
			{
				if (_pshdrdata[s] != NULL)
				{
					delete [] _pshdrdata[s];
					_pshdrdata[s] = NULL;
				}
			}
			delete [] _pshdrdata;
			_pshdrdata = NULL;
		}
		if (_pshdrs != NULL)
		{
			delete [] _pshdrs;
			_pshdrs = NULL;
		}
		if (_pehdr != NULL)
		{
			delete _pehdr;
			_pehdr = NULL;
		}
	}

	// i/o utilities
	void openwronly() {
		if (_efd == NOTOK)
		{
			MustBeTrue((_efd = ::open(_fname, O_WRONLY)) != NOTOK);
			_lastoper = O_WRONLY;
		}
		else if (_lastoper != O_WRONLY)
		{
			close(_efd);
			_efd = NOTOK;
			MustBeTrue((_efd = ::open(_fname, O_WRONLY)) != NOTOK);
			_lastoper = O_WRONLY;
		}
		return;
	}
	void openrdonly() {
		if (_efd == NOTOK)
		{
			MustBeTrue((_efd = ::open(_fname, O_RDONLY)) != NOTOK);
			_lastoper = O_RDONLY;
		}
		else if (_lastoper != O_RDONLY)
		{
			close(_efd);
			_efd = NOTOK;
			MustBeTrue((_efd = ::open(_fname, O_RDONLY)) != NOTOK);
			_lastoper = O_RDONLY;
		}
		return;
	}
	void openrdwr() {
		if (_efd == NOTOK)
		{
			MustBeTrue((_efd = ::open(_fname, O_RDWR)) != NOTOK);
			_lastoper = O_RDWR;
		}
		else if (_lastoper != O_RDWR)
		{
			close(_efd);
			_efd = NOTOK;
			MustBeTrue((_efd = ::open(_fname, O_RDWR)) != NOTOK);
			_lastoper = O_RDWR;
		}
		return;
	}

	// read/write file header data
	void readehdr() {
		// open file, if not opened already
		openrdonly();
		if (_pehdr == NULL)
		{
			_pehdr = new Elf32_Ehdr;
			MustBeTrue(_pehdr != NULL);
		}

		// read in datga
		MustBeTrue(::lseek(_efd, 0, SEEK_SET) != NOTOK);
		MustBeTrue(::read(_efd, _pehdr, 
			sizeof(Elf32_Ehdr)) == sizeof(Elf32_Ehdr));

		// check if file is 32 bits.
		MustBeTrue(_pehdr->e_ident[EI_CLASS] == ELFCLASS32);

		// check if we need to swab the data
		if (_pehdr->e_ident[EI_DATA] == ELFDATA2LSB)
			_swabdata = 1;
		else
			_swabdata = 0;

		// do we swab the data?
		if (isSwabable())
		{
			SWABIT(_pehdr->e_type);
			SWABIT(_pehdr->e_machine);
			SWABIT(_pehdr->e_version);
			SWABIT(_pehdr->e_entry);
			SWABIT(_pehdr->e_phoff);
			SWABIT(_pehdr->e_shoff);
			SWABIT(_pehdr->e_flags);
			SWABIT(_pehdr->e_ehsize);
			SWABIT(_pehdr->e_phentsize);
			SWABIT(_pehdr->e_phnum);
			SWABIT(_pehdr->e_shentsize);
			SWABIT(_pehdr->e_shnum);
			SWABIT(_pehdr->e_shstrndx);
		}

		// enable flag
		_hasehdr = 1;
		return;
	}
	void writeehdr() {
		openrdwr();
		MustBeTrue(_pehdr != NULL)
		MustBeTrue(::lseek(_efd, 0, SEEK_SET) != NOTOK);
		MustBeTrue(::write(_efd, _pehdr, 
			sizeof(Elf32_Ehdr)) == sizeof(Elf32_Ehdr));
		return;
	}

	// read/write section header data
	void readshdrs() {
		// sanity check
		MustBeTrue(hasehdr());

		// check if any section headers 
		MustBeTrue(_pehdr->e_shoff != 0);

		// bytes to write
		int numbytes = _pehdr->e_shentsize*_pehdr->e_shnum;
		MustBeTrue(numbytes > 0);

		// allocate a buffer for reading in section header table
		if (_pshdrs == NULL)
		{
			_pshdrs = (Elf32_Shdr *) new char [numbytes];
			MustBeTrue(_pshdrs != NULL);
		}

		// read in section header tables
		MustBeTrue(::lseek(_efd, _pehdr->e_shoff, SEEK_SET) != NOTOK)
		MustBeTrue(::read(_efd, _pshdrs, numbytes) == numbytes);

		// do we need to swab?
		if (isSwabable())
		{
			for (int s=0; s<_pehdr->e_shnum; s++)
			{
				SWABIT(_pshdrs[s].sh_name);
				SWABIT(_pshdrs[s].sh_type);
				SWABIT(_pshdrs[s].sh_flags);
				SWABIT(_pshdrs[s].sh_addr);
				SWABIT(_pshdrs[s].sh_offset);
				SWABIT(_pshdrs[s].sh_size);
				SWABIT(_pshdrs[s].sh_link);
				SWABIT(_pshdrs[s].sh_info);
				SWABIT(_pshdrs[s].sh_addralign);
				SWABIT(_pshdrs[s].sh_entsize);
			}
		}

		// get section header string table index
		MustBeTrue(_pehdr->e_shstrndx != SHN_UNDEF)

		// allocate array for storing section-specific data
		if (_pshdrdata == NULL)
		{
			_pshdrdata = new char * [_pehdr->e_shnum];
			MustBeTrue(_pshdrdata != NULL);
			for (int s=0; s<_pehdr->e_shnum; s++)
			{
				_pshdrdata[s] = NULL;
			}
		}

		// allocate a string table
		if (_pshdrdata[_pehdr->e_shstrndx] == NULL)
		{
			_pshdrdata[_pehdr->e_shstrndx] = 
				new char [(_pshdrs+
					_pehdr->e_shstrndx)->sh_size];
			MustBeTrue(_pshdrdata[_pehdr->e_shstrndx] != NULL);
		}

		// read in string table 
		MustBeTrue(::lseek(_efd, 
			(_pshdrs+_pehdr->e_shstrndx)->sh_offset, 
				SEEK_SET) != NOTOK)
		int bytestoread = (_pshdrs+_pehdr->e_shstrndx)->sh_size;
		MustBeTrue(::read(_efd, 
			_pshdrdata[_pehdr->e_shstrndx], 
				bytestoread) == bytestoread);

		// enable flag
		_hasshdrs = 1;
		return;
	}
	void writeshdrs() {
		// file must be open to write
		openrdwr();

		// bytes to write
		int numbytes = _pehdr->e_shentsize*_pehdr->e_shnum;
		MustBeTrue(numbytes > 0);

		// write section header tables
		MustBeTrue(::lseek(_efd, _pehdr->e_shoff, SEEK_SET) != NOTOK);
		MustBeTrue(::write(_efd, _pshdrs, numbytes) == numbytes);

		// write string table 
		MustBeTrue(::lseek(_efd, 
			   (_pshdrs+_pehdr->e_shstrndx)->sh_offset, 
				SEEK_SET) != NOTOK);
		int bytestowrite = (_pshdrs+_pehdr->e_shstrndx)->sh_size;
		MustBeTrue(::write(_efd, 
			   _pshdrdata[_pehdr->e_shstrndx], 
				bytestowrite) == bytestowrite);
		return;
	}

	// read/write program header data
	void readphdrs() {
		// sanity check
		MustBeTrue(hasehdr());

		// check if any section headers 
		MustBeTrue(_pehdr->e_phoff != 0);

		// bytes to write
		int numbytes = _pehdr->e_phentsize*_pehdr->e_phnum;
		MustBeTrue(numbytes > 0);

		// allocate a buffer for reading in section header table
		if (_pphdrs == NULL)
		{
			_pphdrs = (Elf32_Phdr *) new char [numbytes];
			MustBeTrue(_pphdrs != NULL);
		}


		// read in program header tables
		MustBeTrue(::lseek(_efd, _pehdr->e_phoff, SEEK_SET) != NOTOK)
		MustBeTrue(::read(_efd, _pphdrs, numbytes) == numbytes);

		// do we need to swab?
		if (isSwabable())
		{
			for (int p=0; p<_pehdr->e_phnum; p++)
			{
				SWABIT(_pphdrs[p].p_type);
				SWABIT(_pphdrs[p].p_offset);
				SWABIT(_pphdrs[p].p_vaddr);
				SWABIT(_pphdrs[p].p_paddr);
				SWABIT(_pphdrs[p].p_filesz);
				SWABIT(_pphdrs[p].p_memsz);
				SWABIT(_pphdrs[p].p_flags);
				SWABIT(_pphdrs[p].p_align);
			}
		}

		// enable flag
		_hasphdrs = 1;
		return;
	}
	void writephdrs() {
		// file must be open to write
		openrdwr();

		// bytes to write
		int numbytes = _pehdr->e_phentsize*_pehdr->e_phnum;
		MustBeTrue(numbytes > 0);

		// write program header tables
		MustBeTrue(::lseek(_efd, _pehdr->e_phoff, SEEK_SET) != NOTOK);
		MustBeTrue(::write(_efd, _pphdrs, numbytes) == numbytes);
		return;
	}

	// read/write symbol table data
	void readsymbols() {
		// sanity checks
		MustBeTrue(hasstrings());

		// read in symbol table raw data
		for (int s=0; s<_pehdr->e_shnum; s++)
		{
			// skip sections that are not symbol tables
			if (_pshdrs[s].sh_type != SHT_SYMTAB &&
			    _pshdrs[s].sh_type != SHT_DYNSYM)
				continue;

			// allocate a symbol table
			if (_pshdrdata[s] == NULL)
			{
				_pshdrdata[s] = 
					new char [_pshdrs[s].sh_size];
				MustBeTrue(_pshdrdata[s] != NULL);
			}

			// seek and read in symbol table
			MustBeTrue(::lseek(_efd, _pshdrs[s].sh_offset, 
					SEEK_SET) != NOTOK)
			int numbytes = _pshdrs[s].sh_size;
			MustBeTrue(::read(_efd, _pshdrdata[s], 
					numbytes) == numbytes);

			// check if swabable
			if (isSwabable())
			{
				char *p0 = _pshdrdata[s];
				char *pe = _pshdrdata[s]+_pshdrs[s].sh_size;
				for (int y=0; 
				    (p0+y*_pshdrs[s].sh_entsize)<pe; y++)
				{
					Elf32_Sym *psym = 
						(Elf32_Sym *)(p0 + 
						y*_pshdrs[s].sh_entsize);
					SWABIT(psym->st_name);
					SWABIT(psym->st_value);
					SWABIT(psym->st_size);
					SWABIT(psym->st_info);
					SWABIT(psym->st_other);
					SWABIT(psym->st_shndx);
				}
			}

			// enable flag
			_hassymbols = 1;
		}
		return;
	}
	void writesymbols(int s) {
		// file must be open to write
		openrdwr();

		// sanity check
		MustBeTrue(_pshdrs[s].sh_type == SHT_SYMTAB ||
			   _pshdrs[s].sh_type == SHT_DYNSYM);

		// write string and symbol entry
		MustBeTrue(::lseek(_efd, _pshdrs[_pshdrs[s].sh_link].sh_offset, 
				SEEK_SET) != NOTOK)
		int numbytes = _pshdrs[_pshdrs[s].sh_link].sh_size;
		MustBeTrue(::write(_efd, _pshdrdata[_pshdrs[s].sh_link],
				numbytes) == numbytes);
		MustBeTrue(::lseek(_efd, _pshdrs[s].sh_offset,
				SEEK_SET) != NOTOK);
		numbytes = _pshdrs[s].sh_size;
		MustBeTrue(::write(_efd, _pshdrdata[s], numbytes) == numbytes);
		return;
	}

	// read/write strings table data
	void readstrings() {
		// sanity checks
		MustBeTrue(hasshdrs());

		// read in strings table raw data
		for (int s=0; s<_pehdr->e_shnum; s++)
		{
			// skip sections that are not string tables
			if (_pshdrs[s].sh_type != SHT_STRTAB)
				continue;

			// allocate a symbol table
			if (_pshdrdata[s] == NULL)
			{
				_pshdrdata[s] = 
					new char [_pshdrs[s].sh_size];
				MustBeTrue(_pshdrdata[s] != NULL);
			}

			// seek and read in string table
			MustBeTrue(::lseek(_efd, _pshdrs[s].sh_offset, 
					SEEK_SET) != NOTOK)
			int numbytes = _pshdrs[s].sh_size;
			MustBeTrue(::read(_efd, _pshdrdata[s], 
					numbytes) == numbytes);
			// enable flag
			_hasstrings = 1;
		}
		return;
	}
	void writestrings(int s) {
		// file must be open to write
		openrdwr();

		// sanity check
		MustBeTrue(_pshdrs[s].sh_type == SHT_STRTAB);

		// write string table entry
		MustBeTrue(::lseek(_efd, _pshdrs[s].sh_offset,
				SEEK_SET) != NOTOK);
		int numbytes = _pshdrs[s].sh_size;
		MustBeTrue(::write(_efd, _pshdrdata[s], numbytes) == numbytes);
		return;
	}

	// read/write relocation entries data
	void readreloc() {
		// sanity checks
		MustBeTrue(hassymbols());

		// read in relocation raw data
		for (int s=0; s<_pehdr->e_shnum; s++)
		{
			// skip sections that are not relocation tables
			if (_pshdrs[s].sh_type != SHT_REL &&
			    _pshdrs[s].sh_type != SHT_RELA)
				continue;

			// allocate a relocation entry table
			if (_pshdrdata[s] == NULL)
			{
				_pshdrdata[s] = 
					new char [_pshdrs[s].sh_size];
				MustBeTrue(_pshdrdata[s] != NULL);
			}

			// seek and read in relocation entry table
			MustBeTrue(::lseek(_efd, _pshdrs[s].sh_offset, 
					SEEK_SET) != NOTOK)
			int numbytes = _pshdrs[s].sh_size;
			MustBeTrue(::read(_efd, _pshdrdata[s], 
					numbytes) == numbytes);

			// enable flag
			_hasrelocs = 1;
		}
		return;
	}
	void writereloc(int s) {
		// file must be open to write
		openrdwr();

		// sanity check
		MustBeTrue(_pshdrs[s].sh_type == SHT_REL ||
			   _pshdrs[s].sh_type == SHT_RELA);

		// write relocation table entry
		MustBeTrue(::lseek(_efd, _pshdrs[s].sh_offset,
				SEEK_SET) != NOTOK);
		int numbytes = _pshdrs[s].sh_size;
		MustBeTrue(::write(_efd, _pshdrdata[s], numbytes) == numbytes);
		return;
	}

	// read/write hash table data
	void readhash() {
		// sanity checks
		MustBeTrue(hassymbols());

		// read in hash table raw data
		for (int s=0; s<_pehdr->e_shnum; s++)
		{
			// skip sections that are not hash tables
			if (_pshdrs[s].sh_type != SHT_HASH)
				continue;

			// allocate a hash table
			if (_pshdrdata[s] == NULL)
			{
				_pshdrdata[s] = 
					new char [_pshdrs[s].sh_size];
				MustBeTrue(_pshdrdata[s] != NULL);
			}

			// seek and read in hash table
			MustBeTrue(::lseek(_efd, _pshdrs[s].sh_offset, 
					SEEK_SET) != NOTOK)
			int numbytes = _pshdrs[s].sh_size;
			MustBeTrue(::read(_efd, _pshdrdata[s], 
					numbytes) == numbytes);

			// enable flag
			_hashash = 1;
		}
		return;
	}
	void writehash(int s) {
		// file must be open to write
		openrdwr();

		// sanity check
		MustBeTrue(_pshdrs[s].sh_type == SHT_HASH);

		// write hash table entry
		MustBeTrue(::lseek(_efd, _pshdrs[s].sh_offset,
				SEEK_SET) != NOTOK);
		int numbytes = _pshdrs[s].sh_size;
		MustBeTrue(::write(_efd, _pshdrdata[s], numbytes) == numbytes);
		return;
	}

	// read/write version data
	void readversions() {
		// sanity checks
		MustBeTrue(hassymbols());
		MustBeTrue(hasstrings());

		// read in version raw data
		for (int s=0; s<_pehdr->e_shnum; s++)
		{
			// skip sections that are not version tables
			if (_pshdrs[s].sh_type == SHT_SUNW_verdef)
				_hasverdef = 1;
			else if (_pshdrs[s].sh_type == SHT_SUNW_verneed)
				_hasverneed = 1;
			else if (_pshdrs[s].sh_type == SHT_SUNW_versym)
				_hasversym = 1;
			else
				continue;

			// allocate a relocation entry table
			if (_pshdrdata[s] == NULL)
			{
				_pshdrdata[s] = 
					new char [_pshdrs[s].sh_size];
				MustBeTrue(_pshdrdata[s] != NULL);
			}

			// seek and read in relocation entry table
			MustBeTrue(::lseek(_efd, _pshdrs[s].sh_offset, 
					SEEK_SET) != NOTOK)
			int numbytes = _pshdrs[s].sh_size;
			MustBeTrue(::read(_efd, _pshdrdata[s], 
					numbytes) == numbytes);
		}

		// construct version definition names
		if (hasverdef())
		{
			int sec;
			if (_pverdefnms != NULL)
			{
				for (int i=0; _pverdefnms[i] != NULL; i++)
				{
					delete [] _pverdefnms[i];
				}
				delete [] _pverdefnms;
				_pverdefnms = NULL;
			}
			for (sec=0; sec<_pehdr->e_shnum; sec++)
			{
				if (_pshdrs[sec].sh_type != SHT_SUNW_verdef)
					continue;
				int maxverno = _pshdrs[sec].sh_info;
                		_pverdefnms = new char * [maxverno+2];
				MustBeTrue(_pverdefnms != NULL);
				for (int i=0; i<(maxverno+2); i++)
				{
					_pverdefnms[i] = NULL;
				}
				char *p0 = _pshdrdata[sec];
				char *pe = _pshdrdata[sec]+_pshdrs[sec].sh_size;
				Elf32_Verdef *pvd = (Elf32_Verdef *)p0;
				Elf32_Verdaux *pva = 
					(Elf32_Verdaux *)(p0+pvd->vd_aux);
				_pverdefnms[0] = 
					new char [strlen("VER_NDX_LOCAL")+1];
				_pverdefnms[0][0] = '\0';
				strcpy(_pverdefnms[0], "VER_NDX_LOCAL");
				for (int cverno=1; cverno <= maxverno; cverno++)
				{
                        		char *pnm = _pshdrdata[
						_pshdrs[sec].sh_link] +
						pva->vda_name;
					_pverdefnms[pvd->vd_ndx] = 
						new char [strlen(pnm)+1];
					MustBeTrue(_pverdefnms[pvd->vd_ndx] 
							!= NULL);
					_pverdefnms[pvd->vd_ndx][0] = '\0';
					strcpy(_pverdefnms[pvd->vd_ndx], pnm);
					char *p = (char *)pvd;
					pva = (Elf32_Verdaux *)(p+pvd->vd_aux);
					pvd = (Elf32_Verdef *)(p+pvd->vd_next);
				}
				break;
			}
		}
		return;
	}
	void writeverdef(int s) {
		// file must be open to write
		openrdwr();

		// write data 
		MustBeTrue(::lseek(_efd, _pshdrs[s].sh_offset,
				SEEK_SET) != NOTOK);
		int numbytes = _pshdrs[s].sh_size;
		MustBeTrue(::write(_efd, _pshdrdata[s], numbytes) == numbytes);
		return;
	}
	void writeverdaux(int s) {
		// file must be open to write
		openrdwr();

		// write data
		MustBeTrue(lseek(_efd, _pshdrs[s].sh_offset, 
				SEEK_SET) != NOTOK)
		int numbytes = _pshdrs[s].sh_size;
		MustBeTrue(write(_efd, _pshdrdata[s], 
				numbytes) == numbytes);
		int strtblidx = _pshdrs[s].sh_link;
		MustBeTrue(lseek(_efd, _pshdrs[strtblidx].sh_offset,
				SEEK_SET) != NOTOK)
		numbytes = _pshdrs[strtblidx].sh_size;
		MustBeTrue(write(_efd, _pshdrdata[strtblidx], 
				numbytes) == numbytes);
		return;
	}
	void writeverneed(int s) {
		// file must be open to write
		openrdwr();

		// write data
		MustBeTrue(lseek(_efd, _pshdrs[s].sh_offset, 
				SEEK_SET) != NOTOK)
		int numbytes = _pshdrs[s].sh_size;
		MustBeTrue(write(_efd, _pshdrdata[s], 
				numbytes) == numbytes);
		int strtblidx = _pshdrs[s].sh_link;
		MustBeTrue(lseek(_efd, _pshdrs[strtblidx].sh_offset,
				SEEK_SET) != NOTOK)
		numbytes = _pshdrs[strtblidx].sh_size;
		MustBeTrue(write(_efd, _pshdrdata[strtblidx], 
				numbytes) == numbytes);
		return;
	}
	void writevernaux(int s) {
		// file must be open to write
		openrdwr();

		// write data
		MustBeTrue(lseek(_efd, _pshdrs[s].sh_offset, 
				SEEK_SET) != NOTOK)
		int numbytes = _pshdrs[s].sh_size;
		MustBeTrue(write(_efd, _pshdrdata[s], 
				numbytes) == numbytes);
		int strtblidx = _pshdrs[s].sh_link;
		MustBeTrue(lseek(_efd, _pshdrs[strtblidx].sh_offset,
				SEEK_SET) != NOTOK)
		numbytes = _pshdrs[strtblidx].sh_size;
		MustBeTrue(write(_efd, _pshdrdata[strtblidx], 
				numbytes) == numbytes);
		return;
	}
	void writeversym(int s) {
		// file must be open to write
		openrdwr();

		// write data 
		MustBeTrue(::lseek(_efd, _pshdrs[s].sh_offset,
				SEEK_SET) != NOTOK);
		int numbytes = _pshdrs[s].sh_size;
		MustBeTrue(::write(_efd, _pshdrdata[s], numbytes) == numbytes);
		return;
	}

	// read/write dynamic tables
	void readdynamic() {
		// sanity checks
		MustBeTrue(hasstrings());

		// read in dynamic tables
		for (int s=0; s<_pehdr->e_shnum; s++)
		{
			// skip sections that are not dynamic tables
			if (_pshdrs[s].sh_type != SHT_DYNAMIC)
				continue;

			// allocate a dynamic table
			if (_pshdrdata[s] == NULL)
			{
				_pshdrdata[s] = 
					new char [_pshdrs[s].sh_size];
				MustBeTrue(_pshdrdata[s] != NULL);
			}

			// seek and read in dynamic table
			MustBeTrue(::lseek(_efd, _pshdrs[s].sh_offset, 
					SEEK_SET) != NOTOK)
			int numbytes = _pshdrs[s].sh_size;
			MustBeTrue(::read(_efd, _pshdrdata[s], 
					numbytes) == numbytes);

			// enable flag
			_hasdynamic = 1;
		}
		return;
	}
	void writedynamic(int s) {
		// file must be open to write
		openrdwr();

		// sanity check
		MustBeTrue(_pshdrs[s].sh_type == SHT_DYNAMIC);

		// write relocation table entry
		MustBeTrue(::lseek(_efd, _pshdrs[s].sh_offset,
				SEEK_SET) != NOTOK);
		int numbytes = _pshdrs[s].sh_size;
		MustBeTrue(::write(_efd, _pshdrdata[s], numbytes) == numbytes);
		return;
	}

	// read/write stabs entries
	void readstabs() {
		// sanity checks
		MustBeTrue(hasstrings());

		// read in stabs table raw data
		for (int s=0; s<_pehdr->e_shnum; s++)
		{
			// skip sections that are not stabs tables
			if (!isStabSection(s))
				continue;

			// allocate a stabs table
			if (_pshdrdata[s] == NULL)
			{
				_pshdrdata[s] = 
					new char [_pshdrs[s].sh_size];
				MustBeTrue(_pshdrdata[s] != NULL);
			}

			// seek and read in stabs table
			MustBeTrue(::lseek(_efd, _pshdrs[s].sh_offset, 
					SEEK_SET) != NOTOK)
			int numbytes = _pshdrs[s].sh_size;
			MustBeTrue(::read(_efd, _pshdrdata[s], 
					numbytes) == numbytes);

			// enable flag
			_hasstabs = 1;
		}

		// calculate the offsets into string table per file
		if (_poffsets == NULL)
		{
			_poffsets = new offsets * [_pehdr->e_shnum];
			MustBeTrue(_poffsets != NULL);
			for (int s=0; s<_pehdr->e_shnum; s++)
			{
				_poffsets[s] = NULL;
			}
		}
#if EXPTOOLS==1
		for (s=0; s<_pehdr->e_shnum; s++)
#else
		for (int s=0; s<_pehdr->e_shnum; s++)
#endif
		{
			long total_offset = 0;
			long old_offset = 0;

			// skip sections that are not stab tables
			if (!isStabSection(s))
				continue;

			// process entries in each stab section
			int nentries = 0;
			char *p0 = _pshdrdata[s];
			char *pe = _pshdrdata[s]+_pshdrs[s].sh_size;

			// count the number of entries in the section
			for (int stabno=0; 
			    (p0+stabno*_pshdrs[s].sh_entsize)<pe;
			     stabno++)
			{
				stab *pstab = (stab *)(p0+
					stabno*_pshdrs[s].sh_entsize);
				if (pstab->n_type == N_UNDF) nentries++;
			}

			// allocate an array to store data
			_poffsets[s] = (new offsets [nentries+1]);

			// kludge - store number of elements in array itself.
			_poffsets[s][0].start = nentries;	// end stabno
			_poffsets[s][0].end = nentries;		// end stabno
			_poffsets[s][0].offset = nentries;	// total offset

			// calculate offsets and store
			long ientry = 0;
#if EXPTOOLS==1
			for (stabno=0; 
			    (p0+stabno*_pshdrs[s].sh_entsize)<pe;
			     stabno++)
#else
			for (int stabno=0; 
			    (p0+stabno*_pshdrs[s].sh_entsize)<pe;
			     stabno++)
#endif
			{
				stab *pstab = (stab *)(p0+
					stabno*_pshdrs[s].sh_entsize);
				if (pstab->n_type == N_ILDPAD)
				{
					// padding, add correction to 
					// string offset.
					total_offset += pstab->n_value;
					continue;
				}
				else if (pstab->n_type != N_UNDF)
					continue;
				ientry++;
				total_offset += old_offset;
#if 0
printf("N_UNDF DATA - sec: %ld, stabno: %ld, \nnentries: %ld, size: %ld, total_offset: %ld\n", 
(long)s, (long)stabno, (long)pstab->n_desc, (long)pstab->n_value, total_offset);
#endif
				old_offset = pstab->n_value;
				_poffsets[s][ientry].start = stabno;
				_poffsets[s][ientry].end = stabno+pstab->n_desc;
				_poffsets[s][ientry].offset = total_offset;
			}
		}
#if 0
for (int s=0; s<pehdr->e_shnum; s++)
{
	if (_poffsets[s] == NULL)
		continue;
	int maxentry = _poffsets[s][0].end;
	for (int ientry=1; ientry<=maxentry; ientry++)
	{
		printf("(sec,ientry,start,end,offset)=(%ld,%ld,%ld,%ld,% ld)\n",
			(long)s, (long)ientry,
			(long) _poffsets[s][ientry].start,
			(long) _poffsets[s][ientry].end,
			(long) _poffsets[s][ientry].offset);
	}
}
#endif
		// check for strange case wher sh_link field, pointer to string
		// table section, is null. why this happens, I don't know. I 
		// saw this with LSS products. 
#if EXPTOOLS==1
		for (s=0; s<_pehdr->e_shnum; s++)
#else
		for (int s=0; s<_pehdr->e_shnum; s++)
#endif
		{
			// skip sections that are not stab tables
			if (!isStabSection(s))
				continue;

			// check if the link field is NULL
			if (_pshdrs[s].sh_link > 0)
				continue;
			char *psecname = pshstrtbl()+_pshdrs[s].sh_name;
			printf("WARNING: invalid sh_link field (%d) for section %d (%s).\n",
				_pshdrs[s].sh_link, s, psecname);

			// try to find string table section
			char strsecname[BUFSIZ];
			strcpy(strsecname, psecname);
			strcat(strsecname, "str");
			for (int strsec=0; strsec<_pehdr->e_shnum; strsec++)
			{
				char *pstrsecname = 
					pshstrtbl()+_pshdrs[strsec].sh_name;
				if (strcmp(strsecname, pstrsecname) == 0)
				{
					_pshdrs[s].sh_link = strsec;
					printf("FOUND sh_link section (%d, %s) for section (%d, %s).\n",
						strsec, pstrsecname, s, psecname);
					break;
				}
			}
		}
		return;
	}
	void writestabs(int s) {
		// file must be open to write
		openrdwr();

		// write stabs data
		MustBeTrue(::lseek(_efd, _pshdrs[s].sh_offset,
				SEEK_SET) != NOTOK);
		int numbytes = _pshdrs[s].sh_size;
		MustBeTrue(::write(_efd, _pshdrdata[s], numbytes) == numbytes);
		return;
	}

	// access functions
	inline const char *fname() const {
		return(_fname);
	}
	inline int filemode() const {
		return(_lastoper);
	}
	inline int efd() const {
		return(_efd);
	}
	inline int isrdonly() const {
		return(_lastoper == O_RDONLY);
	}
	inline int iswronly() const {
		return(_lastoper == O_WRONLY);
	}
	inline int isrdwr() const {
		return(_lastoper == O_RDWR);
	}
	inline int isnotok() const {
		return(_lastoper == NOTOK);
	}
	inline int isunknown() const {
		return(!(isrdonly()||iswronly()||isrdwr()||isnotok()));
	}
	inline Elf32_Ehdr ehdr() const {
		MustBeTrue(hasehdr());
		return(*_pehdr);
	}
	inline Elf32_Shdr shdr(int sec) const {
		MustBeTrue(hasehdr());
		MustBeTrue(0 <= sec && sec < _pehdr->e_shnum);
		MustBeTrue(hasshdrs());
		return(_pshdrs[sec]);
	}
	inline Elf32_Phdr phdr(int sec) const {
		MustBeTrue(hasehdr());
		MustBeTrue(0 <= sec && sec < _pehdr->e_shnum);
		MustBeTrue(hasphdrs());
		return(_pphdrs[sec]);
	}
	inline Elf32_Ehdr *pehdr() const {
		return(_pehdr);
	}
	inline Elf32_Shdr &pshdr(int sec) const {
		MustBeTrue(hasehdr());
		MustBeTrue(hasshdrs());
		MustBeTrue(0 <= sec && sec < _pehdr->e_shnum);
		return(_pshdrs[sec]);
	}
	inline Elf32_Shdr *pshdrs() const {
		return(_pshdrs);
	}
	inline Elf32_Phdr &pphdr(int sec) const {
		MustBeTrue(hasehdr());
		MustBeTrue(hasphdrs());
		MustBeTrue(0 <= sec && sec < _pehdr->e_shnum);
		return(_pphdrs[sec]);
	}
	inline Elf32_Phdr *pphdrs() const {
		return(_pphdrs);
	}
	inline char *pshdrdata(int sec) const {
		MustBeTrue(hasehdr());
		MustBeTrue(0 <= sec && sec < _pehdr->e_shnum);
		MustBeTrue(hasshdrs());
		return(_pshdrdata[sec]);
	}
	inline Elf32_Shdr *pshstrtblh() const {
		MustBeTrue(hasehdr());
		MustBeTrue(hasshdrs());
		return(_pshdrs+_pehdr->e_shstrndx);
	}
	inline char *pshstrtbl() const {
		MustBeTrue(hasehdr());
		MustBeTrue(hasshdrs());
		return(_pshdrdata[_pehdr->e_shstrndx]);
	}
	inline Elf32_Word *phashtbls(int sec) const {
		MustBeTrue(hasehdr());
		MustBeTrue(0 <= sec && sec < _pehdr->e_shnum);
		MustBeTrue(hasshdrs());
		MustBeTrue(_pshdrs[sec].sh_type == SHT_HASH);
		return((Elf32_Word *)_pshdrdata[sec]);
	}
	inline char *shdrnm(int sec) const {
		MustBeTrue(hasehdr());
		MustBeTrue(0 <= sec && sec < _pehdr->e_shnum);
		MustBeTrue(hasshdrs());
		return(_pshdrdata[_pehdr->e_shstrndx]+_pshdrs[sec].sh_name);
	}
	inline char **verdefnms() const {
		return(_pverdefnms);
	}
	inline char *verdefnms(Elf32_Versym vs) const {
		MustBeTrue(_pverdefnms != NULL);
		return(_pverdefnms[vs]);
	}

	// does file contain this type of data
	inline int hasrelocs() const {
		return(_hasrelocs);
	}
	inline int hassymbols() const {
		return(_hassymbols);
	}
	inline int hasstabs() const {
		return(_hasstabs);
	}
	inline int hasphdrs() const {
		return(_hasphdrs);
	}
	inline int hasehdr() const {
		return(_hasehdr);
	}
	inline int hasshdrs() const {
		return(_hasshdrs);
	}
	inline int hashash() const {
		return(_hashash);
	}
	inline int hasstrings() const {
		return(_hasstrings);
	}
	inline int hasversions() const {
		return(_hasverdef||_hasverneed||_hasversym);
	}
	inline int hasverdef() const {
		return(_hasverdef);
	}
	inline int hasverneed() const {
		return(_hasverneed);
	}
	inline int hasversym() const {
		return(_hasversym);
	}
	inline int isSwabable() const {
		MustBeTrue(_swabdata == 0 || _swabdata == 1);
		return(_swabdata);
	}

	// utility functions
	unsigned long
	eoswabit(unsigned char *p, int szofp) {
		unsigned long nval = 0;

		if (!isSwabable())
		{
			switch (szofp)
			{
			case 1:
				nval = *(unsigned char *)p;
				break;
			case 2:
				nval = *(unsigned short *)p;
				break;
			case 4:
				nval = *(unsigned long *)p;
				break;
			default:
				MustBeTrue(szofp == 1 || szofp == 2 || szofp == 4);
			}
		}
		else
		{
			switch (szofp)
			{
			case 1:
				nval = p[0];
				break;
			case 2:
				nval = p[0] | (p[1] << 8);
				break;
			case 4:
				nval = p[0] | (p[1]<<8) | (p[2]<<16) | (p[3]<<24);
				break;
			default:
				MustBeTrue(szofp == 1 || szofp == 2 || szofp == 4);
			}
		}
		return(nval);
	}
	unsigned long
	swabit(unsigned char *p, int szofp) {
		unsigned long nval = 0;
		
		switch (szofp)
		{
		case 1:
			nval = p[0];
			break;
		case 2:
			nval = p[0] | (p[1] << 8);
			break;
		case 4:
			nval = p[0] | (p[1]<<8) | (p[2]<<16) | (p[3]<<24);
			break;
		default:
			MustBeTrue(szofp == 1 || szofp == 2 || szofp == 4);
		}
		return(nval);
	}

	inline int isStabSection(int s) {
		char *psecname = pshstrtbl()+_pshdrs[s].sh_name;
		return(_pshdrs[s].sh_type == SHT_PROGBITS &&
			((strncmp(psecname, ".stab.", 6) == 0) ||
			(strncmp(psecname, ".stab", 5) == 0)));
	}
	inline long calcoffset(int csec, int cstabno) {
		// calculate the offset into the string table. 
		// this is necessary when more than one file contents 
		// are in one object or executable file.
		if ((showsec != csec) || 
		    (cstabno < showstart) || (cstabno > showend))
		{
			// need to recalculate the offset
			MustBeTrue(_poffsets[csec] != NULL);
			int maxe = _poffsets[csec][0].end;
			for (int ie=1; ie<=maxe; ie++)
			{
				if ((_poffsets[csec][ie].start <= cstabno) &&
				    (cstabno <= _poffsets[csec][ie].end))
				{
					showstart = _poffsets[csec][ie].start;
					showend = _poffsets[csec][ie].end;
					showoffset = _poffsets[csec][ie].offset;
					break;
				}
			}
		}
		return(showoffset);
	}

private:
	// not allowed
	ElfObject(const ElfObject &);
	ElfObject &operator=(const ElfObject &);

protected:
	// file data
	char *_fname;
	int _efd;
	int _lastoper;

	// ELF data 
	Elf32_Ehdr *_pehdr;
	Elf32_Shdr *_pshdrs;
	Elf32_Phdr *_pphdrs;

	// raw data 
	char **_pshdrdata;

	// auxiliary data
	char **_pverdefnms;
	offsets **_poffsets;
	int showsec;
	int showstart;
	int showend;
	int showoffset;

	// flags
	int _hasehdr;
	int _hasrelocs;
	int _hassymbols;
	int _hasstabs;
	int _hasphdrs;
	int _hasshdrs;
	int _hashash;
	int _hasstrings;
	int _hasverdef;
	int _hasverneed;
	int _hasversym;
	int _hasdynamic;
	int _swabdata;
};
#endif

