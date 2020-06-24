/* operating system headers */
#include <stdio.h>
#include <string.h>

/* headers for parser */
#include "parser.h"

extern FILE *traceFd, *listFd, *sourceFd;

main(argc, argv)
int argc;
char **argv;
{
	int arg, verbose, trace, ret, reopen;

	/* get options from command line */
	verbose = trace = 0;
	for (arg = 1; arg < argc; )
	{
		if (argv[arg][0] != '-')
		{
			break;
		}
		else if (strcmp(argv[arg], "-t") == 0)
		{
			trace = 1;
			arg++;
		}
		else if (strcmp(argv[arg], "-v") == 0)
		{
			verbose = 1;
			arg++;
		}
		else
		{
			fprintf(stderr, "invalid option: %s\n", argv[arg]);
			fprintf(stderr, "usage: %s [-tv] file\n", argv[0]);
			exit(1);
		}
	}
	if (arg >= argc)
	{
		fprintf(stderr, "usage: %s [-tv] file\n", argv[0]);
		exit(1);
	}

	/* initialize tracing, if any */
	if ((ret = initTrace(argv[arg], trace)) != OK)
	{
		fprintf(stderr, "unable to enable tracing for file %s.\n", argv[arg]);
		exit(1);
	}

	/* initialize parser */
	if ((ret = initParser(argv[arg], FIRSTOPEN, verbose)) != OK)
	{
		fprintf(stderr, "unable to initialize parser for file %s.\n", argv[arg]);
		exit(1);
	}

	/* call parser for this file */
	if ((ret = parser(FIRSTPASS)) != OK)
	{
		reopen = NOREOPEN;
	}
	else
	{
		reopen = REOPEN;
	}

	/* no errors were found */
	if ((ret = initParser(argv[arg], reopen, verbose)) != OK)
	{
		fprintf(stderr, "unable to initialize parser for file %s.\n", argv[arg]);
		exit(1);
	}

	/* call parser for this file */
	(void) parser(SECONDPASS);

	/* close all files */
	closeTrace();
	closeParser();

	/* all done */
	exit(0);
}
