#ifndef __NODEPROXY_Y
#define __NODEPROXY_Y
// reference-counted proxy class for heuristic search node classes

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"

// dereferenced-node class proxy class
template <class DataType> class NodePtr {
public:
        // constructors and destructor
	NodePtr():
		ptr(NULL)
	{
		// do nothing
	}
	NodePtr(const NodePtr<DataType> &src):
		ptr(src.ptr)
	{
		if (ptr != NULL) 
			ptr->increment();
	}
	NodePtr(DataType *src):
		ptr(src)
	{
		MustBeTrue(src != NULL);
		ptr->increment();
	}
	NodePtr(const DataType &src):
		ptr(NULL)
	{
		ptr = new DataType(src);
		MustBeTrue(ptr != NULL);
		ptr->increment();
	}
        ~NodePtr() {
		if (ptr != NULL)
			 ptr->decrement();
		ptr = NULL;
	}

        // comparison operators
	int operator<(const NodePtr<DataType> &rhs) const {
		return(*ptr < *rhs.ptr);
	}
	int operator>(const NodePtr<DataType> &rhs) const {
		return(*ptr > *rhs.ptr);
	}
	int operator==(const NodePtr<DataType> &rhs) const {
		return(*ptr == *rhs.ptr);
	}
	int operator!=(const NodePtr<DataType> &rhs) const {
		return(!(*ptr == *rhs.ptr));
	}

	// assigment operator
        NodePtr<DataType> &operator=(const NodePtr<DataType> &rhs) {
		if (ptr != NULL)
		{
			if (ptr == rhs.ptr)
				return(*this);
			ptr->decrement();
		}
		ptr = rhs.ptr;
		ptr->increment();
		return(*this);
	}
        NodePtr<DataType> &operator=(DataType *rhs) {
		if (ptr != NULL)
		{
			if (ptr == rhs)
				return(*this);
			ptr->decrement();
		}
		ptr = rhs;
		MustBeTrue(ptr != NULL);
		ptr->increment();
		return(*this);
	}

	// type casts and addressing operators.
	DataType *operator->() const {
		return(ptr);
	}
	DataType &operator*() const {
		return(*ptr);
	}
	operator DataType *() const {
		return(ptr);
	}

	// output operator
	friend ostream &
	operator<<(ostream &os, const NodePtr<DataType> &rhs) {
		os << *rhs.ptr;
		return(os);
	}

protected:
        // internal data
        DataType *ptr;
};

#endif
