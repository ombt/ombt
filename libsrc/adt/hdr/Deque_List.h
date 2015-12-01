//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __DEQUE_LIST_H
#define __DEQUE_LIST_H
// stack class definition

// headers
#include "adt/AbsDeque.h"
#include "adt/List.h"

namespace ombt {

// abstract stack class
template <class DataType> class Deque_List:
	public AbstractDeque<DataType> {
public:
        // destructor
        Deque_List();
        Deque_List(const Deque_List &);
        ~Deque_List();

	// assignment 
	Deque_List &operator=(const Deque_List &);

        // stack operations
        int push(const DataType &);
        int pop(DataType &);
        int top(DataType &) const;

        // queue operations
        int enqueue(const DataType &);
        int dequeue(DataType &);
        int front(DataType &) const;

	// shared operations
        void clear();
        int isEmpty() const;

	// output data
	std::ostream &dump(std::ostream &) const;

protected:
	// data
	List<DataType> list;
};

#include "adt/Deque_List.i"

}

#endif

