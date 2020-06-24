#ifndef __FEDERAL_H
#define __FEDERAL_H

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
#include "taxTable.h"

// forward declaration
class FederalTaxTable;

// income class definition
class FederalTaxTable: public TaxTable {
public:
	// constructor and destructor
	FederalTaxTable();
	FederalTaxTable(const FederalTaxTable &);
	~FederalTaxTable();
};

#endif
