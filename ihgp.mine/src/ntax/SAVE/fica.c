// federal tax class functions

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>
#include <math.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "fica.h"

// federal tax rate table
static struct TaxTableRep FICARates[] = {
	TaxTableRep(0.1, 60600.0, 0.0765),
	TaxTableRep(60600.0, MaxFloat, 0.0145)
};

// constructors and destructor
FICATaxTable::FICATaxTable():
	TaxTable()
{
        singleRateTable = FICARates;
        marriedFilingJointReturnRateTable = FICARates;
        marriedFilingSeparateReturnRateTable = FICARates;
        headOfHouseholdRateTable = FICARates;
        qualifyingWidowWithDependentRateTable = FICARates;
}

FICATaxTable::FICATaxTable(const FICATaxTable &src):
	TaxTable(src)
{
	// do nothing
}

FICATaxTable::~FICATaxTable()
{
	// do nothing
}
