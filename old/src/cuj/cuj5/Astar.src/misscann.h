#ifndef __MISSCANN_H
#define __MISSCANN_H
// definitions for n-missionary-cannibal node class used in A* search

// headers
#include <stdlib.h>
#include <iostream.h>
#include <limits.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "list.h"
#include "refcount.h"
#include "nodeptr.h"

// forward definitions
class NMissCannNode;

// node class definition
class NMissCannNode: public RefCount {
public:
	// constructors and destructor
	NMissCannNode();
	NMissCannNode(int, int, int = 0, int = 0, char = 'L');
	NMissCannNode(const NMissCannNode &);
	virtual ~NMissCannNode();

	// comparison operators
	virtual int operator<(const NMissCannNode &) const;
	virtual int operator>(const NMissCannNode &) const;
	virtual int operator==(const NMissCannNode &) const;
	virtual int operator!=(const NMissCannNode &) const;

	// assignment operator
	NMissCannNode &operator=(const NMissCannNode &);

	// function to determine if node is a goal.
	virtual int isGoal() const;

	// get/set heuristic values functions
	int getHvalue() const;
	int getFvalue() const;
	int getGvalue() const;
	void setHvalue(int);
	void setFvalue(int);
	void setGvalue(int);

	// get/set parent functions
	NMissCannNode *getParent() const;
	void setParent(NMissCannNode *);

	// get a pointer to children list 
	List<NodePtr<NMissCannNode> > *getChildren();

	// calculate heuristic value
	virtual int heuristic(const NMissCannNode &, const List<NMissCannNode> &);
	virtual int heuristic(const NMissCannNode &);
	virtual int calculateGvalue();

	// expand node
	virtual int expand();

	// print out
	friend ostream &operator<<(ostream &, const NMissCannNode &);

protected:
	// data specific to search algorithm
	int fvalue;
	int gvalue;
	int hvalue;
	NMissCannNode *parent;
	List<NodePtr<NMissCannNode> > children;

	// data specific to missionary-cannibal problem
	int maxNumber;
	int LHSmissionaries, LHScannibals;
	int RHSmissionaries, RHScannibals;
	char BOATside;
};

#endif

