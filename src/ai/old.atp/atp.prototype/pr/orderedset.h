#ifndef __ORDEREDSET_H
#define __ORDEREDSET_H
//
// ordered set class definition - an ordered set (totally ordered) is 
// defined by a binary relation that is reflexive, antisymmetric,
// and transitive. Also, every element of a the set is comparable
// to every other element in the set. if every element in the set
// was NOT comparable to every other element in the set, then we
// have a partially ordered set and the implementaion used below
// is NOT appropriate since two elements of the set may not have
// relation defined for them.
//
// in summary, the ordered set implemented here requires that every
// element in the set by comparable to every OTHER element in the
// set. 
//

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "absIterator.h"
#include "tuple.h"
#include "mylist.h"
#include "binaryTree.h"

// forward declarations
template <class KeyType> class OrderedSet;
template <class KeyType> class OrderedSetIterator;

// forward declaration of friend functions
template <class KeyType> 
ostream &
operator<<(ostream &, const OrderedSet<KeyType> &);

// ordered set class
template <class KeyType> class OrderedSet {
public:
	// friend classes
	friend class OrderedSetIterator<KeyType>;

        // constructors and destructor
        OrderedSet();
        OrderedSet(const OrderedSet<KeyType> &);
        ~OrderedSet();

        // operators
        OrderedSet<KeyType> &operator=(const OrderedSet<KeyType> &);

	// access operations. since each key really identifies a 
	// equivalent set of items. to identify a specific member
	// within the set an index is used. the index corresponds
	// to the location of the item in the set. insert accepts
	// only individual items. retrieve can return the entire
	// equivalent set, or just a specific item within the set.
	// update can update an entire set with a new set, or update
	// an individual member within the equivalence set. similarly
	// for remove, either the entire equivalence set is removed or
	// a specific member in the set. if an out-or-range index
	// is given, NOMATCH is returned.
	//
	int insert(const KeyType &);
	int retrieve(KeyType &, int) const;
	int retrieve(KeyType &, List<KeyType> &) const;
	int update(const KeyType &, int);
	int update(const KeyType &, List<KeyType> &);
	int remove(const KeyType &);
	int remove(const KeyType &, int);

	// returns the size of sets, either the entire set or an
	// equivalence set.
	//
	int getSize() const;
	int getSize(const KeyType &) const;
	int numberOfEquivalentSets() const;

	// other functions
	int isInSet(const KeyType &) const;
	void clear();

	// print ordered sets
        friend ostream &operator<<(ostream &, const OrderedSet<KeyType> &);

protected:
        // internal data
	BinaryTree<Tuple<KeyType, List<KeyType> * > > oset;
};

// ordered set iterator
template <class KeyType> class OrderedSetIterator {
public:
        // constructors and destructor
        OrderedSetIterator(const OrderedSetIterator<KeyType> &);
        OrderedSetIterator(const OrderedSet<KeyType> &);
        ~OrderedSetIterator();

        // initialization
	void reset();

	// check if at end of list
	int done() const;

        // return data 
        List<KeyType> operator()();
        List<KeyType> data();
        KeyType key();

	// advance iterator to next link
	int operator++(int);

private:
	// not allowed
        OrderedSetIterator();
	OrderedSetIterator<KeyType> &
		operator=(const OrderedSetIterator<KeyType> &);

protected:
        // internal data
	BinaryTreeIterator_InOrder<Tuple<KeyType, List<KeyType> * > > osetIter;
};

#endif
