#ifndef __DEBUG_H
#define __DEBUG_H

// headers
#include <stdio.h>

// simple debugging macros
#ifdef DEBUG
#define TRACE() \
	fprintf(stderr, "%s'%d: \n", \
		__FILE__, __LINE__)
#define RETURN(retval) { \
	fprintf(stderr, "%s'%d:\n", __FILE__, __LINE__); \
	return(retval); \
}
#define DUMPI(value) \
	fprintf(stderr, "%s'%d: value = %d\n", \
		__FILE__, __LINE__, value)
#define DUMPS(value) \
	fprintf(stderr, "%s'%d: value = %s\n", \
		__FILE__, __LINE__, value)
#else
#define TRACE()
#define RETURN(retval) return(retval)
#define DUMPI(value)
#define DUMPS(value)
#endif

#define ERROR(errmsg) \
	fprintf(stderr, "%s'%d: %s\n", \
		__FILE__, __LINE__, errmsg)
#define ERRORI(errmsg, errval) \
	fprintf(stderr, "%s'%d: %s, value = %d\n", \
		__FILE__, __LINE__, errmsg, errval)
#define ERRORS(errmsg, errval) \
	fprintf(stderr, "%s'%d: %s, value = %s\n", \
		__FILE__, __LINE__, errmsg, errval)
#define ERRORF(errmsg, errval) \
	fprintf(stderr, "%s'%d: %s, value = %f\n", \
		__FILE__, __LINE__, errmsg, errval)

#endif
