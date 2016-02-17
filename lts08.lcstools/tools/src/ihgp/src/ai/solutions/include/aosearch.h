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

#ifndef _AOSEARCH_H_
#define _AOSEARCH_H_

#include <stdio.h>
#include "ainodes.h"

/*
 * AOSearch is a base class that implements the basics of AND/OR
 * search. From this class other classes that implement the actual
 * search algorithms such as breadth-first search and depth-first
 * are derived. The behaviour of the search, i.e. the type of the
 * search algorithm, depends on the implementation of virtual function
 * add(). AOSearch processes objects of class AONode and classes
 * derived from AONode (ANDNode and ORNode).
 *
 * Application-level classes that are derived from one of AOSearch
 * derivatives must implement the following function:
 *
 * - is_terminal():  determines if a node is a terminal node,
 *                   1 : yes, 0 : no.
 */
class AOSearch
{
    public:
        AOSearch(int numop, AONode *start);
        virtual ~AOSearch();
        virtual int is_terminal(const AONode *) = 0;

        int generate();			// generate solution
	void display();			// display solution
	IntrList<Node> *get_sol();	// get solution
	void set_startnode(AONode *);	// set root node of search
	void clear();			// clear search space
    private:
        int solve();			// search engine
        virtual int add(AONode *) = 0;	// add node to graph
        int solvable(AONode *);		// solved labelling procedure
        int unsolvable(AONode *);	// unsolvable labelling procedure
        void prune(NodeStat);		// prune nodes from OPEN
        void print_sol(AONode *);
	void addnodetosol(IntrList<Node> *, AONode *);
        int deletable(AONode *, NodeStat);

        int
            num_op;		// number of operators used by the problem
        AONode
              *startnode;
    protected:
	IntrList<AONode> open,
	                 closed;	
};

#endif
