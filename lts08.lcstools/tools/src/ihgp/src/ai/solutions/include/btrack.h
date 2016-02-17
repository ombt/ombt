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

#ifndef _BTRACK_H_
#define _BTRACK_H_

#include <stdio.h>
#include "ainodes.h"

/*
 * class BackTrack implements a backtracking algorithm. Unlike class
 * Search no higher level classes are derived from this class, except
 * for application-level classes. BackTrack generates multiple
 * solutions.
 *
 * Application-level classes that are derived from Search derivatives
 * must implement the following function:
 *
 * - is_goal(): determines if a node is a goal node,
 *              1: yes, 0: no.
 */
class BackTrack
{
    public:
	BackTrack(int n, BackNode *start = 0, BackNode *goal = 0);
	virtual ~BackTrack();
	virtual int is_goal(const BackNode *);

	int generate();				// generate solution
	int generate_next();			// generate next solution
	void display() const;			// display solution
	const IntrList<BackNode> *get_sol();	// get solution
	const BackNode *get_goal() const;	// get goal
	void setdepth(int);			// set depth of search
	void setcheck(int);			// disable or enable cycle check
	void set_startnode(BackNode *);		// set root node
	void set_goalnode(BackNode *);		// set goal
	void clear();				// clear state space
    private:
	int solve();				// search engine

	IntrList<BackNode> stack;
	BackNode *goalnode;
	int num_op,		// number of operators used by the problem
	    do_lookup,		// cycle check: on or off
	    depth;		// depth bound
};

#endif
