#ifndef __STACK_LIST_H
#define __STACK_LIST_H
// stack class definition

// headers
#include "absStack.h"
#include "mylist.h"

// abstract stack class
template <class DataType> class Stack_List:
	public AbstractStack<DataType> {
public:
        // destructor
        Stack_List();
        Stack_List(const Stack_List &);
        ~Stack_List();

	// assignment 
	Stack_List &operator=(const Stack_List &);

        // stack operations
        void clear();
        int push(const DataType &);
        int pop(DataType &);
        int top(DataType &) const;
        int isEmpty() const;

	// output data
	ostream &dump(ostream &) const;

protected:
	// data
	List<DataType> list;
};

#endif
