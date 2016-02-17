#ifndef __POINTER_H
#define __POINTER_H
//
// reference-counted pointer. one very important point, since
// this class has the the class member access operator defined 
// (for example, pa->b), it can not be used with primitive
// data types. The following statements are nonsensical for primitive
// type.
//
//	int *pi = new int;	// allocate an int
//	pi->something = 1;	// error: 
//
// you must remove the declaration for operator->() to use this
// pointer class with primitive types, i.e., int, short, long, char, etc.
//

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"

// forward declarations
template <class DataType> class Pointer;
template <class DataType> class PointerRep;

// reference-counted pointer representation
template <class DataType> 
class PointerRep {
	// friend class
	friend class Pointer<DataType>;
private:
	PointerRep(DataType *pd):
		pdata(pd), counter(1) {
		// do nothing
	}
	~PointerRep() {
		counter = 0;
		if (pdata != NULL)
			delete pdata;
	}

	// internal data
	DataType *pdata;
	int counter;
};

// reference-counted pointer
template <class DataType> 
class Pointer {
public:
	Pointer():
		prep(new PointerRep<DataType>(NULL)) {
		MustBeTrue(prep != NULL);
	}
	Pointer(DataType *pd):
		prep(new PointerRep<DataType>(pd)) {
		MustBeTrue(prep != NULL);
	}
	Pointer(const Pointer<DataType> &p):
		prep(p.prep) {
		MustBeTrue(prep != NULL);
		prep->counter++;
	}
	~Pointer() {
		prep->counter--;
		if (prep->counter <= 0)
		{
			delete prep;
			prep = NULL;
		}
	}

	// assignment
	Pointer<DataType> &operator=(DataType *rhs) {
		if (prep->pdata != rhs)
		{
			prep->counter--;
			if (prep->counter <= 0)
			{
				delete prep;
				prep = NULL;
			}
			prep = new PointerRep<DataType>(rhs);
		}
		MustBeTrue(prep != NULL);
		return(*this);
	}
	Pointer<DataType> &operator=(const Pointer<DataType> &rhs) {
		if (prep != rhs.prep)
		{
			prep->counter--;
			if (prep->counter <= 0)
			{
				delete prep;
				prep = NULL;
			}
			prep = rhs.prep;
			prep->counter++;
		}
		MustBeTrue(prep != NULL);
		return(*this);
	}

	// pointer operators
	operator DataType *() const {
		return(prep->pdata);
	}
	DataType *operator->() const {
		return(prep->pdata);
	}
	DataType &operator*() {
		MustBeTrue(prep->pdata != NULL);
		return(*(prep->pdata));
	}
	DataType &operator*() const {
		MustBeTrue(prep->pdata != NULL);
		return(*(prep->pdata));
	}
	DataType &operator[](int idx) {
		MustBeTrue(prep->pdata != NULL);
		return(prep->pdata[idx]);
	}
	DataType &operator[](int idx) const {
		MustBeTrue(prep->pdata != NULL);
		return(prep->pdata[idx]);
	}

	// comparison operators
	int operator==(const Pointer<DataType> &rhs) const {
		return(prep->pdata == rhs.prep->pdata);
	}
	int operator!=(const Pointer<DataType> &rhs) const {
		return(prep->pdata != rhs.prep->pdata);
	}
	int operator<(const Pointer<DataType> &rhs) const {
		return(prep->pdata < rhs.prep->pdata);
	}
	int operator>(const Pointer<DataType> &rhs) const {
		return(prep->pdata > rhs.prep->pdata);
	}
	int operator<=(const Pointer<DataType> &rhs) const {
		return(prep->pdata <= rhs.prep->pdata);
	}
	int operator>=(const Pointer<DataType> &rhs) const {
		return(prep->pdata >= rhs.prep->pdata);
	}

protected:
	// internal data
	PointerRep<DataType> *prep;
};

#endif
