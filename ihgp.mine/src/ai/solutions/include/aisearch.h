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

#ifndef _AISEARCH_H_
#define _AISEARCH_H_

#include <stdio.h>
#include "ainodes.h"

/*
 * Search is a base class that implements the basics of unidirectional
 * search. From this class other classes that implement the actual
 * search algorithms such as breadth-first search and depth-first
 * are derived. The behaviour of the search, i.e. the type of the
 * search algorithm, depends on the implementation of virtual function
 * add(). Class Search processes objects of class Node and classes
 * derived from Node (DepthNode, UniNode, BestNode).
 *
 * Application-level classes that are derived from one the derivatives
 * of class Search must implement the following function:
 *
 * - is_goal(): determines if a node is a goal node,
 *              1: yes, 0: no.
 */
class Search
{
    public:
        Search(int numop, Node *start, Node *goal);
	virtual ~Search();
	virtual int is_goal(const Node *);

        int generate();				// generate solution
	void display() const;			// display solution
	IntrList<Node> *get_sol();		// get solution
	Node *get_goal() const;			// get goal node
	void set_startnode(Node *);		// set root node
	void set_goalnode(Node *);		// set goal
	void clear();				// clear state space
    private:
        Node *solve();				// search engine
        virtual int add(Node *) = 0;		// add node to gaph
	void print_sol(Node *) const;

        int num_op;		// number of operators used by the problem
	Node *goalnode,		// application specified goal node
	     *solgoal;		// actual goal node found by the search
    protected:
	SortedIntrList<Node> open,
		             closed;
};

#endif
