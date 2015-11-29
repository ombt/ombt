// member functions for pointer-based link list class

// required headers
#include <adt/ptrlist.h>

// list item constructors and destructor
template <class DataType>
PtrListItem<DataType>::PtrListItem(DataType *psrc):
	pdata(psrc), next(NULL), previous(NULL)
{
	MustBeTrue(pdata != NULL);
}

template <class DataType>
PtrListItem<DataType>::PtrListItem(const PtrListItem<DataType> &item):
	pdata(item.pdata), next(NULL), previous(NULL)
{
	MustBeTrue(pdata != NULL);
}

template <class DataType>
PtrListItem<DataType>::~PtrListItem()
{
	pdata = NULL;
	next = NULL;
	previous = NULL;
}

// assignment 
template <class DataType>
PtrListItem<DataType> &
PtrListItem<DataType>::operator=(const PtrListItem<DataType> &item)
{
	// this is a memory leak as is.
	MustBeTrue(0);
	if (this != &item)
	{
		MustBeTrue(item.pdata != NULL);
		pdata = item.pdata;
		next = NULL;
		previous = NULL;
	}
	return(*this);
}

#if USEOSTREAM
// print list item
template <class DataType>
ostream &
operator<<(ostream &os, const PtrListItem<DataType> &item)
{
	MustBeTrue(item.pdata != NULL);
	os << *item.pdata;
	return(os);
}
#endif

// list contructors and destructor
template <class DataType>
PtrList<DataType>::PtrList(): 
	count(0), first(NULL), last(NULL)
{
	// do nothing
}

template <class DataType>
PtrList<DataType>::PtrList(const PtrList<DataType> &list):
	count(0), first(NULL), last(NULL)
{
	for (PtrListItem<DataType> *pos = list.first; pos != NULL; 
	     pos = pos->next, count++)
	{
		insertAtEnd(pos->pdata);
	}
}

template <class DataType>
PtrList<DataType>::~PtrList()
{
	for (PtrListItem<DataType> *pos = first; pos != NULL; )
	{
		PtrListItem<DataType> *save = pos->next;
		delete pos;
		pos = save;
	}
	count = 0;
	first = last = NULL;
}

//assignment
template <class DataType>
PtrList<DataType> &
PtrList<DataType>::operator=(const PtrList<DataType> &list)
{
	// check for self-assignment
	if (this == &list) return(*this);

	// delete old list
	PtrListItem<DataType> *pos;
	for (pos = first; pos != NULL; )
	{
		PtrListItem<DataType> *save = pos->next;
		delete pos;
		pos = save;
	}
	count = 0;
	first = last = NULL;

	// copy new list
	for (pos = list.first; pos != NULL; pos = pos->next, count++)
	{
		insertAtEnd(pos->pdata);
	}
	return(*this);
}

// list operations for unordered lists with duplicates
template <class DataType>
int
PtrList<DataType>::retrieve(DataType *&pdata) const
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check for empty list
	if (isEmpty())
		return(NOTOK);

	// search for data item
	PtrListItem<DataType> *p;
	for (p = first; p != NULL && *pdata != *p->pdata; p = p->next) ;
	if (p != NULL)
	{
		pdata = p->pdata;
		return(OK);
	}
	else
		return(NOMATCH);
}

template <class DataType>
int
PtrList<DataType>::insert(DataType *pdata)
{
	return(insertAtFront(pdata));
}

template <class DataType>
int
PtrList<DataType>::update(DataType *pdata)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check for empty list
	if (isEmpty())
		return(NOTOK);

	// search for data item
	PtrListItem<DataType> *p;
	for (p = first; p != NULL && *pdata != *p->pdata; p = p->next) ;
	if (p != NULL)
	{
		p->pdata = pdata;
		return(OK);
	}
	else
		return(NOMATCH);
}

template <class DataType>
int
PtrList<DataType>::remove(DataType *&pdata)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check for an empty list
	if (isEmpty())
		return(NOTOK);

	// search for correct place to delete data item
	PtrListItem<DataType> *pos;
	for (pos = first; pos != NULL && *pdata != *pos->pdata; pos = pos->next) ;

	// was anything found
	if (pos == NULL)
		return(NOMATCH);
	else
		pdata = pos->pdata;

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
PtrList<DataType>::retrieveUnique(DataType *&pdata) const
{
	return(retrieve(pdata));
}

template <class DataType>
int
PtrList<DataType>::insertUnique(DataType *pdata)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check parameters
	if (pdata == NULL)
		return(NOTOK);

	// search for correct place to insert new item
	PtrListItem<DataType> *pos;
	for (pos = first; pos != NULL && *pos->pdata != *pdata ; 
		pos = pos->next) ;

	// check which case
	if (pos != NULL && *pos->pdata == *pdata)
	{
		// we found it, overwrite current data
		pos->pdata = pdata;
	}
	else
	{
		// insert at the beginning of the list
		return(insertAtFront(pdata));
	}

	// all done
	return(OK);
}

template <class DataType>
int
PtrList<DataType>::updateUnique(DataType *pdata)
{
	return(update(pdata));
}

template <class DataType>
int
PtrList<DataType>::removeUnique(DataType *&pdata)
{
	return(remove(pdata));
}

// list operations for the front
template <class DataType>
int
PtrList<DataType>::retrieveAtFront(DataType *&pdata) const
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	if (!isEmpty())
	{
		pdata = first->pdata;
		return(OK);
	}
	else
		return(NOTOK);
}

template <class DataType>
int
PtrList<DataType>::insertAtFront(DataType *pdata)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check parameters
	if (pdata == NULL)
		return(NOTOK);

	// allocate a new item
	PtrListItem<DataType> *pitem = new PtrListItem<DataType>(pdata);
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
PtrList<DataType>::updateAtFront(DataType *pdata)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	if (!isEmpty())
	{
		first->pdata = pdata;
		return(OK);
	}
	else
		return(NOTOK);
}

template <class DataType>
int
PtrList<DataType>::removeAtFront(DataType *&pdata)
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
		pdata = first->pdata;
		delete first;
		first = NULL;
		last = NULL;
	}
	else
	{
		// return string at front of list
		pdata = first->pdata;

		// delete first list item
		PtrListItem<DataType> *psave = first;
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
PtrList<DataType>::retrieveAtEnd(DataType *&pdata) const
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	if (!isEmpty())
	{
		pdata = last->pdata;
		return(OK);
	}
	else
		return(NOTOK);
}

template <class DataType>
int
PtrList<DataType>::insertAtEnd(DataType *pdata)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check parameters
	if (pdata == NULL)
		return(NOTOK);

	// allocate a new item
	PtrListItem<DataType> *pitem = new PtrListItem<DataType>(pdata);
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
PtrList<DataType>::updateAtEnd(DataType *pdata)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	if (!isEmpty())
	{
		last->pdata = pdata;
		return(OK);
	}
	else
		return(NOTOK);
}

template <class DataType>
int
PtrList<DataType>::removeAtEnd(DataType *&pdata)
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
		pdata = last->pdata;
		delete last;
		first = NULL;
		last = NULL;
	}
	else
	{
		// return string at end of list
		pdata = last->pdata;

		// delete last list item
		PtrListItem<DataType> *psave = last;
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
PtrList<DataType>::retrieveOrdered(DataType *&pdata) const
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check for empty list
	if (isEmpty())
		return(NOTOK);

	// search for data item
	PtrListItem<DataType> *p;
	for (p = first; p != NULL && *p->pdata < *pdata; p = p->next) ;
	if (*pdata == *p->pdata)
	{
		pdata = p->pdata;
		return(OK);
	}
	else
		return(NOMATCH);
}

template <class DataType>
int
PtrList<DataType>::insertOrdered(DataType *pdata)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check parameters
	if (pdata == NULL)
		return(NOTOK);

	// search for correct place to insert new item
	PtrListItem<DataType> *pos;
	for (pos = first; pos != NULL && *pos->pdata <= *pdata; pos = pos->next) ;

	// check which case
	if (pos == NULL)
	{
		// insert at end of list
		return(insertAtEnd(pdata));
	}
	else
	{
		// allocate a new item
		PtrListItem<DataType> *pitem = 
			new PtrListItem<DataType>(pdata);
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
PtrList<DataType>::updateOrdered(DataType *pdata)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check for empty list
	if (isEmpty())
		return(NOTOK);

	// search for data item
	PtrListItem<DataType> *p;
	for (p = first; p != NULL && *p->pdata < *pdata; p = p->next) ;
	if (data == p->data)
	{
		p->pdata = pdata;
		return(OK);
	}
	else
		return(NOMATCH);
}


template <class DataType>
int
PtrList<DataType>::removeOrdered(DataType *&pdata)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check for an empty list
	if (isEmpty())
		return(NOTOK);

	// search for correct place to delete data item
	PtrListItem<DataType> *pos;
	for (pos = first; pos != NULL && *pos->pdata < *pdata; pos = pos->next) ;

	// was anything found
	if (pos == NULL || data != pos->data) 
		return(NOMATCH);
	else
		pdata = pos->pdata;

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
PtrList<DataType>::retrieveOrderedUnique(DataType *&pdata) const
{
	return(retrieveOrdered(pdata));
}

template <class DataType>
int
PtrList<DataType>::insertOrderedUnique(DataType *pdata)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check parameters
	if (pdata == NULL)
		return(NOTOK);

	// search for correct place to insert new item
	PtrListItem<DataType> *pos;
	for (pos = first; pos != NULL && *pos->pdata < *pdata ; pos = pos->next) ;

	// check which case
	if (pos == NULL)
	{
		// insert at end of list
		return(insertAtEnd(pdata));
	}
	else if (*pos->pdata == *pdata)
	{
		// we found it, overwrite current data
		pos->pdata = pdata;
	}
	else
	{
		// allocate a new item
		PtrListItem<DataType> *pitem = 
			new PtrListItem<DataType>(pdata);
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
PtrList<DataType>::updateOrderedUnique(DataType *pdata)
{
	return(updateOrdered(pdata));
}

template <class DataType>
int
PtrList<DataType>::removeOrderedUnique(DataType *&pdata)
{
	return(removeOrdered(pdata));
}

// list operations by position
template <class DataType>
int
PtrList<DataType>::retrieveNth(int n, DataType *&pdata) const
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
	PtrListItem<DataType> *p;
	for (i = 1, p = first; p != NULL && i < n; p = p->next, i++) ;
	if (p != NULL && i == n)
	{
		pdata = p->pdata;
		return(OK);
	}
	else
		return(NOMATCH);
}

template <class DataType>
int
PtrList<DataType>::insertNth(int n, DataType *pdata)
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	// check parameters
	if (pdata == NULL)
		return(NOTOK);

	// check for out of range
	if (n < 1)
		return(NOMATCH);

	// find location to insert new tuple
	if (isEmpty())
	{
		// add at the front
		return(insertAtFront(pdata));
	}
	else if (n > count)
	{
		// add at the end
		return(insertAtEnd(pdata));
	}

	// search for correct place to insert new item
	int i;
	PtrListItem<DataType> *pos;
	for (i = 1, pos = first; pos != NULL && i < n; pos = pos->next, i++) ;

	// check which case
	if (pos != NULL && i == n)
	{
		// allocate a new item
		PtrListItem<DataType> *pitem = 
			new PtrListItem<DataType>(pdata);
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
PtrList<DataType>::updateNth(int n, DataType *pdata)
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
	PtrListItem<DataType> *p;
	for (i = 1, p = first; p != NULL && i < n; p = p->next, i++) ;
	if (p != NULL && i == n)
	{
		p->pdata = pdata;
		return(OK);
	}
	else
		return(NOTOK);
}

template <class DataType>
int
PtrList<DataType>::removeNth(int n, DataType *&pdata)
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
	PtrListItem<DataType> *pos;
	for (i = 1, pos = first; pos != NULL && i < n; pos = pos->next, i++) ;

	// was anything found
	if (pos == NULL || i != n)
		return(NOTOK);
	else
		pdata = pos->pdata;

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
PtrList<DataType>::includes(DataType *pdata) const
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));
	MustBeTrue(pdata != NULL);

	// search for data item
	PtrListItem<DataType> *p;
	for (p = first; p != NULL && *pdata != *p->pdata; p = p->next) ;
	if (p != NULL)
		return(1);
	else
		return(0);
}

template <class DataType>
void
PtrList<DataType>::clear()
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	for (PtrListItem<DataType> *pos = first; pos != NULL; )
	{
		PtrListItem<DataType> *save = pos->next;
		delete pos;
		pos = save;
	}
	first = last = NULL;
	return;
}

template <class DataType>
void
PtrList<DataType>::clearAndDelete()
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	for (PtrListItem<DataType> *pos = first; pos != NULL; )
	{
		PtrListItem<DataType> *save = pos->next;
		delete pos->pdata;
		delete pos;
		pos = save;
	}
	first = last = NULL;
	return;
}

template <class DataType>
int
PtrList<DataType>::isEmpty() const
{
	// invariant
	MustBeTrue((first != NULL || last == NULL) &&
		   (first == NULL || last != NULL));

	return(first == NULL && last == NULL);
}

// print list
template <class DataType>
ostream &
operator<<(ostream &os, const PtrList<DataType> &l)
{
	os << "{ ";
	for (PtrListItem<DataType> *pos = l.first; pos != NULL; pos = pos->next)
	{
		os << *pos << ",";
	}
	os << " }" << endl;
	return(os);
}

// list iterator constructors and destructor
template <class DataType>
PtrListIterator<DataType>::PtrListIterator(const PtrListIterator<DataType> &li): 
	list(li.list), current(li.current)
{
	// do nothing
}

template <class DataType>
PtrListIterator<DataType>::PtrListIterator(const PtrList<DataType> &l): 
	list(l), current(l.first)
{
	// do nothing
}

template <class DataType>
PtrListIterator<DataType>::~PtrListIterator()
{
	current = NULL;
}

// reset iterator to beginning
template <class DataType>
void
PtrListIterator<DataType>::reset()
{
	current = list.first;
	return;
}

// is list empty
template <class DataType>
int
PtrListIterator<DataType>::done() const
{
	return(current == NULL);
}

// return current data
template <class DataType>
DataType *
PtrListIterator<DataType>::operator()()
{
	MustBeTrue(current != NULL);
	MustBeTrue(current->pdata != NULL);
	return(current->pdata);
}

// increment to next data item in list
template <class DataType>
int
PtrListIterator<DataType>::operator++(int)
{
	if (current != NULL)
	{
		PtrListItem<DataType> *save = current;
		current = current->next;
		return(OK);
	}
	else
		return(NOTOK);
}

// list reverse iterator constructors and destructor
template <class DataType>
PtrListIterator_Reverse<DataType>::PtrListIterator_Reverse(
	const PtrListIterator_Reverse<DataType> &li): 
	list(li.list), current(li.current)
{
	// do nothing
}

template <class DataType>
PtrListIterator_Reverse<DataType>::PtrListIterator_Reverse(const PtrList<DataType> &l): 
	list(l), current(l.last)
{
	// do nothing
}

template <class DataType>
PtrListIterator_Reverse<DataType>::~PtrListIterator_Reverse()
{
	current = NULL;
}

// reset iterator to beginning
template <class DataType>
void
PtrListIterator_Reverse<DataType>::reset()
{
	current = list.last;
	return;
}

// is list empty
template <class DataType>
int
PtrListIterator_Reverse<DataType>::done() const
{
	return(current == NULL);
}

// return current data
template <class DataType>
DataType *
PtrListIterator_Reverse<DataType>::operator()()
{
	MustBeTrue(current != NULL);
	MustBeTrue(current->pdata != NULL);
	return(current->pdata);
}

// increment to next data item in list
template <class DataType>
int
PtrListIterator_Reverse<DataType>::operator++(int)
{
	if (current != NULL)
	{
		PtrListItem<DataType> *save = current;
		current = current->previous;
		return(OK);
	}
	else
		return(NOTOK);
}

