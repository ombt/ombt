#ifndef __OMBT_ABSTRACT_ITERATOR_H
#define __OMBT_ABSTRACT_ITERATOR_H
// abstract iterator class definition

// headers
#include <stdlib.h>
#include <iostream>

// local headers
#include <Returns.h>
#include <Debug.h>

namespace ombt {

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

#ifdef MUTABLE_ITERATOR
	// reset after data structure is altered
	virtual void resetByValue(const DataType &datum) {
#if 0
cerr << "resetByValue entry ... " << datum << endl;
#endif
		for ((*this).reset(); (*this)() != datum; (*this)++)
		{
#if 0
cerr << "resetByValue checking against ... " << (*this)() << endl;
#endif
		}
		return;
	}
#endif
};

}

#endif
