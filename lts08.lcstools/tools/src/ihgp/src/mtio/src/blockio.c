/* file contains block i/o functions */

/* unix headers */
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <errno.h>

/* local headers */
#include "returns.h"
#include "debug.h"
#include "blockio.h"
#include "tio.h"

/* function to open a file for read or write */
struct BlockIOFile *
blockOpenFile(fileName, blockSz, operation)
char *fileName;
long blockSz;
int operation;
{
	int openOperation;
	struct BlockIOFile *blockFd;

	/* check parameters */
	if (fileName == (char *)0 || *fileName == 0)
	{
		errno = EINVAL;
		ERROR("invalid filename.", errno);
		return((struct BlockIOFile *)0);
	}
	if (operation < READONLY || operation > CREATEFILE)
	{
		errno = EINVAL;
		ERROR("invalid operation.", errno);
		return((struct BlockIOFile *)0);
	}
	if (blockSz <= 0)
	{
		errno = EINVAL;
		ERROR("block size <= 0.", errno);
		return((struct BlockIOFile *)0);
	}

	/* allocate a descriptor block */
	blockFd = (struct BlockIOFile *)malloc(sizeof(struct BlockIOFile));
	if (blockFd == (struct BlockIOFile *)0)
	{
		ERROR("unable to allocate a block IO file descriptor.", errno);
		return((struct BlockIOFile *)0);
	}

	/* save block size */
	blockFd->blockSize = blockSz;

	/* allocate read/write buffer */
	if ((blockFd->ioBuffer = malloc(blockFd->blockSize)) == (char *)0)
	{
		free(blockFd);
		ERROR("unable to malloc io buffer.", errno);
		return((struct BlockIOFile *)0);
	}

	/* open file */
	if ((openOperation = operation) == CREATEFILE)
		openOperation = WRITEONLY;
	if ((blockFd->ioFd = topen(fileName, openOperation)) == NOTOK)
	{
		if (errno == ENOENT && operation == CREATEFILE)
		{
			blockFd->ioFd = creat(fileName, CREATEMODE);
			if (blockFd->ioFd == NOTOK)
			{
				free(blockFd->ioBuffer);
				free(blockFd);
				ERRORS("create failed for file.", 
					fileName, errno);
				return((struct BlockIOFile *)0);
			}
		}
		else
		{
			free(blockFd->ioBuffer);
			free(blockFd);
			ERRORS("open failed for file.", fileName, errno);
			return((struct BlockIOFile *)0);
		}
	}

	/* all done */
	return(blockFd);
}

/* close a file */
int
blockCloseFile(blockFd)
struct BlockIOFile *blockFd;
{
	/* check parameters */
	if (blockFd != (struct BlockIOFile *)0)
	{
		/* close file and release all data */
		if (blockFd->ioFd != NOTOK) tclose(blockFd->ioFd);
		if (blockFd->ioBuffer != (char *)0) free(blockFd->ioBuffer);
		free(blockFd);
	}

	/* all done */
	return(OK);
}

/* read a record from a file */
int
blockRead(blockFd, inBuffer, bytesToRead)
struct BlockIOFile *blockFd;
char *inBuffer;
long bytesToRead;
{
	long iblock, blocksToRead, extraBytes;

	/* check parameters */
	if (inBuffer == (char *)0 || bytesToRead <= 0 ||
	    blockFd == (struct BlockIOFile *)0)
	{
		errno = EINVAL;
		ERROR("invalid input parameters.", errno);
		return(NOTOK);
	}

	/* get how many blocks to read */
	blocksToRead = bytesToRead/blockFd->blockSize;
	extraBytes = bytesToRead%blockFd->blockSize;

	/* read in blocks and copy to user buffer */
	for (iblock = 0; iblock < blocksToRead; iblock++)
	{
		/* do one read and copy data in user buffer */
		if (tread(blockFd->ioFd, 
			 blockFd->ioBuffer, blockFd->blockSize) == NOTOK)
		{
			ERROR("unable to read a buffer.", errno);
			return(NOTOK);
		}

		/* copy data in user buffer */
		memcpy(inBuffer+iblock*blockFd->blockSize, 
			blockFd->ioBuffer, blockFd->blockSize);
	}
	if (extraBytes > 0)
	{
		/* do one read and copy data in user buffer */
		if (tread(blockFd->ioFd, 
			 blockFd->ioBuffer, blockFd->blockSize) == NOTOK)
		{
			ERROR("unable to read a buffer.", errno);
			return(NOTOK);
		}

		/* copy data in user buffer */
		memcpy(inBuffer+blocksToRead*blockFd->blockSize, 
			blockFd->ioBuffer, extraBytes);
	}

	/* all done */
	return(OK);
}

/* write a record to a file */
int
blockWrite(blockFd, outBuffer, bytesToWrite)
struct BlockIOFile *blockFd;
char *outBuffer;
long bytesToWrite;
{
	long iblock, blocksToWrite, extraBytes;

	/* check parameters */
	if (outBuffer == (char *)0 || bytesToWrite <= 0 ||
	    blockFd == (struct BlockIOFile *)0)
	{
		errno = EINVAL;
		ERROR("invalid input parameters.", errno);
		return(NOTOK);

	}

	/* get how many blocks to write */
	blocksToWrite = bytesToWrite/blockFd->blockSize;
	extraBytes = bytesToWrite%blockFd->blockSize;

	/* write in blocks */
	for (iblock = 0; iblock < blocksToWrite; iblock++)
	{
		/* write data out */
		if (twrite(blockFd->ioFd, 
		          outBuffer+iblock*blockFd->blockSize, 
		          blockFd->blockSize) == NOTOK)
		{
			ERROR("unable to write a buffer.", errno);
			return(NOTOK);
		}
		fprintf(stderr, ".");
	}
	if (extraBytes > 0)
	{
		/* copy data in io buffer */
		memset(blockFd->ioBuffer, 0, blockFd->blockSize);
		memcpy(blockFd->ioBuffer, 
			outBuffer+blocksToWrite*blockFd->blockSize, extraBytes);

		/* write last buffer */
		if (twrite(blockFd->ioFd, blockFd->ioBuffer, 
			  blockFd->blockSize) == NOTOK)
		{
			ERROR("unable to write a buffer.", errno);
			return(NOTOK);
		}
		fprintf(stderr, ".");
	}

	/* all done */
	return(OK);
}

#ifdef M3B
/* set I/O mode for 3B20 */
int
blockSetIO(blockFd, mode)
struct BlockIOFile *blockFd;
int mode;
{
	/* set I/O mode */
	if (setio(blockFd->ioFd, mode) == NOTOK)
	{
		ERROR("unable to set I/O mode.", errno);
		return(NOTOK);
	}

	/* all done */
	return(OK);
}
#endif
