/* base class for sampling a probability distribution */

/* headers */
#include <sysent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>

/* other headers */
#include "debug.h"
#include "probDist.h"

/* constructors and destructor */
ProbDist::ProbDist()
{
	/* do nothing */
	status = OK;
}

ProbDist::ProbDist(const ProbDist &)
{
	/* do nothing */
	status = OK;
}

ProbDist::~ProbDist()
{
	/* do nothing */
	status = OK;
}

/* assignment operator */
ProbDist &
ProbDist::operator=(const ProbDist &)
{
	return(*this);
}

/* set seed for pseudo-random number generator */
int
ProbDist::setSeed(unsigned long myseed)
{
	srand48(myseed);
	status = OK;
	return(OK);
}

/* get random sample */
double 
ProbDist::randomSample()
{
	return(1.0);
}
