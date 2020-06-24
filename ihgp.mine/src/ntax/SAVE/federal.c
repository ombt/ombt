// federal tax class functions

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>
#include <math.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "federal.h"

// federal tax rate table
static struct TaxTableRep singleRates[] = {
	TaxTableRep(0.0, 24650.0, 0.15),
	TaxTableRep(24650.0, 59750.0, 0.28),
	TaxTableRep(59750.0, 124650.0, 0.31),
	TaxTableRep(124650.0, 271050.0, 0.36),
	TaxTableRep(271050.0, MaxFloat, 0.396)
};

static struct TaxTableRep marriedFilingJointReturnRates[] = {
	TaxTableRep(0.0, 41200.0, 0.15),
	TaxTableRep(41200.0, 99600.0, 0.28),
	TaxTableRep(99600.0, 151750.0, 0.31),
	TaxTableRep(151750.0, 271050.0, 0.36),
	TaxTableRep(271050.0, MaxFloat, 0.396)
};

static struct TaxTableRep marriedFilingSeparateReturnRates[] = {
	TaxTableRep(0.0, 20600.0, 0.15),
	TaxTableRep(20600.0, 49800.0, 0.28),
	TaxTableRep(49800.0, 75875.0, 0.31),
	TaxTableRep(75875.0, 135525.0, 0.36),
	TaxTableRep(135525.0, MaxFloat, 0.396)
};

static struct TaxTableRep headOfHouseholdRates[] = {
	TaxTableRep(0.0, 33050.0, 0.15),
	TaxTableRep(33050.0, 85350.0, 0.28),
	TaxTableRep(85350.0, 138200.0, 0.31),
	TaxTableRep(138200.0, 271050.0, 0.36),
	TaxTableRep(271050.0, MaxFloat, 0.396)
};

static struct TaxTableRep qualifyingWidowWithDependentRates[] = {
	TaxTableRep(0.0, 41200.0, 0.15),
	TaxTableRep(41200.0, 99600.0, 0.28),
	TaxTableRep(99600.0, 151750.0, 0.31),
	TaxTableRep(151750.0, 271050.0, 0.36),
	TaxTableRep(271050.0, MaxFloat, 0.396)
};

// constructors and destructor
FederalTaxTable::FederalTaxTable():
	TaxTable(singleRates,
		marriedFilingJointReturnRates,
		marriedFilingSeparateReturnRates,
		headOfHouseholdRates,
		qualifyingWidowWithDependentRates)
{
	// do nothing
}

FederalTaxTable::FederalTaxTable(const FederalTaxTable &src):
	TaxTable(src)
{
	// do nothing
}

FederalTaxTable::~FederalTaxTable()
{
	// do nothing
}
