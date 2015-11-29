//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// member functions for deque class

// constructors and destructor
template <class DataType>
Deque_List<DataType>::Deque_List(): 
	list()
{
        // do nothing
}

template <class DataType>
Deque_List<DataType>::Deque_List(const Deque_List<DataType> &d): 
	list(d.list)
{
        // do nothing
}

template <class DataType>
Deque_List<DataType>::~Deque_List()
{
        // do nothing
}

// assignment
template <class DataType>
Deque_List<DataType> &
Deque_List<DataType>::operator=(const Deque_List<DataType> &d)
{
        if (this != &d)
                list = d.list;
        return(*this);
}

// stack operations
template <class DataType>
void 
Deque_List<DataType>::clear()
{
        list.clear();
        return;
}

template <class DataType>
int 
Deque_List<DataType>::push(const DataType &data) 
{
        return(list.insertAtFront(data));
}

template <class DataType>
int 
Deque_List<DataType>::pop(DataType &data) 
{
        return(list.removeAtFront(data));
}

template <class DataType>
int 
Deque_List<DataType>::top(DataType &data) const
{
        return(list.retrieveAtFront(data));
}

template <class DataType>
int 
Deque_List<DataType>::isEmpty() const
{
        return(list.isEmpty());
}

// print data
template <class DataType>
std::ostream &
Deque_List<DataType>::dump(std::ostream &os) const
{
        os << list;
        return(os);
}

// queue operations
template <class DataType>
int 
Deque_List<DataType>::enqueue(const DataType &data)
{
        return(list.insertAtEnd(data));
}

template <class DataType>
int 
Deque_List<DataType>::dequeue(DataType &data)
{
        return(list.removeAtFront(data));
}

template <class DataType>
int 
Deque_List<DataType>::front(DataType &data) const
{
        return(list.retrieveAtFront(data));
}
