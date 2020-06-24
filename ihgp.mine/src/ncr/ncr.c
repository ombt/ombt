// my simple encryption program. very simple.

// headers
#include <sysent.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "debug.h"

// local definitions for random number generator. using a linear
// congruential generator of the form:
//
//	I(j+1) = (a*I(j) + c) mod m
//
// where I is the initial seed and a, c, and m are given
// below:
//
const unsigned long mR = 121500;
const unsigned long aR = 2041;
const unsigned long cR = 25673;
unsigned long Ikey = 0;
unsigned long Iold = 0;
unsigned long Inew = 0;

// externs
extern int optind;
extern char *optarg;

// definitions
#define IRANDOM(iold) (((aR)*(iold)+(cR))%(mR))
#define USAGE "usage: %s [-?] [-k key] filename ...\n"
	
// main entry point
main(int argc, char **argv)
{
	char ifile[BUFSIZ], ofile[BUFSIZ];

	// initialize some variables
	int keywasgiven = 0;

	// check for options
	for (int c = 0; (c = getopt(argc, argv, "?k:")) != EOF; )
	{
		switch (c)
		{
		case 'k':
			// different key
			Ikey = atol(optarg);
			keywasgiven = (int)Ikey;
			break;

		case '?':
			// usage
			fprintf(stderr, USAGE, argv[0]);
			exit(2);

		default:
			// usage
			fprintf(stderr, "bad option.\n");
			fprintf(stderr, USAGE, argv[0]);
			exit(2);
		}
	}

	// check that a file name was given
	if (optind >= argc)
	{
		// no filename was given
		fprintf(stderr, USAGE, argv[0]);
		return(2);
	}

	// cycle over all files
	for ( ; optind < argc; optind++)
	{
		// get size of file.
		struct stat statbuf;
		if (stat(argv[optind], &statbuf) == -1)
		{
			fprintf(stderr, "unable to stat file %s.\n", 
				argv[optind]);
			fprintf(stderr, "errno is %d.\n", errno);
			return(2);
		}

		// check if a key was given
		if ( ! keywasgiven)
		{
			// no key. get one and make sure it is odd.
			Ikey = statbuf.st_size | 1;
		}

		// get file names
		strcpy(ifile, argv[optind]);
		sprintf(ofile, "tmp%d.%d", optind, getpid());

		// let user know the current file 
		fprintf(stderr, "processing file %s ...\n", ifile);

		// open file for read
		int ifd = open(ifile, O_RDONLY);
		if (ifd == -1)
		{
			fprintf(stderr, "unable to open file %s.\n", ifile);
			fprintf(stderr, "errno is %d.\n", errno);
			return(2);
		}

		// open output file for write
		int ofd = open(ofile, O_WRONLY|O_CREAT|O_TRUNC, 0600);
		if (ofd == -1)
		{
			fprintf(stderr, "unable to open file %s.\n", ofile);
			fprintf(stderr, "errno is %d.\n", errno);
			close(ifd);
			return(2);
		}

		// determine how many shorts to read.
		long bufsToRead = statbuf.st_size/(BUFSIZ*sizeof(short));
		long shortsToRead = (statbuf.st_size%(BUFSIZ*sizeof(short))/2);
		long byteToRead = (statbuf.st_size%(BUFSIZ*sizeof(short))%2);

		// initialize random number generator
		Iold = Ikey;

		// first, read in and convert bufs
		unsigned short sbuf[BUFSIZ];
		for (int ib = 0 ; ib < bufsToRead; ib++)
		{
			// read in a buffer
			int nr = read(ifd, sbuf, sizeof(short)*BUFSIZ);
			if (nr != sizeof(short)*BUFSIZ)
			{
				fprintf(stderr, "unable to read file %s.\n", 
					ifile);
				fprintf(stderr, "errno is %d.\n", errno);
				close(ifd);
				close(ofd);
				unlink(ofile);
				return(2);
			}

			// encrypt
			for (int is = 0 ; is < BUFSIZ; is++)
			{
				Inew = IRANDOM(Iold);
				sbuf[is] = (unsigned short)(sbuf[is] ^ Inew);
				Iold = Inew;
			}

			// write out
			int nw = write(ofd, sbuf, sizeof(short)*BUFSIZ);
			if (nw != sizeof(short)*BUFSIZ)
			{
				fprintf(stderr, "unable to write file %s.\n", 
					ofile);
				fprintf(stderr, "errno is %d.\n", errno);
				close(ifd);
				close(ofd);
				unlink(ofile);
				return(2);
			}
		}
	
		// second, read in and convert shorts
		unsigned short s = 0;
		for (int is = 0 ; is < shortsToRead; is++)
		{
			// read in a buffer
			int nr = read(ifd, &s, sizeof(short));
			if (nr != sizeof(short))
			{
				fprintf(stderr, "unable to read file %s.\n", 
					ifile);
				fprintf(stderr, "errno is %d.\n", errno);
				close(ifd);
				close(ofd);
				unlink(ofile);
				return(2);
			}

			// encrypt
			Inew = IRANDOM(Iold);
			s = (unsigned short)(s ^ Inew);
			Iold = Inew;

			// write out
			int nw = write(ofd, &s, sizeof(short));
			if (nw != sizeof(short))
			{
				fprintf(stderr, "unable to write file %s.\n", 
					ofile);
				fprintf(stderr, "errno is %d.\n", errno);
				close(ifd);
				close(ofd);
				unlink(ofile);
				return(2);
			}
		}

		// third, read in odd byte
		if (byteToRead > 0)
		{
			// read in a buffer
			unsigned char c;
			int nr = read(ifd, &c, sizeof(unsigned char));
			if (nr != sizeof(unsigned char))
			{
				fprintf(stderr, "unable to read file %s.\n", 
					ifile);
				fprintf(stderr, "errno is %d.\n", errno);
				close(ifd);
				close(ofd);
				unlink(ofile);
				return(2);
			}

			// encrypt
			Inew = IRANDOM(Iold);
			c = (unsigned char)(c ^ Inew);
			Iold = Inew;

			// write out
			int nw = write(ofd, &c, sizeof(unsigned char));
			if (nw != sizeof(unsigned char))
			{
				fprintf(stderr, "unable to write file %s.\n", 
					ofile);
				fprintf(stderr, "errno is %d.\n", errno);
				close(ifd);
				close(ofd);
				unlink(ofile);
				return(2);
			}
		}
	
		// close input file
		close(ifd);
		close(ofd);
	
		// unlink old file, and link new file
		unlink(ifile);
		link(ofile, ifile);
		unlink(ofile);
	}

	// all done
	return(0);
}

