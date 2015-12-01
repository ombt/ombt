// perform editing operations

// headers
#include "edelf.h"
#include "trans.h"

// editing hash tables
void
readhash(char *s)
{
	// read in file, section headers, and string tables
	readsymbols(s);

	// delete old hash tables
	if (phashtbls != NULL)
	{
		for (int sec=0; sec<pehdr->e_shnum; sec++)
		{
			if (phashtbls[sec] != NULL)
			{
				delete [] phashtbls[sec];
				phashtbls[sec] = NULL;
			}
		}
		delete [] phashtbls;
		phashtbls = NULL;
	}

	// allocate a new hash table
	if (phashtbls == NULL)
	{
		// allocate a array of hash tables
		phashtbls = new Elf32_Word * [pehdr->e_shnum];
		MustBeTrue(phashtbls != NULL);

		// allocate section hash tables
		for (int sec=0; sec<pehdr->e_shnum; sec++)
		{
			// initialize
			phashtbls[sec] = NULL;

			// skip section that are not hash tables
			if (pshdrs[sec].sh_type != SHT_HASH)
				continue;

			// allocate a hash table
			int tblsz = (pshdrs[sec].sh_size/sizeof(Elf32_Word))+1;
			phashtbls[sec] = new Elf32_Word [tblsz];
			MustBeTrue(phashtbls[sec] != NULL);
		}
	}

	// read in hash tables.
	for (int sec=0; sec<pehdr->e_shnum; sec++)
	{
		// skip section that are not hash tables
		if (pshdrs[sec].sh_type != SHT_HASH)
			continue;

		// seek and read in hash table
		MustBeTrue(lseek(efd, pshdrs[sec].sh_offset, SEEK_SET) != NOTOK)
		int numbytes = pshdrs[sec].sh_size;
		MustBeTrue(read(efd, phashtbls[sec], numbytes) == numbytes);
	}
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
showhash(int &cchain, int &cbucket, int &csec, Elf32_Word *&p0, Elf32_Word *&pe)
{
	int symsec = pshdrs[csec].sh_link;
	int strsec = pshdrs[symsec].sh_link;
	Elf32_Sym *psym = (Elf32_Sym *)
		(psymtbls[symsec] + cchain*pshdrs[symsec].sh_entsize);
	char *psymnm = pstrtbls[strsec] + psym->st_name;
	printf("bucket: %d chain: %d symbol: %s (%lx)\n", 
		cbucket, cchain, mydemangle(psymnm), psym->st_name);
	return;
}

static void
showallhash(int &cchain, int &cbucket, int &csec, Elf32_Word *&p0, Elf32_Word *&pe)
{
	Elf32_Word *phash = p0 + 2;
	Elf32_Word *pchain = phash + MAXBUCKET(csec);
	int maxb = MAXBUCKET(csec);
	for (cbucket=0; cbucket < maxb; cbucket++)
	{
		if (phash[cbucket] == STN_UNDEF) continue;
		for (cchain = phash[cbucket]; cchain != STN_UNDEF; 
		     cchain = pchain[cchain])
		{
			showhash(cchain, cbucket, csec, p0, pe);
		}
	}
	return;
}

static int
showreqhash(int &cchain, int &cbucket, int &csec, 
	Elf32_Word *&p0, Elf32_Word *&pe, const char *ps)
{
	int symsec = pshdrs[csec].sh_link;
	int strsec = pshdrs[symsec].sh_link;
	Elf32_Word *phash = p0 + 2;
	Elf32_Word *pchain = phash + MAXBUCKET(csec);
	int maxb = MAXBUCKET(csec);
	for (cbucket=0; cbucket < maxb; cbucket++)
	{
		if (phash[cbucket] == STN_UNDEF) 
			continue;
		for (cchain = phash[cbucket]; cchain != STN_UNDEF; 
		     cchain = pchain[cchain])
		{
			Elf32_Sym *psym = (Elf32_Sym *)(psymtbls[symsec] + 
				cchain*pshdrs[symsec].sh_entsize);
			char *psymnm = pstrtbls[strsec] + psym->st_name;
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
review(int &cchain, int &cbucket, int &csec, Elf32_Word *&p0, Elf32_Word *&pe)
{
	// get tokens from user stream
	char *pt2 = gettoken(2);
	char *pt3 = gettoken(3);

	// determine user request
	if (pt2 == NULL)
	{
		// check section and symbol
		if (csec < 0 || csec >= pehdr->e_shnum)
		{
			printf("invalid section number.\n");
			return;
		}
		if (pshdrs[csec].sh_type != SHT_HASH)
		{
			printf("section is not a hash table.\n");
			return;
		}
		if (cbucket < 0 || cbucket > MAXBUCKET(csec))
		{
			printf("bucket is out-of-range.\n");
			return;
		}
		if (cchain < 0 || cchain > MAXCHAIN(csec))
		{
			printf("chain is out-of-range.\n");
			return;
		}
		showhash(cchain, cbucket, csec, p0, pe);
	}
	else if (*pt2 == '*')
	{
		if (pt3 == NULL)
		{
			// show all symbols
			for (csec=0; csec<pehdr->e_shnum; csec++)
			{
				if (pshdrs[csec].sh_type != SHT_HASH)
					continue;
				printf("section %d: %s (%d) nb: %ld, nch: %ld\n", 
					csec, pshstrtbl+pshdrs[csec].sh_name,
					pshdrs[csec].sh_name,
					MAXBUCKET(csec), MAXCHAIN(csec));
				p0 = phashtbls[csec];
				pe = phashtbls[csec] + 
					pshdrs[csec].sh_size/sizeof(Elf32_Word);
				cbucket = 0;
				cchain = phashtbls[csec][cbucket+2];
				showallhash(cchain, cbucket, csec, p0, pe);
			}
		}
		else
		{
			// show requested symbol
			for (csec=0; csec<pehdr->e_shnum; csec++)
			{
				if (pshdrs[csec].sh_type != SHT_HASH)
					continue;
				printf("section %d: %s (%d) nb: %ld, nch: %ld\n", 
					csec, pshstrtbl+pshdrs[csec].sh_name,
					pshdrs[csec].sh_name,
					MAXBUCKET(csec), MAXCHAIN(csec));
				p0 = phashtbls[csec];
				pe = phashtbls[csec] + 
					pshdrs[csec].sh_size/sizeof(Elf32_Word);
				cbucket = 0;
				cchain = phashtbls[csec][cbucket+2];
				int done = showreqhash(cchain, cbucket, 
						csec, p0, pe, pt3);
				if (done) break;
			}
		}
	}
	else if (strlen(pt2) == strspn(pt2, "0123456789"))
	{
		// a section number was given
		int sec = MYatoi(pt2);
		if (sec < 0 || sec >= pehdr->e_shnum)
		{
			printf("out-of-range section number.\n");
			printf("minimum section number = 0\n");
			printf("maximum section number = %d\n",
				pehdr->e_shnum);
			return;
		}
		if (pshdrs[sec].sh_type != SHT_HASH)
		{
			printf("section is not a hash table.\n");
			return;
		}
		csec = sec;
		for (cbucket=0; 
		     cbucket < MAXBUCKET(csec) && 
		     (cchain = phashtbls[csec][cbucket+2]) == STN_UNDEF;
		     cbucket++) ;
		if (cbucket >= MAXBUCKET(csec))
		{
			printf("empty hash table.\n");
			return;
		}
		p0 = phashtbls[csec];
		pe = phashtbls[csec] + pshdrs[csec].sh_size/sizeof(Elf32_Word);
		if (pt3 == NULL)
		{
			showhash(cchain, cbucket, csec, p0, pe);
		}
		else if (*pt3 == '*')
		{
			showallhash(cchain, cbucket, csec, p0, pe);
		}
		else
		{
			(void)showreqhash(cchain, cbucket, csec, p0, pe, pt3);
		}
	}
	else
	{
		// a section name was given
		int sec = nametosec(pt2);
		if (sec == NOTOK)
		{
			printf("section not found.\n");
			return;
		}
		if (pshdrs[sec].sh_type != SHT_HASH)
		{
			printf("section is not a hash table.\n");
			return;
		}
		csec = sec;
		for (cbucket=0; 
		     cbucket < MAXBUCKET(csec) && 
		     (cchain = phashtbls[csec][cbucket+2]) == STN_UNDEF;
		     cbucket++) ;
		if (cbucket >= MAXBUCKET(csec))
		{
			printf("empty hash table.\n");
			return;
		}
		p0 = phashtbls[csec];
		pe = phashtbls[csec] + pshdrs[csec].sh_size/sizeof(Elf32_Word);
		if (pt3 == NULL)
		{
			showhash(cchain, cbucket, csec, p0, pe);
		}
		else if (*pt3 == '*')
		{
			showallhash(cchain, cbucket, csec, p0, pe);
		}
		else
		{
			(void)showreqhash(cchain, cbucket, csec, p0, pe, pt3);
		}
	}
	return;
}

static void
update(int &cchain, int &cbucket, int &csec, Elf32_Word *&p0, Elf32_Word *&pe)
{
	printf("update not supported for now.\n");
	return;
}

static void
increment(int &cchain, int &cbucket, int &csec, 
		Elf32_Word *&p0, Elf32_Word *&pe)
{
	// check section and symbol
	if (csec < 0 || csec >= pehdr->e_shnum)
	{
		printf("invalid section number.\n");
		return;
	}
	if (pshdrs[csec].sh_type != SHT_HASH)
	{
		printf("section is not a hash table.\n");
		return;
	}
	if (cbucket < 0 || cbucket > MAXBUCKET(csec))
	{
		printf("bucket is out-of-range.\n");
		return;
	}
	if (cchain < 0 || cchain > MAXCHAIN(csec))
	{
		printf("chain is out-of-range.\n");
		return;
	}

	// check common case
	Elf32_Word *pchain = phashtbls[csec] + MAXBUCKET(csec) + 2;
	cchain = pchain[cchain];
	if (cchain != STN_UNDEF)
	{
		showhash(cchain, cbucket, csec, p0, pe);
		return;
	}
	for (cbucket++; cbucket < MAXBUCKET(csec); cbucket++)
	{
		if (phashtbls[csec][cbucket+2] == STN_UNDEF)
			continue;
		cchain = phashtbls[csec][cbucket+2];
		showhash(cchain, cbucket, csec, p0, pe);
		return;
	}

	// find the next hash table section, if any
	for (csec++; csec < pehdr->e_shnum; csec++)
	{
		if (pshdrs[csec].sh_type != SHT_HASH)
			continue;
		p0 = phashtbls[csec];
		pe = phashtbls[csec]+pshdrs[csec].sh_size;
		for (cbucket=0; cbucket < MAXBUCKET(csec); cbucket++)
		{
			if (phashtbls[csec][cbucket+2] == STN_UNDEF)
				continue;
			cchain = phashtbls[csec][cbucket+2];
			showhash(cchain, cbucket, csec, p0, pe);
			return;
		}
	}

	// nothing left
	printf("no next symbol found.\n");
	return;
}

void
edithash(char *)
{
	char s[BUFSIZ];

	// start of hash table editing
	printf("editing hash tables:\n");

	// initialize hash table pointers
	int csec = pehdr->e_shnum;
	int cbucket = 0;
	int cchain = 0;

	// find the first hash table
	for (int sec=0; sec<pehdr->e_shnum; sec++)
	{
		if (pshdrs[sec].sh_type == SHT_HASH)
		{
			csec = sec;
			break;
		}
	}
	if (csec >= pehdr->e_shnum)
	{
		printf("no hash tables found.\n");
		return;
	}
	cchain = phashtbls[csec][cbucket+2];

	// pointers to hash tables
	Elf32_Word *p0 = phashtbls[csec];
	Elf32_Word *pe = phashtbls[csec] + 
		pshdrs[csec].sh_size/sizeof(Elf32_Word);

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
			increment(cchain, cbucket, csec, p0, pe);
		}
		else if (*pt == 'n')
		{
			// show all section names
			printf("hash table names:\n");
			for (int sec=0; sec<pehdr->e_shnum; sec++)
			{
				if (pshdrs[sec].sh_type != SHT_HASH)
					continue;
				printf("section %d: %s (%d) nb: %ld, nch: %ld\n", 
					sec, pshstrtbl+pshdrs[sec].sh_name,
					pshdrs[sec].sh_name,
					phashtbls[sec][0], phashtbls[sec][1]);
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
			review(cchain, cbucket, csec, p0, pe);
		}
		else if (*pt == 'u')
		{
			update(cchain, cbucket, csec, p0, pe);
		}
		else if (*pt == '+')
		{
			increment(cchain, cbucket, csec, p0, pe);
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

