/* headers */
#include <stdio.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

/* search command line */
char *
FindParameter(argc, argv, look_for)
int argc;
char **argv;
char *look_for;
{
	int count, length;

	/* search argv array for parameter */
	length = strlen(look_for);
	count = 1;
	while (count < argc)
	{
		/* search for a match */
		if (strncmp(look_for, argv[count], length) == 0)
		{
			/* a match was found */
			count++;
			if (count < argc)
				return(argv[count]);
			else
				return(argv[count-1]);
				
		}
		count++;
	}
	
	/* nothing was found */
	return((char *)NULL);
}
