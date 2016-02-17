#ifndef __LIST_H
#define __LIST_H
// list class definition

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>

// local headers
#include "returns.h"
#include "debug.h"

// forward declarations
template <class DataType> class ListItem;
template <class DataType> class List;
template <class DataType> class ListIterator;

// definitions
#define ENDOFLIST NULL

// list item class
template <class DataType> class ListItem {
public:
        // friend classes and functions
        friend class List<DataType>;
        friend class ListIterator<DataType>;

        // constructors and destructor
        ListItem(const DataType &);
        ListItem(const ListItem &);
        ~ListItem();

        // assignment and equivalence
        ListItem &operator=(const ListItem &);
        int operator==(const ListItem &) const;
        int operator!=(const ListItem &) const;

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

        // constructors and destructor
        List();
        List(const List &);
        ~List();

        // assignment
        List &operator=(const List &);

        // list operations
        int insertAtFront(const DataType &);
        int insertAtEnd(const DataType &);
        int insertByValue(const DataType &);
        int removeAtFront(DataType &);
        int removeAtEnd(DataType &);
        int removeByValue(DataType &);
        int retrieveAtFront(DataType &) const;
        int retrieveAtEnd(DataType &) const;
        int retrieveByValue(DataType &) const;
        int isInList(DataType &) const;
	int isEmpty() const;
        int clear();

        // miscellaneous
        friend ostream &operator<<(ostream &, const List<DataType> &);

protected:
        // internal data
        ListItem<DataType> *first;
        ListItem<DataType> *last;
};

// list iterator
template <class DataType> class ListIterator {
public:
        // constructors and destructor
        ListIterator();
        ListIterator(const ListIterator &);
        ListIterator(const List<DataType> &);
        ~ListIterator();

        // assignment
        ListIterator &operator=(const List<DataType> &);
        ListIterator &operator=(const ListIterator &);

	// check if at end of list
	int done() const;

        // return data 
        DataType operator()();

	// advance iterator to next link
	DataType operator++();

protected:
        // internal data
        ListItem<DataType> *current;
};

#endif
