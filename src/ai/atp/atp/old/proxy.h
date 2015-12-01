#ifndef __PROXY_H
#define __PROXY_H
//
// reference-counted pointer. one very important point. since
// this class has the the class member access operator defined 
// (for example, pa->b), it can not be used with primitive
// data types. The following statements are nonsense for primitive
// data types.
//
//	int *pi = new int;	// allocate an int
//	pi->something = 1;	// error: 
//
// you must remove the declaration for operator->() to use this
// pointer class with primitive types, i.e., int, short, long, char, etc.
//
// WARNING: reference-counted pointers do not work with cyclic data
// structures. cycles can cause some major memory leaks since the
// use counts never go to zero and the objects are never deleted.
//
// NOTE: the main difference between Pointer and Proxy is that the
// comparison operators are dereferenced in Proxy, but not in Pointer.
// Proxy acts like a stand-in for the actual object.
//

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"

// forward declarations
template <class DataType> class Proxy;
template <class DataType> class ProxyRep;

// reference-counted pointer representation
template <class DataType> 
class ProxyRep {
	// friend class
	friend class Proxy<DataType>;
private:
	ProxyRep(DataType *pd):
		pdata(pd), counter(0) {
		// do nothing
	}
	ProxyRep(DataType &d):
		pdata(NULL), counter(0) {
		pdata = new DataType(d);
		MustBeTrue(pdata != NULL);
	}
	~ProxyRep() {
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
	int getCounter() const {
		return(counter);
	}
	int setCounter(int c) {
		counter = c;
		return(counter);
	}

	// internal data
	DataType *pdata;
	int counter;
};

// reference-counted pointer
template <class DataType> 
class Proxy {
public:
	Proxy():
		prep(new ProxyRep<DataType>(NULL)) {
		MustBeTrue(prep != NULL);
	}
	Proxy(DataType *pd):
		prep(new ProxyRep<DataType>(pd)) {
		MustBeTrue(prep != NULL);
		prep->increment();
	}
	Proxy(DataType &pd):
		prep(new ProxyRep<DataType>(pd)) {
		MustBeTrue(prep != NULL);
		prep->increment();
	}
	Proxy(const Proxy<DataType> &p):
		prep(p.prep) {
		MustBeTrue(prep != NULL);
		prep->increment();
	}
	~Proxy() {
		if (prep->decrement() <= 0)
			delete prep;
		prep = NULL;
	}

	// assignment
	Proxy<DataType> &operator=(DataType *rhs) {
		if (prep->pdata != rhs)
		{
			if (prep->decrement() <= 0)
			{
				delete prep;
				prep = NULL;
			}
			prep = new ProxyRep<DataType>(rhs);
			MustBeTrue(prep != NULL);
			prep->increment();
		}
		return(*this);
	}
	Proxy<DataType> &operator=(const Proxy<DataType> &rhs) {
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

	// sanity checks
	int isNull() const {
		MustBeTrue(prep != NULL);
		return(prep->pdata == NULL);
	}
	int isNotNull() const {
		MustBeTrue(prep != NULL);
		return(prep->pdata != NULL);
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
	int operator==(const Proxy<DataType> &rhs) const {
		MustBeTrue(prep != NULL && rhs.prep != NULL);
		return(*prep->pdata == *rhs.prep->pdata);
	}
	int operator!=(const Proxy<DataType> &rhs) const {
		MustBeTrue(prep != NULL && rhs.prep != NULL);
		return(*prep->pdata != *rhs.prep->pdata);
	}
	int operator<(const Proxy<DataType> &rhs) const {
		MustBeTrue(prep != NULL && rhs.prep != NULL);
		return(*prep->pdata < *rhs.prep->pdata);
	}
	int operator>(const Proxy<DataType> &rhs) const {
		MustBeTrue(prep != NULL && rhs.prep != NULL);
		return(*prep->pdata > *rhs.prep->pdata);
	}
	int operator<=(const Proxy<DataType> &rhs) const {
		MustBeTrue(prep != NULL && rhs.prep != NULL);
		return(*prep->pdata <= *rhs.prep->pdata);
	}
	int operator>=(const Proxy<DataType> &rhs) const {
		MustBeTrue(prep != NULL && rhs.prep != NULL);
		return(*prep->pdata >= *rhs.prep->pdata);
	}

protected:
	// internal data
	ProxyRep<DataType> *prep;
};

#endif
