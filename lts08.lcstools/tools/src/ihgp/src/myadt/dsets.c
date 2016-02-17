// functions for disjoint sets class

// headers
#include <stdlib.h>
#include <iostream.h>
#include <string.h>
#include <assert.h>

// local headers
#include "dsets.h"

// map class constructors and destructor
DisjointSets::DisjointSets(): 
	numberOfMembers(0), dsets((DisjointSet *)0)
{
	assert(numberOfMembers >= 0);
}

DisjointSets::DisjointSets(int nmems): 
	numberOfMembers(nmems), dsets((DisjointSet *)0)
{
	assert(numberOfMembers >= 0);
	if (numberOfMembers > 0)
	{
		dsets = new DisjointSet[numberOfMembers];
		assert(dsets != (DisjointSet *)0);
		for (int im = 0; im < numberOfMembers; im++)
		{
			dsets[im].parent = im;
			dsets[im].rank = 1;
		}
	}
}

DisjointSets::DisjointSets(const DisjointSets &dss):
	numberOfMembers(dss.numberOfMembers), dsets((DisjointSet *)0)
{
	assert(numberOfMembers >= 0);
	if (numberOfMembers > 0)
	{
		dsets = new DisjointSet[numberOfMembers];
		assert(dsets != (DisjointSet *)0);
		for (int im = 0; im < numberOfMembers; im++)
		{
			dsets[im].parent = dss.dsets[im].parent;
			dsets[im].rank = dss.dsets[im].rank;
		}
	}
}

DisjointSets::~DisjointSets()
{
	if (numberOfMembers > 0)
	{
		delete [] dsets;
		dsets = (DisjointSet *)0;
		numberOfMembers = 0;
	}
}

// initialize set
int
DisjointSets::initialize(int nmems)
{
	// delete old data
	if (numberOfMembers > 0)
	{
		delete [] dsets;
		dsets = (DisjointSet *)0;
		numberOfMembers = 0;
	}

	// allocate new data
	assert((numberOfMembers = nmems) >= 0);
	if (numberOfMembers > 0)
	{
		dsets = new DisjointSet[numberOfMembers];
		assert(dsets != (DisjointSet *)0);
		for (int im = 0; im < numberOfMembers; im++)
		{
			dsets[im].parent = im;
			dsets[im].rank = 1;
		}
	}

	// all done
	return(OK);
}

// assignment
DisjointSets &
DisjointSets::operator=(const DisjointSets &dss)
{
	// check for self-assignment
	if (this == &dss) return(*this);

	// delete old data
	if (numberOfMembers > 0)
	{
		delete [] dsets;
		dsets = (DisjointSet *)0;
		numberOfMembers = 0;
	}

	// copy new data
	assert((numberOfMembers = dss.numberOfMembers) >= 0);
	if (numberOfMembers > 0)
	{
		dsets = new DisjointSet[numberOfMembers];
		assert(dsets != (DisjointSet *)0);
		for (int im = 0; im < numberOfMembers; im++)
		{
			dsets[im].parent = dss.dsets[im].parent;
			dsets[im].rank = dss.dsets[im].rank;
		}
	}

	// all done
	return(*this);
}

// uion of two sets
int
DisjointSets::unionOf(DisjointSets &dss, int xmem, int ymem)
{
	return(link(dss, findSet(dss, xmem), findSet(dss, ymem)));
}

int
DisjointSets::link(DisjointSets &dss, int xmem, int ymem)
{
	// merge sets
	if (dss.dsets[xmem].rank > dss.dsets[ymem].rank)
	{
		dss.dsets[ymem].parent = xmem;
		dss.dsets[ymem].rank += dss.dsets[xmem].rank;
	}
	else
	{
		dss.dsets[xmem].parent = ymem;
		dss.dsets[xmem].rank += dss.dsets[ymem].rank;
	}

	// all done 
	return(OK);
}

// find set for a member
int
DisjointSets::findSet(DisjointSets &dss, int xmem)
{
	// find parent and compress path
	if (dss.dsets[xmem].parent != xmem)
	{
		dss.dsets[xmem].parent = findSet(dss, dss.dsets[xmem].parent);
	}

	// return parent
	return(dss.dsets[xmem].parent);
}

// print disjoint set data
void
DisjointSets::dump(ostream &os) const
{
	os << "dset = { " << endl;
	for (int im = 0; im < numberOfMembers; im++)
	{
		os << "(" << im << "," << dsets[im].parent << ")";
		os << endl;
	}
	os << "}" << endl;
	return;
}

ostream &
operator<<(ostream &os, const DisjointSets &dss)
{
	dss.dump(os);
	return(os);
}
