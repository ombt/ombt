#ifndef __ITEM_H
#define __ITEM_H
/* definitions for simulation item base classes */

/* required headers */
#include "returns.h"

/* define classes */
class Item;
class Queue;
class Iterator;

/* type of item */
enum ItemType { CallerType, SenderType, ReceiverType, UnknownType };

/* constants */
#define MinQueuePriority -1

/* simulation item class */
class Item {
	friend class Queue;

public:
	/* constructor and destructor */
	Item();
	Item(ItemType, long = MinQueuePriority);
	virtual ~Item();

	/* initialization */
	int init(ItemType = UnknownType, long = MinQueuePriority);
	int deleteAll();

	/* handle event for item */
	virtual int process();

	/* other member functions */
	inline int getStatus() { return(status); }
	inline Item *getNext() { return(next); }
	inline Item *getPrevious() { return(previous); }
	inline long getPriority() { return(priority); }
	inline void setPriority(long newp) { priority = newp; }
	inline ItemType getType() { return(type); }

private:
	/* don't allow these */
	Item(const Item &);
	Item &operator=(const Item &);

public:
	/* internal data */
	int status;
	Item *next;
	Item *previous;
	long priority;
	ItemType type;
};

/* simulation queue class */
class Queue {
	friend class Iterator;

public:
	/* constructor and destructor */
	Queue();
	virtual ~Queue();

	/* initializations */
	int init();
	int deleteAll();

	/* queue interface */
	Item *enqueue(Item *);
	Item *dequeue();
	Item *remove(Item *);

	/* stack interface */
	Item *push(Item *);
	Item *pop();

	/* priority queue interface */
	Item *priorityEnq(Item *);
	Item *priorityDeq();

	/* other member functions */
	inline int getStatus() { return(status); }
	inline int isEmpty() { return(first == (Item *)0); }

private:
	/* don't allow these */
	Queue(const Queue &);
	Queue &operator=(const Queue &);

public:
	/* internal data */
	int status;
	Item *first;
	Item *last;
};
	
/* iteration class */
class Iterator {
public:
	/* constructor and destructor */
	Iterator();
	Iterator(const Iterator &);
	Iterator(const Queue &);
	virtual ~Iterator();

	/* initialization */
	int init();
	int init(const Iterator &);
	int init(const Queue &);
	int deleteAll();

	/* assignment */
	Iterator &operator=(const Queue &);
	Iterator &operator=(const Iterator &);

	/* return next link */
	Item *operator()();

	/* other member functions */
	inline int getStatus() { return(status); }

public:
	/* internal data */
	int status;
	Item *next;
};

#endif
