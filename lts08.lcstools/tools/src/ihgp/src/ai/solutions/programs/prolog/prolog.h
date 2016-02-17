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

#ifndef _PROLOG_H_
#define _PROLOG_H_

#include <stdio.h>
#include "btrack.h"
#include "pclause.h"
#include "ptable.h"

class Prolog;

/*
 * Class PrologNode represents nodes in the search tree. It contains
 * the goal stack, binding information and a reference to the clause
 * that was last visited.
 */
class PrologNode : public BackNode
{
    friend Prolog;
    public:
	PrologNode();
	PrologNode(PtrList<Complex> *);
	~PrologNode();

	int operator==(const BackNode &) const;
        void display() const;
        BackNode *expand_one(int);
    private:
	PtrList<Complex> *goalstack;
	Substitution subst;
	int lastdone;
};

/*
 * Class Prolog represents the search technique. In this case,
 * as will be expected, a backtracting algorithm is selected.
 * Special functions are defined to enter queries and to start the
 * search and extract the results.
 */
class Prolog : public BackTrack
{
    friend PrologNode;
    public:
        Prolog(PrologTable &);
	int is_goal(const BackNode *);
	void set_query(const char *);
	int get_result(Substitution &);
	int get_next_result(Substitution &);
    private:
	static PrologTable *table;
};

#endif


