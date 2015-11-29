/* headers */
#include <stdio.h>
#include <string.h>
#include "returns.h"
#include "debug.h"
#include "myxlib.h"

/* local definitions */
#ifndef True
#define True 1
#endif
#ifndef False
#define False 0
#endif

/* get and set a font from argv */
int
CheckForHelp(argc, argv, extra_message)
int argc;
char **argv;
char *extra_message;
{
	char *ptr;

	/* check for help */
	ptr = FindParameter(argc, argv, "-help");
	if (ptr != (char *)NULL)
	{
		PrintUsageMessage(extra_message);
		return(True);
	}

	/* no help message */
	return(False);
}

void
PrintUsageMessage(extra_message)
char *extra_message;
{
	fprintf(stderr, "available options:\n");
	fprintf(stderr, "\t-display displayname\n");
	fprintf(stderr, "\t-geometry geometry specs\n");
	fprintf(stderr, "\t-font font name\n");
	fprintf(stderr, "\t-fn font name\n");
	fprintf(stderr, "\t-title window title\n");
	fprintf(stderr, "\t-name window title\n");
	fprintf(stderr, "\t-iconic\n");
	if (extra_message && *extra_message)
		fprintf(stderr, "%s\n", extra_message);
	return;
}
