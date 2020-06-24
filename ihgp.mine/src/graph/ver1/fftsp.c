/* display a spectrum */

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
	fprintf(stderr, "usage: %s [-?io] in_spectrum out_spectrum\n", cmd);
	return;
}

/* main entry */
submain(int argc, char **argv)
{
	int c;
	int iflag = 0;
	int oflag = 0;

	/* get command line options */
	while ((c = getopt(argc, argv, "?io")) != EOF)
	{
		/* get options */
		switch (c)
		{
		case '?':
			/* help message */
			usage(argv[0]);
			return(0);

		case 'i':
			/* do inverse fft */
			iflag = 1;
			break;

		case 'o':
			/* use N*N fourier transform */
			oflag = 1;
			break;

		default:
			/* error */
			ERROR("invalid option.", EINVAL);
			usage(argv[0]);
			return(2);
		}
	}

	/* check if spectrum and fft files were given */
	if ((optind + 2) > argc)
	{
		ERROR("missing spectrum or fft file.", EINVAL);
		usage(argv[0]);
		return(2);
	}

	/* create a spectrum */
	Spectrum sp(argv[optind]);
	if ((errno = sp.getStatus()) != OK)
	{
		ERRORS("spectrum constructor failed for spectrum.", 
			argv[optind], errno);
		exit(2);
	}

	/* calculate fft or inverse fft of spectrum */
	if (oflag)
	{
		if (iflag)
		{
			/* calculate inverse ft of spectrum */
			Spectrum invftsp = invft(sp);
	
			/* write out ft spectrum */
			invftsp.writeSpectrum(argv[optind+1]);
		}
		else
		{
			/* calculate ft of spectrum */
			Spectrum ftsp = ft(sp);
	
			/* write out ft spectrum */
			ftsp.writeSpectrum(argv[optind+1]);
		}
	}
	else
	{
		if (iflag)
		{
			/* calculate inverse fft of spectrum */
			Spectrum invfftsp = invfft(sp);
	
			/* write out fft spectrum */
			invfftsp.writeSpectrum(argv[optind+1]);
		}
		else
		{
			/* calculate fft of spectrum */
			Spectrum fftsp = fft(sp);
	
			/* write out fft spectrum */
			fftsp.writeSpectrum(argv[optind+1]);
		}
	}

	/* all done */
	return(0);
}

main(int argc, char **argv)
{
	exit(submain(argc, argv));
}

