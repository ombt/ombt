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
 * operators to the constructor class Search.
 */
UnicostGraph::UnicostGraph(int op, Node *start, Node *goal)
    :Search(op, start, goal)
{
}


/*
 * add() adds a node to the graph after checking if it's not already
 * in the graph. A uniform-cost search is established by inserting
 * the node into a ordered list based on its G-value.
 * If the node is found to be already on OPEN the G-values of the old and
 * current node are compared and the node with the best G-value is kept.
 * If the node is found to be already on CLOSED there's no need to check
 * which of the nodes is better because we are guaranteed to always find
 * the cheapest node first (see Nilsson p.53).
 */
int UnicostGraph::add(Node *succ)
{
    UniNode
	*parent,
        *old = 0,
        *bsucc = (UniNode *)succ;

    int
        g;

    /*
     * Compute and set the node's G-value: get the G-value of the node's
     * parent (this represents the overall cost of the solution path
     * generated so far) and add to this the cost of getting from the
     * parent node to this node.
     */
    parent = (UniNode *)bsucc->getparent();
    g = parent->get_g() + compute_g(bsucc);
    bsucc->set_g(g);


    /* Check if the node is already on OPEN. */
    if ((old = (UniNode *)open.lookup(succ)) != 0)
    {
	/* The node is already on OPEN, compare G-values. */
        if (*bsucc < *old)             	     // new node better
        {
            open.remove_found(DoDel);        // remove old node from OPEN
            open.insert(bsucc);		     // insert new node
            return(1);
        }
        return(0);
    }

    /* Check if the node is already on CLOSED. */
    if ((old = (UniNode *)closed.lookup(succ)) != 0)
        return(0);

    /* The node is on neither OPEN nor CLOSED: add it to OPEN. */
    open.insert(bsucc);
    return(1);
}
