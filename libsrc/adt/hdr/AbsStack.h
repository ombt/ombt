//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __OMBT_ABSTRACT_STACK_H
#define __OMBT_ABSTRACT_STACK_H
// abstract stack class definition

// required headers
#include <stdlib.h>
#include <iostream>

// local headers
#include "system/Returns.h"
#include "system/Debug.h"

namespace ombt {

// forward definitions
template <class DataType> class AbstractStack;

#ifdef USEOSTREAM
template <class DataType> 
std::ostream &
operator<<(std::ostream &, const AbstractStack<DataType> &);
#endif

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

#ifdef USEOSTREAM
	// output data
	virtual std::ostream &dump(std::ostream &) const = 0;
	template <typename DT> friend std::ostream &operator<<(std::ostream &, 
		const AbstractStack<DT> &);
#endif
};

#ifdef USEOSTREAM
// output function
template <class DataType> 
std::ostream &
operator<<(std::ostream &os, const AbstractStack<DataType> &s)
{
	s.dump(os);
	return(os);
}
#endif

}

#endif
