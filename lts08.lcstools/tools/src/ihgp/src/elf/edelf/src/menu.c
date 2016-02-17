// menu for elf file editor

// headers
#include "edelf.h"

// editing functions
void edmenu(ElfObject &, char *);
void edfile(ElfObject &, char *);
void edmode(ElfObject &, char *);
void edsection(ElfObject &, char *);
void edprogram(ElfObject &, char *);
void edstring(ElfObject &, char *);
void edsymbol(ElfObject &, char *);
void edoverwrite(ElfObject &, char *);
void eddemangle(ElfObject &, char *);
void edreloc(ElfObject &, char *);
void edhash(ElfObject &, char *);
void edversion(ElfObject &, char *);
void eddynamic(ElfObject &, char *);
void edexec(ElfObject &, char *);
void eddasm(ElfObject &, char *);
void edstabs(ElfObject &, char *);
 
// array of commands
struct {
	const char *cmd;
	const char *help;
	void (*func)(ElfObject &, char *);
} Cmds[] = {
	{ "?", "show menu", edmenu },
	{ "M", "toggle review/update mode", edmode },
	{ "D", "toggle demangle mode", eddemangle },
	{ "A", "disassembler", eddasm },
	{ "f", "file header", edfile },
	{ "s", "section headers", edsection },
	{ "p", "program headers", edprogram },
	{ "y", "symbol tables", edsymbol },
	{ "t", "string tables", edstring },
	{ "S", "stabs debug entries", edstabs },
	{ "r", "relocation entries", edreloc },
	{ "o", "overwrite", edoverwrite },
	{ "h", "hash table", edhash },
	{ "v", "version entries", edversion },
	{ "d", "dynamic table", eddynamic },
	{ "!", "escape to shell", edexec },
	{ "q", "quit current file", NULL },
	{ "Q", "quit edelf", NULL },
	{ NULL, NULL, NULL }
};
 
// escape to shell
void
edexec(ElfObject &, char *)
{
	(void)system("/usr/bin/ksh");
	return;
}

// call disassember
void
eddasm(ElfObject &eo, char *input)
{
	// read in program and section headers
	readdasm(eo, input);

	// disassemble texT
	editdasm(eo, input);
	return;
}

// toggle demangle C++ name mode
void
eddemangle(ElfObject &, char *)
{
	dflag = !dflag;
	if (dflag)
		printf("demangle mode ON.\n");
	else
		printf("demangle mode OFF.\n");
	return;
}

// print menu page
void
edmenu(ElfObject &, char *)
{
	printf("main menu cmds:\n");
	for (int c=0; Cmds[c].cmd != NULL; c++)
	{
		printf("%s --> (%s)\n", Cmds[c].cmd, Cmds[c].help);
	}
	return;
}

// edit version tables
void
edversion(ElfObject &eo, char *input)
{
	// read in version tables
	readversions(eo, input);

	// review or update
	editversions(eo, input);
	return;
}

// edit dynamic tables
void
eddynamic(ElfObject &eo, char *input)
{
	// read in dynamic tables
	readdynamic(eo, input);

	// review or update
	editdynamic(eo, input);
	return;
}

// edit hash tables
void
edhash(ElfObject &eo, char *input)
{
	// read in hash tables
	readhash(eo, input);

	// review or update
	edithash(eo, input);
	return;
}

// edit string tables
void
edstring(ElfObject &eo, char *input)
{
	// read in string tables
	readstrings(eo, input);

	// review or update
	editstrings(eo, input);
	return;
}

// edit symbol tables
void
edsymbol(ElfObject &eo, char *input)
{
	// read in symbol tables
	readsymbols(eo, input);

	// review or update
	editsymbols(eo, input);
	return;
}

// edit relocation entries
void
edreloc(ElfObject &eo, char *input)
{
	// read in relocation entries
	readreloc(eo, input);

	// review or update
	editreloc(eo, input);
	return;
}

// edit program headers
void
edprogram(ElfObject &eo, char *input)
{
	// read in program headers
	readphdrs(eo, input);

	// review or update
	editphdrs(eo, input);
	return;
}

// edit section headers
void
edsection(ElfObject &eo, char *input)
{
	// read in section headers
	readshdrs(eo, input);

	// review or update
	editshdrs(eo, input);
	return;
}

// edit file header
void
edfile(ElfObject &eo, char *input)
{
	// read in file header
	readfhdr(eo, input);

	// review or update
	editfhdr(eo, input);
	return;
}

// overwrite a section or segment data
void
edoverwrite(ElfObject &eo, char *input)
{
	// read in program and section headers
	readoverwriter(eo, input);

	// review or update
	overwriter(eo, input);
	return;
}

// edit stabs debug entries
void
edstabs(ElfObject &eo, char *input)
{
	// read in stabs tables
	readstabs(eo, input);

	// review or update
	editstabs(eo, input);
	return;
}

// toggle file open for read or update
void
edmode(ElfObject &eo, char *)
{
	// open file
	if (eo.isrdonly())
	{
		eo.openrdwr();
		printf("file %s opened for UPDATE.\n", eo.fname());
	}
	else 
	{
		eo.openrdonly();
		printf("file %s opened for READ.\n", eo.fname());
	}
	return;
}

// interrupt handler
extern "C" {
static void
catcher(int sig)
{
        sig;
	throw "interrupt caught!";
        return;
}
}

// start of menu 
int
menu(ElfObject &eo)
{
	int done;

	// set up interrupt handler
	signal(SIGINT, catcher);

	// start menu cycle
	for (done=0; !done; )
	{
		try {
			// get input from user
			printf("cmd: ");
			char input[BUFSIZ];
			gets(input);

			// call cmd function
			void (*pfunc)(ElfObject &, char *) = NULL;
			const char *pcmd = NULL;
			for (int c=0; Cmds[c].cmd != NULL; c++)
			{
				if (strncmp(Cmds[c].cmd, 
					input, strlen(Cmds[c].cmd)) == 0)
				{
					pfunc = Cmds[c].func;
					pcmd = Cmds[c].cmd;
					break;
				}
			}
			if (pcmd != NULL && *pcmd == 'q')
			{
				done = 1;
				continue;
			}
			else if (pcmd != NULL && *pcmd == 'Q')
			{
				done = 2;
				continue;
			}
			else if (pfunc == NULL)
			{
				printf("cmd NOT found. continuing.\n");
				continue;
			}

			// execute cmd
			pfunc(eo, input);
		}
		catch (const char *pe)
		{
			ERROR(pe, errno);
			signal(SIGINT, catcher);
		}
	}

	// turn off interrupt handler
	signal(SIGINT, SIG_DFL);
	if (done == 2)
		return(1);
	else
		return(0);
}


