// functions for bit-vector set class

// headers
#include <stdlib.h>
#include <string.h>
#include <mem.h>
#include <assert.h>

// local headers
#include "set.h"

// constructors and destructor
Set::Set(): 
        arraySize(MINIMUMARRAYSIZE), set(new ulong [MINIMUMARRAYSIZE])
{
        // sanity check
        assert((set != NULLULONGPTR) && (arraySize > 0));

        // now clear 
        clear(*this);
}

Set::Set(const Set &s):
        arraySize(s.arraySize), set(new ulong [s.arraySize])
{
        // copy member set
        memcpy((void *)set, (void *)s.set, bytesInSet(arraySize));

        // sanity check
        assert((set != NULLULONGPTR) && (arraySize > 0));
}

Set::Set(int m): 
        arraySize(0), set(NULLULONGPTR)
{
        // find location of member
        int whichLong = m/BITSPERULONG;
        int whichBit = m%BITSPERULONG;

        // allocate array
        arraySize = whichLong + 1;
        set = new ulong [arraySize];
        assert((set != NULLULONGPTR) && (arraySize > 0));

        // clear, then add member
        clear(*this);
        set[whichLong] |= (ulong)1L << whichBit;
}

Set::~Set()
{
        // delete and clear everything
        delete [] set;
        set = NULLULONGPTR;
        arraySize = 0;
}

// insert members operations
Set &
Set::operator+=(int m)
{
        // is member already in set
        if (isMember(*this, m))
                return(*this);

        // insert new member
        int whichLong = m/BITSPERULONG;
        int whichBit = m%BITSPERULONG;
        if (whichLong < arraySize)
        {
                set[whichLong] |= (ulong)1L << whichBit;
        }
        else
        {
                // reallocate array, to small to hold new member
                int newArraySize = whichLong + 1;
                ulong *newSet = new ulong [newArraySize];
                assert(newSet != NULLULONGPTR);

                // copy set array
                memcpy((void *)newSet, (void *)set, 
                        bytesInSet(newArraySize));
                memset((void *)(newSet+arraySize), 0, 
                        bytesInSet(newArraySize-arraySize));

                // insert new member
                newSet[whichLong] |= (ulong)1L << whichBit;

                // delete old set, save new set
                delete [] set;
                set = newSet;
                arraySize = newArraySize;
        }

        // sanity check
        assert((set != NULLULONGPTR) && (arraySize > 0));
        return(*this);
}

Set
Set::operator+(int m) const
{
        return(Set(*this) += m);
}

Set &
insert(Set &s, int m)
{
        return(s += m);
}

Set &
fill(Set &s)
{
        // turn on all bits in set array
        memset((void *)s.set, ~0L, s.bytesInSet(s.arraySize));
        assert((s.set != NULLULONGPTR) && (s.arraySize > 0));
        return(s);
}

// remove members operations
Set &
Set::operator-=(int m)
{
        // is member in set
        if ( ! isMember(*this, m))
                return(*this);

        // find member location
        int whichLong = m/BITSPERULONG;
        int whichBit = m%BITSPERULONG;

        // remove member
        set[whichLong] &= ~((ulong)1L << whichBit);

        // sanity check
        assert((set != NULLULONGPTR) && (arraySize > 0));
        return(*this);
}

Set
Set::operator-(int m) const
{
        return(Set(*this) -= m);
}

Set &
remove(Set &s, int m)
{
        return(s -= m);
}

Set &
clear(Set &s)
{
        // skip checking if set is empty, just blast it
        memset((void *)s.set, 0L, s.bytesInSet(s.arraySize));

        // sanity check
        assert((s.set != NULLULONGPTR) && (s.arraySize > 0));
        return(s);
}

// assignment/equivalence set operations
Set &
Set::operator=(const Set &s)
{
        // check for self-assignment
        if (this == &s) return(*this);

        // delete old array, allocate new array
        delete [] set;
        arraySize = s.arraySize;
        set = new ulong [s.arraySize];
        assert(set != (ulong *)0);

        // copy set array
        memcpy((void *)set, (void *)s.set, bytesInSet(s.arraySize));
        assert((set != NULLULONGPTR) && (arraySize > 0));
        return(*this);
}

int
Set::operator==(const Set &s) const
{
        // check relative sizes of arrays
        if (arraySize <= s.arraySize)
        {
                // check overlapping parts of arrays
                for (int is = 0; is < arraySize; is++)
                {
                        if (set[is] != s.set[is]) return(0);
                }

                // non-overlapping part must be zero
                for ( ; is < s.arraySize; is++)
                {
                        if (s.set[is] != 0) return(0);
                }
        }
        else
        {
                // check overlapping parts
                for (int is = 0; is < s.arraySize; is++)
                {
                        if (set[is] != s.set[is]) return(0);
                }

                // non-overlapping part must be zero
                for ( ; is < arraySize; is++)
                {
                        if (set[is] != 0) return(0);
                }
        }
        return(1);
}

int
isMember(const Set &s, int m)
{
        // find member location
        int whichLong = m/BITSPERULONG;
        int whichBit = m%BITSPERULONG;

        // check if set can hold this member
        if (whichLong < s.arraySize)
        {
                return((s.set[whichLong] & ((ulong)1L << whichBit)));
        }
        else
                return(0);
}

int 
isEmpty(const Set &s)
{
        return(s.membersInSet() == 0);
}

int
Set::totalBitsOn(const ulong *u, int usz) const 
{
        // structure for counting bits
        union {
                unsigned long ul;
                unsigned short us[2];
                unsigned char uc[4];
        } tmp;

        // check for empty arrays
        if (usz <= 0) return(0);

        // count up little bits
        int totalBits = 0;
        for (int iu; iu < usz; iu++)
        {
                // copy ulong to tmp buffer
                tmp.ul = u[iu];

                // check if ulong is empty
                if (tmp.ul != 0)
                {
                        // check if ushorts are empty
                        if (tmp.us[0] != 0)
                        {
                                // check if uchars are empty
                                if (tmp.uc[0] != 0)
                                {
                                        if (tmp.uc[0]&0x1) totalBits++;
                                        if (tmp.uc[0]&0x2) totalBits++;
                                        if (tmp.uc[0]&0x4) totalBits++;
                                        if (tmp.uc[0]&0x8) totalBits++;
                                        if (tmp.uc[0]&0x10) totalBits++;
                                        if (tmp.uc[0]&0x20) totalBits++;
                                        if (tmp.uc[0]&0x40) totalBits++;
                                        if (tmp.uc[0]&0x80) totalBits++;
                                }
                                if (tmp.uc[1] != 0)
                                {
                                        if (tmp.uc[1]&0x1) totalBits++;
                                        if (tmp.uc[1]&0x2) totalBits++;
                                        if (tmp.uc[1]&0x4) totalBits++;
                                        if (tmp.uc[1]&0x8) totalBits++;
                                        if (tmp.uc[1]&0x10) totalBits++;
                                        if (tmp.uc[1]&0x20) totalBits++;
                                        if (tmp.uc[1]&0x40) totalBits++;
                                        if (tmp.uc[1]&0x80) totalBits++;
                                }
                        }
                        if (tmp.us[1] != 0)
                        {
                                // check if uchars are empty
                                if (tmp.uc[2] != 0)
                                {
                                        if (tmp.uc[2]&0x1) totalBits++;
                                        if (tmp.uc[2]&0x2) totalBits++;
                                        if (tmp.uc[2]&0x4) totalBits++;
                                        if (tmp.uc[2]&0x8) totalBits++;
                                        if (tmp.uc[2]&0x10) totalBits++;
                                        if (tmp.uc[2]&0x20) totalBits++;
                                        if (tmp.uc[2]&0x40) totalBits++;
                                        if (tmp.uc[2]&0x80) totalBits++;
                                }
                                if (tmp.uc[3] != 0)
                                {
                                        if (tmp.uc[2]&0x1) totalBits++;
                                        if (tmp.uc[2]&0x2) totalBits++;
                                        if (tmp.uc[2]&0x4) totalBits++;
                                        if (tmp.uc[2]&0x8) totalBits++;
                                        if (tmp.uc[2]&0x10) totalBits++;
                                        if (tmp.uc[2]&0x20) totalBits++;
                                        if (tmp.uc[2]&0x40) totalBits++;
                                        if (tmp.uc[2]&0x80) totalBits++;
                                }
                        }
                }
        }
        return(totalBits);
}

// union set operations
Set &
Set::operator|=(const Set &s)
{
        // check which set is larger
        if (s.arraySize <= arraySize)
        {      
                // bit-wise or of overlapping parts
                for (int is = 0; is < s.arraySize; is++)
                {
                        set[is] |= s.set[is];
                }
        }
        else
        {
                // need to allocate a new array
                ulong *nset = new ulong [s.arraySize];
                assert(nset != NULLULONGPTR);

                // bit-wise or of overlapping parts
                for (int is = 0; is < arraySize; is++)
                {
                        nset[is] = set[is] | s.set[is];
                }

                // copy non-overlapping part
                for ( ; is < s.arraySize; is++)
                {
                        nset[is] = s.set[is];
                }

                // save data
                delete [] set;
                set = nset;
                arraySize = s.arraySize;
        }

        // sanity check
        assert((set != NULLULONGPTR) && (arraySize > 0));
        return(*this);
}

Set
Set::operator|(const Set &s) const
{
        return(Set(*this) |= s);
}

// intersection set operations
Set &
Set::operator&=(const Set &s)
{
        // check which set is larger
        if (s.arraySize <= arraySize)
        {      
                // bit-wise and of overlapping parts
                for (int is = 0; is < s.arraySize; is++)
                {
                        set[is] &= s.set[is];
                }
                for ( ; is < arraySize; is++)
                {
                        set[is] = 0;
                }
        }
        else
        {
                // bit-wise and of overlapping parts
                for (int is = 0; is < arraySize; is++)
                {
                        set[is] |= s.set[is];
                }
        }

        // sanity check
        assert((set != NULLULONGPTR) && (arraySize > 0));
        return(*this);
}

Set
Set::operator&(const Set &s) const
{
        return(Set(*this) &= s);
}

// difference set operations
Set &
Set::operator-=(const Set &s)
{
        // find smaller set
        int ismin = (arraySize < s.arraySize) ? arraySize : s.arraySize;

        // remove common members from set
        for (int is = 0; is < ismin; is++)
        {
                set[is] &= ~s.set[is];
        }

        // sanity check
        assert((set != NULLULONGPTR) && (arraySize > 0));
        return(*this);
}

Set
Set::operator-(const Set &s) const
{
        return(Set(*this) -= s);
}

// print set
void
Set::dump(ostream &os) const
{
        os << "set = { ";
        for (int is = 0; is < arraySize; is++)
        {
                int base = is*BITSPERULONG;
                for (int ib = 0; ib < BITSPERULONG; ib++)
                {
                        if (set[is] & ((ulong)1L << ib))
                        {
                                os << (base+ib) << " ";
                        }
                }
        }
        os << " }";
        return;
}

ostream &
operator<<(ostream &os, const Set &s)
{
        s.dump(os);
        return(os);
}

// set iterator constructors and destructor
SetIterator::SetIterator(): 
        set(NULLSETPTR), nextLong(0), nextBit(BITSPERULONG)
{
        // do nothing
}

SetIterator::SetIterator(const Set &s):
        set(&s), nextLong(0), nextBit(0)
{
        // do nothing
}

SetIterator::SetIterator(const SetIterator &si):
        set(si.set), nextLong(0), nextBit(0)
{
        // do nothing
}

SetIterator::~SetIterator()
{
        set = NULLSETPTR;
        nextLong = 0;
        nextBit = BITSPERULONG;
}

// assignment
SetIterator &
SetIterator::operator=(const SetIterator &si)
{
        set = si.set;
        nextLong = 0;
        nextBit = 0;
        return(*this);
}

SetIterator &
SetIterator::operator=(const Set &s)
{
        set = &s;
        nextLong = 0;
        nextBit = 0;
        return(*this);
}

// get next member in set
int
SetIterator::operator()()
{
        if (nextLong >= set->arraySize) 
                return(LASTMEMBER);
        if (nextBit >= BITSPERULONG)
        {
                if (++nextLong >= set->arraySize)
                        return(LASTMEMBER);
                nextBit = 0;
        }
        for ( ; nextLong < set->arraySize; nextLong++)
        {
                for ( ; nextBit < BITSPERULONG; nextBit++)
                {
                        if (set->set[nextLong] & ((ulong)1L << nextBit))
                        {
                                int member =
                                        nextLong*BITSPERULONG+nextBit++;
                                return(member);
                        }
                }
                nextBit = 0;
        }
        return(LASTMEMBER);
}






