#ifndef __QUEEN_H
#define __QUEEN_H
// definitions for N-queen node class used in A* search

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
class NQueenNode;

// node class definition
class NQueenNode: public RefCount {
public:
	// constructors and destructor
	NQueenNode();
	NQueenNode(int, int, int);
	NQueenNode(const NQueenNode &);
	virtual ~NQueenNode();

	// comparison operators
	virtual int operator<(const NQueenNode &) const;
	virtual int operator>(const NQueenNode &) const;
	virtual int operator==(const NQueenNode &) const;
	virtual int operator!=(const NQueenNode &) const;

	// assignment operator
	NQueenNode &operator=(const NQueenNode &);

	// board accessor function
	char &operator()(int, int);
	const char &operator()(int, int) const;

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
	NQueenNode *getParent() const;
	void setParent(NQueenNode *);

	// get a pointer to children list 
	List<NodePtr<NQueenNode> > *getChildren();

	// calculate heuristic value
	virtual int heuristic(const NQueenNode &, const List<NQueenNode> &);
	virtual int heuristic(const NQueenNode &);
	virtual int calculateGvalue();

	// expand node
	virtual int expand();

	// print out
	friend ostream &operator<<(ostream &, const NQueenNode &);

protected:
	// internal definitions
	enum Characters {
		SpaceChar = ' ',
		QueenChar = 'Q',
		AttackedChar = 'X'
	};

protected:
	// data specific to search algorithm
	int fvalue;
	int gvalue;
	int hvalue;
	NQueenNode *parent;
	List<NodePtr<NQueenNode> > children;

	// data specific to N-queen problem
	int nqueens, queens, nrows, ncols;
	char *board;
};

#endif

