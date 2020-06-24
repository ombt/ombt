/* class for a poisson probability distribution */

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
#include "poisson.h"

/* constructors */
Poisson::Poisson(): ProbDist()
{
	init();
}

Poisson::Poisson(double mymean): ProbDist()
{
	init(mymean);
}

Poisson::Poisson(const Poisson &src): ProbDist(src)
{
	init(src);
}

/* destructor */
Poisson::~Poisson()
{
	/* do nothing */
	status = OK;
}

/* initialization routines */
int
Poisson::init(double mymean)
{
	mean = mymean;
	status = OK;
	return(OK);
}

int
Poisson::init(const Poisson &src)
{
	mean = src.mean;
	status = OK;
	return(OK);
}

/* assignment operator */
Poisson &
Poisson::operator=(const Poisson &src)
{
	if (this == &src) return(*this);
	ProbDist::operator=(src);
	mean = src.mean;
	return(*this);
}
/* get random sample */
double 
Poisson::randomSample()
{
	/* sample a discrete distribution */
	double x = 0.0;
	double P = exp(-1.0*mean);
	double F = P;
	double u = drand48();
	while (F < u)
	{
		x += 1.0;
		P *= mean/x;
		F += P;
	}
	return(x);
}
