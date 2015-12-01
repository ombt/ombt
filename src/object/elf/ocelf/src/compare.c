// compare two elf object files

// headers
#include "ocelf.h"

// translation table
unsigned char num2ch[] = {
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	' ', '!', '"', '#', '$', '%', '&', '\'',
	'(', ')', '*', '+', ',', '-', '.', '/',
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', ':', ';', '<', '=', '>', '?',
	'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
	'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
	'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
	'X', 'Y', 'Z', '[', '\\', ']', '^', '_',
	'`', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
	'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
	'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
	'x', 'y', 'z', '{', '|', '}', '~', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
	'.', '.', '.', '.', '.', '.', '.', '.',
};

// compare two buffers, byte-per-byte. 
int
cmpbytes(long start, long nbytes1, long nbytes2,
	const unsigned char *ps1, const unsigned char *ps2)
{
	// sanity checks
	MustBeTrue(start >= 0 && nbytes1 > 0 && nbytes2 > 0);
	MustBeTrue(ps1 != NULL && ps2 != NULL);

	// default return code, no changes
	int status = IdenticalFiles;

	// determine ranges to print
	long nbytes = nbytes1;
	if (nbytes < nbytes2) nbytes = nbytes2;

	// compare byte-per-byte
	long printed = 0;
	for (long byte = 0; byte < nbytes; byte++)
	{
		if (ps1[byte] != ps2[byte])
		{
			// report difference, if NOT silent mode.
			if (!sflag)
			{
				if (aflag || printed < maxBytesToPrint)
				{
					if (tflag)
					{
						printf("%*ld: %2x %2x ( %c %c )\n", 
							offsetfldwidth,
							start+byte, 
						(unsigned int)ps1[byte],
						(unsigned int)ps2[byte],
						num2ch[(unsigned int)ps1[byte]],
						num2ch[(unsigned int)ps2[byte]]);
					}
					else
					{
						printf("%*ld: %2x %2x\n", 
							offsetfldwidth,
							start+byte, 
						(unsigned int)ps1[byte],
						(unsigned int)ps2[byte]);
					}
					printed++;
				}
				else if (printed == maxBytesToPrint)
				{
					printf("...\n");
					printed++;
				}
			}

			// exit on first difference?
			status = DifferentFiles;
			if (fflag)
			{
				// report if any difference were found
				return(status);
			}
		}
	}

	// check if files sizes are different. if so, report the 
	// differences.
	if (nbytes1 > nbytes2)
	{
		printf("Section sizes are not equal.\n");
		for (long byte = nbytes2; byte < nbytes1; byte++)
		{
			// report difference, if NOT silent mode.
			if (!sflag)
			{
				if (aflag || printed < maxBytesToPrint)
				{
					if (tflag)
					{
						printf("%*ld: %2x ??? ( %c ??? )\n", 
							offsetfldwidth,
							start+byte, 
						(unsigned int)ps1[byte],
						num2ch[(unsigned int)ps1[byte]]);
					}
					else
					{
						printf("%*ld: %2x ???\n", 
							offsetfldwidth,
							start+byte, 
						(unsigned int)ps1[byte]);
					}
					printed++;
				}
				else if (printed == maxBytesToPrint)
				{
					printf("...\n");
					printed++;
				}
			}

			// exit on first difference?
			status = DifferentFiles;
			if (fflag)
			{
				// report if any difference were found
				return(status);
			}
		}
	}
	else if (nbytes2 > nbytes1)
	{
		printf("Section sizes are not equal.\n");
		for (long byte = nbytes1; byte < nbytes2; byte++)
		{
			// report difference, if NOT silent mode.
			if (!sflag)
			{
				if (aflag || printed < maxBytesToPrint)
				{
					if (tflag)
					{
						printf("%*ld: ??? %2x ( ??? %c )\n", 
							offsetfldwidth,
							start+byte, 
						(unsigned int)ps2[byte],
						num2ch[(unsigned int)ps2[byte]]);
					}
					else
					{
						printf("%*ld: ??? %2x\n", 
							offsetfldwidth,
							start+byte, 
						(unsigned int)ps2[byte]);
					}
					printed++;
				}
				else if (printed == maxBytesToPrint)
				{
					printf("...\n");
					printed++;
				}
			}

			// exit on first difference?
			status = DifferentFiles;
			if (fflag)
			{
				// report if any difference were found
				return(status);
			}
		}
	}

	// report if any difference were found
	return(status);
}

// verify a file is an elf file
int
isElf(const Elf32_Ehdr *pehdr)
{
	return((pehdr->e_ident[EI_MAG0] == ELFMAG0) &&
	       (pehdr->e_ident[EI_MAG1] == ELFMAG1) &&
	       (pehdr->e_ident[EI_MAG2] == ELFMAG2) &&
	       (pehdr->e_ident[EI_MAG3] == ELFMAG3));
}

int
isElf32Bit(const Elf32_Ehdr *pehdr)
{
	return(pehdr->e_ident[EI_CLASS] == ELFCLASS32);
}

// sections to compare explicitly
int
isFilteredSection(const char *sec)
{
	return((strncmp(sec, INITSECTION, strlen(INITSECTION)) == 0) ||
		(strncmp(sec, TEXTSECTION, strlen(TEXTSECTION)) == 0) ||
		(strncmp(sec, FINISECTION, strlen(FINISECTION)) == 0) ||
		(strncmp(sec, RODATASECTION, strlen(RODATASECTION)) == 0) ||
		(strncmp(sec, DATASECTION, strlen(DATASECTION)) == 0) ||
		(Bflag && strncmp(sec, BSSSECTION, strlen(BSSSECTION)) == 0));
}

// compare fields
template <class DataType>
void
cmpfield(DataType fld1, DataType fld2, const char *fldname, 
		int errorstatus, int &status)
{
	if (fld1 != fld2)
	{
		if (!sflag)
		{
			printf("field %s is different", fldname);
			switch (errorstatus)
			{
			case StopProcessing:
				SetStatus(errorstatus, DifferentFiles);
				// fall thru
			case ErrorCondition:
				printf("; cannot continue.\n");
				break;
			case Ignore:
				printf("; ignoring it.\n");
				break;
			default:
				printf(".\n");
				break;
			}
		}
		else if (errorstatus == StopProcessing)
		{
			SetStatus(errorstatus, DifferentFiles);
		}
		if (errorstatus != Ignore)
		{
			ClearStatus(status, IdenticalFiles);
			SetStatus(status, errorstatus);
		}
	}
	return;
}

// compare file headers.
int
cmpehdrs(Elf32_Ehdr *pehdr1, Elf32_Ehdr *pehdr2)
{
	// sanity checks
	MustBeTrue(pehdr1 != NULL && pehdr2 != NULL);

	// print message
	if (!sflag) printf("comparing file headers:\n");

	// default status
	int status = IdenticalFiles;

	// compare field to field
	if (memcmp(pehdr1->e_ident, pehdr2->e_ident, EI_NIDENT) != 0)
	{
		if (!sflag) printf("e_ident are different; cannot continue\n");
		SetStatus(status, StopProcessing);
	}
	cmpfield(pehdr1->e_type, pehdr2->e_type, "e_ident", 
			StopProcessing, status);
	cmpfield(pehdr1->e_machine, pehdr2->e_machine, "e_machine", 
			StopProcessing, status);
	cmpfield(pehdr1->e_version, pehdr2->e_version, "e_version", 
			StopProcessing, status);
	cmpfield(pehdr1->e_entry, pehdr2->e_entry, "e_entry", 
			Ignore, status);
	cmpfield(pehdr1->e_phoff, pehdr2->e_phoff, "e_phoff", 
			Ignore, status);
	cmpfield(pehdr1->e_shoff, pehdr2->e_shoff, "e_shoff", 
			Ignore, status);
	cmpfield(pehdr1->e_flags, pehdr2->e_flags, "e_flags", 
			StopProcessing, status);
	cmpfield(pehdr1->e_ehsize, pehdr2->e_ehsize, "e_ehsize", 
			StopProcessing, status);
	cmpfield(pehdr1->e_phentsize, pehdr2->e_phentsize, "e_phentsize", 
			StopProcessing, status);
	cmpfield(pehdr1->e_phnum, pehdr2->e_phnum, "e_phnum", 
			StopProcessing, status);
	cmpfield(pehdr1->e_shentsize, pehdr2->e_shentsize, "e_shentsize", 
			StopProcessing, status);
	cmpfield(pehdr1->e_shnum, pehdr2->e_shnum, "e_shnum", 
			StopProcessing, status);
	cmpfield(pehdr1->e_shstrndx, pehdr2->e_shstrndx, "e_shstrndx", 
			StopProcessing, status);
	return(status);
}

// compare section headers
int
cmpshdr(Elf32_Ehdr *pehdr1, Elf32_Shdr *pshdr1, char *pstrtbl1,
	Elf32_Ehdr *pehdr2, Elf32_Shdr *pshdr2, char *pstrtbl2)
{
	// sanity check
	MustBeTrue((pehdr1 != NULL && pshdr1 != NULL) &&
		   (pehdr2 != NULL && pshdr2 != NULL));
	MustBeTrue(pstrtbl1 != NULL && pstrtbl2 != NULL);

	// print message
	if (!sflag) printf("comparing section headers:\n");

	// default return code, no changes
	int status = IdenticalFiles;

	// compare each section, for now.
	for (int sec = 0; sec < pehdr1->e_shnum; sec++)
	{
		// skip undefined sections
		if (pshdr1[sec].sh_type == SHT_NULL &&
		    pshdr2[sec].sh_type != SHT_NULL)
		{
			if (!sflag) printf("file1 sec %d is NULL, file2 sec %d is not NULL.\n", sec, sec);
			ClearStatus(status, IdenticalFiles);
			SetStatus(status, DifferentFiles);
			if (fflag)
				break;
			else 
				continue;
		}
		else if (pshdr1[sec].sh_type != SHT_NULL &&
		         pshdr2[sec].sh_type == SHT_NULL)
		{
			if (!sflag) printf("file2 sec %d is NULL, file1 sec %d is not NULL.\n", sec, sec);
			ClearStatus(status, IdenticalFiles);
			SetStatus(status, DifferentFiles);
			if (fflag)
				break;
			else
				continue;
		}
		else if (pshdr1[sec].sh_type == SHT_NULL &&
		         pshdr2[sec].sh_type == SHT_NULL)
		{
			// just ignore
			if (!sflag) printf("ignoring NULL section %d.\n", sec);
			continue;
		}

		// get section names
		char secname1[BUFSIZ], secname2[BUFSIZ];
		strcpy(secname1, pstrtbl1+pshdr1[sec].sh_name);
		strcpy(secname2, pstrtbl2+pshdr2[sec].sh_name);

		// print message
		if (!sflag) printf("comparing section %d header (%s, %s):\n", 
			sec, secname1, secname2);
		if (strcmp(secname1, secname2) != 0)
		{
			if (!sflag) printf("section names are not the same; cannot continue.\n");
			ClearStatus(status, IdenticalFiles);
			SetStatus(status, (DifferentFiles|StopProcessing));
			break;
		}

		// default status
		int secstatus = IdenticalFiles;

		// compare field to field. recovery depends on 
		// type of section.
		//
		Elf32_Shdr *ps1 = pshdr1+sec;
		Elf32_Shdr *ps2 = pshdr2+sec;
		if (Tflag)
		{
			if (ps1->sh_type == SHT_STRTAB && 
			    ps2->sh_type != SHT_STRTAB)
			{
				if (!sflag) printf("file1 sec %d is a string table; file2 sec %d is not.\n", sec, sec);
				ClearStatus(status, IdenticalFiles);
				SetStatus(status, (DifferentFiles|StopProcessing));
				break;
			}
			else if (ps1->sh_type != SHT_STRTAB && 
				 ps2->sh_type == SHT_STRTAB)
			{
				if (!sflag) printf("file2 sec %d is a string table; file1 sec %d is not.\n", sec, sec);
				ClearStatus(status, IdenticalFiles);
				SetStatus(status, (DifferentFiles|StopProcessing));
				break;
			}
		}
		if (Yflag)
		{
			if (ps1->sh_type == SHT_SYMTAB && 
			    ps2->sh_type != SHT_SYMTAB)
			{
				if (!sflag) printf("file1 sec %d is a symbol table; file2 sec %d is not.\n", sec, sec);
				ClearStatus(status, IdenticalFiles);
				SetStatus(status, (DifferentFiles|StopProcessing));
				break;
			}
			else if (ps1->sh_type != SHT_SYMTAB && 
				 ps2->sh_type == SHT_SYMTAB)
			{
				if (!sflag) printf("file2 sec %d is a symbol table; file1 sec %d is not.\n", sec, sec);
				ClearStatus(status, IdenticalFiles);
				SetStatus(status, (DifferentFiles|StopProcessing));
				break;
			}
		}
		if (isFilteredSection(secname1) ||
		   (Tflag && ps1->sh_type == SHT_STRTAB) ||
		   (Yflag && ps1->sh_type == SHT_SYMTAB))
		{
			// compare, differences are marked and reported.
			cmpfield(ps1->sh_name, ps2->sh_name, "sh_name", 
				Ignore, status);
			cmpfield(ps1->sh_type, ps2->sh_type, "sh_type", 
				StopProcessing, status);
			cmpfield(ps1->sh_flags, ps2->sh_flags, "sh_flags", 
				StopProcessing, status);
			cmpfield(ps1->sh_addr, ps2->sh_addr, "sh_addr", 
				Ignore, status);
			cmpfield(ps1->sh_offset, ps2->sh_offset, "sh_offset", 
				Ignore, status);
			cmpfield(ps1->sh_size, ps2->sh_size, "sh_size", 
				StopProcessing, status);
			cmpfield(ps1->sh_link, ps2->sh_link, "sh_link", 
				StopProcessing, status);
			cmpfield(ps1->sh_info, ps2->sh_info, "sh_info", 
				StopProcessing, status);
			cmpfield(ps1->sh_addralign, ps2->sh_addralign, "sh_addralign", 
				StopProcessing, status);
			cmpfield(ps1->sh_entsize, ps2->sh_entsize, "sh_entsize", 
				StopProcessing, status);
		}
		else
		{
			// compare, but always ignore results; just report
			// the differences.
			//
			cmpfield(ps1->sh_name, ps2->sh_name, "sh_name", 
				Ignore, status);
			cmpfield(ps1->sh_type, ps2->sh_type, "sh_type", 
				Ignore, status);
			cmpfield(ps1->sh_flags, ps2->sh_flags, "sh_flags", 
				Ignore, status);
			cmpfield(ps1->sh_addr, ps2->sh_addr, "sh_addr", 
				Ignore, status);
			cmpfield(ps1->sh_offset, ps2->sh_offset, "sh_offset", 
				Ignore, status);
			cmpfield(ps1->sh_size, ps2->sh_size, "sh_size", 
				Ignore, status);
			cmpfield(ps1->sh_link, ps2->sh_link, "sh_link", 
				Ignore, status);
			cmpfield(ps1->sh_info, ps2->sh_info, "sh_info", 
				Ignore, status);
			cmpfield(ps1->sh_addralign, ps2->sh_addralign, "sh_addralign", 
				Ignore, status);
			cmpfield(ps1->sh_entsize, ps2->sh_entsize, "sh_entsize", 
				Ignore, status);
		}
	}
	return(status);
}

// compare section data
int
cmpsdata(int fd1, Elf32_Ehdr *pehdr1, Elf32_Shdr *pshdr1, char *pstrtbl1,
	 int fd2, Elf32_Ehdr *pehdr2, Elf32_Shdr *pshdr2, char *pstrtbl2)
{
	char *psdata1 = NULL;
	char *psdata2 = NULL;

	// sanity check
	MustBeTrue((pehdr1 != NULL && pshdr1 != NULL) &&
		   (pehdr2 != NULL && pshdr2 != NULL));

	// default return code, no changes
	int status = IdenticalFiles;

	// compare each section, for now.
	for (int sec = 0; sec < pehdr1->e_shnum; sec++)
	{
		// any data?
		if (pshdr1[sec].sh_size == 0 && pshdr2[sec].sh_size != 0)
		{
			if (!sflag) printf("file1 sec %d is zero-length, file2 sec %d is not.\n", sec, sec);
			ClearStatus(status, IdenticalFiles);
			SetStatus(status, DifferentFiles);
			if (fflag)
				break;
			else 
				continue;
		}
		else if (pshdr1[sec].sh_size != 0 && pshdr2[sec].sh_size == 0)
		{
			if (!sflag) printf("file2 sec %d is zero-length, file1 sec %d is not.\n", sec, sec);
			ClearStatus(status, IdenticalFiles);
			SetStatus(status, DifferentFiles);
			if (fflag)
				break;
			else 
				continue;
		}
		else if (pshdr1[sec].sh_size == 0 && pshdr2[sec].sh_size == 0)
		{
			// skip zero-length sections
			if (!sflag) printf("ignoring zero-length sections %d.\n", sec);
			continue;
		} 

		// get section names
		char secname1[BUFSIZ], secname2[BUFSIZ];
		strcpy(secname1, pstrtbl1+pshdr1[sec].sh_name);
		strcpy(secname2, pstrtbl2+pshdr2[sec].sh_name);

		// print message
		if (!sflag) printf("comparing section %d data (%s, %s):\n", 
			sec, secname1, secname2);
		if (strcmp(secname1, secname2) != 0)
		{
			if (!sflag) printf("section names are not the same; cannot continue.\n");
			ClearStatus(status, IdenticalFiles);
			SetStatus(status, (DifferentFiles|StopProcessing));
			break;
		}

		// section header data
		Elf32_Shdr *ps1 = pshdr1+sec;
		Elf32_Shdr *ps2 = pshdr2+sec;

		// compare types, if requested.
		if (Tflag)
		{
			if (ps1->sh_type == SHT_STRTAB && 
			    ps2->sh_type != SHT_STRTAB)
			{
				if (!sflag) printf("file1 sec %d is a string table; file2 sec %d is not.\n", sec, sec);
				ClearStatus(status, IdenticalFiles);
				SetStatus(status, (DifferentFiles|StopProcessing));
				break;
			}
			else if (ps1->sh_type != SHT_STRTAB && 
				 ps2->sh_type == SHT_STRTAB)
			{
				if (!sflag) printf("file2 sec %d is a string table; file1 sec %d is not.\n", sec, sec);
				ClearStatus(status, IdenticalFiles);
				SetStatus(status, (DifferentFiles|StopProcessing));
				break;
			}
		}
		if (Yflag)
		{
			if (ps1->sh_type == SHT_SYMTAB && 
			    ps2->sh_type != SHT_SYMTAB)
			{
				if (!sflag) printf("file1 sec %d is a symbol table; file2 sec %d is not.\n", sec, sec);
				ClearStatus(status, IdenticalFiles);
				SetStatus(status, (DifferentFiles|StopProcessing));
				break;
			}
			else if (ps1->sh_type != SHT_SYMTAB && 
				 ps2->sh_type == SHT_SYMTAB)
			{
				if (!sflag) printf("file2 sec %d is a symbol table; file1 sec %d is not.\n", sec, sec);
				ClearStatus(status, IdenticalFiles);
				SetStatus(status, (DifferentFiles|StopProcessing));
				break;
			}
		}

		// there are certain sections that contain no
		// information in the file. these sections
		// have a sh_type set to SHT_NOBITS. skip reading
		// in these sections.
		//
		if (ps1->sh_type == SHT_NOBITS || ps2->sh_type == SHT_NOBITS)
		{
			if (!sflag) printf("skipping NOBITS section(s).\n");
			continue;
		}

		// read in sections
		readsdata(fd1, pehdr1, pshdr1, sec, psdata1);
		readsdata(fd2, pehdr2, pshdr2, sec, psdata2);

		// compare data
		int secstatus = cmpbytes(pshdr1[sec].sh_offset, 
			pshdr1[sec].sh_size, pshdr2[sec].sh_size, 
			(unsigned char *)psdata1, (unsigned char *)psdata2);
		if (!IsIdentical(secstatus))
		{
			// check if section of interest.
			if (isFilteredSection(secname1) ||
			   (Tflag && ps1->sh_type == SHT_STRTAB) ||
			   (Yflag && ps1->sh_type == SHT_SYMTAB))
			{
				// set overall status
				ClearStatus(status, IdenticalFiles);
				SetStatus(status, secstatus);

				// do we stop?
				if (IsDifferent(secstatus)) 
				{
					if (!sflag) printf("differences found.\n");
					if (fflag)
						break;
				}
				if (IsStop(secstatus) || IsError(secstatus))
					break;
			}
			else
			{
				// ignore differences, just report them.
				if (IsDifferent(secstatus)) 
				{
					if (!sflag) printf("differences found; ignoring them.\n");
				}
			}
		}
	}
	return(status);
}

// compare program headers
int
cmpphdr(Elf32_Ehdr *pehdr1, Elf32_Phdr *pphdr1, 
	Elf32_Ehdr *pehdr2, Elf32_Phdr *pphdr2)
{
	// sanity check
	MustBeTrue((pehdr1 != NULL && pphdr1 != NULL) &&
		   (pehdr2 != NULL && pphdr2 != NULL));

	// print message
	if (!sflag) printf("comparing program headers:\n");

	// default return code, no changes
	int status = IdenticalFiles;

	// compare each segment
	for (int sec = 0; sec < pehdr1->e_phnum; sec++)
	{
		// skip undefined sections
		if (pphdr1[sec].p_type == PT_NULL &&
		    pphdr2[sec].p_type != PT_NULL)
		{
			if (!sflag) printf("file1 seg %d is NULL, file2 seg %d is not NULL.\n", sec, sec);
			ClearStatus(status, IdenticalFiles);
			SetStatus(status, DifferentFiles);
			if (fflag)
				break;
			else 
				continue;
		}
		else if (pphdr1[sec].p_type != PT_NULL &&
		         pphdr2[sec].p_type == PT_NULL)
		{
			if (!sflag) printf("file2 seg %d is NULL, file1 seg %d is not NULL.\n", sec, sec);
			ClearStatus(status, IdenticalFiles);
			SetStatus(status, DifferentFiles);
			if (fflag)
				break;
			else
				continue;
		}
		else if (pphdr1[sec].p_type == PT_NULL &&
		         pphdr2[sec].p_type == PT_NULL)
		{
			// just ignore
			if (!sflag) printf("ignoring NULL segment %d.\n", sec);
			continue;
		}

		// print message
		if (!sflag) 
			printf("comparing segment %d header:\n", sec);

		// default status
		int secstatus = IdenticalFiles;

		// compare field to field. recovery depends on 
		// type of section.
		//
		Elf32_Phdr *pp1 = pphdr1+sec;
		Elf32_Phdr *pp2 = pphdr2+sec;
		cmpfield(pp1->p_type, pp2->p_type, "p_type", 
			StopProcessing, status);
		cmpfield(pp1->p_offset, pp2->p_offset, "p_offset", 
			Ignore, status);
		cmpfield(pp1->p_vaddr, pp2->p_vaddr, "p_vaddr", 
			Ignore, status);
		cmpfield(pp1->p_paddr, pp2->p_paddr, "p_paddr", 
			Ignore, status);
		cmpfield(pp1->p_filesz, pp2->p_filesz, "p_filessz", 
			StopProcessing, status);
		cmpfield(pp1->p_memsz, pp2->p_memsz, "p_memsz", 
			StopProcessing, status);
		cmpfield(pp1->p_flags, pp2->p_flags, "p_flags", 
			StopProcessing, status);
		cmpfield(pp1->p_align, pp2->p_align, "p_align", 
			StopProcessing, status);
	}
	return(status);
}

// compare program data
int
cmppdata(int fd1, Elf32_Ehdr *pehdr1, Elf32_Phdr *pphdr1, 
	 int fd2, Elf32_Ehdr *pehdr2, Elf32_Phdr *pphdr2)
{
	char *ppdata1 = NULL;
	char *ppdata2 = NULL;

	// sanity check
	MustBeTrue((pehdr1 != NULL && pphdr1 != NULL) &&
		   (pehdr2 != NULL && pphdr2 != NULL));

	// default return code, no changes
	int status = IdenticalFiles;

	// compare each section, for now.
	for (int sec = 0; sec < pehdr1->e_phnum; sec++)
	{
		// any data?
		if (pphdr1[sec].p_filesz == 0 || pphdr2[sec].p_filesz == 0)
		{
			// skip zero-length sections
			continue;
		}

		// print message
		if (!sflag) printf("comparing segment %d data:\n", sec);

		// read in sections
		readpdata(fd1, pehdr1, pphdr1, sec, ppdata1);
		readpdata(fd2, pehdr2, pphdr2, sec, ppdata2);

		// compare data
		int secstatus = cmpbytes(pphdr1[sec].p_offset, 
			pphdr1[sec].p_filesz, pphdr2[sec].p_filesz, 
			(unsigned char *)ppdata1, (unsigned char *)ppdata2);
		if (!IsIdentical(secstatus))
		{
			// set overall status
			ClearStatus(status, IdenticalFiles);
			SetStatus(status, secstatus);

			// do we stop?
			if (IsDifferent(secstatus)) 
			{
				if (!sflag) printf("differences found.\n");
				if (fflag)
					break;
			}
			if (IsStop(secstatus) || IsError(secstatus))
				break;
		}
	}
	return(status);
}

// compare files
int
compare(int fd1, int fd2)
{
	int status = IdenticalFiles;
	int substatus = UnknownStatus;

	// sanity checks
	MustBeTrue(fd1 != NOTOK && fd2 != NOTOK);

	// read in file headers
	Elf32_Ehdr *pehdr1 = NULL;
	readehdr(fd1, pehdr1);
	Elf32_Ehdr *pehdr2 = NULL;
	readehdr(fd2, pehdr2);

	// files must be elf object files
	MustBeTrue(isElf(pehdr1));
	MustBeTrue(isElf(pehdr2));
	MustBeTrue(isElf32Bit(pehdr1));
	MustBeTrue(isElf32Bit(pehdr2));

	// compare file headers
	substatus = cmpehdrs(pehdr1, pehdr2);
	if (!IsIdentical(substatus))
	{
		// set overall status
		ClearStatus(status, IdenticalFiles);
		SetStatus(status, substatus);

		// do we stop?
		if (fflag && IsDifferent(substatus)) 
			return(status);
		if (IsStop(substatus) || IsError(substatus))
			return(status);
	}

	// compare section headers
	char *pshstrtbl1 = NULL;
	char *pshstrtbl2 = NULL;
	Elf32_Shdr *pshdr1 = NULL;
	Elf32_Shdr *pshdr2 = NULL;
	if (!Sflag || (pehdr1->e_shnum == 0 && pehdr2->e_shnum == 0))
	{
		if (!sflag) printf("skipping section comparison.\n");
	}
	else if (pehdr1->e_shnum == 0 && pehdr2->e_shnum != 0)
	{
		if (!sflag) printf("file1 has no sections, file2 does.\n");
		ClearStatus(status, IdenticalFiles);
		SetStatus(status, DifferentFiles);
		if (fflag) return(status);
	}
	else if (pehdr1->e_shnum != 0 && pehdr2->e_shnum == 0)
	{
		if (!sflag) printf("file2 has no sections, file1 does.\n");
		ClearStatus(status, IdenticalFiles);
		SetStatus(status, DifferentFiles);
		if (fflag) return(status);
	}
	else
	{
		// read in section headers
		readshdr(fd1, pehdr1, pshdr1, pshstrtbl1);
		readshdr(fd2, pehdr2, pshdr2, pshstrtbl2);

		// compare section headers
		substatus = cmpshdr(pehdr1, pshdr1, pshstrtbl1, 
				    pehdr2, pshdr2, pshstrtbl2);
		if (!IsIdentical(substatus))
		{
			// set overall status
			ClearStatus(status, IdenticalFiles);
			SetStatus(status, substatus);

			// do we stop?
			if (fflag && IsDifferent(substatus)) 
				return(status);
			if (IsStop(substatus) || IsError(substatus))
				return(status);
		}

		// compare section data
		substatus = cmpsdata(fd1, pehdr1, pshdr1, pshstrtbl1, 
				     fd2, pehdr2, pshdr2, pshstrtbl2);
		if (!IsIdentical(substatus))
		{
			// set overall status
			ClearStatus(status, IdenticalFiles);
			SetStatus(status, substatus);

			// do we stop?
			if (fflag && IsDifferent(substatus)) 
				return(status);
			if (IsStop(substatus) || IsError(substatus))
				return(status);
		}
	}

	// compare program headers
	int phdrstatus = 0;
	Elf32_Phdr *pphdr1 = NULL;
	Elf32_Phdr *pphdr2 = NULL;
	if (!Pflag || (pehdr1->e_phnum == 0 && pehdr2->e_phnum == 0))
	{
		if (!sflag) printf("skipping segment comparison.\n");
	}
	else if (pehdr1->e_phnum == 0 && pehdr2->e_phnum != 0)
	{
		if (!sflag) printf("file1 has no segments, file2 does.\n");
		ClearStatus(status, IdenticalFiles);
		SetStatus(status, DifferentFiles);
		if (fflag) return(status);
	}
	else if (pehdr1->e_phnum != 0 && pehdr2->e_phnum == 0)
	{
		if (!sflag) printf("file2 has no segments, file1 does.\n");
		ClearStatus(status, IdenticalFiles);
		SetStatus(status, DifferentFiles);
		if (fflag) return(status);
	}
	else
	{
		// read in section headers
		readphdr(fd1, pehdr1, pphdr1);
		readphdr(fd2, pehdr2, pphdr2);

		// compare program headers
		substatus = cmpphdr(pehdr1, pphdr1, pehdr2, pphdr2);
		if (!IsIdentical(substatus))
		{
			// set overall status
			ClearStatus(status, IdenticalFiles);
			SetStatus(status, substatus);

			// do we stop?
			if (fflag && IsDifferent(substatus)) 
				return(status);
			if (IsStop(substatus) || IsError(substatus))
				return(status);
		}

		// compare program data
		substatus = cmppdata(fd1, pehdr1, pphdr1, fd2, pehdr2, pphdr2);
		if (!IsIdentical(substatus))
		{
			// set overall status
			ClearStatus(status, IdenticalFiles);
			SetStatus(status, substatus);

			// do we stop?
			if (fflag && IsDifferent(substatus)) 
				return(status);
			if (IsStop(substatus) || IsError(substatus))
				return(status);
		}
	}

	// close files
	close(fd1);
	close(fd2);

	// return difference status
	return(status);
}

