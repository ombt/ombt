// member functions for adjacency list class

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>

// local headers
#include "adjlist.h"

// adjacency list item constructors and destructor
AdjacencyListNode::AdjacencyListNode():
	next((AdjacencyListNode *)0), node(-1)
{
	// do nothing
}

AdjacencyListNode::AdjacencyListNode(const AdjacencyListNode &src):
	next((AdjacencyListNode *)0), node(src.node)
{
	// do nothing
}

AdjacencyListNode::~AdjacencyListNode()
{
	next = (AdjacencyListNode *)0;
	node = -1;
}

// assignment and equivalence
AdjacencyListNode &
AdjacencyListNode::operator=(const AdjacencyListNode &rhs)
{
	// check for self-assignment
	if (this != &rhs)
	{
		// copy node, not pointer
		node = rhs.node;
		next = (AdjacencyListNode *)0;
	}
	return(*this);
}

// adjacency list contructors and destructor
AdjacencyList::AdjacencyList():
	numberOfNodes(DEFAULTNUMBEROFNODES), 
	head(new AdjacencyListNode *[DEFAULTNUMBEROFNODES])
{
	// check that headers list was allocated
	assert(head != (AdjacencyListNode **)0);

	// initialize array
	for (int in = 0; in < numberOfNodes; in++)
	{
		head[in] = (AdjacencyListNode *)0;
	}
}

AdjacencyList::AdjacencyList(const AdjacencyList &src):
	numberOfNodes(src.numberOfNodes), 
	head(new AdjacencyListNode *[src.numberOfNodes])
{
	// check that headers list was allocated
	assert(head != (AdjacencyListNode **)0);

	// initialize array
	for (int in = 0; in < numberOfNodes; in++)
	{
		// check if a list exists
		if (src.head[in] == (AdjacencyListNode *)0)
		{
			head[in] = (AdjacencyListNode *)0;
			continue;
		}

		// copy list of nodes 
		AdjacencyListNode *pnode = src.head[in];
		AdjacencyListNode **pnext = &head[in];
		for ( ; pnode != (AdjacencyListNode *)0; pnode = pnode->next)
		{
			
		}
	}
}
