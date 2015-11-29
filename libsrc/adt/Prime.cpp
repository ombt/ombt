//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// functions for prime number class

// header
#include <adt/Prime.h>

namespace ombt {

// constructors and destructor
Prime::Prime(int maxno): 
        maxNumber(maxno), bitVector(maxno)
{
	generatePrimes();
}

Prime::Prime(const Prime &p):
        maxNumber(p.maxNumber), bitVector(p.bitVector)
{
	// do nothing
}

Prime::~Prime()
{
	// do nothing
}

// assignment operations
Prime &
Prime::operator=(const Prime &rhs)
{
        // check for self-assignment
        if (this != &rhs)
		bitVector = rhs.bitVector;
        return(*this);
}

// check if a number is prime
int
Prime::isItPrime(int n) const
{
	return(bitVector[n]);
}

// function to generator prime numbers
void
Prime::generatePrimes()
{
	// assume all are prime
	for (int number = 0; number < maxNumber; number++)
	{
		bitVector += number;
	}

	// turn off 0 and 1
	bitVector -= 0;
	bitVector -= 1;

	// start sieve
	for (int prime = 0; prime < maxNumber; prime++)
	{
		// check for a prime
		if ( ! bitVector[prime])
			continue;

		// turn off all multiples of prime number
		for (int notprime = 2*prime; notprime < maxNumber;
		     notprime += prime)
		{
			bitVector -= notprime;
		}
	}

	// all done
	return;
}

// print prime numbers
std::ostream &
operator<<(std::ostream &os, const Prime &p)
{
	Set_BitVector_Iterator iter(p.bitVector);
	for ( ; ! iter.done(); iter++)
	{
		os << iter() << std::endl;
	}
        return(os);
}

// prime number iterator constructors and destructor
PrimeIterator::PrimeIterator(const PrimeIterator &pi):
	primeNumbers(pi.primeNumbers), iterator(pi.iterator)
{
        // do nothing
}

PrimeIterator::PrimeIterator(const Prime &p):
	primeNumbers(p), iterator(p.bitVector)
{
        // do nothing
}

PrimeIterator::~PrimeIterator()
{
	// do nothing
}

// initialize iterator
void
PrimeIterator::reset()
{
	iterator.reset();
	return;
}

// is iterator finished
int
PrimeIterator::done() const
{
	return(iterator.done());
}

// return current set member
int
PrimeIterator::operator()()
{
	return(iterator());
}

// get next member in set
int
PrimeIterator::operator++()
{
        return(iterator++);
}

}
