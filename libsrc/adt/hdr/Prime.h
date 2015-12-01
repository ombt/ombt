//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __PRIME_H
#define __PRIME_H
// prime number class definition

// headers
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <errno.h>

// local headers
#include "system/Returns.h"
#include "system/Debug.h"
#include "adt/Set_BitVector.h"

namespace ombt {

// forward declarations
class Prime;
class PrimeIterator;

// prime numberclass
class Prime {
public:
        // friend classes
        friend class PrimeIterator;

        // constructors and destructor
        Prime(int);
        Prime(const Prime &);
        ~Prime();

        // assignment
        Prime &operator=(const Prime &);

	// check if number is prime
	int isItPrime(int) const;

        // miscellaneous
        friend std::ostream &operator<<(std::ostream &, const Prime &);

private:
	// not allowed
	Prime();

	// utility functions
	void generatePrimes();

protected:
        // internal data
	int maxNumber;
	Set_BitVector bitVector;
};

// prime number iterator
class PrimeIterator {
public:
        // constructors and destructor
        PrimeIterator(const PrimeIterator &);
        PrimeIterator(const Prime &);
        ~PrimeIterator();

        // initialization
	void reset();

	// check if at end of list
	int done() const;

        // return data 
        int operator()();

	// advance iterator to next link
	int operator++();

private:
	// not allowed
        PrimeIterator();
	PrimeIterator &operator=(const PrimeIterator &);

protected:
        // internal data
	const Prime &primeNumbers;
	Set_BitVector_Iterator iterator;
};

}

#endif
