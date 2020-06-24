#ifndef __HASHTABLE_H
#define __HASHTABLE_H
// hash table class definitions

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "list.h"

// forward declarations
template <class DataType> class HashTable;

// definitions
#define DefaultHashTableSize 199

// hash table class
template <class DataType> class HashTable {
public:
	// constructors and destructor
	HashTable(int, int (*)(DataType &));
	HashTable(const HashTable &);
	~HashTable();

	// assignment
	HashTable &operator=(const HashTable &);

	// hash table operations
	int insert(DataType &);
	int retrieve(DataType &) const;
	int isInTable(DataType &) const;
	int clear();

	// print table
	friend ostream &operator<<(ostream &, const HashTable<DataType> &);

protected:
	// internal data
	int tableSize;
	int (*hashFn)(DataType &);
	List<DataType> *table;
};

#endif
