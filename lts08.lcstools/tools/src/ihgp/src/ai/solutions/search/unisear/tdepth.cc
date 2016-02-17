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
 * The constructor passes the start node, goal node and the number of
 * operators to the constructor of class Search.
 */
DepthTree::DepthTree(int op, Node *start, Node *goal)
    :Search(op, start, goal)
{
    maxdepth = 0;
}


/*
 * setdepth() is called by the application to limit the depth of
 * of the search.
 */
void DepthTree::setdepth(unsigned depth)
{
    maxdepth = depth;
}


unsigned DepthTree::getdepth() const
{
    return(maxdepth);
}


/*
 * add() adds a node to the search graph. add() does not check whether the
 * node is already in the graph. It does check however if the depth bound
 * has been reached (if a depth bound for the search has been set). If so,
 * the node is not added to the graph. A depth-first search is established
 * by adding the node to the head of OPEN.
 */
int DepthTree::add(Node *succ)
{
    if (maxdepth)
    {
        unsigned depth;
        DepthNode *par,
	          *dsucc = (DepthNode *)succ;

	/*
	 * Compute current depth of the tree and compare this to
	 * the depth bound.
	 */
        par = (DepthNode *)dsucc->getparent();
	depth = par->getdepth() + 1;

        if (depth >= maxdepth)
	    return(0);

	dsucc->setdepth(depth);
    }

    open.addtohead(succ);
    return(1);
}
