#ifndef __DEBUG_H
#define __DEBUG_H

// headers
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <iostream.h>

// externs 
extern int sys_nerr;
extern char *sys_errlist[];

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

#define ShouldBeTrue(EXPR) \
	if (!(EXPR)) { \
		cerr << "WARNING AT "; \
		cerr << __FILE__ << "'" << __LINE__; \
		cerr << ": " << #EXPR << endl; \
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

#define ShouldBeTrue(EXPR) \
	if (!(EXPR)) { \
		cerr << "WARNING AT "; \
		cerr << __FILE__ << "'" << __LINE__; \
		cerr << ": " << "EXPR" << endl; \
	}
#endif

// simple debugging macros
#ifdef DEBUG

#define TMSG(_tmsg) \
	cerr << "TRACE " << __FILE__ << "'" << __LINE__ << ": " << _tmsg << endl
#define TRACE() \
	cerr << "TRACE " << __FILE__ << "'" << __LINE__ << ": " << endl
#define RETURN(myretval) { \
	cerr << "RETURN " << __FILE__ << "'" << __LINE__ << ": " << endl; \
	return(myretval); \
}
#if defined(__STDC__)
#define DUMP(myvalue) \
	cerr << "DUMP " << __FILE__ << "'" << __LINE__ << ": " #myvalue " = " << myvalue << endl
#else
#define DUMP(myvalue) \
	cerr << "DUMP " << __FILE__ << "'" << __LINE__ << ": myvalue = " << myvalue << endl
#endif
#define DUMPS(myvalue) \
	cerr << "DUMP " << __FILE__ << "'" << __LINE__ << ": " << myvalue << endl

#define IFTRACE() { \
	extern int _iftrace; \
	if (_iftrace) \
		cerr << "TRACE " << __FILE__ << "'" << __LINE__ << ": " << endl; \
}

#define IFTMSG(_tmsg) { \
	extern int _iftrace; \
	if (_iftrace) \
		cerr << "TRACE " << __FILE__ << "'" << __LINE__ << ": " << _tmsg << endl; \
}

#define DEFINETRACE() \
	int _iftrace

#define EXTERNTRACE() \
	extern int _iftrace

#define ENABLETRACE() { \
	extern int _iftrace; \
	_iftrace = 1; \
}

#define DISABLETRACE() { \
	extern int _iftrace; \
	_iftrace = 0; \
}

#else

#define TMSG(_tmsg)
#define TRACE()
#define RETURN(myretval)
#define DUMP(myvalue)
#define DUMPS(myvalue)
#define IFTMSG()
#define IFTRACE()
#define DEFINETRACE()
#define EXTERNTRACE()
#define ENABLETRACE()
#define DISABLETRACE()

#endif

#define ERROR(errmsg, errval) { \
	cerr << __FILE__ << "'" << __LINE__ << ": "; \
	if (((errval) > 0) && ((errval) < sys_nerr)) \
	{ \
		cerr << (errmsg) << " (errno = " << (errval); \
		cerr << ", " << sys_errlist[(errval)] << ")" << endl; \
	} \
	else \
	{ \
		cerr << (errmsg) << " (errno = " << (errval) << ")" << endl; \
	} \
}
#define ERRORD(errmsg, errdata, errval) { \
	cerr << __FILE__ << "'" << __LINE__ << ": "; \
	if (((errval) > 0) && ((errval) < sys_nerr)) \
	{ \
		cerr << (errmsg) << " (errno = " << (errval); \
		cerr << ", " << sys_errlist[(errval)] << ")" << endl; \
	} \
	else \
	{ \
		cerr << (errmsg) << " (errno = " << (errval) << ")" << endl; \
	} \
	cerr << "(data = " << (errdata) << ")" << endl; \
}

#endif
