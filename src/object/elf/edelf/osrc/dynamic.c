// perform editing operations

// headers
#include "edelf.h"
#include "trans.h"

// dynamic table entries
void
readdynamic(char *s)
{
	// read in file, section headers, and string tables
	readstrings(s);

	// delete old dynamic tables
	if (pdyntbls != NULL)
	{
		for (int sec=0; sec<pehdr->e_shnum; sec++)
		{
			if (pdyntbls[sec] != NULL)
			{
				delete [] pdyntbls[sec];
				pdyntbls[sec] = NULL;
			}
		}
		delete [] pdyntbls;
		pdyntbls = NULL;
	}

	// allocate a new dynamic table
	if (pdyntbls == NULL)
	{
		// allocate a array of dynamic tables
		pdyntbls = new char * [pehdr->e_shnum];
		MustBeTrue(pdyntbls != NULL);

		// allocate section dynamic tables
		for (int sec=0; sec<pehdr->e_shnum; sec++)
		{
			// initialize
			pdyntbls[sec] = NULL;

			// skip section that are not dynbol tables
			if (pshdrs[sec].sh_type != SHT_DYNAMIC)
				continue;

			// allocate a dynamic table
			pdyntbls[sec] = new char [pshdrs[sec].sh_size];
			MustBeTrue(pdyntbls[sec] != NULL);
		}
	}

	// read in dynamic tables.
	for (int sec=0; sec<pehdr->e_shnum; sec++)
	{
		// skip section that are not dynamic tables
		if (pshdrs[sec].sh_type != SHT_DYNAMIC)
			continue;

		// seek and read in dynamic table
		MustBeTrue(lseek(efd, pshdrs[sec].sh_offset, SEEK_SET) != NOTOK)
		int numbytes = pshdrs[sec].sh_size;
		MustBeTrue(read(efd, pdyntbls[sec], numbytes) == numbytes);
	}
	return;
}

static void
printmenu()
{
	printf("\ndynamic table menu:\n\n");
	printf("? or h - show menu\n");
	printf("n - show all dynamic table section names\n");
	printf("r - review current dynamic entry data\n");
	printf("r * - review all dynamic data\n");
	printf("r <number> - review <number> entry\n");
	printf("+ - show next entry \n");
	printf("- - show previous entry\n");
	printf("u - update current entry\n");
	printf("q - quit\n\n");
	return;
}

static void
showentry(int &cdynno, int &csec, char *&p0, char *&pe)
{
	Elf32_Dyn *pdyn = (Elf32_Dyn *)(p0+cdynno*pshdrs[csec].sh_entsize);
	printf("%d: d_tag     : 0x%lx (%s)\n", 
		cdynno, pdyn->d_tag, ir2s(d_tag, rd_tag, pdyn->d_tag));
	printf("%d: d_un.d_val: 0x%lx\n", cdynno, pdyn->d_un.d_val);
	printf("%d: d_un.d_ptr: 0x%lx\n", cdynno, pdyn->d_un.d_ptr);
	printf("%d: d_un.d_off: 0x%lx\n", cdynno, pdyn->d_un.d_off);
	return;
}

static void
review(int &cdynno, int &csec, char *&p0, char *&pe)
{
	// get tokens from user stream
	char *pt2 = gettoken(2);
	char *pt3 = gettoken(3);

	// determine user request
	if (pt2 == NULL)
	{
		// check current section and location
		if (pshdrs[csec].sh_type != SHT_DYNAMIC)
		{
			// initialize dynamic pointers
			csec = pehdr->e_shnum;
			cdynno = 0;
			for (int sec=0; sec<pehdr->e_shnum; sec++)
			{
				if (pshdrs[sec].sh_type == SHT_DYNAMIC)
				{
					csec = sec;
					break;
				}
			}
			if (csec >= pehdr->e_shnum)
			{
				printf("no dynamic tables found.\n");
				return;
			}
		}
		if (cdynno < 0 || 
		    cdynno*pshdrs[csec].sh_entsize > pshdrs[csec].sh_size)
		{
			printf("entry number is out of range.\n");
			return;
		}

		// show current entry
		showentry(cdynno, csec, p0, pe);
	}
	else if (*pt2 == '*')
	{
		// show all entries
		for (csec=0; csec<pehdr->e_shnum; csec++)
		{
			// skip sections that are not symbol tables
			if (pshdrs[csec].sh_type != SHT_DYNAMIC)
				continue;
	
			// print section name
			printf("section %d: %s (%d)\n", csec,
				pshstrtbl+pshdrs[csec].sh_name, 
				pshdrs[csec].sh_name);

			// print entries in section
			p0 = pdyntbls[csec];
			pe = pdyntbls[csec]+pshdrs[csec].sh_size;
			for (cdynno=0; 
			    (p0+cdynno*pshdrs[csec].sh_entsize)<pe;
			     cdynno++)
			{
				Elf32_Dyn *pdyn = (Elf32_Dyn *)(p0+
					cdynno*pshdrs[csec].sh_entsize);
				if (pdyn->d_tag == DT_NULL) break;
				showentry(cdynno, csec, p0, pe);
			}
		}
	}
	else if (strlen(pt2) == strspn(pt2, "0123456789"))
	{
		// check current section and location
		if (pshdrs[csec].sh_type != SHT_DYNAMIC)
		{
			printf("current section is not a dynamic table.\n");
			return;
		}
		cdynno = MYatoi(pt2);
		if (cdynno < 0 || 
		    cdynno*pshdrs[csec].sh_entsize > pshdrs[csec].sh_size)
		{
			printf("entry number is out of range.\n");
			return;
		}

		// show current entry
		showentry(cdynno, csec, p0, pe);
	}
	else
	{
		printf("unknown command.\n");
	}
	return;
}

static void
update(int &cdynno, int &csec, char *&p0, char *&pe)
{
	int upd = 0;
	char s[BUFSIZ];

	// check section and entry
	if (csec < 0 || csec > pehdr->e_shnum)
	{
		printf("invalid section number.\n");
		return;
	}
	if (pshdrs[csec].sh_type != SHT_DYNAMIC)
	{
		printf("section is not a dynamic table.\n");
		return;
	}
	if (cdynno < 0 || cdynno*pshdrs[csec].sh_entsize > pshdrs[csec].sh_size)
	{
		printf("current entry number is out-of-range.\n");
		return;
	}
	MustBeTrue(pdyntbls[csec] == p0);

	// update data
	Elf32_Dyn *pdyn = (Elf32_Dyn *)(p0+cdynno*pshdrs[csec].sh_entsize);
	printf("%d: d_tag [cr=0x%lx]: ", cdynno, pdyn->d_tag);
        rmvnlgets(s);
        if (*s != '\0')
        {
                upd++;
                pdyn->d_tag= MYatoi(s);
                printf("d_tag: 0x%lx\n", pdyn->d_tag);
	}
	int done = 0;
	printf("%d: d_un.d_val [cr=0x%lx]: ", cdynno, pdyn->d_un.d_val);
        rmvnlgets(s);
        if (*s != '\0')
        {
                upd++; done++;
                pdyn->d_un.d_val= MYatoi(s);
                printf("d_un.d_val: 0x%lx\n", pdyn->d_un.d_val);
	}
	if (!done)
	{
		printf("%d: d_un.d_ptr [cr=0x%lx]: ", cdynno, pdyn->d_un.d_ptr);
        	rmvnlgets(s);
        	if (*s != '\0')
        	{
                	upd++; done++;
                	pdyn->d_un.d_ptr= MYatoi(s);
                	printf("d_un.d_ptr: 0x%lx\n", pdyn->d_un.d_ptr);
		}
	}
	if (!done)
	{
		printf("%d: d_un.d_off [cr=0x%lx]: ", cdynno, pdyn->d_un.d_off);
        	rmvnlgets(s);
        	if (*s != '\0')
        	{
                	upd++; done++;
                	pdyn->d_un.d_off= MYatoi(s);
                	printf("d_un.d_off: 0x%lx\n", pdyn->d_un.d_off);
		}
	}

	// write to file
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
			MustBeTrue(lseek(efd, pshdrs[csec].sh_offset, 
					SEEK_SET) != NOTOK)
			int numbytes = pshdrs[csec].sh_size;
			MustBeTrue(write(efd, pdyntbls[csec], 
					numbytes) == numbytes);
		}

		// reread data
		char dummy[1];
		readdynamic(dummy);
	}
	return;
}

static void
increment(int &cdynno, int &csec, char *&p0, char *&pe)
{
	// check section and symbol
	if (csec < 0 || csec > pehdr->e_shnum)
	{
		printf("invalid section number.\n");
		return;
	}
	if (pshdrs[csec].sh_type != SHT_DYNAMIC)
	{
		printf("section is not a dynamic table.\n");
		return;
	}
	if (cdynno < 0 || cdynno*pshdrs[csec].sh_entsize > pshdrs[csec].sh_size)
	{
		printf("current entry number is out-of-range.\n");
		return;
	}

	// check most common case
	if ((cdynno+1)*pshdrs[csec].sh_entsize < pshdrs[csec].sh_size)
	{
		cdynno++;
		showentry(cdynno, csec, p0, pe);
		return;
	}
	
	// we are passed the end of the current section, look for
	// the next dynamic table section, if any.
	//
	int ncsec = csec+1;
	for ( ; ncsec < pehdr->e_shnum; ncsec++)
	{
		if (pshdrs[ncsec].sh_type == SHT_DYNAMIC)
			break;
	}
	if (ncsec >= pehdr->e_shnum)
	{
		printf("no next dynamic table found.\n");
		return;
	}

	// return the first entry in the section
	cdynno = 0;
	csec = ncsec;
	p0 = pdyntbls[csec];
	pe = pdyntbls[csec]+pshdrs[csec].sh_size;
	showentry(cdynno, csec, p0, pe);
	return;
	return;
}

static void
decrement(int &cdynno, int &csec, char *&p0, char *&pe)
{
	// check section and symbol
	if (csec < 0 || csec > pehdr->e_shnum)
	{
		printf("invalid section number.\n");
		return;
	}
	if (pshdrs[csec].sh_type != SHT_DYNAMIC)
	{
		printf("section is not a dynamic table.\n");
		return;
	}
	if (cdynno < 0 || cdynno*pshdrs[csec].sh_entsize > pshdrs[csec].sh_size)
	{
		printf("current entry number is out-of-range.\n");
		return;
	}

	// check most common case
	if ((cdynno-1) >= 0)
	{
		cdynno--;
		showentry(cdynno, csec, p0, pe);
		return;
	}
	
	// we are passed the end of the current section, look for
	// the previous dynamic table section, if any.
	//
	int ncsec = csec-1;
	for ( ; ncsec >= 0 ; ncsec--)
	{
		if (pshdrs[ncsec].sh_type == SHT_DYNAMIC)
			break;
	}
	if (ncsec < 0)
	{
		printf("no previous dynamic table found.\n");
		return;
	}

	// return the last entry in the section
	csec = ncsec;
	p0 = pdyntbls[csec];
	pe = pdyntbls[csec]+pshdrs[csec].sh_size;
	cdynno = pshdrs[csec].sh_size/pshdrs[csec].sh_entsize;
	showentry(cdynno, csec, p0, pe);
	return;
}

void
editdynamic(char *)
{
	char s[BUFSIZ];

	// start of dynamic editing
	printf("editing dynamic tables:\n");

	// initialize dynamic pointers
	int csec = pehdr->e_shnum;
	int cdynno = 0;

	// find the first dynamic table
	for (int sec=0; sec<pehdr->e_shnum; sec++)
	{
		if (pshdrs[sec].sh_type == SHT_DYNAMIC)
		{
			csec = sec;
			break;
		}
	}
	if (csec >= pehdr->e_shnum)
	{
		printf("no dynamic tables found.\n");
		return;
	}

	// pointers to dynamic tables
	char *p0 = pdyntbls[csec];
	char *pe = pdyntbls[csec]+pshdrs[csec].sh_size;

	// start interactive loop
	for (int done=0; !done; )
	{
		// get cmd from user
		printf("dynamic cmd: ");
		rmvnlgets(s);
		tokenize(s, " \t");
		char *pt = gettoken(1);

		// what is the command
		if (pt == NULL || *pt == '\0')
		{
			increment(cdynno, csec, p0, pe);
		}
		else if (*pt == 'n')
		{
			// show all section names
			printf("symbol table names:\n");
			for (int sec=0; sec<pehdr->e_shnum; sec++)
			{
				if (pshdrs[sec].sh_type != SHT_DYNAMIC)
					continue;
				printf("section %d: %s (%d)\n", 
					sec, pshstrtbl+pshdrs[sec].sh_name,
					pshdrs[sec].sh_name);
			}
		}
		else if (*pt == '?' || *pt == 'h')
		{
			printmenu();
		}
		else if (*pt == 'D')
		{
			dflag = !dflag;
			if (dflag)
				printf("demangle mode ON.\n");
			else
				printf("demangle mode OFF.\n");
		}
		else if (*pt == 'r')
		{
			review(cdynno, csec, p0, pe);
		}
		else if (*pt == 'u')
		{
			update(cdynno, csec, p0, pe);
		}
		else if (*pt == '+')
		{
			increment(cdynno, csec, p0, pe);
		}
		else if (*pt == '-')
		{
			decrement(cdynno, csec, p0, pe);
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

