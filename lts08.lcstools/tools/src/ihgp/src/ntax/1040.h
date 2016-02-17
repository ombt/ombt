#ifndef __1040_H
#define __1040_H

// federal tax class definition

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

// forward declaration
class FilingStatus;
class Exemptions;
class Income;
class AdjustmentsToIncome;
class TaxComputation;
class Credits;
class OtherTaxes;
class Payments;
class RefundOrAmountOwed;

// filing status
enum MyFilingStatus {
	Single,
	MarriedFilingJointReturn,
	MarriedFilingSeparateReturn,
	HeadOfHousehold,
	QualifyingWidowWithDependent
};

// deduction types
enum DeductionType {
	Standard,
	Itemized
};

// filing status class definition
class FilingStatus {
public:
	// constructor and destructor
	FilingStatus();
	FilingStatus(const FilingStatus &);
	~FilingStatus();

	// assignment
	FilingStatus &operator=(const FilingStatus &);

	// input and output
	MyFilingStatus &operator()();
	friend ostream &operator<<(ostream &, const FilingStatus &);
	friend istream &operator>>(istream &, FilingStatus &);

protected:
	// other data
	MyFilingStatus status;
};

// exemptions class definition
class Exemptions: public Number {
public:
	// constructor and destructor
	Exemptions();
	Exemptions(const Exemptions &);
	~Exemptions();

	// assignment
	Exemptions &operator=(const Exemptions &);
	Number &operator=(const Number &);

	// input and output
	friend ostream &operator<<(ostream &, const Exemptions &);
	friend istream &operator>>(istream &, Exemptions &);

protected:
	// other data 
	Boolean yourself;
	Boolean spouse;
	Boolean dependents;
	Number numberOfDependents;
};

// income class definition
class Income: public Money {
public:
	// constructor and destructor
	Income();
	Income(const Income &);
	~Income();

	// assignment
	Income &operator=(const Income &);
	Money &operator=(const Money &);
	Money &operator=(const Number &);

	// input and output
	friend ostream &operator<<(ostream &, const Income &);
	friend istream &operator>>(istream &, Income &);

protected:
	// other data
	Money wagesSalaryTipsEtc;
	Money taxableInterestIncome;
	Money taxExemptIncome;
	Money dividendIncome;
	Money taxableRefundsOfStateAndLocalIncomeTaxes;
	Money alimonyReceived;
	Money businessIncomeOrLoss;
	Money capitalGainOrLoss;
	Money otherGainsOrLosses;
	Money totalIraDistributions;
	Money totalPensionsAndAnnuities;
	Money rentalRealEstateEtc;
	Money farmIncomeOrLoss;
	Money unemploymentCompensation;
	Money socialSecurity;
	Money otherIncome;
};

// adjustments to income class definition
class AdjustmentsToIncome: public Money {
public:
	// constructor and destructor
	AdjustmentsToIncome();
	AdjustmentsToIncome(const AdjustmentsToIncome &);
	~AdjustmentsToIncome();

	// assignment
	AdjustmentsToIncome &operator=(const AdjustmentsToIncome &);
	Money &operator=(const Money &);
	Money &operator=(const Number &);

	// input and output
	friend ostream &operator<<(ostream &, const AdjustmentsToIncome &);
	friend istream &operator>>(istream &, AdjustmentsToIncome &);

protected:
	// other data
	Money yourIraDeduction;
	Money spousesIraDeduction;
	Money movingExpenses;
	Money oneHalfSelfEmploymentTax;
	Money selfEmployedHealthInsuranceDeduction;
	Money keoghAndSelfEmployedSEPPlans;
	Money penaltyOnEarlyWithdrawalOfSavings;
	Money alimoneyPaid;
};

// tax computation class definition
class TaxComputation: public Money {
public:
	// constructor and destructor
	TaxComputation();
	TaxComputation(const TaxComputation &);
	TaxComputation(const Exemptions &, const FilingStatus &, 
		       const Money &);
	~TaxComputation();

	// assignment
	TaxComputation &operator=(const TaxComputation &);
	Money &operator=(const Money &);
	Money &operator=(const Number &);

	// input and output
	friend ostream &operator<<(ostream &, const TaxComputation &);
	friend istream &operator>>(istream &, TaxComputation &);

protected:
	// other data
	FilingStatus status;
	Exemptions exemptions;
	Money adjustedGrossIncome;
	Boolean sixtyFiveOrOlder;
	Boolean blind;
	Boolean spouseSixtyFiveOrOlder;
	Boolean spouseBlind;
	Boolean claimedAsDependent;
	Boolean marriedAndFilingSeparately;
	DeductionType deductionType;
	Money itemizedDeductions;
	Money standardDeductions;
	Money taxableIncome;
	Money tax;
	Money additionalTaxes;
};

// credits class definition
class Credits: public Money {
public:
	// constructor and destructor
	Credits();
	Credits(const Credits &);
	~Credits();

	// assignment
	Credits &operator=(const Credits &);
	Money &operator=(const Money &);
	Money &operator=(const Number &);

	// input and output
	friend ostream &operator<<(ostream &, const Credits &);
	friend istream &operator>>(istream &, Credits &);

protected:
	// other data;
	Money creditForChildAndDependentCare;
	Money creditForElderlyOrDisabled;
	Money foreignTaxCredit;
	Money otherCredits;
};

// other taxes class definition
class OtherTaxes: public Money {
public:
	// constructor and destructor
	OtherTaxes();
	OtherTaxes(const OtherTaxes &);
	~OtherTaxes();

	// assignment
	OtherTaxes &operator=(const OtherTaxes &);
	Money &operator=(const Money &);
	Money &operator=(const Number &);

	// input and output
	friend ostream &operator<<(ostream &, const OtherTaxes &);
	friend istream &operator>>(istream &, OtherTaxes &);

protected:
	// other data
	Money selfEmploymentTax;
	Money alternativeMinimumTax;
	Money recaptureTaxes;
	Money socialSecurityAndMedicareTaxOnNonreportedIncome;
	Money taxOnQualifiedRetirementPlans;
	Money advancedEarnedIncomeCreditPayments;
	Money householdEmploymentTaxes;
};

// payments class definition
class Payments: public Money {
public:
	// constructor and destructor
	Payments();
	Payments(const Payments &);
	~Payments();

	// assignment
	Payments &operator=(const Payments &);
	Money &operator=(const Money &);
	Money &operator=(const Number &);

	// input and output
	friend ostream &operator<<(ostream &, const Payments &);
	friend istream &operator>>(istream &, Payments &);

protected:
	// other data
	Money federalIncomeTaxWithheld;
	Money estimatedTaxPayments;
	Money earnedIncomeCredit;
	Money amountPaidFromExtensions;
	Money excessSocialSecurityAndRRTATax;
	Money otherPayments;
};

// refund or amount owed class definition
class RefundOrAmountOwed: public Money {
public:
	// constructor and destructor
	RefundOrAmountOwed();
	RefundOrAmountOwed(const RefundOrAmountOwed &);
	RefundOrAmountOwed(const Money &, const Payments &);
	~RefundOrAmountOwed();

	// assignment
	RefundOrAmountOwed &operator=(const RefundOrAmountOwed &);
	Money &operator=(const Money &);
	Money &operator=(const Number &);

	// input and output
	friend ostream &operator<<(ostream &, const RefundOrAmountOwed &);
	friend istream &operator>>(istream &, RefundOrAmountOwed &);

protected:
	// other data
	Money totalTaxes;
	Payments payments;
	Money refund;
	Money owed;
	Money penalty;
};

#endif
