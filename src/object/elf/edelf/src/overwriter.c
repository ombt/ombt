// perform editing operations

// headers
#include "edelf.h"
#include "trans.h"

// globals 
static const char nibble2char[] = "0123456789abcdef";

// overwriter of sections or segments
void
readoverwriter(ElfObject &eo, char *s)
{
	eo.readehdr();
	eo.readshdrs();
	if (eo.pehdr()->e_phoff != 0)
		eo.readphdrs();
	return;
}

static void
printmenu()
{
	printf("\noverwriter menu:\n\n");
	printf("? or h - show menu\n");
	printf("s - show section header data\n");
	printf("p - show program header data\n");
	printf("so <offset> - file overwriter (uses section headers)\n");
	printf("so <section> <reloffset> - file overwriter (uses section headers)\n");
	printf("so <section#> <reloffset> - file overwriter (uses section headers)\n");
	printf("po <address> - memory overwriter (uses program headers)\n");
	printf("q - quit\n\n");
	return;
}

static void
overwriter(ElfObject &eo, long base, long fldsz, char *fld, int &upd, int usehex)
{
	char input[BUFSIZ];

	// number of chars per row
	static const int CharsPerRow = 4*sizeof(long);
	static const int NibblesPerRow = 2*CharsPerRow;

	// determine the number of rows and extra characters
	int totalchars = sizeof(char)*fldsz;
	int nrows = totalchars/CharsPerRow;
	int extrachars = totalchars%CharsPerRow;

	// pointer to current row
	char *prow = (char *)fld;
	char *pstart = (char *)fld;
	char *pbase = (char *)base;

	// start displaying and updating row by row
	int row, done;
	for (done = row = 0; row < nrows; row++, prow += CharsPerRow)
	{
		// first display the row
		if (usehex)
			printf("%08lx: ", pbase+(prow-pstart));
		else
			printf("%010ld: ", pbase+(prow-pstart));

		for (int ichar = 0; ichar < CharsPerRow; ichar++)
		{
			unsigned char highnibble = (prow[ichar]&0xf0) >> 4;
			unsigned char lownibble = prow[ichar]&0xf;
			printf("%c%c", nibble2char[highnibble], 
				nibble2char[lownibble]);
		}
		printf("    ");
#if EXPTOOLS==1
		for (ichar = 0; ichar < CharsPerRow; ichar++)
#else
		for (int ichar = 0; ichar < CharsPerRow; ichar++)
#endif
		{
			printf("%c", num2ch[(unsigned char)prow[ichar]]);
		}
		printf("\n");

		// now let user update
		zap(input, ' ', NibblesPerRow+2);
		if (usehex)
			printf("%08lx: ", pbase+(prow-pstart));
		else
			printf("%010ld: ", pbase+(prow-pstart));
		gets(input);

		// check the input
		if (strspn(input, " \n") == strlen(input))
		{
			// just white space, skip current line
			continue;
		}
		else if (strspn(input, "0123456789abcdefABCDEF \n") != 
				strlen(input))
		{
			// junk, exit from line
			done = 1;
			break;;
		}

		// we have a good line
		upd = 1;
		for (int n=0; n<NibblesPerRow && input[n] != '\n'; n++)
		{
			if (n%2 == 0)
			{
				if ('0' <= input[n] && input[n] <= '9')
				{
					prow[n/2] &= 0x0f;
					prow[n/2] |= ((input[n]-'0') << 4);
				}
				else if ('a' <= input[n] && input[n] <= 'f')
				{
					prow[n/2] &= 0x0f;
					prow[n/2] |= ((10+input[n]-'a') << 4);
				}
				else if ('A' <= input[n] && input[n] <= 'F')
				{
					prow[n/2] &= 0x0f;
					prow[n/2] |= ((10+input[n]-'A') << 4);
				}
			}
			else
			{
				if ('0' <= input[n] && input[n] <= '9')
				{
					prow[n/2] &= 0xf0;
					prow[n/2] |= (input[n]-'0');
				}
				else if ('a' <= input[n] && input[n] <= 'f')
				{
					prow[n/2] &= 0xf0;
					prow[n/2] |= (10+input[n]-'a');
				}
				else if ('A' <= input[n] && input[n] <= 'F')
				{
					prow[n/2] &= 0xf0;
					prow[n/2] |= (10+input[n]-'A');
				}
			}
		}

		// display the row again
		if (usehex)
			printf("%08lx: ", pbase+(prow-pstart));
		else
			printf("%010ld: ", pbase+(prow-pstart));
#if EXPTOOLS==1
		for (ichar = 0; ichar < CharsPerRow; ichar++)
#else
		for (int ichar = 0; ichar < CharsPerRow; ichar++)
#endif
		{
			unsigned char highnibble = (prow[ichar]&0xf0) >> 4;
			unsigned char lownibble = prow[ichar]&0xf;
			printf("%c%c", nibble2char[highnibble], 
				nibble2char[lownibble]);
		}
		printf("    ");
#if EXPTOOLS==1
		for (ichar = 0; ichar < CharsPerRow; ichar++)
#else
		for (int ichar = 0; ichar < CharsPerRow; ichar++)
#endif
		{
			printf("%c", num2ch[(unsigned char)prow[ichar]]);
		}
		printf("\n");
	}
	while (!done && extrachars > 0)
	{
		// print extra bits to user
		if (usehex)
			printf("%08lx: ", pbase+(prow-pstart));
		else
			printf("%010lx: ", pbase+(prow-pstart));
		for (int ichar = 0; ichar < extrachars; ichar++)
		{
			unsigned char highnibble = (prow[ichar]&0xf0) >> 4;
			unsigned char lownibble = prow[ichar]&0xf;
			printf("%c%c", nibble2char[highnibble], 
				nibble2char[lownibble]);
		}
		printf("    ");
#if EXPTOOLS==1
		for (ichar = 0; ichar < extrachars; ichar++)
#else
		for (int ichar = 0; ichar < extrachars; ichar++)
#endif
		{
			printf("%c", num2ch[(unsigned char)prow[ichar]]);
		}
		printf("\n");

		// now let user update
		zap(input, ' ', NibblesPerRow+2);
		if (usehex)
			printf("%08lx: ", pbase+(prow-pstart));
		else
			printf("%010lx: ", pbase+(prow-pstart));
		gets(input);

		// check the input
		if (strspn(input, " \n") == strlen(input))
		{
			// just white space, skip current line
			break;
		}
		else if (strspn(input, "0123456789abcdefABCDEF \n") != 
				strlen(input))
		{
			// junk, exit from line
			done = 1;
			break;
		}

		// we have a good line
		upd = 1;
		for (int n=0; n<extrachars && input[n] != '\n'; n++)
		{
			if (n%2 == 0)
			{
				if ('0' <= input[n] && input[n] <= '9')
				{
					prow[n/2] &= 0x0f;
					prow[n/2] |= ((input[n]-'0') << 4);
				}
				else if ('a' <= input[n] && input[n] <= 'f')
				{
					prow[n/2] &= 0x0f;
					prow[n/2] |= ((10+input[n]-'a') << 4);
				}
				else if ('A' <= input[n] && input[n] <= 'F')
				{
					prow[n/2] &= 0x0f;
					prow[n/2] |= ((10+input[n]-'A') << 4);
				}
			}
			else
			{
				if ('0' <= input[n] && input[n] <= '9')
				{
					prow[n/2] &= 0xf0;
					prow[n/2] |= (input[n]-'0');
				}
				else if ('a' <= input[n] && input[n] <= 'f')
				{
					prow[n/2] &= 0xf0;
					prow[n/2] |= (10+input[n]-'a');
				}
				else if ('A' <= input[n] && input[n] <= 'F')
				{
					prow[n/2] &= 0xf0;
					prow[n/2] |= (10+input[n]-'A');
				}
			}
		}

		// print extra bits to user
		if (usehex)
			printf("%08lx: ", pbase+(prow-pstart));
		else
			printf("%010lx: ", pbase+(prow-pstart));
#if EXPTOOLS==1
		for (ichar = 0; ichar < extrachars; ichar++)
#else
		for (int ichar = 0; ichar < extrachars; ichar++)
#endif
		{
			unsigned char highnibble = (prow[ichar]&0xf0) >> 4;
			unsigned char lownibble = prow[ichar]&0xf;
			printf("%c%c", nibble2char[highnibble], 
				nibble2char[lownibble]);
		}
		printf("    ");
#if EXPTOOLS==1
		for (ichar = 0; ichar < extrachars; ichar++)
#else
		for (int ichar = 0; ichar < extrachars; ichar++)
#endif
		{
			printf("%c", num2ch[(unsigned char)prow[ichar]]);
		}
		printf("\n");

		// break out of loop
		break;
	}
	return;
}

static void
poverwriter(ElfObject &eo, long addr)
{
	int addrseg = -1;

	// find segment that contains the address
	for (int seg=0; seg < eo.pehdr()->e_phnum; seg++)
	{
		if (eo.pphdr(seg).p_vaddr <= addr &&
		    addr < (eo.pphdr(seg).p_vaddr+eo.pphdr(seg).p_memsz))
		{
			addrseg = seg;
			break;
		}
	}
	if (addrseg == -1)
	{
		printf("address NOT found.\n");
		return;
	}

	// read in segment for overwriting or perusing
	long numbytes = eo.pphdr(addrseg).p_filesz;
	char *p = new char [numbytes];
	MustBeTrue(p != NULL);
	MustBeTrue(lseek(eo.efd(), eo.pphdr(addrseg).p_offset, 
			SEEK_SET) != NOTOK)
	MustBeTrue(read(eo.efd(), p, numbytes) == numbytes);

	// allow user to review or update
	int upd = 0;
	long start = addr-eo.pphdr(addrseg).p_vaddr;
	overwriter(eo, addr, eo.pphdr(addrseg).p_filesz-start, 
			p+start, upd, 1);

	// check if anything changed
	if (upd > 0)
	{
		printf("write to file [cr=n/n/y] ? ");
		char s[BUFSIZ];
		rmvnlgets(s);
		if (*s == 'y')
		{
			eo.openrdwr();
			MustBeTrue(lseek(eo.efd(), eo.pphdr(addrseg).p_offset, 
					SEEK_SET) != NOTOK)
			MustBeTrue(write(eo.efd(), p, numbytes) == numbytes);
		}
	}
	delete [] p;
	return;
}

static void
soverwriter(ElfObject &eo, long offset)
{
	int offsetsec = -1;

	// find section containing the offset
	for (int sec=0; sec < eo.pehdr()->e_shnum; sec++)
	{
		if (eo.pshdr(sec).sh_offset <= offset && 
		    offset < (eo.pshdr(sec).sh_offset+eo.pshdr(sec).sh_size))
		{
			offsetsec = sec;
			break;
		}
	}
	if (offsetsec == -1)
	{
		printf("offset not found.\n");
		return;
	}

	// read in section for overwriting or perusing
	long numbytes = eo.pshdr(offsetsec).sh_size;
	char *p = new char [numbytes];
	MustBeTrue(p != NULL);
	MustBeTrue(lseek(eo.efd(), eo.pshdr(offsetsec).sh_offset, 
			SEEK_SET) != NOTOK)
	MustBeTrue(read(eo.efd(), p, numbytes) == numbytes);

	// allow user to review or update
	int upd = 0;
	long start = offset - eo.pshdr(offsetsec).sh_offset;
	overwriter(eo, offset, eo.pshdr(offsetsec).sh_size-start, 
			p+start, upd, 0);

	// check if anything changed
	if (upd > 0)
	{
		printf("write to file [cr=n/n/y] ? ");
		char s[BUFSIZ];
		rmvnlgets(s);
		if (*s == 'y')
		{
			eo.openrdwr();
			MustBeTrue(lseek(eo.efd(), 
				eo.pshdr(offsetsec).sh_offset, 
					SEEK_SET) != NOTOK)
			MustBeTrue(write(eo.efd(), p, numbytes) == numbytes);
		}
	}
	delete [] p;
	return;
}

void
overwriter(ElfObject &eo, char *)
{
	char s[BUFSIZ];

	// start of overwriter
	printf("section/segment overwriter:\n");

	// start interactive loop
	for (int done=0; !done; )
	{
		// get cmd from user
		printf("overwriter cmd: ");
		rmvnlgets(s);
		tokenize(s, " \t");
		char *pt = gettoken(1);

		// what is the command
		if (pt == NULL)
		{
			printf("unknown cmd.\n");
		}
		else if (*pt == '?' || *pt == 'h')
		{
			printmenu();
		}
		else if (strcmp(pt, "s") == 0)
		{
			// show section data
			printf("section data:\n");
			for (int sec=0; sec<eo.pehdr()->e_shnum; sec++)
			{
				printf("section %d: %s (%ld, %ld)\n", sec, 
					eo.shdrnm(sec),
					eo.pshdr(sec).sh_offset, 
					eo.pshdr(sec).sh_size);
			}
		}
		else if (strcmp(pt, "p") == 0)
		{
			// show section data
			printf("program data:\n");
			for (int seg=0; seg<eo.pehdr()->e_phnum; seg++)
			{
				printf("segment %d: (0x%lx, 0x%lx, %ld)\n", 
					seg, eo.pphdr(seg).p_vaddr, 
					eo.pphdr(seg).p_paddr,
					eo.pphdr(seg).p_memsz);
			}
		}
		else if (strcmp(pt, "po") == 0)
		{
			// get address
			pt = gettoken(2);
			MustBeTrue(pt != NULL && *pt != '\0');
			long addr = MYatoi(pt);
			poverwriter(eo, addr);
		}
		else if (strcmp(pt, "so") == 0)
		{
			// get tokens 
			char *pt2 = gettoken(2);
			char *pt3 = gettoken(3);

			// check if a section was given 
			if (pt2 != NULL && pt3 != NULL)
			{
				long offset = 0;

				// get section
				if (strspn(pt2, "0123456789") == strlen(pt2))
				{
					// section number was given
					int csec = MYatoi(pt2);
					if (csec < 0 || 
					    csec >= eo.pehdr()->e_shnum)
					{
						printf("out-of-range section number.\n");
						printf("minimum section number = 0\n");
						printf("maximum section number = %d\n",
							eo.pehdr()->e_shnum);
						return;
					}
					offset = eo.pshdr(csec).sh_offset;
				}
				else
				{
					// section name was given
					int csec = nametosec(eo, pt2);
					if (csec == NOTOK)
					{
						printf("section not found.\n");
						return;
					}
					offset = eo.pshdr(csec).sh_offset;
				}

				// add relative offset to section offset
				MustBeTrue(pt3 != NULL && *pt3 != '\0');
				offset += MYatoi(pt3);
				soverwriter(eo, offset);
			}
			else if (pt2 != NULL && pt3 == NULL)
			{
				// no section was given, get offset
				MustBeTrue(pt2 != NULL && *pt2 != '\0');
				long offset = MYatoi(pt2);
				soverwriter(eo, offset);
			}
			else
				MustBeTrue(pt2 != NULL || pt3 != NULL);
			
		}
		else if (*pt == 'q')
		{
			done = 1;
		}
		else
		{
			printf("unknown cmd.\n");
		}
	}
	return;
}


