//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __QUEUE_LIST_H
#define __QUEUE_LIST_H
// queue class definition

// headers
#include <adt/AbsQueue.h>
#include <adt/List.h>

namespace ombt {

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
	std::ostream &dump(std::ostream &) const;

protected:
	// data
	List<DataType> list;
};

#include <adt/Queue_List.cpp>
}


#endif
