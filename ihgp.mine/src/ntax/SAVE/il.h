#ifndef __ILLINOIS_H
#define __ILLINOIS_H

// illinois tax class definition

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
class IllinoisTaxTable;

// class definition
class IllinoisTaxTable: public TaxTable {
public:
	// constructor and destructor
	IllinoisTaxTable();
	IllinoisTaxTable(const IllinoisTaxTable &);
	~IllinoisTaxTable();
};

#endif
