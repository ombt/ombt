#ifndef __PTR_H
#define __PTR_H
//
// a simple pointer class. it has different semantics than auto_ptr.
// I want this pointer to clean up after it self. unlike autp_ptr
// which transfer ownership during assignment and copying, I want
// this pointer to make a duplicate of itself. it does delete
// whatever it was pointing to. I don't want any aliasing to occur.
//

// headers
#include <stdlib.h>

// forward declarations
template <class DT> class Ptr;

// simple pointer class
template <class DT> 
class Ptr {
public:
	// constructors and destructor
	Ptr(): 
		ptr(NULL) {
		// do nothing
	}
	Ptr(DT *p):
		ptr(p) {
		// do nothing
	}
	Ptr(const Ptr<DT> &p):
		ptr(NULL) {
		if (p.ptr != NULL)
		{
			ptr = new DT(*p.ptr);
			MustBeTrue(ptr != NULL);
		}	
	}
	~Ptr() {
		if (ptr != NULL)
			delete ptr;
		ptr = NULL;
	}

	// assignment
	Ptr<DT> &operator=(DT *rhs) {
		if (ptr != rhs)
		{
			if (ptr != NULL)
				delete ptr;
			ptr = rhs;
		}
		return(*this);
	}
	Ptr<DT> &operator=(const Ptr<DT> &rhs) {
		if (this != &rhs)
		{
			if (ptr != NULL)
				delete ptr;
			if (rhs.ptr != NULL)
			{
				ptr = new DT(*rhs.ptr);
				MustBeTrue(ptr != NULL);
			}
		}
		return(*this);
	}

	// sanity checks
	int isNull() const {
		return(ptr == NULL);
	}

	// pointer operators
	operator DT *() const {
		return(ptr);
	}
	DT *operator->() const {
		MustBeTrue(ptr != NULL);
		return(ptr);
	}
	DT &operator*() {
		MustBeTrue(ptr != NULL);
		return(*ptr);
	}
	DT &operator*() const {
		MustBeTrue(ptr != NULL);
		return(*ptr);
	}

	// comparison operators
	int operator==(const Ptr<DT> &rhs) const {
		return(ptr == rhs.ptr);
	}
	int operator!=(const Ptr<DT> &rhs) const {
		return(ptr != rhs.ptr);
	}
	int operator<(const Ptr<DT> &rhs) const {
		return(ptr < rhs.ptr);
	}
	int operator>(const Ptr<DT> &rhs) const {
		return(ptr > rhs.ptr);
	}
	int operator<=(const Ptr<DT> &rhs) const {
		return(ptr <= rhs.ptr);
	}
	int operator>=(const Ptr<DT> &rhs) const {
		return(ptr >= rhs.ptr);
	}

protected:
	// internal data
	DT *ptr;
};

#endif
