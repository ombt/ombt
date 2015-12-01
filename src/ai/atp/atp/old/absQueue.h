#ifndef __ABSTRACT_QUEUE_H
#define __ABSTRACT_QUEUE_H
// abstract queue class definition

// required headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"

#ifdef SUNCC
// forward definitions
template <class DataType> class AbstractQueue;

template <class DataType> 
ostream &
operator<<(ostream &, const AbstractQueue<DataType> &);

#endif
// abstract queue class
template <class DataType> class AbstractQueue {
public:
        // destructor
        virtual ~AbstractQueue() { }

        // queue operations
        virtual void clear() = 0;
        virtual int enqueue(const DataType &) = 0;
        virtual int dequeue(DataType &) = 0;
        virtual int front(DataType &) const = 0;
        virtual int isEmpty() const = 0;

	// output data
	virtual ostream &dump(ostream &) const = 0;
	friend ostream &operator<<(ostream &os, 
		const AbstractQueue<DataType> &q) {
		q.dump(os);
		return(os);
	}
};

#endif
