// member functions for link list class

// required headers
#include "mylist.h"

// list item constructors and destructor
template <class DataType>
ListItem<DataType>::ListItem(const DataType &src):
	data(src), next(NULL), previous(NULL)
{
	// do nothing
}

template <class DataType>
ListItem<DataType>::ListItem(const ListItem<DataType> &item):
	data(item.data), next(NULL), previous(NULL)
{
	// do nothing
}

template <class DataType>
ListItem<DataType>::~ListItem()
{
	next = NULL;
	previous = NULL;
}

// assignment 
template <class DataType>
ListItem<DataType> &
ListItem<DataType>::operator=(const ListItem<DataType> &item)
{
	if (this != &item)
	{
		data = item.data;
		next = NULL;
		previous = NULL;
	}
	return(*this);
}

// print list item
template <class DataType>
ostream &
operator<<(ostream &os, const ListItem<DataType> &item)
{
	os << item.data;
	return(os);
}

// list contructors and destructor
template <class DataType>
List<DataType>::List(): 
	count(0), first(NULL), last(NULL)
{
	// do nothing
}

template <class DataType>
List<DataType>::List(const List<DataType> &list):
	count(0), first(NULL), last(NULL)
{
	for (ListItem<DataType> *pos = list.first; pos != NULL; 
	     pos = pos->next, count++)
	{
		insertAtEnd(pos->data);
	}
}

template <class DataType>
List<DataType>::~List()
{
	for (ListItem<DataType> *pos = first; pos != NULL; )
	{
		ListItem<DataType> *save = pos->next;
		delete pos;
		pos = save;
	}
	count = 0;
	first = last = NULL;
}

//assignment
template <class DataType>
List<DataType> &
List<DataType>::operator=(const List<DataType> &list)
{
	// check for self-assignment
	if (this == &list) return(*this);

	// delete old list
	ListItem<DataType> *pos;
	for (pos = first; pos != NULL; )
	{
		ListItem<DataType> *save = pos->next;
		delete pos;
		pos = save;
	}
	count = 0;
	first = last = NULL;

	// copy new list
	for (pos = list.first; pos != NULL; pos = pos->next, count++)
	{
		insertAtEnd(pos->data);
	}
	return(*this);
}

// list operations for unordered lists with duplicates
template <class DataType>
int
List<DataType>::retrieve(DataType &data) const
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check for empty list
	if (isEmpty())
		return(NOTOK);

	// search for data item
	ListItem<DataType> *p;
	for (p = first; p != NULL && data != p->data; p = p->next) ;
	if (p != NULL)
	{
		data = p->data;
		return(OK);
	}
	else
		return(NOMATCH);
}

template <class DataType>
int
List<DataType>::insert(const DataType &data)
{
	return(insertAtFront(data));
}

template <class DataType>
int
List<DataType>::update(const DataType &data)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check for empty list
	if (isEmpty())
		return(NOTOK);

	// search for data item
	ListItem<DataType> *p;
	for (p = first; p != NULL && data != p->data; p = p->next) ;
	if (p != NULL)
	{
		p->data = data;
		return(OK);
	}
	else
		return(NOMATCH);
}

template <class DataType>
int
List<DataType>::remove(DataType &data)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check for an empty list
	if (isEmpty())
		return(NOTOK);

	// search for correct place to delete data item
	ListItem<DataType> *pos;
	for (pos = first; pos != NULL && data != pos->data; pos = pos->next) ;

	// was anything found
	if (pos == NULL)
		return(NOMATCH);
	else
		data = pos->data;

	// delete item
	if (pos->previous != NULL && pos->next != NULL)
	{
		// delete item in middle of list
		pos->previous->next = pos->next;
		pos->next->previous = pos->previous;
	}
	else if (pos->previous == NULL && pos->next != NULL)
	{
		// delete first item in list
		first = pos->next;
		pos->next->previous = NULL;
	}
	else if (pos->previous != NULL && pos->next == NULL)
	{
		// delete last item in list
		last = pos->previous;
		pos->previous->next = NULL;
	}
	else
	{
		// delete only item in list
		first = NULL;
		last = NULL;
	}
	delete pos;
	count--;

	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// all done 
	return(OK);
}

// list operations for unordered lists without duplicates
template <class DataType>
int
List<DataType>::retrieveUnique(DataType &data) const
{
	return(retrieve(data));
}

template <class DataType>
int
List<DataType>::insertUnique(const DataType &data)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// search for correct place to insert new item
	ListItem<DataType> *pos;
	for (pos = first; pos != NULL && pos->data != data ; pos = pos->next) ;

	// check which case
	if (pos != NULL && pos->data == data)
	{
		// we found it, overwrite current data
		pos->data = data;
		count++;
	}
	else
	{
		// insert at the beginning of the list
		return(insertAtFront(data));
	}

	// all done
	return(OK);
}

template <class DataType>
int
List<DataType>::updateUnique(const DataType &data)
{
	return(update(data));
}

template <class DataType>
int
List<DataType>::removeUnique(DataType &data)
{
	return(remove(data));
}

// list operations for the front
template <class DataType>
int
List<DataType>::retrieveAtFront(DataType &data) const
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	if (!isEmpty())
	{
		data = first->data;
		return(OK);
	}
	else
		return(NOTOK);
}

template <class DataType>
int
List<DataType>::insertAtFront(const DataType &data)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// allocate a new item
	ListItem<DataType> *pitem = new ListItem<DataType>(data);
	if (pitem == NULL) return(NOTOK);

	// insert new item in list
	if (isEmpty())
	{
		// first element in list
		first = pitem;
		last = pitem;
	}
	else
	{
		// insert item at beginning of list
		pitem->next = first;
		pitem->next->previous = pitem;
		pitem->previous = NULL;
		first = pitem;
	}
	count++;

	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));
	return(OK);
}

template <class DataType>
int
List<DataType>::updateAtFront(const DataType &data)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	if (!isEmpty())
	{
		first->data = data;
		return(OK);
	}
	else
		return(NOTOK);
}

template <class DataType>
int
List<DataType>::removeAtFront(DataType &data)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check for an empty list
	if (isEmpty())
		return(NOTOK);

	// check if you are removing the last list item
	if (first == last)
	{
		// removing the last list item
		data = first->data;
		delete first;
		first = NULL;
		last = NULL;
	}
	else
	{
		// return string at front of list
		data = first->data;

		// delete first list item
		ListItem<DataType> *psave = first;
		first = first->next;
		first->previous = NULL;
		delete psave;
	}
	count--;

	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));
	return(OK);
}

// list operations for the front
template <class DataType>
int
List<DataType>::retrieveAtEnd(DataType &data) const
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	if (!isEmpty())
	{
		data = last->data;
		return(OK);
	}
	else
		return(NOTOK);
}

template <class DataType>
int
List<DataType>::insertAtEnd(const DataType &data)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// allocate a new item
	ListItem<DataType> *pitem = new ListItem<DataType>(data);
	if (pitem == NULL) return(NOTOK);

	// insert new item in list
	if (isEmpty())
	{
		// first element in list
		first = pitem;
		last = pitem;
	}
	else
	{
		// insert item at end of list
		pitem->previous = last;
		pitem->previous->next = pitem;
		pitem->next = NULL;
		last = pitem;
	}
	count++;

	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));
	return(OK);
}

template <class DataType>
int
List<DataType>::updateAtEnd(const DataType &data)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	if (!isEmpty())
	{
		last->data = data;
		return(OK);
	}
	else
		return(NOTOK);
}

template <class DataType>
int
List<DataType>::removeAtEnd(DataType &data)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check for an empty list
	if (isEmpty())
		return(NOTOK);

	// check if you are removing the last list item
	if (first == last)
	{
		// removing the last list item
		data = last->data;
		delete last;
		first = NULL;
		last = NULL;
	}
	else
	{
		// return string at end of list
		data = last->data;

		// delete last list item
		ListItem<DataType> *psave = last;
		last = last->previous;
		last->next = NULL;
		delete psave;
	}
	count--;

	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));
	return(OK);
}

// ordered list operations allowing duplicates
template <class DataType>
int
List<DataType>::retrieveOrdered(DataType &data) const
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check for empty list
	if (isEmpty())
		return(NOTOK);

	// search for data item
	ListItem<DataType> *p;
	for (p = first; p != NULL && p->data < data; p = p->next) ;
	if (data == p->data)
	{
		data = p->data;
		return(OK);
	}
	else
		return(NOMATCH);
}

template <class DataType>
int
List<DataType>::insertOrdered(const DataType &data)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// search for correct place to insert new item
	ListItem<DataType> *pos;
	for (pos = first; pos != NULL && pos->data <= data; pos = pos->next) ;

	// check which case
	if (pos == NULL)
	{
		// insert at end of list
		return(insertAtEnd(data));
	}
	else
	{
		// allocate a new item
		ListItem<DataType> *pitem = 
			new ListItem<DataType>(data);
		if (pitem == NULL) return(NOTOK);

		// update links
		pitem->previous = pos->previous;
		pitem->next = pos;
		pos->previous = pitem;
		if (pitem->previous != NULL)
		{
			// previous link must point to new link
			pitem->previous->next = pitem;
		}
		else
		{
			// at beginning of list
			first = pitem;
		}
		count++;
		return(OK);
	}
}

template <class DataType>
int
List<DataType>::updateOrdered(const DataType &data)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check for empty list
	if (isEmpty())
		return(NOTOK);

	// search for data item
	ListItem<DataType> *p;
	for (p = first; p != NULL && p->data < data; p = p->next) ;
	if (data == p->data)
	{
		p->data = data;
		return(OK);
	}
	else
		return(NOMATCH);
}


template <class DataType>
int
List<DataType>::removeOrdered(DataType &data)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check for an empty list
	if (isEmpty())
		return(NOTOK);

	// search for correct place to delete data item
	ListItem<DataType> *pos;
	for (pos = first; pos != NULL && pos->data < data; pos = pos->next) ;

	// was anything found
	if (pos == NULL || data != pos->data) 
		return(NOMATCH);
	else
		data = pos->data;

	// delete item
	if (pos->previous != NULL && pos->next != NULL)
	{
		// delete item in middle of list
		pos->previous->next = pos->next;
		pos->next->previous = pos->previous;
	}
	else if (pos->previous == NULL && pos->next != NULL)
	{
		// delete first item in list
		first = pos->next;
		pos->next->previous = NULL;
	}
	else if (pos->previous != NULL && pos->next == NULL)
	{
		// delete last item in list
		last = pos->previous;
		pos->previous->next = NULL;
	}
	else
	{
		// delete only item in list
		first = NULL;
		last = NULL;
	}
	delete pos;
	count--;

	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// all done 
	return(OK);
}

// ordered list operations without duplicates
template <class DataType>
int
List<DataType>::retrieveOrderedUnique(DataType &data) const
{
	return(retrieveOrdered(data));
}

template <class DataType>
int
List<DataType>::insertOrderedUnique(const DataType &data)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// search for correct place to insert new item
	ListItem<DataType> *pos;
	for (pos = first; pos != NULL && pos->data < data ; pos = pos->next) ;

	// check which case
	if (pos == NULL)
	{
		// insert at end of list
		return(insertAtEnd(data));
	}
	else if (pos->data == data)
	{
		// we found it, overwrite current data
		pos->data = data;
		count++;
	}
	else
	{
		// allocate a new item
		ListItem<DataType> *pitem = 
			new ListItem<DataType>(data);
		if (pitem == NULL) return(NOTOK);

		// updata links
		pitem->previous = pos->previous;
		pitem->next = pos;
		pos->previous = pitem;
		if (pitem->previous != NULL)
		{
			// previous link must point to new link
			pitem->previous->next = pitem;
		}
		else
		{
			// at beginning of list
			first = pitem;
		}
		count++;
	}

	// all done
	return(OK);
}

template <class DataType>
int
List<DataType>::updateOrderedUnique(const DataType &data)
{
	return(updateOrdered(data));
}

template <class DataType>
int
List<DataType>::removeOrderedUnique(DataType &data)
{
	return(removeOrdered(data));
}

// list operations by position
template <class DataType>
int
List<DataType>::retrieveNth(int n, DataType &data) const
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check for empty list
	if (isEmpty())
		return(NOTOK);

	// check for out of range
	if (n < 1 || n > count)
		return(NOMATCH);

	// search for data item
	int i;
	ListItem<DataType> *p;
	for (i = 1, p = first; p != NULL && i < n; p = p->next, i++) ;
	if (p != NULL && i == n)
	{
		data = p->data;
		return(OK);
	}
	else
		return(NOMATCH);
}

template <class DataType>
int
List<DataType>::insertNth(int n, const DataType &data)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check for out of range
	if (n < 1)
		return(NOMATCH);

	// find location to insert new tuple
	if (isEmpty())
	{
		// add at the front
		return(insertAtFront(data));
	}
	else if (n > count)
	{
		// add at the end
		return(insertAtEnd(data));
	}

	// search for correct place to insert new item
	int i;
	ListItem<DataType> *pos;
	for (i = 1, pos = first; pos != NULL && i < n; pos = pos->next, i++) ;

	// check which case
	if (pos != NULL && i == n)
	{
		// allocate a new item
		ListItem<DataType> *pitem = 
			new ListItem<DataType>(data);
		if (pitem == NULL) return(NOTOK);

		// update links
		pitem->previous = pos->previous;
		pitem->next = pos;
		pos->previous = pitem;
		if (pitem->previous != NULL)
		{
			// previous link must point to new link
			pitem->previous->next = pitem;
		}
		else
		{
			// at beginning of list
			first = pitem;
		}

		// increment the counter
		count++;
		return(OK);
	}
	else
		return(NOTOK);
}

template <class DataType>
int
List<DataType>::updateNth(int n, const DataType &data)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check for empty list
	if (isEmpty())
		return(NOTOK);

	// check for out of range
	if (n < 1 || n > count)
		return(NOMATCH);

	// search for data item
	int i;
	ListItem<DataType> *p;
	for (i = 1, p = first; p != NULL && i < n; p = p->next, i++) ;
	if (p != NULL && i == n)
	{
		p->data = data;
		return(OK);
	}
	else
		return(NOTOK);
}

template <class DataType>
int
List<DataType>::removeNth(int n, DataType &data)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check for an empty list
	if (isEmpty())
		return(NOTOK);

	// check for out of range
	if (n < 1 || n > count)
		return(NOMATCH);

	// search for correct place to delete data item
	int i;
	ListItem<DataType> *pos;
	for (i = 1, pos = first; pos != NULL && i < n; pos = pos->next, i++) ;

	// was anything found
	if (pos == NULL || i != n)
		return(NOTOK);
	else
		data = pos->data;

	// delete item
	if (pos->previous != NULL && pos->next != NULL)
	{
		// delete item in middle of list
		pos->previous->next = pos->next;
		pos->next->previous = pos->previous;
	}
	else if (pos->previous == NULL && pos->next != NULL)
	{
		// delete first item in list
		first = pos->next;
		pos->next->previous = NULL;
	}
	else if (pos->previous != NULL && pos->next == NULL)
	{
		// delete last item in list
		last = pos->previous;
		pos->previous->next = NULL;
	}
	else
	{
		// delete only item in list
		first = NULL;
		last = NULL;
	}
	delete pos;
	count--;

	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// all done 
	return(OK);
}

// test functions
template <class DataType>
int
List<DataType>::includes(const DataType &data) const
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// search for data item
	ListItem<DataType> *p;
	for (p = first; p != NULL && data != p->data; p = p->next) ;
	if (p != NULL)
		return(1);
	else
		return(0);
}

template <class DataType>
void
List<DataType>::clear()
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	for (ListItem<DataType> *pos = first; pos != NULL; )
	{
		ListItem<DataType> *save = pos->next;
		delete pos;
		pos = save;
	}
	first = last = NULL;
	return;
}

template <class DataType>
int
List<DataType>::isEmpty() const
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	return(first == NULL && last == NULL);
}

// print list
template <class DataType>
ostream &
operator<<(ostream &os, const List<DataType> &l)
{
	os << "{ ";
	for (ListItem<DataType> *pos = l.first; pos != NULL; pos = pos->next)
	{
		os << *pos << ",";
	}
	os << " }" << endl;
	return(os);
}

// list iterator constructors and destructor
template <class DataType>
ListIterator<DataType>::ListIterator(const ListIterator<DataType> &li): 
	list(li.list), current(li.current)
{
	// do nothing
}

template <class DataType>
ListIterator<DataType>::ListIterator(const List<DataType> &l): 
	list(l), current(l.first)
{
	// do nothing
}

template <class DataType>
ListIterator<DataType>::~ListIterator()
{
	current = NULL;
}

// reset iterator to beginning
template <class DataType>
void
ListIterator<DataType>::reset()
{
	current = list.first;
	return;
}

// is list empty
template <class DataType>
int
ListIterator<DataType>::done() const
{
	return(current == NULL);
}

// return current data
template <class DataType>
DataType
ListIterator<DataType>::operator()()
{
	MustBeTrue(current != NULL);
	return(current->data);
}

// increment to next data item in list
template <class DataType>
int
ListIterator<DataType>::operator++(int)
{
	if (current != NULL)
	{
		ListItem<DataType> *save = current;
		current = current->next;
		return(OK);
	}
	else
		return(NOTOK);
}

// list reverse iterator constructors and destructor
template <class DataType>
ListIterator_Reverse<DataType>::ListIterator_Reverse(
	const ListIterator_Reverse<DataType> &li): 
	list(li.list), current(li.current)
{
	// do nothing
}

template <class DataType>
ListIterator_Reverse<DataType>::ListIterator_Reverse(const List<DataType> &l): 
	list(l), current(l.last)
{
	// do nothing
}

template <class DataType>
ListIterator_Reverse<DataType>::~ListIterator_Reverse()
{
	current = NULL;
}

// reset iterator to beginning
template <class DataType>
void
ListIterator_Reverse<DataType>::reset()
{
	current = list.last;
	return;
}

// is list empty
template <class DataType>
int
ListIterator_Reverse<DataType>::done() const
{
	return(current == NULL);
}

// return current data
template <class DataType>
DataType
ListIterator_Reverse<DataType>::operator()()
{
	MustBeTrue(current != NULL);
	return(current->data);
}

// increment to next data item in list
template <class DataType>
int
ListIterator_Reverse<DataType>::operator++(int)
{
	if (current != NULL)
	{
		ListItem<DataType> *save = current;
		current = current->previous;
		return(OK);
	}
	else
		return(NOTOK);
}

