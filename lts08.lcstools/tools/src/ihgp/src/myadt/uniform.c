/* class for a uniform probability distribution */

/* headers */
#include <sysent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <math.h>
#include <errno.h>

/* other headers */
#include "debug.h"
#include "uniform.h"

/* constructors */
Uniform::Uniform(): ProbDist()
{
	init();
}

Uniform::Uniform(double min, double max): ProbDist()
{
	init(min, max);
}

Uniform::Uniform(const Uniform &src): ProbDist(src)
{
	init(src);
}

/* destructor */
Uniform::~Uniform()
{
	/* do nothing */
	status = OK;
}

/* initialization routines */
int
Uniform::init(double min, double max)
{
	minimum = min;
	maximum = max;
	status = OK;
	return(OK);
}

int
Uniform::init(const Uniform &src)
{
	minimum = src.minimum;
	maximum = src.maximum;
	status = OK;
	return(OK);
}

/* assignment operator */
Uniform &
Uniform::operator=(const Uniform &src)
{
	if (this == &src) return(*this);
	ProbDist::operator=(src);
	init(src);
	return(*this);
}

/* get random sample */
double 
Uniform::randomSample()
{
	return(minimum+(maximum-minimum)*drand48());
}
