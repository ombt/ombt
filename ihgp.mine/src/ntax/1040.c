// federal tax class functions

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
#include "1040.h"
#include "federal.h"
#include "scheduleA.h"
#include "db.h"

// constructors and destructor
FilingStatus::FilingStatus():
	status(Single)
{
	// do nothing
}

FilingStatus::FilingStatus(const FilingStatus &src):
	status(src.status)
{
	// do nothing
}

FilingStatus::~FilingStatus()
{
	// do nothing
}

// assignment
FilingStatus &
FilingStatus::operator=(const FilingStatus &rhs)
{
	if (this != &rhs)
	{
		status = rhs.status;
	}
	return(*this);
}

// read and write 
ostream &
operator<<(ostream &os, const FilingStatus &s)
{
	os << "FILING STATUS" << endl;
	os << "=============" << endl;

	os << "Filing Status: ";
	switch (s.status)
	{
	case Single:
		os << "Single";
		break;
	case MarriedFilingJointReturn:
		os << "Married Filing Joint Return";
		break;
	case MarriedFilingSeparateReturn:
		os << "Married Filing Separate Return";
		break;
	case HeadOfHousehold:
		os << "Head Of Household";
		break;
	case QualifyingWidowWithDependent:
		os << "Qualifying Widow With Dependent";
		break;
	}
	os << endl;
	return(os);
}

istream &
operator>>(istream &is, FilingStatus &s)
{
	int answer;

	cout << "FILING STATUS" << endl;
	cout << "=============" << endl;

	// list choices
	cout << "Filing Status: " << endl;
	cout << "0) Single" << endl;
	cout << "1) Married Filing Joint Return" << endl;
	cout << "2) Married Filing Separate Return" << endl;
	cout << "3) Head Of Household" << endl;
	cout << "4) Qualifying Widow With Dependent" << endl;
	cout << "choose one: ";

	// get choice
	is >> answer;
	cout << "=============" << endl;

	// save status
	s.status = MyFilingStatus(answer);
	return(is);
}

MyFilingStatus &
FilingStatus::operator()()
{
	return(status);
}

// constructors and destructor
Exemptions::Exemptions():
	Number(1), 
	yourself(True), 
	spouse(False), 
	dependents(False),
	numberOfDependents(0.0)
{
	// do nothing
}

Exemptions::Exemptions(const Exemptions &src):
	Number(), 
	yourself(src.yourself), 
	spouse(src.spouse), 
	dependents(src.dependents), 
	numberOfDependents(src.numberOfDependents)
{
	// do nothing
}

Exemptions::~Exemptions()
{
	// do nothing
}

// assignment
Exemptions &
Exemptions::operator=(const Exemptions &rhs)
{
	if (this != &rhs)
	{
		Number::operator=(rhs);
		yourself = rhs.yourself;
		spouse = rhs.spouse;
		dependents = rhs.dependents;
		numberOfDependents = rhs.numberOfDependents;
	}
	return(*this);
}

Number &
Exemptions::operator=(const Number &rhs)
{
	return(Number::operator=(rhs));
}

// read and write 
ostream &
operator<<(ostream &os, const Exemptions &e)
{
	os << "EXEMPTIONS" << endl;
	os << "==========" << endl;

	os << "Exemptions: " << e.n << endl;
	os << "==========" << endl;
	return(os);
}

istream &
operator>>(istream &is, Exemptions &e)
{
	char ans;
	int ians;

	cout << "EXEMPTIONS" << endl;
	cout << "==========" << endl;

	e = 0.0;
	cout << "Yourself (y/n): ";
	is >> ans;
	if (ans == 'y')
	{
		e.yourself = True;
		e += 1;
	}
	else
		e.yourself = False;

	cout << "Spouse (y/n): ";
	is >> ans;
	if (ans == 'y')
	{
		e.spouse = True;
		e += 1;
	}
	else
		e.spouse = False;

	cout << "Dependents (y/n): ";
	is >> ans;
	if (ans == 'y')
	{
		cout << "Number of dependents: ";
		is >> ians;
		e.dependents = True;
		e.numberOfDependents = ians;
		e += ians;
	}
	else
	{
		e.dependents = False;
		e.numberOfDependents = 0;
	}
	cout << "Exemptions: " << e.n << endl;
	cout << "==========" << endl;
	return(is);
}

// constructors and destructor
Income::Income():
	Money("Income", 0.0),
	wagesSalaryTipsEtc("Wages Salary Tips Etc", 0.0),
	taxableInterestIncome("Taxable Interest Income", 0.0),
	taxExemptIncome("Tax Exempt Income", 0.0),
	dividendIncome("Dividend Income", 0.0),
	taxableRefundsOfStateAndLocalIncomeTaxes(
		"Taxable Refunds Of State And Local Income Taxes", 0.0),
	alimonyReceived("Alimony Received", 0.0),
	businessIncomeOrLoss("Business Income Or Loss", 0.0),
	capitalGainOrLoss("Capital Gain Or Loss", 0.0),
	otherGainsOrLosses("Other Gains Or Losses", 0.0),
	totalIraDistributions("Total Ira Distributions", 0.0),
	totalPensionsAndAnnuities("Total Pensions And Annuities", 0.0),
	rentalRealEstateEtc("Rental Real Estate Etc", 0.0),
	farmIncomeOrLoss("Farm Income Or Loss", 0.0),
	unemploymentCompensation("Unemployment Compensation", 0.0),
	socialSecurity("Social Security", 0.0),
	otherIncome("Other Income", 0.0)
{
	// do nothing
}

Income::Income(const Income &src):
	Money(src),
	wagesSalaryTipsEtc(src.wagesSalaryTipsEtc), 
	taxableInterestIncome(src.taxableInterestIncome), 
	taxExemptIncome(src.taxExemptIncome),
	dividendIncome(src.dividendIncome), 
	taxableRefundsOfStateAndLocalIncomeTaxes(
		src.taxableRefundsOfStateAndLocalIncomeTaxes),
	alimonyReceived(src.alimonyReceived), 
	businessIncomeOrLoss(src.businessIncomeOrLoss),
	capitalGainOrLoss(src.capitalGainOrLoss), 
	otherGainsOrLosses(src.otherGainsOrLosses), 
	totalIraDistributions(src.totalIraDistributions),
	totalPensionsAndAnnuities(src.totalPensionsAndAnnuities), 
	rentalRealEstateEtc(src.rentalRealEstateEtc), 
	farmIncomeOrLoss(src.farmIncomeOrLoss), 
	unemploymentCompensation(src.unemploymentCompensation), 
	socialSecurity(src.socialSecurity),
	otherIncome(src.otherIncome)
{
	// do nothing
}

Income::~Income()
{
	// do nothing
}


// assignment
Income &
Income::operator=(const Income &rhs)
{
	if (this != &rhs)
	{
		Money::operator=(rhs);
		wagesSalaryTipsEtc = rhs.wagesSalaryTipsEtc;
		taxableInterestIncome = rhs.taxableInterestIncome;
		taxExemptIncome = rhs.taxExemptIncome;
		dividendIncome = rhs.dividendIncome;
		taxableRefundsOfStateAndLocalIncomeTaxes =
			rhs.taxableRefundsOfStateAndLocalIncomeTaxes;
		alimonyReceived = rhs.alimonyReceived;
		businessIncomeOrLoss = rhs.businessIncomeOrLoss;
		capitalGainOrLoss = rhs.capitalGainOrLoss;
		otherGainsOrLosses = rhs.otherGainsOrLosses;
		totalIraDistributions = rhs.totalIraDistributions;
		totalPensionsAndAnnuities = rhs.totalPensionsAndAnnuities;
		rentalRealEstateEtc = rhs.rentalRealEstateEtc;
		farmIncomeOrLoss = rhs.farmIncomeOrLoss;
		unemploymentCompensation = rhs.unemploymentCompensation;
		socialSecurity = rhs.socialSecurity;
		otherIncome = rhs.otherIncome;
	}
	return(*this);
}

Money &
Income::operator=(const Money &rhs)
{
	return(Money::operator=(rhs));
}

Money &
Income::operator=(const Number &rhs)
{
	return(Money::operator=(rhs));
}

// read and write 
ostream &
operator<<(ostream &os, const Income &income)
{
	os << "INCOME" << endl;
	os << "======" << endl;

	os << income.wagesSalaryTipsEtc << endl;
	os << income.taxableInterestIncome << endl;
	os << income.taxExemptIncome << endl;
	os << income.dividendIncome << endl;
	os << income.taxableRefundsOfStateAndLocalIncomeTaxes << endl;
	os << income.alimonyReceived << endl;
	os << income.businessIncomeOrLoss << endl;
	os << income.capitalGainOrLoss << endl;
	os << income.otherGainsOrLosses << endl;
	os << income.totalIraDistributions << endl;
	os << income.totalPensionsAndAnnuities << endl;
	os << income.rentalRealEstateEtc << endl;
	os << income.farmIncomeOrLoss << endl;
	os << income.unemploymentCompensation << endl;
	os << income.socialSecurity << endl;
	os << income.otherIncome << endl;

	os << income.name << ": " << income.amount << endl;
	os << "======" << endl;

	return(os);
}

istream &
operator>>(istream &is, Income &income)
{
	cout << "INCOME" << endl;
	cout << "======" << endl;

	is >> income.wagesSalaryTipsEtc;
	is >> income.taxableInterestIncome;
	is >> income.taxExemptIncome;
	is >> income.dividendIncome;
	is >> income.taxableRefundsOfStateAndLocalIncomeTaxes;
	is >> income.alimonyReceived;
	is >> income.businessIncomeOrLoss;
	is >> income.capitalGainOrLoss;
	is >> income.otherGainsOrLosses;
	is >> income.totalIraDistributions;
	is >> income.totalPensionsAndAnnuities;
	is >> income.rentalRealEstateEtc;
	is >> income.farmIncomeOrLoss;
	is >> income.unemploymentCompensation;
	is >> income.socialSecurity;
	is >> income.otherIncome;

	income = income.wagesSalaryTipsEtc +
		income.taxableInterestIncome +
		income.dividendIncome +
		income.taxableRefundsOfStateAndLocalIncomeTaxes + 
		income.alimonyReceived + 
		income.businessIncomeOrLoss +
		income.capitalGainOrLoss + 
		income.otherGainsOrLosses +
		income.totalIraDistributions + 
		income.totalPensionsAndAnnuities +
		income.rentalRealEstateEtc + 
		income.farmIncomeOrLoss  +
		income.unemploymentCompensation + 
		income.socialSecurity +
		income.otherIncome;

	cout << income.name << ": " << income.amount << endl;
	cout << "======" << endl;

	return(is);
}

// constructors and destructor
AdjustmentsToIncome::AdjustmentsToIncome():
	Money("Adjustments To Income", 0.0),
	yourIraDeduction("Your Ira Deduction", 0.0),
	spousesIraDeduction("Spouses Ira Deduction", 0.0),
	movingExpenses("Moving Expenses", 0.0),
	oneHalfSelfEmploymentTax("One Half Self Employment Tax", 0.0),
	selfEmployedHealthInsuranceDeduction(
		"Self Employed Health Insurance Deduction", 0.0), 
	keoghAndSelfEmployedSEPPlans(
		"Keogh And Self Employed SEP Plans", 0.0),
	penaltyOnEarlyWithdrawalOfSavings(
		"Penalty On Early Withdrawal Of Savings", 0.0), 
	alimoneyPaid("Alimoney Paid", 0.0)
{
	// do nothing
}

AdjustmentsToIncome::AdjustmentsToIncome(const AdjustmentsToIncome &src):
	Money(src),
	yourIraDeduction(src.yourIraDeduction), 
	spousesIraDeduction(src.spousesIraDeduction), 
	movingExpenses(src.movingExpenses),
	oneHalfSelfEmploymentTax(src.oneHalfSelfEmploymentTax), 
	selfEmployedHealthInsuranceDeduction(
		src.selfEmployedHealthInsuranceDeduction), 
	keoghAndSelfEmployedSEPPlans(src.keoghAndSelfEmployedSEPPlans), 
	penaltyOnEarlyWithdrawalOfSavings(
		src.penaltyOnEarlyWithdrawalOfSavings), 
	alimoneyPaid(src.alimoneyPaid)
{
	// do nothing
}

AdjustmentsToIncome::~AdjustmentsToIncome()
{
	// do nothing
}

// assignment
AdjustmentsToIncome &
AdjustmentsToIncome::operator=(const AdjustmentsToIncome &rhs)
{
	if (this != &rhs)
	{
		Money::operator=(rhs);
		yourIraDeduction = rhs.yourIraDeduction;
		spousesIraDeduction = rhs.spousesIraDeduction;
		movingExpenses = rhs.movingExpenses;
		oneHalfSelfEmploymentTax = rhs.oneHalfSelfEmploymentTax;
		selfEmployedHealthInsuranceDeduction = 
			rhs.selfEmployedHealthInsuranceDeduction;
		keoghAndSelfEmployedSEPPlans = 
			rhs.keoghAndSelfEmployedSEPPlans;
		penaltyOnEarlyWithdrawalOfSavings = 
			rhs.penaltyOnEarlyWithdrawalOfSavings;
		alimoneyPaid = rhs.alimoneyPaid;
	}
	return(*this);
}

Money &
AdjustmentsToIncome::operator=(const Money &rhs)
{
	return(Money::operator=(rhs));
}

Money &
AdjustmentsToIncome::operator=(const Number &rhs)
{
	return(Money::operator=(rhs));
}

// read and write 
ostream &
operator<<(ostream &os, const AdjustmentsToIncome &income)
{
	os << "ADJUSTMENTS TO INCOME" << endl;
	os << "=====================" << endl;

	os << income.yourIraDeduction << endl;
	os << income.spousesIraDeduction << endl;
	os << income.movingExpenses << endl;
	os << income.oneHalfSelfEmploymentTax << endl;
	os << income.selfEmployedHealthInsuranceDeduction << endl;
	os << income.keoghAndSelfEmployedSEPPlans << endl;
	os << income.penaltyOnEarlyWithdrawalOfSavings << endl;
	os << income.alimoneyPaid << endl;

	os << income.name << ": " << income.amount << endl;
	os << "=====================" << endl;

	return(os);
}

istream &
operator>>(istream &is, AdjustmentsToIncome &income)
{
	cout << "ADJUSTMENTS TO INCOME" << endl;
	cout << "=====================" << endl;

	is >> income.yourIraDeduction;
	is >> income.spousesIraDeduction;
	is >> income.movingExpenses;
	is >> income.oneHalfSelfEmploymentTax;
	is >> income.selfEmployedHealthInsuranceDeduction;
	is >> income.keoghAndSelfEmployedSEPPlans;
	is >> income.penaltyOnEarlyWithdrawalOfSavings;
	is >> income.alimoneyPaid;

	income = income.yourIraDeduction + 
		income.spousesIraDeduction +
		income.movingExpenses + 
		income.oneHalfSelfEmploymentTax +
		income.selfEmployedHealthInsuranceDeduction +
		income.keoghAndSelfEmployedSEPPlans +
		income.penaltyOnEarlyWithdrawalOfSavings +
		income.alimoneyPaid;

	cout << income.name << ": " << income.amount << endl;
	cout << "=====================" << endl;

	return(is);
}

// constructors and destructor
TaxComputation::TaxComputation():
	Money("Tax Computation", 0.0),
	status(),
	exemptions(),
	adjustedGrossIncome(),
	sixtyFiveOrOlder(False),
	blind(False),
	spouseSixtyFiveOrOlder(False),
	spouseBlind(False),
	claimedAsDependent(False),
	marriedAndFilingSeparately(False),
	deductionType(Standard),
	itemizedDeductions("Itemized Deductions", 0.0),
	standardDeductions("Standard Deductions", 0.0),
	taxableIncome("Taxable Income", 0.0),
	tax("Tax", 0.0),
	additionalTaxes("Additional Taxes", 0.0)
{
	// do nothing
}

TaxComputation::TaxComputation(const TaxComputation &src):
	Money(src),
	status(src.status),
	exemptions(src.exemptions),
	adjustedGrossIncome(src.adjustedGrossIncome),
	sixtyFiveOrOlder(src.sixtyFiveOrOlder),
	blind(src.blind),
	spouseSixtyFiveOrOlder(src.spouseSixtyFiveOrOlder),
	spouseBlind(src.spouseBlind),
	claimedAsDependent(src.claimedAsDependent),
	marriedAndFilingSeparately(src.marriedAndFilingSeparately),
	deductionType(src.deductionType),
	itemizedDeductions(src.itemizedDeductions),
	standardDeductions(src.standardDeductions),
	taxableIncome(src.taxableIncome),
	tax(src.tax),
	additionalTaxes(src.additionalTaxes)
{
	// do nothing
}

TaxComputation::TaxComputation(const Exemptions &e, const FilingStatus &s, 
			       const Money &agi):
	Money("Tax Computation", 0.0),
	status(s),
	exemptions(e),
	adjustedGrossIncome(agi),
	sixtyFiveOrOlder(False),
	blind(False),
	spouseSixtyFiveOrOlder(False),
	spouseBlind(False),
	claimedAsDependent(False),
	marriedAndFilingSeparately(False),
	deductionType(Standard),
	itemizedDeductions("Itemized Deductions", 0.0),
	standardDeductions("Standard Deductions", 0.0),
	taxableIncome("Taxable Income", 0.0),
	tax("Tax", 0.0),
	additionalTaxes("Additional Taxes", 0.0)
{
	// do nothing
}

TaxComputation::~TaxComputation()
{
	// do nothing
}

// assignment
TaxComputation &
TaxComputation::operator=(const TaxComputation &rhs)
{
	if (this != &rhs)
	{
		Money::operator=(rhs);
		status = rhs.status;
		exemptions = rhs.exemptions;
		adjustedGrossIncome = rhs.adjustedGrossIncome;
		sixtyFiveOrOlder = rhs.sixtyFiveOrOlder;
		blind = rhs.blind;
		spouseSixtyFiveOrOlder = rhs.spouseSixtyFiveOrOlder;
		spouseBlind = rhs.spouseBlind;
		claimedAsDependent = rhs.claimedAsDependent;
		marriedAndFilingSeparately = rhs.marriedAndFilingSeparately;
		deductionType = rhs.deductionType;
		itemizedDeductions = rhs.itemizedDeductions;
		standardDeductions = rhs.standardDeductions;
		taxableIncome = rhs.taxableIncome;
		tax = rhs.tax;
		additionalTaxes = rhs.additionalTaxes;
	}
	return(*this);
}

Money &
TaxComputation::operator=(const Money &rhs)
{
	return(Money::operator=(rhs));
}

Money &
TaxComputation::operator=(const Number &rhs)
{
	return(Money::operator=(rhs));
}

// read and write 
ostream &
operator<<(ostream &os, const TaxComputation &income)
{
	os << "TAX COMPUTATION" << endl;
	os << "===============" << endl;

	os << "Sixty Five Or Older: "
		<< ((income.sixtyFiveOrOlder == True) ? "True" : "False")
		<< endl;

	os << "Blind: "
		<< ((income.blind == True) ? "True" : "False")
		<< endl;

	os << "Spouse Sixty Five Or Older: "
		<< ((income.spouseSixtyFiveOrOlder == True) ? "True" : "False")
		<< endl;

	os << "Spouse Blind: "
		<< ((income.spouseBlind == True) ? "True" : "False")
		<< endl;

	os << "Claimed As Dependent: "
		<< ((income.claimedAsDependent == True) ? "True" : "False")
		<< endl;

	os << "Married And Filing Separately: "
	<< ((income.marriedAndFilingSeparately == True) ? "True" : "False")
		<< endl;

	if (income.deductionType == Standard)
	{
		os << income.standardDeductions << endl;
	}
	else
	{
		os << income.itemizedDeductions << endl;
	}

	os << income.taxableIncome << endl;
	os << income.tax << endl;
	os << income.additionalTaxes << endl;

	os << income.name << ": " << income.amount << endl;
	os << "===============" << endl;

	return(os);
}

istream &
operator>>(istream &is, TaxComputation &income)
{
	char ans;

	cout << "TAX COMPUTATION" << endl;
	cout << "===============" << endl;

	cout << "Sixty Five Or Older (y/n): ";
	is >> ans;
	if (ans == 'y')
		income.sixtyFiveOrOlder = True;
	else
		income.sixtyFiveOrOlder = False;

	cout << "Blind (y/n): ";
	is >> ans;
	if (ans == 'y')
		income.blind = True;
	else
		income.blind = False;

	cout << "Spouse Sixty Five Or Older (y/n): ";
	is >> ans;
	if (ans == 'y')
		income.spouseSixtyFiveOrOlder = True;
	else
		income.spouseSixtyFiveOrOlder = False;

	cout << "Spouse Blind (y/n): ";
	is >> ans;
	if (ans == 'y')
		income.spouseBlind = True;
	else
		income.spouseBlind = False;

	cout << "Claimed As Dependent (y/n): ";
	is >> ans;
	if (ans == 'y')
		income.claimedAsDependent = True;
	else
		income.claimedAsDependent = False;

	cout << "Married And Filing Separately(y/n): ";
	is >> ans;
	if (ans == 'y')
		income.marriedAndFilingSeparately = True;
	else
		income.marriedAndFilingSeparately = False;

	cout << "Standardized or Itemized Deductions (s/i): ";
	is >> ans;
	if (ans == 's')
	{
		income.deductionType = Standard;

		if ((income.sixtyFiveOrOlder == True) ||
		    (income.blind == True) ||
		    (income.spouseSixtyFiveOrOlder == True) ||
		    (income.spouseBlind == True))
		{
		}
		else if (income.claimedAsDependent == True)
		{
		}
		else if (income.marriedAndFilingSeparately == True)
		{
			income.standardDeductions = 0.0;
		}
		else
		{
			Database db;
			Number sd;

			switch (income.status())
			{
			case Single:
				if (db.query( "Single Standard Deduction", 
					"value?", sd) == OK)
				{
					income.standardDeductions = sd;
				}
				else
				{
					ERROR("defaulting standard deduction",
						3900.0);
					income.standardDeductions = 3900.0;
				}
				break;

			case MarriedFilingJointReturn:
				if (db.query(
			"Married Filing Joint Return Standard Deduction", 
					"value?", sd) == OK)
				{
					income.standardDeductions = sd;
				}
				else
				{
					ERROR("defaulting standard deduction",
						6550.0);
					income.standardDeductions = 6550.0;
				}
				break;

			case MarriedFilingSeparateReturn:
				if (db.query(
			"Married Filing Separate Return Standard Deduction", 
					"value?", sd) == OK)
				{
					income.standardDeductions = sd;
				}
				else
				{
					ERROR("defaulting standard deduction",
						3275.0);
					income.standardDeductions = 3275.0;
				}
				break;

			case HeadOfHousehold:
				if (db.query(
			"Head Of Household Standard Deduction", 
					"value?", sd) == OK)
				{
					income.standardDeductions = sd;
				}
				else
				{
					ERROR("defaulting standard deduction",
						5750.0);
					income.standardDeductions = 5750.0;
				}
				break;

			case QualifyingWidowWithDependent:
				if (db.query(
			"Qualifying Widow With Dependent Standard Deduction", 
					"value?", sd) == OK)
				{
					income.standardDeductions = sd;
				}
				else
				{
					ERROR("defaulting standard deduction",
						6550.0);
					income.standardDeductions = 6550.0;
				}
				break;
			}
		}
		income.taxableIncome = 
			income.adjustedGrossIncome - income.standardDeductions;
	}
	else
	{
		income.deductionType = Itemized;

		RealEstate realEstate;

		Database db;
		Boolean yesno;
		if ((db.query("Real Estate", "calculate?", 
				yesno) == OK) && (yesno == True))
		{
			is >> realEstate;
		}

		ScheduleA scheduleA(income.adjustedGrossIncome, 
				income.status, realEstate);
		is >> scheduleA;
 
		income.itemizedDeductions = scheduleA;

		income.taxableIncome = 
			income.adjustedGrossIncome - income.itemizedDeductions;
	}

	// compute exemptions.
#if 0
	if (income.adjustedGrossIncome <= 86025.0)
	{
		income.taxableIncome -= income.exemptions*2500.0;
	}
	else
	{
		ERROR("OOPS !!! Not done doing exemptions !!!", EINVAL);
	}
#else
	ERROR("TEMPORARY !!! Not done doing exemptions !!!", EINVAL);
	income.taxableIncome -= income.exemptions*2500.0;
#endif

	// check if there is any taxable income.
	if (income.taxableIncome <= 0.0)
		income.taxableIncome = 0.0;

	cout << income.taxableIncome << endl;

	Database db;
	Boolean yesno;

	if ((db.query("Federal Taxes", "calculate?", yesno) == OK) &&
	    (yesno == True))
	{
		FederalTaxTable federalTaxes;
		income.tax = federalTaxes(income.status, income.taxableIncome);
		cout << income.tax << endl;
	}
	else
	{
		is >> income.tax;
	}

	is >> income.additionalTaxes;

	income = income.tax + 
		income.additionalTaxes;

	cout << income.name << ": " << income.amount << endl;

	return(is);
}

// constructors and destructor
Credits::Credits():
	Money("Credits", 0.0),
	creditForChildAndDependentCare(
		"Credit For Child And Dependent Care", 0.0),
	creditForElderlyOrDisabled("Credit For Elderly Or Disabled", 0.0),
	foreignTaxCredit("Foreign Tax Credit", 0.0),
	otherCredits("Other Credits", 0.0)
{
	// do nothing
}

Credits::Credits(const Credits &src):
	Money(src),
	creditForChildAndDependentCare(src.creditForChildAndDependentCare), 
	creditForElderlyOrDisabled(src.creditForElderlyOrDisabled), 
	foreignTaxCredit(src.foreignTaxCredit),
	otherCredits(src.otherCredits)
{
	// do nothing
}

Credits::~Credits()
{
	// do nothing
}

// assignment
Credits &
Credits::operator=(const Credits &rhs)
{
	if (this != &rhs)
	{
		Money::operator=(rhs);
		creditForChildAndDependentCare = 
			rhs.creditForChildAndDependentCare;
		creditForElderlyOrDisabled = 
			rhs.creditForElderlyOrDisabled;
		foreignTaxCredit = rhs.foreignTaxCredit;
		otherCredits = rhs.otherCredits;
	}
	return(*this);
}

Money &
Credits::operator=(const Money &rhs)
{
	return(Money::operator=(rhs));
}

Money &
Credits::operator=(const Number &rhs)
{
	return(Money::operator=(rhs));
}

// read and write 
ostream &
operator<<(ostream &os, const Credits &income)
{
	os << "CREDITS" << endl;
	os << "=======" << endl;
	
	os << income.creditForChildAndDependentCare << endl;
	os << income.creditForElderlyOrDisabled << endl;
	os << income.foreignTaxCredit << endl;
	os << income.otherCredits << endl;

	os << income.name << ": " << income.amount << endl;
	os << "=======" << endl;

	return(os);
}

istream &
operator>>(istream &is, Credits &income)
{
	cout << "CREDITS" << endl;
	cout << "=======" << endl;
	
	is >> income.creditForChildAndDependentCare;
	is >> income.creditForElderlyOrDisabled;
	is >> income.foreignTaxCredit;
	is >> income.otherCredits;

	income = income.creditForChildAndDependentCare + 
		income.creditForElderlyOrDisabled +
		income.foreignTaxCredit + 
		income.otherCredits;

	cout << income.name << ": " << income.amount << endl;
	cout << "=======" << endl;

	return(is);
}

// constructors and destructor
OtherTaxes::OtherTaxes():
	Money("Other Taxes", 0.0),
	selfEmploymentTax("Self Employment Tax", 0.0),
	alternativeMinimumTax("Alternative Minimum Tax", 0.0),
	recaptureTaxes("Recapture Taxes", 0.0),
	socialSecurityAndMedicareTaxOnNonreportedIncome(
		"Social Security And Medicare Tax On Nonreported Income", 0.0),
	taxOnQualifiedRetirementPlans(
		"Tax On Qualified Retirement Plans", 0.0),
	advancedEarnedIncomeCreditPayments(
		"Advanced Earned Income Credit Payments", 0.0),
	householdEmploymentTaxes(
		"Household Employment Taxes", 0.0)
{
	// do nothing
}

OtherTaxes::OtherTaxes(const OtherTaxes &src):
	Money(src),
	selfEmploymentTax(src.selfEmploymentTax),
	alternativeMinimumTax(src.alternativeMinimumTax),
	recaptureTaxes(src.recaptureTaxes),
	socialSecurityAndMedicareTaxOnNonreportedIncome(
		src.socialSecurityAndMedicareTaxOnNonreportedIncome),
	taxOnQualifiedRetirementPlans(
		src.taxOnQualifiedRetirementPlans),
	advancedEarnedIncomeCreditPayments(
		src.advancedEarnedIncomeCreditPayments),
	householdEmploymentTaxes(src.householdEmploymentTaxes)
{
	// do nothing
}

OtherTaxes::~OtherTaxes()
{
	// do nothing
}

// assignment
OtherTaxes &
OtherTaxes::operator=(const OtherTaxes &rhs)
{
	if (this != &rhs)
	{
		Money::operator=(rhs);
		selfEmploymentTax = rhs.selfEmploymentTax;
		alternativeMinimumTax = rhs.alternativeMinimumTax;
		recaptureTaxes = rhs.recaptureTaxes;
		socialSecurityAndMedicareTaxOnNonreportedIncome = 
			rhs.socialSecurityAndMedicareTaxOnNonreportedIncome;
		taxOnQualifiedRetirementPlans = 
			rhs.taxOnQualifiedRetirementPlans;
		advancedEarnedIncomeCreditPayments = 
			rhs.advancedEarnedIncomeCreditPayments;
		householdEmploymentTaxes = rhs.householdEmploymentTaxes;
	}
	return(*this);
}

Money &
OtherTaxes::operator=(const Money &rhs)
{
	return(Money::operator=(rhs));
}

Money &
OtherTaxes::operator=(const Number &rhs)
{
	return(Money::operator=(rhs));
}

// read and write 
ostream &
operator<<(ostream &os, const OtherTaxes &income)
{
	os << "OTHER TAXES" << endl;
	os << "===========" << endl;
	
	os << income.selfEmploymentTax << endl;
	os << income.alternativeMinimumTax << endl;
	os << income.recaptureTaxes << endl;
	os << income.socialSecurityAndMedicareTaxOnNonreportedIncome << endl;
	os << income.taxOnQualifiedRetirementPlans << endl;
	os << income.advancedEarnedIncomeCreditPayments << endl;
	os << income.householdEmploymentTaxes << endl;

	os << income.name << ": " << income.amount << endl;
	os << "===========" << endl;

	return(os);
}

istream &
operator>>(istream &is, OtherTaxes &income)
{
	cout << "OTHER TAXES" << endl;
	cout << "===========" << endl;
	
	is >> income.selfEmploymentTax;
	is >> income.alternativeMinimumTax;
	is >> income.recaptureTaxes;
	is >> income.socialSecurityAndMedicareTaxOnNonreportedIncome;
	is >> income.taxOnQualifiedRetirementPlans;
	is >> income.advancedEarnedIncomeCreditPayments;
	is >> income.householdEmploymentTaxes;

	income = income.selfEmploymentTax +
		income.alternativeMinimumTax +
		income.recaptureTaxes +
		income.socialSecurityAndMedicareTaxOnNonreportedIncome +
		income.taxOnQualifiedRetirementPlans +
		income.advancedEarnedIncomeCreditPayments +
		income.householdEmploymentTaxes;

	cout << income.name << ": " << income.amount << endl;
	cout << "===========" << endl;

	return(is);
}

// constructors and destructor
Payments::Payments():
	Money("Payments", 0.0),
	federalIncomeTaxWithheld("Federal Income Tax Withheld", 0.0),
	estimatedTaxPayments("Estimated Tax Payments", 0.0),
	earnedIncomeCredit("Earned Income Credit", 0.0),
	amountPaidFromExtensions(
		"Amount Paid From Extensions", 0.0),
	excessSocialSecurityAndRRTATax(
		"Excess Social Security And RRTA Tax", 0.0),
	otherPayments("Other Payments", 0.0)
{
	// do nothing
}

Payments::Payments(const Payments &src):
	Money(src),
	federalIncomeTaxWithheld(src.federalIncomeTaxWithheld),
	estimatedTaxPayments(src.estimatedTaxPayments),
	earnedIncomeCredit(src.earnedIncomeCredit),
	amountPaidFromExtensions(src.amountPaidFromExtensions),
	excessSocialSecurityAndRRTATax(src.excessSocialSecurityAndRRTATax),
	otherPayments(src.otherPayments)
{
	// do nothing
}

Payments::~Payments()
{
	// do nothing
}

// assignment
Payments &
Payments::operator=(const Payments &rhs)
{
	if (this != &rhs)
	{
		Money::operator=(rhs);
		federalIncomeTaxWithheld = rhs.federalIncomeTaxWithheld;
		estimatedTaxPayments = rhs.estimatedTaxPayments;
		earnedIncomeCredit = rhs.earnedIncomeCredit;
		amountPaidFromExtensions = 
			rhs.amountPaidFromExtensions;
		excessSocialSecurityAndRRTATax = 
			rhs.excessSocialSecurityAndRRTATax;
		otherPayments = rhs.otherPayments;
	}
	return(*this);
}

Money &
Payments::operator=(const Money &rhs)
{
	return(Money::operator=(rhs));
}

Money &
Payments::operator=(const Number &rhs)
{
	return(Money::operator=(rhs));
}

// read and write 
ostream &
operator<<(ostream &os, const Payments &income)
{
	os << "PAYMENTS" << endl;
	os << "========" << endl;

	os << income.federalIncomeTaxWithheld << endl;
	os << income.estimatedTaxPayments << endl;
	os << income.earnedIncomeCredit << endl;
	os << income.amountPaidFromExtensions << endl;
	os << income.excessSocialSecurityAndRRTATax << endl;
	os << income.otherPayments << endl;

	os << income.name << ": " << income.amount << endl;
	os << "========" << endl;

	return(os);
}

istream &
operator>>(istream &is, Payments &income)
{
	cout << "PAYMENTS" << endl;
	cout << "========" << endl;

	is >> income.federalIncomeTaxWithheld;
	is >> income.estimatedTaxPayments;
	is >> income.earnedIncomeCredit;
	is >> income.amountPaidFromExtensions;
	is >> income.excessSocialSecurityAndRRTATax;
	is >> income.otherPayments;

	income = income.federalIncomeTaxWithheld +
		income.estimatedTaxPayments +
		income.earnedIncomeCredit +
		income.amountPaidFromExtensions +
		income.excessSocialSecurityAndRRTATax +
		income.otherPayments;

	cout << income.name << ": " << income.amount << endl;
	cout << "========" << endl;

	return(is);
}

// constructors and destructor
RefundOrAmountOwed::RefundOrAmountOwed():
	Money("Refund Or Amount Owed", 0.0),
	totalTaxes("Total Taxes", 0.0),
	payments(),
	refund("Refund", 0.0),
	owed("Owed", 0.0),
	penalty("Penalty", 0.0)
{
	// do nothing
}

RefundOrAmountOwed::RefundOrAmountOwed(const RefundOrAmountOwed &src):
	Money(src),
	totalTaxes(src.totalTaxes),
	payments(src.payments),
	refund(src.refund),
	owed(src.owed),
	penalty(src.penalty)
{
	// do nothing
}

RefundOrAmountOwed::RefundOrAmountOwed(
		const Money &tt, const Payments &p):
	Money("Refund Or Amount Owed", 0.0),
	totalTaxes(tt),
	payments(p),
	refund("Refund", 0.0),
	owed("Owed", 0.0),
	penalty("Penalty", 0.0)
{
	if (payments > totalTaxes)
	{
		refund = payments - totalTaxes;
		*this = refund;
	}
	else if (payments < totalTaxes)
	{
		owed = totalTaxes - payments;
		*this = -1.0*owed;
	}
}

RefundOrAmountOwed::~RefundOrAmountOwed()
{
	// do nothing
}

// assignment
RefundOrAmountOwed &
RefundOrAmountOwed::operator=(const RefundOrAmountOwed &rhs)
{
	if (this != &rhs)
	{
		Money::operator=(rhs);
		refund = rhs.refund;
		owed = rhs.owed;
		penalty = rhs.penalty;
	}
	return(*this);
}

Money &
RefundOrAmountOwed::operator=(const Money &rhs)
{
	return(Money::operator=(rhs));
}

Money &
RefundOrAmountOwed::operator=(const Number &rhs)
{
	return(Money::operator=(rhs));
}

// read and write 
ostream &
operator<<(ostream &os, const RefundOrAmountOwed &income)
{
	os << "REFUND OR AMOUNT OWED" << endl;
	os << "=====================" << endl;

	os << income.refund << endl;
	os << income.owed << endl;
	os << income.penalty << endl;

	os << income.name << ": " << income.amount << endl;
	os << "=====================" << endl;

	return(os);
}

istream &
operator>>(istream &is, RefundOrAmountOwed &income)
{
	cout << "REFUND OR AMOUNT OWED" << endl;
	cout << "=====================" << endl;

	is >> income.totalTaxes;
	is >> income.payments;

	income.refund = 0.0;
	income.owed = 0.0;
	income.penalty = 0.0;

	if (income.payments > income.totalTaxes)
	{
		income.refund = income.payments - income.totalTaxes;
		income = income.refund;
	}
	else if (income.payments < income.totalTaxes)
	{
		income.owed = income.totalTaxes - income.payments;
		income = -1.0*income.owed;
	}

	cout << income.name << ": " << income.amount << endl;
	cout << "=====================" << endl;

	return(is);
}

