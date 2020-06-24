#ifndef __REALESTATE_H
#define __REALESTATE_H

// real estate class definition

// headers
#include <stdlib.h>
#include <iostream.h>
#include <math.h>
#include <errno.h>
#include <assert.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "number.h"
#include "money.h"

// forward declaration
class RealEstate;

// real estate class definition
class RealEstate: public Money {
public:
	// constructor and destructor
	RealEstate();
	RealEstate(const RealEstate &);
	~RealEstate();

	// assignment
	RealEstate &operator=(const RealEstate &);
	Money &operator=(const Money &);
	Money &operator=(const Number &);

	// query functions
	Money pointsPaid() const;
	Money interestPaid(const Number &) const;
	Money realEstateTaxesPaid() const;
	Money monthlyMortgagePayment() const;
	Money monthlyRealEstateTaxPayment() const;
	Money monthlyTotalPayment() const;

	// input and output
	friend ostream &operator<<(ostream &, const RealEstate &);
	friend istream &operator>>(istream &, RealEstate &);

protected:
	// other data
	Money loanPrincipal;
	Number loanInterest;
	Number loanLength;
	Money loanPoints;
	Money loanAdditionalCosts;
	Number taxRate;
};

#endif
