//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// member functions for priority queue class

// constructors and destructor
template <class DataType>
PriorityQueue_List<DataType>::PriorityQueue_List(): 
	list()
{
        // do nothing
}

template <class DataType>
PriorityQueue_List<DataType>::PriorityQueue_List(
	const PriorityQueue_List<DataType> &pq): 
		list(pq.list)
{
        // do nothing
}

template <class DataType>
PriorityQueue_List<DataType>::~PriorityQueue_List()
{
        // do nothing
}

// assignment
template <class DataType>
PriorityQueue_List<DataType> &
PriorityQueue_List<DataType>::operator=(const PriorityQueue_List<DataType> &pq)
{
        if (this != &pq)
                list = pq.list;
        return(*this);
}

// queue operations
template <class DataType>
void 
PriorityQueue_List<DataType>::clear()
{
        list.clear();
        return;
}

template <class DataType>
int 
PriorityQueue_List<DataType>::enqueue(const DataType &data)
{
        return(list.insertOrdered(data));
}

template <class DataType>
int 
PriorityQueue_List<DataType>::dequeue(DataType &data)
{
        return(list.removeAtFront(data));
}

template <class DataType>
int 
PriorityQueue_List<DataType>::front(DataType &data) const
{
        return(list.retrieveAtFront(data));
}

template <class DataType>
int 
PriorityQueue_List<DataType>::isEmpty() const
{
        return(list.isEmpty());
}

// print data
template <class DataType>
std::ostream &
PriorityQueue_List<DataType>::dump(std::ostream &os) const
{
	os << list;
        return(os);
}

