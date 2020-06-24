// scan a file and determine the statistics of different
// codes, and longest runs for each code.
//

// headers
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// local headers
#include "returns.h"
#include "debug.h"

// definitions
#define NumberOfCodes 256

// globals
unsigned long frequency[NumberOfCodes][2];
unsigned long longestruns[NumberOfCodes][2];

// print usage message
void
usage(const char *cmd)
{
	printf("\nusage: %s infile \n", cmd);
	return;
}

// print frequency distribution
void
printdist(unsigned long table[][2], const char *msg)
{
	printf("%s ...\n", msg);
	printf("code  frequency\n");
	printf("---------------\n");
	for (unsigned int b=0; b < NumberOfCodes; b++)
	{
		printf("%4x  %d\n", table[b][0], table[b][1]);
	}
	return;
}

// comparison function 
extern "C" {
int
freqcmp(const void *p1, const void *p2)
{
	const unsigned long *pul1 = (unsigned long *)p1;
	const unsigned long *pul2 = (unsigned long *)p2;
	if (pul1[1] > pul2[1])
		return(-1);
	else if (pul1[1] < pul2[1])
		return(1);
	else
		return(0);
	
}
}

// open file and determine the binary code distribution.
int
getfreqdist(const char *fname)
{
	// open file for read
	int fd = open(fname, O_RDONLY);
	if (fd == NOTOK)
	{
		ERRORD("unable to open file.", fname, errno);
		return(NOTOK);
	}

	// initialize table
	for (int b = 0; b < NumberOfCodes; b++)
	{
		frequency[b][0] = b;
		frequency[b][1] = 0;
		longestruns[b][0] = b;
		longestruns[b][1] = 0;
	}

	// read and populate frequency table
	int b, first, nr, lastchar, nchar;
	first = 1;
	long currentrun = 0;
	lastchar = -1;
	unsigned char buf[BUFSIZ];
	while ((nr = read(fd, (void *)buf, BUFSIZ)) > 0)
	{
		for (b = 0; b < nr; b++)
		{
			MustBeTrue(frequency[buf[b]][0] == buf[b]);
			frequency[buf[b]][1]++;

			MustBeTrue(longestruns[buf[b]][0] == buf[b]);
			if (lastchar == buf[b])
			{
				currentrun++;
			}
			else
			{
				if (longestruns[buf[b]][1] < currentrun)
					longestruns[buf[b]][1] = currentrun;
				lastchar = buf[b];
				currentrun = 1;
			}
		}
	}
	if (longestruns[buf[b]][1] < currentrun)
		longestruns[buf[b]][1] = currentrun;

	// sort results
	qsort((void *)frequency, (size_t)NumberOfCodes, 
		(size_t)(2*sizeof(unsigned long)), freqcmp);
	qsort((void *)longestruns, (size_t)NumberOfCodes, 
		(size_t)(2*sizeof(unsigned long)), freqcmp);

	// close file
	close(fd);

	// all done
	return(OK);
}

// huffman encoding algorithm
main(int argc, char **argv)
{
	// need a file name and an output filename.
	if (argc != 2)
	{
		usage(argv[0]);
		return(2);
	}

	// open file and generate statistics
	MustBeTrue(getfreqdist(argv[1]) == OK);

	// print frequency distribution
	printdist(frequency, "frequency data");
	printdist(longestruns, "longest runs data");

	// all done
	return(0);
}
