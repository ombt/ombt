// functions for hash table class

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>

// local headers
#include "hash.h"

// hash table constructors and destructor
template <class DataType>
HashTable<DataType>::HashTable(int tsz,  int (*hfn)(DataType &)):
	tableSize(tsz), 
	hashFn(hfn), 
	table(new List<DataType> [tsz])
{
	// check table
	assert(tsz > 0 && table != 0);
}

template <class DataType>
HashTable<DataType>::HashTable(const HashTable<DataType> &ht):
	tableSize(ht.tableSize), 
	hashFn(ht.hashFn), 
	table(new List<DataType> [ht.tableSize])
{
	// check table
	assert(table != 0);

	// initialize table entries 
	for (int ib = 0; ib < tableSize; ib++)
	{
		table[ib] = ht.table[ib];
	}
}

template <class DataType>
HashTable<DataType>::~HashTable()
{
	// delete hash table
	delete [tableSize] table;
	table = 0;
	tableSize = 0;
	hashFn = 0;
}

// assignment
template <class DataType>
HashTable<DataType> &
HashTable<DataType>::operator=(const HashTable<DataType> &ht)
{
	// check for self-assignment
	if (this != &ht)
	{
		// delete old table
		delete [tableSize] table;

		// add new table
		tableSize = ht.tableSize;
		hashFn = ht.hashFn;
		table = new List<DataType> [ht.tableSize];
		assert(table != 0);

		// initialize table entries 
		for (int ib = 0; ib < tableSize; ib++)
		{
			table[ib] = ht.table[ib];
		}
	}

	// all done
	return(*this);
}


// hash table operations
template <class DataType>
int
HashTable<DataType>::insert(DataType &d)
{
	// get bucket for data item
	int ib = hashFn(d)%tableSize;

	// insert into bucket
	return(table[ib].insertAtEnd(d));
}

template <class DataType>
int
HashTable<DataType>::retrieve(DataType &d) const
{
	// get bucket for data item
	int ib = hashFn(d)%tableSize;

	// insert into bucket
	return(table[ib].retrieveByValue(d));
}

template <class DataType>
int
HashTable<DataType>::isInTable(DataType &d) const
{
	// get bucket for data item
	int ib = hashFn(d)%tableSize;

	// insert into bucket
	return(table[ib].isInList(d));
}

template <class DataType>
int
HashTable<DataType>::clear()
{
	// initialize table entries 
	for (int ib = 0; ib < tableSize; ib++)
	{
		table[ib].clear();
	}
	return(OK);
}

// print table
template <class DataType>
ostream &
operator<<(ostream &os, const HashTable<DataType> &ht)
{
	os << "hash table = {" << endl;
	for (int ib = 0; ib < ht.tableSize; ib++)
	{
	        os << "bucket[" << ib << "] = " << ht.table[ib] << endl;
	}
	os << "}" << endl;
	return(os);
}
