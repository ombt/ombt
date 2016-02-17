// member functions for link list class

// required headers
#include "list.h"

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
	first(NULL), last(NULL)
{
	// do nothing
}

template <class DataType>
List<DataType>::List(const List<DataType> &list):
	first(NULL), last(NULL)
{
	for (ListItem<DataType> *pos = list.first; pos != NULL; 
	     pos = pos->next)
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
	for (ListItem<DataType> *pos = first; pos != NULL; )
	{
		ListItem<DataType> *save = pos->next;
		delete pos;
		pos = save;
	}
	first = last = NULL;

	// copy new list
	for (pos = list.first; pos != NULL; pos = pos->next)
	{
		insertAtEnd(pos->data);
	}
	return(*this);
}

// list operations
template <class DataType>
void
List<DataType>::insertAtFront(const DataType &data)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// allocate a new item
	ListItem<DataType> *pitem = new ListItem<DataType>(data);
	MustBeTrue(pitem != NULL);

	// insert new item in list
	if (first == NULL && last == NULL)
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

	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));
	return;
}

template <class DataType>
void
List<DataType>::insertAtEnd(const DataType &data)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// allocate a new item
	ListItem<DataType> *pitem = new ListItem<DataType>(data);
	MustBeTrue(pitem != NULL);

	// insert new item in list
	if (first == NULL && last == NULL)
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

	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));
	return;
}

template <class DataType>
void
List<DataType>::insertByValue(const DataType &data)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// search for correct place to insert new item
	for (ListItem<DataType> *pos = first; pos != NULL; 
	     pos = pos->next)
	{
		// find correct position to insert new link
		// there are possible ways for this insert
		// to work. it can either allow more than
		// one item with the same key, or allow
		// only one by overwriting the old one.
		// this function allows multiple copies
		// of the same data.
		//
		if (pos->data > data)
		{
			// allocate a new item
			ListItem<DataType> *pitem = 
				new ListItem<DataType>(data);
			MustBeTrue(pitem != NULL);

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
			return;
		}
	}

	// insert at end of list
	insertAtEnd(data);

	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));
	return;
}

template <class DataType>
void
List<DataType>::insertByValueUnique(const DataType &data)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// search for correct place to insert new item
	for (ListItem<DataType> *pos = first; pos != NULL; 
	     pos = pos->next)
	{
		// find correct position to insert new link
		// there are two possible ways for this insert
		// to work. it can either allow more than
		// one item with the same key, or allow
		// only one by overwriting the old one.
		// this function overwrites existing tuples.
		//
		if (pos->data == data)
		{
			pos->data = data;
			return;
		}
		else if (pos->data > data)
		{
			// allocate a new item
			ListItem<DataType> *pitem = 
				new ListItem<DataType>(data);
			MustBeTrue(pitem != NULL);

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
			return;
		}
	}

	// insert at end of list
	insertAtEnd(data);

	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));
	return;
}

template <class DataType>
int
List<DataType>::removeAtFront(DataType &data)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check for an empty list
	if (first == NULL && last == NULL)
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

	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));
	return(OK);
}

template <class DataType>
int
List<DataType>::removeAtEnd(DataType &data)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check for an empty list
	if (first == NULL && last == NULL)
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

	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));
	return(OK);
}

template <class DataType>
int
List<DataType>::removeByValue(DataType &data)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check for an empty list
	if (first == NULL && last == NULL)
		return(NOTOK);

	// search for correct place to delete data item
	ListItem<DataType> *pos;
	for (pos = first; pos != NULL; pos = pos->next)
	{
		// find correct position to delete link
		if (pos->data == data) break;
	}

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

	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// all done 
	return(OK);
}

template <class DataType>
int
List<DataType>::retrieveAtFront(DataType &data) const
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	if (first != NULL)
	{
		data = first->data;
		return(OK);
	}
	else
		return(NOTOK);
}

template <class DataType>
int
List<DataType>::retrieveAtEnd(DataType &data) const
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	if (last != NULL)
	{
		data = last->data;
		return(OK);
	}
	else
		return(NOTOK);
}

template <class DataType>
int
List<DataType>::retrieveByValue(DataType &data) const
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check for empty list
	if (first == NULL && last == NULL)
		return(NOTOK);

	// search for data item
	for (ListItem<DataType> *p = first; p != NULL; p = p->next)
	{
		if (data == p->data)
		{
			data = p->data;
			return(OK);
		}
	}
	return(NOMATCH);
}

template <class DataType>
int
List<DataType>::isInList(const DataType &data) const
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	for (ListItem<DataType> *p = first; p != NULL; p = p->next)
	{
		if (data == p->data)
			return(1);
	}
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
	for (ListItem<DataType> *pos = l.first; pos != l.last; pos = pos->next)
	{
		os << *pos << endl;
	}
	if (pos != NULL) os << *pos;
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

