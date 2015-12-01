#ifndef __DEBUG_H
#define __DEBUG_H

/* debug statements */
#ifdef DEBUG
#define TRACE() fprintf(stdout, "%s'%d\n", __FILE__, __LINE__)
#define DUMPS(str) fprintf(stdout, "%s'%d: <%s>\n", __FILE__, __LINE__, str)
#define DUMPL(str) fprintf(stdout, "%s'%d: <%lu>\n", __FILE__, __LINE__, str)
#define DUMPI(str) fprintf(stdout, "%s'%d: <%d>\n", __FILE__, __LINE__, str)
#define RETURN(retval) { TRACE(); return(retval); }
#else
#define TRACE() 
#define DUMPS(str) 
#define DUMPL(str) 
#define DUMPI(str) 
#define RETURN(retval) return(retval)
#endif
#define ERROR(errmsg, errval) \
	fprintf(stdout, "%s'%d: %s (errno=%d)\n", __FILE__, __LINE__, errmsg, errval)
#define ERRORS(errmsg, errdata, errval) \
	fprintf(stdout, "%s'%d: %s (data = %s, errno=%d)\n", __FILE__, __LINE__, errmsg, errdata, errval)
#define ERRORI(errmsg, errdata, errval) \
	fprintf(stdout, "%s'%d: %s (data = %d, errno=%d)\n", __FILE__, __LINE__, errmsg, (int)errdata, errval)

#endif
