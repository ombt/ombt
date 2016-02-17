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

#define TRACE() \
	cout << "TRACE " << __FILE__ << "'" << __LINE__ << ": " << endl
#define RETURN(myretval) { \
	cout << "RETURN " << __FILE__ << "'" << __LINE__ << ": " << endl; \
	return(myretval); \
}
#define DUMP(myvalue) \
	cout << "DUMP " << __FILE__ << "'" << __LINE__ << ": value = " << myvalue << endl

#else

#define TRACE()
#define RETURN(myretval)
#define DUMP(myvalue)

#endif

#define ERROR(errmsg, errval) { \
	cout << __FILE__ << "'" << __LINE__ << ": "; \
	cout << errmsg << " (errno = " << errval << ")" << endl; \
}
#define ERRORD(errmsg, errdata, errval) { \
	cout << __FILE__ << "'" << __LINE__ << ": "; \
	cout << errmsg << " (errno = " << errval << ")" << endl; \
	cout << "(data = " << errdata << ")" << endl; \
}

#endif
