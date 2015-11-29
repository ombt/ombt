// perform editing operations

// headers
#include "edelf.h"
#include "trans.h"

// editing relocation entries
void
readreloc(ElfObject &eo, char *s)
{
	eo.readehdr();
	eo.readshdrs();
	eo.readstrings();
	eo.readsymbols();
	eo.readreloc();
	return;
}

static void
printmenu()
{
	printf("relocation entries menu:\n");
	printf("? or h - show menu\n");
	printf("n - show all relcation tables section names\n");
	printf("r - review relocation entry\n");
	printf("r * - review all entries in relocation tables\n");
	printf("r * <symbol> - review <symbol> in all relocation tables\n");
	printf("r <section> - review 1st entry in <section> relocation table\n");
	printf("r <section> * - review all entries in <section> relocation table\n");
	printf("r <section> <symbol> - review <symbol> in <section> relocation table\n");
	printf("r <section> <entry#> - review <entry#> in <section> relocation table\n");
	printf("r <section#> - review 1st symbol in <section#> relocation table\n");
	printf("r <section#> * - review all entries in <section#> relocation table\n");
	printf("r <section#> <symbol> - review <symbol> in <section#> relocation table\n");
	printf("r <section#> <entry#> - review <entry#> in <section#> relocation table\n");
	printf("+ - next relocation entry\n");
	printf("- - previous relocation entry\n");
	printf("u - update current relocation entry\n");
	printf("D - toggle demangle mode\n");
	printf("q - quit\n");
	return;
}

static const char *
showtype(ElfObject &eo, int t)
{
	switch (eo.pehdr()->e_machine)
	{
	case EM_M32:
		return(i2s(r_type_M32, t));
	case EM_386:
		return(i2s(r_type_386, t));
	case EM_SPARC:
		return(i2s(r_type_SPARC, t));
#ifdef OS5_5
	case EM_PPC:
		return(i2s(r_type_PPC, t));
#endif
	default:
		return("unknown");
	}
}

static void
showreloc(ElfObject &eo, int &crelocno, int &csec, char *&p0, char *&pe)
{
	if (eo.pshdr(csec).sh_type == SHT_REL)
	{
		Elf32_Rel *prel = 
			(Elf32_Rel *)(p0 + crelocno*eo.pshdr(csec).sh_entsize);
		long type = ELF32_R_TYPE(prel->r_info);
		long sym = ELF32_R_SYM(prel->r_info);
		printf("%d: r_offset: 0x%lx\n", crelocno, prel->r_offset);
		printf("%d: r_type: 0x%lx (%s)\n", 
			crelocno, type, showtype(eo, type));
		printf("%d: r_sym: 0x%lx\n", crelocno, sym);
		if (sym != STN_UNDEF)
		{
			long symsec = eo.pshdr(csec).sh_link;
			MustBeTrue(eo.pshdr(symsec).sh_type == SHT_SYMTAB ||
				   eo.pshdr(symsec).sh_type == SHT_DYNSYM);
			Elf32_Sym *psym = 
				(Elf32_Sym *)(eo.pshdrdata(symsec) +
				sym*eo.pshdr(symsec).sh_entsize);
			char *psymnm = eo.pshdrdata(eo.pshdr(symsec).sh_link)+
					psym->st_name;
			printf("%d: r_name: %s (%lx)\n", crelocno, 
				mydemangle(psymnm), psym->st_name);
		}
		else
		{
			printf("%d: r_name: STN_UNDEF (0)\n", crelocno);
		}
	}
	else if (eo.pshdr(csec).sh_type == SHT_RELA)
	{
		Elf32_Rela *prela = 
			(Elf32_Rela *)(p0 + crelocno*eo.pshdr(csec).sh_entsize);
		long type = ELF32_R_TYPE(prela->r_info);
		long sym = ELF32_R_SYM(prela->r_info);
		printf("%d: r_offset: 0x%lx\n", crelocno, prela->r_offset);
		printf("%d: r_type: 0x%lx (%s)\n", 
			crelocno, type, showtype(eo, type));
		printf("%d: r_sym: 0x%lx\n", crelocno, sym);
		printf("%d: r_addend: 0x%lx\n", crelocno, prela->r_addend);
		if (sym != STN_UNDEF)
		{
			long symsec = eo.pshdr(csec).sh_link;
			MustBeTrue(eo.pshdr(symsec).sh_type == SHT_SYMTAB ||
				   eo.pshdr(symsec).sh_type == SHT_DYNSYM);
			Elf32_Sym *psym = 
				(Elf32_Sym *)(eo.pshdrdata(symsec) +
				sym*eo.pshdr(symsec).sh_entsize);
			char *psymnm = eo.pshdrdata(eo.pshdr(symsec).sh_link)+
					psym->st_name;
			printf("%d: r_name: %s (%lx)\n", crelocno, 
				mydemangle(psymnm), psym->st_name);
		}
		else
		{
			printf("%d: r_name: STN_UNDEF (0)\n", crelocno);
		}
	}
	else
	{
		MustBeTrue(eo.pshdr(csec).sh_type == SHT_REL ||
			   eo.pshdr(csec).sh_type == SHT_RELA);
	}
	return;
}

static void
showreloc(ElfObject &eo, int &crelocno, int &csec, char *&p0, char *&pe, 
		const char *re, int &done)
{
	// get section indices
	int symsec = eo.pshdr(csec).sh_link;
	int strsec = eo.pshdr(symsec).sh_link;

	// get symbol and string data
	char *psymtbl = eo.pshdrdata(symsec);
	char *pstrtbl = eo.pshdrdata(strsec);

	// relocation entry size
	int relentsz = eo.pshdr(csec).sh_entsize;
	int symentsz = eo.pshdr(symsec).sh_entsize;

	// start searching for the symbol
	for (crelocno=0; (p0+crelocno*relentsz)<pe; crelocno++)
	{
		if (eo.pshdr(csec).sh_type == SHT_REL)
		{
			Elf32_Rel *prel = 
				(Elf32_Rel *)(p0+crelocno*relentsz);
			long type = ELF32_R_TYPE(prel->r_info);
			long sym = ELF32_R_SYM(prel->r_info);
			if (sym == STN_UNDEF) 
				continue;
			Elf32_Sym *psym = (Elf32_Sym *)(psymtbl+sym*symentsz);
			char *psymnm = pstrtbl+psym->st_name;
			if (!REequal(re, psymnm)) 
				continue;
			printf("%d: r_offset: 0x%lx\n", 
				crelocno, prel->r_offset);
			printf("%d: r_type: 0x%lx (%s)\n", 
				crelocno, type, showtype(eo, type));
			printf("%d: r_sym: 0x%lx\n", 
				crelocno, sym);
			printf("%d: r_name: %s (%lx)\n", crelocno, 
				mydemangle(psymnm), psym->st_name);
			char s[BUFSIZ];
			printf("next symbol? [n/y/cr=y] ");
			rmvnlgets(s);
			if (*s != 'y' && *s != '\0')
			{
				done = 1;
				break;
			}
		}
		else if (eo.pshdr(csec).sh_type == SHT_RELA)
		{
			Elf32_Rela *prela = 
				(Elf32_Rela *)(p0+crelocno*relentsz);
			long type = ELF32_R_TYPE(prela->r_info);
			long sym = ELF32_R_SYM(prela->r_info);
			if (sym == STN_UNDEF) 
				continue;
			Elf32_Sym *psym = (Elf32_Sym *)(psymtbl+sym*symentsz);
			char *psymnm = pstrtbl+psym->st_name;
			if (!REequal(re, psymnm)) 
				continue;
			printf("%d: r_offset: 0x%lx\n", 
				crelocno, prela->r_offset);
			printf("%d: r_type: 0x%lx (%s)\n", 
				crelocno, type, showtype(eo, type));
			printf("%d: r_sym: 0x%lx\n", 
				crelocno, sym);
			printf("%d: r_name: %s (%lx)\n", crelocno, 
				mydemangle(psymnm), psym->st_name);
			printf("%d: r_addend: 0x%lx\n", 
				crelocno, prela->r_addend);
			char s[BUFSIZ];
			printf("next symbol? [n/y/cr=y] ");
			rmvnlgets(s);
			if (*s != 'y' && *s != '\0')
			{
				done = 1;
				break;
			}
		}
		else
		{
			MustBeTrue((eo.pshdr(csec).sh_type == SHT_REL) ||
				   (eo.pshdr(csec).sh_type == SHT_RELA));
		}
	}
	return;
}

static void
review(ElfObject &eo, int &crelocno, int &csec, char *&p0, char *&pe)
{
	// get tokens from user stream
	char *pt2 = gettoken(2);
	char *pt3 = gettoken(3);

	// determine user request
	if (pt2 == NULL)
	{
		// check current section and location
		if (eo.pshdr(csec).sh_type != SHT_REL &&
		    eo.pshdr(csec).sh_type != SHT_RELA)
		{
			printf("current section is not a relocation table.\n");
			return;
		}
		if (crelocno < 0 || 
		    crelocno*eo.pshdr(csec).sh_entsize > eo.pshdr(csec).sh_size)
		{
			printf("entry number is out of range.\n");
			return;
		}

		// show relocation entry
		showreloc(eo, crelocno, csec, p0, pe);
	}
	else if (*pt2 == '*')
	{
		// check if a string was given
		if (pt3 == NULL)
		{
			// show all relocation entries
			for (csec=0; csec<eo.pehdr()->e_shnum; csec++)
			{
				// skip sections that are not relocation tables
				if (eo.pshdr(csec).sh_type != SHT_REL &&
				    eo.pshdr(csec).sh_type != SHT_RELA)
					continue;
	
				// print section name
				printf("section %d: %s (%d)\n", csec,
					eo.shdrnm(csec),
					eo.pshdr(csec).sh_name);
	
				// print entries in section
				p0 = eo.pshdrdata(csec);
				pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
				for (crelocno=0; 
				    (p0+crelocno*eo.pshdr(csec).sh_entsize)<pe;
				     crelocno++)
				{
					showreloc(eo, crelocno, csec, p0, pe);
				}
			}
		}
		else if (strspn(pt3, "0123456789") == strlen(pt3))
		{
			// show a specific entry
			crelocno = MYatoi(pt3);
			for (csec=0; csec<eo.pehdr()->e_shnum; csec++)
			{
				// skip sections that are not relocation tables
				int sh_type = eo.pshdr(csec).sh_type;
				if (sh_type != SHT_REL && sh_type != SHT_RELA)
					continue;
	
				// print section name
				printf("section %d: %s (%d)\n", csec,
					eo.shdrnm(csec),
					eo.pshdr(csec).sh_name);
	
				// print entries in section
				p0 = eo.pshdrdata(csec);
				pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
				showreloc(eo, crelocno, csec, p0, pe);

				// all done ?
				char s[BUFSIZ];
				printf("next symbol? [n/y/cr=y] ");
				rmvnlgets(s);
				if (*s != 'y' && *s != '\0')
					break;
			}
		}
		else
		{
			// show a specific entry
			int done=0;
			for (csec=0; csec<eo.pehdr()->e_shnum; csec++)
			{
				// skip sections that are not relocation tables
				int sh_type = eo.pshdr(csec).sh_type;
				if (sh_type != SHT_REL && sh_type != SHT_RELA)
					continue;
	
				// print section name
				printf("section %d: %s (%d)\n", csec,
					eo.shdrnm(csec),
					eo.pshdr(csec).sh_name);
	
				// print entries in section
				p0 = eo.pshdrdata(csec);
				pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
				showreloc(eo, crelocno, csec, p0, pe, pt3, done);
				if (done) break;
			}
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
		if (eo.pshdr(sec).sh_type != SHT_REL &&
		    eo.pshdr(sec).sh_type != SHT_RELA)
		{
			printf("section is not a relocation table.\n");
			return;
		}
		csec = sec;
		crelocno = 0;
		p0 = eo.pshdrdata(csec);
		pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;

		// was a string given
		if (pt3 == NULL)
		{
			// show first entry in table
			showreloc(eo, crelocno, csec, p0, pe);
		}
		else if (*pt3 == '*')
		{
			// show all entries in section
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	
			// print entries in section
			for (crelocno=0; 
			    (p0+crelocno*eo.pshdr(csec).sh_entsize)<pe; 
			     crelocno++)
			{
				showreloc(eo, crelocno, csec, p0, pe);
			}
		}
		else if (strspn(pt3, "0123456789") == strlen(pt3))
		{
			// search for symbol within the section
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	
			// print entry in section
			crelocno = MYatoi(pt3);
			showreloc(eo, crelocno, csec, p0, pe);
		}
		else
		{
			// search for symbol within the section
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	
			// print entry in section
			int done = 0;
			showreloc(eo, crelocno, csec, p0, pe, pt3, done);
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
		if (eo.pshdr(sec).sh_type != SHT_REL &&
		    eo.pshdr(sec).sh_type != SHT_RELA)
		{
			printf("section is not a relocation table.\n");
			return;
		}
		csec = sec;
		crelocno = 0;
		p0 = eo.pshdrdata(csec);
		pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;

		// was a string given
		if (pt3 == NULL)
		{
			// show first entry in table
			showreloc(eo, crelocno, csec, p0, pe);
		}
		else if (*pt3 == '*')
		{
			// show all entries in section
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	
			// print entries in section
			for (crelocno=0; 
			    (p0+crelocno*eo.pshdr(csec).sh_entsize)<pe; 
			     crelocno++)
			{
				showreloc(eo, crelocno, csec, p0, pe);
			}
		}
		else if (strspn(pt3, "0123456789") == strlen(pt3))
		{
			// search for symbol within the section
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	
			// print entry in section
			crelocno = MYatoi(pt3);
			showreloc(eo, crelocno, csec, p0, pe);
		}
		else
		{
			// search for symbol within the section
			// print section name
			printf("section %d: %s (%d)\n", csec,
				eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	
			// print symbol in section
			int done = 0;
			showreloc(eo, crelocno, csec, p0, pe, pt3, done);
		}
	}
	return;
}

static void
update(ElfObject &eo, int &crelocno, int &csec, char *&p0, char *&pe)
{
	// check section and entry
	if (csec < 0 || csec >= eo.pehdr()->e_shnum)
	{
		printf("invalid section number.\n");
		return;
	}
	if (eo.pshdr(csec).sh_type != SHT_REL &&
	    eo.pshdr(csec).sh_type != SHT_RELA)
	{
		printf("section is not a relocation table.\n");
		return;
	}
	if (crelocno < 0 || 
	    crelocno*eo.pshdr(csec).sh_entsize > eo.pshdr(csec).sh_size)
	{
		printf("current entry number is out-of-range.\n");
		return;
	}
	MustBeTrue(eo.pshdrdata(csec) == p0);

	// update symbol
	int upd = 0;
	char s[BUFSIZ];
	printf("section %d: %s (%d)\n", csec,
		eo.shdrnm(csec), eo.pshdr(csec).sh_name);
	if (eo.pshdr(csec).sh_type == SHT_REL)
	{
		Elf32_Rel *prel = 
			(Elf32_Rel *)(p0+crelocno*eo.pshdr(csec).sh_entsize);
		long offset = prel->r_offset;
		long type = ELF32_R_TYPE(prel->r_info);
		long sym = ELF32_R_SYM(prel->r_info);
		printf("%d: r_offset [cr=0x%lx]: ", crelocno, prel->r_offset);
        	rmvnlgets(s);
        	if (*s != '\0')
        	{
                	upd++;
                	prel->r_offset= MYatoi(s);
                	printf("r_offset: 0x%lx\n", prel->r_offset);
		}
		printf("%d: r_type [cr=0x%lx]: ", crelocno, type);
        	rmvnlgets(s);
        	if (*s != '\0')
        	{
                	upd++;
                	type = MYatoi(s);
			prel->r_info = ELF32_R_INFO(sym, type);
                	printf("r_type: 0x%lx\n", ELF32_R_TYPE(prel->r_info));
		}
		printf("%d: r_sym [cr=0x%lx]: ", crelocno, sym);
        	rmvnlgets(s);
        	if (*s != '\0')
        	{
                	upd++;
                	sym = MYatoi(s);
			prel->r_info = ELF32_R_INFO(sym, type);
                	printf("r_sym: 0x%lx\n", ELF32_R_SYM(prel->r_info));
		}
	}
	else if (eo.pshdr(csec).sh_type == SHT_RELA)
	{
		Elf32_Rela *prela = 
			(Elf32_Rela *)(p0+crelocno*eo.pshdr(csec).sh_entsize);
		long offset = prela->r_offset;
		long type = ELF32_R_TYPE(prela->r_info);
		long sym = ELF32_R_SYM(prela->r_info);
		long addend = prela->r_addend;
		printf("%d: r_offset [cr=0x%lx]: ", crelocno, prela->r_offset);
        	rmvnlgets(s);
        	if (*s != '\0')
        	{
                	upd++;
                	prela->r_offset = MYatoi(s);
                	printf("r_offset: 0x%lx\n", prela->r_offset);
		}
		printf("%d: r_type [cr=0x%lx]: ", crelocno, type);
        	rmvnlgets(s);
        	if (*s != '\0')
        	{
                	upd++;
                	type = MYatoi(s);
			prela->r_info = ELF32_R_INFO(sym, type);
                	printf("r_type: 0x%lx\n", ELF32_R_TYPE(prela->r_info));
		}
		printf("%d: r_sym [cr=0x%lx]: ", crelocno, sym);
        	rmvnlgets(s);
        	if (*s != '\0')
        	{
                	upd++;
                	sym = MYatoi(s);
			prela->r_info = ELF32_R_INFO(sym, type);
                	printf("r_sym: 0x%lx\n", ELF32_R_SYM(prela->r_info));
		}
		printf("%d: r_addend [cr=0x%lx]: ", crelocno, prela->r_addend);
        	rmvnlgets(s);
        	if (*s != '\0')
        	{
                	upd++;
                	prela->r_addend = MYatoi(s);
                	printf("r_addend: 0x%lx\n", prela->r_addend);
		}
	}
	else
	{
		MustBeTrue((eo.pshdr(csec).sh_type == SHT_REL) ||
			   (eo.pshdr(csec).sh_type == SHT_RELA));
	}
		
	// write to file
	if (upd > 0)
	{
		printf("write to file [cr=n/n/y] ? ");
		rmvnlgets(s);
		if (*s == 'y')
		{
			eo.writereloc(csec);
		}

		// reread data
		char dummy[1];
		readreloc(eo, dummy);
	}
	return;
}

static void
increment(ElfObject &eo, int &crelocno, int &csec, char *&p0, char *&pe)
{
	// check section and entry
	if (csec < 0 || csec >= eo.pehdr()->e_shnum)
	{
		printf("invalid section number.\n");
		return;
	}
	if (eo.pshdr(csec).sh_type != SHT_REL &&
	    eo.pshdr(csec).sh_type != SHT_RELA)
	{
		printf("section is not a relocation table.\n");
		return;
	}
	if (crelocno < 0 || 
	    crelocno*eo.pshdr(csec).sh_entsize > eo.pshdr(csec).sh_size)
	{
		printf("current relocation number is out-of-range.\n");
		return;
	}

	// check most common case
	if ((crelocno+1)*eo.pshdr(csec).sh_entsize < eo.pshdr(csec).sh_size)
	{
		crelocno++;
		showreloc(eo, crelocno, csec, p0, pe);
		return;
	}
	
	// we are passed the end of the current section, look for
	// the next relocation table section, if any.
	//
	int ncsec = csec+1;
	for ( ; ncsec < eo.pehdr()->e_shnum; ncsec++)
	{
		if (eo.pshdr(ncsec).sh_type == SHT_REL ||
		    eo.pshdr(ncsec).sh_type == SHT_RELA)
			break;
	}
	if (ncsec >= eo.pehdr()->e_shnum)
	{
		printf("no next relocation table found.\n");
		return;
	}

	// return the first entry in the section
	crelocno = 0;
	csec = ncsec;
	p0 = eo.pshdrdata(csec);
	pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
	showreloc(eo, crelocno, csec, p0, pe);
	return;
}

static void
decrement(ElfObject &eo, int &crelocno, int &csec, char *&p0, char *&pe)
{
	// check section and entry
	if (csec < 0 || csec >= eo.pehdr()->e_shnum)
	{
		printf("invalid section number.\n");
		return;
	}
	if (eo.pshdr(csec).sh_type != SHT_REL &&
	    eo.pshdr(csec).sh_type != SHT_RELA)
	{
		printf("section is not a relocation table.\n");
		return;
	}
	if (crelocno < 0 || 
	    crelocno*eo.pshdr(csec).sh_entsize > eo.pshdr(csec).sh_size)
	{
		printf("current section number is out-of-range.\n");
		return;
	}

	// check most common case
	if ((crelocno-1) >= 0)
	{
		crelocno--;
		showreloc(eo, crelocno, csec, p0, pe);
		return;
	}
	
	// we are passed the end of the current section, look for
	// the previous relocation table section, if any.
	//
	int ncsec = csec-1;
	for ( ; ncsec >= 0; ncsec--)
	{
		if (eo.pshdr(ncsec).sh_type == SHT_REL ||
		    eo.pshdr(ncsec).sh_type == SHT_RELA)
			break;
	}
	if (ncsec < 0)
	{
		printf("no previous relocation table found.\n");
		return;
	}

	// return the last entry in the section
	csec = ncsec;
	p0 = eo.pshdrdata(csec);
	pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;
	crelocno = eo.pshdr(csec).sh_size/eo.pshdr(csec).sh_entsize;
	showreloc(eo, crelocno, csec, p0, pe);
	return;
}

void
editreloc(ElfObject &eo, char *)
{
	char s[BUFSIZ];

	// start of relocation editing
	printf("editing relocation entries:\n");

	// initialize pointers
	int csec = eo.pehdr()->e_shnum;
	int crelocno = 0;

	// find the first relocation table
	for (int sec=0; sec<eo.pehdr()->e_shnum; sec++)
	{
		if (eo.pshdr(sec).sh_type == SHT_REL ||
		    eo.pshdr(sec).sh_type == SHT_RELA)
		{
			csec = sec;
			break;
		}
	}
	if (csec >= eo.pehdr()->e_shnum)
	{
		printf("no relocation tables found.\n");
		return;
	}

	// pointers to relocation tables
	char *p0 = eo.pshdrdata(csec);
	char *pe = eo.pshdrdata(csec)+eo.pshdr(csec).sh_size;

	// start interactive loop
	for (int done=0; !done; )
	{
		// get cmd from user
		printf("relocation cmd: ");
		rmvnlgets(s);
		tokenize(s, " \t");
		char *pt = gettoken(1);

		// what is the command
		if (pt == NULL || *pt == '\0')
		{
			increment(eo, crelocno, csec, p0, pe);
		}
		else if (*pt == 'n')
		{
			// show all section names
			printf("relocation table names:\n");
			for (int sec=0; sec<eo.pehdr()->e_shnum; sec++)
			{
				if (eo.pshdr(sec).sh_type != SHT_REL &&
				    eo.pshdr(sec).sh_type != SHT_RELA)
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
		else if (*pt == 'r')
		{
			review(eo, crelocno, csec, p0, pe);
		}
		else if (*pt == 'D')
		{
			dflag = !dflag;
			if (dflag)
				printf("demangle mode ON.\n");
			else
				printf("demangle mode OFF.\n");
		}
		else if (*pt == 'u')
		{
			update(eo, crelocno, csec, p0, pe);
		}
		else if (*pt == '+')
		{
			increment(eo, crelocno, csec, p0, pe);
		}
		else if (*pt == '-')
		{
			decrement(eo, crelocno, csec, p0, pe);
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
