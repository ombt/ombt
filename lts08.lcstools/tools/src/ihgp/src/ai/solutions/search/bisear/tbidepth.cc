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

#include "aitree.h"

/*
 * The constructor passes the start node, goal node and the number
 * of operators to the constructor of class BiSearch.
 */
BiDepthTree::BiDepthTree(int op, Node *start, Node *goal)
    :BiSearch(op, start, goal)
{
}


/*
 * add() adds a node to the search graph. add() does not check whether the
 * node is already in the graph. A depth-first search is established by
 * adding the node to the head of xOPEN.
 */
int BiDepthTree::add(IntrList<Node> *x_open, IntrList<Node> *, Node *succ)
{
    x_open->addtohead(succ);
    return(1);
}
