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

        // list operations
        void insertAtFront(const DataType &);
        void insertAtEnd(const DataType &);
        void insertByValue(const DataType &);
        int removeAtFront(DataType &);
        int removeAtEnd(DataType &);
        int removeByValue(DataType &);
        int retrieveAtFront(DataType &) const;
        int retrieveAtEnd(DataType &) const;
        int retrieveByValue(DataType &) const;
        int isInList(const DataType &) const;
	int isEmpty() const;
        void clear();

        // miscellaneous
        friend ostream &operator<<(ostream &, const List<DataType> &);

protected:
        // internal data
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
