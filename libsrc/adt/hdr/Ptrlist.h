//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __PTRLIST_H
#define __PTRLIST_H
// pointer list class definition. this list implementation works
// with pointers, that is, it stores pointers to actual objects,
// it does not make copies. also, it dereferences the pointers
// when comparing objects. this means that you better not pass
// in invalid or NULL pointers. very important. when clearing a 
// pointer list, the data pointers are NOT deleted. also, the destructors
// will NOT delete all data pointers. a member function clearAndDelete()
// is provided for clearing a list AND for deleting all data pointers
// contained in the list.
//
// since this list stores pointers, it is possible for multiple list 
// to contain the a pointer, this can lead to problems if the pointer 
// is deleted in one list and used in the another list. the used 
// must be careful about these cases. a possible solution is to 
// use reference-counted pointers.
//

// headers
#include <stdlib.h>
#include <iostream>

// local headers
#include "system/Returns.h"
#include "system/Debug.h"
#include "adt/AbsIterator.h"

// forward declarations
template <class DataType> class PtrListItem;
template <class DataType> class PtrList;
template <class DataType> class PtrListIterator;
template <class DataType> class PtrListIterator_Reverse;

#if USEOSTREAM
// forward declarations of friend functions 
template <class DataType> 
ostream &
operator<<(ostream &, const PtrListItem<DataType> &);

template <class DataType> 
ostream &
operator<<(ostream &, const PtrList<DataType> &);
#endif

// list item class
template <class DataType> 
class PtrListItem {
public:
        // friend classes and functions
        friend class PtrList<DataType>;
        friend class PtrListIterator<DataType>;
        friend class PtrListIterator_Reverse<DataType>;

        // constructors and destructor
        PtrListItem(DataType *);
        PtrListItem(const PtrListItem &);
        ~PtrListItem();

        // assignment 
        PtrListItem &operator=(const PtrListItem &);

#if USEOSTREAM
        // miscellaneous
        friend ostream &operator<<(ostream &, const PtrListItem<DataType> &);
        friend ostream &operator<<(ostream &, const PtrList<DataType> &);
#endif

protected:
        // internal data
        DataType *pdata;
        PtrListItem<DataType> *next;
        PtrListItem<DataType> *previous;
};

// list class
template <class DataType> 
class PtrList {
public:
        // friend classes
        friend class PtrListIterator<DataType>;
        friend class PtrListIterator_Reverse<DataType>;

        // constructors and destructor
        PtrList();
        PtrList(const PtrList &);
        ~PtrList();

        // assignment
        PtrList &operator=(const PtrList &);

        // unordered list operations with duplicates
        int retrieve(DataType *&) const;
        int insert(DataType *);
        int update(DataType *);
        int remove(DataType *&);

        // unordered list operations without duplicates
        int retrieveUnique(DataType *&) const;
        int insertUnique(DataType *);
        int updateUnique(DataType *);
        int removeUnique(DataType *&);

        // list operations for the front 
        int retrieveAtFront(DataType *&) const;
        int insertAtFront(DataType *);
        int updateAtFront(DataType *);
        int removeAtFront(DataType *&);

        // list operations for the end 
        int retrieveAtEnd(DataType *&) const;
        int insertAtEnd(DataType *);
        int updateAtEnd(DataType *);
        int removeAtEnd(DataType *&);

        // ordered list operations with duplicates
        int retrieveOrdered(DataType *&) const;
        int insertOrdered(DataType *);
        int updateOrdered(DataType *);
        int removeOrdered(DataType *&);

        // ordered list operations without duplicates
        int retrieveOrderedUnique(DataType *&) const;
        int insertOrderedUnique(DataType *);
        int updateOrderedUnique(DataType *);
        int removeOrderedUnique(DataType *&);

	// list operations using the item position
        int retrieveNth(int, DataType *&) const;
        int insertNth(int, DataType *);
        int updateNth(int, DataType *);
        int removeNth(int, DataType *&);
	int getCount() const {
		return(count);
	}

	// test functions
        int includes(DataType *) const;
	int isEmpty() const;

	// clean up 
        void clear();
        void clearAndDelete();

#if USEOSTREAM
        // I/O functions
        friend ostream &operator<<(ostream &, const PtrList<DataType> &);
#endif

protected:
        // internal data
	int count;
        PtrListItem<DataType> *first;
        PtrListItem<DataType> *last;
};

// list iterator
template <class DataType> 
class PtrListIterator {
public:
        // constructors and destructor
        PtrListIterator(const PtrListIterator &);
        PtrListIterator(const PtrList<DataType> &);
        ~PtrListIterator();

        // initialization
	void reset();

	// check if at end of list
	int done() const;

        // return data 
        DataType *operator()();

	// advance iterator to next link
	int operator++(int);

private:
	// not allowed
        PtrListIterator();
	PtrListIterator &operator=(const PtrListIterator<DataType> &);

protected:
        // internal data
	const PtrList<DataType> &list;
        PtrListItem<DataType> *current;
};

// list reverse iterator
template <class DataType> class PtrListIterator_Reverse:
	public AbstractIterator<DataType>
{
public:
        // constructors and destructor
        PtrListIterator_Reverse(const PtrListIterator_Reverse &);
        PtrListIterator_Reverse(const PtrList<DataType> &);
        ~PtrListIterator_Reverse();

        // initialization
	void reset();

	// check if at end of list
	int done() const;

        // return data 
        DataType *operator()();

	// advance iterator to next link
	int operator++(int);

private:
	// not allowed
        PtrListIterator_Reverse();
	PtrListIterator_Reverse &operator=(
		const PtrListIterator_Reverse<DataType> &);

protected:
        // internal data
	const PtrList<DataType> &list;
        PtrListItem<DataType> *current;
};

#endif
