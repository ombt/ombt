/* program to write a list of file into one file */

/* unix headers */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <malloc.h>
#include <memory.h>
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

/* local definitions */
#define VERSION "version 1.0 - 01/18/95\n"
#define USAGEFORMAT \
	"usage: %s [-?HVS] [-b block_size] [-t tape_drive | -o output_file] -l list_of_files\n"

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
checkParameters(blockSize, inputFile, tapeNumber, outputFile)
long blockSize;
char *inputFile;
long tapeNumber;
char *outputFile;
{
	int status;

	/* assume success */
	status = OK;

	/* verify block size is valid */
	if (blockSize <= 0)
	{
		errno = EINVAL;
		ERROR("block size <= 0.", errno);
		status = NOTOK;
	}

	/* verify that a list of files was given and that it exists */
	if (*inputFile == 0)
	{
		errno = EINVAL;
		ERROR("no input file was given.", errno);
		status = NOTOK;
	}
	else if (access(inputFile, 04) != OK)
	{
		ERRORS("input file does not exist.", inputFile, errno);
		status = NOTOK;
	}

	/* verify an output device was given */
	if (tapeNumber >= 0)
	{
		*outputFile = 0;
#ifdef M3B
		sprintf(outputFile, "/dev/mt%ld0", tapeNumber);
#else
		sprintf(outputFile, "/dev/rmt/%ldh", tapeNumber);
#endif
	}
	if (*outputFile == 0)
	{
		errno = EINVAL;
		ERROR("no output file was given.", errno);
		status = NOTOK;
	}

	/* all done */
	return(status);
}

/* read input file and generate a list of files */
int
getListOfFiles(listOfFiles, inputFile)
struct ListOfFiles **listOfFiles;
char *inputFile;
{
	int ifile;
	FILE *inFd;
	char tmpBuffer[BUFSIZ+1];

	/* check input parameters */
	if ((listOfFiles == (struct ListOfFiles **)0) ||
	    (inputFile == (char *)0) || (*inputFile == 0))
	{
		errno = EINVAL;
		ERROR("invalid parameter for getListOfFiles.", errno);
		return(NOTOK);
	}

	/* allocate list-of-files structure */
	*listOfFiles = 
		(struct ListOfFiles *) malloc(sizeof(struct ListOfFiles));
	if (*listOfFiles == (struct ListOfFiles *)0)
	{
		errno = ENOMEM;
		ERROR("unable to malloc list-of-files structure.", errno);
		return(NOTOK);
	}

	/* open input file for read */
	if ((inFd = fopen(inputFile, "r")) == (FILE *)0)
	{
		ERRORS("unable to open input file.", inputFile, errno);
		return(NOTOK);
	}

	/* find out how many files are to be copied */
	for ((*listOfFiles)->numberOfFiles = 0, fgets(tmpBuffer, BUFSIZ, inFd);
	    (!feof(inFd) && !ferror(inFd));
	    (*listOfFiles)->numberOfFiles++, fgets(tmpBuffer, BUFSIZ, inFd)) ;

	/* rewind file to start */
	rewind(inFd);

	/* check if any files to read */
	if ((*listOfFiles)->numberOfFiles <= 0)
	{
		fclose(inFd);
		errno = EINVAL;
		ERROR("no files to copy.", errno);
		return(NOTOK);
	}

	/* allocate file name buffers */
	(*listOfFiles)->sourceFiles = (char (*)[MAXFILEPATHSIZE]) 
	malloc(sizeof(char [MAXFILEPATHSIZE])*(*listOfFiles)->numberOfFiles);
	if ((*listOfFiles)->sourceFiles == (char (*)[MAXFILEPATHSIZE])0)
	{
		fclose(inFd);
		errno = ENOMEM;
		ERROR("unable to malloc source file list.", errno);
		return(NOTOK);
	}
	(*listOfFiles)->destinationFiles = (char (*)[MAXFILEPATHSIZE]) 
	malloc(sizeof(char [MAXFILEPATHSIZE])*(*listOfFiles)->numberOfFiles);
	if ((*listOfFiles)->destinationFiles == (char (*)[MAXFILEPATHSIZE])0)
	{
		fclose(inFd);
		errno = ENOMEM;
		ERROR("unable to malloc destination file list.", errno);
		return(NOTOK);
	}

	/* read input file and parse records */
	for (ifile = 0; ifile < (*listOfFiles)->numberOfFiles; ifile++)
	{
		/* read a line from input file */
		if (fscanf(inFd, "%s%s", 
			(*listOfFiles)->destinationFiles[ifile],
			(*listOfFiles)->sourceFiles[ifile]) != 2)
		{
			fclose(inFd);
			ERROR("unable to read src/dest file names.", errno);
			return(NOTOK);
		}
	}

	/* close input file */
	fclose(inFd);

	/* all done */
	return(OK);
}

/* write file header to output file */
int
wrFileHeader(outFd, useHashSums, numberOfFiles)
struct BlockIOFile *outFd;
int useHashSums;
long numberOfFiles;
{
	struct FileHeader fhdr;

	/* check parameters */
	if (outFd == (struct BlockIOFile *)0)
	{
		errno = EINVAL;
		ERROR("output file descriptor = 0.", errno);
		return(NOTOK);
	}
	if (numberOfFiles <= 0)
	{
		errno = EINVAL;
		ERROR("number of files <= 0.", errno);
		return(NOTOK);
	}

	/* store data in header */
	fhdr.numberOfFiles = numberOfFiles;

	/* store whether or not hash sums are stored */
	fhdr.useHashSums = useHashSums;

	/* tell user that file header is being written */
	PRINTNEWLINE();
	fprintf(stderr, "Writing file header to output file.\n");

	/* write file header to output file */
	if (blockWrite(outFd, &fhdr, sizeof(struct FileHeader)) != OK)
	{
		ERROR("unable to write file header.", errno);
		return(NOTOK);
	}
	PRINTNEWLINE();

	/* all done */
	return(OK);
}

/* return size of a file */
long
fileSize(fileName)
char *fileName;
{
	struct stat statbuf;

	/* get file data */
	if (stat(fileName, &statbuf) != OK)
	{
		ERRORS("unable to stat file.", fileName, errno);
		return(NOTOK);
	}

	/* return file size */
	return(statbuf.st_size);
}

/* fill in record header */
int
fillRecordHeader(recHdr, blockSize, sourceFile, destinationFile)
struct RecordHeader *recHdr;
long blockSize;
char *sourceFile;
char *destinationFile;
{
	/* check parameters */
	if (recHdr == (struct RecordHeader *)0 || blockSize <= 0 || 
	    sourceFile == (char *)0 || *sourceFile == 0 ||
	    destinationFile == (char *)0 || *destinationFile == 0)
	{
		errno = EINVAL;
		ERROR("invalid parameters for filling record header.", errno);
		return(NOTOK);
	}

	/* verify that source file exists */
	if (access(sourceFile, 04) != OK)
	{
		ERRORS("input file does not exist.", sourceFile, errno);
		return(NOTOK);
	}

	/* initialize record header */
	strcpy(recHdr->fromFilePath, sourceFile);
	strcpy(recHdr->toFilePath, destinationFile);
	if ((recHdr->sizeInBytes = fileSize(sourceFile)) <= 0)
	{
		ERRORS("input file has zero length.", sourceFile, errno);
		return(NOTOK);
	}
	recHdr->sizeInBlocks = recHdr->sizeInBytes/blockSize;
	if (recHdr->sizeInBytes%blockSize != 0)
		recHdr->sizeInBlocks++;

	/* all done */
	return(OK);
}

/* copy input file to output file */
int
copyFile(outFd, useHashSums, blockSize, hashSum, recHdr)
struct BlockIOFile *outFd;
int useHashSums;
long blockSize;
unsigned short *hashSum;
struct RecordHeader *recHdr;
{
	int inFd;
	char *outputBuffer;
	long iblock, bytesLeftToRead, sizeInBlocks;

	/* check parameters and initialize */
	if (blockSize <= 0 || recHdr == (struct RecordHeader *)0 ||
	    outFd == (struct BlockIOFile *)0 || 
	    hashSum == (unsigned short *)0)
	{
		errno = EINVAL;
		ERROR("invalid parameters for copying file.", errno);
		return(NOTOK);
	}
	*hashSum = 0;

	/* allocate io buffer */
	if ((outputBuffer = malloc(blockSize)) == (char *)0)
	{
		ERROR("unable to malloc an output buffer.", errno);
		return(NOTOK);
	}

	/* open input file for read only */
	if ((inFd = open(recHdr->fromFilePath, READONLY)) == NOTOK)
	{
		free(outputBuffer);
		ERRORS("unable to open file.", recHdr->fromFilePath, errno);
		return(NOTOK);
	}

	/* tell user that file is written */
	PRINTNEWLINE();
	fprintf(stderr, "Writing file %s to output file.\n", 
		recHdr->fromFilePath);

	/* check if any odd bytes must be read */
	sizeInBlocks = recHdr->sizeInBlocks;
	if ((bytesLeftToRead = recHdr->sizeInBytes%blockSize) > 0)
	{
		/* don't read in last partial block */
		sizeInBlocks--;
	}

	/* read input file and copy to output file */
	for (iblock = 1; iblock <= sizeInBlocks; iblock++)
	{
		/* read next block from input file */
		if (read(inFd, outputBuffer, blockSize) == NOTOK)
		{
			free(outputBuffer);
			close(inFd);
			ERRORS("unable to read file.", 
				recHdr->fromFilePath, errno);
			return(NOTOK);
		}

		/* check if hash sum are being used */
		if (useHashSums && 
		   (hash(hashSum, outputBuffer, blockSize) != OK))
		{
			free(outputBuffer);
			close(inFd);
			ERRORS("failed to calculate hash sum.", 
				recHdr->fromFilePath, errno);
			return(NOTOK);
		}

		/* write block to output file */
		if (blockWrite(outFd, outputBuffer, blockSize) != OK)
		{
			free(outputBuffer);
			close(inFd);
			ERROR("unable to write to output file.", errno);
			return(NOTOK);
		}
	}

	/* check if any left over bytes to read */
	if (bytesLeftToRead > 0)
	{
		/* clear output buffer */
		memset(outputBuffer, 0, blockSize);

		/* read remaining bytes from input file */
		if (read(inFd, outputBuffer, bytesLeftToRead) == NOTOK)
		{
			free(outputBuffer);
			close(inFd);
			ERRORS("unable to read file.", 
				recHdr->fromFilePath, errno);
			return(NOTOK);
		}

		/* check if hash sum are being used */
		if (useHashSums &&
		   (hash(hashSum, outputBuffer, bytesLeftToRead) != OK))
		{
			free(outputBuffer);
			close(inFd);
			ERRORS("failed to calculate hash sum.", 
				recHdr->fromFilePath, errno);
			return(NOTOK);
		}

		/* write a full block to output file */
		if (blockWrite(outFd, outputBuffer, bytesLeftToRead) != OK)
		{
			free(outputBuffer);
			close(inFd);
			ERROR("unable to write to output file.", errno);
			return(NOTOK);
		}
	}
	PRINTNEWLINE();

	/* free buffer and close file */
	close(inFd);
	free(outputBuffer);

	/* all done */
	return(OK);
}

/* copy input file to output file */
int
writeFile(outFd, useHashSums, blockSize, sourceFile, destinationFile)
struct BlockIOFile *outFd;
int useHashSums;
long blockSize;
char *sourceFile;
char *destinationFile;
{
	struct RecordHeader recHdr;
	struct HashSumRecord hsumHdr;

	/* let user know which files are processing */
	PRINTNEWLINE();
	fprintf(stderr, "Processing files : \n");
	fprintf(stderr, "source file      : %s\n", sourceFile);
	fprintf(stderr, "destination file : %s\n", destinationFile);

	/* populate record header */
	if (fillRecordHeader(&recHdr, blockSize, 
		sourceFile, destinationFile) != OK)
	{
		ERRORS("unable to fill record header.", sourceFile, errno);
		return(NOTOK);
	}

	/* write record header to output file */
	PRINTNEWLINE();
	fprintf(stderr, "Writing record header for %s.\n", recHdr.fromFilePath);
	if (blockWrite(outFd, &recHdr, sizeof(struct RecordHeader)) != OK)
	{
		ERRORS("unable to write record header.", sourceFile, errno);
		return(NOTOK);
	}
	PRINTNEWLINE();

	/* copy input file to output file */
	if (copyFile(outFd, useHashSums, 
		blockSize, &hsumHdr.hashSum, &recHdr) != OK)
	{
		ERRORS("unable to copy file.", sourceFile, errno);
		return(NOTOK);
	}

	/* write hash sum header to output file */
	if (useHashSums)
	{
		PRINTNEWLINE();
		fprintf(stderr, "Writing hashsum header (%x) for %s.\n", 
			(unsigned int) hsumHdr.hashSum, recHdr.fromFilePath);
		if (blockWrite(outFd, &hsumHdr, 
				sizeof(struct HashSumRecord)) != OK)
		{
			ERRORS("unable to write hashsum record.", 
				sourceFile, errno);
			return(NOTOK);
		}
		PRINTNEWLINE();
	}

	/* all done */
	return(OK);
}

/* main entry point */
main(argc, argv)
int argc;
char **argv;
{
	int c, ifile, setioFlag, useHashSums;
	struct BlockIOFile *outFd;
	long tapeNumber, blockSize;
	struct ListOfFiles *listOfFiles;
	char inputFile[MAXFILEPATHSIZE];
	char outputFile[MAXFILEPATHSIZE];

	/* initialize parameters */
	*inputFile = 0;
	*outputFile = 0;
	tapeNumber = -1;
	blockSize = DEFAULTBLOCKSIZE;
	outFd = (struct BlockIOFile *)0;
	setioFlag = 0;
	useHashSums = 0;

	/* check command line arguements */
	while ((c = getopt(argc, argv, "?HVSt:l:o:b:")) != EOF)
	{
		/* get option */
		switch (c)
		{
		case 'H':
			/* use hashsums */
			useHashSums = 1;
			break;

		case 'S':
			/* set io flag to true */
			setioFlag = 1;
			break;

		case 't':
			/* tape drive number */
			tapeNumber = atol(optarg);
			break;

		case 'l':
			/* file containing list of files to read */
			strcpy(inputFile, optarg);
			break;

		case 'o':
			/* output file */
			strcpy(outputFile, optarg);
			break;

		case 'b':
			/* block size */
			blockSize = atol(optarg);
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
	if (checkParameters(blockSize, inputFile, tapeNumber, outputFile) != OK)
	{
		ERROR("invalid parameters, use -? option for help.", errno);
		exit(2);
	}

	/* get list of files to copy to output file */
	if (getListOfFiles(&listOfFiles, inputFile) != OK)
	{
		ERROR("failed to get list of files.", errno);
		exit(2);
	}

	/* open output file for write */
	PRINTNEWLINE();
	fprintf(stderr, "Opening output file %s for write.\n", outputFile);
	outFd = blockOpenFile(outputFile, blockSize, CREATEFILE);
	if (outFd == (struct BlockIOFile *)0)
	{
		ERROR("failed to open output file.", errno);
		exit(2);
	}

#ifdef M3B
	/* is output device a magnetic tape */
	if (tapeNumber > -1 || setioFlag)
	{
		if (blockSetIO(outFd, 1) != OK)
		{
			blockCloseFile(outFd);
			ERROR("unable to set I/O mode.", errno);
			exit(2);
		}
	}
#endif

	/* write file header to output file */
	if (wrFileHeader(outFd, useHashSums, listOfFiles->numberOfFiles) != OK)
	{
		ERROR("unable to write output file header.", errno);
		blockCloseFile(outFd);
		exit(2);
	}

	/* cycle through list of files and write to output file */
	for (ifile = 0; ifile < listOfFiles->numberOfFiles; ifile++)
	{
		/* copy file to output file */
		if (writeFile(outFd, useHashSums, blockSize, 
			listOfFiles->sourceFiles[ifile], 
			listOfFiles->destinationFiles[ifile]) != OK)
		{
			ERROR("unable to write output file.", errno);
			blockCloseFile(outFd);
			exit(2);
		}
	}

	/* close output file */
	blockCloseFile(outFd);

	/* let user know that writing is done */
	PRINTNEWLINE();
	fprintf(stderr, "Finished writing output file %s.\n", outputFile);

	/* all done */
	exit(0);
}
