/* make a spectrum from a data file */

/* headers */
#include <sysent.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <memory.h>
#include "returns.h"
#include "debug.h"

/* globals */
extern int optind;
extern char *optarg;

/* definitions */
#define GETMIN 0x1
#define GETMAX 0x2
#define DEFAULTCHANNELS 1024
#define DELIMITERS ";:"

/* usage msg */
void
usage(char *cmd)
{
	fprintf(stderr, 
		"usage: %s [-s start] [-e end] [-c channels] infile outfile\n", 
		cmd);
	return;

}

/* get data for input file */
int
getData(FILE *infd, int flags, double &min, double &max)
{
	double newmin, newmax;
	double val;
	char *pval, buf[BUFSIZ+1];

	/* check if anything need to be done */
	if (flags == 0) return(OK);

	/* read first data record */
	fgets(buf, BUFSIZ, infd);
	if (ferror(infd) || feof(infd))
	{
		ERROR("fgets failed.", errno);
		return(NOTOK);
	}
	pval = strtok(buf, DELIMITERS);
	if (pval == (char *)0)
	{
		ERROR("null token found in string.", EINVAL);
		return(NOTOK);
	}
	sscanf(pval, "%le", &val);
	newmin = val;
	newmax = val;

	/* read remaining file */
	while ( 1 )
	{
		/* read a line from file */
		fgets(buf, BUFSIZ, infd);
		if (ferror(infd))
		{
			ERROR("fgets failed.", errno);
			return(NOTOK);
		}
		if (feof(infd)) break;

		/* parse record */
		pval = strtok(buf, DELIMITERS);
		if (pval == (char *)0)
		{
			ERROR("null token found in string.", EINVAL);
			return(NOTOK);
		}
		sscanf(pval, "%le", &val);

		/* do comparisons */
		if (val < newmin) newmin = val;
		if (val > newmax) newmax = val;
	}

	/* rewind file for another pass */
	rewind(infd);

	/* set requested values */
	if ((flags & GETMIN) == GETMIN) min = newmin;
	if ((flags & GETMAX) == GETMAX) max = newmax;

	/* all done */
	return(OK);
}

/* create spectrum from data read from file */
int
getSpectrum(double *&spectrum, FILE *infd, double min, double max, int numch)
{
	/* check parameters */
	if (infd == (FILE *)0 || min >= max || numch <= 0)
	{
		errno = EINVAL;
		ERROR("invalid input parameters.", errno);
		return(NOTOK);
	}

	/* allocate spectrum */
	spectrum = new double [numch];
	if (spectrum == (double *)0)
	{
		errno = ENOMEM;
		ERROR("ENOMEM for spectrum.", errno);
		return(NOTOK);
	}
	memset((char *)spectrum, 0, numch*sizeof(double));

	/* get size of each channel */
	double delta = (max - min)/numch;

	/* read in data and store in spectrum */
	double xval, yval;
	while ( 1 )
	{
		/* read a line from file */
		char buf[BUFSIZ+1];
		fgets(buf, BUFSIZ, infd);
		if (ferror(infd))
		{
			ERROR("fgets failed.", errno);
			return(NOTOK);
		}
		if (feof(infd)) break;

		/* get x-value (channel) */
		char *pval = strtok(buf, DELIMITERS);
		if (pval == (char *)0)
		{
			ERROR("null token found in string.", EINVAL);
			return(NOTOK);
		}
		sscanf(pval, "%le", &xval);
		int ichan = (int)((xval - min)/delta);

		/* check if within range */
		if (xval < min || xval > max) continue;

		/* get y-value (counts) */
		pval = strtok((char *)0, DELIMITERS);
		if (pval == (char *)0)
		{
			ERROR("null token found in string.", EINVAL);
			return(NOTOK);
		}
		sscanf(pval, "%le", &yval);

		/* store in spectrum */
		spectrum[ichan] += yval;
	}

	/* rewind file for another pass */
	rewind(infd);

	/* all done */
	return(OK);
}

/* write out spectrum to a file */
int
writeSpectrum(double *spectrum, char *outfile, double min, double max, 
		int numch)
{
	/* check parameters */
	if (spectrum == (double *)0 || outfile == (char *)0 || 
	    outfile == 0 || min >= max || numch <= 0)
	{
		errno = EINVAL;
		ERROR("invalid input parameters.", errno);
		return(NOTOK);
	}

	/* open output file for write */
	FILE *outfd = fopen(outfile, "w+");
	if (outfd == (FILE *)0)
	{
		ERRORS("unable to open file.", outfile, errno);
		return(2);
	}

	/* write out channels, min, max */
	fprintf(outfd, "PARAMETER: NUMCH %d\n", numch);
	fprintf(outfd, "PARAMETER: MIN %.12le\n", min);
	fprintf(outfd, "PARAMETER: MAX %.12le\n", max);
	for (int ich = 0; ich < numch; ich++)
	{
		fprintf(outfd, "DATA: %d %.12le %.12le\n", 
			ich, spectrum[ich], 0.0);
	}

	/* close file */
	fclose(outfd);

	/* all done */
	return(OK);
}

/* main entry */
submain(int argc, char **argv)
{
	int c;
	double min, max, *spectrum;
	int numch = DEFAULTCHANNELS;
	int flags = GETMIN | GETMAX;

	/* get command line options */
	while ((c = getopt(argc, argv, "?s:e:c:")) != EOF)
	{
		/* get options */
		switch (c)
		{
		case 's':
			/* get start value */
			flags ^= GETMIN;
			sscanf(optarg, "%le", &min);
			break;

		case 'e':
			/* get end value */
			flags ^= GETMAX;
			sscanf(optarg, "%le", &max);
			break;

		case 'c':
			/* get number of channels */
			numch = atoi(optarg);
			break;

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

	/* check if file names were given */
	if ((optind + 2) > argc)
	{
		ERROR("missing input or output file names.", EINVAL);
		usage(argv[0]);
		return(2);
	}

	/* open input file */
	char *infile = argv[optind++];
	char *outfile = argv[optind];
	FILE *infd = fopen(infile, "r");
	if (infd == (FILE *)0)
	{
		ERRORS("unable to open file.", infile, errno);
		return(2);
	}

	/* get parameters */
	if (getData(infd, flags, min, max) != OK)
	{
		ERRORS("unable to get parameters for file.", infile, errno);
		return(2);
	}

	/* create spectrum */
	if (getSpectrum(spectrum, infd, min, max, numch) != OK)
	{
		ERRORS("unable to get spectrum for file.", infile, errno);
		return(2);
	}

	/* close input file */
	fclose(infd);

	/* write spectrum */
	if (writeSpectrum(spectrum, outfile, min, max, numch) != OK)
	{
		ERRORS("unable to write spectrum to file.", outfile, errno);
		unlink(outfile);
		return(2);
	}

	/* all done */
	return(0);
}

main(int argc, char **argv)
{
	exit(submain(argc, argv));
}
