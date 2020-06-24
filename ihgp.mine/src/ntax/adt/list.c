// member functions for link list class

// required headers
#include "list.h"

// list item constructors and destructor
template <class DataType>
ListItem<DataType>::ListItem(const DataType &src):
	data(src), next(ENDOFLIST), previous(ENDOFLIST)
{
	// do nothing
}

template <class DataType>
ListItem<DataType>::ListItem(const ListItem<DataType> &item):
	data(item.data), next(ENDOFLIST), previous(ENDOFLIST)
{
	// do nothing
}

template <class DataType>
ListItem<DataType>::~ListItem()
{
	next = ENDOFLIST;
	previous = ENDOFLIST;
}

// assignment and equivalence
template <class DataType>
ListItem<DataType> &
ListItem<DataType>::operator=(const ListItem<DataType> &item)
{
	if (this != &item)
	{
		data = item.data;
		next = ENDOFLIST;
		previous = ENDOFLIST;
	}
	return(*this);
}

template <class DataType>
int
ListItem<DataType>::operator==(const ListItem<DataType> &item) const
{
	return(data == item.data);
}

template <class DataType>
int
ListItem<DataType>::operator!=(const ListItem<DataType> &item) const
{
	return( ! (data == item.data));
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
	first(ENDOFLIST), last(ENDOFLIST)
{
	// do nothing
}

template <class DataType>
List<DataType>::List(const List<DataType> &list):
	first(ENDOFLIST), last(ENDOFLIST)
{
	for (ListItem<DataType> *pos = list.first; pos != ENDOFLIST; 
	     pos = pos->next)
	{
		assert(insertAtEnd(pos->data) == OK);
	}
}

template <class DataType>
List<DataType>::~List()
{
	for (ListItem<DataType> *pos = first; pos != ENDOFLIST; )
	{
		ListItem<DataType> *save = pos->next;
		delete pos;
		pos = save;
	}
	first = last = ENDOFLIST;
}

//assignment
template <class DataType>
List<DataType> &
List<DataType>::operator=(const List<DataType> &list)
{
	// check for self-assignment
	if (this == &list) return(*this);

	// delete old list
	for (ListItem<DataType> *pos = first; pos != ENDOFLIST; )
	{
		ListItem<DataType> *save = pos->next;
		delete pos;
		pos = save;
	}
	first = last = ENDOFLIST;

	// copy new list
	for (pos = list.first; pos != ENDOFLIST; pos = pos->next)
	{
		assert(insertAtEnd(pos->data) == OK);
	}
	return(*this);
}

// list operations
template <class DataType>
int
List<DataType>::insertAtFront(const DataType &data)
{
	// allocate a new item
	ListItem<DataType> *pitem = new ListItem<DataType>(data);
	assert(pitem != 0);

	// insert new item in list
	if (first == ENDOFLIST && last == ENDOFLIST)
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
		pitem->previous = ENDOFLIST;
		first = pitem;
	}
	return(OK);
}

template <class DataType>
int
List<DataType>::insertAtEnd(const DataType &data)
{
	// allocate a new item
	ListItem<DataType> *pitem = new ListItem<DataType>(data);
	assert(pitem != 0);

	// insert new item in list
	if (first == ENDOFLIST && last == ENDOFLIST)
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
		pitem->next = ENDOFLIST;
		last = pitem;
	}
	return(OK);
}

template <class DataType>
int
List<DataType>::insertByValue(const DataType &data)
{
	// search for correct place to insert new item
	for (ListItem<DataType> *pos = first; pos != ENDOFLIST; 
	     pos = pos->next)
	{
		// find correct position to insert new link
		if (pos->data > data)
		{
			// allocate a new item
			ListItem<DataType> *pitem = 
				new ListItem<DataType>(data);
			assert(pitem != 0);

			// updata links
			pitem->previous = pos->previous;
			pitem->next = pos;
			pos->previous = pitem;
			if (pitem->previous != ENDOFLIST)
			{
				// previous link must point to new link
				pitem->previous->next = pitem;
			}
			else
			{
				// at beginning of list
				first = pitem;
			}
			return(OK);
		}
	}

	// insert at end of list
	return(insertAtEnd(data));
}

template <class DataType>
int
List<DataType>::removeAtFront(DataType &data)
{
	// check for an empty list
	if (first == ENDOFLIST && last == ENDOFLIST)
		return(NOTOK);

	// check if you are removing the last list item
	if (first == last)
	{
		// removing the last list item
		data = first->data;
		delete first;
		first = ENDOFLIST;
		last = ENDOFLIST;
	}
	else
	{
		// return string at front of list
		data = first->data;

		// delete first list item
		ListItem<DataType> *psave = first;
		first = first->next;
		first->previous = ENDOFLIST;
		delete psave;
	}
	return(OK);
}

template <class DataType>
int
List<DataType>::removeAtEnd(DataType &data)
{
	// check for an empty list
	if (first == ENDOFLIST && last == ENDOFLIST)
		return(NOTOK);

	// check if you are removing the last list item
	if (first == last)
	{
		// removing the last list item
		data = last->data;
		delete last;
		first = ENDOFLIST;
		last = ENDOFLIST;
	}
	else
	{
		// return string at end of list
		data = last->data;

		// delete last list item
		ListItem<DataType> *psave = last;
		last = last->previous;
		last->next = ENDOFLIST;
		delete psave;
	}
	return(OK);
}

template <class DataType>
int
List<DataType>::removeByValue(DataType &data)
{
	// search for correct place to delete data item
	ListItem<DataType> *pos;
	for (pos = first; pos != ENDOFLIST; pos = pos->next)
	{
		// find correct position to delete link
		if (pos->data == data) break;
	}

	// was anything found
	if (pos == ENDOFLIST) 
		return(NOMATCH);
	else
		data = pos->data;

	// delete item
	if (pos->previous != ENDOFLIST && pos->next != ENDOFLIST)
	{
		// delete item in middle of list
		pos->previous->next = pos->next;
		pos->next->previous = pos->previous;
		delete pos;
	}
	else if (pos->previous == ENDOFLIST && pos->next != ENDOFLIST)
	{
		// delete first item in list
		first = pos->next;
		pos->next->previous = ENDOFLIST;
		delete pos;
	}
	else if (pos->previous != ENDOFLIST && pos->next == ENDOFLIST)
	{
		// delete last item in list
		last = pos->previous;
		pos->previous->next = ENDOFLIST;
		delete pos;
	}
	else
	{
		// delete onLy item in list
		first = ENDOFLIST;
		last = ENDOFLIST;
		delete pos;
	}

	// all done 
	return(OK);
}

template <class DataType>
int
List<DataType>::retrieveAtFront(DataType &data) const
{
	if (first != ENDOFLIST)
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
	if (last != ENDOFLIST)
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
	for (ListItem<DataType> *p = first; p != ENDOFLIST; p = p->next)
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
List<DataType>::isInList(DataType &data) const
{
	for (ListItem<DataType> *p = first; p != ENDOFLIST; p = p->next)
	{
		if (data == p->data)
		{
			return(1);
		}
	}
	return(0);
}

template <class DataType>
int
List<DataType>::clear()
{
	for (ListItem<DataType> *pos = first; pos != ENDOFLIST; )
	{
		ListItem<DataType> *save = pos->next;
		delete pos;
		pos = save;
	}
	first = last = ENDOFLIST;
	return(OK);
}

template <class DataType>
int
List<DataType>::isEmpty() const
{
	return((first == ENDOFLIST) && (last == ENDOFLIST));
}

// print list
template <class DataType>
ostream &
operator<<(ostream &os, const List<DataType> &l)
{
	os << "{ ";
	for (ListItem<DataType> *pos = l.first; pos != l.last; pos = pos->next)
	{
		os << *pos << ",";
	}
	if (pos != ENDOFLIST) os << *pos;
	os << " }" << endl;
	return(os);
}

// list iterator constructors and destructor
template <class DataType>
ListIterator<DataType>::ListIterator(): 
	current(ENDOFLIST)
{
	// do nothing
}

template <class DataType>
ListIterator<DataType>::ListIterator(const ListIterator<DataType> &li): 
	current(li.next)
{
	// do nothing
}

template <class DataType>
ListIterator<DataType>::ListIterator(const List<DataType> &list): 
	current(list.first)
{
	// do nothing
}

template <class DataType>
ListIterator<DataType>::~ListIterator()
{
	current = ENDOFLIST;
}

// assignment
template <class DataType>
ListIterator<DataType> &
ListIterator<DataType>::operator=(const ListIterator<DataType> &li)
{
	if (this != &li)
		current = li.current;
	return(*this);
}

template <class DataType>
ListIterator<DataType> &
ListIterator<DataType>::operator=(const List<DataType> &list)
{
	current = list.first;
	return(*this);
}

// is list empty
template <class DataType>
int
ListIterator<DataType>::done() const
{
	return(current == ENDOFLIST);
}

// return current data
template <class DataType>
DataType
ListIterator<DataType>::operator()()
{
	assert(current != ENDOFLIST);
	return(current->data);
}

// increment to next data item in list
template <class DataType>
DataType
ListIterator<DataType>::operator++()
{
	assert(current != ENDOFLIST);
	ListItem<DataType> *save = current;
	current = current->next;
	return(save->data);
}

