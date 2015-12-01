// code for generating a unique index number

// headers
#include "index.h"

// counter used for generating names
static long counter = 0;

// function to generate index numbers
unsigned long
nextIndex()
{
	return(++counter);
}

unsigned long
getIndex()
{
	return(counter);
}

