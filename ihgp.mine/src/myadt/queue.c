// member functions for queue class

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>

// local headers
#include "queue.h"

// constructors and destructor
Queue::Queue(): queue()
{
        // do nothing
}

Queue::Queue(const Queue &q): queue(q.queue)
{
        // do nothing
}

Queue::~Queue()
{
        // do nothing
}

// assignment
Queue &
Queue::operator=(const Queue &q)
{
        if (this != &q)
                queue = q.queue;
        return(*this);
}

// queue operations
int 
front(const Queue &q, DataItem *&d)
{
        return(retrieve(q.queue, d, STARTOFLIST));
}

int 
enqueue(Queue &q, DataItem *d)
{
        return(insert(q.queue, d, ENDOFLIST));
}

int 
dequeue(Queue &q, DataItem *&d)
{
        return(retrieveAndRemove(q.queue, d, STARTOFLIST));
}

int 
makeNull(Queue &q)
{
        return(makeNull(q.queue));
}

int 
isEmpty(const Queue &q)
{
        return(isEmpty(q.queue));
}

// print data
void 
Queue::dump(ostream &os) const
{
	queue.dump(os);
}

ostream &
operator<<(ostream &os, const Queue &q) 
{
        q.dump(os);
        return(os);
}

// constructors and destructor
QueueIterator::QueueIterator(): qiter() 
{
        // do nothing
}

QueueIterator::QueueIterator(const QueueIterator &qi): 
        qiter(qi.qiter) 
{
        // do nothing
}

QueueIterator::QueueIterator(const Queue &q): 
        qiter(q.queue) 
{
        // do nothing
}
        
QueueIterator::~QueueIterator() 
{
        // do nothing
}

// assignment
QueueIterator &
QueueIterator::operator=(const QueueIterator &qi) 
{
        if (this != &qi)
                qiter = qi.qiter;
        return(*this);
}

QueueIterator &
QueueIterator::operator=(const Queue &q) 
{
        qiter = q.queue;
        return(*this);
}

// iterator
DataItem *
QueueIterator::operator()() 
{
        return(qiter());
}
