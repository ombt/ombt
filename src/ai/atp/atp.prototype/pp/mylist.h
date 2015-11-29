#ifndef __LIST_H
#define __LIST_H
// list class definition

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "absIterator.h"

// forward declarations
template <class DataType> class ListItem;
template <class DataType> class List;
template <class DataType> class ListIterator;
template <class DataType> class ListIterator_Reverse;

// forward declarations of friend functions 
template <class DataType> 
ostream &
operator<<(ostream &, const ListItem<DataType> &);

template <class DataType> 
ostream &
operator<<(ostream &, const List<DataType> &);

// list item class
template <class DataType> class ListItem {
public:
        // friend classes and functions
        friend class List<DataType>;
        friend class ListIterator<DataType>;
        friend class ListIterator_Reverse<DataType>;

        // constructors and destructor
        ListItem(const DataType &);
        ListItem(const ListItem &);
        ~ListItem();

        // assignment 
        ListItem &operator=(const ListItem &);

        // miscellaneous
        friend ostream &operator<<(ostream &, const ListItem<DataType> &);
        friend ostream &operator<<(ostream &, const List<DataType> &);

protected:
        // internal data
        DataType data;
        ListItem<DataType> *next;
        ListItem<DataType> *previous;
};

// list class
template <class DataType> class List {
public:
        // friend classes
        friend class ListIterator<DataType>;
        friend class ListIterator_Reverse<DataType>;

        // constructors and destructor
        List();
        List(const List &);
        ~List();

        // assignment
        List &operator=(const List &);

        // unordered list operations with duplicates
        int retrieve(DataType &) const;
        int insert(const DataType &);
        int update(const DataType &);
        int remove(DataType &);

        // unordered list operations without duplicates
        int retrieveUnique(DataType &) const;
        int insertUnique(const DataType &);
        int updateUnique(const DataType &);
        int removeUnique(DataType &);

        // list operations for the front 
        int retrieveAtFront(DataType &) const;
        int insertAtFront(const DataType &);
        int updateAtFront(const DataType &);
        int removeAtFront(DataType &);

        // list operations for the end 
        int retrieveAtEnd(DataType &) const;
        int insertAtEnd(const DataType &);
        int updateAtEnd(const DataType &);
        int removeAtEnd(DataType &);

        // ordered list operations with duplicates
        int retrieveOrdered(DataType &) const;
        int insertOrdered(const DataType &);
        int updateOrdered(const DataType &);
        int removeOrdered(DataType &);

        // ordered list operations without duplicates
        int retrieveOrderedUnique(DataType &) const;
        int insertOrderedUnique(const DataType &);
        int updateOrderedUnique(const DataType &);
        int removeOrderedUnique(DataType &);

	// list operations using the item position
        int retrieveNth(int, DataType &) const;
        int insertNth(int, const DataType &);
        int updateNth(int, const DataType &);
        int removeNth(int, DataType &);
	int getCount() const {
		return(count);
	}

	// test functions
        int includes(const DataType &) const;
	int isEmpty() const;

	// clean up 
        void clear();

        // I/O functions
        friend ostream &operator<<(ostream &, const List<DataType> &);

protected:
        // internal data
	int count;
        ListItem<DataType> *first;
        ListItem<DataType> *last;
};

// list iterator
template <class DataType> class ListIterator:
	public AbstractIterator<DataType>
{
public:
        // constructors and destructor
        ListIterator(const ListIterator &);
        ListIterator(const List<DataType> &);
        ~ListIterator();

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
        ListIterator();
	ListIterator &operator=(const ListIterator<DataType> &);

protected:
        // internal data
	const List<DataType> &list;
        ListItem<DataType> *current;
};

// list reverse iterator
template <class DataType> class ListIterator_Reverse:
	public AbstractIterator<DataType>
{
public:
        // constructors and destructor
        ListIterator_Reverse(const ListIterator_Reverse &);
        ListIterator_Reverse(const List<DataType> &);
        ~ListIterator_Reverse();

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
        ListIterator_Reverse();
	ListIterator_Reverse &operator=(
		const ListIterator_Reverse<DataType> &);

protected:
        // internal data
	const List<DataType> &list;
        ListItem<DataType> *current;
};

#endif
