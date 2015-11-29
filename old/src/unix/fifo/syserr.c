/* include unix headers */
#include <stdio.h>
#include <errno.h>

/* error handling */
void syserr(msg)
char *msg;
{
    extern int errno, sys_nerr;
    extern char *sys_errlist[];

    fprintf(stderr, "ERROR: %s (%d", msg, errno);
    if (errno > 0 && errno < sys_nerr)
    {
	fprintf(stderr, "; %s)\n", sys_errlist[errno]);
    }
    else
    {
	fprintf(stderr, ")\n");
    }
    exit(1);
}
