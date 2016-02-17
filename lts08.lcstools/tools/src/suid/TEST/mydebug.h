#ifndef __MYDEBUG_H
#define __MYDEBUG_H
/* debugging functions */

/* header files */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

/* declarations */
extern void dbgopen(char *);
extern void dbgprintf(char *, ...);
extern void dbgclose();

/* definitions */
#define DBGDUMP(_fmt, _val) { \
	long curtime; \
	char _nfmt[BUFSIZ]; \
	time(&curtime); \
	sprintf(_nfmt, "PID=%d PPID=%d %s[%s'%d] %s", \
		getpid(), getppid(), ctime(&curtime), \
		__FILE__, __LINE__, _fmt); \
	dbgprintf(_nfmt, _val); \
}

#endif
