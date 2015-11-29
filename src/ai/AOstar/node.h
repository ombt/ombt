#ifndef __NODE_H
#define __NODE_H
// definitions for node class used in AO* search

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

// status of node or connector
enum SolutionStatus {
	NotExpanded,
	Expanded,
	Solved,
	NotSolved,
	NoSolution
};

// forward definitions
class Node;
template <class DataType> class Connector;

// connector class definition
template <class DataType> class Connector: 
	public List<DataType> {
public:
	// constructors and destructor
	Connector():
		marked(0), status(NotSolved), hvalue(INT_MAX), 
		List<DataType>() {
		// do nothing
	}
	Connector(int m, SolutionStatus s, int hv):
		marked(m), status(s), hvalue(hv), List<DataType>() {
		// do nothing
	}
	Connector(const Connector &src):
		marked(src.marked), status(src.status), hvalue(src.hvalue),
		List<DataType>(src) {
		// do nothing
	}
	~Connector() {
		marked = 0;
		hvalue = INT_MAX;
		status = NoSolution;
	}

	// assignment
	Connector &operator=(const Connector &rhs) {
		if (this != &rhs)
		{
			marked = rhs.marked;
			status = rhs.status;
			hvalue = rhs.hvalue;
			List<DataType>::operator=(rhs);
		}
		return(*this);
	}

	// access functions
	int getMarked() const {
		return(marked);
	}
	void setMarked(int m) {
		marked = m;
	}

	// get/set node status.
	SolutionStatus getStatus() const
	{
		return(status);
	}
	void setStatus(SolutionStatus ns)
	{
		status = ns;
	}

	// get/set heuristic values functions
	int getHvalue() const
	{
		return(hvalue);
	}
	void setHvalue(int hv)
	{
		hvalue = hv;
	}

	// print out
	friend ostream &operator<<(ostream &os, const Connector &n)
	{
		os << "status: " << n.status << endl;
		os << "marked: " << n.marked << endl;
		os << "hvalue: " << n.hvalue << endl;
		return(os);
	}

protected:
	// new data
	SolutionStatus status;
	int marked;
	int hvalue;
};

// node class definition
class Node: public RefCount {
public:
	// constructors and destructor
	Node():
		hvalue(INT_MAX), status(NotExpanded),
		parents(), children()
	{
		// do nothing
	}
	Node(const Node &n):
		hvalue(n.hvalue), status(n.status),
		parents(), children()
	{
		// do nothing
	}
	virtual ~Node()
	{
		// do nothing
	}

	// comparison operators
	virtual int operator<(const Node &n) const
	{
		return(hvalue < n.hvalue);
	}
	virtual int operator>(const Node &n) const
	{
		return(hvalue > n.hvalue);
	}
	virtual int operator==(const Node &n) const
	{
		if (hvalue == n.hvalue)
			return(1);
		else
			return(0);
	}
	virtual int operator!=(const Node &n) const
	{
		return(!(operator==(n)));
	}

	// assignment operator
	Node &operator=(const Node &n)
	{
		if (this != &n)
		{
			// copy reference-counting data
			RefCount::operator=(n);

			// copy search-related data
			hvalue = n.hvalue;
			status = n.status;
			parents.clear();
			children.clear();
		}
		return(*this);
	}

	// functions to determine if node is a terminal node.
	virtual int isTerminal() const
	{
		return(0);
	}

	// get/set node status.
	SolutionStatus getStatus() const
	{
		return(status);
	}
	void setStatus(SolutionStatus ns)
	{
		status = ns;
	}

	// get/set heuristic values functions
	int getHvalue() const
	{
		return(hvalue);
	}
	void setHvalue(int hv)
	{
		hvalue = hv;
	}

	// get parent function
	List<NodePtr<Node> > *getParents()
	{
		return(&parents);
	}

	// get a pointer to children list 
	List<Connector<NodePtr<Node> > > *getChildren()
	{
		return(&children);
	}

	// calculate heuristic value
	virtual int heuristic()
	{
		hvalue = INT_MAX;
		return(OK);
	}

	// expand node
	virtual int expand()
	{
		return(OK);
	}

	// print out
	friend ostream &operator<<(ostream &os, const Node &n)
	{
		os << "hvalue: " << n.hvalue << endl;
		os << "status: " << n.status << endl;
		return(os);
	}

protected:
	// data specific to search algorithm
	int hvalue;
	SolutionStatus status;
	List<NodePtr<Node> > parents;
	List<Connector<NodePtr<Node> > > children;
};

#endif
