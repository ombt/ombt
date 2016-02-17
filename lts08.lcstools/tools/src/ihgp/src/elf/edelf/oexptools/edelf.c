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
	printf("%s: Version 1.3.1, Author: M.A. Rumore (mrumore@lucent.com)\n",
		cmd);
	return;
}

// editing function
int
edelf(const char *fn)
{
	// save file name
	strcpy(efn, fn);

	// open file for read or write
	if (uflag)
	{
		MustBeTrue((efd = open(efn, O_RDWR)) != NOTOK);
	}
	else
	{
		MustBeTrue((efd = open(efn, O_RDONLY)) != NOTOK);
	}

	// start interactive editing
	int done = menu();

	// close file
	if (efd >= 0) 
		close(efd);
	efd = NOTOK;
	return(done);
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
			done = edelf(argv[arg]);
		}
		catch (const char *pe)
		{
			printf("EXCEPTION CAUGHT: %s\n", pe);
			if (efd != NOTOK)
				close(efd);
			efd = NOTOK;
		}
		catch (char *pe)
		{
			printf("EXCEPTION CAUGHT: %s\n", pe);
			if (efd != NOTOK)
				close(efd);
			efd = NOTOK;
		}
	}

	// all done
	return(0);
}
