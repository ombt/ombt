#ifndef __DEBUG_H
#define __DEBUG_H

/* debug statements */
#ifdef DEBUG
#define TRACE() fprintf(stderr, "%s'%d\n", __FILE__, __LINE__)
#define DUMPS(str) fprintf(stderr, "%s'%d: <%s>\n", __FILE__, __LINE__, (str))
#define DUMPL(str) fprintf(stderr, "%s'%d: <%lu>\n", __FILE__, __LINE__, (str))
#define DUMPX(str) fprintf(stderr, "%s'%d: <%lx>\n", __FILE__, __LINE__, (str))
#define DUMPI(str) fprintf(stderr, "%s'%d: <%d>\n", __FILE__, __LINE__, (str))
#define DUMPF(str) fprintf(stderr, "%s'%d: <%lf>\n", __FILE__, __LINE__, (double)(str))
#define RETURN(retval) { TRACE(); return(retval); }
#else
#define TRACE() 
#define DUMPS(str) 
#define DUMPL(str) 
#define DUMPX(str) 
#define DUMPI(str) 
#define DUMPF(str) 
#define RETURN(retval) return(retval)
#endif
#define ERROR(errmsg, errval) \
	fprintf(stderr, "%s'%d: %s (errno=%d)\n", __FILE__, __LINE__, errmsg, errval)
#define ERRORS(errmsg, errdata, errval) \
	fprintf(stderr, "%s'%d: %s (data = %s, errno=%d)\n", __FILE__, __LINE__, errmsg, errdata, errval)
#define ERRORI(errmsg, errdata, errval) \
	fprintf(stderr, "%s'%d: %s (data = %d, errno=%d)\n", __FILE__, __LINE__, errmsg, (int)errdata, errval)

#endif
