/* file contains buffer i/o functions */

/* unix headers */
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <errno.h>

/* local headers */
#include "returns.h"
#include "debug.h"
#include "bufferio.h"
#include "tio.h"

/* function to open a file for write */
struct BufferIOFile *
bufferOpenFile(fileName, bufferSz, operation)
char *fileName;
long bufferSz;
int operation;
{
	int openOperation;
	struct BufferIOFile *bufferFd;

	/* check parameters */
	if (fileName == (char *)0 || *fileName == 0)
	{
		errno = EINVAL;
		ERROR("invalid filename.", errno);
		return((struct BufferIOFile *)0);
	}
	if (operation != WRITEONLY && operation != CREATEFILE)
	{
		errno = EINVAL;
		ERROR("invalid operation.", errno);
		return((struct BufferIOFile *)0);
	}
	if (bufferSz <= 0)
	{
		errno = EINVAL;
		ERROR("buffer size <= 0.", errno);
		return((struct BufferIOFile *)0);
	}

	/* allocate a descriptor buffer */
	bufferFd = (struct BufferIOFile *)malloc(sizeof(struct BufferIOFile));
	if (bufferFd == (struct BufferIOFile *)0)
	{
		ERROR("unable to allocate a buffer IO file descriptor.", errno);
		return((struct BufferIOFile *)0);
	}

	/* save buffer size and initialize bytes-in-buffer */
	bufferFd->bufferSize = bufferSz;
	bufferFd->bytesInBuffer = 0;

	/* allocate write buffer */
	if ((bufferFd->ioBuffer = malloc(bufferFd->bufferSize)) == (char *)0)
	{
		free(bufferFd);
		ERROR("unable to malloc io buffer.", errno);
		return((struct BufferIOFile *)0);
	}

	/* open file */
	if ((openOperation = operation) == CREATEFILE)
		openOperation = WRITEONLY;
	if ((bufferFd->ioFd = topen(fileName, openOperation)) == NOTOK)
	{
		if (errno == ENOENT && operation == CREATEFILE)
		{
			bufferFd->ioFd = creat(fileName, CREATEMODE);
			if (bufferFd->ioFd == NOTOK)
			{
				free(bufferFd->ioBuffer);
				free(bufferFd);
				ERRORS("create failed for file.", 
					fileName, errno);
				return((struct BufferIOFile *)0);
			}
		}
		else
		{
			free(bufferFd->ioBuffer);
			free(bufferFd);
			ERRORS("open failed for file.", fileName, errno);
			return((struct BufferIOFile *)0);
		}
	}

	/* all done */
	return(bufferFd);
}

/* close a file */
int
bufferCloseFile(bufferFd)
struct BufferIOFile *bufferFd;
{
	/* check parameters */
	if (bufferFd != (struct BufferIOFile *)0)
	{
		/* check if open */
		if ((bufferFd->ioFd != NOTOK) && 
		    (bufferFd->ioBuffer != (char *)0))
		{
			/* flush buffer, if neccesary */
			if (bufferFd->bytesInBuffer > 0)
			{
				if (twrite(bufferFd->ioFd, bufferFd->ioBuffer, 
					bufferFd->bytesInBuffer) == NOTOK)
				{
					ERROR("unable to write a buffer.", 
						errno);
					return(NOTOK);
				}
				fprintf(stderr, ".");
			}
		}

		/* close file and release all data */
		if (bufferFd->ioFd != NOTOK) tclose(bufferFd->ioFd);
		if (bufferFd->ioBuffer != (char *)0) free(bufferFd->ioBuffer);
		free(bufferFd);
	}

	/* all done */
	return(OK);
}

/* write a record to a file */
int
bufferWrite(bufferFd, outBuffer, bytesToWrite)
struct BufferIOFile *bufferFd;
char *outBuffer;
long bytesToWrite;
{
	/* check parameters */
	if (outBuffer == (char *)0 || bytesToWrite <= 0 ||
	    bufferFd == (struct BufferIOFile *)0)
	{
		errno = EINVAL;
		ERROR("invalid input parameters.", errno);
		return(NOTOK);

	}

	/* check if out buffer fits into io buffer */
	if ((bufferFd->bytesInBuffer + bytesToWrite) < bufferFd->bufferSize)
	{
		/* store data in temporary buffer */
		memcpy(bufferFd->ioBuffer + bufferFd->bytesInBuffer,
		       outBuffer, bytesToWrite);
		bufferFd->bytesInBuffer += bytesToWrite;
		return(OK);
	}

	/* flush anything in buffer */
	if (bufferFd->bytesInBuffer > 0)
	{
		if (twrite(bufferFd->ioFd, bufferFd->ioBuffer, 
			  bufferFd->bytesInBuffer) == NOTOK)
		{
			ERROR("unable to write a buffer.", errno);
			return(NOTOK);
		}
		bufferFd->bytesInBuffer = 0;
		fprintf(stderr, ".");
	}

	/* check if user buffer can be cached */
	if (bytesToWrite < bufferFd->bufferSize)
	{
		/* store data in temporary buffer */
		memcpy(bufferFd->ioBuffer, outBuffer, bytesToWrite);
		bufferFd->bytesInBuffer = bytesToWrite;
		return(OK);
	}

	/* write buffer out to file */
	if (twrite(bufferFd->ioFd, outBuffer, bytesToWrite) == NOTOK)
	{
		ERROR("unable to write a buffer.", errno);
		return(NOTOK);
	}
	fprintf(stderr, ".");

	/* all done */
	return(OK);
}

