#ifndef __ABSTRACT_ITERATOR_H
#define __ABSTRACT_ITERATOR_H
// abstract iterator class definition

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"

// abstract iterator class
template <class DataType> class AbstractIterator {
public:
        // constructors and destructor
        virtual ~AbstractIterator() { }

	// reset iterator to start
	virtual void reset() = 0;

	// check if at end of list
	virtual int done() const = 0;

        // return data 
        virtual DataType operator()() = 0;

	// advance iterator to next link
	virtual int operator++(int) = 0;
};

#endif
