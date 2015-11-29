#ifndef __COINS_H
#define __COINS_H
// definitions for coin puzzle node class used in A* search

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
class CoinsNode;

// node class definition
class CoinsNode: public RefCount {
public:
	// constructors and destructor
	CoinsNode();
	CoinsNode(int);
	CoinsNode(const CoinsNode &);
	virtual ~CoinsNode();

	// comparison operators
	virtual int operator<(const CoinsNode &) const;
	virtual int operator>(const CoinsNode &) const;
	virtual int operator==(const CoinsNode &) const;
	virtual int operator!=(const CoinsNode &) const;

	// assignment operator
	CoinsNode &operator=(const CoinsNode &);

	// board accessor function
	char &operator[](int);
	const char &operator[](int) const;

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
	CoinsNode *getParent() const;
	void setParent(CoinsNode *);

	// get a pointer to children list
	List<NodePtr<CoinsNode> > *getChildren();

	// calculate heuristic value
	virtual int heuristic(const CoinsNode &, const List<CoinsNode> &);
	virtual int heuristic(const CoinsNode &);
	virtual int calculateGvalue();

	// expand node
	virtual int expand();

	// print out
	friend ostream &operator<<(ostream &, const CoinsNode &);

protected:
	// data specific to search algorithm
	int fvalue;
	int gvalue;
	int hvalue;
	CoinsNode *parent;
	List<NodePtr<CoinsNode> > children;

	// data specific to coins-puzzle problem
	int number, space;
	char *board;
};

#endif

