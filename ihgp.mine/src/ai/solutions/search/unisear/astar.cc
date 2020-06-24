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

#include "aigraph.h"

/*
 * The constructor passes the start node, goal node and number of
 * operators to the constructor of class Search.
 * Because the start node will be the first node to be expanded
 * its F-value does not really matter and will not be computed.
 */
AStar::AStar(int op, Node *start, Node *goal)
    :Search(op, start, goal)
{   
}


/*
 * add() adds a node to the graph after checking if it's not already
 * in the graph. A best-first search (or more precisely: A*) is established
 * by inserting the node into an ordered list based on its F-value (F =
 * G + H).
 * After the node's G-value and F-value have been computed add() checks
 * if the node is already on OPEN. If so, the F-values of the old node
 * and the current node are compared. If the new node is better the old
 * node is removed from OPEN and the new node is inserted into its place.
 * If the old node is better nothing happens. If the node is not on OPEN
 * the same procedure is applied with respect to CLOSED: if the new node
 * is better the old node is removed from CLOSED and the new node is
 * added to OPEN; if the old node is better nothing happens. A node that is
 * neither on OPEN nor on CLOSED is simply added to OPEN.
 * Note that a disadvantage of this implementation is that if a node is
 * found to be already on CLOSED and if it's better than the old node it
 * will re-inserted to OPEN, meaning that the node may eventually be expanded
 * a second time, which may result in a lot of work being done twice. A
 * solution for this problem is described by Rich "Artificial Intelligence"
 * p.80+81 (1st edition). But note that in the algorithm that Ritch describes
 * the nodes have a pointer to their successors beside the parent pointer,
 * meaning that the algorithm that is used here would have to be changed
 * considerably to support this solution.
 */
int AStar::add(Node *succ)
{
    BestNode
	*parent,
        *old = 0,
        *bsucc = (BestNode *)succ;

    int
        g;

    parent = (BestNode *)bsucc->getparent();

    /*
     * Compute and set the node's G-value: get the G-value of the node's
     * parent (this represents the overall cost of the solution path
     * generated so far) and add to this the cost of getting from the
     * parent node to this node.
     */
    g = parent->get_g() + compute_g(bsucc);
    bsucc->set_g(g);

    /*
     * Compute and set the node's F-value: get the node's G-value and
     * and add to this the heuristic estimate of the value of the node,
     * i.e. H.
     */
    bsucc->set_f(g + compute_h(bsucc));

    /* Check if the node is already on OPEN. */
    if ((old = (BestNode *)open.lookup(succ)) != 0)
    {
        /* The node is already on OPEN, compare F-values. */
        if (*bsucc < *old)            	     // new node better
        {
            open.remove_found(DoDel);        // remove old node from OPEN
            open.insert(bsucc);              // insert new node
            return(1);
        }
        return(0);
    }

    /* Check if the node is already on CLOSED. */
    if ((old = (BestNode *)closed.lookup(succ)) != 0)
    {
        /* The node is already on CLOSED, compare F-values. */
        if (*bsucc < *old)                  // new node better
        {
            closed.remove_found(DoDel);     // remove old node from CLOSED
            open.insert(bsucc);		    // insert new node to OPEN
            return(1);
        }
        return(0);
    }

    /* The node is on neither OPEN nor CLOSED: add it to OPEN. */
    open.insert(bsucc);
    return(1);
}

