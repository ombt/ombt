#ifndef __BFSNODE_H
#define __BFSNODE_H
// definitions for theorem-prover node class used in A* search

// headers
#include <stdlib.h>
#include <iostream.h>
#include <limits.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "mylist.h"
#include "binaryTree_AVL.h"
#include "proxy.h"
#include "literal.h"
#include "clause.h"
#include "statistics.h"
#include "globals.h"
#include "deletion.h"
#include "substitution.h"
#include "options.h"
#include "memoryleak.h"

// forward definitions
class BFSNode;

// node class definition
class BFSNode {
public:
	// constructors and destructor
	BFSNode();
	BFSNode(const Literal &, const Literal &,
		const Clause &, const Clause &);
	BFSNode(const BFSNode &);
	~BFSNode();

	// comparison operators
	int operator==(const BFSNode &) const;
	int operator!=(const BFSNode &) const;
	int operator<(const BFSNode &) const;
	int operator<=(const BFSNode &) const;
	int operator>(const BFSNode &) const;
	int operator>=(const BFSNode &) const;

	// assignment operator
	BFSNode &operator=(const BFSNode &);

	// function to determine if node is a goal.
	virtual int isGoal(BinaryTree_AVL<Clause> &, BinaryTree_AVL<Clause> &);

	// get/set heuristic values functions
	int getHvalue() const;
	int getFvalue() const;
	int getGvalue() const;
	int getDepth() const;
	void setHvalue(int);
	void setFvalue(int);
	void setGvalue(int);
	void setDepth(int);

	// return clauses
	String getClauses() const;
	String getNormalizedClauses() const;

	// get/set parent functions
	Proxy<BFSNode> getParent() const;
	void setParent(Proxy<BFSNode> &);

	// get a pointer to children list 
	List<Proxy<BFSNode> > *getChildren();

	// calculate heuristic value
	int heuristic(BinaryTree_AVL<Clause> &, BinaryTree_AVL<Clause> &);
	int calculateGvalue();

	// expand node
	int expand(BinaryTree_AVL<Clause> &, BinaryTree_AVL<Clause> &);

	// return if a query or not
	int getQuery() const {
		return(resolventclause.getQuery());
	}

#ifdef MEMORYLEAK
	// overload new and delete for memory-leak detection
	void *operator new(size_t);
	void operator delete(void *);
#endif

	// print out
	friend ostream &operator<<(ostream &, const BFSNode &);
	void dumpMinData(ostream &) const;

protected:
	// data specific to search algorithm
	int depth;
	int fvalue;
	int gvalue;
	int hvalue;
	Proxy<BFSNode> parent;
	List<Proxy<BFSNode> > children;

	// data specific to theorem prover
	int resolved;
	Literal lit1;
	Clause lit1clause;
	Clause factoredlit1clause;
	Substitutions factoredlit1subs;
	Literal lit2;
	Clause lit2clause;
	Clause factoredlit2clause;
	Substitutions factoredlit2subs;
	Clause resolventclause;
	Substitutions substitutions;

public:
#ifdef MEMORYLEAK
	// memory leak data
	static MemoryLeak memoryleak;
#endif
};

#endif

