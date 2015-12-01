//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __PRIORITY_QUEUE_LIST
#define __PRIORITY_QUEUE_LIST
// priority queue class definition

// headers
#include "adt/AbsPriorityQueue.h"
#include "adt/BinaryTree_AVL.h"

namespace ombt {

// priority queue class
template <class DataType> class PriorityQueue_AVLTree: 
	public AbstractPriorityQueue<DataType> {
public:
        // constructors and destructor
        PriorityQueue_AVLTree();
        PriorityQueue_AVLTree(const PriorityQueue_AVLTree &);
        ~PriorityQueue_AVLTree();

        // assignment
        PriorityQueue_AVLTree &operator=(const PriorityQueue_AVLTree &);

        // priority queue operations
        void clear();
        int enqueue(const DataType &);
        int dequeue(DataType &);
        int front(DataType &) const;
        int isEmpty() const;

	// additional operations
	int includes(const DataType &) const;

	// output data
	std::ostream &dump(std::ostream &) const;

protected:
        // data
        BinaryTree_AVL<DataType> tree;
};

#include "adt/PriorityQueue_AVLTree.i"

}

#endif
