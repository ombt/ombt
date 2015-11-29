#ifndef __STACK_LIST_H
#define __STACK_LIST_H
// stack class definition

// headers
#include <adt/AbsStack.h>
#include <adt/List.h>

namespace ombt {

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
	std::ostream &dump(std::ostream &) const;

protected:
	// data
	List<DataType> list;
};

#include <adt/Stack_List.cpp>

}

#endif
