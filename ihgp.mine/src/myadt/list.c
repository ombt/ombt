// member functions for link list class

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>

// local headers
#include "list.h"

// list item constructors and destructor
ListItem::ListItem(DataItem *d):
	data(d), next(ENDOFLIST), previous(ENDOFLIST)
{
	// do nothing
}

ListItem::ListItem(const ListItem &item):
	data(item.data), next(ENDOFLIST), previous(ENDOFLIST)
{
	// do nothing
}

ListItem::~ListItem()
{
	next = ENDOFLIST;
	previous = ENDOFLIST;
}

// assignment and equivalence
ListItem &
ListItem::operator=(const ListItem &item)
{
	if (this != &item)
	{
		data = item.data;
		next = ENDOFLIST;
		previous = ENDOFLIST;
	}
	return(*this);
}

int
ListItem::operator==(const ListItem &item) const
{
	return(*data == *item.data);
}

int
ListItem::operator!=(const ListItem &item) const
{
	return(*data != *item.data);
}

// return next or previous items in list
ListPosition
ListItem::getPrevious() const
{
	return(previous);
}

ListPosition
ListItem::getNext() const
{
	return(next);
}

//print list item
void
ListItem::dump(ostream &os) const
{
	os << *data;
	return;
}

ostream &
operator<<(ostream &os, const ListItem &item)
{
	item.dump(os);
	return(os);
}

// list contructors and destructor
List::List(): 
	first(ENDOFLIST), last(ENDOFLIST)
{
	// do nothing
}

List::List(const List &list):
	first(ENDOFLIST), last(ENDOFLIST)
{
	for (ListPosition pos = list.first; pos != ENDOFLIST; 
	     pos = pos->next)
	{
		assert(insert(*this, pos->data, ENDOFLIST) == OK);
	}
}

List::~List()
{
	for (ListPosition pos = first; pos != ENDOFLIST; )
	{
		ListPosition save = pos->next;
		delete pos;
		pos = save;
	}
	first = last = ENDOFLIST;
}

//assignment
List &
List::operator=(const List &l)
{
	// check for self-assignment
	if (this == &l) return(*this);

	// delete old list
	for (ListPosition pos = first; pos != ENDOFLIST; )
	{
		ListPosition save = pos->next;
		delete pos;
		pos = save;
	}
	first = last = ENDOFLIST;

	// copy new list
	for (pos = l.first; pos != ENDOFLIST; pos = pos->next)
	{
		assert(insert(*this, pos->data, ENDOFLIST) == OK);
	}
	return(*this);
}

// list operations
int
insert(List &l, DataItem *d, ListPosition p)
{
	// create a new list item for the data
	ListItem *newitem = new ListItem(d);
	assert(newitem != (ListItem *)0);

	// insert new item in list
	if (p == STARTOFLIST)
	{
		if (l.first == ENDOFLIST)
		{
			// first item in list
			newitem->previous = ENDOFLIST;
			newitem->next = ENDOFLIST;
			l.first = newitem;
			l.last = newitem;
		}
		else
		{
			// add item to beginning of list
			newitem->next = l.first;
			newitem->next->previous = newitem;
			newitem->previous = ENDOFLIST;
			l.first = newitem;
		}
	}
	else if (p == ENDOFLIST)
	{
		if (l.last == ENDOFLIST)
		{
			// first item in list
			newitem->previous = ENDOFLIST;
			newitem->next = ENDOFLIST;
			l.first = newitem;
			l.last = newitem;
		}
		else
		{
			// add item to end of list
			newitem->previous = l.last;
			newitem->previous->next = newitem;
			newitem->next = ENDOFLIST;
			l.last = newitem;
		}
	}
	else if (p == l.first)
	{
		if (l.first == ENDOFLIST)
		{
			// first item in list
			newitem->previous = ENDOFLIST;
			newitem->next = ENDOFLIST;
			l.first = newitem;
			l.last = newitem;
		}
		else
		{
			// add item to beginning of list
			newitem->next = l.first;
			newitem->next->previous = newitem;
			newitem->previous = ENDOFLIST;
			l.first = newitem;
		}
	}
	else if (p == l.last)
	{
		if (l.last == ENDOFLIST)
		{
			// first item in list
			newitem->previous = ENDOFLIST;
			newitem->next = ENDOFLIST;
			l.first = newitem;
			l.last = newitem;
		}
		else
		{
			// add item to end of list
			newitem->previous = l.last;
			newitem->previous->next = newitem;
			newitem->next = ENDOFLIST;
			l.last = newitem;
		}
	}
	else
	{
		newitem->previous = p->previous;
		newitem->next = p;
		newitem->previous->next = newitem;
		newitem->next->previous = newitem;
	}
	return(OK);
}

int
locate(const List &l, const DataItem *d, ListPosition &p)
{
	for (ListPosition pos = l.first; pos != ENDOFLIST; pos = pos->next)
	{
		if (*pos->data == *d)
		{
			p = pos;
			return(OK);
		}
	}
	return(NOMATCH);
}

int
retrieve(const List &l, DataItem *&d, const ListPosition p)
{
	if (p == STARTOFLIST)
	{
		if (l.first != ENDOFLIST)
		{
			d = l.first->data;
			return(OK);
		}
		else
			return(NOMATCH);
	}
	else if (p == ENDOFLIST)
	{
		if (l.last != ENDOFLIST)
		{
			d = l.last->data;
			return(OK);
		}
		else
			return(NOMATCH);
	}
	else if (p != (const)NULLLIST)
	{
		d = p->data;
		return(OK);
	}
	else
		return(NOMATCH);
}

int
remove(List &l, ListPosition p)
{
	// set pointer
	if (p == STARTOFLIST)
		p = l.first;
	else if (p == ENDOFLIST)
		p = l.last;
	else if (p == (ListPosition)NULLLIST)
		return(NOTOK);
		
	// remove list item from link list
	if (p == l.first && p != ENDOFLIST)
	{
		// save next link and delete first link
		ListPosition next = l.first->next;
		delete l.first;

		// check if list is empty
		if (next != ENDOFLIST)
		{
			// list is not empty
			next->previous = ENDOFLIST;
			l.first = next;
		}
		else
		{
			// delete last item in list
			l.first = ENDOFLIST;
			l.last = ENDOFLIST;
		}
	}
	else if (p == l.last && p != ENDOFLIST)
	{
		// save previous link and delete last link
		ListPosition previous = l.last->previous;
		delete l.last;

		// check if list is empty
		if (previous != ENDOFLIST)
		{
			// list is not empty
			previous->next = ENDOFLIST;
			l.last = previous;
		}
		else
		{
			// delete last item in list
			l.first = ENDOFLIST;
			l.last = ENDOFLIST;
		}
	}
	else if (p != ENDOFLIST)
	{
		p->previous->next = p->next;
		p->next->previous = p->previous;
		delete p;
	}
	else
	{
		return(NOTOK);
	}
	return(OK);
}

int
retrieveAndRemove(List &l, DataItem *&d, ListPosition p)
{
	int status;
	if ((status = retrieve(l, d, p)) != OK)
		return(status);
	else
		return(remove(l, p));
}

ListPosition
getPrevious(const List &l, const ListPosition p)
{
	if (p == STARTOFLIST)
	{
		return(ENDOFLIST);
	}
	else if (p == ENDOFLIST)
	{
		if (l.last != ENDOFLIST)
			return(l.last->previous);
		else
			return(ENDOFLIST);
	}
	else if (p != (const)NULLLIST)
	{
		return(p->previous);
	}
	else
		return(ENDOFLIST);
}

ListPosition
getNext(const List &l, const ListPosition p)
{
	if (p == ENDOFLIST)
	{
		return(ENDOFLIST);
	}
	else if (p == STARTOFLIST)
	{
		if (l.first != ENDOFLIST)
			return(l.first->next);
		else
			return(ENDOFLIST);
	}
	else if (p != (const)NULLLIST)
	{
		return(p->next);
	}
	else
		return(ENDOFLIST);
}

ListPosition
getFirst(const List &l)
{
	return(l.first);
}

ListPosition
getLast(const List &l)
{
	return(l.last);
}

int
makeNull(List &l)
{
	for (ListPosition pos = l.first; pos != ENDOFLIST; )
	{
		ListPosition save = pos->next;
		delete pos;
		pos = save;
	}
	l.first = l.last = ENDOFLIST;
	return(OK);
}

int
isEmpty(const List &l)
{
	return((l.first == l.last) && (l.last == ENDOFLIST));
}

// print list
void
List::dump(ostream &os) const
{
	os << "{ ";
	for (ListPosition pos = first; pos != last; pos = pos->next)
	{
		os << *pos << ",";
	}
	if (pos != ENDOFLIST) os << *pos;
	os << " }" << endl;
	return;
}

ostream &
operator<<(ostream &os, const List &l)
{
	l.dump(os);
	return(os);
}

// list iterator constructors and destructor
ListIterator::ListIterator(): 
	next(ENDOFLIST)
{
	// do nothing
}

ListIterator::ListIterator(const ListIterator &li): 
	next(li.next)
{
	// do nothing
}
ListIterator::ListIterator(const List &l): 
	next(l.first)
{
	// do nothing
}

ListIterator::~ListIterator()
{
	next = ENDOFLIST;
}

// assignment
ListIterator &
ListIterator::operator=(const ListIterator &li)
{
	if (this != &li)
		next = li.next;
	return(*this);
}

ListIterator &
ListIterator::operator=(const List &l)
{
	next = l.first;
	return(*this);
}

// iterator function
DataItem *
ListIterator::operator()()
{
	if (next != ENDOFLIST)
	{
		DataItem *pdata = next->data;
		next = next->next;
		return(pdata);
	}
	else
		return((DataItem *)0);
}

