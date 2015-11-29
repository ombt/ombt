// member functions for list-based set class

// required headers
#include "set_List.h"

// set constructors and destructor
template <class DataType>
Set_List<DataType>::Set_List():
	list()
{
	// do nothing
}

template <class DataType>
Set_List<DataType>::Set_List(const Set_List<DataType> &sl):
	list(sl.list)
{
	// do nothing
}

template <class DataType>
Set_List<DataType>::~Set_List()
{
	// do nothing
}

// insert a new member operators
template <class DataType>
Set_List<DataType> &
Set_List<DataType>::operator+=(const DataType &m)
{
	return(insert(m));
}

template <class DataType>
Set_List<DataType> 
Set_List<DataType>::operator+(const DataType &m) const
{
	return(Set_List<DataType>(*this) += m);
}

template <class DataType>
Set_List<DataType> &
Set_List<DataType>::insert(const DataType &m)
{
	list.insertByValueUnique(m);
	return(*this);
}

// remove a member operators
template <class DataType>
Set_List<DataType> &
Set_List<DataType>::operator-=(DataType &m)
{
	return(remove(m));
}

template <class DataType>
Set_List<DataType> 
Set_List<DataType>::operator-(DataType &m) const
{
	return(Set_List<DataType>(*this) -= m);
}

template <class DataType>
Set_List<DataType> &
Set_List<DataType>::remove(DataType &m)
{
	list.removeByValue(m);
	return(*this);
}

template <class DataType>
Set_List<DataType> &
Set_List<DataType>::clear()
{
	list.clear();
	return(*this);
}

// equality and assignment set operators
template <class DataType>
Set_List<DataType> &
Set_List<DataType>::operator=(const Set_List<DataType> &sl)
{
	// check for self-assignment
	if (this != &sl)
		list = sl.list;
	return(*this);
}

template <class DataType>
int
Set_List<DataType>::operator==(const Set_List<DataType> &sl) const
{
	// check for self-comparison
	if (this == &sl)
		return(1);

	// compare individual elements
	ListIterator<DataType> pli1(list);
	ListIterator<DataType> pli2(sl.list);
	for ( ; ! pli1.done() && ! pli2.done(); pli1++, pli2++)
	{
		// do comparison
		if (pli1() != pli2())
			break;
	}
	if (pli1.done() && pli2.done())
		return(1);
	else
		return(0);
}

template <class DataType>
int
Set_List<DataType>::operator!=(const Set_List<DataType> &sl) const
{
	return(!(operator==(sl)));
}

template <class DataType>
int
Set_List<DataType>::isMember(const DataType &m) const
{
	return(list.isInList(m));
}

template <class DataType>
int
Set_List<DataType>::isEmpty() const
{
	return(list.isEmpty());
}

// union set operations
template <class DataType>
Set_List<DataType> &
Set_List<DataType>::operator|=(const Set_List<DataType> &sl)
{
	// insert new elements
	ListIterator<DataType> pli(sl.list);
	for ( ; ! pli.done(); pli++)
	{
		list.insertByValueUnique(pli());
	}
	return(*this);
}

template <class DataType>
Set_List<DataType>
Set_List<DataType>::operator|(const Set_List<DataType> &sl) const
{
	return(Set_List<DataType>(*this) |= sl);
}

// intersection set operations
template <class DataType>
Set_List<DataType> &
Set_List<DataType>::operator&=(const Set_List<DataType> &sl)
{
	// generate intersection
	List<DataType> pi;
	ListIterator<DataType> pli(sl.list);
	for ( ; ! pli.done(); pli++)
	{
		if (list.isInList(pli()))
		{
			pi.insertByValueUnique(pli());
		}
	}
	list = pi;
	return(*this);
}

template <class DataType>
Set_List<DataType>
Set_List<DataType>::operator&(const Set_List<DataType> &sl) const
{
	return(Set_List<DataType>(*this) &= sl);
}

// difference set operations
template <class DataType>
Set_List<DataType> &
Set_List<DataType>::operator-=(const Set_List<DataType> &sl)
{
	// generate difference
	ListIterator<DataType> pli(sl.list);
	for ( ; ! pli.done(); pli++)
	{
		DataType tmp = pli();
		(void)list.removeByValue(tmp);
	}
	return(*this);
}

template <class DataType>
Set_List<DataType>
Set_List<DataType>::operator-(const Set_List<DataType> &sl) const
{
	return(Set_List<DataType>(*this) -= sl);
}

// check if this set is a subset of another set.
template <class DataType>
int
Set_List<DataType>::isSubsetOf(const Set_List<DataType> &set) const
{
	// check if this set is a subset of another set
	ListIterator<DataType> subIter(list);
	ListIterator<DataType> setIter(set.list);
	while (!subIter.done() && !setIter.done())
	{
		if (subIter() == setIter())
		{
			// both are equal, increment both iterators
			setIter++;
			subIter++;
		}
		else if (subIter() > setIter())
		{
			// increment set to match sub.
			setIter++;
		}
		else
		{
			// a member was not found
			return(0);
		}
	}
	if (subIter.done())
		return(1);
	else
		return(0);
}

template <class DataType>
int
Set_List<DataType>::isSupersetOf(const Set_List<DataType> &super) const
{
	return(super.isSubsetOf(*this));
}

template <class DataType>
int
Set_List<DataType>::isProperSubsetOf(const Set_List<DataType> &set) const
{
	// check if this set is a subset of another set
	int subno = 0;
	int setno = 0;
	ListIterator<DataType> subIter(list);
	ListIterator<DataType> setIter(set.list);
	while (!subIter.done() && !setIter.done())
	{
		if (subIter() == setIter())
		{
			// both are equal, increment both iterators
			setno++;
			setIter++;
			subno++;
			subIter++;
		}
		else if (subIter() > setIter())
		{
			// increment set to match sub.
			setno++;
			setIter++;
		}
		else
		{
			// a member was not found
			return(0);
		}
	}
	if (!setIter.done())
	{
		setno++;
		setIter++;
	}
	if (subIter.done() && (subno < setno))
		return(1);
	else
		return(0);
}

template <class DataType>
int
Set_List<DataType>::isProperSupersetOf(const Set_List<DataType> &super) const
{
	return(super.isProperSubsetOf(*this));
}

// print contents of set
template <class DataType>
ostream &
operator<<(ostream &os, const Set_List<DataType> &sl)
{
	os << sl.list;
	return(os);
}

// set iterator constructors and destructor
template <class DataType>
Set_List_Iterator<DataType>::Set_List_Iterator(const Set_List<DataType> &sl):
	iterator(sl.list)
{
	// do nothing
}

template <class DataType>
Set_List_Iterator<DataType>::Set_List_Iterator(
	const Set_List_Iterator<DataType> &sliter):
	iterator(sliter.iterator)
{
	// do nothing
}

template <class DataType>
Set_List_Iterator<DataType>::~Set_List_Iterator()
{
	// do nothing
}

// reset iterator to start
template <class DataType>
void
Set_List_Iterator<DataType>::reset()
{
	iterator.reset();
	return;
}

// is iterator done ?
template <class DataType>
int
Set_List_Iterator<DataType>::done() const
{
	return(iterator.done());
}

// return current data
template <class DataType>
DataType
Set_List_Iterator<DataType>::operator()()
{
	return(iterator());
}

// increment to next data item in list
template <class DataType>
int
Set_List_Iterator<DataType>::operator++(int)
{
	return(iterator++);
}

