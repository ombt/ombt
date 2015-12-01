#ifndef __SEARCH_H
#define __SEARCH_H
// heuristic search routines

// headers
#include <stdlib.h>
#include <iostream>
#include <assert.h>

// local headers
#include <Returns.h>
#include <Debug.h>
#include <adt/List.h>
#include <adt/BinaryTree_AVL.h>
#include <adt/Proxy.h>
#include "varnames.h"

using namespace std;
using namespace ombt;

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
