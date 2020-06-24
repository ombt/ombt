/*
 * Copyright (C) 1995, 1996 Peter Bouthoorn.
 *
 * This software may be freely distributed and modified provided
 * this copyright message is left intact. The copyright message must be
 * included both with this (the original) software and with any modified
 * copies of this software or with any new software based on this software.
 * Furthermore any modified copies of this software must carry prominent
 * notices stating the software was changed and the date of any change.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. In no event
 * will the copyright holder be liable for any damage arising out of
 * the use of this software.
 *
 * As a matter of courtesy, the author requests to be informed about
 * any bugs found in this software and about improvements that may be of
 * general interest.
 *
 * Peter Bouthoorn
 * peter@obelix.icce.rug.nl
 */

#ifndef _AIGRAPH_H_
#define _AIGRAPH_H_

#include <stdio.h>
#include "aisearch.h"
#include "bisearch.h"
#include "ainodes.h"


/*
 * Class BreadtGraph implements a breadth-first search algorithm.
 * A search graph rather than a search tree is generated. Class
 * BreadthGraph processes objects of class Node.
 */
class BreadthGraph : public Search
{
    public:
        BreadthGraph(int op, Node *start = 0, Node *goal = 0);
        int add(Node *);
};


/*
 * Class DepthGraph implements a depth-first search algorithm,
 * with depth-bound. A search graph rather than a search tree is
 * generated. Class DepthGraph processes objects of class DepthNode.
 */
class DepthGraph : public Search
{
    public:
        DepthGraph(int op, Node *start = 0, Node *goal = 0);
        int add(Node *);
        void setdepth(unsigned);
        unsigned getdepth() const;
    private:
        unsigned maxdepth;
};


/*
 * Class BiBreadthGraph implements a bidirectional breadth-first
 * search algorithm. A search graph rather than a search tree is
 * generated. Class BiBreadthGraph processes objects of class Node.
 */
class BiBreadthGraph : public BiSearch
{
    public:
        BiBreadthGraph(int op, Node *start = 0, Node *goal = 0);
        int add(IntrList<Node> *, IntrList<Node> *, Node *);
};


/*
 * Class BiDepthGraph implements a bidirectional depth-first
 * search algorithm. A search graph rather than a search tree is
 * generated. Class BiDepthGraph processes objects of class Node.
 */
class BiDepthGraph : public BiSearch
{
    public:
        BiDepthGraph(int op, Node *start = 0, Node *goal = 0);
        int add(IntrList<Node> *, IntrList<Node> *, Node *);
};


/*
 * Class AStar implements the A* algorithm. A search graph graph rather
 * than a search tree is generated. Class Astar processes nodes of
 * type BestNode.
 * 
 * Application-level classes that are derived from this class must
 * implement the following functions (apart from the functions
 * defined virtual in class Search):
 *
 * - compute_g() : computes the G-value of a node.
 * - compute_h() : computes the H-value of a node.
 */
class AStar : public Search
{
    public:
        AStar(int op, Node *start = 0, Node *goal = 0);
        int add(Node *);
        virtual int compute_g(const Node *) = 0;
        virtual int compute_h(const Node *) = 0;
};


/*
 * UniCostGraph implements a uniform cost search algorithm. A search
 * graph rather than a search tree is generated. Class UniCostGraph
 * processes objects of class UniNode.
 *
 * Application-level classes that are derived from this class must
 * implement the following functions (apart from the functions
 * defined virtual in class Search):
 *
 * - compute_g() : computes the G-value of a node.
 */
class UnicostGraph : public Search
{
    public:
        UnicostGraph(int op, Node *start = 0, Node *goal = 0);
        int add(Node *);
        virtual int compute_g(const Node *) = 0;
};

#endif


