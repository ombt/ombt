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

#include "prolog.h"

PrologTable *Prolog::table;

PrologNode::PrologNode()
{
    lastdone = -1;
}

PrologNode::~PrologNode()
{
    delete goalstack;
}

/*
 * Note that the argument passed to this function must point to
 * to allocated memory.
 */
PrologNode::PrologNode(PtrList<Complex> *solveme)
{
    goalstack = solveme;
    lastdone = -1;

    /*
     * Rename all variables in the goal stack so as to make them unique.
     * Store the original and new values of the variables in the
     * substitution so that the original variables with their
     * bindings can be returned when the solution has been found.
     */
    PtrListIterator<Complex> iter(*goalstack);
    for (Complex *c = iter.getfirst(); c; c = iter.getnext())
	c->update_vars(subst);
}

int PrologNode::operator==(const BackNode &other) const
{
    const PrologNode &pother = (PrologNode &)other;
    if (!(*goalstack == *pother.goalstack))
	return(0);
    if (!(subst == pother.subst))
	return(0);
    return(lastdone == pother.lastdone);
}

void PrologNode::display() const
{
    cout << subst << "\n";
}

/*
 * The procedure for expanding a PrologNode is fairly simple. The Prolog
 * rules and facts are searched for one that matches the top goal of the
 * goal stack. If any new goals are introduced by this match (this
 * will happen if a rule instead of a simple fact was involved in the
 * match) these goals are placed on top of the goal stack. Because the
 * new goals are placed on top of the stack they will be solved before the
 * old goals, which is exactly the way Prolog works.
 */
BackNode *PrologNode::expand_one(int)
{
    Substitution subst_used;
    PtrList<Complex> *newgoals;
    PrologNode *succ;
    Complex *first;

    newgoals = new PtrList<Complex>;

    /* Get top goal from goal stack. */
    first = goalstack->gethead();

    /*
     * Search the Prolog database for a match, starting at the entry
     * following the one that was last visited.
     */
    lastdone = Prolog::table->findmatch(lastdone + 1, *first, *newgoals,
				        subst_used);

    /* If no match could be find there are no successor nodes. */
    if (lastdone < 0)
    {
        succ = NULL;
        delete newgoals;
    }
    else
    {
	PtrListIterator<Complex> iter(*goalstack);
	Complex *c, *d;

        /* Create a successor node. */
	succ = new PrologNode;

	/*
	 * The new goals (if any) must be solved before the old goals:
	 * place them on top of the successor's stack.
	 */
	succ->goalstack = newgoals;

	/*
	 * Copy the old goal stack, minus the first element which is
	 * now solved and must be popped off, to the goal stack of
	 * the successor. All variables occurring in the old goals
	 * must be updated with the new binding information that was
	 * obtained from the match with the clause in the Prolog database.
         */
	for (iter.getfirst(), c = iter.getnext(); c; c = iter.getnext())
	{
            d = c->clone();
	    d->update_vars(subst_used);
	    succ->goalstack->addtotail(d);
        }

	/*
	 * Copy the substitution to the successor and update it with
         * the new binding information.
	 */
	succ->subst = subst;
	succ->subst.update(subst_used);
    }

    return(succ);
}

Prolog::Prolog(PrologTable &pt)
    : BackTrack(0)
{
    table = &pt;
}

int Prolog::is_goal(const BackNode *node)
{
    const PrologNode *n = (PrologNode *)node;

    /* The node is a goal node if its stack is empty. */
    return(n->goalstack->getcount() == 0);
}

/*
 * set_query() creates a new goal stack and initializes the search
 * tree.
 */
void Prolog::set_query(const char *str)
{
    PtrList<Complex> *goal;
    PrologNode *start;

    /* Create new PrologNode and new goal stack. */
    goal = new PtrList<Complex>;
    goal->addtohead(new Complex(str));
    start = new PrologNode(goal);

    /* Empty the search tree and insert a new start node. */
    clear();
    set_startnode(start);
}

/*
 * get_result() starts the search process. If a solution is found
 * 1 is returned and 0 otherwise. The bindings that were needed to
 * satisfy the goal, if any, are returned in the supplied substitution.
 */
int Prolog::get_result(Substitution &answer)
{
    if (generate())
    {
	const PrologNode *sol = (PrologNode *)get_goal();
	answer = sol->subst;
	return(1);
    }
    return(0);
}

/*
 * get_next_result() restarts the search process and returns the
 * next solution if one could be found.
 */
int Prolog::get_next_result(Substitution &answer)
{
    if (generate_next())
    {
	const PrologNode *sol = (PrologNode *)get_goal();
	answer = sol->subst;
	return(1);
    }
    return(0);
}


