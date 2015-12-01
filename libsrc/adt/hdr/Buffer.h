//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __BUFFER_H
#define __BUFFER_H
// buffer class definition

// headers
#include <stdlib.h>
#include <iostream>

// local headers
#include "system/Returns.h"
#include "system/Debug.h"

// buffer class
template <class DataType> class Buffer {
public:
        // constructors and destructor
        Buffer(int sz):
		size(sz), ptr(new DataType [sz]) {
		MustBeTrue(size > 0);
		MustBeTrue(ptr != NULL);
	}
	Buffer(const Buffer &src):
		size(src.size), ptr(new DataType [src.size]) {
		MustBeTrue(size > 0);
		MustBeTrue(ptr != NULL);
	}
        ~Buffer() {
		delete [] ptr;
		ptr = NULL;
		size = 0;
	}

        // operators
        Buffer &operator=(const Buffer &rhs) {
		if (this != &rhs)
		{
			delete [] ptr;
			size = rhs.size;
			MustBeTrue(size > 0);
			ptr = new DataType [rhs.size];
			MustBeTrue(ptr != NULL);
			for (int idx=0; idx < size; idx++)
				ptr[idx] = rhs.ptr[idx];
		}
		return(*this);
	}
        DataType &operator[](int idx) {
		MustBeTrue(0 <= idx && idx < size);
		return(ptr[idx]);
	}
	DataType &operator*() {
		return(*ptr);
	}
	operator DataType *() {
		return(ptr);
	}
	DataType *operator&() {
		return(ptr);
	}

private:
	// not allowed
	Buffer();

protected:
        // internal data
	int size;
        DataType *ptr;
};

#endif
