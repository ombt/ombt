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

#ifndef _BISEARCH_H_
#define _BISEARCH_H_

#include <stdio.h>
#include "ainodes.h"

/*
 * BiSearch is a base class that implements the basics of bidirectional
 * search. From this class other classes that implement the actual
 * search algorithms such as breadth-first search and depth-first
 * are derived. The behaviour of the search, i.e. the type of the
 * search algorithm, depends on the implementation of virtual function
 * add(). BiSearch processes objects of class Node.
 * Because a bidirectional search is performed two sets of lists
 * are used: one performs the forward search (s_open, s_closed) and the
 * other performs the backward search (t_open, t_closed).
 */ 
class BiSearch
{
    public:
        BiSearch(int op, Node *start, Node *goal);
	virtual ~BiSearch();

        int generate();				// generate solution
	void display() const;			// display solution
        IntrList<Node> *get_sol();		// get solution
        void set_startnode(Node *);		// set root node
        void set_goalnode(Node *);		// set goal node
        void clear();				// clear search space
     private:
	/* bisolve() decides in which direction the search proceeds */
        Node *bisolve();
        /* solve is the actual search engine */
        Node *solve(IntrList<Node> *, IntrList<Node> *, IntrList<Node> *);
        /* add() adds a node to the graph */
        virtual int add(IntrList<Node> *, IntrList<Node> *, Node *) = 0;
	void print_sol(Node *) const;
	void print_sol_2(Node *) const;

        int num_op,		// number of operators used by the problem
	    foundsol;
    protected:
        IntrList<Node> s_open,
                       s_closed,
                       t_open,
	               t_closed;
};

#endif

