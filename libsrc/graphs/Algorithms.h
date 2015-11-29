#ifndef __GRAPH_ALGORITHMS_H
#define __GRAPH_ALGORITHMS_H

#include <Debug.h>
#include <Returns.h>
#include <graphs/Graph.h>

namespace ombt {

void depthFirstTraversal(const Graph &graph, bool verbose = false);
void depthFirstTraversal(const Graph &graph, Graph::VertexId vid, bool verbose = false);
void depthFirstTraversal2(const Graph &graph, bool verbose = false);

void breadthFirstTraversal(const Graph &graph, bool verbose = false);
void breadthFirstTraversal(const Graph &graph, Graph::VertexId vid, bool verbose = false);
void breadthFirstTraversal2(const Graph &graph, bool verbose = false);

}

#endif
