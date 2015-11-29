// member functions for hash class

// hash table constructors and destructor
template <class DataType>
HashTable_Array<DataType>::HashTable_Array(
	int sz,  int (*hfn)(const DataType &)):
	size(sz), hash(hfn), status(sz), array(sz) 
{
	// do nothing
}

template <class DataType>
HashTable_Array<DataType>::HashTable_Array(
	const HashTable_Array<DataType> &ht):
	size(ht.size), hash(ht.hash), status(ht.status), array(ht.array)
{
	// do nothing
}

template <class DataType>
HashTable_Array<DataType>::~HashTable_Array()
{
	// do nothing
}

// assignment
template <class DataType>
HashTable_Array<DataType> &
HashTable_Array<DataType>::operator=(const HashTable_Array<DataType> &ht)
{
	// check for self-assignment
	if (this != &ht)
	{
		// copy data
		size = ht.size;
		hash = ht.hash;
		status = ht.status;
		array = ht.array;
	}

	// all done
	return(*this);
}


// hash table operations
template <class DataType>
void
HashTable_Array<DataType>::clear()
{
	// initialize table entries 
	for (int ib = 0; ib < size; ib++)
	{
		status[ib].status = HashTableBucketStatus::Empty;
	}
	return;
}

template <class DataType>
int
HashTable_Array<DataType>::insert(const DataType &d)
{
	// get bucket for data item
	int ib = hash(d)%size;

	// insert into bucket
	if (status[ib].status != HashTableBucketStatus::Used)
	{
		// bucket is empty, use it.
		status[ib].status = HashTableBucketStatus::Used;
		array[ib] = d;
	}
	else if (array[ib] == d)
	{
		// overwrite existing data
		array[ib] = d;
	}
	else
	{
		// bucket is in use. use linear probe.
		int oldib = ib;
		for (ib = (ib+1)%size; ib != oldib; ib = (ib+1)%size)
		{
			if (status[ib].status != HashTableBucketStatus::Used)
			{
				// found an empty bucket
				status[ib].status = HashTableBucketStatus::Used;
				array[ib] = d;
				return(OK);
			}
			else if (array[ib] == d)
			{
				// overwrite existing data
				array[ib] = d;
				return(OK);
			}
		}

		// hash table is completely full, return an error
		return(NOTOK);
	}
	return(OK);
}

template <class DataType>
int
HashTable_Array<DataType>::remove(DataType &d)
{
	// get bucket for data item
	int ib = hash(d)%size;

	// remove data
	if (array[ib] == d)
	{
		// found it
		d = array[ib];
		status[ib].status = HashTableBucketStatus::Deleted;
		return(OK);
	}
	else
	{
		// not found. use linear probe.
		int oldib = ib;
		for (ib = (ib+1)%size; ib != oldib; ib = (ib+1)%size)
		{
			if (status[ib].status == HashTableBucketStatus::Empty)
				break;
			if ((status[ib].status == HashTableBucketStatus::Used) 
			     && (array[ib] == d))
			{
				// found it
				d = array[ib];
				status[ib].status = 
					HashTableBucketStatus::Deleted;
				return(OK);
			}
		}

		// not found
		return(NOMATCH);
	}
}

template <class DataType>
int
HashTable_Array<DataType>::retrieve(DataType &d) const
{
	// get bucket for data item
	int ib = hash(d)%size;

	// remove data
	if (array[ib] == d)
	{
		// found it
		d = array[ib];
		return(OK);
	}
	else
	{
		// not found, use linear probe.
		int oldib = ib;
		for (ib = (ib+1)%size; ib != oldib; ib = (ib+1)%size)
		{
			if (status[ib].status == HashTableBucketStatus::Empty)
				break;
			if ((status[ib].status == HashTableBucketStatus::Used)
			    && (array[ib] == d))
			{
				// found it
				d = array[ib];
				return(OK);
			}
		}

		// not found
		return(NOMATCH);
	}
}

template <class DataType>
int
HashTable_Array<DataType>::includes(const DataType &d) const
{
	// get bucket for data item
	int ib = hash(d)%size;

	// check for data
	if (array[ib] == d)
	{
		// found it
		return(1);
	}
	else
	{
		// not found, use linear probe.
		int oldib = ib;
		for (ib = (ib+1)%size; ib != oldib; ib = (ib+1)%size)
		{
			if (status[ib].status == HashTableBucketStatus::Empty)
				break;
			if ((status[ib].status == HashTableBucketStatus::Used)
			    && (array[ib] == d))
			{
				// found it
				return(1);
			}
		}

		// not found
		return(0);
	}
}

template <class DataType>
int
HashTable_Array<DataType>::isEmpty() const
{
	// check if all buckets are empty
	for (int ib = 0; ib < size; ib++)
	{
		if (status[ib].status == HashTableBucketStatus::Used)
			return(0);
	}
	return(1);
}

// print table
template <class DataType>
std::ostream &
HashTable_Array<DataType>::dump(std::ostream &os) const
{
	os << "hash table = {" << std::endl;
	for (int ib = 0; ib < size; ib++)
	{
		if (status[ib].status == HashTableBucketStatus::Used)
	        	os << "bucket[" << ib << "] = " << array[ib] << std::endl;
	}
	os << "}" << std::endl;
	return(os);
}

// hash table iterator constructors and destructor
template <class DataType>
HashTable_Array_Iterator<DataType>::HashTable_Array_Iterator(
	const HashTable_Array_Iterator<DataType> &htai): 
	table(htai.table), current(htai.current)
{
	// do nothing
}

template <class DataType>
HashTable_Array_Iterator<DataType>::HashTable_Array_Iterator(
	const HashTable_Array<DataType> &hta): 
	table(hta), current(0)
{
	// find first non-empty slot
	for (current = 0;
	     current < table.size &&
	     table.status[current].status != HashTableBucketStatus::Used;
	     current++) ;
}

template <class DataType>
HashTable_Array_Iterator<DataType>::~HashTable_Array_Iterator()
{
	// do nothing
}

// reset iterator to beginning
template <class DataType>
void
HashTable_Array_Iterator<DataType>::reset()
{
	// find first non-empty slot
	for (current = 0;
	     current < table.size &&
	     table.status[current].status != HashTableBucketStatus::Used;
	     current++) ;
	return;
}

// is list empty
template <class DataType>
int
HashTable_Array_Iterator<DataType>::done() const
{
	return(current >= table.size);
}

// return current data
template <class DataType>
DataType
HashTable_Array_Iterator<DataType>::operator()()
{
	return(table.array[current]);
}

// increment to next data item in list
template <class DataType>
int
HashTable_Array_Iterator<DataType>::operator++(int)
{
	for (current++;
	     current < table.size &&
	     table.status[current].status != HashTableBucketStatus::Used;
	     current++) ;
	if (current < table.size)
		return(OK);
	else
		return(NOTOK);
}

