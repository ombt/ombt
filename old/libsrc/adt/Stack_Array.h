#ifndef __STACK_ARRAY_H
#define __STACK_ARRAY_H
// stack class definition

// headers
#include <adt/AbsStack.h>
#include <adt/Array.h>

namespace ombt {

// abstract stack class
template <class DataType> class Stack_Array:
	public AbstractStack<DataType> {
public:
        // destructor
        Stack_Array(int);
        Stack_Array(const Stack_Array &);
        ~Stack_Array();

	// assignment 
	Stack_Array &operator=(const Stack_Array &);

        // queue operations
        void clear();
        int push(const DataType &);
        int pop(DataType &);
        int top(DataType &) const;
        int isEmpty() const;
        int isFull() const;

	// output data
	std::ostream &dump(std::ostream &) const;

protected:
	// data
	int first;
	Array<DataType> array;
};

#include <adt/Stack_Array.cpp>

}

#endif
