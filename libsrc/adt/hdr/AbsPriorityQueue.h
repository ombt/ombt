//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __OMBT_ABSTRACT_PRIORITY_QUEUE_H
#define __OMBT_ABSTRACT_PRIORITY_QUEUE_H
// abstract priority queue class definition

// required headers
#include <stdlib.h>
#include <iostream>

// local headers
#include "system/Returns.h"
#include "system/Debug.h"

namespace ombt {

// forward definitions
template <class DataType> class AbstractPriorityQueue;

#ifdef USEOSTREAM
template <class DataType> 
std::ostream &
operator<<(std::ostream &, const AbstractPriorityQueue<DataType> &);
#endif

// abstract priority queue class
template <class DataType> class AbstractPriorityQueue {
public:
        // destructor
        virtual ~AbstractPriorityQueue() { }

        // priority queue operations
        virtual void clear() = 0;
        virtual int enqueue(const DataType &) = 0;
        virtual int dequeue(DataType &) = 0;
        virtual int front(DataType &) const = 0;
        virtual int isEmpty() const = 0;

#ifdef USEOSTREAM
	// output data
	virtual std::ostream &dump(std::ostream &) const = 0;
	template <typename DT> friend std::ostream &operator<<(
		std::ostream &os, const AbstractPriorityQueue<DT> &abq) {
		abq.dump(os);
		return(os);
	}
#endif
};

}

#endif


