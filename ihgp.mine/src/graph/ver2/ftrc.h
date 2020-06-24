#ifndef __FTRC_H
#define __FTRC_H
/* class for tracing entry/exit into functions */

/* required headers */
#include <sysent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* define class */
class Ftrc;

/* function trace class definition */
class Ftrc {
public:
	/* constructor and destructor */
	Ftrc(char * = (char *)0, int = 0);
	~Ftrc();

protected:
	/* internal data */
	char *function;
	int lineno;
};

#endif
