#ifndef __EXPONENTIAL_H
#define __EXPONENTIAL_H
/* class for an exponential probability distributions */

/* headers */
#include "returns.h"
#include "probDist.h"

/* declare class */
class Exponential;

/* class for an exponential probability distribution */
class Exponential: public ProbDist {
public:
	/* constructors and destructor */
	Exponential();
	Exponential(double);
	Exponential(const Exponential &);
	~Exponential();

	/* assignment */
	Exponential &operator=(const Exponential &);

	/* sampling routines */
	double randomSample();

	/* other routines */
	int init(double = 1.0);
	int init(const Exponential &);
	inline int getStatus() { return(status); }

public:
	/* internal data */
	double mean;
};

#endif
