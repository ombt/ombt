// schedule A class functions

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>
#include <math.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "number.h"
#include "money.h"
#include "scheduleA.h"
#include "il.h"
#include "realEstate.h"

// constructors and destructor
ScheduleA::ScheduleA():
	Money("Schedule A", 0.0),
	adjustedGrossIncome("Adjusted Gross Income", 0.0),
	filingStatus(),
	medicalAndDentalExpenses(),
	taxesYouPaid(),
	interestYouPaid(),
	giftsToCharity(),
	casualtyAndTheftLosses(),
	jobExpenses(),
	otherMiscellaneousExpenses(),
	totalItemizedDeductions("Total Itemized Deductions", 0.0)
{
	// do nothing
}

ScheduleA::ScheduleA(const ScheduleA &src):
	Money(src),
	adjustedGrossIncome(src.adjustedGrossIncome),
	filingStatus(src.filingStatus),
	medicalAndDentalExpenses(src.medicalAndDentalExpenses),
	taxesYouPaid(src.taxesYouPaid),
	interestYouPaid(src.interestYouPaid),
	giftsToCharity(src.giftsToCharity),
	casualtyAndTheftLosses(src.casualtyAndTheftLosses),
	jobExpenses(src.jobExpenses),
	otherMiscellaneousExpenses(src.otherMiscellaneousExpenses),
	totalItemizedDeductions(src.totalItemizedDeductions)
{
	// do nothing
}

ScheduleA::ScheduleA(const Money &agi, const FilingStatus &fs, 
			const RealEstate &re):
	Money("Schedule A", 0.0),
	adjustedGrossIncome(agi),
	filingStatus(fs),
	medicalAndDentalExpenses(agi),
	taxesYouPaid(agi, fs, re),
	interestYouPaid(re),
	giftsToCharity(),
	casualtyAndTheftLosses(),
	jobExpenses(agi),
	otherMiscellaneousExpenses(),
	totalItemizedDeductions("Total Itemized Deductions", 0.0)
{
	// do nothing
}

ScheduleA::~ScheduleA()
{
	// do nothing
}


// assignment
ScheduleA &
ScheduleA::operator=(const ScheduleA &rhs)
{
	if (this != &rhs)
	{
		Money::operator=(rhs);
		adjustedGrossIncome = rhs.adjustedGrossIncome;
		filingStatus = rhs.filingStatus;
		medicalAndDentalExpenses = rhs.medicalAndDentalExpenses;
		taxesYouPaid = rhs.taxesYouPaid;
		interestYouPaid = rhs.interestYouPaid;
		giftsToCharity = rhs.giftsToCharity;
		casualtyAndTheftLosses = rhs.casualtyAndTheftLosses;
		jobExpenses = rhs.jobExpenses;
		otherMiscellaneousExpenses = rhs.otherMiscellaneousExpenses;
		totalItemizedDeductions = rhs.totalItemizedDeductions;
	}
	return(*this);
}

Money &
ScheduleA::operator=(const Money &rhs)
{
	return(Money::operator=(rhs));
}

Money &
ScheduleA::operator=(const Number &rhs)
{
	return(Money::operator=(rhs));
}

// read and write 
ostream &
operator<<(ostream &os, const ScheduleA &income)
{
	os << "SCHEDULE A" << endl;
	os << "==========" << endl;

	os << income.medicalAndDentalExpenses << endl;
	os << income.taxesYouPaid << endl;
	os << income.interestYouPaid << endl;
	os << income.giftsToCharity << endl;
	os << income.casualtyAndTheftLosses << endl;
	os << income.jobExpenses << endl;
	os << income.otherMiscellaneousExpenses << endl;
	os << income.totalItemizedDeductions << endl;

	os << income.name << ": " << income.amount << endl;
	os << "==========" << endl;

	return(os);
}

istream &
operator>>(istream &is, ScheduleA &income)
{
	cout << "SCHEDULE A" << endl;
	cout << "==========" << endl;

	is >> income.medicalAndDentalExpenses;
	is >> income.taxesYouPaid;
	is >> income.interestYouPaid;
	is >> income.giftsToCharity;
	is >> income.casualtyAndTheftLosses;
	is >> income.jobExpenses;
	is >> income.otherMiscellaneousExpenses;

	income.totalItemizedDeductions = 
		income.medicalAndDentalExpenses +
		income.taxesYouPaid +
		income.interestYouPaid +
		income.giftsToCharity +
		income.casualtyAndTheftLosses +
		income.jobExpenses +
		income.otherMiscellaneousExpenses;

	if (income.adjustedGrossIncome > 114700.0)
	{
		Money amt1 = income.medicalAndDentalExpenses +
			income.taxesYouPaid +
			income.interestYouPaid +
			income.giftsToCharity +
			income.casualtyAndTheftLosses +
			income.jobExpenses +
			income.otherMiscellaneousExpenses;
		Money amt2 = income.medicalAndDentalExpenses +
			income.interestYouPaid.getInvestmentInterest() +
			income.casualtyAndTheftLosses;
		Money diff1 = amt2 - amt1;
		if (diff1 <= 0.0)
		{
			income.totalItemizedDeductions = amt1;
		}
		else
		{
			Money diff2;
			diff1 = 0.8*diff1;
			switch (income.filingStatus())
			{
			case MarriedFilingSeparateReturn:
				diff2 = income.adjustedGrossIncome - 57350.0;
				break;
			default:
				diff2 = income.adjustedGrossIncome - 114700.0;
				break;
			}
			if (diff2 <= 0.0)
			{
				income.totalItemizedDeductions = amt1;
			}
			else
			{
				diff2 = 0.03*diff2;
				if (diff1 < diff2)
				{
					income.totalItemizedDeductions = 
						amt1 - diff1;
				}
				else
				{
					income.totalItemizedDeductions = 
						amt1 - diff2;
				}
			}
		}
	}	
	cout << income.totalItemizedDeductions << endl;

	income = income.totalItemizedDeductions;

	cout << income.name << ": " << income.amount << endl;
	cout << "==========" << endl;

	return(is);
}

// constructors and destructor
MedicalAndDentalExpenses::MedicalAndDentalExpenses():
	Money("Medical And Dental Expenses", 0.0),
	adjustedGrossIncome("Adjusted Gross Income", 0.0),
	medicalAndDentalExpenses("Medical And Dental Expenses", 0.0)
{
	// do nothing
}

MedicalAndDentalExpenses::MedicalAndDentalExpenses(
	const MedicalAndDentalExpenses &src):
	Money(src),
	adjustedGrossIncome(src.adjustedGrossIncome),
	medicalAndDentalExpenses(src.medicalAndDentalExpenses)
{
	// do nothing
}

MedicalAndDentalExpenses::MedicalAndDentalExpenses(const Money &agi):
	Money("Medical And Dental Expenses", 0.0),
	adjustedGrossIncome(agi),
	medicalAndDentalExpenses("Medical And Dental Expenses", 0.0)
{
	// do nothing
}

MedicalAndDentalExpenses::~MedicalAndDentalExpenses()
{
	// do nothing
}


// assignment
MedicalAndDentalExpenses &
MedicalAndDentalExpenses::operator=(const MedicalAndDentalExpenses &rhs)
{
	if (this != &rhs)
	{
		Money::operator=(rhs);
		adjustedGrossIncome = rhs.adjustedGrossIncome;
		medicalAndDentalExpenses = rhs.medicalAndDentalExpenses;
	}
	return(*this);
}

Money &
MedicalAndDentalExpenses::operator=(const Money &rhs)
{
	return(Money::operator=(rhs));
}

Money &
MedicalAndDentalExpenses::operator=(const Number &rhs)
{
	return(Money::operator=(rhs));
}

// read and write 
ostream &
operator<<(ostream &os, const MedicalAndDentalExpenses &income)
{
	os << "MEDICAL AND DENTAL EXPENSES" << endl;
	os << "===========================" << endl;

	os << income.medicalAndDentalExpenses << endl;

	os << income.name << ": " << income.amount << endl;
	os << "==========" << endl;

	return(os);
}

istream &
operator>>(istream &is, MedicalAndDentalExpenses &income)
{
	cout << "MEDICAL AND DENTAL EXPENSES" << endl;
	cout << "===========================" << endl;

	is >> income.medicalAndDentalExpenses;

	income = income.medicalAndDentalExpenses - 
		0.075*income.adjustedGrossIncome;
	if (income < 0.0) 
		income = 0.0;

	cout << income.name << ": " << income.amount << endl;
	cout << "==========" << endl;

	return(is);
}

// constructors and destructor
TaxesYouPaid::TaxesYouPaid():
	Money("Taxes You Paid", 0.0),
	adjustedGrossIncome("Adjusted Gross Income", 0.0),
	filingStatus(),
	realEstate(),
	stateAndLocalTaxes("State And Local Taxes", 0.0),
	realEstateTaxes("Real Estate Taxes", 0.0),
	personalPropertyTaxes("Personal Property Taxes", 0.0),
	otherTaxes("Other Taxes", 0.0)
{
	// do nothing
}

TaxesYouPaid::TaxesYouPaid(const TaxesYouPaid &src):
	Money(src),
	adjustedGrossIncome(src.adjustedGrossIncome),
	filingStatus(src.filingStatus),
	realEstate(src.realEstate),
	stateAndLocalTaxes(src.stateAndLocalTaxes),
	realEstateTaxes(src.realEstateTaxes),
	personalPropertyTaxes(src.personalPropertyTaxes),
	otherTaxes(src.otherTaxes)
{
	// do nothing
}

TaxesYouPaid::TaxesYouPaid(const Money &agi, const FilingStatus &fs,
				const RealEstate &re):
	Money("Taxes You Paid", 0.0),
	adjustedGrossIncome(agi),
	filingStatus(fs),
	realEstate(re),
	stateAndLocalTaxes("State And Local Taxes", 0.0),
	realEstateTaxes("Real Estate Taxes", 0.0),
	personalPropertyTaxes("Personal Property Taxes", 0.0),
	otherTaxes("Other Taxes", 0.0)
{
	// do nothing
}

TaxesYouPaid::~TaxesYouPaid()
{
	// do nothing
}

// assignment
TaxesYouPaid &
TaxesYouPaid::operator=(const TaxesYouPaid &rhs)
{
	if (this != &rhs)
	{
		Money::operator=(rhs);
		adjustedGrossIncome = rhs.adjustedGrossIncome;
		filingStatus = rhs.filingStatus;
		realEstate = rhs.realEstate;
		stateAndLocalTaxes = rhs.stateAndLocalTaxes;
		realEstateTaxes = rhs.realEstateTaxes;
		personalPropertyTaxes = rhs.personalPropertyTaxes;
		otherTaxes = rhs.otherTaxes;
	}
	return(*this);
}

Money &
TaxesYouPaid::operator=(const Money &rhs)
{
	return(Money::operator=(rhs));
}

Money &
TaxesYouPaid::operator=(const Number &rhs)
{
	return(Money::operator=(rhs));
}

// read and write 
ostream &
operator<<(ostream &os, const TaxesYouPaid &income)
{
	os << "TAXES YOU PAID" << endl;
	os << "==============" << endl;

	os << income.stateAndLocalTaxes << endl;
	os << income.realEstateTaxes << endl;
	os << income.personalPropertyTaxes << endl;
	os << income.otherTaxes << endl;

	os << income.name << ": " << income.amount << endl;
	os << "==============" << endl;

	return(os);
}

istream &
operator>>(istream &is, TaxesYouPaid &income)
{
	cout << "TAXES YOU PAID" << endl;
	cout << "==============" << endl;

	Database db;
	Boolean yesno;

	if (db.query("State Taxes", "calculate?", yesno) == OK &&
	    (yesno == True))
	{
		IllinoisTaxTable iltax;
		income.stateAndLocalTaxes = 
			iltax(income.filingStatus, income.adjustedGrossIncome);
		cout << income.stateAndLocalTaxes << endl; 
	}
	else
	{
		is >> income.stateAndLocalTaxes;
	}
	
	if (db.query("Real Estate Taxes", "calculate?", yesno) == OK &&
	    (yesno == True))
	{
		income.realEstateTaxes = 
			income.realEstate.realEstateTaxesPaid();
		cout << income.realEstateTaxes << endl; 
	}
	else
	{
		is >> income.realEstateTaxes;
	}

	is >> income.personalPropertyTaxes;
	is >> income.otherTaxes;

	income = income.stateAndLocalTaxes +
		income.realEstateTaxes +
		income.personalPropertyTaxes +
		income.otherTaxes;

	cout << income.name << ": " << income.amount << endl;
	cout << "==============" << endl;

	return(is);
}

// constructors and destructor
InterestYouPaid::InterestYouPaid():
	Money("Interest You Paid", 0.0),
	homeMortgageInterestAndPoints("Home Mortage Interest And Points", 0.0),
	investmentInterest("Investment Interest", 0.0)
{
	// do nothing
}

InterestYouPaid::InterestYouPaid(const RealEstate &re):
	Money("Interest You Paid", 0.0),
	realEstate(re),
	homeMortgageInterestAndPoints("Home Mortage Interest And Points", 0.0),
	investmentInterest("Investment Interest", 0.0)
{
	// do nothing
}

InterestYouPaid::InterestYouPaid(const InterestYouPaid &src):
	Money(src),
	realEstate(src.realEstate),
	homeMortgageInterestAndPoints(src.homeMortgageInterestAndPoints),
	investmentInterest(src.investmentInterest)
{
	// do nothing
}

InterestYouPaid::~InterestYouPaid()
{
	// do nothing
}

// assignment
InterestYouPaid &
InterestYouPaid::operator=(const InterestYouPaid &rhs)
{
	if (this != &rhs)
	{
		Money::operator=(rhs);
		realEstate = 
			rhs.realEstate;
		homeMortgageInterestAndPoints = 
			rhs.homeMortgageInterestAndPoints;
		investmentInterest = 
			rhs.investmentInterest;
	}
	return(*this);
}

Money &
InterestYouPaid::operator=(const Money &rhs)
{
	return(Money::operator=(rhs));
}

Money &
InterestYouPaid::operator=(const Number &rhs)
{
	return(Money::operator=(rhs));
}

// read and write 
ostream &
operator<<(ostream &os, const InterestYouPaid &income)
{
	os << "INTEREST YOU PAID" << endl;
	os << "=================" << endl;

	os << income.homeMortgageInterestAndPoints << endl;
	os << income.investmentInterest << endl;

	os << income.name << ": " << income.amount << endl;
	os << "=================" << endl;

	return(os);
}

istream &
operator>>(istream &is, InterestYouPaid &income)
{
	cout << "INTEREST YOU PAID" << endl;
	cout << "=================" << endl;

	Database db;
	Boolean yesno;
	if (db.query("Real Estate Interest", "calculate?", yesno) == OK &&
	    (yesno == True))
	{
		Number year("Current year for real estate calculations", 0.0);
		is >> year;

		income.homeMortgageInterestAndPoints = 
			income.realEstate.interestPaid(year);
		cout << income.homeMortgageInterestAndPoints << endl; 
	}
	else
	{
		is >> income.homeMortgageInterestAndPoints;
	}

	is >> income.investmentInterest;

	income = income.homeMortgageInterestAndPoints +
		income.investmentInterest;

	cout << income.name << ": " << income.amount << endl;
	cout << "==============" << endl;;

	return(is);
}

// constructors and destructor
GiftsToCharity::GiftsToCharity():
	Money("Gifts To Charity", 0.0),
	giftsByCashOrCheck("Gifts By Cash Or Check", 0.0),
	giftsNotByCashOrCheck("Gifts Not By Cash Or Check", 0.0),
	carryOverFromPriorYear("Carry Over From Prior year", 0.0)
{
	// do nothing
}

GiftsToCharity::GiftsToCharity(const GiftsToCharity &src):
	Money(src),
	giftsByCashOrCheck(src.giftsByCashOrCheck),
	giftsNotByCashOrCheck(src.giftsNotByCashOrCheck),
	carryOverFromPriorYear(src.carryOverFromPriorYear)
{
	// do nothing
}

GiftsToCharity::~GiftsToCharity()
{
	// do nothing
}

// assignment
GiftsToCharity &
GiftsToCharity::operator=(const GiftsToCharity &rhs)
{
	if (this != &rhs)
	{
		Money::operator=(rhs);
		giftsByCashOrCheck = rhs.giftsByCashOrCheck;
		giftsNotByCashOrCheck = rhs.giftsNotByCashOrCheck;
		carryOverFromPriorYear = rhs.carryOverFromPriorYear;
	}
	return(*this);
}

Money &
GiftsToCharity::operator=(const Money &rhs)
{
	return(Money::operator=(rhs));
}

Money &
GiftsToCharity::operator=(const Number &rhs)
{
	return(Money::operator=(rhs));
}

// read and write 
ostream &
operator<<(ostream &os, const GiftsToCharity &income)
{
	os << "GIFTS TO CHARITY" << endl;
	os << "================" << endl;

	os << income.giftsByCashOrCheck << endl;
	os << income.giftsNotByCashOrCheck << endl;
	os << income.carryOverFromPriorYear << endl;

	os << income.name << ": " << income.amount << endl;
	os << "================" << endl;

	return(os);
}

istream &
operator>>(istream &is, GiftsToCharity &income)
{
	cout << "GIFTS TO CHARITY" << endl;
	cout << "================" << endl;

	is >> income.giftsByCashOrCheck;
	is >> income.giftsNotByCashOrCheck;
	is >> income.carryOverFromPriorYear;

	income = income.giftsByCashOrCheck +
		income.giftsNotByCashOrCheck +
		income.carryOverFromPriorYear;

	cout << income.name << ": " << income.amount << endl;
	cout << "================" << endl;

	return(is);
}

// constructors and destructor
CasualtyAndTheftLosses::CasualtyAndTheftLosses():
	Money("Casualty And Theft Losses", 0.0),
	casualtyAndTheftLosses("Casualty And Theft Losses", 0.0)
{
	// do nothing
}

CasualtyAndTheftLosses::CasualtyAndTheftLosses(
		const CasualtyAndTheftLosses &src):
	Money(src),
	casualtyAndTheftLosses(src.casualtyAndTheftLosses)
{
	// do nothing
}

CasualtyAndTheftLosses::~CasualtyAndTheftLosses()
{
	// do nothing
}

// assignment
CasualtyAndTheftLosses &
CasualtyAndTheftLosses::operator=(const CasualtyAndTheftLosses &rhs)
{
	if (this != &rhs)
	{
		Money::operator=(rhs);
		casualtyAndTheftLosses = rhs.casualtyAndTheftLosses;
	}
	return(*this);
}

Money &
CasualtyAndTheftLosses::operator=(const Money &rhs)
{
	return(Money::operator=(rhs));
}

Money &
CasualtyAndTheftLosses::operator=(const Number &rhs)
{
	return(Money::operator=(rhs));
}

// read and write 
ostream &
operator<<(ostream &os, const CasualtyAndTheftLosses &income)
{
	os << "CASUALTY AND THEFT LOSSES" << endl;
	os << "=========================" << endl;

	os << income.casualtyAndTheftLosses << endl;

	os << income.name << ": " << income.amount << endl;
	os << "=========================" << endl;

	return(os);
}

istream &
operator>>(istream &is, CasualtyAndTheftLosses &income)
{
	cout << "CASUALTY AND THEFT LOSSES" << endl;
	cout << "=========================" << endl;

	is >> income.casualtyAndTheftLosses;

	income = income.casualtyAndTheftLosses;

	cout << income.name << ": " << income.amount << endl;
	cout << "=========================" << endl;

	return(is);
}

// constructors and destructor
UnreimbursedEmployeeExpenses::UnreimbursedEmployeeExpenses():
	Money("Unreimbursed Employee Expenses", 0.0),
	unreimbursedEmployeeExpenses("Unreimbursed Employee Expenses", 0.0)
{
	// do nothing
}

UnreimbursedEmployeeExpenses::UnreimbursedEmployeeExpenses(
		const UnreimbursedEmployeeExpenses &src):
	Money(src),
	unreimbursedEmployeeExpenses(src.unreimbursedEmployeeExpenses)
{
	// do nothing
}

UnreimbursedEmployeeExpenses::~UnreimbursedEmployeeExpenses()
{
	// do nothing
}

// assignment
UnreimbursedEmployeeExpenses &
UnreimbursedEmployeeExpenses::operator=(const 
	UnreimbursedEmployeeExpenses &rhs)
{
	if (this != &rhs)
	{
		Money::operator=(rhs);
		unreimbursedEmployeeExpenses = rhs.unreimbursedEmployeeExpenses;
	}
	return(*this);
}

Money &
UnreimbursedEmployeeExpenses::operator=(const Money &rhs)
{
	return(Money::operator=(rhs));
}

Money &
UnreimbursedEmployeeExpenses::operator=(const Number &rhs)
{
	return(Money::operator=(rhs));
}

// read and write 
ostream &
operator<<(ostream &os, const UnreimbursedEmployeeExpenses &income)
{
	os << "UNREIMBURSED EMPLOYEE EXPENSES" << endl;
	os << "==============================" << endl;

	os << "Unreimbursed Employee Expenses: "
		<< income.unreimbursedEmployeeExpenses << endl;

	os << income.name << ": " << income.amount << endl;
	os << "==============================" << endl;

	return(os);
}

istream &
operator>>(istream &is, UnreimbursedEmployeeExpenses &income)
{
	cout << "UNREIMBURSED EMPLOYEE EXPENSES" << endl;
	cout << "==============================" << endl;

	is >> income.unreimbursedEmployeeExpenses;

	income = income.unreimbursedEmployeeExpenses;

	cout << income.name << ": " << income.amount << endl;
	cout << "==============================" << endl;

	return(is);
}

// constructors and destructor
JobExpenses::JobExpenses():
	Money("Job Expenses", 0.0),
	adjustedGrossIncome("Adjusted Gross Income", 0.0),
	unreimbursedEmployeeExpenses(),
	taxPreparationFees("Tax Preparation Fees", 0.0),
	otherExpenses("Other Expenses", 0.0)
{
	// do nothing
}

JobExpenses::JobExpenses(
	const JobExpenses &src):
	Money(src),
	adjustedGrossIncome(src.adjustedGrossIncome),
	unreimbursedEmployeeExpenses(src.unreimbursedEmployeeExpenses),
	taxPreparationFees(src.taxPreparationFees),
	otherExpenses(src.otherExpenses)
{
	// do nothing
}

JobExpenses::JobExpenses(const Money &agi):
	Money("Job Expenses", 0.0),
	adjustedGrossIncome(agi),
	unreimbursedEmployeeExpenses(),
	taxPreparationFees("Tax Preparation Fees", 0.0),
	otherExpenses("Other Expenses", 0.0)
{
	// do nothing
}

JobExpenses::~JobExpenses()
{
	// do nothing
}


// assignment
JobExpenses &
JobExpenses::operator=(const JobExpenses &rhs)
{
	if (this != &rhs)
	{
		Money::operator=(rhs);
		adjustedGrossIncome = 
			rhs.adjustedGrossIncome;
		unreimbursedEmployeeExpenses = 
			rhs.unreimbursedEmployeeExpenses;
		taxPreparationFees = 
			rhs.taxPreparationFees;
		otherExpenses = 
			rhs.otherExpenses;
	}
	return(*this);
}

Money &
JobExpenses::operator=(const Money &rhs)
{
	return(Money::operator=(rhs));
}

Money &
JobExpenses::operator=(const Number &rhs)
{
	return(Money::operator=(rhs));
}

// read and write 
ostream &
operator<<(ostream &os, const JobExpenses &income)
{
	os << "JOB EXPENSES" << endl;
	os << "============" << endl;

	os << income.unreimbursedEmployeeExpenses << endl;
	os << income.taxPreparationFees << endl;
	os << income.otherExpenses << endl;

	os << income.name << ": " << income.amount << endl;
	os << "============" << endl;

	return(os);
}

istream &
operator>>(istream &is, JobExpenses &income)
{
	cout << "JOB EXPENSES" << endl;
	cout << "============" << endl;

	is >> income.unreimbursedEmployeeExpenses;
	is >> income.taxPreparationFees;
	is >> income.otherExpenses;
	income = income.unreimbursedEmployeeExpenses + 

		income.taxPreparationFees + 
		income.otherExpenses - 
		0.02*income.adjustedGrossIncome;
	if (income < 0.0) 
		income = 0.0;

	cout << income.name << ": " << income.amount << endl;
	cout << "============" << endl;

	return(is);
}

// constructors and destructor
OtherMiscellaneousExpenses::OtherMiscellaneousExpenses():
	Money("Other Miscellaneous Expenses", 0.0),
	otherMiscellaneousExpenses("Other Miscellaneous Expenses", 0.0)
{
	// do nothing
}

OtherMiscellaneousExpenses::OtherMiscellaneousExpenses(
		const OtherMiscellaneousExpenses &src):
	Money(src),
	otherMiscellaneousExpenses(src.otherMiscellaneousExpenses)
{
	// do nothing
}

OtherMiscellaneousExpenses::~OtherMiscellaneousExpenses()
{
	// do nothing
}

// assignment
OtherMiscellaneousExpenses &
OtherMiscellaneousExpenses::operator=(const OtherMiscellaneousExpenses &rhs)
{
	if (this != &rhs)
	{
		Money::operator=(rhs);
		otherMiscellaneousExpenses = rhs.otherMiscellaneousExpenses;
	}
	return(*this);
}

Money &
OtherMiscellaneousExpenses::operator=(const Money &rhs)
{
	return(Money::operator=(rhs));
}

Money &
OtherMiscellaneousExpenses::operator=(const Number &rhs)
{
	return(Money::operator=(rhs));
}

// read and write 
ostream &
operator<<(ostream &os, const OtherMiscellaneousExpenses &income)
{
	os << "OTHER MISCELLANEOUS EXPENSES" << endl;
	os << "============================" << endl;

	os << income.otherMiscellaneousExpenses << endl;

	os << income.name << ": " << income.amount << endl;
	os << "============================" << endl;

	return(os);
}

istream &
operator>>(istream &is, OtherMiscellaneousExpenses &income)
{
	cout << "OTHER MISCELLANEOUS EXPENSES" << endl;
	cout << "============================" << endl;

	is >> income.otherMiscellaneousExpenses;

	income = income.otherMiscellaneousExpenses;

	cout << income.name << ": " << income.amount << endl;
	cout << "============================" << endl;

	return(is);
}
