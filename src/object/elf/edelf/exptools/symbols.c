// perform editing operations

// headers
#include "edelf.h"
#include "trans.h"

// editing symbols
void
readsymbols(ElfObject &eo, char *s)
{
	eo.readehdr();
	eo.readshdrs();
	eo.readstrings();
	eo.readsymbols();
	return;
}

static void
printmenu()
{
	printf("symbols menu:\n");
	printf("? or h - show menu\n");
	printf("? t or h t - list types\n");
	printf("n - show all symbol tables section names\n");
	printf("r - review current symbol\n");
	printf("r * - review all symbols in symbol tables\n");
	printf("r * <symbol> - review <symbol> in all symbol tables\n");
	printf("rt * <type> - review <type> in all symbol tables\n");
	printf("r <section> - review 1st symbol in <section> symbol table\n");
	printf("r <section> * - review all symbols in <section> symbol table\n");
	printf("r <section> <symbol> - review <symbol> in <section> symbol table\n");
	printf("rt <section> <type> - review <type> in <section> symbol table\n");
	printf("r <section#> - review 1st symbol in <section#> symbol table\n");
	printf("r <section#> * - review all symbols in <section#> symbol table\n");
	printf("r <section#> <symbol> - review <symbol> in <section#> symbol table\n");
	printf("rt <section#> <type> - review <type> in <section#> symbol table\n");
	printf("+ - next symbol\n");
	printf("- - previous symbol\n");
	printf("u - update current symbol in current symbol table\n");
	printf("D - toggle demangle mode\n");
	printf("q - quit\n");
	return;
}

void
showsymbolname(ElfObject &eo, int &csymno, int &csec, char *&p0, char *&pe)
{
	Elf32_Sym *psym = (Elf32_Sym *)(p0+csymno*eo.pshdr(csec).sh_entsize);
	char *psymnm = eo.pshdrdata(eo.pshdr(csec).sh_link)+psym->st_name;
	printf("%d: st_name : %s (%lx)\n", csymno, 
		mydemangle(psymnm), psym->st_name);
	return;
}

static void
listtypes()
{
	printf("\nsupported symbol types:\n");
	for (int i=0; st_type[i].data != NULL; i++)
	{
		printf("%-12s = %ld\n", st_type[i].data, n_type[i].key);
	}
	return;
}

void
showsymbol(ElfObject &eo, int &csymno, int &csec, char *&p0, char *&pe)
{
	Elf32_Sym *psym = (Elf32_Sym *)(p0+csymno*eo.pshdr(csec).sh_entsize);
	char *psymnm = eo.pshdrdata(eo.pshdr(csec).sh_link)+psym->st_name;
	printf("%d: st_name : %s (%lx)\n", csymno, 
		mydemangle(psymnm), psym->st_name);
	printf("%d: st_value: 0x%lx\n", 
		csymno, psym->st_value);
	printf("%d: st_size : 0x%lx\n", 
		csymno, psym->st_size);
	printf("%d: st_bind : 0x%x (%s)\n", 
		csymno, ELF32_ST_BIND(psym->st_info),
		ir2s(st_bind, rst_bind, ELF32_ST_BIND(psym->st_info)));
	printf("%d: st_type : 0x%x (%s)\n", 
		csymno, ELF32_ST_TYPE(psym->st_info),
		ir2s(st_type, rst_type, ELF32_ST_TYPE(psym->st_info)));
	printf("%d: st_other: 0x%x\n", 
		csymno, psym->st_other);
	printf("%d: st_shndx: 0x%x (%s)\n", 
		csymno, psym->st_shndx,
		ir2s(st_shndx, rst_shndx, psym->st_shndx));
	return;
}

static void
review_type(ElfObject &eo, int &csymno, int &csec, char *&p0, char *&pe)
{
	char buf;
	char save_pt3[BUFSIZ];
	*save_pt3 = '\0';

	// get tokens from user stream
	char *pt1 = gettoken(1);
	char *pt2 = gettoken(2);
	char *pt3 = gettoken(3);

	// determine user request
	if (pt2 == NULL)
	{
		printf("unknown 'rt' cmd.\n");
		return;
	}
	else if (*pt2 == '*')
	{
		// check if a string was given
		if (pt3 == NULL || *pt3 == '\0')
		{
			printf("unknown 'rt' cmd.\n");
			return;
		}
		else
		{
			long search_type = -1;

			// get type, either a number or a mnemonic.
			if ((strncmp(pt3, "0x", 2) == 0) ||
			    (strncmp(pt3, "0X", 2) == 0))
			{
				search_type = strtol(pt3, NULL, 16);
			}
			else if (strlen(pt3) == strspn(pt3, "0123456789"))
			{
				search_type = strtol(pt3, NULL, 10);
			}
			else
			{
				strcpy(save_pt3, pt3);
				search_type = s2i(st_type, pt3);
				if (search_type == NOTOK)
				{
					printf("unknown type: %s.\n", save_pt3);
					return;
				}
			}

			// show a specific symbol, search by type
			for (csec=0; csec<eo.pehdr()->e_shnum; csec++)
			{
				// skip sections that are not symbol tables
				if (eo.pshdr(csec).sh_type != SHT_SYMTAB &&
				    eo.pshdr(csec).sh_type != SHT_DYNSYM)
					continue;
	
				// print section name
				printf("section %d: %s (%d)\n", csec,
					eo.shdrnm(csec), 
					eo.pshdr(csec).sh_name);
	
				// print symbols in section
				p0 = eo.pshdrdata(csec);
				pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
				char *pstr = eo.pshdrdata(eo.pshdr(csec).sh_link);
				for (csymno=0; 
				    (p0+csymno*eo.pshdr(csec).sh_entsize)<pe;
				     csymno++)
				{
					Elf32_Sym *psym = (Elf32_Sym *)(p0+
						csymno*eo.pshdr(csec).sh_entsize);
					if (ELF32_ST_TYPE(
						psym->st_info)!=search_type)
						continue;
					showsymbol(eo, csymno, csec, p0, pe);
					char s[BUFSIZ];
					printf("next symbol? [n/y/cr=y] ");
					rmvnlgets(s);
					if (*s != 'y' && *s != '\0')
						goto all_done2;
				}
			}
			all_done2: ;
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
		if (eo.pshdr(csec).sh_type != SHT_SYMTAB &&
		    eo.pshdr(csec).sh_type != SHT_DYNSYM)
		{
			printf("section is not a symbol table.\n");
			return;
		}
		csec = sec;
		csymno = 0;
		p0 = eo.pshdrdata(csec);
		pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;

		// was a string given
		if (pt3 == NULL)
		{
			printf("unknown 'rt' cmd.\n");
			return;
		}
		else 
		{
			long search_type = -1;

			// get type, either a number or a mnemonic.
			if ((strncmp(pt3, "0x", 2) == 0) ||
			    (strncmp(pt3, "0X", 2) == 0))
			{
				search_type = strtol(pt3, NULL, 16);
			}
			else if (strlen(pt3) == strspn(pt3, "0123456789"))
			{
				search_type = strtol(pt3, NULL, 10);
			}
			else
			{
				strcpy(save_pt3, pt3);
				search_type = s2i(st_type, pt3);
				if (search_type == NOTOK)
				{
					printf("unknown type: %s.\n", save_pt3);
					return;
				}
			}

			// search for symbol within the section
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	
			// print symbol in section
			for (csymno=0; 
			    (p0+csymno*eo.pshdr(csec).sh_entsize)<pe; csymno++)
			{
				Elf32_Sym *psym = (Elf32_Sym *)(p0+
					csymno*eo.pshdr(csec).sh_entsize);
				if (ELF32_ST_TYPE(psym->st_info)!=search_type)
					continue;
				showsymbol(eo, csymno, csec, p0, pe);
				char s[BUFSIZ];
				printf("next symbol? [n/y/cr=y] ");
				rmvnlgets(s);
				if (*s != 'y' && *s != '\0')
					break;
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
		if (eo.pshdr(csec).sh_type != SHT_SYMTAB &&
		    eo.pshdr(csec).sh_type != SHT_DYNSYM)
		{
			printf("section is not a symbol table.\n");
			return;
		}
		csec = sec;
		csymno = 0;
		p0 = eo.pshdrdata(csec);
		pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;

		// was a string given
		if (pt3 == NULL || *pt3 == '\0')
		{
			printf("unknown 'rt' cmd.\n");
			return;
		}
		else 
		{
			long search_type = -1;

			// get type, either a number or a mnemonic.
			if ((strncmp(pt3, "0x", 2) == 0) ||
			    (strncmp(pt3, "0X", 2) == 0))
			{
				search_type = strtol(pt3, NULL, 16);
			}
			else if (strlen(pt3) == strspn(pt3, "0123456789"))
			{
				search_type = strtol(pt3, NULL, 10);
			}
			else
			{
				strcpy(save_pt3, pt3);
				search_type = s2i(st_type, pt3);
				if (search_type == NOTOK)
				{
					printf("unknown type: %s.\n", save_pt3);
					return;
				}
			}
			// search for symbol within the section
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	
			// print symbol in section
			for (csymno=0; 
			    (p0+csymno*eo.pshdr(csec).sh_entsize)<pe; csymno++)
			{
				Elf32_Sym *psym = (Elf32_Sym *)(p0+
					csymno*eo.pshdr(csec).sh_entsize);
				if (ELF32_ST_TYPE(psym->st_info)!=search_type) 
					continue;
				showsymbol(eo, csymno, csec, p0, pe);
				char s[BUFSIZ];
				printf("next symbol? [n/y/cr=y] ");
				rmvnlgets(s);
				if (*s != 'y' && *s != '\0')
					break;
			}
		}
	}
	return;
}

static void
review_other(ElfObject &eo, int &csymno, int &csec, char *&p0, char *&pe)
{
	// get tokens from user stream
	char *pt2 = gettoken(2);
	char *pt3 = gettoken(3);

	// determine user request
	if (pt2 == NULL)
	{
		// check current section and location
		if (eo.pshdr(csec).sh_type != SHT_SYMTAB &&
		    eo.pshdr(csec).sh_type != SHT_DYNSYM)
		{
			printf("current section is not a symbol table.\n");
			return;
		}
		if (csymno < 0 || 
		    csymno*eo.pshdr(csec).sh_entsize > eo.pshdr(csec).sh_size)
		{
			printf("symbol number is out of range.\n");
			return;
		}

		// show current symbol
		showsymbol(eo, csymno, csec, p0, pe);
	}
	else if (*pt2 == '*')
	{
		// check if a string was given
		if (pt3 == NULL)
		{
			// show all symbols
			for (csec=0; csec<eo.pehdr()->e_shnum; csec++)
			{
				// skip sections that are not symbol tables
				if (eo.pshdr(csec).sh_type != SHT_SYMTAB &&
				    eo.pshdr(csec).sh_type != SHT_DYNSYM)
					continue;
	
				// print section name
				printf("section %d: %s (%d)\n", csec,
					eo.shdrnm(csec), 
					eo.pshdr(csec).sh_name);
	
				// print symbols in section
				p0 = eo.pshdrdata(csec);
				pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
				char *pstr = eo.pshdrdata(eo.pshdr(csec).sh_link);
				for (csymno=0; 
				    (p0+csymno*eo.pshdr(csec).sh_entsize)<pe;
				     csymno++)
				{
					showsymbol(eo, csymno, csec, p0, pe);
				}
			}
		}
		else
		{
			// show a specific symbol
			for (csec=0; csec<eo.pehdr()->e_shnum; csec++)
			{
				// skip sections that are not symbol tables
				if (eo.pshdr(csec).sh_type != SHT_SYMTAB &&
				    eo.pshdr(csec).sh_type != SHT_DYNSYM)
					continue;
	
				// print section name
				printf("section %d: %s (%d)\n", csec,
					eo.shdrnm(csec), 
					eo.pshdr(csec).sh_name);
	
				// print symbols in section
				p0 = eo.pshdrdata(csec);
				pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
				char *pstr = eo.pshdrdata(eo.pshdr(csec).sh_link);
				for (csymno=0; 
				    (p0+csymno*eo.pshdr(csec).sh_entsize)<pe;
				     csymno++)
				{
					Elf32_Sym *psym = 
						(Elf32_Sym *)(p0 + 
						csymno*eo.pshdr(csec).sh_entsize);
					char *psymnm = pstr + psym->st_name;
					if (!REequal(pt3, mydemangle(psymnm)))
						continue;
					showsymbol(eo, csymno, csec, p0, pe);
					char s[BUFSIZ];
					printf("next symbol? [n/y/cr=y] ");
					rmvnlgets(s);
					if (*s != 'y' && *s != '\0')
						goto all_done;
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
		if (eo.pshdr(csec).sh_type != SHT_SYMTAB &&
		    eo.pshdr(csec).sh_type != SHT_DYNSYM)
		{
			printf("section is not a symbol table.\n");
			return;
		}
		csec = sec;
		csymno = 0;
		p0 = eo.pshdrdata(csec);
		pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;

		// was a string given
		if (pt3 == NULL)
		{
			// show first symbol in table
			showsymbol(eo, csymno, csec, p0, pe);
		}
		else if (*pt3 == '*')
		{
			// show all strings in section
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	
			// print symbols in section
			char *pstr = eo.pshdrdata(eo.pshdr(csec).sh_link);
			for (csymno=0; 
			    (p0+csymno*eo.pshdr(csec).sh_entsize)<pe; csymno++)
			{
				showsymbol(eo, csymno, csec, p0, pe);
			}
		}
		else
		{
			// search for symbol within the section
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	
			// print symbol in section
			char *pstr = eo.pshdrdata(eo.pshdr(csec).sh_link);
			for (csymno=0; 
			    (p0+csymno*eo.pshdr(csec).sh_entsize)<pe; csymno++)
			{
				Elf32_Sym *psym = (Elf32_Sym *)(p0 + 
					csymno*eo.pshdr(csec).sh_entsize);
				char *psymnm = pstr + psym->st_name;
				if (!REequal(pt3, mydemangle(psymnm)))
					continue;
				showsymbol(eo, csymno, csec, p0, pe);
				char s[BUFSIZ];
				printf("next string? [n/y/cr=y] ");
				rmvnlgets(s);
				if (*s != 'y' && *s != '\0')
					break;
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
		if (eo.pshdr(csec).sh_type != SHT_SYMTAB &&
		    eo.pshdr(csec).sh_type != SHT_DYNSYM)
		{
			printf("section is not a symbol table.\n");
			return;
		}
		csec = sec;
		csymno = 0;
		p0 = eo.pshdrdata(csec);
		pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;

		// was a string given
		if (pt3 == NULL)
		{
			// show first symbol in table
			showsymbol(eo, csymno, csec, p0, pe);
		}
		else if (*pt3 == '*')
		{
			// show all strings in section
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	
			// print symbols in section
			char *pstr = eo.pshdrdata(eo.pshdr(csec).sh_link);
			for (csymno=0; 
			    (p0+csymno*eo.pshdr(csec).sh_entsize)<pe; csymno++)
			{
				showsymbol(eo, csymno, csec, p0, pe);
			}
		}
		else
		{
			// search for symbol within the section
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	
			// print symbol in section
			char *pstr = eo.pshdrdata(eo.pshdr(csec).sh_link);
			for (csymno=0; 
			    (p0+csymno*eo.pshdr(csec).sh_entsize)<pe; csymno++)
			{
				Elf32_Sym *psym = (Elf32_Sym *)(p0 + 
					csymno*eo.pshdr(csec).sh_entsize);
				char *psymnm = pstr + psym->st_name;
				if (!REequal(pt3, mydemangle(psymnm)))
					continue;
				showsymbol(eo, csymno, csec, p0, pe);
				char s[BUFSIZ];
				printf("next string? [n/y/cr=y] ");
				rmvnlgets(s);
				if (*s != 'y' && *s != '\0')
					break;
			}
		}
	}
	return;
}

static void
review(ElfObject &eo, int &csymno, int &csec, char *&p0, char *&pe)
{
	// get tokens from user stream
	char *pt1 = gettoken(1);

	// check the type of review
	if (*(pt1+1) == 't')
	{
		review_type(eo, csymno, csec, p0, pe);
	}
	else if (*(pt1+1) == '\0')
	{
		review_other(eo, csymno, csec, p0, pe);
	}
	else
	{
		printf("unknown review type.\n");
	}
	return;
}

static void
update(ElfObject &eo, int &csymno, int &csec, char *&p0, char *&pe)
{
	// check section and symbol
	if (csec < 0 || csec >= eo.pehdr()->e_shnum)
	{
		printf("invalid section number.\n");
		return;
	}
	if (eo.pshdr(csec).sh_type != SHT_SYMTAB &&
	    eo.pshdr(csec).sh_type != SHT_DYNSYM)
	{
		printf("section is not a symbol table.\n");
		return;
	}
	if (csymno < 0 || csymno*eo.pshdr(csec).sh_entsize > eo.pshdr(csec).sh_size)
	{
		printf("current symbol number is out-of-range.\n");
		return;
	}
	MustBeTrue(eo.pshdrdata(csec) == p0);

	// update symbol
	int upd = 0;
	char s[BUFSIZ];
	printf("section %d: %s (%d)\n", csec,
		eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	Elf32_Sym *psym = (Elf32_Sym *)(p0 + csymno*eo.pshdr(csec).sh_entsize);
	char *pstr = eo.pshdrdata(eo.pshdr(csec).sh_link);
	char *psymnm = pstr + psym->st_name;
	printf("%d: st_name [cr=%s]: ", csymno, psymnm);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		int sz = strlen(psymnm);
		strncpy(psymnm, s, sz);
		psymnm[sz] = '\0';
		printf("st_name: %s\n", psymnm);
	}
	printf("%d: st_value [cr=0x%lx]: ", csymno, psym->st_value);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		psym->st_value= MYatoi(s);
		printf("st_value: 0x%lx\n", psym->st_value);
	}
	printf("%d: st_size [cr=0x%lx]: ", csymno, psym->st_size);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		psym->st_size= MYatoi(s);
		printf("st_size: 0x%lx\n", psym->st_size);
	}

	printf("%d: st_bind [cr=0x%x]: ", csymno, ELF32_ST_BIND(psym->st_info));
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		int type = ELF32_ST_TYPE(psym->st_info);
		int bind = MYatoi(s);
		psym->st_info = ELF32_ST_INFO(bind, type);
		printf("st_bind: 0x%lx\n", ELF32_ST_BIND(psym->st_info));
	}
	printf("%d: st_type [cr=0x%x]: ", csymno, ELF32_ST_TYPE(psym->st_info));
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		int bind = ELF32_ST_BIND(psym->st_info);
		int type = MYatoi(s);
		psym->st_info = ELF32_ST_INFO(bind, type);
		printf("st_type: 0x%lx\n", ELF32_ST_TYPE(psym->st_info));
	}

	printf("%d: st_other [cr=0x%x]: ", csymno, psym->st_other);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		psym->st_other= MYatoi(s);
		printf("st_other: 0x%lx\n", psym->st_other);
	}
	printf("%d: st_shndx: [cr=0x%x]: ", csymno, psym->st_shndx);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		psym->st_shndx= MYatoi(s);
		printf("st_shndx: 0x%lx\n", psym->st_shndx);
	}

	// write to file
	if (upd > 0)
	{
		printf("write to file [cr=n/n/y] ? ");
		rmvnlgets(s);
		if (*s == 'y')
		{
			eo.writesymbols(csec);
		}

		// reread data
		char dummy[1];
		readsymbols(eo, dummy);
	}
	return;
}

static void
increment(ElfObject &eo, int &csymno, int &csec, char *&p0, char *&pe)
{
	// check section and symbol
	if (csec < 0 || csec >= eo.pehdr()->e_shnum)
	{
		printf("invalid section number.\n");
		return;
	}
	if (eo.pshdr(csec).sh_type != SHT_SYMTAB &&
	    eo.pshdr(csec).sh_type != SHT_DYNSYM)
	{
		printf("section is not a symbol table.\n");
		return;
	}
	if (csymno < 0 || csymno*eo.pshdr(csec).sh_entsize > eo.pshdr(csec).sh_size)
	{
		printf("current symbol number is out-of-range.\n");
		return;
	}

	// check most common case
	if ((csymno+1)*eo.pshdr(csec).sh_entsize < eo.pshdr(csec).sh_size)
	{
		csymno++;
		showsymbol(eo, csymno, csec, p0, pe);
		return;
	}
	
	// we are passed the end of the current section, look for
	// the next symbol table section, if any.
	//
	int ncsec = csec+1;
	for ( ; ncsec < eo.pehdr()->e_shnum; ncsec++)
	{
		if (eo.pshdr(ncsec).sh_type == SHT_SYMTAB ||
		    eo.pshdr(ncsec).sh_type == SHT_DYNSYM)
			break;
	}
	if (ncsec >= eo.pehdr()->e_shnum)
	{
		printf("no next symbol table found.\n");
		return;
	}

	// return the first symbol in the section
	csymno = 0;
	csec = ncsec;
	p0 = eo.pshdrdata(csec);
	pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
	showsymbol(eo, csymno, csec, p0, pe);
	return;
}

static void
decrement(ElfObject &eo, int &csymno, int &csec, char *&p0, char *&pe)
{
	// check section and symbol
	if (csec < 0 || csec >= eo.pehdr()->e_shnum)
	{
		printf("invalid section number.\n");
		return;
	}
	if (eo.pshdr(csec).sh_type != SHT_SYMTAB &&
	    eo.pshdr(csec).sh_type != SHT_DYNSYM)
	{
		printf("section is not a symbol table.\n");
		return;
	}
	if (csymno < 0 || 
	    csymno*eo.pshdr(csec).sh_entsize > eo.pshdr(csec).sh_size)
	{
		printf("current symbol number is out-of-range.\n");
		return;
	}

	// check most common case
	if ((csymno-1) >= 0)
	{
		csymno--;
		showsymbol(eo, csymno, csec, p0, pe);
		return;
	}
	
	// we are passed the end of the current section, look for
	// the previous symbol table section, if any.
	//
	int ncsec = csec-1;
	for ( ; ncsec >= 0; ncsec--)
	{
		if (eo.pshdr(ncsec).sh_type == SHT_SYMTAB ||
		    eo.pshdr(ncsec).sh_type == SHT_DYNSYM)
			break;
	}
	if (ncsec < 0)
	{
		printf("no previous symbol table found.\n");
		return;
	}

	// return the last symbol in the section
	csec = ncsec;
	p0 = eo.pshdrdata(csec);
	pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
	csymno = eo.pshdr(csec).sh_size/eo.pshdr(csec).sh_entsize;
	showsymbol(eo, csymno, csec, p0, pe);
	return;
}

void
editsymbols(ElfObject &eo, char *)
{
	char s[BUFSIZ];

	// start of symbol editing
	printf("editing symbols:\n");

	// initialize symbol pointers
	int csec = eo.pehdr()->e_shnum;
	int csymno = 0;

	// find the first symbol table
	for (int sec=0; sec<eo.pehdr()->e_shnum; sec++)
	{
		if (eo.pshdr(sec).sh_type == SHT_SYMTAB ||
		    eo.pshdr(sec).sh_type == SHT_DYNSYM)
		{
			csec = sec;
			break;
		}
	}
	if (csec >= eo.pehdr()->e_shnum)
	{
		printf("no symbol tables found.\n");
		return;
	}

	// pointers to symbol tables
	char *p0 = eo.pshdrdata(csec);
	char *pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;

	// start interactive loop
	for (int done=0; !done; )
	{
		// get cmd from user
		printf("symbols cmd: ");
		rmvnlgets(s);
		tokenize(s, " \t");
		char *pt = gettoken(1);

		// what is the command
		if (pt == NULL || *pt == '\0')
		{
			// printf("unknown cmd.\n");
			increment(eo, csymno, csec, p0, pe);
		}
		else if (*pt == 'n')
		{
			// show all section names
			printf("symbol table names:\n");
			for (int sec=0; sec<eo.pehdr()->e_shnum; sec++)
			{
				if (eo.pshdr(sec).sh_type != SHT_SYMTAB &&
				    eo.pshdr(sec).sh_type != SHT_DYNSYM)
					continue;
				printf("section %d: %s (%d)\n", 
					sec, eo.shdrnm(sec), 
					eo.pshdr(sec).sh_name);
			}
		}
		else if (*pt == '?' || *pt == 'h')
		{
			char *pt2 = gettoken(2);
			if (pt2 == NULL || *pt2 == '\0')
				printmenu();
			else if (*pt2 == 't')
				listtypes();
			else
				printf("unknow help request.\n");
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
			review(eo, csymno, csec, p0, pe);
		}
		else if (*pt == 'u')
		{
			update(eo, csymno, csec, p0, pe);
		}
		else if (*pt == '+')
		{
			increment(eo, csymno, csec, p0, pe);
		}
		else if (*pt == '-')
		{
			decrement(eo, csymno, csec, p0, pe);
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

