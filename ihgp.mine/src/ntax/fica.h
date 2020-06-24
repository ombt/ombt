#ifndef __FICA_H
#define __FICA_H

// FICA tax class definition

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
class FICATaxTable;

// class definition
class FICATaxTable: public TaxTable {
public:
	// constructor and destructor
	FICATaxTable();
	FICATaxTable(const FICATaxTable &);
	~FICATaxTable();
};

#endif
