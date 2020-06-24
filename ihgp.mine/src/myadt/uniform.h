#ifndef __UNIFORM_H
#define __UNIFORM_H
/* class for a uniform probability distributions */

/* headers */
#include "returns.h"
#include "probDist.h"

/* declare class */
class Uniform;

/* class for a uniform probability distribution */
class Uniform: public ProbDist {
public:
	/* constructors and destructor */
	Uniform();
	Uniform(double, double);
	Uniform(const Uniform &);
	~Uniform();

	/* assignment */
	Uniform &operator=(const Uniform &);

	/* sampling routines */
	double randomSample();

	/* other routines */
	int init(double = 0.0, double = 1.0);
	int init(const Uniform &);
	inline int getStatus() { return(status); }

public:
	/* internal data */
	double minimum;
	double maximum;
};

#endif
