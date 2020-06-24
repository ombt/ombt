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
unsigned long total1 = 0;
unsigned long total2 = 0;
unsigned long frequency[NumberOfCodes][2];
unsigned long longestruns[NumberOfCodes][2];
unsigned long numberofrunsgt2[NumberOfCodes][2];

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

// read characters
int 
getc(int fd, unsigned char &c)
{
	static int b = BUFSIZ;
	static int nr = BUFSIZ;
	static unsigned char buf[BUFSIZ];

	if (++b >= nr)
	{
		nr = read(fd, (void *)buf, BUFSIZ);
		if (nr <= 0) return(nr);
		b = 0;
	}
	c = buf[b];
	return(1);
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
		numberofrunsgt2[b][0] = b;
		numberofrunsgt2[b][1] = 0;
	}

	// read and populate frequency table
	unsigned char c;
	if (getc(fd, c) <= 0)
		return(NOTOK);
	total1++;
	frequency[c][1]++;
	long currentrun = 1;
	unsigned lastchar = c;
	while (getc(fd, c) > 0)
	{
		total1++;
		frequency[c][1]++;
		if (c == lastchar)
		{
			currentrun++;
		}
		else
		{
			total2 += currentrun;
			if (longestruns[lastchar][1] < currentrun)
				longestruns[lastchar][1] = currentrun;
			if (currentrun > 1)
				numberofrunsgt2[lastchar][1]++;
			lastchar = c;
			currentrun = 1;
		}
	}
	if (longestruns[lastchar][1] < currentrun)
		longestruns[lastchar][1] = currentrun;
	total2 += currentrun;

	// sort results
	qsort((void *)frequency, (size_t)NumberOfCodes, 
		(size_t)(2*sizeof(unsigned long)), freqcmp);
	qsort((void *)longestruns, (size_t)NumberOfCodes, 
		(size_t)(2*sizeof(unsigned long)), freqcmp);
	qsort((void *)numberofrunsgt2, (size_t)NumberOfCodes, 
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
	printf("totals from frequency = %lu\n", total1);
	printf("totals from longest run = %lu\n", total2);
	printdist(frequency, "frequency data");
	printdist(longestruns, "longest runs data");
	printdist(numberofrunsgt2, "number of runs greater than 2 data");

	// all done
	return(0);
}
