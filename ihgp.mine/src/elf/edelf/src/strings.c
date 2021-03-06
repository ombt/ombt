// perform editing operations

// headers
#include "edelf.h"

// editing strings
void
readstrings(ElfObject &eo, char *s)
{
	eo.readehdr();
	eo.readshdrs();
	eo.readstrings();
	return;
}

static void
printmenu()
{
	printf("strings menu:\n");
	printf("? or h - show menu\n");
	printf("n - show all string tables section names\n");
	printf("r - review current string\n");
	printf("r * - review all strings in string tables\n");
	printf("r * <string> - review <string> in all string tables\n");
	printf("r <section> - review 1st string in <section> string table\n");
	printf("r <section> * - review all strings in <section> string table\n");
	printf("r <section> <string> - review <string> in <section> string table\n");
	printf("r <section#> - review 1st string in <section#> string table\n");
	printf("r <section#> * - review all strings in <section#> string table\n");
	printf("r <section#> <string> - review <string> in <section#> string table\n");
	printf("+ - next string\n");
	printf("- - previous string\n");
	printf("u - update current string in current string table\n");
	printf("q - quit\n");
	return;
}

static void
review(ElfObject &eo, int &cstrno, int &cstroff, int &csec, char *&p0, char *&pe)
{
	// get tokens from user stream
	char *pt2 = gettoken(2);
	char *pt3 = gettoken(3);

	// detemine user request
	if (pt2 == NULL)
	{
		// check current section and location
		if (eo.pshdr(csec).sh_type != SHT_STRTAB)
		{
			printf("current section is not a string table.\n");
			return;
		}
		if (cstroff < 0 || cstroff >= (pe-p0))
		{
			printf("current string offset is out-of-range.\n");
			return;
		}

		// show current string
		printf("section %d: %s (%d)\n", csec,
			eo.shdrnm(csec), eo.pshdr(csec).sh_name);
		printf("%d, %d: %s\n", cstrno, cstroff, p0+cstroff);
	}
	else if (*pt2 == '*')
	{
		// check if a string was given
		if (pt3 == NULL)
		{
			// show all strings
			for (csec=0; csec<eo.pehdr()->e_shnum; csec++)
			{
				// skip sections that are not string tables
				if (eo.pshdr(csec).sh_type != SHT_STRTAB)
					continue;
	
				// print section name
				printf("section %d: %s (%d)\n", csec,
					eo.shdrnm(csec), 
					eo.pshdr(csec).sh_name);
	
				// print strings in section
				int nullstrcnt = 0;
				cstroff = 0;
				cstrno = 0;
				p0 = eo.pshdrdata(csec);
				pe = eo.pshdrdata(csec)+
					eo.pshdr(csec).sh_size;
				for ( ;
				    (p0+cstroff) < pe; 
				     cstroff += strlen(p0+cstroff)+1, cstrno++)
				{
					if (*(p0+cstroff) == '\0')
					{
						nullstrcnt++;
						continue;
					}
					if (nullstrcnt > 0)
					{
						printf("===>>> skipped %d null-strings <<<===\n" ,
							nullstrcnt);
						nullstrcnt = 0;
					}
					printf("%d, %d: %s\n", 
					       cstrno, cstroff, p0+cstroff);
				}
			}
		}
		else
		{
			// show a specific string
			for (csec=0; csec<eo.pehdr()->e_shnum; csec++)
			{
				// skip sections that are not string tables
				if (eo.pshdr(csec).sh_type != SHT_STRTAB)
					continue;
	
				// print section name
				printf("section %d: %s (%d)\n", csec,
					eo.shdrnm(csec), 
					eo.pshdr(csec).sh_name);
	
				// print strings in section
				int nullstrcnt = 0;
				cstroff = 0;
				cstrno = 0;
				p0 = eo.pshdrdata(csec);
				pe = eo.pshdrdata(csec)+
					eo.pshdr(csec).sh_size;
				for ( ;
				    (p0+cstroff) < pe; 
				     cstroff += strlen(p0+cstroff)+1, cstrno++)
				{
					if (*(p0+cstroff) == '\0')
					{
						nullstrcnt++;
						continue;
					}
					if (nullstrcnt > 0)
					{
						printf("===>>> skipped %d null-strings <<<===\n" ,
							nullstrcnt);
						nullstrcnt = 0;
					}
					if (REequal(pt3, p0+cstroff))
					{
						char s[BUFSIZ];
						printf("%d, %d: %s\n", 
							cstrno, cstroff, 
							p0+cstroff);
						printf("next string? [n/y/cr=y] ");
						rmvnlgets(s);
						if (*s != 'y' && *s != '\0') 
							goto all_done;
					}
				}
			}
			all_done: ;
		}
	}
	else if (strlen(pt2) == strspn(pt2, "0123456789"))
	{
		// a section number was given
		int sec = MYatoi(pt2);
		if (sec < 0 || sec >= eo.pehdr()->e_shnum)
		{
			printf("out-of-range section number.\n");
			printf("minimum section number = 0\n");
			printf("maximum section number = %d\n",
				eo.pehdr()->e_shnum);
			return;
		}
		if (eo.pshdr(sec).sh_type != SHT_STRTAB)
		{
			printf("section is not a string table.\n");
			return;
		}
		csec = sec;
		cstrno = 0;
		cstroff = 0;
		p0 = eo.pshdrdata(csec);
		pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;

		// was a string given
		if (pt3 == NULL)
		{
			// show first string in table
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);
			printf("%d, %d: %s\n", cstrno, cstroff, p0+cstroff);
		}
		else if (*pt3 == '*')
		{
			// show all strings in section
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	
			// print strings in section
			int nullstrcnt = 0;
			cstroff = 0;
			cstrno = 0;
			p0 = eo.pshdrdata(csec);
			pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
			for ( ; (p0+cstroff) < pe; 
			     cstroff += strlen(p0+cstroff)+1, cstrno++)
			{
				if (*(p0+cstroff) == '\0')
				{
					nullstrcnt++;
					continue;
				}
				if (nullstrcnt > 0)
				{
					printf("===>>> skipped %d null-strings <<<===\n" ,
						nullstrcnt);
					nullstrcnt = 0;
				}
				printf("%d, %d: %s\n", 
				       cstrno, cstroff, p0+cstroff);
			}
		}
		else
		{
			// search for string within the section
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	
			// print strings in section
			int nullstrcnt = 0;
			cstroff = 0;
			cstrno = 0;
			p0 = eo.pshdrdata(csec);
			pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
			for ( ; (p0+cstroff) < pe; 
			     cstroff += strlen(p0+cstroff)+1, cstrno++)
			{
				if (*(p0+cstroff) == '\0')
				{
					nullstrcnt++;
					continue;
				}
				if (nullstrcnt > 0)
				{
					printf("===>>> skipped %d null-strings <<<===\n" ,
						nullstrcnt);
					nullstrcnt = 0;
				}
				if (REequal(pt3, p0+cstroff))
				{
					char s[BUFSIZ];
					printf("%d, %d: %s\n", 
						cstrno, cstroff, 
						p0+cstroff);
					printf("next string? [n/y/cr=y] ");
					rmvnlgets(s);
					if (*s != 'y' && *s != '\0') 
						break;
				}
			}
		}
	}
	else
	{
		// a section name was given
		int sec = nametosec(eo, pt2);
		if (sec == NOTOK)
		{
			printf("section not found.\n");
			return;
		}
		if (eo.pshdr(sec).sh_type != SHT_STRTAB)
		{
			printf("section is not a string table.\n");
			return;
		}
		csec = sec;
		cstrno = 0;
		cstroff = 0;
		p0 = eo.pshdrdata(csec);
		pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;

		// was a string given
		if (pt3 == NULL)
		{
			// show first string in table
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);
			printf("%d, %d: %s\n", cstrno, cstroff, p0+cstroff);
		}
		else if (*pt3 == '*')
		{
			// show all strings in section
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	
			// print strings in section
			int nullstrcnt = 0;
			cstroff = 0;
			cstrno = 0;
			p0 = eo.pshdrdata(csec);
			pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
			for ( ; (p0+cstroff) < pe; 
			     cstroff += strlen(p0+cstroff)+1, cstrno++)
			{
				if (*(p0+cstroff) == '\0')
				{
					nullstrcnt++;
					continue;
				}
				if (nullstrcnt > 0)
				{
					printf("===>>> skipped %d null-strings <<<===\n" ,
						nullstrcnt);
					nullstrcnt = 0;
				}
				printf("%d, %d: %s\n", 
				       cstrno, cstroff, p0+cstroff);
			}
		}
		else
		{
			// search for string within the section
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	
			// print strings in section
			int nullstrcnt = 0;
			cstroff = 0;
			cstrno = 0;
			p0 = eo.pshdrdata(csec);
			pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
			for ( ; (p0+cstroff) < pe; 
			     cstroff += strlen(p0+cstroff)+1, cstrno++)
			{
				if (*(p0+cstroff) == '\0')
				{
					nullstrcnt++;
					continue;
				}
				if (nullstrcnt > 0)
				{
					printf("===>>> skipped %d null-strings <<<===\n" ,
						nullstrcnt);
					nullstrcnt = 0;
				}
				if (REequal(pt3, p0+cstroff))
				{
					char s[BUFSIZ];
					printf("%d, %d: %s\n", 
						cstrno, cstroff, p0+cstroff);
					printf("next string? [n/y/cr=y] ");
					rmvnlgets(s);
					if (*s != 'y' && *s != '\0') 
						break;
				}
			}
		}
	}
	return;
}

static void
update(ElfObject &eo, int &cstrno, int &cstroff, int &csec, char *&p0, char *&pe)
{
	// check section and string
	if (csec < 0 || csec >= eo.pehdr()->e_shnum)
	{
		printf("invalid section number.\n");
		return;
	}
	if (eo.pshdr(csec).sh_type != SHT_STRTAB)
	{
		printf("section is not a string table.\n");
		return;
	}
	if (cstroff < 0 || cstroff >= (pe-p0))
	{
		printf("current string offset is out-of-range.\n");
		return;
	}

	// update string
	char s[BUFSIZ];
	printf("section %d: %s (%d)\n", csec,
		eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	printf("updating string %d, %d [cr=%s]: ", 
		cstrno, cstroff, p0+cstroff);
	rmvnlgets(s);
	if (*s != '\0')
	{
		// save new string
		int sz = strlen(p0+cstroff);
		strncpy(p0+cstroff, s, sz);
		(p0+cstroff)[sz] = '\0';
		printf("new string %d, %d: %s\n", 
			cstrno, cstroff, p0+cstroff);

		// write to file
		printf("write to file [cr=n/n/y] ? ");
		rmvnlgets(s);
		if (*s == 'y')
		{
			// write out data
			eo.writestrings(csec);

			// refresh internal buffers
			*s = '\0';
			readstrings(eo, s);
		}
	}
	return;
}

static void
increment(ElfObject &eo, int &cstrno, int &cstroff, int &csec, char *&p0, char *&pe)
{
	// check section and string
	if (csec < 0 || csec >= eo.pehdr()->e_shnum)
	{
		printf("invalid section number.\n");
		return;
	}
	if (eo.pshdr(csec).sh_type != SHT_STRTAB)
	{
		printf("section is not a string table.\n");
		return;
	}
	if (cstroff < 0 || cstroff >= (pe-p0))
	{
		printf("current string offset is out-of-range.\n");
		return;
	}

	// check most common case
	int ncstroff = cstroff + strlen(p0+cstroff)+1;
	if ((p0+ncstroff) < pe)
	{
		cstroff = ncstroff;
		cstrno++;
		printf("%d, %d: %s\n", cstrno, cstroff, p0+cstroff);
		return;
	}
	
	// we are passed the end of the current section, look for
	// the next string table section, if any.
	//
	int ncsec = csec+1;
	for ( ; ncsec < eo.pehdr()->e_shnum; ncsec++)
	{
		if (eo.pshdr(ncsec).sh_type == SHT_STRTAB) break;
	}
	if (ncsec >= eo.pehdr()->e_shnum)
	{
		printf("no next string table found.\n");
		return;
	}

	// return the first string in the section
	cstrno = 0;
	cstroff = 0;
	csec = ncsec;
	p0 = eo.pshdrdata(csec);
	pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
	printf("%d, %d: %s\n", cstrno, cstroff, p0+cstroff);
	return;
}

static void
decrement(ElfObject &eo, int &cstrno, int &cstroff, int &csec, char *&p0, char *&pe)
{
	// check section and string
	if (csec < 0 || csec >= eo.pehdr()->e_shnum)
	{
		printf("invalid section number.\n");
		return;
	}
	if (eo.pshdr(csec).sh_type != SHT_STRTAB)
	{
		printf("section is not a string table.\n");
		return;
	}
	if (cstroff < 0 || cstroff >= (pe-p0))
	{
		printf("current string offset is out-of-range.\n");
		return;
	}

	// skip any nulls before the current string
	for (cstroff--; cstroff >= 0 && *(p0+cstroff) == '\0'; 
		cstroff--, cstrno--) ;

	// check if we found something
	if (cstroff >= 0)
	{
		// look for beginning of string
		for (cstroff--; cstroff >= 0 && *(p0+cstroff) != '\0'; 
			cstroff--) ;
		cstroff++;
		printf("%d, %d: %s\n", cstrno, cstroff, p0+cstroff);
		return;
	}

	// we are before the start of the current section, look for
	// the previous string table section, if any.
	int ncsec = csec-1;
	for ( ; ncsec >= 0; ncsec--)
	{
		if (eo.pshdr(ncsec).sh_type == SHT_STRTAB) break;
	}
	if (ncsec < 0)
	{
		printf("no previous string table found.\n");
		return;
	}

	// return the last string in the section
	csec = ncsec;
	cstrno = 0;
	cstroff = 0;
	int oldcstrno = cstrno;
	int oldcstroff = cstroff;
	p0 = eo.pshdrdata(csec);
	pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
	for ( ; (p0+cstroff) < pe; cstroff += strlen(p0+cstroff)+1, cstrno++)
	{
		oldcstrno = cstrno;
		oldcstroff = cstroff;
	}
	cstrno = oldcstrno;
	cstroff = oldcstroff;
	printf("%d, %d: %s\n", cstrno, cstroff, p0+cstroff);
	return;
}

void
editstrings(ElfObject &eo, char *)
{
	char s[BUFSIZ];

	// start of string editing
	printf("editing strings:\n");

	// initialize string pointers
	int csec = eo.pehdr()->e_shnum;
	int cstrno = 0;
	int cstroff = 0;

	// find the first string table
	for (int sec=0; sec<eo.pehdr()->e_shnum; sec++)
	{
		if (eo.pshdr(sec).sh_type == SHT_STRTAB)
		{
			csec = sec;
			break;
		}
	}
	if (csec >= eo.pehdr()->e_shnum)
	{
		printf("no string tables found.\n");
		return;
	}

	// pointers to string tables
	char *p0 = eo.pshdrdata(csec);
	char *pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;

	// start interactive loop
	for (int done=0; !done; )
	{
		// get cmd from user
		printf("strings cmd: ");
		rmvnlgets(s);
		tokenize(s, " \t");
		char *pt = gettoken(1);

		// what is the command
		if (pt == NULL || *pt == '\0')
		{
			// printf("unknown cmd.\n");
			increment(eo, cstrno, cstroff, csec, p0, pe);
		}
		else if (*pt == 'n')
		{
			// show all section names
			printf("string table names:\n");
			for (int sec=0; sec<eo.pehdr()->e_shnum; sec++)
			{
				if (eo.pshdr(sec).sh_type != SHT_STRTAB)
					continue;
				printf("section %d: %s (%d)\n", sec, 
					eo.shdrnm(sec), 
					eo.pshdr(sec).sh_name);
			}
		}
		else if (*pt == '?' || *pt == 'h')
		{
			printmenu();
		}
		else if (*pt == 'r')
		{
			review(eo, cstrno, cstroff, csec, p0, pe);
		}
		else if (*pt == 'u')
		{
			update(eo, cstrno, cstroff, csec, p0, pe);
		}
		else if (*pt == '+')
		{
			increment(eo, cstrno, cstroff, csec, p0, pe);
		}
		else if (*pt == '-')
		{
			decrement(eo, cstrno, cstroff, csec, p0, pe);
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

