//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// functions for bit-vector set class

// header
#include <adt/Set_BitVector.h>

namespace ombt {

// constructors and destructor
Set_BitVector::Set_BitVector(unsigned long setsz):
	arraySize(0), array(NULL)
{
	// allocate array
	arraySize = setSizeToLongs(setsz);
	array = new unsigned long [arraySize];
	MustBeTrue(array != 0);

	// zap it
	clear();
}

Set_BitVector::Set_BitVector(const Set_BitVector &src):
	arraySize(src.arraySize), array(new unsigned long [src.arraySize])
{
	MustBeTrue(array != NULL);
	memcpy((void *)array, (void *)src.array, bytesInSet());
}

Set_BitVector::~Set_BitVector()
{
	delete [] array;
	array = NULL;
	arraySize = 0;
}

// insert members operations
Set_BitVector &
Set_BitVector::operator+=(unsigned long m)
{
	unsigned long mlong = m/LONG_BIT;
	unsigned long mbit = m%LONG_BIT;
	if (mlong < arraySize)
	{
		array[mlong] |= (unsigned long)1L << mbit;
	}
	else
	{
		// allocate new array
		unsigned long newasz = mlong + 1;
		unsigned long *newa = new unsigned long [newasz];
		MustBeTrue(newa != NULL);

		// copy old array 
		memcpy((void *)newa, (void *)array, bytesInSet());

		// clear remaining part of array
		memset((void *)(newa+arraySize), 0, 
			bytesInSet(newasz-arraySize));

		// insert new member
		newa[mlong] |= (unsigned long)1L << mbit;

		// delete old array and save new data
		delete [] array;
		array = newa;
		arraySize = newasz;
	}
	return(*this);
}

Set_BitVector
Set_BitVector::operator+(unsigned long m) const
{
	return(Set_BitVector(*this) += m);
}

Set_BitVector &
Set_BitVector::insert(unsigned long m)
{
	return(*this += m);
}

Set_BitVector &
Set_BitVector::fill()
{
	
	memset((void *)array, ~0L, bytesInSet());
	return(*this);
}

// remove members operations
Set_BitVector &
Set_BitVector::operator-=(unsigned long m)
{
	unsigned long mlong = m/LONG_BIT;
	unsigned long mbit = m%LONG_BIT;
	if (mlong < arraySize)
		array[mlong] &= ~((unsigned long)1L << mbit);
	return(*this);
}

Set_BitVector
Set_BitVector::operator-(unsigned long m) const
{
	return(Set_BitVector(*this) -= m);
}

Set_BitVector &
Set_BitVector::remove(unsigned long m)
{
	return(*this -= m);
}

Set_BitVector &
Set_BitVector::clear()
{
	memset((void *)array, 0, bytesInSet());
	return(*this);
}

// assignment/equivalence set operations
Set_BitVector &
Set_BitVector::operator=(const Set_BitVector &set)
{
	if (this != &set)
	{
		// delete old array and allocate new array
		delete [] array;
		arraySize = set.arraySize;
		array = new unsigned long [arraySize];
		MustBeTrue(array != NULL);

		// copy array 
		memcpy((void *)array, (void *)set.array, bytesInSet());
	}
	return(*this);
}

int
Set_BitVector::operator==(const Set_BitVector &set) const
{
	// check relative sizes of arrays
	if (arraySize <= set.arraySize)
	{
		// check overlapping parts of arrays
		unsigned long ia;
		for (ia = 0; ia < arraySize; ia++)
		{
			if (array[ia] != set.array[ia]) return(0);
		}

		// non-overlapping part must be zero
		for ( ; ia < set.arraySize; ia++)
		{
			if (set.array[ia] != 0) return(0);
		}
	}
	else
	{
		// check overlapping parts
		unsigned long ia;
		for (ia = 0; ia < set.arraySize; ia++)
		{
			if (array[ia] != set.array[ia]) return(0);
		}

		// non-overlapping part must be zero
		for ( ; ia < arraySize; ia++)
		{
			if (array[ia] != 0) return(0);
		}
	}
	return(1);
}

unsigned long
Set_BitVector::operator[](unsigned long m) const
{
	if (isMember(m))
		return(1);
	else 
		return(0);
}

int
Set_BitVector::isMember(unsigned long m) const
{
	// find member location
	unsigned long mlong = m/LONG_BIT;
	unsigned long mbit = m%LONG_BIT;

	// check if set can hold this member
	if (mlong < arraySize)
		return((array[mlong] & ((unsigned long)1L << mbit)));
	else
		return(0);
}

int 
Set_BitVector::isEmpty() const
{
	for (unsigned long ia = 0; ia < arraySize; ia++)
	{
		if (array[ia] != 0) return(0);
	}
	return(1);
}

// union set operations
Set_BitVector &
Set_BitVector::operator|=(const Set_BitVector &set)
{
	// check which set is larger
	if (set.arraySize <= arraySize)
	{      
		// bit-wise or of overlapping parts
		for (unsigned long ia = 0; ia < set.arraySize; ia++)
		{
			array[ia] |= set.array[ia];
		}
	}
	else
	{
		// need to allocate a new array
		unsigned long *nset = new unsigned long [set.arraySize];
		MustBeTrue(nset != 0);

		// bit-wise or of overlapping parts
		unsigned long ia;
		for (ia = 0; ia < arraySize; ia++)
		{
			nset[ia] = array[ia] | set.array[ia];
		}

		// copy non-overlapping part
		for ( ; ia < set.arraySize; ia++)
		{
			nset[ia] = set.array[ia];
		}

		// save data
		delete [] array;
		array = nset;
		arraySize = set.arraySize;
	}
	return(*this);
}

Set_BitVector
Set_BitVector::operator|(const Set_BitVector &set) const
{
	return(Set_BitVector(*this) |= set);
}

// intersection set operations
Set_BitVector &
Set_BitVector::operator&=(const Set_BitVector &set)
{
	// check which set is larger
	if (set.arraySize <= arraySize)
	{      
		// bit-wise and of overlapping parts
		unsigned long ia;
		for (ia = 0; ia < set.arraySize; ia++)
		{
			array[ia] &= set.array[ia];
		}
		for ( ; ia < arraySize; ia++)
		{
			array[ia] = 0;
		}
	}
	else
	{
		// bit-wise and of overlapping parts
		for (unsigned long ia = 0; ia < arraySize; ia++)
		{
			array[ia] |= set.array[ia];
		}
	}
	return(*this);
}

Set_BitVector
Set_BitVector::operator&(const Set_BitVector &set) const
{
	return(Set_BitVector(*this) &= set);
}

// difference set operations
Set_BitVector &
Set_BitVector::operator-=(const Set_BitVector &set)
{
	// find smaller set
	unsigned long iamin = 
		(arraySize < set.arraySize) ? arraySize : set.arraySize;

	// remove common members from set
	for (unsigned long ia = 0; ia < iamin; ia++)
	{
		array[ia] &= ~set.array[ia];
	}
	return(*this);
}

Set_BitVector
Set_BitVector::operator-(const Set_BitVector &s) const
{
	return(Set_BitVector(*this) -= s);
}

// print set
void
Set_BitVector::dump(std::ostream &os) const
{
	os << "set = { ";
	for (unsigned long ia = 0; ia < arraySize; ia++)
	{
		if (array[ia] != 0)
		{
			for (unsigned long ib = 0 ; ib < LONG_BIT; ib++)
			{
				if (array[ia] & ((unsigned long)1L << ib))
				{
					os << (ia*LONG_BIT+ib) << " ";
				}
			}
		}
	}
	os << "}";
	return;
}

std::ostream &
operator<<(std::ostream &os, const Set_BitVector &set)
{
	set.dump(os);
	return(os);
}

// set iterator constructors and destructor
Set_BitVector_Iterator::Set_BitVector_Iterator(
	const Set_BitVector &s):
	set(&s), nextLong(0), nextBit(0)
{
	reset();
}

Set_BitVector_Iterator::Set_BitVector_Iterator(
	const Set_BitVector_Iterator &si):
	set(si.set), nextLong(si.nextLong), nextBit(si.nextBit)
{
	// do nothing
}

Set_BitVector_Iterator::~Set_BitVector_Iterator()
{
	set = NULL;
	nextLong = 0;
	nextBit = LONG_BIT;
}

// get next member in set
void
Set_BitVector_Iterator::reset()
{
	unsigned long *pa = set->array;
	unsigned long pasz = set->arraySize;
	for (nextLong = nextBit = 0; nextLong < pasz; nextLong++)
	{
		for ( ; nextBit < LONG_BIT; nextBit++)
		{
			if (pa[nextLong] & ((unsigned long)1L << nextBit))
				return;
		}
		nextBit = 0;
	}
	return;
}

// is iterator finished
int
Set_BitVector_Iterator::done() const
{
	return(nextLong >= set->arraySize);
}

// return current set member
unsigned long
Set_BitVector_Iterator::operator()() const
{
	return(nextLong*LONG_BIT+nextBit);
}

// get next member in set
int
Set_BitVector_Iterator::operator++(int)
{
	if (nextLong >= set->arraySize) 
		return(NOTOK);
	if (++nextBit >= LONG_BIT)
	{
		if (++nextLong >= set->arraySize)
			return(NOTOK);
		nextBit = 0;
	}
	for ( ; nextLong < set->arraySize; nextLong++)
	{
		for ( ; nextBit < LONG_BIT; nextBit++)
		{
			if (set->array[nextLong] & ((unsigned long)1L << nextBit))
			{
				int member =
					nextLong*LONG_BIT+nextBit;
				return(member);
			}
		}
		nextBit = 0;
	}
	return(NOTOK);
}

}
