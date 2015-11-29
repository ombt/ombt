#ifndef __PROBDIST_H
#define __PROBDIST_H
/* base class for probability distributions */

/* headers */
#include "returns.h"

/* declare class */
class ProbDist;

/* base class for sampling a probability distribution */
class ProbDist {
public:
	/* constructors and destructor */
	ProbDist();
	ProbDist(const ProbDist &);
	virtual ~ProbDist();

	/* assignment */
	ProbDist &operator=(const ProbDist &);

	/* sampling routines */
	virtual int setSeed(unsigned long);
	virtual double randomSample();

	/* other routines */
	inline int getStatus() { return(status); }

public:
	/* internal data */
	int status;
};

#endif
