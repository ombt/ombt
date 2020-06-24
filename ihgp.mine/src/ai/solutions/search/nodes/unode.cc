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
 * The constructor initializes the node's G-value to 0.
 */
UniNode::UniNode()
{
    g = 0;
}


/*
 * get_g() returns the node's G-value.
 */
int UniNode::get_g() const
{
    return(g);
}


/*
 * set_g() sets the node's G-value.
 */
void UniNode::set_g(int val)
{
    g = val;
}


/*
 * operator< determines the order of two nodes based on their G-values,
 * i.e., if node A has got a lower G-value than node B node A
 * ranks above B: nodes are ordered in order of ascending G-values;
 * this is because a lower G-value means that the node is closer to the
 * goal.
 */
int UniNode::operator<(const Node &other) const
{
    return(g - ((UniNode &)other).g);
}
