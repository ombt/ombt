#ifndef __DEBUG_H
#define __DEBUG_H

// headers
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <iostream.h>

// externs for debugging
extern int _iftrace;

// check macros
#if defined(__STDC__)
#if defined(USEEXCEPTION)
#define MustBeTrue(EXPR) \
	if (!(EXPR)) { \
		cout << "INTERNAL ERROR AT "; \
		cout << __FILE__ << "'" << __LINE__; \
		cout << ": " << #EXPR << endl; \
		throw #EXPR ; \
	}

#else
#define MustBeTrue(EXPR) \
	if (!(EXPR)) { \
		cout << "INTERNAL ERROR AT "; \
		cout << __FILE__ << "'" << __LINE__; \
		cout << ": " << #EXPR << endl; \
		abort(); \
	}
#endif

#define CheckReturn(EXPR) \
	if (!(EXPR)) { \
		cout << "ERROR RETURN AT "; \
		cout << __FILE__ << "'" << __LINE__; \
		cout << ": " << #EXPR << endl; \
		return(NOTOK); \
	}

#else

#if defined(USEEXCEPTION)
#define MustBeTrue(EXPR) \
	if (!(EXPR)) { \
		cout << "INTERNAL ERROR AT "; \
		cout << __FILE__ << "'" << __LINE__; \
		cout << ": " << "EXPR" << endl; \
		throw "EXPR" ; \
	}
#else
#define MustBeTrue(EXPR) \
	if (!(EXPR)) { \
		cout << "INTERNAL ERROR AT "; \
		cout << __FILE__ << "'" << __LINE__; \
		cout << ": " << "EXPR" << endl; \
		abort(); \
	}
#endif

#define CheckReturn(EXPR) \
	if (!(EXPR)) { \
		cout << "ERROR RETURN AT "; \
		cout << __FILE__ << "'" << __LINE__; \
		cout << ": " << "EXPR" << endl; \
		return(NOTOK); \
	}
#endif

// simple debugging macros
#ifdef DEBUG

#define TMSG(_tmsg) \
	cout << "TRACE " << __FILE__ << "'" << __LINE__ << ": " << _tmsg << endl
#define TRACE() \
	cout << "TRACE " << __FILE__ << "'" << __LINE__ << ": " << endl
#define RETURN(myretval) { \
	cout << "RETURN " << __FILE__ << "'" << __LINE__ << ": " << endl; \
	return(myretval); \
}
#if defined(__STDC__)
#define DUMP(myvalue) \
	cout << "DUMP " << __FILE__ << "'" << __LINE__ << ": " #myvalue " = " << myvalue << endl
#else
#define DUMP(myvalue) \
	cout << "DUMP " << __FILE__ << "'" << __LINE__ << ": myvalue = " << myvalue << endl
#endif
#define DUMPS(myvalue) \
	cout << "DUMP " << __FILE__ << "'" << __LINE__ << ": " << myvalue << endl

#define IFTRACE() { \
	extern int _iftrace; \
	if (_iftrace) \
		cout << "TRACE " << __FILE__ << "'" << __LINE__ << ": " << endl; \
}

#define IFTMSG(_tmsg) { \
	extern int _iftrace; \
	if (_iftrace) \
		cout << "TRACE " << __FILE__ << "'" << __LINE__ << ": " << _tmsg << endl; \
}

#define DEFINETRACE() int _iftrace
#define ENABLETRACE() _iftrace = 1
#define DISABLETRACE() _iftrace = 0
#define GETTRACE() _iftrace

#else

#define TMSG(_tmsg)
#define TRACE()
#define RETURN(myretval)
#define DUMP(myvalue)
#define DUMPS(myvalue)
#define IFTMSG()
#define IFTRACE()
#define DEFINETRACE()
#define ENABLETRACE()
#define DISABLETRACE()

#endif

#define ERROR(errmsg, errval) { \
	cout << __FILE__ << "'" << __LINE__ << ": "; \
	cout << (errmsg) << " (errno = " << (errval) << ")" << endl; \
}
#define ERRORD(errmsg, errdata, errval) { \
	cout << __FILE__ << "'" << __LINE__ << ": "; \
	cout << (errmsg) << " (errno = " << (errval) << ")" << endl; \
	cout << "(data = " << (errdata) << ")" << endl; \
}

#endif
