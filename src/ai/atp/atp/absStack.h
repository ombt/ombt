#ifndef __ABSTRACT_STACK_H
#define __ABSTRACT_STACK_H
// abstract stack class definition

// required headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"

// forward definitions
template <class DataType> class AbstractStack;

template <class DataType> 
ostream &
operator<<(ostream &, const AbstractStack<DataType> &);

// abstract stack class
template <class DataType> class AbstractStack {
public:
        // destructor
        virtual ~AbstractStack() { }

        // stack operations
        virtual void clear() = 0;
        virtual int push(const DataType &) = 0;
        virtual int pop(DataType &) = 0;
        virtual int top(DataType &) const = 0;
        virtual int isEmpty() const = 0;

	// output data
	virtual ostream &dump(ostream &) const = 0;
	friend ostream &operator<<(ostream &, 
		const AbstractStack<DataType> &);
};

// output function
template <class DataType> 
ostream &
operator<<(ostream &os, const AbstractStack<DataType> &s)
{
	s.dump(os);
	return(os);
}

#endif
