#ifndef __PQUEUE_H
#define __PQUEUE_H
// priority queue class definition

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "data.h"

// forward declarations
class PriorityQueue;

// definitions
#define DEFAULTQUEUESIZE 255

// priority queue class
class PriorityQueue {
public:
        // constructors and destructor
        PriorityQueue(int = DEFAULTQUEUESIZE);
        PriorityQueue(const PriorityQueue &);
        virtual ~PriorityQueue();

        // assignment
        virtual PriorityQueue &operator=(const PriorityQueue &);

        // priority queue operations
        friend int makeNull(PriorityQueue &);
        friend int insert(PriorityQueue &, DataItem *);
        friend int remove(PriorityQueue &, DataItem *&);
        friend int isEmpty(const PriorityQueue &);

        // print data
        virtual void dump(ostream & = cout) const;
        friend ostream &operator<<(ostream &, const PriorityQueue &);

protected:
        // data
	int last;
	int heapSize;
        DataItem **heap;
};


#endif
