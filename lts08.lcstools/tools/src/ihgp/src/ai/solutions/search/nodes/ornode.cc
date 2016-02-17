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

#include "ainodes.h"

/*
 * The constructor initializes the node's successor pointer to 0.
 */
OrNode::OrNode()
{
    succ = 0;
}


TypeOfNode OrNode::gettype() const
{
    return(type);
}


int OrNode::get_nsucc() const
{
    return(1);
}


AONode *OrNode::getsucc(int) const
{
    return(succ);
}


void OrNode::setstatus(NodeStat s)
{
    status = s;
}


NodeStat OrNode::getstatus() const
{
    return(status);
}


/*
 * setsolved() checks if the node can be labelled solved and if so it
 * labels the node solved. This routine is called by the solved labelling
 * procedure when one of the successor nodes of this node has been solved.
 * An OR node is automatically solved when one of its successor nodes is
 * solved. Therefore, this routine always labels the node solved and returns
 * 1. setsolved() also initializes its successor pointer to the successor
 * node that was labelled solved, so that the solution path can be
 * traced afterwards.
 */
int OrNode::setsolved(AONode *succnode)
{
    status = Solved;
    succ = succnode;
    return(1);
}


/*
 * setunsolvable() checks if the node can be labelled unsolvable and if it
 * so labels the node unsolvable. This routine is called by the unsolvable
 * labelling procedure when one of the successor nodes of the node has been
 * labelled unsolvable. An OR node becomes unsolvable only if all of its
 * successor nodes are unsolvable. Accordingly, this routine decreases the
 * number of successor nodes that may be solved by one and if there are
 * no more successors left the node itself is labelled unsolvable.
 * setunsolvable() returns 1 if the node is labelled unsolvable and 0
 * otherwise.
 */
int OrNode::setunsolvable()
{
    n_left--;
    if (!n_left)
    {
	status = Unsolvable;
	return(1);
    }
    return(0);
}
