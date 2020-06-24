/* program to read a file and split into component files */

/* unix headers */
#include <stdio.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>
#ifdef M3B
#include <dtype.h>
#include <lla/lla.h>
#include <lla/log_sub.h>
#endif
#include <sys/stat.h>

/* local headers */
#include "returns.h"
#include "debug.h"
#include "mtio.h"
#include "blockio.h"
#include "hash.h"
#ifdef M3B
#include "crc.h"
#endif

/* local definitions */
#define VERSION "version 1.0 - 01/18/95\n"
#define USAGEFORMAT \
	"usage: %s [-?IVS] [-B blocks_in_buffer] [-b block_size] [-t tape_drive | -i input_file]\n"

/* externs */
extern char *optarg;
extern int optind;
extern int errno;

/* usage message */
void
usage(arg0)
char *arg0;
{
	fprintf(stderr, USAGEFORMAT, arg0);
	return;
}

/* check command line parameters */
int
checkParameters(blocksInBuffer, blockSize, tapeNumber, inputFile)
long blocksInBuffer;
long blockSize;
long tapeNumber;
char *inputFile;
{
	int status;

	/* assume success */
	status = OK;

	/* check blocks in buffer */
	if (blocksInBuffer <= 0)
	{
		errno = EINVAL;
		ERROR("blocksInBuffer size <= 0.", errno);
		status = NOTOK;
	}

	/* check block size */
	if (blockSize <= 0)
	{
		errno = EINVAL;
		ERROR("block size <= 0.", errno);
		status = NOTOK;
	}

	/* verify an input device was given */
	if (tapeNumber >= 0)
	{
		*inputFile = 0;
#ifdef M3B
		sprintf(inputFile, "/dev/mt%ld0", tapeNumber);
#else
		sprintf(inputFile, "/dev/rmt/%ldh", tapeNumber);
#endif
	}
	if (*inputFile == 0)
	{
		errno = EINVAL;
		ERROR("no input file was given.", errno);
		status = NOTOK;
	}

	/* all done */
	return(status);
}

/* does a file exist */
int
exists(toFilePath)
char *toFilePath;
{
	/* does file exist */
	return(access(toFilePath, 0) == 0);
}

#ifdef M3B
/* get mode for file */
unsigned long
getMode(toFilePath)
char *toFilePath;
{
	struct stat statbuf;

	/* get file data */
	if (stat(toFilePath, &statbuf) != OK)
	{
		/* assume file does not exist */
		return(NOTOK);
	}

	/* return file mode */
	return(statbuf.st_mode & 07777);
}

/* is file contiguous */
int
contiguous(toFilePath)
char *toFilePath;
{
	struct stat statbuf;

	/* get file data */
	if (stat(toFilePath, &statbuf) != OK)
	{
		/* assume file does not exist */
		return(0);
	}

	/* return if file is contiguous */
	return(!(((statbuf.st_mode & S_IFMT) != S_IF1EXT) &&
	       ((statbuf.st_mode & S_IFMT) != S_IFEXT)));
}

/* check if file is large enough */
int
bigEnough(toFilePath, sizeInBytes)
char *toFilePath;
long sizeInBytes;
{
#if 0
	struct stat statbuf;

	/* get file data */
	if (stat(toFilePath, &statbuf) != OK)
	{
		/* assume file is not large enough */
		return(0);
	}

	/* return if file is large enough */
	return(statbuf.st_size >= sizeInBytes);
#else
	extern long filespace;
	int fd;

	/* open file for read */
	if ((fd = open(toFilePath, READONLY)) == NOTOK)
	{
		/* assume file is not large enough */
		return(0);
	}

	/* set size of allocated file */
	if (fsize(fd) < 0)
	{
		/* assume file is not large enough */
		close(fd);
		return(0);
	}

	/* clean up */
	close(fd);

	/* return if file is large enough */
	return(filespace >= sizeInBytes);
#endif
}
#endif

/* create a contiguous file */
int
createFile(toFilePath, sizeInBytes, mode)
char *toFilePath;
long sizeInBytes;
unsigned long mode;
{
	int fd;

#ifdef M3B
	/* allocate a contiguous file */
	if ((fd = falloc(toFilePath, S_IF1EXT | mode, sizeInBytes)) < 0)
	{
		ERRORS("unable to falloc file.", toFilePath, errno);
		return(NOTOK);
	}
#else
	/* create a file */
	sizeInBytes;
	if ((fd = creat(toFilePath, mode)) < 0)
	{
		ERRORS("unable to create file.", toFilePath, errno);
		return(NOTOK);
	}
#endif
	close(fd);

	/* all done */
	return(OK);
}

/* check if a file exists */
int
checkFile(toFilePath, sizeInBytes, sizeInBlocks)
char *toFilePath;
long sizeInBytes;
long sizeInBlocks;
{
	unsigned long mode;

	/* initialize */
	sizeInBlocks;
	mode = DEFAULTMODE;

	/* does file exist */
	if ( ! exists(toFilePath)) 
	{
		return(createFile(toFilePath, sizeInBytes, mode));
	}

#ifdef M3B
	/* get mode of present file */
	if ((mode = getMode(toFilePath)) == (unsigned long)NOTOK)
	{
		ERRORS("unable to get mode for file.", toFilePath, errno);
		return(NOTOK);
	}

	/* is file contiguous */
	if ( ! contiguous(toFilePath)) 
	{
		unlink(toFilePath);
		return(createFile(toFilePath, sizeInBytes, mode));
	}

	/* is file large enough */
	if ( ! bigEnough(toFilePath, sizeInBytes)) 
	{
		unlink(toFilePath);
		return(createFile(toFilePath, sizeInBytes, mode));
	}
#endif

	/* all done */
	return(OK);
}

/* read in file and install */
int
readAndInstallFile(inFd, blockSize, 
		useHashSums, ignoreHashSums, hashSum, 
		fromFilePath, toFilePath, 
		sizeInBytes, sizeInBlocks, blocksInBuffer)
struct BlockIOFile *inFd;
long blockSize;
long useHashSums;
int ignoreHashSums;
unsigned short *hashSum;
char *fromFilePath;
char *toFilePath;
long sizeInBytes;
long sizeInBlocks;
long blocksInBuffer;
{
	int outFd;
	char *workBuffer, *lastslash;
	long iblock, extraBytes;

	/* check parameters and initialize */
	errno = EINVAL;
	if (blockSize <= 0)
	{
		ERROR("block size <= 0.", errno);
		return(NOTOK);
	}
	if (fromFilePath == (char *)0 || *fromFilePath == 0)
	{
		ERROR("invalid source file path.", errno);
		return(NOTOK);
	}
	if (toFilePath == (char *)0 || *toFilePath == 0)
	{
		ERROR("invalid destination file path.", errno);
		return(NOTOK);
	}
	if (sizeInBytes <= 0 || sizeInBlocks <= 0)
	{
		ERROR("invalid file sizes in bytes or blocks.", errno);
		return(NOTOK);
	}
	*hashSum = 0;

	/* allocate buffer to read and write file */
	if ((workBuffer = malloc(blockSize)) == (char *)0)
	{
		ERROR("unable to malloc work buffer.", errno);
		return(NOTOK);
	}

	/* check if directory exists */
	if ((lastslash = strrchr(toFilePath, SLASH)) != (char *)0)
	{
		/* check if directory exists */
		*lastslash = 0;
		if (access(toFilePath, 02) != OK)
		{
			/* directory does not exist, make it */
			if (makeDir(toFilePath, 0755) != OK)
			{
				ERRORS("unable to make directory.", 
					toFilePath, errno);
				return(NOTOK);
			}
		}
		*lastslash = SLASH;
	}

	/* tell user the file that is being written */
	PRINTNEWLINE();
	fprintf(stderr, "Writing file %s from %s.\n", toFilePath, fromFilePath);

	/* check if file exists and if large enough */
	if (checkFile(toFilePath, sizeInBytes, sizeInBlocks) != OK)
	{
		ERRORS("unable to create file.", toFilePath, errno);
		return(NOTOK);
	}

	/* open output file */
	if ((outFd = bufferOpenFile(toFilePath, 
		blocksInBuffer*blockSize, WRITEONLY)) == NOTOK)
	{
		ERRORS("unable to open file.", toFilePath, errno);
		return(NOTOK);
	}

	/* check if any odd bytes to read */
	if ((extraBytes = sizeInBytes%blockSize) > 0)
	{
		/* read in extra bytes separately */
		sizeInBlocks--;
	}

	/* read in file and install file */
	for (iblock = 1; iblock <= sizeInBlocks; iblock++)
	{
		/* read from input file */
		if (blockRead(inFd, workBuffer, blockSize) != OK)
		{
			close(outFd);
			free(workBuffer);
			ERROR("unable to read a block from input file.", errno);
			return(NOTOK);
		}

		/* check if hash sums are being used */
		if (useHashSums && !ignoreHashSums &&
		   (hash(hashSum, workBuffer, blockSize) != OK))
		{
			close(outFd);
			free(workBuffer);
			ERRORS("failed to calculate hash sum.", 
				toFilePath, errno);
			return(NOTOK);
		}

		/* write to output file */
		if (bufferWrite(outFd, workBuffer, blockSize) == NOTOK)
		{
			close(outFd);
			free(workBuffer);
			ERRORS("unable to write a block to output file.", 
				toFilePath, errno);
			return(NOTOK);
		}
	}

	/* check for any unread bytes */
	if (extraBytes > 0)
	{
		/* read from input file */
		if (blockRead(inFd, workBuffer, blockSize) != OK)
		{
			close(outFd);
			free(workBuffer);
			ERROR("unable to read a block from input file.", errno);
			return(NOTOK);
		}

		/* check if hash sum are being used */
		if (useHashSums && !ignoreHashSums &&
		   (hash(hashSum, workBuffer, extraBytes) != OK))
		{
			close(outFd);
			free(workBuffer);
			ERRORS("failed to calculate hash sum.", 
				toFilePath, errno);
			return(NOTOK);
		}

		/* write to output file */
		if (bufferWrite(outFd, workBuffer, extraBytes) == NOTOK)
		{
			close(outFd);
			free(workBuffer);
			ERRORS("unable to write a block to output file.", 
				toFilePath, errno);
			return(NOTOK);
		}
	}
	PRINTNEWLINE();

	/* clean up */
	bufferCloseFile(outFd);
	free(workBuffer);

	/* all done */
	return(OK);
}

/* read the next file from the input and install */
int
installFile(inFd, ignoreHashSums, useHashSums, blockSize, blocksInBuffer)
struct BlockIOFile *inFd;
int ignoreHashSums;
long useHashSums;
long blockSize;
long blocksInBuffer;
{
	struct RecordHeader rhdr;
	struct HashSumRecord hshdr;
	unsigned short hashSum;

	/* check parameters */
	if (blockSize <= 0 || blocksInBuffer <= 0)
	{
		errno = EINVAL;
		ERROR("block size <= 0 or blocksInBuffer <= 0.", errno);
		return(NOTOK);
	}

	/* read next record header */
	PRINTNEWLINE();
	fprintf(stderr, "Reading next record header from input file.\n");
	if (blockRead(inFd, &rhdr, sizeof(struct RecordHeader)) != OK)
	{
		ERROR("unable to read record header.", errno);
		return(NOTOK);
	}

	/* read and write out next file */
	if (readAndInstallFile(inFd, blockSize, 
		useHashSums, ignoreHashSums, &hashSum,
		rhdr.fromFilePath, rhdr.toFilePath,
		rhdr.sizeInBytes, rhdr.sizeInBlocks, blocksInBuffer) != OK)
	{
		ERRORS("unable to install file.", rhdr.toFilePath, errno);
		return(NOTOK);
	}

	/* read hash sum record */
	if (useHashSums)
	{
		PRINTNEWLINE();
		fprintf(stderr, "Reading hashsum record for file.\n");
		if (blockRead(inFd, &hshdr, sizeof(struct HashSumRecord)) != OK)
		{
			ERROR("unable to read hashsum record.", errno);
			return(NOTOK);
		} 
		if (ignoreHashSums)
		{
			PRINTNEWLINE();
			fprintf(stderr, 
				"IGNORING hashsum for file %s.\n", 
				rhdr.toFilePath);
			PRINTNEWLINE();
		}
		else
		{
			PRINTNEWLINE();
			fprintf(stderr, 
			"FILE: %s, calculated sum (%x), header sum (%x)\n",
			rhdr.toFilePath, hshdr.hashSum, hashSum);
			if (hshdr.hashSum != hashSum)
			{
				errno = EINVAL;
				ERRORS("HASHSUM ERROR !!!.", 
					rhdr.toFilePath, errno);
				return(NOTOK);
			}
			fprintf(stderr, "Hashsum OK for file %s.\n", 
				rhdr.toFilePath);
			PRINTNEWLINE();
		}
	}

#ifdef M3B
	/* check if file is in crc file */
	crc_check(rhdr.toFilePath);
#endif

	/* all done */
	return(OK);
}

/* main entry point */
main(argc, argv)
int argc;
char **argv;
{
	int c, setioFlag, ignoreHashSums;
	long ifile, tapeNumber, blockSize, blocksInBuffer;
	struct BlockIOFile* inFd;
	char inputFile[MAXFILEPATHSIZE];
	struct FileHeader fhdr;

	/* initialize parameters */
	*inputFile = 0;
	tapeNumber = -1;
	blockSize = DEFAULTBLOCKSIZE;
	blocksInBuffer = 20;
	setioFlag = 0;
	ignoreHashSums = 0;

	/* check command line arguements */
	while ((c = getopt(argc, argv, "?ISVb:t:i:B:")) != EOF)
	{
		/* get option */
		switch (c)
		{
		case 'I':
			/* ignore hash sums */
			ignoreHashSums = 1;
			break;

		case 'S':
			/* set io flag is true */
			setioFlag = 1;
			break;

		case 'B':
			/* blocks in buffer */
			blocksInBuffer = atol(optarg);
			break;

		case 'b':
			/* block size */
			blockSize = atol(optarg);
			break;

		case 't':
			/* tape drive number */
			tapeNumber = atol(optarg);
			break;

		case 'i':
			/* input file */
			strcpy(inputFile, optarg);
			break;

		case '?':
			/* help message */
			usage(argv[0]);
			exit(0);

		case 'V':
			/* version */
			fprintf(stderr, VERSION);
			exit(0);

		default:
			/* invalid option */
			ERROR("invalid command line option.", EINVAL);
			usage(argv[0]);
			exit(2);
		}
	}

	/* check required parameters */
	if (checkParameters(blocksInBuffer, 
			blockSize, tapeNumber, inputFile) != OK)
	{
		ERROR("invalid parameters, use -? option for help.", EINVAL);
		exit(2);
	}

	/* open input file for read */
	PRINTNEWLINE();
	fprintf(stderr, "Opening input file %s for read.\n", inputFile);
	inFd = blockOpenFile(inputFile, blockSize, READONLY);
	if (inFd == (struct BlockIOFile *)0)
	{
		ERRORS("failed to open input file.", inputFile, errno);
		exit(2);
	}

#ifdef M3B
	/* is input device a magnetic tape */
	if (tapeNumber > -1 || setioFlag)
	{
		if (blockSetIO(inFd, 1) != OK)
		{
			blockCloseFile(inFd);
			ERROR("unable to set I/O mode.", errno);
			exit(2);
		}
	}
#endif

	/* read in file header */
	PRINTNEWLINE();
	fprintf(stderr, "Reading file header for input file %s.\n", inputFile);
	if (blockRead(inFd, &fhdr, sizeof(struct FileHeader)) != OK)
	{
		blockCloseFile(inFd);
		ERROR("unable to read file header.", errno);
		exit(2);
	}

#ifdef M3B
	/* initialize crc table */
	crc_open(CRCFILEPATH);
#endif

	/* cycle thru input file and install individual files */
	for (ifile = 1; ifile <= fhdr.numberOfFiles; ifile++)
	{
		/* read the next file from input and install */
		if (installFile(inFd, ignoreHashSums, fhdr.useHashSums,
				blockSize, blocksInBuffer) != OK)
		{
			blockCloseFile(inFd);
			ERROR("unable to install a file.", errno);
			exit(2);
		}
	}

	/* close input file */
	blockCloseFile(inFd);

#ifdef M3B
	/* close crc table and update crc values file */
	crc_close();
#endif

	/* let user know that writing is done */
	fprintf(stderr, "Finished reading input file %s.\n", inputFile);

	/* all done */
	exit(0);
}
