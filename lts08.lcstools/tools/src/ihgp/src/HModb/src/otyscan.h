#ifndef __OTYSCAN_H
#define __OTYSCAN_H

// required headers
#include <sysent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "returns.h"
#include "debug.h"

// definitions
#define TAB_NL " \t"
#define TAB_NL_TILDE " \t~"
#define TAB_NL_TILDE_EQUAL " \t~="
#define ALL_DELIMITERS "!@#$%^&*()_+|{}:\"~?><-=;'`,./"
#define VERBOSE if (verbose) fprintf

// externs
extern int verbose;

// declarations
extern int MYgetc(FILE *);
extern void dumpTokens(char *);
extern int msgstartSemantics();
extern int msgendSemantics();
extern int msgidSemantics(char *);
extern int timeSemantics(char *);
extern int classSemantics(char *info);
extern int actionSemantics(char *);
extern int outpriorSemantics(char *);
extern int freqSemantics(char *);
extern int prototypeSemantics(char *);
extern int fieldSemantics(char *);
extern int fieldupSemantics(char *);
extern int npvalSemantics(char *);
extern int widthSemantics(char *);
extern int enumSemantics(char *);

#endif
