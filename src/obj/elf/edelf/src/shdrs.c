// perform editing operations

// headers
#include "edelf.h"
#include "trans.h"

// editing section headers 
void
readshdrs(ElfObject &eo, char *s)
{
	eo.readehdr();
	eo.readshdrs();
	return;
}

static void
review(ElfObject &eo, int sec)
{
	if (sec < 0 || sec >= eo.pehdr()->e_shnum)
	{
		printf("out-of-range section number.\n");
		return;
	}
	printf("section %d: %s (%d)\n", 
		sec, eo.shdrnm(sec), eo.pshdr(sec).sh_name);
	printf("sh_name     : 0x%lx\n", 
		eo.pshdr(sec).sh_name);
	printf("sh_type     : 0x%lx (%s)\n",
		eo.pshdr(sec).sh_type,
		ir2s(sh_type, rsh_type, eo.pshdr(sec).sh_type));
	printf("sh_flags    : 0x%lx (%s)\n",
		eo.pshdr(sec).sh_flags, 
		b2s(sh_flags, eo.pshdr(sec).sh_flags));
	printf("sh_addr     : 0x%lx\n",
		eo.pshdr(sec).sh_addr);
	printf("sh_offset   : 0x%lx\n",
		eo.pshdr(sec).sh_offset);
	printf("sh_size     : 0x%lx\n",
		eo.pshdr(sec).sh_size);
	printf("sh_link     : 0x%lx\n",
		eo.pshdr(sec). sh_link);
	printf("sh_info     : 0x%lx\n",
		eo.pshdr(sec).sh_info);
	printf("sh_addralign: 0x%lx\n",
		eo.pshdr(sec).sh_addralign);
	printf("sh_entsize  : 0x%lx\n",
		eo.pshdr(sec).sh_entsize);
	return;
}

static void
update(ElfObject &eo, int sec)
{
	int upd = 0;
	char s[BUFSIZ];

	// tell user which section is being updated
	printf("\nsection (%d) %s header:\n", sec, eo.shdrnm(sec));

	// update section fields
	printf("sh_name [cr=%s]: ", eo.shdrnm(sec));
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		int sz = strlen(eo.shdrnm(sec));
		strncpy(eo.shdrnm(sec), s, sz);
		eo.shdrnm(sec)[sz] = '\0';
		printf("sh_name: %s\n", eo.shdrnm(sec));
	}
	printf("sh_type [cr=0x%lx]: ", eo.pshdr(sec).sh_type);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		eo.pshdr(sec).sh_type = MYatoi(s);
		printf("sh_type: 0x%lx\n", eo.pshdr(sec).sh_type);
	}
	printf("sh_flags [cr=0x%lx]: ", eo.pshdr(sec).sh_flags);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		eo.pshdr(sec).sh_flags = MYatoi(s);
		printf("sh_flags: 0x%lx\n", eo.pshdr(sec).sh_flags);
	}
	printf("sh_addr [cr=0x%lx]: ", eo.pshdr(sec).sh_addr);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		eo.pshdr(sec).sh_addr = MYatoi(s);
		printf("sh_addr: 0x%lx\n", eo.pshdr(sec).sh_addr);
	}
	printf("sh_offset [cr=0x%lx]: ", eo.pshdr(sec).sh_offset);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		eo.pshdr(sec).sh_offset = MYatoi(s);
		printf("sh_offset: 0x%lx\n", eo.pshdr(sec).sh_offset);
	}
	printf("sh_size [cr=0x%lx]: ", eo.pshdr(sec).sh_size);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		eo.pshdr(sec).sh_size = MYatoi(s);
		printf("sh_size: 0x%lx\n", eo.pshdr(sec).sh_size);
	}
	printf("sh_link [cr=0x%lx]: ", eo.pshdr(sec).sh_link);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		eo.pshdr(sec).sh_link = MYatoi(s);
		printf("sh_link: 0x%lx\n", eo.pshdr(sec).sh_link);
	}
	printf("sh_info [cr=0x%lx]: ", eo.pshdr(sec).sh_info);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		eo.pshdr(sec).sh_info = MYatoi(s);
		printf("sh_info: 0x%lx\n", eo.pshdr(sec).sh_info);
	}
	printf("sh_addralign [cr=0x%lx]: ", eo.pshdr(sec).sh_addralign);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		eo.pshdr(sec).sh_addralign = MYatoi(s);
		printf("sh_addralign: 0x%lx\n", eo.pshdr(sec).sh_addralign);
	}
	printf("sh_entsize [cr=0x%lx]: ", eo.pshdr(sec).sh_entsize);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		eo.pshdr(sec).sh_entsize = MYatoi(s);
		printf("sh_entsize: 0x%lx\n", eo.pshdr(sec).sh_entsize);
	}
	if (upd > 0)
	{
		printf("write to file [cr=n/n/y] ? ");
		rmvnlgets(s);
		if (*s == 'y')
		{
			eo.writeshdrs();
		}
		else
		{
			// no data was written, but now the current data
			// is wrong. reread the data from disk.
			char dummy[1];
			readshdrs(eo, dummy);
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
editshdrs(ElfObject &eo, char *)
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
			if ((csec + 1) < eo.pehdr()->e_shnum)
			{
				csec++;
				review(eo, csec);
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
			for (int sec=0; sec<eo.pehdr()->e_shnum; sec++)
			{
				printf("section %d: %s (%d, %s)\n", sec, 
					eo.shdrnm(sec), eo.pshdr(sec).sh_name, 
					ir2s(sh_type, rsh_type, 
						eo.pshdr(sec).sh_type));
			}
		}
		else if (*pt == 'r')
		{
			// get next token
			pt = gettoken(2);

			// type of review
			if (pt == NULL || *pt == '\0')
			{
				review(eo, csec);
			}
			else if (*pt == '*')
			{
				printf("section hdrs data:\n");
				for (int sec=0; sec<eo.pehdr()->e_shnum; sec++)
				{
					review(eo, sec);
				}
			}
			else if (strspn(pt, "0123456789") == strlen(pt))
			{
				// show a specific section
				printf("section hdr data:\n");
				int sec = MYatoi(pt);
				if (sec < 0 || sec >= eo.pehdr()->e_shnum)
				{
					printf("out-of-range section number.\n");
					return;
				}
				csec = sec;
				review(eo, csec);
			}
			else
			{
				// show a specific section
				char s[BUFSIZ];
				printf("section hdr data:\n");
				int found = 0;
				for (int sec=0; 
				    (sec = nametosec(eo, pt, sec)) != NOTOK; sec++)
				{
					found = 1;
					csec = sec;
					review(eo, csec);
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
			update(eo, csec);
		}
		else if (*pt == '+')
		{
			if ((csec + 1) < eo.pehdr()->e_shnum)
			{
				csec++;
				review(eo, csec);
			}
			else
				printf("cannot increment beyond last section.\n");
		}
		else if (*pt == '-')
		{
			if ((csec - 1) >= 0)
			{
				csec--;
				review(eo, csec);
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

