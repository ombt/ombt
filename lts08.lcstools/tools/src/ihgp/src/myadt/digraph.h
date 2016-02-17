#ifndef __DIGRAPH_H
#define __DIGRAPH_H
// directed graph class definition

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "data.h"
#ifdef ADJACENCYLIST
#include "adjlist.h"
#else
#include "adjmatrix.h"
#endif

// forward declarations
class DirectedGraph;

// list class
class DirectedGraph {
public:
        // constructors and destructor
        DirectedGraph();
        DirectedGraph(const Graph &);
        virtual ~DirectedGraph();

        // assignment
        Graph &operator=(const Graph &);

        // basic graph operations
        friend int insert(Graph &, int, int);
        friend int remove(Graph &, int, int);
        friend int edgeExists(Graph &, int, int);

        // miscellaneous
        virtual void dump(ostream & = cout) const;
        friend ostream &operator<<(ostream &, const DirectedGraph &);

protected:
        // internal data
#ifdef ADJACENCYLIST
	AdjacencyList edges;
#else
	AdjacencyMatrix edges;
#endif
};

#endif
