// member functions for stack class

// constructors and destructor
template <class DataType>
Stack_Array<DataType>::Stack_Array(int sz): 
	first(sz), array(sz)
{
        // do nothing
}

template <class DataType>
Stack_Array<DataType>::Stack_Array(const Stack_Array<DataType> &s): 
	first(s.first), array(s.array)
{
        // do nothing
}

template <class DataType>
Stack_Array<DataType>::~Stack_Array()
{
        // do nothing
}

// assignment
template <class DataType>
Stack_Array<DataType> &
Stack_Array<DataType>::operator=(const Stack_Array<DataType> &s)
{
        if (this != &s)
	{
		first = s.first;
                array = s.array;
	}
        return(*this);
}

// stack operations
template <class DataType>
void 
Stack_Array<DataType>::clear()
{
	first = array.getSize();
        return;
}

template <class DataType>
int 
Stack_Array<DataType>::push(const DataType &data) 
{
	// is stack full
	if (isFull())
		return(NOTOK);

	// put new data in stack
	array[--first] = data;
	return(OK);
}

template <class DataType>
int 
Stack_Array<DataType>::pop(DataType &data) 
{
	// is stack empty
	if (isEmpty())
		return(NOTOK);

	// pop data from stack
	data = array[first++];
        return(OK);
}

template <class DataType>
int 
Stack_Array<DataType>::top(DataType &data) const
{
	// is stack empty
	if (isEmpty())
		return(NOTOK);

	// get data from stack
	data = array[first];
        return(OK);
}

template <class DataType>
int 
Stack_Array<DataType>::isEmpty() const
{
        return(first >= array.getSize());
}

template <class DataType>
int 
Stack_Array<DataType>::isFull() const
{
        return(first <= 0);
}

// print data
template <class DataType>
std::ostream &
Stack_Array<DataType>::dump(std::ostream &os) const
{
	for (int i = first; i < array.getSize(); i++)
	{
		os << array[i] << " ";
	}
        return(os);
}

