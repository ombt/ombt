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
 * The constructor initializes the F-value and G-value of the node
 * to 0 (the G-value is initialized by the constructor of class UniNode
 * from which BestNode is derived).
 */
BestNode::BestNode()
{
    f = 0;
}


/*
 * get_f() returns the node's F-value.
 */
int BestNode::get_f() const
{
    return(f);
}


/*
 * set_f() sets the node's F-value.
 */
void BestNode::set_f(int val)
{
    f = val;
}


/*
 * operator< determines the order of two nodes based on their F-values,
 * i.e., if node A has got a lower F-value than node B node A
 * ranks above B: nodes are ordered in order of ascending F-values;
 * this is because a lower F-value means that the node is closer to the
 * goal.
 */
int BestNode::operator<(const Node &other) const
{
    return(f < ((BestNode &)other).f);
}
