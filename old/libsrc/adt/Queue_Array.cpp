// member functions for queue class

// constructors and destructor
template <class DataType>
Queue_Array<DataType>::Queue_Array(int sz): 
	start(0), end(0), full(0), array(sz)
{
        // do nothing
}

template <class DataType>
Queue_Array<DataType>::Queue_Array(const Queue_Array<DataType> &q): 
	start(q.start), end(q.end), full(q.full), array(q.array)
{
        // do nothing
}

template <class DataType>
Queue_Array<DataType>::~Queue_Array()
{
        // do nothing
}

// assignment
template <class DataType>
Queue_Array<DataType> &
Queue_Array<DataType>::operator=(const Queue_Array<DataType> &q)
{
        if (this != &q)
	{
		start = q.start;
		end = q.end;
		full = q.full;
                array = q.array;
	}
        return(*this);
}

// queue operations
template <class DataType>
void 
Queue_Array<DataType>::clear()
{
	start = end = 0;
	full = 0;
        return;
}

template <class DataType>
int 
Queue_Array<DataType>::enqueue(const DataType &data) 
{
	// check if queue is full
	if (isFull())
		return(NOTOK);

	// add new item
	end = ++end%array.getSize();
	array[end] = data;
	if (start == end)
		full = 1;
        return(OK);
}

template <class DataType>
int 
Queue_Array<DataType>::dequeue(DataType &data) 
{
	// check if queue is empty
	if (isEmpty())
		return(NOTOK);

	// delete first item
	start = ++start%array.getSize();
	data = array[start];
	full = 0;
        return(OK);
}

template <class DataType>
int 
Queue_Array<DataType>::front(DataType &data) const
{
	// check if queue is empty
	if (isEmpty())
		return(NOTOK);

	// return first item
	data = array[(start+1)%array.getSize()];
        return(OK);
}

template <class DataType>
int 
Queue_Array<DataType>::isEmpty() const
{
        return(start == end && ! full);
}

template <class DataType>
int 
Queue_Array<DataType>::isFull() const
{
        return(start == end && full);
}

// print data
template <class DataType>
std::ostream &
Queue_Array<DataType>::dump(std::ostream &os) const
{
	if ( ! isEmpty())
	{
		for (int i = (start+1)%array.getSize(); 
		     i != end; 
		     i = ++i%array.getSize())
		{
			os << array[i] << " ";
		}
		os << array[end] << " ";
	}
        return(os);
}

