// menu for elf file editor

// headers
#include "edelf.h"

// editing functions
void edmenu(char *);
void edfile(char *);
void edmode(char *);
void edsection(char *);
void edprogram(char *);
void edstring(char *);
void edsymbol(char *);
void edoverwrite(char *);
void eddemangle(char *);
void edreloc(char *);
void edhash(char *);
void edversion(char *);
void eddynamic(char *);
void edexec(char *);
void eddasm(char *);
void edstabs(char *);
 
// array of commands
struct {
	const char *cmd;
	const char *help;
	void (*func)(char *);
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
edexec(char *)
{
	(void)system("/usr/bin/ksh");
	return;
}

// call disassember
void
eddasm(char *input)
{
	// read in program and section headers
	readdasm(input);

	// disassemble texT
	editdasm(input);
	return;
}

// toggle demangle C++ name mode
void
eddemangle(char *)
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
edmenu(char *)
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
edversion(char *input)
{
	// read in version tables
	readversions(input);

	// review or update
	editversions(input);
	return;
}

// edit dynamic tables
void
eddynamic(char *input)
{
	// read in dynamic tables
	readdynamic(input);

	// review or update
	editdynamic(input);
	return;
}

// edit hash tables
void
edhash(char *input)
{
	// read in hash tables
	readhash(input);

	// review or update
	edithash(input);
	return;
}

// edit string tables
void
edstring(char *input)
{
	// read in string tables
	readstrings(input);

	// review or update
	editstrings(input);
	return;
}

// edit symbol tables
void
edsymbol(char *input)
{
	// read in symbol tables
	readsymbols(input);

	// review or update
	editsymbols(input);
	return;
}

// edit relocation entries
void
edreloc(char *input)
{
	// read in relocation entries
	readreloc(input);

	// review or update
	editreloc(input);
	return;
}

// edit program headers
void
edprogram(char *input)
{
	// read in program headers
	readphdrs(input);

	// review or update
	editphdrs(input);
	return;
}

// edit section headers
void
edsection(char *input)
{
	// read in section headers
	readshdrs(input);

	// review or update
	editshdrs(input);
	return;
}

// edit file header
void
edfile(char *input)
{
	// read in file header
	readfhdr(input);

	// review or update
	editfhdr(input);
	return;
}

// overwrite a section or segment data
void
edoverwrite(char *input)
{
	// read in program and section headers
	readoverwriter(input);

	// review or update
	overwriter(input);
	return;
}

// edit stabs debug entries
void
edstabs(char *input)
{
	// read in stabs tables
	readstabs(input);

	// review or update
	editstabs(input);
	return;
}

// toggle file open for read or update
void
edmode(char *)
{
	// close file
	if (efd >= 0)
		close(efd);
	efd = NOTOK;

	// open file
	uflag = !uflag;
	if (uflag)
	{
		MustBeTrue((efd = open(efn, O_RDWR)) != NOTOK);
		printf("file %s opened for UPDATE.\n", efn);
	}
	else 
	{
		MustBeTrue((efd = open(efn, O_RDONLY)) != NOTOK);
		printf("file %s opened for READ.\n", efn);
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
menu()
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
			void (*pfunc)(char *) = NULL;
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
			pfunc(input);
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


