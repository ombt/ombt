/* do a convolution of two spectra */

/* unix headers */
#include <sysent.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/* local headers */
#include "returns.h"
#include "debug.h"
#include "complex.h"
#include "spectrum.h"

/* globals */
extern int optind;
extern char *optarg;

/* usage msg */
void
usage(char *cmd)
{
	fprintf(stderr, "usage: %s [-?] spec1 spec2 outspec\n", cmd);
	return;
}

/* main entry */
submain(int argc, char **argv)
{
	int c;

	/* get command line options */
	while ((c = getopt(argc, argv, "?")) != EOF)
	{
		/* get options */
		switch (c)
		{
		case '?':
			/* help message */
			usage(argv[0]);
			return(0);

		default:
			/* error */
			ERROR("invalid option.", EINVAL);
			usage(argv[0]);
			return(2);
		}
	}

	/* check if spectra and output file were given */
	if ((optind + 3) > argc)
	{
		ERROR("missing spectrum or output file.", EINVAL);
		usage(argv[0]);
		return(2);
	}

	/* create spectra */
	Spectrum sp1(argv[optind]);
	if ((errno = sp1.getStatus()) != OK)
	{
		ERRORS("spectrum constructor failed for spectrum.", 
			argv[optind], errno);
		exit(2);
	}
	Spectrum sp2(argv[optind+1]);
	if ((errno = sp2.getStatus()) != OK)
	{
		ERRORS("spectrum constructor failed for spectrum.", 
			argv[optind+1], errno);
		exit(2);
	}

	/* do convolution */
	Spectrum convsp = convolution(sp1, sp2);
	
	/* write out spectrum */
	convsp.writeSpectrum(argv[optind+2]);

	/* all done */
	return(0);
}

main(int argc, char **argv)
{
	exit(submain(argc, argv));
}

