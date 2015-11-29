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
#include "mylist.h"
#include "binaryTree_AVL.h"
#include "proxy.h"
#include "varnames.h"

// best-first search routine 
template <class NodeType, class DataType>
int
BestFirstSearch(List<NodeType> &, 
	BinaryTree_AVL<DataType> &, BinaryTree_AVL<DataType> &);

// depth-first search routine 
template <class NodeType, class DataType>
int
DepthFirstSearch(List<NodeType> &, 
	BinaryTree_AVL<DataType> &, BinaryTree_AVL<DataType> &);

// depth-first with hiil-climb search routine 
template <class NodeType, class DataType>
int
DepthFirstHillClimbSearch(List<NodeType> &, 
	BinaryTree_AVL<DataType> &, BinaryTree_AVL<DataType> &);

// breadth-first search routine 
template <class NodeType, class DataType>
int
BreadthFirstSearch(List<NodeType> &, 
	BinaryTree_AVL<DataType> &, BinaryTree_AVL<DataType> &);

// iterative deepening search routine 
template <class NodeType, class DataType>
int
IterativeDeepeningSearch(List<NodeType> &, 
	BinaryTree_AVL<DataType> &, BinaryTree_AVL<DataType> &);

#endif
