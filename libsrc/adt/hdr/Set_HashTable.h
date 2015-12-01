//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __SET_HASHTABLE_H
#define __SET_HASHTABLE_H
// hash table-based set class definition

// headers
#include "adt/HashTable_List.h"

namespace ombt {

// forward declarations
template <class DataType> class Set_HashTable;
template <class DataType> class Set_HashTable_Iterator;

#if USEOSTREAM
// forward declarations
template <class DataType>
std::ostream &
operator<<(std::ostream &, const Set_HashTable<DataType> &);
#endif

// hash table-based set class
template <class DataType> class Set_HashTable {
public:
	// friends
	friend class Set_HashTable_Iterator<DataType>;

        // destructor
        Set_HashTable(int, int (*)(const DataType &));
        Set_HashTable(const Set_HashTable &);
        ~Set_HashTable();

        // insert member operations
        Set_HashTable &operator+=(const DataType &);
        Set_HashTable operator+(const DataType &) const;
        Set_HashTable &insert(const DataType &);

        // delete member operations
        Set_HashTable &operator-=(DataType &);
        Set_HashTable operator-(DataType &) const;
        Set_HashTable &remove(DataType &);
        Set_HashTable &clear();

        // equality and assignment set operations
        Set_HashTable &operator=(const Set_HashTable &);
        int operator==(const Set_HashTable &) const;
        int isMember(const DataType &) const;
        int isEmpty() const;

        // union set operations
        Set_HashTable &operator|=(const Set_HashTable &);
        Set_HashTable operator|(const Set_HashTable &) const;

        // intersection set operations
        Set_HashTable &operator&=(const Set_HashTable &);
        Set_HashTable operator&(const Set_HashTable &) const;

        // difference set operations
        Set_HashTable &operator-=(const Set_HashTable &);
        Set_HashTable operator-(const Set_HashTable &) const;

	// access data functions
	int getSize() const;
	int (*getHash() const)(const DataType &);

#if USEOSTREAM
	// output data
	template <typename DT> friend std::ostream &operator<<(std::ostream &, const Set_HashTable<DT> &);
#endif

protected:
	// data
	HashTable_List<DataType> table;
};

// hash table-based set iterator class
template <class DataType> class Set_HashTable_Iterator
{
public:
        // constructors and destructor
        Set_HashTable_Iterator(const Set_HashTable<DataType> &);
        Set_HashTable_Iterator(const Set_HashTable_Iterator<DataType> &);
        ~Set_HashTable_Iterator();

	// reset iterator to start
	void reset();

	// check if at end of set
	int done() const;

	// return current set member
        DataType operator()();

	// advance to next set member
	int operator++(int);

private:
	// not allowed
        Set_HashTable_Iterator &operator=(const Set_HashTable_Iterator<DataType> &);

protected:
	// internal hash table iterator
	HashTable_List_Iterator<DataType> iterator;
};

#include "adt/Set_HashTable.i"

}


#endif
