// member functions for hash table-based set class

// set constructors and destructor
template <class DataType>
Set_HashTable<DataType>::Set_HashTable(int tblsz, 
	int (*hfunc)(const DataType &)):
	table(tblsz, hfunc)
{
	// do nothing
}

template <class DataType>
Set_HashTable<DataType>::Set_HashTable(const Set_HashTable<DataType> &sl):
	table(sl.table)
{
	// do nothing
}

template <class DataType>
Set_HashTable<DataType>::~Set_HashTable()
{
	// do nothing
}

// insert a new member operators
template <class DataType>
Set_HashTable<DataType> &
Set_HashTable<DataType>::operator+=(const DataType &m)
{
	return(insert(m));
}

template <class DataType>
Set_HashTable<DataType> 
Set_HashTable<DataType>::operator+(const DataType &m) const
{
	return(Set_HashTable<DataType>(*this) += m);
}

template <class DataType>
Set_HashTable<DataType> &
Set_HashTable<DataType>::insert(const DataType &m)
{
	table.set_insert(m);
	return(*this);
}

// remove a member operators
template <class DataType>
Set_HashTable<DataType> &
Set_HashTable<DataType>::operator-=(DataType &m)
{
	return(remove(m));
}

template <class DataType>
Set_HashTable<DataType> 
Set_HashTable<DataType>::operator-(DataType &m) const
{
	return(Set_HashTable<DataType>(*this) -= m);
}

template <class DataType>
Set_HashTable<DataType> &
Set_HashTable<DataType>::remove(DataType &m)
{
	table.set_remove(m);
	return(*this);
}

template <class DataType>
Set_HashTable<DataType> &
Set_HashTable<DataType>::clear()
{
	table.clear();
	return(*this);
}

// equality and assignment set operators
template <class DataType>
Set_HashTable<DataType> &
Set_HashTable<DataType>::operator=(const Set_HashTable<DataType> &sl)
{
	// check for self-assignment
	if (this != &sl)
		table = sl.table;
	return(*this);
}

template <class DataType>
int
Set_HashTable<DataType>::operator==(const Set_HashTable<DataType> &sl) const
{
	// check for self-comparison
	if (this == &sl)
		return(1);

	// compare individual elements
	HashTable_List_Iterator<DataType> pli1(table);
	HashTable_List_Iterator<DataType> pli2(sl.table);
	for ( ; !pli1.done() && !pli2.done(); pli1++, pli2++)
	{
		// do comparison
		if (pli1() != pli2()) 
			break;
	}
	int status;
	if (pli1.done() && pli2.done())
		status = 1;
	else
		status = 0;

	// all done
	return(status);
}

template <class DataType>
int
Set_HashTable<DataType>::isMember(const DataType &m) const
{
	return(table.includes(m));
}

template <class DataType>
int
Set_HashTable<DataType>::isEmpty() const
{
	return(table.isEmpty());
}

// union set operations
template <class DataType>
Set_HashTable<DataType> &
Set_HashTable<DataType>::operator|=(const Set_HashTable<DataType> &sl)
{
	// insert new elements
	HashTable_List_Iterator<DataType> pli(sl.table);
	for ( ; !pli.done(); pli++)
	{
		table.set_insert(pli());
	}
	return(*this);
}

template <class DataType>
Set_HashTable<DataType>
Set_HashTable<DataType>::operator|(const Set_HashTable<DataType> &sl) const
{
	return(Set_HashTable<DataType>(*this) |= sl);
}

// intersection set operations
template <class DataType>
Set_HashTable<DataType> &
Set_HashTable<DataType>::operator&=(const Set_HashTable<DataType> &sl)
{
	// generate intersection
	HashTable_List<DataType> pi(sl.getSize(), sl.getHash());
	HashTable_List_Iterator<DataType> pli(sl.table);
	for ( ; ! pli.done(); pli++)
	{
		if (table.includes(pli()))
		{
			pi.set_insert(pli());
		}
	}
	table = pi;
	return(*this);
}

template <class DataType>
Set_HashTable<DataType>
Set_HashTable<DataType>::operator&(const Set_HashTable<DataType> &sl) const
{
	return(Set_HashTable<DataType>(*this) &= sl);
}

// difference set operations
template <class DataType>
Set_HashTable<DataType> &
Set_HashTable<DataType>::operator-=(const Set_HashTable<DataType> &sl)
{
	// generate difference
	HashTable_List_Iterator<DataType> pli(sl.table);
	for ( ; !pli.done(); pli++)
	{
		DataType tmp = pli();
		(void)table.set_remove(tmp);
	}
	return(*this);
}

template <class DataType>
Set_HashTable<DataType>
Set_HashTable<DataType>::operator-(const Set_HashTable<DataType> &sl) const
{
	return(Set_HashTable<DataType>(*this) -= sl);
}

// access some data
template <class DataType>
int
Set_HashTable<DataType>::getSize() const
{
	return(table.getSize());
}

template <class DataType>
int
(*Set_HashTable<DataType>::getHash() const)(const DataType &)
{
	return(table.getHash());
}

#if USEOSTREAM
// print contents of set
template <class DataType>
std::ostream &
operator<<(std::ostream &os, const Set_HashTable<DataType> &sl)
{
	os << sl.table;
	return(os);
}
#endif

// set iterator constructors and destructor
template <class DataType>
Set_HashTable_Iterator<DataType>::Set_HashTable_Iterator(const Set_HashTable<DataType> &sl):
	iterator(sl.table)
{
	// do nothing
}

template <class DataType>
Set_HashTable_Iterator<DataType>::Set_HashTable_Iterator(
	const Set_HashTable_Iterator<DataType> &sliter):
	iterator(sliter)
{
	// do nothing
}

template <class DataType>
Set_HashTable_Iterator<DataType>::~Set_HashTable_Iterator()
{
	// do nothing
}

// reset iterator to start
template <class DataType>
void
Set_HashTable_Iterator<DataType>::reset()
{
	iterator.reset();
	return;
}

// is iterator done ?
template <class DataType>
int
Set_HashTable_Iterator<DataType>::done() const
{
	return(iterator.done());
}

// return current data
template <class DataType>
DataType
Set_HashTable_Iterator<DataType>::operator()()
{
	return(iterator());
}

// increment to next data item in table
template <class DataType>
int
Set_HashTable_Iterator<DataType>::operator++(int)
{
	return(iterator++);
}

