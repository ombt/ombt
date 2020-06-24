#ifndef __DISJOINT_SETS_H
#define __DISJOINT_SETS_H
// disjoint sets class definitions

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>

// local headers
#include "returns.h"
#include "debug.h"

// forward declarations
class DisjointSets;

// disjoint sets class definition
class DisjointSets {
public:
        // constructors and destructor
        DisjointSets();
        DisjointSets(int);
        DisjointSets(const DisjointSets &);
        ~DisjointSets();

	// initialization
	int initialize(int);

	// assignment
	DisjointSets &operator=(const DisjointSets &);

	// union of two sets
	int unionOf(DisjointSets &, int, int);

	// member of which set
	int findSet(DisjointSets &, int);

	// dump data 
        void dump(ostream &) const;
        friend ostream &operator<<(ostream &, const DisjointSets &);

protected:
	// internal use only
	int link(DisjointSets &, int, int);

	// internal structure definition
	struct DisjointSet {
	public:
		// constructor and destructor
		DisjointSet(): 
			parent(-1), rank(0) {
		}
		~DisjointSet() {
		}

	public:
		// data
		int parent;
		int rank;
	};

protected:
        // internal data
	int numberOfMembers;
	DisjointSet *dsets;
};

#endif
