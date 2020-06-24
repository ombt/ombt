// my simple encryption program. very simple.

//
// REMEMBER: if you change this program, any previously encrypted
// files will not be decrypted if you use the new program !!!
//

// headers
#include <sysent.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
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

// other constants
const int base = 7;

// externs
extern int optind;
extern char *optarg;

// definitions
#define IRANDOM(iold) (((aR)*(iold)+(cR))%(mR))
#define USAGE "usage: %s [-?V] [-P passwd ] filename ...\n"

// convert pass word to a number
unsigned long
MYatoi(char *pw)
{
	// check argument
	assert(pw != NULL && *pw != 0);

	// find end of pass word
	char *pwend = pw + strlen(pw) - 1;

	// convert to a number
	int key = 0;
	int multiplier = 1;
	for ( ; pwend >= pw; pwend--, multiplier*=base)
	{
		key += (*pwend)*multiplier;
	}

	// return key
	return(key);
}

// main entry point
main(int argc, char **argv)
{
	int verbose = 0;
	char pword[BUFSIZ], ifile[BUFSIZ], ofile[BUFSIZ];

	// check for options
	pword[0] = '\0';
	for (int c = 0; (c = getopt(argc, argv, "?VP:")) != EOF; )
	{
		switch (c)
		{
		case '?':
			// usage
			fprintf(stderr, USAGE, argv[0]);
			exit(2);

		case 'V':
			// turn on verbose
			verbose = 1;
			break;

		case 'P':
			// passwd from command line
			strcpy(pword, optarg);
			break;

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

	// get key from user
	char *pkey, key1[BUFSIZ], key2[BUFSIZ];
	if(strlen(pword) == 0) 
	{
		if ((pkey = getpass("enter key: ")) == NULL)
		{
			fprintf(stderr, "bad key was entered.\n");
			exit(2);
		}
		strcpy(key1, pkey);
		if ((pkey = getpass("enter key again: ")) == NULL)
		{
			fprintf(stderr, "bad key was entered.\n");
			exit(2);
		}
		strcpy(key2, pkey);
		if (strcmp(key1, key2) != 0)
		{
			fprintf(stderr, "keys don't match.\n");
			exit(2);
		}
	}
	else
	{
		strcpy(key1, pword);
	}
	Ikey = MYatoi(key1);

	// make sure it is odd.
	Ikey |= 1;

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

		// get file names
		strcpy(ifile, argv[optind]);
		sprintf(ofile, "tmp%d.%d", optind, getpid());

		// let user know the current file 
		if (verbose) 
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

