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
#include "node.h"
#include "nodeptr.h"
#include "refcount.h"

// best-first search routine with checking for redundant states,
// and using a termination predicate function.
//
template <class NodeType>
int
BestFirst_AOstar(const NodeType &);


#endif

