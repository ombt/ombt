/* functions for tracing functions */

/* headers */
#include "ftrc.h"

/* constructor and destructor */
Ftrc::Ftrc(char *fname, int lnno)
{
	if (fname != (char *)0)
	{
		fprintf(stdout, "entering %s'%d ...\n", fname, lnno);
		function = new char [strlen(fname)+1];
		lineno = lnno;
		strcpy(function, fname);
	}
	else
	{
		function = (char *)0;
		lineno = 0;
	}
}

Ftrc::~Ftrc()
{
	if (function != (char *)0)
	{
		fprintf(stdout, "leaving %s'%d ...\n", 
			function, lineno);
		delete [] function;
		function = (char *)0;
	}
}
