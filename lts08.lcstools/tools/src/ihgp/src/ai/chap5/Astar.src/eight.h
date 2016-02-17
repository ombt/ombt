#ifndef __EIGHT_H
#define __EIGHT_H
// definitions for 8-puzzle node class used in A* search

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
class EightNode;

// node class definition
class EightNode: public RefCount {
public:
	// constructors and destructor
	EightNode();
	EightNode(const char [3][3]);
	EightNode(const EightNode &);
	virtual ~EightNode();

	// comparison operators
	virtual int operator<(const EightNode &) const;
	virtual int operator>(const EightNode &) const;
	virtual int operator==(const EightNode &) const;
	virtual int operator!=(const EightNode &) const;

	// assignment operator
	EightNode &operator=(const EightNode &);

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
	EightNode *getParent() const;
	void setParent(EightNode *);

	// get a pointer to children list
	List<NodePtr<EightNode> > *getChildren();

	// calculate heuristic value
	virtual int heuristic(const EightNode &, const List<EightNode> &);
	virtual int heuristic(const EightNode &);
	virtual int calculateGvalue();

	// expand node
	virtual int expand();

	// print out
	friend ostream &operator<<(ostream &, const EightNode &);

protected:
	// data specific to search algorithm
	int fvalue;
	int gvalue;
	int hvalue;
	EightNode *parent;
	List<NodePtr<EightNode> > children;

	// data specific to 8-puzzle problem
	int blankrow, blankcol;
	char board[3][3];
};

#endif

