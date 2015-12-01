//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// member functions for queue class

// constructors and destructor
template <class DataType>
Queue_List<DataType>::Queue_List(): 
        list()
{
        // do nothing
}

template <class DataType>
Queue_List<DataType>::Queue_List(const Queue_List<DataType> &q): 
        list(q.list)
{
        // do nothing
}

template <class DataType>
Queue_List<DataType>::~Queue_List()
{
        // do nothing
}

// assignment
template <class DataType>
Queue_List<DataType> &
Queue_List<DataType>::operator=(const Queue_List<DataType> &q)
{
        if (this != &q)
                list = q.list;
        return(*this);
}

// queue operations
template <class DataType>
void 
Queue_List<DataType>::clear()
{
        list.clear();
        return;
}

template <class DataType>
int 
Queue_List<DataType>::enqueue(const DataType &data)
{
        return(list.insertAtEnd(data));
}

template <class DataType>
int 
Queue_List<DataType>::dequeue(DataType &data)
{
        return(list.removeAtFront(data));
}

template <class DataType>
int 
Queue_List<DataType>::front(DataType &data) const
{
        return(list.retrieveAtFront(data));
}

template <class DataType>
int 
Queue_List<DataType>::isEmpty() const
{
        return(list.isEmpty());
}

template <class DataType>
int 
Queue_List<DataType>::getCount() const
{
        return(list.getCount());
}

// print data
template <class DataType>
std::ostream &
Queue_List<DataType>::dump(std::ostream &os) const
{
        os << list;
        return(os);
}