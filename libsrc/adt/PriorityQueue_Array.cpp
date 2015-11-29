//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// member functions for priority queue class

// constructors and destructor
template <class DataType>
PriorityQueue_Array<DataType>::PriorityQueue_Array(int sz): 
	size(sz), last(0), array(1,sz)
{
        // do nothing
}

template <class DataType>
PriorityQueue_Array<DataType>::PriorityQueue_Array(
	const PriorityQueue_Array<DataType> &pq): 
		size(pq.size), last(pq.last), array(pq.array)
{
        // do nothing
}

template <class DataType>
PriorityQueue_Array<DataType>::~PriorityQueue_Array()
{
        // do nothing
}

// assignment
template <class DataType>
PriorityQueue_Array<DataType> &
PriorityQueue_Array<DataType>::operator=(const PriorityQueue_Array<DataType> &pq)
{
        if (this != &pq)
	{
                size = pq.size;
                last = pq.last;
                array = pq.array;
	}
        return(*this);
}

// queue operations
template <class DataType>
void 
PriorityQueue_Array<DataType>::clear()
{
	last = 0;
	return;
}

template <class DataType>
int 
PriorityQueue_Array<DataType>::enqueue(const DataType &data)
{
	// check if array is large enough
	if (last >= size)
	{
		size = 10*size;
		array.setBounds(1, size);
	}

	// insert new member in priority queue
	array[++last] = data;
	for (int i = last; 
	     i > 1 && (array[i] < array[parentOf(i)]); 
	     i = parentOf(i))
	{
		DataType tmp = array[i];
		array[i] = array[parentOf(i)];
		array[parentOf(i)] = tmp;
	}
	return(OK);
}

template <class DataType>
int 
PriorityQueue_Array<DataType>::dequeue(DataType &data)
{
	// is queue empty
	if (isEmpty())
		return(NOTOK);

	// get highest priority item from queue
	data = array[1];

	// delete item from queue
	array[1] = array[last--];

	// float to correct position
	for (int parent = 1; parent <= parentOf(last); )
	{
		// get left and right children
		int min = 0;
		int left = leftChildOf(parent);
		int right = rightChildOf(parent);

		// float to correct position
		if ((array[left] < array[right]) || (left == last))
			min = left;
		else
			min = right;
		if (array[parent] > array[min])
		{
			DataType tmp = array[parent];
			array[parent] = array[min];
			array[min] = tmp;
			parent = min;
		}
		else
			break;
	}
	return(OK);
}

template <class DataType>
int 
PriorityQueue_Array<DataType>::front(DataType &data) const
{
	// is queue empty
	if (isEmpty())
		return(NOTOK);

	// return first data item
	data = array[1];
	return(OK);
}

template <class DataType>
int 
PriorityQueue_Array<DataType>::isEmpty() const
{
	return(last <= 0);
}

// print data
template <class DataType>
std::ostream &
PriorityQueue_Array<DataType>::dump(std::ostream &os) const
{
	os << array;
        return(os);
}
