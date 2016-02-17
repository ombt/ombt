/* hash a block of data */

/* unix headers */
#include <stdio.h>
#include <errno.h>

/* local headers */
#include "hash.h"

/* hashing function */
int
hash(sum, buffer, blockSize)
unsigned short *sum;
char *buffer;
long blockSize;
{
	register unsigned long psum;
	register unsigned long c;

	/* sanity check */
	errno = EINVAL;
	if (buffer == (char *)0)
	{
		ERROR("buffer is null.", errno);
		return(NOTOK);
	}
	if (blockSize <= 0)
	{
		ERROR("block size is 0.", errno);
		return(NOTOK);
	}

	/* calculate hash sum and combine with previous one */
	for (psum = *sum; blockSize > 0; buffer++, blockSize--)
	{
		c = *buffer;
		if (psum & 01)
			psum = (psum >> 1) + 0x8000;
		else
			psum >>= 1;
		psum += c;
		psum &= 0xffff;
	}
	*sum = (unsigned short)(0xffff&psum);

	/* all done */
	return(OK);
}
