#ifndef __QUEUE_H
#define __QUEUE_H
// queue class definition

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "data.h"
#include "list.h"

// forward declarations
class Queue;
class QueueIterator;

// queue class
class Queue {
public:
        // friend classes
        friend class QueueIterator;

        // constructors and destructor
        Queue();
        Queue(const Queue &);
        virtual ~Queue();

        // assignment
        Queue &operator=(const Queue &);

        // queue operations
        friend int front(const Queue &, DataItem *&);
        friend int enqueue(Queue &, DataItem *);
        friend int dequeue(Queue &, DataItem *&);
        friend int makeNull(Queue &);
        friend int isEmpty(const Queue &);

        // print data
        virtual void dump(ostream & = cout) const;
        friend ostream &operator<<(ostream &, const Queue &);


protected:
        // data
        List queue;
};

// queue iterator class
class QueueIterator {
public:
        // constructors and destructor
        QueueIterator();
        QueueIterator(const QueueIterator &);
        QueueIterator(const Queue &);
        virtual ~QueueIterator();

        // assignment
        QueueIterator &operator=(const QueueIterator &);
        QueueIterator &operator=(const Queue &);

        // iterator
        DataItem *operator()();

protected:
        // data
        ListIterator qiter;
};

#endif
