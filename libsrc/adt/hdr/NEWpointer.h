//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
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
#include <iostream>

// local headers
#include "system/Returns.h"
#include "system/Debug.h"

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
		pdata(pd), counter(0) {
		// do nothing
	}
	~PointerRep() {
		counter = 0;
		if (pdata != NULL)
			delete pdata;
	}
	int increment() {
		return(++counter);
	}
	int decrement() {
		return(--counter);
	}
	int getCounter() const () {
		return(counter);
	}
	int setCounter(int c) () {
		counter = c;
		return(counter);
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
		prep->increment();
	}
	Pointer(const Pointer<DataType> &p):
		prep(p.prep) {
		MustBeTrue(prep != NULL);
		prep->increment();
	}
	~Pointer() {
		if (prep->decrement() <= 0)
			delete prep;
		prep = NULL;
	}

	// assignment
	Pointer<DataType> &operator=(DataType *rhs) {
		if (prep->pdata != rhs)
		{
			if (prep->decrement() <= 0)
			{
				delete prep;
				prep = NULL;
			}
			prep = new PointerRep<DataType>(rhs);
			MustBeTrue(prep != NULL);
			prep->increment();
		}
		return(*this);
	}
	Pointer<DataType> &operator=(const Pointer<DataType> &rhs) {
		rhs.prep->increment();
		if (prep->decrement() <= 0)
		{
			delete prep;
			prep = NULL;
		}
		prep = rhs.prep;
		MustBeTrue(prep != NULL);
		return(*this);
	}

	// pointer operators
	operator DataType *() const {
		MustBeTrue(prep != NULL);
		return(prep->pdata);
	}
	DataType *operator->() const {
		MustBeTrue(prep != NULL);
		return(prep->pdata);
	}
	DataType &operator*() {
		MustBeTrue(prep != NULL);
		return(*(prep->pdata));
	}
	DataType &operator*() const {
		MustBeTrue(prep != NULL);
		return(*(prep->pdata));
	}
	DataType &operator[](int idx) {
		MustBeTrue(prep != NULL);
		return(prep->pdata[idx]);
	}
	DataType &operator[](int idx) const {
		MustBeTrue(prep != NULL);
		return(prep->pdata[idx]);
	}

	// comparison operators
	int operator==(const Pointer<DataType> &rhs) const {
		MustBeTrue(prep != NULL && rhs.prep != NULL);
		return(prep->pdata == rhs.prep->pdata);
	}
	int operator!=(const Pointer<DataType> &rhs) const {
		MustBeTrue(prep != NULL && rhs.prep != NULL);
		return(prep->pdata != rhs.prep->pdata);
	}
	int operator<(const Pointer<DataType> &rhs) const {
		MustBeTrue(prep != NULL && rhs.prep != NULL);
		return(prep->pdata < rhs.prep->pdata);
	}
	int operator>(const Pointer<DataType> &rhs) const {
		MustBeTrue(prep != NULL && rhs.prep != NULL);
		return(prep->pdata > rhs.prep->pdata);
	}
	int operator<=(const Pointer<DataType> &rhs) const {
		MustBeTrue(prep != NULL && rhs.prep != NULL);
		return(prep->pdata <= rhs.prep->pdata);
	}
	int operator>=(const Pointer<DataType> &rhs) const {
		MustBeTrue(prep != NULL && rhs.prep != NULL);
		return(prep->pdata >= rhs.prep->pdata);
	}

protected:
	// internal data
	PointerRep<DataType> *prep;
};

#endif
