#ifndef __POISSON_H
#define __POISSON_H
/* class for a poisson probability distributions */

/* headers */
#include "returns.h"
#include "probDist.h"

/* declare class */
class Poisson;

/* class for a poisson probability distribution */
class Poisson: public ProbDist {
public:
	/* constructors and destructor */
	Poisson();
	Poisson(double);
	Poisson(const Poisson &);
	~Poisson();

	/* assignment */
	Poisson &operator=(const Poisson &);

	/* sampling routines */
	double randomSample();

	/* other routines */
	int init(double = 1.0);
	int init(const Poisson &);
	inline int getStatus() { return(status); }

public:
	/* internal data */
	double mean;
};

#endif
