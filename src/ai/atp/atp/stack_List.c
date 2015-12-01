// member functions for stack class

// headers
#include "stack_List.h"

// constructors and destructor
template <class DataType>
Stack_List<DataType>::Stack_List(): 
	list()
{
        // do nothing
}

template <class DataType>
Stack_List<DataType>::Stack_List(const Stack_List<DataType> &s): 
	list(s.list)
{
        // do nothing
}

template <class DataType>
Stack_List<DataType>::~Stack_List()
{
        // do nothing
}

// assignment
template <class DataType>
Stack_List<DataType> &
Stack_List<DataType>::operator=(const Stack_List<DataType> &s)
{
        if (this != &s)
                list = s.list;
        return(*this);
}

// stack operations
template <class DataType>
void 
Stack_List<DataType>::clear()
{
        list.clear();
        return;
}

template <class DataType>
int 
Stack_List<DataType>::push(const DataType &data) 
{
        return(list.insertAtFront(data));
}

template <class DataType>
int 
Stack_List<DataType>::pop(DataType &data) 
{
        return(list.removeAtFront(data));
}

template <class DataType>
int 
Stack_List<DataType>::top(DataType &data) const
{
        return(list.retrieveAtFront(data));
}

template <class DataType>
int 
Stack_List<DataType>::isEmpty() const
{
        return(list.isEmpty());
}

// print data
template <class DataType>
ostream &
Stack_List<DataType>::dump(ostream &os) const
{
        os << list;
        return(os);
}

