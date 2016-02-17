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

#ifndef _AITREE_H_
#define _AITREE_H_

#include <stdio.h>
#include "ainodes.h"
#include "aisearch.h"
#include "bisearch.h"
#include "aosearch.h"


/*
 * Class BreadtTree implements a breadth-first search algorithm.
 * A search tree rather than a search graph is generated. Class
 * BreadthTree processes objects of class Node.
 */
class BreadthTree : public Search
{
    public:
        BreadthTree(int op, Node *start = 0, Node *goal = 0);
        int add(Node *);
};


/*
 * Class DepthTree implements a depth-first search algorithm,
 * with depth-bound. A search tree rather than a search graph is
 * generated. Class DepthTree processes objects of class DepthNode.
 */
class DepthTree : public Search
{
    public:
        DepthTree(int op, Node *start = 0, Node *goal = 0);
        int add(Node *);
	void setdepth(unsigned);
	unsigned getdepth() const;
    private:
	unsigned maxdepth;
};


/*
 * UniCostTree implements a uniform cost search algorithm. A search
 * tree rather than a search graph is generated. Class UniCostTree
 * processes objects of class UniNode.
 *
 * Application-level classes that are derived from this class must
 * implement the following functions (apart from the functions
 * defined virtual in class Search):
 *
 * - compute_g() : computes the G-value of a node.
 */
class UnicostTree : public Search
{
    public:
        UnicostTree(int op, Node *start = 0, Node *goal = 0);
        int add(Node *);
        virtual int compute_g(const Node *) = 0;
};


/*
 * Class BiDepthTree implements a bidirectional depth-first
 * search algorithm. A search tree rather than a search graph is
 * generated. Class BiDepthTree processes objects of class Node.
 */
class BiDepthTree : public BiSearch
{
    public:
        BiDepthTree(int op, Node *start = 0, Node *goal = 0);
        int add(IntrList<Node> *, IntrList<Node> *, Node *);
};


/*
 * Class BiBreadthTree implements a bidirectional breadth-first
 * search algorithm. A search tree rather than a search graph is
 * generated. Class BiBreadthTree processes objects of class Node.
 */
class BiBreadthTree : public BiSearch
{
    public:
        BiBreadthTree(int op, Node *start = 0, Node *goal = 0);
        int add(IntrList<Node> *, IntrList<Node> *, Node *);
};



/*
 * AODepthTree implements a depth-first AND-OR search. A search tree
 * rather than a search graph is generated. Class AODepthTree processes
 * objects of class AONode.
 */
class AODepthTree : public AOSearch
{
    public:
        AODepthTree(int op, AONode *start = 0);
        int add(AONode *);
};


/*
 * AOBreadthTree implements a breadth-first AND-OR search. A search tree
 * rather than a search graph is generated. Class AOBreadthTree processes
 * objects of class AONode.
 */
class AOBreadthTree : public AOSearch
{
    public:
        AOBreadthTree(int op, AONode *start = 0);
        int add(AONode *);
};

#endif
