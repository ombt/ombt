// number class functions

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>
#include <math.h>

// local headers
#include "realEstate.h"

// local constants
static const double MonthsPerYear = 12.0;

// constructors and destructor
RealEstate::RealEstate():
	Money("Real Estate Value", 0.0),
	loanPrincipal("Real Estate Loan Principal", 0.0),
	loanInterest("Real Estate Loan Interest (in percentage)", 0.0),
	loanLength("Real Estate Loan Length (in years)", 0.0),
	loanPoints("Real Estate Loan Points (in percentage)", 0.0),
	loanAdditionalCosts("Real Estate Loan Additional Costs", 0.0),
	taxRate("Real Estate Tax Rate (in percentage)", 0.0)
{
	// do nothing
}

RealEstate::RealEstate(const RealEstate &src):
	Money(src),
	loanPrincipal(src.loanPrincipal),
	loanInterest(src.loanInterest),
	loanLength(src.loanLength),
	loanPoints(src.loanPoints),
	loanAdditionalCosts(src.loanAdditionalCosts),
	taxRate(src.taxRate)
{
	// do nothing
}

RealEstate::~RealEstate()
{
	// do nothing
}


// assignment
RealEstate &
RealEstate::operator=(const RealEstate &rhs)
{
	if (this != &rhs)
	{
		Money::operator=(rhs);
		loanPrincipal = rhs.loanPrincipal;
		loanInterest = rhs.loanInterest;
		loanLength = rhs.loanLength;
		loanPoints = rhs.loanPoints;
		loanAdditionalCosts = rhs.loanAdditionalCosts;
		taxRate = rhs.taxRate;
	}
	return(*this);
}

Money &
RealEstate::operator=(const Money &rhs)
{
	return(Money::operator=(rhs));
}

Money &
RealEstate::operator=(const Number &rhs)
{
	return(Money::operator=(rhs));
}

// query functions
Money
RealEstate::pointsPaid() const
{
	return(loanPoints);
}

Money
RealEstate::interestPaid(const Number &year) const
{
	Money lp = loanPrincipal;

	Number lly = loanLength;
	Number llm = loanLength*MonthsPerYear;

	Number liy = loanInterest/100.0;
	Number lim = liy/MonthsPerYear;

	Money pm = lp*lim/(1.0-pow((1.0+lim), -1.0*llm));

	Number s = (year - 1.0)*MonthsPerYear;
	Number e = year*MonthsPerYear;
	if (s < 0.0)
		s = 0.0;

	Money sp = lp*pow((1.0+lim), s) - pm*(-1.0+pow((1.0+lim), s))/lim;
	Money ep = lp*pow((1.0+lim), e) - pm*(-1.0+pow((1.0+lim), e))/lim;

	return(Money(MonthsPerYear*pm - (sp - ep)));
}

Money
RealEstate::realEstateTaxesPaid() const
{
	return(*this*taxRate/Number(100.0));
}

Money
RealEstate::monthlyMortgagePayment() const
{
	Money lp = loanPrincipal;

	Number lly = loanLength;
	Number llm = loanLength*MonthsPerYear;

	Number liy = loanInterest/100.0;
	Number lim = liy/MonthsPerYear;

	return(Money(lp*lim/(1.0-pow((1.0+lim), -1.0*llm))));
}

Money
RealEstate::monthlyRealEstateTaxPayment() const
{
	return(realEstateTaxesPaid()/MonthsPerYear);
}

Money
RealEstate::monthlyTotalPayment() const
{
	return(Money(monthlyMortgagePayment()+monthlyRealEstateTaxPayment()));
}


// read and write 
ostream &
operator<<(ostream &os, const RealEstate &re)
{
	os << "REAL ESTATE" << endl;
	os << "===========" << endl;

	os << re.name << ": " << re.amount << endl;

	os << re.loanPrincipal << endl;
	os << re.loanInterest << endl;
	os << re.loanLength << endl;
	os << re.loanPoints << endl;
	os << re.loanAdditionalCosts << endl;
	os << re.taxRate << endl;

	os << "===========" << endl;

	return(os);
}

istream &
operator>>(istream &is, RealEstate &re)
{
	cout << "REAL ESTATE" << endl;
	cout << "===========" << endl;

	cout << "Enter " << re.name << ": ";
	is >> re.amount;

	is >> re.loanPrincipal;
	is >> re.loanInterest;
	is >> re.loanLength;
	is >> re.loanPoints;
	is >> re.loanAdditionalCosts;
	is >> re.taxRate;

	cout << "===========" << endl;

	return(is);
}

