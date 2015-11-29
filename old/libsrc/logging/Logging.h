#ifndef __OMBT_LOGGING_H
#define __OMBT_LOGGING_H

/* debugging functions */

/* header files */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

namespace ombt {

/* declarations */
extern void dbgopen(FILE *);
extern void dbgopen(const char *);
extern void dbgprintf(const char *, ...);
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

#define DBGTRACE() dbgprintf("Trace at %s'%s'%d\n", __FILE__, __FUNCTION__, __LINE__)
#define DBGPRINTF(FMT, XXX) dbgprintf("DUMP at %s'%s'%d " FMT " \n", __FILE__, __FUNCTION__, __LINE__, XXX )

}

#endif
