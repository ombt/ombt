// perform editing operations

// headers
#include "edelf.h"
#include "trans.h"

// editing program headers 
void
readphdrs(char *s)
{
	// read in file header
	readfhdr(s);

	// check if file contains any section headers
	MustBeTrue(pehdr->e_phoff != 0);

	// allocate a buffer for reading in section header table
	if (pphdrs != NULL)
	{
		delete [] pphdrs;
		pphdrs = NULL;
	}
	int numbytes = pehdr->e_phentsize*pehdr->e_phnum;
	MustBeTrue(numbytes > 0);
	pphdrs = (Elf32_Phdr *) new char [numbytes];
	MustBeTrue(pphdrs != NULL);

	// read in program header tables
	MustBeTrue(lseek(efd, pehdr->e_phoff, SEEK_SET) != NOTOK)
	MustBeTrue(read(efd, pphdrs, numbytes) == numbytes);
	return;
}

static void
review(int seg)
{
	if (seg < 0 || seg > pehdr->e_phnum)
	{
		printf("out-of-range segment number.\n");
		return;
	}
	printf("\nsegment : %d\n", seg);
	printf("p_type  : 0x%lx (%s)\n",
		pphdrs[seg].p_type,
		ir2s(p_types, rp_types, pphdrs[seg].p_type));
	printf("p_offset: 0x%lx\n",
		pphdrs[seg].p_offset);
	printf("p_vaddr : 0x%lx\n",
		pphdrs[seg].p_vaddr);
	printf("p_paddr : 0x%lx\n",
		pphdrs[seg].p_paddr);
	printf("p_filesz: 0x%lx\n",
		pphdrs[seg].p_filesz);
	printf("p_memsz : 0x%lx\n",
		pphdrs[seg].p_memsz);
	printf("p_flags : 0x%lx (%s)\n",
		pphdrs[seg].p_flags,
		b2s(p_flags, pphdrs[seg].p_flags));
	printf("p_align : 0x%lx\n",
		pphdrs[seg].p_align);
	return;
}


static void
update(int seg)
{
	int upd = 0;
	char s[BUFSIZ];

	// tell user which segment is being updated
	printf("\nsegment %d header:\n", seg);

	// update segment fields
	printf("p_type [cr=0x%lx]: ", pphdrs[seg].p_type);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pphdrs[seg].p_type = MYatoi(s);
		printf("p_type: 0x%lx\n", pphdrs[seg].p_type);
	}
	printf("p_offset [cr=0x%lx]: ", pphdrs[seg].p_offset);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pphdrs[seg].p_offset = MYatoi(s);
		printf("p_offset: 0x%lx\n", pphdrs[seg].p_offset);
	}
	printf("p_vaddr [cr=0x%lx]: ", pphdrs[seg].p_vaddr);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pphdrs[seg].p_vaddr = MYatoi(s);
		printf("p_vaddr: 0x%lx\n", pphdrs[seg].p_vaddr);
	}
	printf("p_paddr [cr=0x%lx]: ", pphdrs[seg].p_paddr);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pphdrs[seg].p_paddr = MYatoi(s);
		printf("p_paddr: 0x%lx\n", pphdrs[seg].p_paddr);
	}
	printf("p_filesz [cr=0x%lx]: ", pphdrs[seg].p_filesz);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pphdrs[seg].p_filesz = MYatoi(s);
		printf("p_filesz: 0x%lx\n", pphdrs[seg].p_filesz);
	}
	printf("p_memsz [cr=0x%lx]: ", pphdrs[seg].p_memsz);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pphdrs[seg].p_memsz = MYatoi(s);
		printf("p_memsz: 0x%lx\n", pphdrs[seg].p_memsz);
	}
	printf("p_flags [cr=0x%lx]: ", pphdrs[seg].p_flags);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pphdrs[seg].p_flags = MYatoi(s);
		printf("p_flags: 0x%lx\n", pphdrs[seg].p_flags);
	}
	printf("p_align [cr=0x%lx]: ", pphdrs[seg].p_align);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pphdrs[seg].p_align = MYatoi(s);
		printf("p_align: 0x%lx\n", pphdrs[seg].p_align);
	}
	if (upd > 0)
	{
		printf("write to file [cr=n/n/y] ? ");
		rmvnlgets(s);
		if (*s == 'y')
		{
			if (!uflag)
			{
				close(efd);
				MustBeTrue((efd = open(efn, O_RDWR)) != NOTOK);
				uflag = 1;
			}
			int numbytes = pehdr->e_phentsize*pehdr->e_phnum;
			MustBeTrue(numbytes > 0);

			// write in segment header tables
			MustBeTrue(lseek(efd, 
				pehdr->e_phoff, SEEK_SET) != NOTOK)
			MustBeTrue(write(efd, pphdrs, numbytes) == numbytes);

		}
		else
		{
			// no data was written, but now the current data
			// is wrong. reread the data from disk.
			char dummy[1];
			readphdrs(dummy);
		}
	}
	return;
}

static void
printmenu()
{
	printf("\nprogram headers menu:\n\n");
	printf("? or h - show menu\n");
	printf("r - review current program header data\n");
	printf("r * - review all program headers data\n");
	printf("r <number> - review <number> program header data\n");
	printf("+ - show next program header data\n");
	printf("- - show previous program header data\n");
	printf("u - update current program data\n");
	printf("q - quit\n\n");
	return;
}

void
editphdrs(char *)
{
	char s[BUFSIZ];

	// start of program headers editing
	printf("editing program headers:\n");

	// set current segment
	int cseg = 0;

	// start interactive loop
	for (int done=0; !done; )
	{
		// get cmd from user
		printf("phdrs cmd: ");
		rmvnlgets(s);
		tokenize(s, " \t");
		char *pt = gettoken(1);

		// what is the command
		if (pt == NULL || *pt == '\0')
		{
			if ((cseg + 1) < pehdr->e_phnum)
			{
				cseg++;
				review(cseg);
			}
			else
				printf("cannot increment beyond last segment.\n");
		}
		else if (*pt == '?' || *pt == 'h')
		{
			printmenu();
		}
		else if (*pt == 'r')
		{
			// get next token
			pt = gettoken(2);

			// type of review
			if (pt == NULL || *pt == '\0')
			{
				review(cseg);
			}
			else if (*pt == '*')
			{
				printf("program segment hdrs data:\n");
				for (int seg=0; seg<pehdr->e_phnum; seg++)
				{
					review(seg);
				}
			}
			else if (strspn(pt, "0123456789") == strlen(pt))
			{
				// show a specific segment
				printf("segment hdr data:\n");
				int seg = MYatoi(pt);
				if (seg < 0 || seg > pehdr->e_phnum)
				{
					printf("out-of-range segment number.\n");
					return;
				}
				cseg = seg;
				review(cseg);
			}
			else
			{
				printf("invalid segment number.\n");
			}
		}
		else if (*pt == 'u')
		{
			update(cseg);
		}
		else if (*pt == '+')
		{
			if ((cseg + 1) < pehdr->e_phnum)
			{
				cseg++;
				review(cseg);
			}
			else
				printf("cannot increment beyond last segment.\n");
		}
		else if (*pt == '-')
		{
			if ((cseg - 1) >= 0)
			{
				cseg--;
				review(cseg);
			}
			else
				printf("cannot decrement before first segment.\n");
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

