//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __SET_BIT_VECTOR_H
#define __SET_BIT_VECTOR_H
// bit-vector set class definitions

// headers
#include <stdlib.h>
#include <iostream>
#include <limits.h>

// local headers
#include "system/Returns.h"
#include "system/Debug.h"

namespace ombt {

// forward declarations
class Set_BitVector;
class Set_BitVector_Iterator;

// definitions
#define MINIMUMSETSIZE 100

// set class definition
class Set_BitVector
{
public:
        // friends
        friend class Set_BitVector_Iterator;

        // constructors and destructor
        Set_BitVector(unsigned long = MINIMUMSETSIZE);
        Set_BitVector(const Set_BitVector &);
        ~Set_BitVector();

        // insert member operations
        Set_BitVector &operator+=(unsigned long);
        Set_BitVector operator+(unsigned long) const;
        Set_BitVector &fill();
        Set_BitVector &insert(unsigned long);

        // delete member operations
        Set_BitVector &operator-=(unsigned long);
        Set_BitVector operator-(unsigned long) const;
        Set_BitVector &clear();
        Set_BitVector &remove(unsigned long);

        // equality/assignment set operations
        Set_BitVector &operator=(const Set_BitVector &);
        int operator==(const Set_BitVector &) const;
        unsigned long operator[](unsigned long) const;
        int isMember(unsigned long) const;
        int isEmpty() const;

        // union set operations
        Set_BitVector &operator|=(const Set_BitVector &);
        Set_BitVector operator|(const Set_BitVector &) const;

        // intersection set operations
        Set_BitVector &operator&=(const Set_BitVector &);
        Set_BitVector operator&(const Set_BitVector &) const;

        // difference set operations
        Set_BitVector &operator-=(const Set_BitVector &);
        Set_BitVector operator-(const Set_BitVector &) const;

        void dump(std::ostream &) const;
        friend std::ostream &operator<<(std::ostream &, const Set_BitVector &);

protected:
        // internal use only
        inline unsigned long setSizeToLongs(unsigned long setsz) const {
		unsigned long nlongs = setsz/LONG_BIT;
		if ((setsz%LONG_BIT) != 0) nlongs++;
                return(nlongs);
        }
        inline unsigned long bytesInSet() const {
                return(arraySize*sizeof(unsigned long));
        }
        inline unsigned long bytesInSet(unsigned long asz) const {
                return(asz*sizeof(unsigned long));
        }
	inline int maxMembersInSet() {
		return(arraySize*LONG_BIT);
	}

protected:
        // data
        unsigned long arraySize;
        unsigned long *array;
};

// set iterator class
class Set_BitVector_Iterator
{
public:
        // constructors and destructor
        Set_BitVector_Iterator(const Set_BitVector &);
        Set_BitVector_Iterator(const Set_BitVector_Iterator &);
        ~Set_BitVector_Iterator();

	// initialization
	void reset();

	// check if at end of set
	int done() const;

	// return current set member
        unsigned long operator()() const;

	// advance to next set member
	int operator++(int);

private:
	// not allowed
        Set_BitVector_Iterator();
        Set_BitVector_Iterator &operator=(const Set_BitVector_Iterator &);


protected:
        // data
        const Set_BitVector *set;
        int nextLong;
        int nextBit;
};

}

#endif
