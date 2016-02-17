#ifndef __SET_H
#define __SET_H
// set class definitions

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>

// local headers
#include "returns.h"
#include "debug.h"

// forward declarations
class Set;
class SetIterator;
typedef unsigned long ulong;

// definitions
#define MINIMUMARRAYSIZE 8
#define FIRSTMEMBER 0
#define LASTMEMBER (-1)
#define NULLULONGPTR ((ulong *)0)
#define NULLSETPTR ((Set *)0)
#define BYTESPERULONG (sizeof(ulong))
#define BITSPERBYTE 8
#define BITSPERULONG (BITSPERBYTE*BYTESPERULONG)

// forward declarations
class Set;
class SetIterator;

// set class definition
class Set {
public:
        // friends
        friend class SetIterator;

        // constructors and destructor
        Set();
        Set(const Set &);
        ~Set();

        // insert member operations
        Set &operator+=(int);
        Set operator+(int) const;
        friend Set &fill(Set &);
        friend Set &insert(Set &, int);

        // delete member operations
        Set &operator-=(int);
        Set operator-(int) const;
        friend Set &clear(Set &);
        friend Set &remove(Set &, int);

        // equality/assignment set operations
        Set &operator=(const Set &);
        int operator==(const Set &) const;
        friend int isMember(const Set &, int);
        friend int isEmpty(const Set &);

        // union set operations
        Set &operator|=(const Set &);
        Set operator|(const Set &) const;

        // intersection set operations
        Set &operator&=(const Set &);
        Set operator&(const Set &) const;

        // difference set operations
        Set &operator-=(const Set &);
        Set operator-(const Set &) const;

        // other functions
        int membersInSet() const {
                return(totalBitsOn(set, arraySize));
        }
        void dump(ostream &) const;
        friend ostream &operator<<(ostream &, const Set &);

protected:
        // internal use only
        Set(int);
        int totalBitsOn(const ulong *, int) const;
        inline int bytesInSet(int nl) const {
                return(nl*sizeof(ulong));
        }
        inline int bitsInSet(int nl) const {
                return(BITSPERBYTE*bytesInSet(nl));
        }

protected:
        // data
        int arraySize;
        ulong *set;
};

// set iterator class
class SetIterator {
public:
        // constructors and destructor
        SetIterator();
        SetIterator(const Set &);
        SetIterator(const SetIterator &);
        ~SetIterator();

        // assignment
        SetIterator &operator=(const SetIterator &);
        SetIterator &operator=(const Set &);

        // return next member
        int operator()();

protected:
        // data
        const Set *set;
        int nextLong;
        int nextBit;
};

#endif
