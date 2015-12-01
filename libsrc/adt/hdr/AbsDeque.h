//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __OMBT_ABSTRACT_DEQUE_H
#define __OMBT_ABSTRACT_DEQUE_H
// abstract stack class definition

// required headers
#include <stdlib.h>
#include <iostream>

// local headers
#include "system/Returns.h"
#include "system/Debug.h"

namespace ombt {

// forward definitions
template <class DataType> class AbstractDeque;

#if USEOSTREAM
template <class DataType> 
std::ostream &
operator<<(std::ostream &, const AbstractDeque<DataType> &);
#endif

// abstract stack class
template <class DataType> class AbstractDeque {
public:
        // destructor
        virtual ~AbstractDeque() { }

        // stack operations
        virtual int push(const DataType &) = 0;
        virtual int pop(DataType &) = 0;
        virtual int top(DataType &) const = 0;

        // queue operations
        virtual int enqueue(const DataType &) = 0;
        virtual int dequeue(DataType &) = 0;
        virtual int front(DataType &) const = 0;

	// shared operations
        virtual void clear() = 0;
        virtual int isEmpty() const = 0;

#if USEOSTREAM
	// output data
	virtual std::ostream &dump(std::ostream &) const = 0;
	template <typename DT> friend std::ostream &operator<<(std::ostream &, 
		const AbstractDeque<DT> &);
#endif
};

#if USEOSTREAM
// output function
template <class DataType> 
std::ostream &
operator<<(std::ostream &os, const AbstractDeque<DataType> &s)
{
	s.dump(os);
	return(os);
}
#endif

}

#endif

