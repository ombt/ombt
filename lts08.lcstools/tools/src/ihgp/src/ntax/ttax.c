#include <stdlib.h>
#include <iostream.h>
#include "federal.h"
#include "il.h"
#include "fica.h"
#include "realEstate.h"

main(int , char **)
{
	FilingStatus fs;
	cin >> fs;

	Money income("Income", 0.0);
	cin >> income;

	RealEstate re;
	Number cy("Current Year For Real Estate Interest Calculation", 0.0);
	cin >> re;
	cin >> cy;

	IllinoisTaxTable il;
	Money statetaxes("State Taxes", 0.0);
	statetaxes = il(fs, income);

	Money rep("Real Estate Deduction Points", 0.0);
	Money rei("Real Estate Deduction Interest", 0.0);
	Money ret("Real Estate Deduction Taxes", 0.0);

	rep = re.pointsPaid();
	rei = re.interestPaid(cy);
	ret = re.realEstateTaxesPaid();

	cout << rep << endl;
	cout << rei << endl;
	cout << ret << endl;

	Money adjustedIncome("Adjusted Income", 0.0);
	adjustedIncome = income - rep - rei - ret - statetaxes;
	cout << adjustedIncome << endl;

	FederalTaxTable fed;
	Money fedtaxes("Federal Taxes", 0.0);
	fedtaxes = fed(fs, adjustedIncome);
	cout << fedtaxes << endl;

	cout << statetaxes << endl;

	FICATaxTable fica;
	Money ficataxes("Fica Taxes", 0.0);
	ficataxes = fica(fs, income);
	cout << ficataxes << endl;

	Money total("Total Taxes", 0.0);
	total = fedtaxes + statetaxes + ficataxes;
	cout << total << endl;

	Money permonth("Income Per Month After Taxes", 0.0);
	permonth = (income - total)/12.0;
	cout << permonth << endl;

	Money mortgage("Monthly Mortgage payment", 0.0);
	mortgage = re.monthlyMortgagePayment();
	cout << mortgage << endl;

	Money realEstateTaxes("Monthly Real Estate Tax payment", 0.0);
	realEstateTaxes = re.monthlyRealEstateTaxPayment();
	cout << realEstateTaxes << endl;

	Money permonthafteretc("Income/Month After Taxes, Real Estate, etc.", 
				0.0);
	permonthafteretc = permonth - mortgage - realEstateTaxes;
	cout << permonthafteretc << endl;

	return(0);
}
