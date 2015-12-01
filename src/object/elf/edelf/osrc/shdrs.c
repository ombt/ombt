// perform editing operations

// headers
#include "edelf.h"
#include "trans.h"

// editing section headers 
void
readshdrs(char *s)
{
	// read in file header
	readfhdr(s);

	// check if file contains any section headers
	MustBeTrue(pehdr->e_shoff != 0);

	// allocate a buffer for reading in section header table
	if (pshdrs != NULL)
	{
		delete [] pshdrs;
		pshdrs = NULL;
	}
	int numbytes = pehdr->e_shentsize*pehdr->e_shnum;
	MustBeTrue(numbytes > 0);
	pshdrs = (Elf32_Shdr *) new char [numbytes];
	MustBeTrue(pshdrs != NULL);

	// read in section header tables
	MustBeTrue(lseek(efd, pehdr->e_shoff, SEEK_SET) != NOTOK)
	MustBeTrue(read(efd, pshdrs, numbytes) == numbytes);

	// get section header string table index
	if (pehdr->e_shstrndx == SHN_UNDEF)
	{
		// no string table. 
		printf("WARNING: no section header string table index.\n");

		// delete anything that is there.
		if (pshstrtbl != NULL)
		{
			delete [] pshstrtbl;
			pshstrtbl = NULL;
		}
	}
	else
	{
		// section header to section header table string table
		pshstrtblh = pshdrs+pehdr->e_shstrndx;

		// allocate a string table
		if (pshstrtbl != NULL)
		{
			delete [] pshstrtbl;
			pshstrtbl = NULL;
		}
		pshstrtbl = new char [pshstrtblh->sh_size];
		MustBeTrue(pshstrtbl != NULL);

		// read in string table 
		MustBeTrue(lseek(efd, pshstrtblh->sh_offset, SEEK_SET) != NOTOK)
		int bytestoread = pshstrtblh->sh_size;
		MustBeTrue(read(efd, pshstrtbl, bytestoread) == bytestoread);
	}
	return;
}

static void
review(int sec)
{
	if (sec < 0 || sec > pehdr->e_shnum)
	{
		printf("out-of-range section number.\n");
		return;
	}
	printf("section %d: %s (%d)\n", 
		sec, pshstrtbl+pshdrs[sec].sh_name,
		pshdrs[sec].sh_name);
	printf("sh_name     : 0x%lx\n", 
		pshdrs[sec].sh_name);
	printf("sh_type     : 0x%lx (%s)\n",
		pshdrs[sec].sh_type,
		ir2s(sh_type, rsh_type, pshdrs[sec].sh_type));
	printf("sh_flags    : 0x%lx (%s)\n",
		pshdrs[sec].sh_flags, 
		b2s(sh_flags, pshdrs[sec].sh_flags));
	printf("sh_addr     : 0x%lx\n",
		pshdrs[sec].sh_addr);
	printf("sh_offset   : 0x%lx\n",
		pshdrs[sec].sh_offset);
	printf("sh_size     : 0x%lx\n",
		pshdrs[sec].sh_size);
	printf("sh_link     : 0x%lx\n",
		pshdrs[sec]. sh_link);
	printf("sh_info     : 0x%lx\n",
		pshdrs[sec].sh_info);
	printf("sh_addralign: 0x%lx\n",
		pshdrs[sec].sh_addralign);
	printf("sh_entsize  : 0x%lx\n",
		pshdrs[sec].sh_entsize);
	return;
}


static void
update(int sec)
{
	int upd = 0;
	char s[BUFSIZ];

	// tell user which section is being updated
	printf("\nsection (%d) %s header:\n", 
		sec, pshstrtbl+pshdrs[sec].sh_name);

	// update section fields
	printf("sh_name [cr=%s]: ", pshstrtbl+pshdrs[sec].sh_name);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		int sz = strlen(pshstrtbl+pshdrs[sec].sh_name);
		strncpy(pshstrtbl+pshdrs[sec].sh_name, s, sz);
		(pshstrtbl+pshdrs[sec].sh_name)[sz] = '\0';
		printf("sh_name: %s\n", pshstrtbl+pshdrs[sec].sh_name);
	}
	printf("sh_type [cr=0x%lx]: ", pshdrs[sec].sh_type);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pshdrs[sec].sh_type = MYatoi(s);
		printf("sh_type: 0x%lx\n", pshdrs[sec].sh_type);
	}
	printf("sh_flags [cr=0x%lx]: ", pshdrs[sec].sh_flags);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pshdrs[sec].sh_flags = MYatoi(s);
		printf("sh_flags: 0x%lx\n", pshdrs[sec].sh_flags);
	}
	printf("sh_addr [cr=0x%lx]: ", pshdrs[sec].sh_addr);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pshdrs[sec].sh_addr = MYatoi(s);
		printf("sh_addr: 0x%lx\n", pshdrs[sec].sh_addr);
	}
	printf("sh_offset [cr=0x%lx]: ", pshdrs[sec].sh_offset);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pshdrs[sec].sh_offset = MYatoi(s);
		printf("sh_offset: 0x%lx\n", pshdrs[sec].sh_offset);
	}
	printf("sh_size [cr=0x%lx]: ", pshdrs[sec].sh_size);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pshdrs[sec].sh_size = MYatoi(s);
		printf("sh_size: 0x%lx\n", pshdrs[sec].sh_size);
	}
	printf("sh_link [cr=0x%lx]: ", pshdrs[sec].sh_link);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pshdrs[sec].sh_link = MYatoi(s);
		printf("sh_link: 0x%lx\n", pshdrs[sec].sh_link);
	}
	printf("sh_info [cr=0x%lx]: ", pshdrs[sec].sh_info);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pshdrs[sec].sh_info = MYatoi(s);
		printf("sh_info: 0x%lx\n", pshdrs[sec].sh_info);
	}
	printf("sh_addralign [cr=0x%lx]: ", pshdrs[sec].sh_addralign);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pshdrs[sec].sh_addralign = MYatoi(s);
		printf("sh_addralign: 0x%lx\n", pshdrs[sec].sh_addralign);
	}
	printf("sh_entsize [cr=0x%lx]: ", pshdrs[sec].sh_entsize);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pshdrs[sec].sh_entsize = MYatoi(s);
		printf("sh_entsize: 0x%lx\n", pshdrs[sec].sh_entsize);
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
			int numbytes = pehdr->e_shentsize*pehdr->e_shnum;
			MustBeTrue(numbytes > 0);

			// write in section header tables
			MustBeTrue(lseek(efd, 
				pehdr->e_shoff, SEEK_SET) != NOTOK)
			MustBeTrue(write(efd, pshdrs, numbytes) == numbytes);

			// write in string table 
			MustBeTrue(lseek(efd, 
				pshstrtblh->sh_offset, SEEK_SET) != NOTOK)
			int bytestowrite = pshstrtblh->sh_size;
			MustBeTrue(write(efd, 
				pshstrtbl, bytestowrite) == bytestowrite);
		}
		else
		{
			// no data was written, but now the current data
			// is wrong. reread the data from disk.
			char dummy[1];
			readshdrs(dummy);
		}
	}
	return;
}

static void
printmenu()
{
	printf("\nsection headers menu:\n\n");
	printf("? or h - show menu\n");
	printf("n - show all section names\n");
	printf("r - review current section header data\n");
	printf("r * - review all section headers data\n");
	printf("r <name> - review <name> section header data\n");
	printf("r <number> - review <number> section header data\n");
	printf("+ - show next section header data\n");
	printf("- - show previous section header data\n");
	printf("u - update current section data\n");
	printf("q - quit\n\n");
	return;
}

void
editshdrs(char *)
{
	char s[BUFSIZ];

	// start of section headers editing
	printf("editing sections headers:\n");

	// set current section
	int csec = 0;

	// start interactive loop
	for (int done=0; !done; )
	{
		// get cmd from user
		printf("shdrs cmd: ");
		rmvnlgets(s);
		tokenize(s, " \t");
		char *pt = gettoken(1);

		// what is the command
		if (pt == NULL || *pt == '\0')
		{
			if ((csec + 1) < pehdr->e_shnum)
			{
				csec++;
				review(csec);
			}
			else
				printf("cannot increment beyond last section.\n");
		}
		else if (*pt == '?' || *pt == 'h')
		{
			printmenu();
		}
		else if (*pt == 'n')
		{
			// show all section names
			printf("section names:\n");
			for (int sec=0; sec<pehdr->e_shnum; sec++)
			{
				printf("section %d: %s (%d, %s)\n", sec, 
					pshstrtbl+pshdrs[sec].sh_name,
					pshdrs[sec].sh_name, 
					ir2s(sh_type, rsh_type, 
						pshdrs[sec].sh_type));
			}
		}
		else if (*pt == 'r')
		{
			// get next token
			pt = gettoken(2);

			// type of review
			if (pt == NULL || *pt == '\0')
			{
				review(csec);
			}
			else if (*pt == '*')
			{
				printf("section hdrs data:\n");
				for (int sec=0; sec<pehdr->e_shnum; sec++)
				{
					review(sec);
				}
			}
			else if (strspn(pt, "0123456789") == strlen(pt))
			{
				// show a specific section
				printf("section hdr data:\n");
				int sec = MYatoi(pt);
				if (sec < 0 || sec > pehdr->e_shnum)
				{
					printf("out-of-range section number.\n");
					return;
				}
				csec = sec;
				review(csec);
			}
			else
			{
				// show a specific section
				char s[BUFSIZ];
				printf("section hdr data:\n");
				int found = 0;
				for (int sec=0; 
				    (sec = nametosec(pt, sec)) != NOTOK; sec++)
				{
					found = 1;
					csec = sec;
					review(csec);
					printf("next section ? [n/y/cr=y] ");
					rmvnlgets(s);
					if (*s != 'y' && *s != '\0')
						break;
				}
				if (!found)
				{
					printf("invalid section name.\n");
					continue;
				}
			}
		}
		else if (*pt == 'u')
		{
			update(csec);
		}
		else if (*pt == '+')
		{
			if ((csec + 1) < pehdr->e_shnum)
			{
				csec++;
				review(csec);
			}
			else
				printf("cannot increment beyond last section.\n");
		}
		else if (*pt == '-')
		{
			if ((csec - 1) >= 0)
			{
				csec--;
				review(csec);
			}
			else
				printf("cannot decrement before first section.\n");
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

