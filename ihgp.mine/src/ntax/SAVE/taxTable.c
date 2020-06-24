// federal tax class functions

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>
#include <math.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "taxTable.h"

// constructors and destructor
TaxTable::TaxTable():
	singleRateTable(0),
	marriedFilingJointReturnRateTable(0),
	marriedFilingSeparateReturnRateTable(0),
	headOfHouseholdRateTable(0),
	qualifyingWidowWithDependentRateTable(0)
{
	// do nothing
}

TaxTable::TaxTable(const TaxTable &src):
	singleRateTable(
		src.singleRateTable),
	marriedFilingJointReturnRateTable(
		src.marriedFilingJointReturnRateTable),
	marriedFilingSeparateReturnRateTable(
		src.marriedFilingSeparateReturnRateTable),
	headOfHouseholdRateTable(
		src.headOfHouseholdRateTable),
	qualifyingWidowWithDependentRateTable(
		src.qualifyingWidowWithDependentRateTable)
{
	// do nothing
}

TaxTable::TaxTable(const TaxTableRep *srt, const TaxTableRep *mfjrrt, 
		const TaxTableRep *mfsrrt, const TaxTableRep *hohrt, 
		const TaxTableRep *qwwdrt):
	singleRateTable(srt),
	marriedFilingJointReturnRateTable(mfjrrt),
	marriedFilingSeparateReturnRateTable(mfsrrt),
	headOfHouseholdRateTable(hohrt), 
	qualifyingWidowWithDependentRateTable(qwwdrt)
{
	// do nothing
}

TaxTable::~TaxTable()
{
	// do nothing
}

// calculate taxes for taxable income
Money
TaxTable::operator()(FilingStatus &fs, const Money &ti) const
{
	// choose tax table
	const TaxTableRep *table;
	switch (fs())
	{
	case Single:
		table = singleRateTable;
		break;
	case MarriedFilingJointReturn:
		table = marriedFilingJointReturnRateTable;
		break;
	case MarriedFilingSeparateReturn:
		table = marriedFilingSeparateReturnRateTable;
		break;
	case HeadOfHousehold:
		table = headOfHouseholdRateTable;
		break;
	case QualifyingWidowWithDependent:
		table = qualifyingWidowWithDependentRateTable;
		break;
	}

	// check if any table is assigned
	if (table == 0)
		return(Money(0.0));

	// calculate tax
	Money tax;
	tax = 0.0;
	int irate = -1;
	do {
		irate += 1;
		if ((table[irate].end - ti) < 0.0)
		{
			tax += table[irate].rate*
				(table[irate].end - table[irate].start);
		}
		else
		{
			tax += table[irate].rate*
				(ti - table[irate].start);
		}
	} while ( ! (table[irate].start < ti && ti <= table[irate].end));

	// all done
	return(tax);
}
