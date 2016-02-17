// illinois tax class functions

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>
#include <math.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "il.h"

// federal tax rate table
static struct TaxTableRep IllinoisRates[] = {
	TaxTableRep(0.0, MaxFloat, 0.03)
};

// constructors and destructor
IllinoisTaxTable::IllinoisTaxTable():
	TaxTable(IllinoisRates, IllinoisRates, IllinoisRates,
        	IllinoisRates, IllinoisRates)
{
	// do nothing
}

IllinoisTaxTable::IllinoisTaxTable(const IllinoisTaxTable &src):
	TaxTable(src)
{
	// do nothing
}

IllinoisTaxTable::~IllinoisTaxTable()
{
	// do nothing
}
