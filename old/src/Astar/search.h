#ifndef __SEARCH_H
#define __SEARCH_H
// heuristic search routines

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "list.h"

// best-first search routine without checking for redundant states,
// and with a set of goals states are passed in.
template <class NodeType>
int
BestFirst_Astar_WOCheck(const NodeType &, const List<NodeType> &, 
	int &, int &);

// best-first search routine without checking for redundant states,
// and using a termination predicate function.
template <class NodeType>
int
BestFirst_Astar_WOCheck(const NodeType &, int &, int &);

// best-first search routine with checking for redundant states,
// and with a set of goal states passed in. redundant states
// are just removed since g(node) is assumed to be larger.
template <class NodeType>
int
BestFirst_Astar_BasicCheck(const NodeType &, const List<NodeType> &, 
	int &, int &);

// best-first search routine with checking for redundant states,
// and using a termination predicate function. redundant states
// are just removed since g(node) is assumed to be larger.
template <class NodeType>
int
BestFirst_Astar_BasicCheck(const NodeType &, int &, int &);

// best-first search routine with checking for redundant states,
// and with a set of goal states passed in.
template <class NodeType>
int
BestFirst_Astar_Check(const NodeType &, const List<NodeType> &,
	int &, int &);

// best-first search routine with checking for redundant states,
// and using a termination predicate function.
template <class NodeType>
int
BestFirst_Astar_Check(const NodeType &, int &, int &);

#endif
