// member functions for priority queue class

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>

// local headers
#include "pqueue.h"

// constructors and destructor
PriorityQueue::PriorityQueue(int hsz): 
	heapSize(hsz), last(0), heap((DataItem **)0)
{
	// check heap size
	assert(0 < heapSize && 0 <= last && last <= heapSize);

	// allocate heap, skip entry 0
	heap = new DataItem *[heapSize+1];
	assert(heap != (DataItem **)0);

	// initialize array to null
	for (int ih = 1; ih <= heapSize; ih++)
	{
		heap[ih] = (DataItem *)0;
	}
}

PriorityQueue::PriorityQueue(const PriorityQueue &pq): 
	heapSize(pq.heapSize), last(pq.last), heap((DataItem **)0)
{
	// check heap size and number of items
	assert(0 < heapSize && 0 <= last && last <= heapSize);

	// allocate heap
	heap = new DataItem *[heapSize+1];
	assert(heap != (DataItem **)0);

	// initialize array
	for (int ih = 1; ih <= last; ih++)
	{
		heap[ih] = pq.heap[ih];
	}
	for ( ; ih <= heapSize; ih++)
	{
		heap[ih] = (DataItem *)0;
	}
}

PriorityQueue::~PriorityQueue() 
{
	if (heap != (DataItem **)0) delete [] heap;
	heap = (DataItem **)0;
	heapSize = 0;
	last = 0;
}

// assignment
PriorityQueue &
PriorityQueue::operator=(const PriorityQueue &pq) 
{
	// check for self-assignment
        if (this == &pq) return(*this);

	// delete old array
	if (heap != (DataItem **)0) delete [] heap;
	heap = (DataItem **)0;

	// store data
	heapSize = pq.heapSize;
	last = pq.last;
	assert(0 < heapSize && 0 <= last && last <= heapSize);

	// allocate a new heap
	heap = new DataItem *[heapSize+1];
	assert(heap != (DataItem **)0);

	// initialize array
	for (int ih = 1; ih <= last; ih++)
	{
		heap[ih] = pq.heap[ih];
	}
	for ( ; ih <= heapSize; ih++)
	{
		heap[ih] = (DataItem *)0;
	}

	// all done
        return(*this);
}

// heap operations
int 
makeNull(PriorityQueue &pq) 
{
	// no items in heap
	pq.last = 0;
        return(OK);

}

int 
insert(PriorityQueue &pq, DataItem *d) 
{
	// check if heap is full 
	if (pq.last >= pq.heapSize)
	{
		ERRORI("heap is full.", pq.heapSize);
		return(NOTOK);
	}

	// insert new data item into heap, and percolate.
	pq.heap[++pq.last] = d;
	int id = pq.last;
	while ((id > 1) && (*pq.heap[id] < *pq.heap[id/2]))
	{
		DataItem *temp = pq.heap[id];
		pq.heap[id] = pq.heap[id/2];
		pq.heap[id/2] = temp;
		id = id/2;
	}

	// all done
	return(OK);
}

int 
remove(PriorityQueue &pq, DataItem *&d) 
{
	// check if queue is empty
	if (pq.last == 0)
	{
		ERRORI("heap is empty.", pq.last);
		return(NOTOK);
	}

	// return data item at top of heap
	d = pq.heap[1];

	// restore heap structure
	pq.heap[1] = pq.heap[pq.last--];
	int jh;
	int ih = 1;
	while (ih <= pq.last/2)
	{
		if ((*pq.heap[2*ih] < *pq.heap[2*ih+1]) ||
		    (2*ih == pq.last))
			jh = 2*ih;
		else
			jh = 2*ih + 1;
		if (*pq.heap[ih] > *pq.heap[jh])
		{
			DataItem *temp = pq.heap[ih];
			pq.heap[ih] = pq.heap[jh];
			pq.heap[jh] = temp;
			ih = jh;
		}
		else
		{
			// done percolating
			break;
		}
	}

	// all done
        return(OK);
}

int
isEmpty(const PriorityQueue &pq) 
{
        return(pq.last == 0);
}

// print data
void 
PriorityQueue::dump(ostream &os) const
{
	os << "{ ";
	for (int ih = 1; ih <= last; ih++)
	{
		os << *heap[ih] << ",";
	}
	os << " }" << endl;
	return;
}

ostream &
operator<<(ostream &os, const PriorityQueue &pq) 
{
        pq.dump(os);
        return(os);
}
