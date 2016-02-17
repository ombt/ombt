#ifndef __TAXTABLE_H
#define __TAXTABLE_H

// tax table class definition

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>
#include <values.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "number.h"
#include "money.h"
#include "1040.h"

// definitions
#define MaxFloat MAXFLOAT

// forward declaration
class TaxTable;

// internal represenation
struct TaxTableRep {
	// constructor
	TaxTableRep(double s, double e, double r):
		start(Number(s)), end(Number(e)), rate(r) { }
	TaxTableRep(const TaxTableRep &s):
		start(s.start), end(s.end), rate(s.rate) { }

	// output
	friend ostream &operator<<(ostream &os, const TaxTableRep &ttr) {
		os << "start: " << ttr.start << endl;
		os << "end  : " << ttr.end << endl;
		os << "rate : " << ttr.rate << endl;
		return(os);
	}

	// data
	Money start;
	Money end;
	Number rate;
};

// tax table class definition
class TaxTable {
public:
	// constructor and destructor
	TaxTable();
	TaxTable(const TaxTable &);
	virtual ~TaxTable();

	// get taxes
	virtual Money operator()(FilingStatus &, const Money &) const;

protected:
	// constructor, but only for derived classes
	TaxTable(const TaxTableRep *, const TaxTableRep *, 
		const TaxTableRep *, const TaxTableRep *, 
		const TaxTableRep *);

protected:
	// other data
	const TaxTableRep *singleRateTable;
	const TaxTableRep *marriedFilingJointReturnRateTable;
	const TaxTableRep *marriedFilingSeparateReturnRateTable;
	const TaxTableRep *headOfHouseholdRateTable;
	const TaxTableRep *qualifyingWidowWithDependentRateTable;
};

#endif
