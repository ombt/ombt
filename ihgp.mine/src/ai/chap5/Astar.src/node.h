#ifndef __NODE_H
#define __NODE_H
// definitions for node class used in A* search

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
class Node;

// node class definition
class Node: public RefCount {
public:
	// constructors and destructor
	Node();
	Node(const Node &);
	virtual ~Node();

	// comparison operators
	virtual int operator<(const Node &) const;
	virtual int operator>(const Node &) const;
	virtual int operator==(const Node &) const;
	virtual int operator!=(const Node &) const;

	// assignment operator
	Node &operator=(const Node &);

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
	Node *getParent() const;
	void setParent(Node *);

	// get a pointer to children list 
	List<NodePtr<Node> > *getChildren();

	// calculate heuristic value
	virtual int heuristic(const Node &, const List<Node> &);
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

