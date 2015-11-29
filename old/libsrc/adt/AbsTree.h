#ifndef __ABSTRACT_TREE_H
#define __ABSTRACT_TREE_H
// abstract tree class definition

// required headers
#include <stdlib.h>
#include <iostream>

// local headers
#include <Returns.h>
#include <Debug.h>

namespace ombt {

// forward definitions
template <class DataType> class AbstractTree;

#ifdef USEOSTREAM
template <class DataType> 
std::ostream &
operator<<(std::ostream &, const AbstractTree<DataType> &);
#endif

// abstract tree class
template <class DataType> class AbstractTree {
public:
        // destructor
        virtual ~AbstractTree() { }

        // binary tree operations
        virtual int insert(const DataType &) = 0;
        virtual int remove(DataType &) = 0;
        virtual int retrieve(DataType &) const = 0;
        virtual int update(const DataType &) = 0;
        virtual int includes(const DataType &) const = 0;
	virtual int isEmpty() const = 0;
        virtual void clear() = 0;

	// tree traversal functions
	virtual int preOrder(int (*)(DataType &)) = 0;
	virtual int inOrder(int (*)(DataType &)) = 0;
	virtual int postOrder(int (*)(DataType &)) = 0;
	virtual int levelOrder(int (*)(DataType &)) = 0;

#ifdef USEOSTREAM
	// output data
	virtual std::ostream &dump(std::ostream &) const = 0;
	friend std::ostream &operator<<(std::ostream &os,
		const AbstractTree<DataType> &bt) {
		bt.dump(os);
		return(os);
	}
#endif
};

}

#endif
