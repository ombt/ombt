/* get size of a file */

/* unix headers */
#include <stdio.h>
#include <errno.h>
#include <dtype.h>
#include <lla/lla.h>
#include <lla/log_sub.h>
#include <sys/stat.h>

/* local headers */
#include "returns.h"
#include "debug.h"

/* externs */
extern char *optarg;
extern int optind;
extern int errno;
extern long filespace;

/* usage message */
void
usage(arg0)
char *arg0;
{
	fprintf(stderr, "usage: %s file ...\n", arg0);
	return;
}

/* main entry point */
main(argc, argv)
int argc;
char **argv;
{
	int c, fd, arg;
	struct stat statbuf;

	/* check command line arguements */
	while ((c = getopt(argc, argv, "?")) != EOF)
	{
		/* get option */
		switch (c)
		{
		case '?':
			/* help message */
			usage(argv[0]);
			exit(0);

		default:
			/* invalid option */
			ERROR("invalid command line option.", EINVAL);
			usage(argv[0]);
			exit(2);
		}
	}

	/* cycle thru input file and install individual files */
	for (arg = optind; arg < argc; arg++)
	{
		/* open file for read */
		if ((fd = open(argv[arg], 0)) == NOTOK)
		{
			/* skip file */
			continue;
		}

		/* get file data */
		if (fstat(fd, &statbuf) != OK)
		{
			/* skip file */
			close(fd);
			continue;
		}

		/* set size of allocated file */
		if (fsize(fd) < 0)
		{
			/* skip file */
			close(fd);
			continue;
		}

		/* clean up */
		close(fd);

		/* print data */
		printf("file = %s fstat.size = %ld fsize = %ld\n",
			argv[arg], (long)statbuf.st_size, filespace);
	}

	/* all done */
	exit(0);
}
