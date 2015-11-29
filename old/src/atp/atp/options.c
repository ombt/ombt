// get options 

// headers
#include "options.h"

// functions to get option data
int
getOption(const String &optnm, const String &doptval, String &optval)
{
	if (options.isInMap(optnm))
		optval = options[optnm];
	else
		optval = doptval;
	return(OK);
}

int
getOption(const String &optnm, int doptval, int &optval)
{
	if (options.isInMap(optnm))
		optval = atoi(options[optnm]);
	else
		optval = doptval;
	return(OK);
}
