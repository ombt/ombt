#ifndef __ABSTRACT_HASH_TABLE_H
#define __ABSTRACT_HASH_TABLE_H
// abstract hash table class definitions

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"

// hash table class
template <class DataType> class AbstractHashTable {
public:
	// constructors and destructor
	virtual ~AbstractHashTable() { }

	// hash table operations
	virtual void clear() = 0;
	virtual int insert(const DataType &) = 0;
	virtual int remove(DataType &) = 0;
	virtual int retrieve(DataType &) const = 0;
	virtual int includes(const DataType &) const = 0;
	virtual int isEmpty() const = 0;

	// print table
	virtual ostream &dump(ostream &) const = 0;
	friend ostream &operator<<(ostream &os, 
		const AbstractHashTable<DataType> &ht) {
		ht.dump(os);
		return(os);
	}
};

#endif
