// elf file editor

// headers 
#include "edelf.h"

// externs
extern char *optarg;
extern int optind;

// usage msg
void
usage(const char *cmd)
{
	printf("usage: %s [-?dV] elf_file ...\n\n", cmd);
	printf("\t-? - print usage message\n");
	printf("\t-d - demangle C++ names\n");
	printf("\t-V - version information\n");
	return;
}

// print version information
void
version(const char *cmd)
{
	printf("%s: Version 1.3.3, Author: M.A. Rumore (mrumore@lucent.com)\n",
		cmd);
	return;
}

// main entry point
main(int argc, char ** argv)
{
	// get command line options
	for (int c=0; (c=getopt(argc, argv, "Vd?")) != EOF; )
	{
		switch (c)
		{
		case 'd':
			dflag = 1;
			break;

		case '?':
			usage(argv[0]);
			return(0);

		case 'V':
			version(argv[0]);
			return(0);

		default:
			ERRORD("invalid command option.", c, EINVAL);
			return(2);
		}
	}

	// edit elf files
	if (optind >= argc)
	{
		printf("no files given.\n");
		return(2);
	}
	int done=0;
	for (int arg=optind; arg < argc && !done; arg++)
	{
		printf("\nediting file %s ...\n", argv[arg]);
		try {
			ElfObject eo(argv[arg]);
			eo.openrdonly();
			done = menu(eo);
		}
		catch (const char *pe)
		{
			printf("EXCEPTION CAUGHT: %s\n", pe);
		}
		catch (char *pe)
		{
			printf("EXCEPTION CAUGHT: %s\n", pe);
		}
	}

	// all done
	return(0);
}
