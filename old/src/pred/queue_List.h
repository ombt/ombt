#ifndef __QUEUE_LIST_H
#define __QUEUE_LIST_H
// queue class definition

// headers
#include "absQueue.h"
#include "list.h"

// abstract stack class
template <class DataType> class Queue_List:
	public AbstractQueue<DataType> {
public:
        // destructor
        Queue_List();
        Queue_List(const Queue_List &);
        ~Queue_List();

	// assignment 
	Queue_List &operator=(const Queue_List &);

        // queue operations
        void clear();
        int enqueue(const DataType &);
        int dequeue(DataType &);
        int front(DataType &) const;
        int isEmpty() const;

	// output data
	ostream &dump(ostream &) const;

protected:
	// data
	List<DataType> list;
};

#endif
