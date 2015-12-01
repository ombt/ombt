// perform editing operations

// headers
#include "edelf.h"
#include "trans.h"

// editing hash tables
void
readhash(ElfObject &eo, char *s)
{
	eo.readehdr();
	eo.readshdrs();
	eo.readstrings();
	eo.readsymbols();
	eo.readhash();
	return;
}

static void
printmenu()
{
	printf("hash table menu:\n");
	printf("? or h - show menu\n");
	printf("n - show all hash tables section names\n");
	printf("r - review current symbol\n");
	printf("r * - review all symbols in hash tables\n");
	printf("r * <symbol> - review <symbol> in hash tables\n");
	printf("r <section> - review first symbol in <section> hash table\n");
	printf("r <section> * - review all symbols in <section> hash table\n");
	printf("r <section> <symbol> - review <symbol> in <section> hash table\n");
	printf("r <section#> - review first symbol in <section#> hash table\n");
	printf("r <section#> * - review all symbols in <section#> hash table\n");
	printf("r <section#> <symbol> - review <symbol> in <section#> hash table\n");
	printf("+ - next symbol\n");
	printf("u - update current symbol in current hash table\n");
	printf("D - toggle demangle mode\n");
	printf("q - quit\n");
	return;
}

static void
showhash(ElfObject &eo, int &cchain, int &cbucket, int &csec, Elf32_Word *&p0, Elf32_Word *&pe)
{
	int symsec = eo.pshdr(csec).sh_link;
	int strsec = eo.pshdr(symsec).sh_link;
	Elf32_Sym *psym = (Elf32_Sym *)
		(eo.pshdrdata(symsec) + cchain*eo.pshdr(symsec).sh_entsize);
	char *psymnm = eo.pshdrdata(strsec) + psym->st_name;
	printf("bucket: %d chain: %d symbol: %s (%lx)\n", 
		cbucket, cchain, mydemangle(psymnm), psym->st_name);
	return;
}

static void
showallhash(ElfObject &eo, int &cchain, int &cbucket, int &csec, Elf32_Word *&p0, Elf32_Word *&pe)
{
	Elf32_Word *phash = p0 + 2;
	Elf32_Word *pchain = phash + MAXBUCKET(eo, csec);
	int maxb = MAXBUCKET(eo, csec);
	for (cbucket=0; cbucket < maxb; cbucket++)
	{
		if (phash[cbucket] == STN_UNDEF) continue;
		for (cchain = phash[cbucket]; cchain != STN_UNDEF; 
		     cchain = pchain[cchain])
		{
			showhash(eo, cchain, cbucket, csec, p0, pe);
		}
	}
	return;
}

static int
showreqhash(ElfObject &eo, int &cchain, int &cbucket, int &csec, 
	Elf32_Word *&p0, Elf32_Word *&pe, const char *ps)
{
	int symsec = eo.pshdr(csec).sh_link;
	int strsec = eo.pshdr(symsec).sh_link;
	Elf32_Word *phash = p0 + 2;
	Elf32_Word *pchain = phash + MAXBUCKET(eo, csec);
	int maxb = MAXBUCKET(eo, csec);
	for (cbucket=0; cbucket < maxb; cbucket++)
	{
		if (phash[cbucket] == STN_UNDEF) 
			continue;
		for (cchain = phash[cbucket]; cchain != STN_UNDEF; 
		     cchain = pchain[cchain])
		{
			Elf32_Sym *psym = (Elf32_Sym *)(eo.pshdrdata(symsec) + 
				cchain*eo.pshdr(symsec).sh_entsize);
			char *psymnm = eo.pshdrdata(strsec) + psym->st_name;
			if (!REequal(ps, mydemangle(psymnm))) 
				continue;
			printf("bucket: %d chain: %d symbol: %s (%lx)\n", 
				cbucket, cchain, mydemangle(psymnm), 
			psym->st_name);
			char s[BUFSIZ];
			printf("next symbol? [n/y/cr=y] ");
			rmvnlgets(s);
			if (*s != 'y' && *s != '\0')
				return(1);
		}
	}
	return(0);
}

static void
review(ElfObject &eo, int &cchain, int &cbucket, int &csec, Elf32_Word *&p0, Elf32_Word *&pe)
{
	// get tokens from user stream
	char *pt2 = gettoken(2);
	char *pt3 = gettoken(3);

	// determine user request
	if (pt2 == NULL)
	{
		// check section and symbol
		if (csec < 0 || csec >= eo.pehdr()->e_shnum)
		{
			printf("invalid section number.\n");
			return;
		}
		if (eo.pshdr(csec).sh_type != SHT_HASH)
		{
			printf("section is not a hash table.\n");
			return;
		}
		if (cbucket < 0 || cbucket > MAXBUCKET(eo, csec))
		{
			printf("bucket is out-of-range.\n");
			return;
		}
		if (cchain < 0 || cchain > MAXCHAIN(eo, csec))
		{
			printf("chain is out-of-range.\n");
			return;
		}
		showhash(eo, cchain, cbucket, csec, p0, pe);
	}
	else if (*pt2 == '*')
	{
		if (pt3 == NULL)
		{
			// show all symbols
			for (csec=0; csec<eo.pehdr()->e_shnum; csec++)
			{
				if (eo.pshdr(csec).sh_type != SHT_HASH)
					continue;
				printf("section %d: %s (%d) nb: %ld, nch: %ld\n", 
					csec, eo.shdrnm(csec),
					eo.pshdr(csec).sh_name,
					MAXBUCKET(eo, csec), MAXCHAIN(eo, csec));
				p0 = eo.phashtbls(csec);
				pe = eo.phashtbls(csec) + 
					eo.pshdr(csec).sh_size/sizeof(Elf32_Word);
				cbucket = 0;
				cchain = eo.phashtbls(csec)[cbucket+2];
				showallhash(eo, cchain, cbucket, csec, p0, pe);
			}
		}
		else
		{
			// show requested symbol
			for (csec=0; csec<eo.pehdr()->e_shnum; csec++)
			{
				if (eo.pshdr(csec).sh_type != SHT_HASH)
					continue;
				printf("section %d: %s (%d) nb: %ld, nch: %ld\n", 
					csec, eo.shdrnm(csec),
					eo.pshdr(csec).sh_name,
					MAXBUCKET(eo, csec), MAXCHAIN(eo, csec));
				p0 = eo.phashtbls(csec);
				pe = eo.phashtbls(csec) + 
					eo.pshdr(csec).sh_size/sizeof(Elf32_Word);
				cbucket = 0;
				cchain = eo.phashtbls(csec)[cbucket+2];
				int done = showreqhash(eo, cchain, cbucket, 
						csec, p0, pe, pt3);
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
		if (eo.pshdr(sec).sh_type != SHT_HASH)
		{
			printf("section is not a hash table.\n");
			return;
		}
		csec = sec;
		for (cbucket=0; 
		     cbucket < MAXBUCKET(eo, csec) && 
		     (cchain = eo.phashtbls(csec)[cbucket+2]) == STN_UNDEF;
		     cbucket++) ;
		if (cbucket >= MAXBUCKET(eo, csec))
		{
			printf("empty hash table.\n");
			return;
		}
		p0 = eo.phashtbls(csec);
		pe = eo.phashtbls(csec) + 
				eo.pshdr(csec).sh_size/sizeof(Elf32_Word);
		if (pt3 == NULL)
		{
			showhash(eo, cchain, cbucket, csec, p0, pe);
		}
		else if (*pt3 == '*')
		{
			showallhash(eo, cchain, cbucket, csec, p0, pe);
		}
		else
		{
			(void)showreqhash(eo, cchain, cbucket, csec, p0, pe, pt3);
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
		if (eo.pshdr(sec).sh_type != SHT_HASH)
		{
			printf("section is not a hash table.\n");
			return;
		}
		csec = sec;
		for (cbucket=0; 
		     cbucket < MAXBUCKET(eo, csec) && 
		     (cchain = eo.phashtbls(csec)[cbucket+2]) == STN_UNDEF;
		     cbucket++) ;
		if (cbucket >= MAXBUCKET(eo, csec))
		{
			printf("empty hash table.\n");
			return;
		}
		p0 = eo.phashtbls(csec);
		pe = eo.phashtbls(csec) + 
			eo.pshdr(csec).sh_size/sizeof(Elf32_Word);
		if (pt3 == NULL)
		{
			showhash(eo, cchain, cbucket, csec, p0, pe);
		}
		else if (*pt3 == '*')
		{
			showallhash(eo, cchain, cbucket, csec, p0, pe);
		}
		else
		{
			(void)showreqhash(eo, cchain, cbucket, csec, p0, pe, pt3);
		}
	}
	return;
}

static void
update(ElfObject &eo, int &cchain, int &cbucket, int &csec, Elf32_Word *&p0, Elf32_Word *&pe)
{
	printf("update not supported for now.\n");
	return;
}

static void
increment(ElfObject &eo, int &cchain, int &cbucket, int &csec, 
		Elf32_Word *&p0, Elf32_Word *&pe)
{
	// check section and symbol
	if (csec < 0 || csec >= eo.pehdr()->e_shnum)
	{
		printf("invalid section number.\n");
		return;
	}
	if (eo.pshdr(csec).sh_type != SHT_HASH)
	{
		printf("section is not a hash table.\n");
		return;
	}
	if (cbucket < 0 || cbucket > MAXBUCKET(eo, csec))
	{
		printf("bucket is out-of-range.\n");
		return;
	}
	if (cchain < 0 || cchain > MAXCHAIN(eo, csec))
	{
		printf("chain is out-of-range.\n");
		return;
	}

	// check common case
	Elf32_Word *pchain = eo.phashtbls(csec) + MAXBUCKET(eo, csec) + 2;
	cchain = pchain[cchain];
	if (cchain != STN_UNDEF)
	{
		showhash(eo, cchain, cbucket, csec, p0, pe);
		return;
	}
	for (cbucket++; cbucket < MAXBUCKET(eo, csec); cbucket++)
	{
		if (eo.phashtbls(csec)[cbucket+2] == STN_UNDEF)
			continue;
		cchain = eo.phashtbls(csec)[cbucket+2];
		showhash(eo, cchain, cbucket, csec, p0, pe);
		return;
	}

	// find the next hash table section, if any
	for (csec++; csec < eo.pehdr()->e_shnum; csec++)
	{
		if (eo.pshdr(csec).sh_type != SHT_HASH)
			continue;
		p0 = eo.phashtbls(csec);
		pe = eo.phashtbls(csec)+eo.pshdr(csec).sh_size;
		for (cbucket=0; cbucket < MAXBUCKET(eo, csec); cbucket++)
		{
			if (eo.phashtbls(csec)[cbucket+2] == STN_UNDEF)
				continue;
			cchain = eo.phashtbls(csec)[cbucket+2];
			showhash(eo, cchain, cbucket, csec, p0, pe);
			return;
		}
	}

	// nothing left
	printf("no next symbol found.\n");
	return;
}

void
edithash(ElfObject &eo, char *)
{
	char s[BUFSIZ];

	// start of hash table editing
	printf("editing hash tables:\n");

	// initialize hash table pointers
	int csec = eo.pehdr()->e_shnum;
	int cbucket = 0;
	int cchain = 0;

	// find the first hash table
	for (int sec=0; sec<eo.pehdr()->e_shnum; sec++)
	{
		if (eo.pshdr(sec).sh_type == SHT_HASH)
		{
			csec = sec;
			break;
		}
	}
	if (csec >= eo.pehdr()->e_shnum)
	{
		printf("no hash tables found.\n");
		return;
	}
	cchain = eo.phashtbls(csec)[cbucket+2];

	// pointers to hash tables
	Elf32_Word *p0 = eo.phashtbls(csec);
	Elf32_Word *pe = eo.phashtbls(csec) + 
		eo.pshdr(csec).sh_size/sizeof(Elf32_Word);

	// start interactive loop
	for (int done=0; !done; )
	{
		// get cmd from user
		printf("hash cmd: ");
		rmvnlgets(s);
		tokenize(s, " \t");
		char *pt = gettoken(1);

		// what is the command
		if (pt == NULL || *pt == '\0')
		{
			increment(eo, cchain, cbucket, csec, p0, pe);
		}
		else if (*pt == 'n')
		{
			// show all section names
			printf("hash table names:\n");
			for (int sec=0; sec<eo.pehdr()->e_shnum; sec++)
			{
				if (eo.pshdr(sec).sh_type != SHT_HASH)
					continue;
				printf("section %d: %s (%d) nb: %ld, nch: %ld\n", 
					sec, eo.shdrnm(sec), 
					eo.pshdr(sec).sh_name,
					eo.phashtbls(sec)[0], 
					eo.phashtbls(sec)[1]);
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
			review(eo, cchain, cbucket, csec, p0, pe);
		}
		else if (*pt == 'u')
		{
			update(eo, cchain, cbucket, csec, p0, pe);
		}
		else if (*pt == '+')
		{
			increment(eo, cchain, cbucket, csec, p0, pe);
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

