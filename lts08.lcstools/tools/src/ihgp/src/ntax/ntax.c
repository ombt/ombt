// federal tax program

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>
#include <math.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "boolean.h"
#include "number.h"
#include "money.h"
#include "1040.h"
#include "db.h"

// main entry point
main(int argc, char **argv)
{
	// start program
	cout << "Federal Tax Program" << endl;

	// initialize database
	char *dbFile = "ntax.db";
	if (argc > 1) 
		dbFile = argv[1];
	Database db(dbFile);

	// turn on screening of fields
	Money::doScreening();

	// filing status
	FilingStatus filingStatus;
	cin >> filingStatus;
	
	// exemptions
	Exemptions exemptions;
	cin >> exemptions;
	
	// income
	Income income;
	cin >> income;
	
	// adjustments to income
	AdjustmentsToIncome adjustmentsToIncome;
	cin >> adjustmentsToIncome;

	// adjusted gross income
	Money adjustedGrossIncome("Adjusted Gross Income", 0.0);
	adjustedGrossIncome = income - adjustmentsToIncome;
	cout << adjustedGrossIncome << endl;

	// tax computation
	TaxComputation taxComputation(exemptions, 
			filingStatus, adjustedGrossIncome);
	cin >> taxComputation;

	// credits
	Credits credits;
	cin >> credits;

	// other taxes
	OtherTaxes otherTaxes;
	cin >> otherTaxes;

	// total taxes
	Money totalTaxes("Total Taxes", 0.0);
	totalTaxes = taxComputation + otherTaxes - credits;
	cout << totalTaxes << endl;

	// payments
	Payments payments;
	cin >> payments;

	// refund or amount owed
	RefundOrAmountOwed refundOrAmountOwed(totalTaxes, payments);
	cout << refundOrAmountOwed << endl;

	// all done
	return(0);
}
