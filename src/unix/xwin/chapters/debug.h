#ifndef __DEBUG_H
#define __DEBUG_H

// headers
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <iostream.h>

// check macros
#if defined(__STDC__)
#if defined(USEEXCEPTION)
#define MustBeTrue(EXPR) \
	if (!(EXPR)) { \
		cerr << "INTERNAL ERROR AT "; \
		cerr << __FILE__ << "'" << __LINE__; \
		cerr << ": " << #EXPR << endl; \
		throw #EXPR ; \
	}

#else
#define MustBeTrue(EXPR) \
	if (!(EXPR)) { \
		cerr << "INTERNAL ERROR AT "; \
		cerr << __FILE__ << "'" << __LINE__; \
		cerr << ": " << #EXPR << endl; \
		abort(); \
	}
#endif

#define CheckReturn(EXPR) \
	if (!(EXPR)) { \
		cerr << "ERROR RETURN AT "; \
		cerr << __FILE__ << "'" << __LINE__; \
		cerr << ": " << #EXPR << endl; \
		return(NOTOK); \
	}

#else

#if defined(USEEXCEPTION)
#define MustBeTrue(EXPR) \
	if (!(EXPR)) { \
		cerr << "INTERNAL ERROR AT "; \
		cerr << __FILE__ << "'" << __LINE__; \
		cerr << ": " << "EXPR" << endl; \
		throw "EXPR" ; \
	}
#else
#define MustBeTrue(EXPR) \
	if (!(EXPR)) { \
		cerr << "INTERNAL ERROR AT "; \
		cerr << __FILE__ << "'" << __LINE__; \
		cerr << ": " << "EXPR" << endl; \
		abort(); \
	}
#endif

#define CheckReturn(EXPR) \
	if (!(EXPR)) { \
		cerr << "ERROR RETURN AT "; \
		cerr << __FILE__ << "'" << __LINE__; \
		cerr << ": " << "EXPR" << endl; \
		return(NOTOK); \
	}
#endif

// simple debugging macros
#ifdef DEBUG

#define TRACE() \
	cerr << "TRACE " << __FILE__ << "'" << __LINE__ << ": " << endl
#define RETURN(myretval) { \
	cerr << "RETURN " << __FILE__ << "'" << __LINE__ << ": " << endl; \
	return(myretval); \
}
#define DUMP(myvalue) \
	cerr << "DUMP " << __FILE__ << "'" << __LINE__ << ": value = " << myvalue << endl

#else

#define TRACE()
#define RETURN(myretval)
#define DUMP(myvalue)

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
