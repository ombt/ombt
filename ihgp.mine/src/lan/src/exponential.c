/* class for an exponential probability distribution */

/* headers */
#include <sysent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <math.h>
#include <errno.h>

/* other headers */
#include "debug.h"
#include "exponential.h"

/* constructors */
Exponential::Exponential(): ProbDist()
{
	init();
}

Exponential::Exponential(double mymean): ProbDist()
{
	init(mymean);
}

Exponential::Exponential(const Exponential &src): ProbDist(src)
{
	init(src);
}

/* destructor */
Exponential::~Exponential()
{
	/* do nothing */
	status = OK;
}

/* initialization routines */
int
Exponential::init(double mymean)
{
	mean = mymean;
	status = OK;
	return(OK);
}

int
Exponential::init(const Exponential &src)
{
	mean = src.mean;
	status = OK;
	return(OK);
}

/* assignment operator */
Exponential &
Exponential::operator=(const Exponential &src)
{
	if (this == &src) return(*this);
	ProbDist::operator=(src);
	init(src);
	return(*this);
}
/* get random sample */
double 
Exponential::randomSample()
{
	return(-1.0*mean*log(drand48()));
}
