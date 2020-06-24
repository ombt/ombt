// open an elf file

// headers
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <libelf.h>
#include <demangle.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "data.h"

// externs
extern int optind;
extern char *optarg;

// globals
int showstrings = 0;
int showsymbols = 0;
int showsections = 0;
int showfileinfo = 0;
int showreloc = 0;
int demanglecpp = 0;
Elf *elf_fd = NULL;

// get data for a section
Elf_Void *
getsecdata(Elf32_Half strndx)
{
	Elf_Scn *scn = elf_getscn(elf_fd, strndx);
	if (scn == NULL) 
		return(NULL);
	Elf_Data *data = elf_getdata(scn, NULL);
	if (data == NULL || data->d_buf == NULL)
		return(NULL);
	else
		return(data->d_buf);
}

// get section hdr for an index
Elf32_Shdr *
getsechdr(Elf32_Half secndx)
{
	Elf_Scn *scn = elf_getscn(elf_fd, secndx);
	if (scn == NULL) 
		return(NULL);
	return(elf32_getshdr(scn));
}

// convert types to a string
const char *
s_etype(Elf32_Half e_type)
{
	switch (e_type)
	{
	case 0:
		return("no file type");
	case 1:
		return("relocatable file");
	case 2:
		return("executable file");
	case 3:
		return("shared object file");
	case 4:
		return("core file");
	case 0xff00:
		return("processor-specific");
	case 0xffff:
		return("processor-specific");
	default:
		return("unknown");
	}
}

const char *
s_emachine(Elf32_Half e_machine)
{
	for (int m=0; E_Machine[m].key != -1; m++)
	{
		if (E_Machine[m].key == e_machine)
			return(E_Machine[m].data);
	}
	return("unknown");
}

const char *
s_ephoff(Elf32_Off e_phoff)
{
	if (e_phoff == 0)
		return("no program header");
	else
		return("contains program header");
}

const char *
s_ephnum(Elf32_Half e_phnum)
{
	if (e_phnum == 0)
		return("no program header table");
	else
		return("contains program header table");
}

const char *
s_eentry(Elf32_Addr e_entry)
{
	if (e_entry == (Elf32_Addr) 0)
		return("no entry point");
	else
		return("entry point");
}

const char *
s_eshstrndx(Elf32_Half e_shstrndx)
{
	if (e_shstrndx == SHN_UNDEF)
		return("no section string table");
	else
		return("contains section string table");
}

const char *
s_eversion(Elf32_Word e_version)
{
	if (e_version == EV_NONE)
		return("invalid version");
	else if (e_version >= EV_CURRENT)
		return("current version");
	else
		return("invalid version");
}

const char *
s_ident(unsigned char *e_ident)
{
	static char buf[BUFSIZ];
	if (strncmp((char *)e_ident, ELFMAG, strlen(ELFMAG)) == 0)
		strcpy(buf, "(elf,");
	else
		strcpy(buf, "(not elf,");
	switch (e_ident[EI_CLASS])
	{
	case ELFCLASS32:
		strcat(buf, "32-bit objs,");
		break;
	case ELFCLASS64:
		strcat(buf, "64-bit objs,");
		break;
	default:
		strcat(buf, "invalid class objs,");
		break;
	}
	switch (e_ident[EI_DATA])
	{
	case ELFDATANONE:
		strcat(buf, "invalid data encoding,");
		break;
	case ELFDATA2LSB:
		strcat(buf, "LSB at low addr,");
		break;
	case ELFDATA2MSB:
		strcat(buf, "MSB at low addr,");
		break;
	default:
		strcat(buf, "reserved data encoding,");
		break;
	}
	if (e_ident[EI_VERSION] == EV_NONE)
		strcat(buf, "invalid version)");
	else if (e_ident[EI_VERSION] >= EV_CURRENT)
		strcat(buf, "current version)");
	else
		strcat(buf, "invalid version)");
	return(buf);
}

// print elf header data
void
printehdr(char *file, Elf32_Ehdr *ehdr)
{
	if (!showfileinfo)
		return;
	printf("\n");
	printf("ELF file        : %s\n", file);
	printf("ident           : %s\n", 
		s_ident(ehdr->e_ident));
	printf("file type       : %08lx (%s)\n", 
		ehdr->e_type, s_etype(ehdr->e_type));
	printf("target machine  : %08lx (%s)\n", 
		ehdr->e_machine, s_emachine(ehdr->e_machine));
	printf("file version    : %08lx (%s)\n", 
		ehdr->e_version, s_eversion(ehdr->e_version));
	printf("start address   : %08lx (%s)\n", 
		ehdr->e_entry, s_eentry(ehdr->e_entry));
	printf("phdr file offset: %08lx (%s)\n", 
		ehdr->e_phoff, s_ephoff(ehdr->e_phoff));
	printf("shdr file offset: %08lx\n", 
		ehdr->e_shoff);
	printf("file flags      : %08lx\n", 
		ehdr->e_flags);
	printf("size of ehdr    : %08lx\n", 
		ehdr->e_ehsize);
	printf("size of phdr    : %08lx\n", 
		ehdr->e_phentsize);
	printf("number of phdrs : %08lx (%s)\n", 
		ehdr->e_phnum, s_ephnum(ehdr->e_phnum));
	printf("sizeof shdr     : %08lx\n", 
		ehdr->e_shentsize);
	printf("number of shdrs : %08lx\n", 
		ehdr->e_shnum);
	printf("shdr string index: %08lx (%s)\n", 
		ehdr->e_shstrndx, 
		s_eshstrndx(ehdr->e_shstrndx));
	return;
}

// print kind of file
void
printekind(char *file, Elf_Kind ekind)
{
	if (!showfileinfo)
		return;
	printf("\n");
	printf("ELF file    : %s\n", file);
	switch (ekind) 
	{
	case ELF_K_AR:
		printf("kind of file: archive\n");
		break;
	case ELF_K_COFF:
		printf("kind of file: coff\n");
		break;
	case ELF_K_ELF:
		printf("kind of file: elf\n");
		break;
	case ELF_K_NONE:
	default:
		printf("kind of file: unknown\n");
		break;
	}
	return;
}

// print data hdr
void
printdatahdr(const char *label, Elf_Data *data)
{
	if (!showsections)
		return;
	printf("\n%s\n", label);
	printf("d_buf    : 0x%0lx\n", data->d_buf);
	printf("d_type   : %d\n", data->d_type);
	printf("d_size   : %d\n", data->d_size);
	printf("d_off    : %d\n", data->d_off);
	printf("d_align  : %d\n", data->d_align);
	printf("d_version: %d\n", data->d_version);
	return;
}

// print all the strings in the string table
void
printstrtbl(const char *secnm, Elf_Data *data)
{
	if (!showstrings)
		return;
	printf("\nsection %s string table contents\n", secnm);
	char *p0 = (char *)data->d_buf;
	char *p = (char *)data->d_buf;
	char *pmax = p + data->d_size;
	int nullstrcnt = 0;
	for (int i=1; p < pmax; i++, p += strlen(p)+1)
	{
		int strsz = strlen(p);
		int spnsz = strspn(p, " \t\n");
		if (strsz == 0 || strsz == spnsz)
		{
			nullstrcnt += 1;
			continue;
		}
		if (nullstrcnt > 0)
		{
			printf("%d, %d: %d NULL/WHITE-SPACE-ONLY strings\n", 
				i-1, long(p-p0-1), nullstrcnt);
			nullstrcnt = 0;
		}
		printf("%d, %d: %s\n", i, long(p-p0), p);
	}
	return;
}

// section header type
const char *
s_shtype(Elf32_Word sh_type)
{
	for (int m=0; Sh_Type[m].key != -1; m++)
	{
		if (Sh_Type[m].key == sh_type)
			return(Sh_Type[m].data);
	}
	return("unknown section");
}

const char *
s_shname(const char *s)
{
	for (int m=0; Sh_Name[m].key != NULL; m++)
	{
		if (strcmp(Sh_Name[m].key,s) == 0)
			return(Sh_Name[m].data);
	}
	if (strncmp(s, ".rel", 4) == 0)
	{
		static char buf[BUFSIZ];
		sprintf(buf, "relocation information for section %s", s+5);
		return(buf);
	}
	else if (strncmp(s, ".rela", 5) == 0)
	{
		static char buf[BUFSIZ];
		sprintf(buf, "relocation information for section %s", s+6);
		return(buf);
	}
	else
		return("none");
}

// print section header flags
const char *
s_shflags(Elf32_Word sh_flags)
{
	static char buf[BUFSIZ];
	buf[0] = '\0';
	for (int m=0; Sh_Flags[m].key != 0; m++)
	{
		if ((Sh_Flags[m].key & sh_flags) != 0)
			strcat(buf, Sh_Flags[m].data);
	}
	if (sh_flags == 0 || buf[0] == '\0')
		strcat(buf, "none");
	return(buf);
}

// get section name
void
getsecnm(char *secnm, Elf_Data *stbl, Elf32_Shdr *shdr)
{
	MustBeTrue(secnm != NULL);
	MustBeTrue(stbl != NULL);
	MustBeTrue(shdr != NULL);
	strcpy(secnm, (char *)stbl->d_buf+shdr->sh_name);
	
}

// print section header
void
printsechdr(int cnt, Elf_Data *stbl, Elf32_Shdr *shdr)
{
	if (!showsections)
		return;
	printf("\nsection %d header\n", cnt);
	printf("name     : %s (%s, %d)\n", 
		(char *)stbl->d_buf+shdr->sh_name, 
		s_shname((char *)stbl->d_buf+shdr->sh_name),
		shdr->sh_name);
	printf("type     : 0x%lx (%s)\n", 
		shdr->sh_type, 
		s_shtype(shdr->sh_type));
	printf("flags    : 0x%lx (%s)\n", 
		shdr->sh_flags,
		s_shflags(shdr->sh_flags));
	printf("addr     : 0x%lx\n", shdr->sh_addr);
	printf("offset   : %d\n", shdr->sh_offset);
	printf("size     : %d\n", shdr->sh_size);
	switch (shdr->sh_type)
	{
	case SHT_DYNAMIC:
	case SHT_HASH:
		printf("shdr index for associated string table: %d\n", 
			shdr->sh_link);
		printf("info     : %d\n", shdr->sh_info);
		break;
	case SHT_REL:
	case SHT_RELA:
		printf("shdr index for associated string table: %d\n", 
			shdr->sh_link);
		printf("shdr index to section where relocation applies: %d\n", 
			shdr->sh_info);
		break;
	case SHT_SYMTAB:
	case SHT_DYNSYM:
		printf("shdr index for associated string table: %d\n", 
			shdr->sh_link);
		printf("one more than the symbol table index of the last local symbol: %d\n", 
			shdr->sh_info);
		break;
	case SHT_SUNW_verdef:
		printf("shdr index for associated string table: %d\n", 
			shdr->sh_link);
		printf("number of version definitions in section: %d\n", 
			shdr->sh_info);
		break;
	case SHT_SUNW_verneed:
		printf("shdr index for associated string table: %d\n", 
			shdr->sh_link);
		printf("number of version dependencies in section: %d\n", 
			shdr->sh_info);
		break;
	case SHT_SUNW_versym:
		printf("shdr index for associated string table: %d\n", 
			shdr->sh_link);
		printf("info     : %d\n", shdr->sh_info);
		break;
	default:
		printf("link     : %d\n", shdr->sh_link);
		printf("info     : %d\n", shdr->sh_info);
		break;
	}
	printf("addralign: 0x%lx\n", shdr->sh_addralign);
	printf("entsize  : 0x%lx\n", shdr->sh_entsize);
	return;
}

// print section string table
void
printsecstrtbl(Elf32_Shdr *shdr, Elf_Scn *scn, const char *secnm)
{
	// do we print strings?
	if (!showstrings)
		return;

	// check if section is a string table
	if (shdr->sh_type != SHT_STRTAB)
		return;

	// get string table for this section
	Elf_Data *stringtbl;
	if ((stringtbl = elf_getdata(scn, NULL)) == NULL)
	{
		if ((errno = elf_errno()) != 0)
		{
			const char *emsg = elf_errmsg(errno);
			ERRORD("elf_getdata failed.", emsg, errno);
		}
		else
		{
			ERROR("elf_getdata failed.", errno);
		}
		return;
	}

	// print strings
	printstrtbl(secnm, stringtbl);
	return;
}

// convert to strings
const char *
bindtostring(int bind)
{
	if (bind == STB_LOCAL) 
		return("LOCAL");
	else if (bind == STB_GLOBAL) 
		return("GLOBAL");
	else if (bind == STB_WEAK) 
		return("WEAK");
	else if (bind == STB_NUM) 
		return("NUM");
	else if (STB_LOPROC <= bind && bind <= STB_HIPROC)
	{
		return("PROC-SPEC-BINDING");
	}
	else
		return("UNKNOWN-BINDING");
}

const char *
typetostring(int type)
{
	if (type == STT_NOTYPE) 
		return("NOTYPE");
	else if (type == STT_OBJECT) 
		return("DATA-OBJECT");
	else if (type == STT_FUNC) 
		return("FUNCTION");
	else if (type == STT_SECTION) 
		return("SECTION");
	else if (type == STT_NUM) 
		return("NUM");
	else if (type == STT_FILE) 
		return("FILE");
	else if (STT_LOPROC <= type && type <= STT_HIPROC)
	{
		return("PROC-SPEC-TYPES");
	}
	else
		return("NO-TYPE");
}

const char *
shndxtostring(int shndx)
{
	if (shndx == SHN_ABS) 
		return("ABSOLUTE");
	else if (shndx == SHN_COMMON) 
		return("COMMON");
	else if (shndx == SHN_UNDEF) 
		return("UNDEFINED");
	else
		return("RELOCATION-SECTION");
}

// print contents of a symbol table
void
printsymtbl(const char *label, Elf32_Shdr *shdr, Elf_Data *data)
{
	if (!showsymbols)
		return;
	printf("\nsection %s symbol table\n", label);
	char *p0 = (char *)data->d_buf;
	char *p = (char *)data->d_buf;
	char *pmax = p + data->d_size;
	int nullstrcnt = 0;
	const char *strtbl = (const char *)getsecdata(shdr->sh_link);
	for (int i=1; p < pmax; i++, p += shdr->sh_entsize)
	{
		const char *pnm = NULL;
		char buf[BUFSIZ];
		Elf32_Sym *psym = (Elf32_Sym *)p;
		if (strtbl == NULL || psym->st_name == 0)
			pnm = "none";
		else
		{
			pnm = strtbl + psym->st_name;
			if (demanglecpp && 
			   (cplus_demangle(pnm, buf, BUFSIZ-1) == 0))
			{
				pnm = buf;
			}
		}
		printf("\n%d: nm: %d,%s\n", i, psym->st_name, pnm);
		printf("%d: val: 0x%x, sz: %d, bind: %d,%s, type: %d,%s, othr: %d, shndx: 0x%x,%s\n",
		i, psym->st_value, psym->st_size, 
		ELF32_ST_BIND(psym->st_info),
		bindtostring(ELF32_ST_BIND(psym->st_info)),
		ELF32_ST_TYPE(psym->st_info),
		typetostring(ELF32_ST_TYPE(psym->st_info)),
		psym->st_other, 
		psym->st_shndx,
		shndxtostring(psym->st_shndx));
	}
	return;
}

// print section symbol table
void
printsecsymtbl(Elf32_Shdr *shdr, Elf_Scn *scn, const char *secnm)
{
	// do we print symbols
	if (!showsymbols)
		return;

	// check if section is a symbol table
	if (shdr->sh_type != SHT_SYMTAB && shdr->sh_type != SHT_DYNSYM)
		return;

	// get symbol table for this section
	Elf_Data *symtbl;
	if ((symtbl = elf_getdata(scn, NULL)) == NULL)
	{
		if ((errno = elf_errno()) != 0)
		{
			const char *emsg = elf_errmsg(errno);
			ERRORD("elf_getdata failed.", emsg, errno);
		}
		else
		{
			ERROR("elf_getdata failed.", errno);
		}
		return;
	}

	// print strings
	printsymtbl(secnm, shdr, symtbl);
	return;
}

// get name of symbol
const char *
getsymnm(Elf32_Sym *symtbl, const char *strtbl, int ndx)
{
	symtbl += ndx;
	if (symtbl->st_name == 0)
		return("none");
	else
		return(strtbl+symtbl->st_name);
}

// print section relocation table
void
printreloc(const char *label, Elf32_Shdr *shdr, Elf_Data *data)
{
	if (!showreloc)
		return;
	printf("\nsection %s relocation table\n", label);
	char *p0 = (char *)data->d_buf;
	char *p = (char *)data->d_buf;
	char *pmax = p + data->d_size;
	Elf32_Shdr *symhdr = getsechdr(shdr->sh_link);
	Elf32_Sym *symtbl= (Elf32_Sym *)getsecdata(shdr->sh_link);
	const char *strtbl = (const char *)getsecdata(symhdr->sh_link);
	for (int i=1; p < pmax; i++, p += shdr->sh_entsize)
	{
		if (shdr->sh_type == SHT_REL)
		{
			Elf32_Rel *prel = (Elf32_Rel *)p;
			printf("\n%d: offset: 0x%x, sym: 0x%x,%s, type: 0x%x\n", 
				i, prel->r_offset, 
				getsymnm(symtbl, strtbl, 
					ELF32_R_SYM(prel->r_info)),
				ELF32_R_SYM(prel->r_info),
				ELF32_R_TYPE(prel->r_info));
		}
		else if (shdr->sh_type == SHT_RELA)
		{
			Elf32_Rela *prela = (Elf32_Rela *)p;
			printf("\n%d: offset: 0x%x, sym: 0x%x,%s, type: 0x%x, add: 0x%x\n", 
				i, prela->r_offset, 
				getsymnm(symtbl, strtbl, 
					ELF32_R_SYM(prela->r_info)),
				ELF32_R_SYM(prela->r_info),
				ELF32_R_TYPE(prela->r_info),
				prela->r_addend);
		}
	}
	return;
}

void
printsecreloc(Elf32_Shdr *shdr, Elf_Scn *scn, const char *secnm)
{
	// do we print symbols
	if (!showreloc)
		return;

	// check if section is a symbol table
	if (shdr->sh_type != SHT_REL && shdr->sh_type != SHT_RELA)
		return;

	// get relocation table for this section
	Elf_Data *reloc;
	if ((reloc = elf_getdata(scn, NULL)) == NULL)
	{
		if ((errno = elf_errno()) != 0)
		{
			const char *emsg = elf_errmsg(errno);
			ERRORD("elf_getdata failed.", emsg, errno);
		}
		else
		{
			ERROR("elf_getdata failed.", errno);
		}
		return;
	}

	// print strings
	printreloc(secnm, shdr, reloc);
	return;
}

// usage message
void
usage(const char *cmd)
{
	printf("\nusage: %s [-?rdasytf] elf_file ...\n\n", cmd);
	printf("\t-? = usage msg\n");
	printf("\t-t = print strings\n");
	printf("\t-y = print symbols\n");
	printf("\t-s = print sections\n");
	printf("\t-f = print file info\n");
	printf("\t-r = print relocation info\n");
	printf("\t-a = print all info\n");
	printf("\t-d = demangle C++ names\n");
	return;
}

// main entry 
main(int argc, char **argv)
{
	// get cmd line options
	for (int c = 0; (c = getopt(argc, argv, "?rdsaytf")) != EOF; )
	{
		switch (c)
		{
		case 'd':
			demanglecpp = 1;
			break;

		case 'r':
			showreloc = 1;
			break;

		case 'f':
			showfileinfo = 1;
			break;

		case 't':
			showstrings = 1;
			break;

		case 'y':
			showsymbols = 1;
			break;

		case 's':
			showsections = 1;
			break;

		case 'a':
			showfileinfo = 1;
			showstrings = 1;
			showsymbols = 1;
			showsections = 1;
			showreloc = 1;
			break;

		case '?':
			usage(argv[0]);
			return(0);

		default:
			ERRORD("invalid command option.", c, EINVAL);
			return(2);
		}
	}

	// set the current elf version
	elf_version(EV_CURRENT);

	// cycle through any files
	for (int arg=optind; arg < argc; arg++)
	{
		// file to process
		printf("\nELF file: %s\n", argv[arg]);

		// open the elf file
		int fd = open(argv[arg], O_RDONLY);
		if (fd < 0)
		{
			ERRORD("open failed.", argv[arg], errno);
			return(2);
		}

		// reset elf errno
		(void)elf_errno();

		// assign an elf descriptor to the file
		elf_fd = elf_begin(fd, ELF_C_READ, (Elf *)0);
		if (elf_fd == NULL)
		{
			if ((errno = elf_errno()) != 0)
			{
				const char *emsg = elf_errmsg(errno);
				ERRORD("elf_begin failed.", emsg, errno);
			}
			else
			{
				ERRORD("elf_begin failed.", argv[arg], errno);
			}
			close(fd);
			continue;
		}

		// get type of file
		Elf_Kind ekind = elf_kind(elf_fd);

		// print type of file
		printekind(argv[arg], ekind);

		// get elf file header
		Elf32_Ehdr *ehdr = elf32_getehdr(elf_fd);
		if (ehdr == NULL)
		{
			if ((errno = elf_errno()) != 0)
			{
				const char *emsg = elf_errmsg(errno);
				ERRORD("elf32_getehdr failed.", emsg, errno);
			}
			else
			{
				ERRORD("elf32_getehdr failed.", argv[arg], errno);
			}
			elf_end(elf_fd);
			close(fd);
			continue;
		}

		// print ehdr data 
		printehdr(argv[arg], ehdr);

		// get string section table header
		Elf_Scn *scn;
		Elf_Data *stringtbl;
		if ((scn = elf_getscn(elf_fd, ehdr->e_shstrndx)) == NULL)
		{
			if ((errno = elf_errno()) != 0)
			{
				const char *emsg = elf_errmsg(errno);
				ERRORD("elf_getscn failed.", emsg, errno);
			}
			else
			{
				ERRORD("elf_getscn failed.", argv[arg], errno);
			}
			elf_end(elf_fd);
			close(fd);
			continue;
		}

		// get pointer to string table.
		if ((stringtbl = elf_getdata(scn, NULL)) == NULL)
		{
			if ((errno = elf_errno()) != 0)
			{
				const char *emsg = elf_errmsg(errno);
				ERRORD("elf_getdata failed.", emsg, errno);
			}
			else
			{
				ERRORD("elf_getdata failed.", argv[arg], errno);
			}
			elf_end(elf_fd);
			close(fd);
			continue;
		}
		printdatahdr("section headers string table header", stringtbl);
		printstrtbl("section headers string table", stringtbl);

		// dump section data
		if (showsections)
			printf("\nELF section headers\n");
		int cnt;
		for (cnt=1, scn=NULL; scn = elf_nextscn(elf_fd, scn); cnt++)
		{
			Elf32_Shdr *shdr;
			char secnm[BUFSIZ];
			if ((shdr = elf32_getshdr(scn)) == NULL)
			{
				if ((errno = elf_errno()) != 0)
				{
					const char *emsg = elf_errmsg(errno);
					ERRORD("elf32_getshdr failed.", 
						emsg, errno);
				}
				else
				{
					ERRORD("elf_getdata failed.", 
						argv[arg], errno);
				}
				elf_end(elf_fd);
				close(fd);
				break;
			}
			getsecnm(secnm, stringtbl, shdr);
			printsechdr(cnt, stringtbl, shdr);
			printsecstrtbl(shdr, scn, secnm);
			printsecsymtbl(shdr, scn, secnm);
			printsecreloc(shdr, scn, secnm);
		}

		// close the elf file
		(void)elf_end(elf_fd);

		// close file
		(void) close(fd);
	}

	// all done
	return(0);
}

