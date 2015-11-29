//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __PRIORITY_QUEUE_HEAP
#define __PRIORITY_QUEUE_HEAP
// priority queue class definition

// headers
#include <adt/AbsPriorityQueue.h>
#include <adt/Array.h>

namespace ombt {

// priority queue class
template <class DataType> class PriorityQueue_Array: 
	public AbstractPriorityQueue<DataType> {
public:
	enum { DefaultSize = 100 };

        // constructors and destructor
        PriorityQueue_Array(int=DefaultSize);
        PriorityQueue_Array(const PriorityQueue_Array &);
        ~PriorityQueue_Array();

        // assignment
        PriorityQueue_Array &operator=(const PriorityQueue_Array &);

        // priority queue operations
        void clear();
        int enqueue(const DataType &);
        int dequeue(DataType &);
        int front(DataType &) const;
        int isEmpty() const;

	// output data
	std::ostream &dump(std::ostream &) const;

protected:
	// internal use only
	inline int parentOf(int node) const {
		return(node/2);
	}
	inline int leftChildOf(int node) const {
		return(2*node);
	}
	inline int rightChildOf(int node) const {
		return(2*node+1);
	}

protected:
        // data
	int size, last;
        Array<DataType> array;
};

#include <adt/PriorityQueue_Array.cpp>

}

#endif
