#ifndef __MYBC_H
#define __MYBC_H

/* headers */
#include <stdio.h>
#include <stdlib.h>

/* enumeration for parser */
enum VALUETYPE { ULONG, LONG, DOUBLE, UNKNOWN };
typedef enum VALUETYPE TYPE;

/* mybc definitions */
#define NUMBEROFPOINTS 200
#define XPIXNUM 50
#define YPIXNUM 50

/* structure for storing symbol information */
struct VALUEDATA {
	TYPE type;
	union {
		long lvalue;
		unsigned long ulvalue;
		double dvalue;
	} value;
};
typedef struct VALUEDATA VALUE;

/* functions */
extern void mybcerror();
extern char *num2base();
extern VALUE number2double();

#endif
