#ifndef __DEBUG_H
#define __DEBUG_H

// headers
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <iostream.h>

// simple debugging macros
#ifdef DEBUG

#define TRACE() \
	cerr << "Trace: " << __FILE__ << "'" << __LINE__ << ": " << endl

#define RETURN(retval) { \
	cerr << "Return: " << __FILE__ << "'" << __LINE__ << ": " << endl; \
	return(retval); \
}

#define DUMP(val) \
	cerr << "Dump " << __FILE__ << "'" << __LINE__ << ": value = " << val << endl

#else

#define TRACE()
#define RETURN(retval) return(retval)
#define DUMP(value)

#endif

#define ERROR(errmsg, errval) { \
	cerr << __FILE__ << "'" << __LINE__ << ": "; \
	cerr << errmsg << " (errno = " << errval << ")" << endl; \
}

#define ERRORD(errmsg, errdata, errval) { \
	cerr << __FILE__ << "'" << __LINE__ << ": "; \
	cerr << errmsg << " (errno = " << errval << ")" << endl; \
	cerr << "(data = " << errdata << ")" << endl; \
}

#endif
