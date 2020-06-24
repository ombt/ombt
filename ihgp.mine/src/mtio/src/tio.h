#ifndef __TIO_H
#define __TIO_H
/* header for io functions with sanity timers */

/* headers */
#include <stdio.h>

/* local definitions */
#define DEFAULTMAXTIME 120

/* function declarations */
extern int topen();
extern int tclose();
extern int tread();
extern int twrite();
extern int tsettime();
extern int tgettime();

#endif
