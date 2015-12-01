//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __HASHTABLE_LIST_H
#define __HASHTABLE_LIST_H
// hash table class definition

// headers
#include "adt/AbsHashTable.h"
#include "adt/AbsIterator.h"
#include "adt/List.h"
#include "adt/Array.h"

namespace ombt {

// forward declarations
template <class DataType> class HashTable_List;
template <class DataType> class HashTable_List_Iterator;

// abstract hash table class
template <class DataType> class HashTable_List:
	public AbstractHashTable<DataType> {
public:
        // friend classes
        friend class HashTable_List_Iterator<DataType>;

        // constructors and destructor
	HashTable_List(int, int (*)(const DataType &));
        HashTable_List(const HashTable_List &);
        ~HashTable_List();

	// assignment 
	HashTable_List &operator=(const HashTable_List &);

	// hash table operations
	void clear();
	int insert(const DataType &);
	int remove(DataType &);
	int retrieve(DataType &) const;
	int includes(const DataType &) const;
	int isEmpty() const;

	// additional hash table operations for sets, no
	// duplicates are allowed.
	//
	int set_insert(const DataType &);
	int set_remove(DataType &);
	int set_retrieve(DataType &) const;

	// access data functions
	int getSize() const;
	int (*getHash() const)(const DataType &);

	// output data
	std::ostream &dump(std::ostream &) const;

protected:
	// data
	int size;
	int (*hash)(const DataType &);
	Array<List<DataType> > array;
};

// hash table iterator
template <class DataType> class HashTable_List_Iterator:
	public AbstractIterator<DataType>
{
public:
        // constructors and destructor
        HashTable_List_Iterator(const HashTable_List_Iterator &);
        HashTable_List_Iterator(const HashTable_List<DataType> &);
        ~HashTable_List_Iterator();

        // initialization
	void reset();

	// check if at end of list
	int done() const;

        // return data 
        DataType operator()();

	// advance iterator to next link
	int operator++(int);

private:
	// not allowed
        HashTable_List_Iterator();
	HashTable_List_Iterator &operator=(
		const HashTable_List_Iterator<DataType> &);

protected:
        // internal data
	const HashTable_List<DataType> &table;
	ListIterator<DataType> *listIter;
	int current;
};

#include "adt/HashTable_List.i"
}
#endif
