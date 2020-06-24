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
 * The constructor passes the start node and number of operators
 * to the constructor of class AOSearch.
 */
AODepthTree::AODepthTree(int op, AONode *start)
    :AOSearch(op, start)
{
}


/*
 * add() adds a node to the search graph. add() does not check whether
 * the node is already in the graph. Special provisions must be
 * taken when an AND node is encountered: an AND node itself must
 * be stored on OPEN, but each of successor must.
 * A depth-first search is established by adding the node to the
 * head of OPEN.
 */
int AODepthTree::add(AONode *succ)
{
    AONode
	*node;
    int
	i;

    if (succ->gettype() == AND)
    {
        /*
         * For every successor of the AND node: set it to point
         * back to the AND node, which is its parent node, and
         * add it to OPEN.
         */
        i = succ->get_nsucc() - 1;
	for ( ;i >= 0; i--)
	{
	    node = succ->getsucc(i);
	    node->setparent(succ);
	    open.addtohead(node);
	}
    }
    else
	open.addtohead(succ);

    return(1);
}
