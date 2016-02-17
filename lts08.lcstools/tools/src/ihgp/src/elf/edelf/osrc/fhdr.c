// perform editing operations

// headers
#include "edelf.h"
#include "trans.h"

// editing file headers 
void
readfhdr(char *)
{
	// check if a file is open
	MustBeTrue(efd >= 0);

	// check if we have a file header
	if (pehdr == NULL)
	{
		pehdr = new Elf32_Ehdr;
		MustBeTrue(pehdr != NULL);
	}

	// read in file header 
	MustBeTrue(lseek(efd, 0, SEEK_SET) != NOTOK);
	MustBeTrue(read(efd, pehdr, sizeof(Elf32_Ehdr)) == sizeof(Elf32_Ehdr));

	// check if file is 32 bits.
	MustBeTrue(pehdr->e_ident[EI_CLASS] == ELFCLASS32);
	return;
}

static void
review()
{
	// show data to user
	printf("\nFile Header:\n");
	printf("e_ident    : %s\n", 
		pehdr->e_ident);
	printf("e_class    : 0x%x (%s)\n", 
		pehdr->e_ident[EI_CLASS], 
		i2s(e_class, pehdr->e_ident[EI_CLASS]));
	printf("e_data     : 0x%x (%s)\n", 
		pehdr->e_ident[EI_DATA],
		i2s(e_data, pehdr->e_ident[EI_DATA]));
	printf("e_version  : 0x%x (%s)\n", 
		pehdr->e_ident[EI_VERSION],
		i2s(e_version, pehdr->e_ident[EI_VERSION]));
	printf("e_type     : 0x%x (%s)\n", 
		pehdr->e_type,
		ir2s(e_type, re_type, pehdr->e_type));
	printf("e_machine  : 0x%x (%s)\n", 
		pehdr->e_machine,
		i2s(e_machine, pehdr->e_machine));
	printf("e_version  : 0x%lx (%s)\n", 
		pehdr->e_version,
		i2s(e_version, pehdr->e_version));
	printf("e_entry    : 0x%lx\n", 
		pehdr->e_entry);
	printf("e_phoff    : 0x%lx\n", 
		pehdr->e_phoff);
	printf("e_shoff    : 0x%lx\n", 
		pehdr->e_shoff);
	printf("e_flags    : 0x%lx\n", 
		pehdr->e_flags);
	printf("e_ehsize   : 0x%lx\n", 
		pehdr->e_ehsize);
	printf("e_phentsize: 0x%lx\n", 
		pehdr->e_phentsize);
	printf("e_phnum    : 0x%lx\n", 
		pehdr->e_phnum);
	printf("e_shentsize: 0x%lx\n", 
		pehdr->e_shentsize);
	printf("e_shnum    : 0x%lx\n", 
		pehdr->e_shnum);
	printf("e_shstrndx : 0x%lx\n", 
		pehdr->e_shstrndx);
	return;
}

static void
update()
{
	int upd = 0;
	char s[BUFSIZ];

	// show data to user and update
	printf("\nFile Header:\n");

	printf("e_ident [cr=%s]: ", pehdr->e_ident);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		strncpy((char *)pehdr->e_ident, s, EI_NIDENT);
		pehdr->e_ident[EI_NIDENT-1] = '\0';
		printf("e_ident: %s\n", pehdr->e_ident);
	}
	printf("e_class [cr=0x%x]: ", pehdr->e_ident[EI_CLASS]);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pehdr->e_ident[EI_CLASS] = MYatoi(s);
		printf("e_class: 0x%x\n", pehdr->e_ident[EI_CLASS]);
	}
	printf("e_data [cr=0x%x]: ", pehdr->e_ident[EI_DATA]);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pehdr->e_ident[EI_DATA] = MYatoi(s);
		printf("e_data: 0x%x\n", pehdr->e_ident[EI_DATA]);
	}
	printf("e_version [cr=0x%x]: ", pehdr->e_ident[EI_VERSION]);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pehdr->e_ident[EI_VERSION] = MYatoi(s);
		printf("e_version: 0x%x\n", pehdr->e_ident[EI_VERSION]);
	}
	printf("e_type [cr=0x%x]: ", pehdr->e_type);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pehdr->e_type = MYatoi(s);
		printf("e_type: 0x%x\n", pehdr->e_type);
	}
	printf("e_machine [cr=0x%x]: ", pehdr->e_machine);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pehdr->e_machine = MYatoi(s);
		printf("e_machine: 0x%x\n", pehdr->e_machine);
	}
	printf("e_version [cr=0x%lx]: ", pehdr->e_version);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pehdr->e_version = MYatoi(s);
		printf("e_version: 0x%lx\n", pehdr->e_version);
	}
	printf("e_entry [cr=0x%lx]: ", pehdr->e_entry);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pehdr->e_entry = MYatoi(s);
		printf("e_entry: 0x%lx\n", pehdr->e_entry);
	}
	printf("e_phoff [cr=0x%lx]: ", pehdr->e_phoff);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pehdr->e_phoff = MYatoi(s);
		printf("e_phoff: 0x%lx\n", pehdr->e_phoff);
	}
	printf("e_shoff [cr=0x%lx]: ", pehdr->e_shoff);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pehdr->e_shoff = MYatoi(s);
		printf("e_shoff: 0x%lx\n", pehdr->e_shoff);
	}
	printf("e_flags [cr=0x%lx]: ", pehdr->e_flags);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pehdr->e_flags = MYatoi(s);
		printf("e_flags: 0x%lx\n", pehdr->e_flags);
	}
	printf("e_ehsize [crc=0x%lx]: ", pehdr->e_ehsize);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pehdr->e_ehsize = MYatoi(s);
		printf("e_ehsize: 0x%lx\n", pehdr->e_ehsize);
	}
	printf("e_phentsize [cr=0x%lx]: ", pehdr->e_phentsize);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pehdr->e_phentsize = MYatoi(s);
		printf("e_phentsize: 0x%lx\n", pehdr->e_phentsize);
	}
	printf("e_phnum [cr=0x%lx]: ", pehdr->e_phnum);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pehdr->e_phnum = MYatoi(s);
		printf("e_phnum: 0x%lx\n", pehdr->e_phnum);
	}
	printf("e_shentsize [cr=0x%lx]: ", pehdr->e_shentsize);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pehdr->e_shentsize = MYatoi(s);
		printf("e_shentsize: 0x%lx\n", pehdr->e_shentsize);
	}
	printf("e_shnum [cr=0x%lx]: ", pehdr->e_shnum);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pehdr->e_shnum = MYatoi(s);
		printf("e_shnum: 0x%lx\n", pehdr->e_shnum);
	}
	printf("e_shstrndx [cr=0x%lx]: ", pehdr->e_shstrndx);
	rmvnlgets(s);
	if (*s != '\0')
	{
		upd++;
		pehdr->e_shstrndx = MYatoi(s);
		printf("e_shstrndx: 0x%lx\n", pehdr->e_shstrndx);
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
			MustBeTrue(lseek(efd, 0, SEEK_SET) != NOTOK);
			MustBeTrue(write(efd, pehdr, 
				sizeof(Elf32_Ehdr)) == sizeof(Elf32_Ehdr));
		}
		else
		{
			// no data was written. however, current data is 
			// now wrong. reread from disk.
			char dummy[1];
			readfhdr(dummy);
		}
	}
	return;
}

static void
printmenu()
{
	printf("\nfile header menu:\n\n");
	printf("? or h - show menu\n");
	printf("r or <cr> - review file header\n");
	printf("u - update file header\n");
	printf("q - quit\n\n");
	return;
}

void
editfhdr(char *)
{
	char s[BUFSIZ];

	// start of file headers editing
	printf("editing file headers:\n");

	// start interactive loop
	for (int done=0; !done; )
	{
		// get cmd from user
		printf("fhdr cmd: ");
		rmvnlgets(s);
		tokenize(s, " \t");
		char *pt = gettoken(1);

		// what is the command
		if (pt == NULL || *pt == '\0')
		{
			review();
		}
		else if (*pt == '?' || *pt == 'h')
		{
			printmenu();
		}
		else if (*pt == 'r')
		{
			review();
		}
		else if (*pt == 'u')
		{
			update();
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

