//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __OMBT_ABSTRACT_HASH_TABLE_H
#define __OMBT_ABSTRACT_HASH_TABLE_H
// abstract hash table class definitions

// headers
#include <stdlib.h>
#include <iostream>

// local headers
#include <Returns.h>
#include <Debug.h>

namespace ombt {

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

#if USEOSTREAM
	// print table
	virtual std::ostream &dump(std::ostream &) const = 0;
	friend std::ostream &operator<<(std::ostream &os, 
		const AbstractHashTable<DataType> &ht) {
		ht.dump(os);
		return(os);
	}
#endif
};

}

#endif
