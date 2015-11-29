// code for unique name generation

// headers
#include "uniquename.h"

// counter used for generating names
static long counter = 0;

// function to generate unique names
String
uniqueName(const String &prefix)
{
	char buf[BUFSIZ];
	sprintf(buf, "%d", ++counter);
	return(prefix + String(buf));
}

