#ifndef __ADJLIST_H
#define __ADJLIST_H
// adjacency list representation definition

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"

// forward declarations
class AdjacencyList;
class AdjacencyListNode;

// definitions
#define NULLNODE -1

// list node
class AdjacencyListNode {
	// friend classes
	friend class AdjacencyList;

public:
	// constructors and destructor
	AdjacencyListNode();
	AdjacencyListNode(const AdjacencyListNode &);
	~AdjacencyListNode();

	// assignment
	AdjacencyListNode &operator=(const AdjacencyListNode &);
	
protected:
	AdjacencyListNode *next;
	int node;
};

// adjacency list class
class AdjacencyList {
public:
        // constructors and destructor
        AdjacencyList();
        AdjacencyList(const AdjacencyList &);
        virtual ~AdjacencyList();

        // assignment
        AdjacencyList &operator=(const AdjacencyList &);

        // basic graph operations
        int insert(int, int);
        int remove(int, int);
        int edgeExists(int, int);

        // miscellaneous
        virtual void dump(ostream & = cout) const;
        friend ostream &operator<<(ostream &, const AdjacencyList &);

protected:
        // internal data
	int numberOfNodes;
	AdjacencyListNode **head;
};

#endif
