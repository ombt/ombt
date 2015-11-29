#ifndef __REFCOUNT_H
#define __REFCOUNT_H
// definition for a reference-counting base class

// headers
#include <stdlib.h>
#include <iostream.h>
#include <limits.h>

// local headers
#include "returns.h"
#include "debug.h"

// forward definitions
class RefCount;

// node class definition
class RefCount {
public:
	// constructors and destructor
	RefCount():
		counter(0) {
		// do nothing
	}
	RefCount(const RefCount &):
		counter(0) {
		// do nothing
	}
	virtual ~RefCount() {
		// do nothing
	}

	// assignment operator
	RefCount &operator=(const RefCount &) {
		return(*this);
	}

	// function to determine if data is shared
	int isShared() const {
		return(counter > 1);
	}

	// increment/decrement reference-counts
	void increment() {
		counter++;
	}
	void decrement() {
		if (--counter == 0) delete this;
	}

protected:
	// internal data
	int counter;
};

#endif

