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

#include "btrack.h"

/*
 * The constructor puts the start node on the stack, saves the goal node
 * so that goal checking can take place and sets the number of operators
 * to the specified value.
 */
BackTrack::BackTrack(int op, BackNode *start, BackNode *goal)
{
    if (start)
        stack.addtohead(start);
    goalnode = goal;
    num_op = op;
    depth = do_lookup = 0;
}


/*
 * The destructor only needs to remove the goal node that was stored
 * by the constructor. The destruction of OPEN and CLOSED is taken care
 * of automatically by the destructors of the list objects.
 */
BackTrack::~BackTrack()
{
    delete(goalnode);
}


/*
 * is_goal() checks if the specified node is a goal node. Note that
 * the equality operator used to do the checking is a pure virtual
 * function, so that the method used to compare the nodes is defined
 * by the problem at hand.
 */
int BackTrack::is_goal(const BackNode *node)
{
#ifdef DEBUG
    if (!node)
    {
	puts("BackTrack::is_goal() called with 0 (NULL) argument - did you forget to implement is_goal()?");
        exit(0);
    }
#endif
    return(*goalnode == *node);
}


/*
 * setdepth() is called by the application to limit the depth of
 * of the search.
 */
void BackTrack::setdepth(int dpth)
{
    depth = dpth;
}


/*
 * setcheck() is called by the application to enable or disable 
 * the cycle check.
 */
void BackTrack::setcheck(int check)
{
    do_lookup = check;
}


/*
 * display() prints the solution path.
 */
void BackTrack::display() const
{
    ROIntrListIterator<BackNode> iter(stack);

    for (const BackNode *n = iter.getfirst(); n; n = iter.getnext())
	n->display();
}


/*
 * get_goal() returns the solution node only, i.e. the last node
 * generated by the search (assuming the search was successfull). A const
 * pointer is returned because if the node were modified this would affect
 * the stack. This in turn affects the consistency of the search on
 * the next call of generate_next().
 */
const BackNode *BackTrack::get_goal() const
{
    return(stack.gettail());
}


/*
 * get_sol() returns the solution path. For the same reason as in
 * get_goal() a const pointer is returned.
 */
const IntrList<BackNode> *BackTrack::get_sol()
{
    return(&stack);
}


/*
 * set_goalnode() saves the goal node of the problem. This function
 * can be called if passing the goal node through the constructor of
 * class BackTrack is for some reason not feasible or if the search
 * space has been cleared by clear() and a new search must be started.
 */
void BackTrack::set_goalnode(BackNode *node)
{
    goalnode = node;
}


/*
 * set_startnode() adds the node used to start the search on open.
 * This function can be called if passing the start node through the
 * constructor of class BackTract is for some reason not feasible or if
 * the search space has been cleared by clear() and a new search must be
 * started.
 */
void BackTrack::set_startnode(BackNode *node)
{
    stack.addtohead(node);
}


/*
 * clear() clears the state space so that the object can be reused for a
 * new problem.
 */
void BackTrack::clear()
{
    stack.clear(DoDel);
    delete goalnode;
    goalnode = 0;
}


/*
 * generate() starts the search.
 * generate() returns 1 if a solution was found and 0 otherwise.
 */
int BackTrack::generate()
{
    return(solve());
}


/*
 * generate_next() restarts the search, generating a new solution.
 * generate_next() returns 1 if a solution was found and 0 otherwise.
 */
int BackTrack::generate_next()
{
    /*
     * Remove the goal node that was produced on the last run and
     * restart the solution process.
     */
    stack.remove_tail(DoDel);
    return(solve());
}


/*
 * solve() is the actual search engine. The steps taken by this
 * routine are:
 *
 * 1. Get the last node from the stack. If the stack is empty the
 *    search ends with failure.
 * 2. Check if the node is a goal node. If true, the search ends
 *    with success. 
 * 3. Check if the depth bound has been reached. If true, remove the
 *    node from the stack, i.e. backtrack, and go to step 1.
 * 4. Expand the node by generating ONE of it's successors. If there are
 *    no more successors left (all operators have been applied) remove
 *    the node from the stack, i.e. backtrack, and go to step 1.
 * 5. Add the successor to the stack, optionally after checking if it's
 *    not already on the stack.
 *
 * Solve returns 1 if a solution is found and 0 otherwise.
 */
int BackTrack::solve()
{
     BackNode
        *father,
        *child;

    while((father = stack.gettail()) != 0)
    {
        if (is_goal(father))		 // found goal node
	    return(1);
	if (stack.getcount() == depth)   // reached depth bound
	    stack.remove_tail(DoDel);	 // backtrack
	else
	{
	    child = father->expand_one(num_op);

	    if (!child)				// no more successors
	        stack.remove_tail(DoDel);	// backtrack
	    else
	    {
	        if (do_lookup && stack.lookup(child))	// node already visited
	            delete child;
		else
		{
	            stack.addtotail(child);
#ifdef DEBUG
		    puts("adding node:");
		    child->display();
		    putchar('\n');
#endif
		}
	    }
	}
    }
    return(0);
}

