#ifndef __MYBC_H
#define __MYBC_H

/* headers */
#include <stdio.h>
#include <stdlib.h>

/* local headers */
#include "mycomplex.h"
#include "epsilon.h"

/* enumeration for parser */
enum VALUETYPE { COMPLEX, ULONG, LONG, DOUBLE, VOID, UNKNOWN };
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
		struct {
			double r;
			double i;
		} cvalue;
	} value;
};
typedef struct VALUEDATA VALUE;

/* functions */
extern void mybcerror();
extern char *num2base();
extern VALUE number2double();
extern VALUE number2complex();
extern unsigned long perm(unsigned long, unsigned long);
extern unsigned long comb(unsigned long, unsigned long);
extern unsigned long fact(unsigned long);
extern double perm(double, double);
extern double comb(double, double);
extern void usury(double, double, double);
extern double fact(double);

#endif
