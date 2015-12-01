// perform editing operations

// headers
#include "edelf.h"
#include "trans.h"

// dynamic table entries
void
readdynamic(ElfObject &eo, char *s)
{
	eo.readehdr();
	eo.readshdrs();
	eo.readstrings();
	eo.readdynamic();
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
showentry(ElfObject &eo, int &cdynno, int &csec, char *&p0, char *&pe)
{
	Elf32_Dyn *pdyn = (Elf32_Dyn *)(p0+cdynno*eo.pshdr(csec).sh_entsize);
	printf("%d: d_tag     : 0x%lx (%s)\n", 
		cdynno, pdyn->d_tag, ir2s(d_tag, rd_tag, pdyn->d_tag));
	printf("%d: d_un.d_val: 0x%lx\n", cdynno, pdyn->d_un.d_val);
	printf("%d: d_un.d_ptr: 0x%lx\n", cdynno, pdyn->d_un.d_ptr);
	printf("%d: d_un.d_off: 0x%lx\n", cdynno, pdyn->d_un.d_off);
	return;
}

static void
review(ElfObject &eo, int &cdynno, int &csec, char *&p0, char *&pe)
{
	// get tokens from user stream
	char *pt2 = gettoken(2);
	char *pt3 = gettoken(3);

	// determine user request
	if (pt2 == NULL)
	{
		// check current section and location
		if (eo.pshdr(csec).sh_type != SHT_DYNAMIC)
		{
			// initialize dynamic pointers
			csec = eo.pehdr()->e_shnum;
			cdynno = 0;
			for (int sec=0; sec<eo.pehdr()->e_shnum; sec++)
			{
				if (eo.pshdr(sec).sh_type == SHT_DYNAMIC)
				{
					csec = sec;
					break;
				}
			}
			if (csec >= eo.pehdr()->e_shnum)
			{
				printf("no dynamic tables found.\n");
				return;
			}
		}
		if (cdynno < 0 || 
		    cdynno*eo.pshdr(csec).sh_entsize > eo.pshdr(csec).sh_size)
		{
			printf("entry number is out of range.\n");
			return;
		}

		// show current entry
		showentry(eo, cdynno, csec, p0, pe);
	}
	else if (*pt2 == '*')
	{
		// show all entries
		for (csec=0; csec<eo.pehdr()->e_shnum; csec++)
		{
			// skip sections that are not symbol tables
			if (eo.pshdr(csec).sh_type != SHT_DYNAMIC)
				continue;
	
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);

			// print entries in section
			p0 = eo.pshdrdata(csec);
			pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
			for (cdynno=0; 
			    (p0+cdynno*eo.pshdr(csec).sh_entsize)<pe;
			     cdynno++)
			{
				Elf32_Dyn *pdyn = (Elf32_Dyn *)(p0+
					cdynno*eo.pshdr(csec).sh_entsize);
				if (pdyn->d_tag == DT_NULL) break;
				showentry(eo, cdynno, csec, p0, pe);
			}
		}
	}
	else if (strlen(pt2) == strspn(pt2, "0123456789"))
	{
		// check current section and location
		if (eo.pshdr(csec).sh_type != SHT_DYNAMIC)
		{
			printf("current section is not a dynamic table.\n");
			return;
		}
		cdynno = MYatoi(pt2);
		if (cdynno < 0 || 
		    cdynno*eo.pshdr(csec).sh_entsize > eo.pshdr(csec).sh_size)
		{
			printf("entry number is out of range.\n");
			return;
		}

		// show current entry
		showentry(eo, cdynno, csec, p0, pe);
	}
	else
	{
		printf("unknown command.\n");
	}
	return;
}

static void
update(ElfObject &eo, int &cdynno, int &csec, char *&p0, char *&pe)
{
	int upd = 0;
	char s[BUFSIZ];

	// check section and entry
	if (csec < 0 || csec >= eo.pehdr()->e_shnum)
	{
		printf("invalid section number.\n");
		return;
	}
	if (eo.pshdr(csec).sh_type != SHT_DYNAMIC)
	{
		printf("section is not a dynamic table.\n");
		return;
	}
	if (cdynno < 0 || 
	    cdynno*eo.pshdr(csec).sh_entsize > eo.pshdr(csec).sh_size)
	{
		printf("current entry number is out-of-range.\n");
		return;
	}
	MustBeTrue(eo.pshdrdata(csec) == p0);

	// update data
	Elf32_Dyn *pdyn = (Elf32_Dyn *)(p0+cdynno*eo.pshdr(csec).sh_entsize);
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
			eo.writedynamic(csec);
		}

		// reread data
		char dummy[1];
		readdynamic(eo, dummy);
	}
	return;
}

static void
increment(ElfObject &eo, int &cdynno, int &csec, char *&p0, char *&pe)
{
	// check section and symbol
	if (csec < 0 || csec >= eo.pehdr()->e_shnum)
	{
		printf("invalid section number.\n");
		return;
	}
	if (eo.pshdr(csec).sh_type != SHT_DYNAMIC)
	{
		printf("section is not a dynamic table.\n");
		return;
	}
	if (cdynno < 0 || cdynno*eo.pshdr(csec).sh_entsize > eo.pshdr(csec).sh_size)
	{
		printf("current entry number is out-of-range.\n");
		return;
	}

	// check most common case
	if ((cdynno+1)*eo.pshdr(csec).sh_entsize < eo.pshdr(csec).sh_size)
	{
		cdynno++;
		showentry(eo, cdynno, csec, p0, pe);
		return;
	}
	
	// we are passed the end of the current section, look for
	// the next dynamic table section, if any.
	//
	int ncsec = csec+1;
	for ( ; ncsec < eo.pehdr()->e_shnum; ncsec++)
	{
		if (eo.pshdr(ncsec).sh_type == SHT_DYNAMIC)
			break;
	}
	if (ncsec >= eo.pehdr()->e_shnum)
	{
		printf("no next dynamic table found.\n");
		return;
	}

	// return the first entry in the section
	cdynno = 0;
	csec = ncsec;
	p0 = eo.pshdrdata(csec);
	pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
	showentry(eo, cdynno, csec, p0, pe);
	return;
	return;
}

static void
decrement(ElfObject &eo, int &cdynno, int &csec, char *&p0, char *&pe)
{
	// check section and symbol
	if (csec < 0 || csec >= eo.pehdr()->e_shnum)
	{
		printf("invalid section number.\n");
		return;
	}
	if (eo.pshdr(csec).sh_type != SHT_DYNAMIC)
	{
		printf("section is not a dynamic table.\n");
		return;
	}
	if (cdynno < 0 || 
	    cdynno*eo.pshdr(csec).sh_entsize > eo.pshdr(csec).sh_size)
	{
		printf("current entry number is out-of-range.\n");
		return;
	}

	// check most common case
	if ((cdynno-1) >= 0)
	{
		cdynno--;
		showentry(eo, cdynno, csec, p0, pe);
		return;
	}
	
	// we are passed the end of the current section, look for
	// the previous dynamic table section, if any.
	//
	int ncsec = csec-1;
	for ( ; ncsec >= 0 ; ncsec--)
	{
		if (eo.pshdr(ncsec).sh_type == SHT_DYNAMIC)
			break;
	}
	if (ncsec < 0)
	{
		printf("no previous dynamic table found.\n");
		return;
	}

	// return the last entry in the section
	csec = ncsec;
	p0 = eo.pshdrdata(csec);
	pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
	cdynno = eo.pshdr(csec).sh_size/eo.pshdr(csec).sh_entsize;
	showentry(eo, cdynno, csec, p0, pe);
	return;
}

void
editdynamic(ElfObject &eo, char *)
{
	char s[BUFSIZ];

	// start of dynamic editing
	printf("editing dynamic tables:\n");

	// initialize dynamic pointers
	int csec = eo.pehdr()->e_shnum;
	int cdynno = 0;

	// find the first dynamic table
	for (int sec=0; sec<eo.pehdr()->e_shnum; sec++)
	{
		if (eo.pshdr(sec).sh_type == SHT_DYNAMIC)
		{
			csec = sec;
			break;
		}
	}
	if (csec >= eo.pehdr()->e_shnum)
	{
		printf("no dynamic tables found.\n");
		return;
	}

	// pointers to dynamic tables
	char *p0 = eo.pshdrdata(csec);
	char *pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;

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
			increment(eo, cdynno, csec, p0, pe);
		}
		else if (*pt == 'n')
		{
			// show all section names
			printf("symbol table names:\n");
			for (int sec=0; sec<eo.pehdr()->e_shnum; sec++)
			{
				if (eo.pshdr(sec).sh_type != SHT_DYNAMIC)
					continue;
				printf("section %d: %s (%d)\n", 
					sec, eo.shdrnm(sec), 
					eo.pshdr(sec).sh_name);
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
			review(eo, cdynno, csec, p0, pe);
		}
		else if (*pt == 'u')
		{
			update(eo, cdynno, csec, p0, pe);
		}
		else if (*pt == '+')
		{
			increment(eo, cdynno, csec, p0, pe);
		}
		else if (*pt == '-')
		{
			decrement(eo, cdynno, csec, p0, pe);
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

