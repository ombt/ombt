#ifndef __LIST_H
#define __LIST_H
// list class definition

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "data.h"

// forward declarations
class ListItem;
class List;
class ListIterator;

// position definition
typedef ListItem *ListPosition;

// definitions
#define STARTOFLIST ((ListPosition)-1)
#define ENDOFLIST ((ListPosition)0)
#define NULLLIST ((List *)0)

// list item class
class ListItem {
public:
        // friend classes and functions
        friend class List;
        friend class ListIterator;
        friend int insert(List &, DataItem *, ListPosition);
        friend int locate(const List &, const DataItem *, ListPosition &);
        friend int retrieve(const List &, DataItem *&, const ListPosition);
        friend int remove(List &, ListPosition);
        friend int retrieveAndRemove(List &, DataItem *&, ListPosition);
        friend ListPosition getNext(const List &, const ListPosition);
        friend ListPosition getPrevious(const List &, const ListPosition);
        friend ListPosition getFirst(const List &);
        friend ListPosition getLast(const List &);
        friend int makeNull(List &);
        friend int isEmpty(List &);

        // constructors and destructor
        ListItem(DataItem *);
        ListItem(const ListItem &);
        virtual ~ListItem();

        // assignment and equivalence
        virtual ListItem &operator=(const ListItem &);
        virtual int operator==(const ListItem &) const;
        virtual int operator!=(const ListItem &) const;

        // get previous and next links
        virtual ListPosition getNext() const;
        virtual ListPosition getPrevious() const;

        // miscellaneous
        virtual void dump(ostream & = cout) const;
        friend ostream &operator<<(ostream &, const ListItem &);

private:
        // disallow list item without data
        ListItem();

protected:
        // internal data
        DataItem *data;
        ListPosition next;
        ListPosition previous;
};

// list class
class List {
public:
        // friend classes
        friend class ListIterator;

        // constructors and destructor
        List();
        List(const List &);
        virtual ~List();

        // assignment
        List &operator=(const List &);

        // list operations
        friend int insert(List &, DataItem *, ListPosition);
        friend int locate(const List &, const DataItem *, ListPosition &);
        friend int retrieve(const List &, DataItem *&, const ListPosition);
        friend int remove(List &, ListPosition);
        friend int retrieveAndRemove(List &, DataItem *&, ListPosition);
        friend ListPosition getNext(const List &, const ListPosition);
        friend ListPosition getPrevious(const List &, const ListPosition);
        friend ListPosition getFirst(const List &);
        friend ListPosition getLast(const List &);
        friend int makeNull(List &);
        friend int isEmpty(const List &);

        // miscellaneous
        virtual void dump(ostream & = cout) const;
        friend ostream &operator<<(ostream &, const List &);

protected:
        // internal data
        ListPosition first;
        ListPosition last;
};

// list iterator
class ListIterator {
public:
        // constructors and destructor
        ListIterator();
        ListIterator(const ListIterator &);
        ListIterator(const List &);
        virtual ~ListIterator();

        // assignment
        ListIterator &operator=(const List &);
        ListIterator &operator=(const ListIterator &);

        // return next link
        DataItem *operator()();

protected:
        // internal data
        ListPosition next;
};

#endif
