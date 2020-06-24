/* my version of bsplit */

/* required headers */
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

/* local definitions */
#define BLOCKS_PARAM (optind)
#define FILENAME_PARAM (optind+1)
#define BITSPERLONG 32

/* externs and globals */
extern int optind;
extern char *optarg;

/* usage message */
void
usage(cmd)
char *cmd;
{
	fprintf(stderr, "usage: %s [-?] [-b blocksize] blocks filename\n", cmd);
	return;
}

/* check if file descriptor is a pipe */
int
isAPipe(fd)
int fd;
{
	struct stat statbuf;

	/* get file info */
	if (fstat(fd, &statbuf) != 0) return(0);

	/* check if a pipe */
	return((statbuf.st_mode & S_IFMT) == S_IFIFO);
}

/* convert a number to a power of two */
long 
nearestPowerOf2(number)
long number;
{
	int power;
	for (power = 0; power < BITSPERLONG; power++)
	{
		if (number <= (1L << power))
		{
			return(1L << power);
		}
	}
	return(-1);
}

/* main entry point */
main(argc, argv)
int argc;
char **argv;
{
	int outFd;
	int c, writtenInto, nr, blocksRead;
	int blocksPerFile;
	int nextFileNo;
	long blockSize;
	char fileName[BUFSIZ];
	char outFileName[BUFSIZ];
	char *inBuffer;

	/* initialize variables */
	blockSize = BUFSIZ;

	/* get command line options */
	while ((c = getopt(argc, argv, "?b:")) != EOF)
	{
		switch (c)
		{
		case 'b':
			/* get block size */
			if ((blockSize = nearestPowerOf2(atol(optarg))) <= 0)
			{
				fprintf(stderr, "invalid block size.\n");
				exit(2);
			}
			if (blockSize > BUFSIZ && isAPipe(fileno(stdin)))
			{
				fprintf(stderr, "stdin is a pipe.\n");
				fprintf(stderr, "setting block size to %d.\n",
					BUFSIZ);
				blockSize = BUFSIZ;
			}
			break;
		case '?':
			/* help message */
			usage(argv[0]);
			exit(0);
		default:
			fprintf(stderr, "invalid option.\n");
			usage(argv[0]);
			exit(2);
		}
	}

	/* check remaining parameters */
	if ((argc - optind) != 2)
	{
		fprintf(stderr, "missing parameters.\n");
		usage(argv[0]);
		exit(2);
	}

	/* get parameter data */
	blocksPerFile = atoi(argv[BLOCKS_PARAM]);
	if (blocksPerFile <= 0)
	{
		fprintf(stderr, "invalid block size.\n");
		usage(argv[0]);
		exit(2);
	}
	strcpy(fileName, argv[FILENAME_PARAM]);
	if (strlen(fileName) == 0)
	{
		fprintf(stderr, "invalid filename.\n");
		usage(argv[0]);
		exit(2);
	}

	/* generate the first file name */
	nextFileNo = 0;
	sprintf(outFileName, "%s.%02d", fileName, nextFileNo++);

	/* create output file */
	outFd = open(outFileName, (O_CREAT|O_RDWR|O_TRUNC), 0644);
	if (outFd == -1)
	{
		fprintf(stderr, "open failed for file %s, errno = %d.\n",
			outFileName, errno);
		exit(2);
	}
	writtenInto = 0;

	/* allocate input buffer */
	if ((inBuffer = malloc(blockSize)) == (char *)0)
	{
		fprintf(stderr, "unable to malloc input buffer, errno = %d\n",
			errno);
		unlink(outFileName);
		exit(2);
	}
	
	/* read stdin and write to output file */
	for (blocksRead = 0; 
	    (nr = read(fileno(stdin), inBuffer, (unsigned)blockSize)) > 0; )
	{
		/* increment number of buffers read */
		blocksRead++;

		/* check if enough buffers were read */
		if (blocksRead >= blocksPerFile)
		{
			/* write last record and close file */
			write(outFd, inBuffer, nr);
			close(outFd);

			/* get next file name */
			sprintf(outFileName, "%s.%02d", fileName, nextFileNo++);

			/* create output file */
			outFd = open(outFileName, (O_CREAT|O_RDWR|O_TRUNC), 
					0644);
			if (outFd == -1)
			{
				fprintf(stderr, 
				"open failed for file %s, errno = %d.\n",
				outFileName, errno);
				exit(2);
			}

			/* reset blocks read to zero */
			blocksRead = 0;
			writtenInto = 0;
		}
		else
		{
			/* write buffer out */
			writtenInto = 1;
			write(outFd, inBuffer, nr);
		}
	}


	/* close last file */
	close(outFd);

	/* is last file empty */
	if ( ! writtenInto) unlink(outFileName);

	/* all done */
	fprintf(stderr, "All done, Ma.\n");
	exit(0);
}
