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
 * The constructor initializes the number of the operator that was
 * applied last to zero.
 */
BackNode::BackNode()
{
     last_op = 0;
}


/*
 * do_operator() is a dummy function that is implemented here because
 * either one of do_operator() and expand_one() is implemented by the
 * class derived from BackNode.
 */
BackNode *BackNode::do_operator(int)
{
    puts("Wrong do_operator function called - did you forget to implement it (or expand_one)!?");
    exit(0);
    return(0);
}


/*
 * expand_one() generates the next successor of the node by applying
 * the next operator to the current node. Node that since expand_one()
 * is a virtual function the class that is derived from BackNode may
 * override this implementation.
 */
BackNode *BackNode::expand_one(int numop)
{
    BackNode *succ = 0;

    /*
     * If the operator is not applicable apply the next operator
     * until a successor is generated or all operators have been applied.
     */
    while (last_op < numop && !succ)
        succ = do_operator(last_op++);

    return(succ);
}


/*
 * Because BackNode objects are stored in an object of class IntrList
 * clone() must be implemented. But because the elements in this list
 * are actually never copied clone() is made a dummy routine.
 */
BackNode *BackNode::clone() const
{
    puts("BackNode::clone() not implemented - should never be called");
    exit(0);
    return(0);
}
