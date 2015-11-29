// member functions for hash class

// hash table constructors and destructor
template <class DataType>
HashTable_List<DataType>::HashTable_List(
	int sz,  int (*hfn)(const DataType &)):
	size(sz), hash(hfn), array(sz)
{
	// do nothing
}

template <class DataType>
HashTable_List<DataType>::HashTable_List(
	const HashTable_List<DataType> &ht):
	size(ht.size), hash(ht.hash), array(ht.array)
{
	// do nothing
}

template <class DataType>
HashTable_List<DataType>::~HashTable_List()
{
	// do nothing
}

// assignment
template <class DataType>
HashTable_List<DataType> &
HashTable_List<DataType>::operator=(
	const HashTable_List<DataType> &ht)
{
	// check for self-assignment
	if (this != &ht)
	{
		// copy data
		size = ht.size;
		hash = ht.hash;
		array = ht.array;
	}

	// all done
	return(*this);
}


// hash table operations
template <class DataType>
void
HashTable_List<DataType>::clear()
{
	// initialize table entries 
	for (int ib = 0; ib < size; ib++)
	{
		array[ib].clear();
	}
	return;
}

template <class DataType>
int
HashTable_List<DataType>::insert(const DataType &d)
{
	// get bucket for data item
	int ib = hash(d)%size;

	// insert into bucket
	return(array[ib].insertAtEnd(d));
}

template <class DataType>
int
HashTable_List<DataType>::remove(DataType &d)
{
	// get bucket for data item
	int ib = hash(d)%size;

	// remove from bucket
	return(array[ib].remove(d));
	
}

template <class DataType>
int
HashTable_List<DataType>::retrieve(DataType &d) const
{
	// get bucket for data item
	int ib = hash(d)%size;

	// retrieve from bucket
	return(array[ib].retrieve(d));
}

template <class DataType>
int
HashTable_List<DataType>::includes(const DataType &d) const
{
	// get bucket for data item
	int ib = hash(d)%size;

	// check if in bucket
	return(array[ib].includes(d));
}

template <class DataType>
int
HashTable_List<DataType>::isEmpty() const
{
	// check if all lists are empty
	for (int ib = 0; ib < size; ib++)
	{
		if ( ! array[ib].isEmpty()) 
			return(0);
	}
	return(1);
}

// hash table operations for sets, no duplicates allowed
template <class DataType>
int
HashTable_List<DataType>::set_insert(const DataType &d)
{
	// get bucket for data item
	int ib = hash(d)%size;

	// insert into bucket
	return(array[ib].insertUnique(d));
}

template <class DataType>
int
HashTable_List<DataType>::set_remove(DataType &d)
{
	// get bucket for data item
	int ib = hash(d)%size;

	// remove from bucket
	return(array[ib].removeUnique(d));
	
}

template <class DataType>
int
HashTable_List<DataType>::set_retrieve(DataType &d) const
{
	// get bucket for data item
	int ib = hash(d)%size;

	// retrieve from bucket
	return(array[ib].retrieveUnique(d));
}

// access functions
template <class DataType>
int
HashTable_List<DataType>::getSize() const
{
	return(size);
}

template <class DataType>
int
(*HashTable_List<DataType>::getHash() const)(const DataType &)
{
	return(hash);
}

// print table
template <class DataType>
std::ostream &
HashTable_List<DataType>::dump(std::ostream &os) const
{
	os << "hash table = {" << std::endl;
	for (int ib = 0; ib < size; ib++)
	{
	        os << "bucket[" << ib << "] = " << array[ib] << std::endl;
	}
	os << "}" << std::endl;
	return(os);
}

// hash table iterator constructors and destructor
template <class DataType>
HashTable_List_Iterator<DataType>::HashTable_List_Iterator(
	const HashTable_List_Iterator<DataType> &htli): 
	table(htli.table), listIter(NULL), current(htli.current)
{
	if (current < table.size)
	{
		listIter = new ListIterator<DataType>(*htli.listIter);
		MustBeTrue(listIter != NULL);
	}
}

template <class DataType>
HashTable_List_Iterator<DataType>::HashTable_List_Iterator(
	const HashTable_List<DataType> &htl): 
	table(htl), listIter(NULL), current(0)
{
	// find a non-empty list
	for ( ; 
	     current < table.size && table.array[current].isEmpty(); 
	     current++) ;

	// create a list iterator for first non-empty list
	if (current < table.size)
	{
		listIter = new ListIterator<DataType>(table.array[current]);
		MustBeTrue(listIter != NULL);
	}
}

template <class DataType>
HashTable_List_Iterator<DataType>::~HashTable_List_Iterator()
{
	if (listIter != NULL)
	{
		delete listIter;
		listIter = NULL;
	}
}

// reset iterator to beginning
template <class DataType>
void
HashTable_List_Iterator<DataType>::reset()
{
	// delete old iterator
	if (listIter != NULL)
	{
		delete listIter;
		listIter = NULL;
	}

	// find first non-empty list
	for (current = 0; 
	     current < table.size && table.array[current].isEmpty(); 
	     current++) ;

	// create a list iterator for non-empty list
	if (current < table.size)
	{
		listIter = new ListIterator<DataType>(table.array[current]);
		MustBeTrue(listIter != NULL);
	}

	// all done
	return;
}

// is list empty
template <class DataType>
int
HashTable_List_Iterator<DataType>::done() const
{
	return(current >= table.size);
}

// return current data
template <class DataType>
DataType
HashTable_List_Iterator<DataType>::operator()()
{
	MustBeTrue(listIter != NULL && ! (listIter->done()));
	return((*listIter)());
}

// increment to next data item in list
template <class DataType>
int
HashTable_List_Iterator<DataType>::operator++(int)
{
	// check if already done
	if (current >= table.size)
		return(NOTOK);

	// not done, make sure
	MustBeTrue(listIter != NULL);

	// check if list is done
	(*listIter)++;
	if ( ! (listIter->done()))
		return(OK);

	// delete old iterator
	if (listIter != NULL)
	{
		delete listIter;
		listIter = NULL;
	}

	// find a non-empty list
	for (current++;
	     current < table.size && table.array[current].isEmpty(); 
	     current++) ;

	// create a list iterator for non-empty list
	if (current < table.size)
	{
		listIter = new ListIterator<DataType>(table.array[current]);
		MustBeTrue(listIter != NULL);
		return(OK);
	}
	else
		return(NOTOK);
}

