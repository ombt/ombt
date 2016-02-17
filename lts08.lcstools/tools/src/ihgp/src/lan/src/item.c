/* functions for a queue class */

/* unix headers */
#include <sysent.h>
#include <stdio.h>
#include <stdlib.h>

/* other headers */
#include "item.h"

/* queue item constructors and destructor */
Item::Item()
{
	init();
}

Item::Item(ItemType mytype, long mypriority)
{
	init(mytype, mypriority);
}

Item::~Item()
{
	deleteAll();
}

/* queue item initialization routines */
int
Item::init(ItemType mytype, long mypriority)
{
	/* initialize linked list pointers and priority */
	next = previous = (Item *)0;
	priority = mypriority;
	type = mytype;
	status = OK;
	return(OK);
}

int
Item::deleteAll()
{
	/* remove node from linked list */
	if (next != (Item *)0) next->previous = previous;
	if (previous != (Item *)0) previous->next = next;
	next = (Item *)0;
	previous = (Item *)0;
	priority = MinQueuePriority;
	status = OK;
	return(OK);
}

/* queue constructors and destructor */
Queue::Queue()
{
	init();
}

Queue::~Queue()
{
	deleteAll();
}

/* initialization functions */
int
Queue::init()
{
	/* initialize head and tail pointers */
	first = last = (Item *)0;
	status = OK;
	return(OK);
}

int
Queue::deleteAll()
{
	Item *item, *save;

	/* delete everything in linked list */
	for (item = first; item != (Item *)0; )
	{
		save = item->next;
		delete item;
		item = save;
	}
	first = last = (Item *)0;
	status = OK;
	return(OK);
}

/* queue interface */
Item *
Queue::enqueue(Item *newItem)
{
	/* put new item at end of list */
	if (last != (Item *)0)
	{
		last->next = newItem;
		newItem->previous = last;
	}
	else
	{
		first = newItem;
	}
	last = newItem;
	status = OK;
	return(newItem);
}

Item *
Queue::dequeue()
{
	return(remove(first));
}

Item *
Queue::remove(Item *oldItem)
{
	/* remove given item from link list */
	if (oldItem == first) first = first->next;
	if (oldItem == last) last = last->previous;
	if (oldItem->next != (Item *)0)
	{
		oldItem->next->previous = oldItem->previous;
	}
	if (oldItem->previous != (Item *)0)
	{
		oldItem->previous->next = oldItem->next;
	}
	oldItem->next = (Item *)0;
	oldItem->previous = (Item *)0;
	status = OK;
	return(oldItem);
}

/* stack interface */
Item *
Queue::push(Item *newItem)
{
	/* put new item at beginning of list */
	if (first != (Item *)0)
	{
		first->previous = newItem;
		newItem->next = first;
	}
	else
	{
		last = newItem;
	}
	first = newItem;
	status = OK;
	return(newItem);
}

Item *
Queue::pop()
{
	return(remove(first));
}

/* priority queue interface */
Item *
Queue::priorityEnq(Item *newItem)
{
	Item *item;

	/* find where in list the new item belongs */
	for (item = first; item != (Item *)0; item = item->next)
	{
		/* compare priorities */
		if (item->priority > newItem->priority) break;
	}

	/* check if item is null */
	if (item == (Item *)0)
	{
		if (first == (Item *)0)
		{
			/* this is the first item in list */
			first = newItem;
			last = newItem;
		}
		else
		{
			/* new item belongs at the end of the list */
			last->next = newItem;
			newItem->previous = last;
			last = newItem;
		}
		status = OK;
		return(OK);
	}

	/* put new item in list */
	if (item->previous == (Item *)0)
	{
		/* item belongs at front of list */
		first->previous = newItem;
		newItem->next = first;
		first = newItem;
	}
	else
	{
		/* put new item in middle of list */
		newItem->previous = item->previous;
		newItem->next = item;
		item->previous->next = newItem;
		item->previous = newItem;
		
	}
	status = OK;
	return(OK);
}

Item *
Queue::priorityDeq()
{
	return(remove(first));
}

/* queue iterator constructors and destructor */
Iterator::Iterator()
{
	init();
}

Iterator::Iterator(const Iterator &src)
{
	init(src);
}

Iterator::Iterator(const Queue &src)
{
	init(src);
}

Iterator::~Iterator()
{
	deleteAll();
}

/* initialization routines */
int
Iterator::init()
{
	next = (Item *)0;
	status = OK;
	return(OK);
}

int
Iterator::init(const Iterator &src)
{
	next = src.next;
	status = OK;
	return(OK);
}

int
Iterator::init(const Queue &src)
{
	next = src.first;
	status = OK;
	return(OK);
}

int
Iterator::deleteAll()
{
	next = (Item *)0;
	status = OK;
	return(OK);
}

/* assignment functions */
Iterator &
Iterator::operator=(const Queue &src)
{
	init(src);
	return(*this);
}

Iterator &
Iterator::operator=(const Iterator &src)
{
	if (this == &src)
		status = OK;
	else
		init(src);
	return(*this);
}

/* iterator function */
Item *
Iterator::operator()()
{
	Item *save = next;
	if (save != (Item *)0)
	{
		next = next->getNext();
	}
	status = OK;
	return(save);
}

/* handle event for item */
int
Item::process()
{
	/* do nothing */
	status = OK;
	return(OK);
}
