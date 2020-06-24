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

TypeOfNode OrNode::type = OR,
	   AndNode::type = AND;

/*
 * The constructor sets the number of successor nodes left to be
 * solved or that may be solved (this depends on the type of the node)
 * to zero and declares the status of the node to be undefined.
 */
AONode::AONode()
{
    n_left = 0;
    status = Undef;
}


/*
 * The alternative constructor sets the number of successor nodes left
 * to be solved or that may be solved (this depends on the type of the
 * node) to the specified value. It also declares the status of the node
 * to be undefined.
 */
AONode::AONode(int num_succ)
{
    n_left = num_succ;
    status = Undef;
}


/*
 * inc_nleft() increases the number of successor nodes that must
 * be solved or that may be solved (this depends on the type of the
 * node) by one.
 */
void AONode::incn_left()
{
    n_left++;
}


/*
 * close() is implemented because class AndNode which is derived from
 * this class includes an PtrArray<AONode>. But because the elements
 * in this array are actually never copied clone() is a made a dummy
 * routine.
 */ 
AONode *AONode::clone() const
{
    puts("AONode::clone() not implemented - should never be called");
    exit(0);
    return(0);
}
