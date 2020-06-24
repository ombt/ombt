#ifndef __SCHEDULE_H
#define __SCHEDULE_H

// schedule A tax class definition

// headers
#include <stdlib.h>
#include <iostream.h>
#include <math.h>
#include <errno.h>
#include <assert.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "boolean.h"
#include "number.h"
#include "money.h"
#include "1040.h"
#include "realEstate.h"

// forward declaration

class ScheduleA;
class MedicalAndDentalExpenses;
class TaxesYouPaid;
class InterestYouPaid;
class GiftsToCharity;
class CasualtyAndTheftLosses;
class JobExpenses;
class UnreimbursedEmployeeExpenses;
class OtherMiscellaneousExpenses;
class TotalItemizedDeductions;

// medical and dental expenses class definition
class MedicalAndDentalExpenses: public Money {
public:
	// constructor and destructor
	MedicalAndDentalExpenses();
	MedicalAndDentalExpenses(const MedicalAndDentalExpenses &);
	MedicalAndDentalExpenses(const Money &);
	~MedicalAndDentalExpenses();

	// assignment
	MedicalAndDentalExpenses 	
		&operator=(const MedicalAndDentalExpenses &);
	Money &operator=(const Money &);
	Money &operator=(const Number &);

	// input and output
	friend ostream &operator<<(ostream &, 
		const MedicalAndDentalExpenses &);
	friend istream &operator>>(istream &, 
		MedicalAndDentalExpenses &);

protected:
	// other data
	Money adjustedGrossIncome;
	Money medicalAndDentalExpenses;
};

// taxes you paid class definition
class TaxesYouPaid: public Money {
public:
	// constructor and destructor
	TaxesYouPaid();
	TaxesYouPaid(const TaxesYouPaid &);
	TaxesYouPaid(const Money &, const FilingStatus &, const RealEstate &);
	~TaxesYouPaid();

	// assignment
	TaxesYouPaid &operator=(const TaxesYouPaid &);
	Money &operator=(const Money &);
	Money &operator=(const Number &);

	// input and output
	friend ostream &operator<<(ostream &, const TaxesYouPaid &);
	friend istream &operator>>(istream &, TaxesYouPaid &);

protected:
	// other data;
	Money adjustedGrossIncome;
	FilingStatus filingStatus;
	RealEstate realEstate;
	Money stateAndLocalTaxes;
	Money realEstateTaxes;
	Money personalPropertyTaxes;
	Money otherTaxes;
};

// interest you paid class definition
class InterestYouPaid: public Money {
public:
	// constructor and destructor
	InterestYouPaid();
	InterestYouPaid(const RealEstate &);
	InterestYouPaid(const InterestYouPaid &);
	~InterestYouPaid();

	// assignment
	InterestYouPaid &operator=(const InterestYouPaid &);
	Money &operator=(const Money &);
	Money &operator=(const Number &);

	// input and output
	friend ostream &operator<<(ostream &, const InterestYouPaid &);
	friend istream &operator>>(istream &, InterestYouPaid &);

	// other functions
	inline Money getInvestmentInterest() const {
		return(investmentInterest);
	};

protected:
	// other data
	RealEstate realEstate;
	Money homeMortgageInterestAndPoints;
	Money investmentInterest;
};

// gifts to charity class definition
class GiftsToCharity: public Money {
public:
	// constructor and destructor
	GiftsToCharity();
	GiftsToCharity(const GiftsToCharity &);
	~GiftsToCharity();

	// assignment
	GiftsToCharity &operator=(const GiftsToCharity &);
	Money &operator=(const Money &);
	Money &operator=(const Number &);

	// input and output
	friend ostream &operator<<(ostream &, const GiftsToCharity &);
	friend istream &operator>>(istream &, GiftsToCharity &);

protected:
	// other data
	Money giftsByCashOrCheck;
	Money giftsNotByCashOrCheck;
	Money carryOverFromPriorYear;
};

// casualty and theft losses class definition
class CasualtyAndTheftLosses: public Money {
public:
	// constructor and destructor
	CasualtyAndTheftLosses();
	CasualtyAndTheftLosses(const CasualtyAndTheftLosses &);
	~CasualtyAndTheftLosses();

	// assignment
	CasualtyAndTheftLosses 
		&operator=(const CasualtyAndTheftLosses &);
	Money &operator=(const Money &);
	Money &operator=(const Number &);

	// input and output
	friend ostream &operator<<(ostream &, 
		const CasualtyAndTheftLosses &);
	friend istream &operator>>(istream &, 
		CasualtyAndTheftLosses &);
protected:
	// other data
	Money casualtyAndTheftLosses;
};

// unreimbursed employee expenses class definition
class UnreimbursedEmployeeExpenses: public Money {
public:
	// constructor and destructor
	UnreimbursedEmployeeExpenses();
	UnreimbursedEmployeeExpenses(const 
		UnreimbursedEmployeeExpenses &);
	~UnreimbursedEmployeeExpenses();

	// assignment
	UnreimbursedEmployeeExpenses 
		&operator=(const UnreimbursedEmployeeExpenses &);
	Money &operator=(const Money &);
	Money &operator=(const Number &);

	// input and output
	friend ostream &operator<<(ostream &, 
		const UnreimbursedEmployeeExpenses &);
	friend istream &operator>>(istream &, 
		UnreimbursedEmployeeExpenses &);

protected:
	// other data
	Money unreimbursedEmployeeExpenses;
};

// job expenses class definition
class JobExpenses: public Money {
public:
	// constructor and destructor
	JobExpenses();
	JobExpenses(const JobExpenses &);
	JobExpenses(const Money &);
	~JobExpenses();

	// assignment
	JobExpenses &operator=(const JobExpenses &);
	Money &operator=(const Money &);
	Money &operator=(const Number &);

	// input and output
	friend ostream &operator<<(ostream &, const JobExpenses &);
	friend istream &operator>>(istream &, JobExpenses &);

protected:
	// other data;
	Money adjustedGrossIncome;
	UnreimbursedEmployeeExpenses unreimbursedEmployeeExpenses;
	Money taxPreparationFees;
	Money otherExpenses;
};

// other miscellaneous expenses class definition
class OtherMiscellaneousExpenses: public Money {
public:
	// constructor and destructor
	OtherMiscellaneousExpenses();
	OtherMiscellaneousExpenses(const 
		OtherMiscellaneousExpenses &);
	~OtherMiscellaneousExpenses();

	// assignment
	OtherMiscellaneousExpenses 
		&operator=(const OtherMiscellaneousExpenses &);
	Money &operator=(const Money &);
	Money &operator=(const Number &);

	// input and output
	friend ostream &operator<<(ostream &, 
		const OtherMiscellaneousExpenses &);
	friend istream &operator>>(istream &, 
		OtherMiscellaneousExpenses &);
protected:

	// other data;
	Money otherMiscellaneousExpenses;
};

// schedule A class definition
class ScheduleA: public Money {
public:
	// constructor and destructor
	ScheduleA();
	ScheduleA(const ScheduleA &);
	ScheduleA(const Money &, const FilingStatus &, const RealEstate &);
	~ScheduleA();

	// assignment
	ScheduleA &operator=(const ScheduleA &);
	Money &operator=(const Money &);
	Money &operator=(const Number &);

	// input and output
	friend ostream &operator<<(ostream &, const ScheduleA &);
	friend istream &operator>>(istream &, ScheduleA &);

protected:
	// other data
	Money adjustedGrossIncome;
	FilingStatus filingStatus;
	MedicalAndDentalExpenses medicalAndDentalExpenses;
	TaxesYouPaid taxesYouPaid;
	InterestYouPaid interestYouPaid;
	GiftsToCharity giftsToCharity;
	CasualtyAndTheftLosses casualtyAndTheftLosses;
	JobExpenses jobExpenses;
	OtherMiscellaneousExpenses otherMiscellaneousExpenses;
	Money totalItemizedDeductions;
};

#endif
