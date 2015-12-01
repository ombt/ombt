//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __QUEUE_ARRAY_H
#define __QUEUE_ARRAY_H
// queue class definition

// headers
#include "adt/AbsQueue.h"
#include "adt/Array.h"

namespace ombt {

// abstract stack class
template <class DataType> class Queue_Array:
	public AbstractQueue<DataType> {
public:
        // destructor
        Queue_Array(int);
        Queue_Array(const Queue_Array &);
        ~Queue_Array();

	// assignment 
	Queue_Array &operator=(const Queue_Array &);

        // queue operations
        void clear();
        int enqueue(const DataType &);
        int dequeue(DataType &);
        int front(DataType &) const;
        int isEmpty() const;
        int isFull() const;

	// output data
	std::ostream &dump(std::ostream &) const;

protected:
	// data
	int start, end, full;
	Array<DataType> array;
};

#include "adt/Queue_Array.i"

}

#endif

