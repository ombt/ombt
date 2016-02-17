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
 * Remember that the terminology for AND and OR nodes used here
 * differs from standard terminology. Here, an AND node represents a
 * collection of other nodes, each of which must be solved in order for
 * the AND node to be solved. Any other node is an OR node. In standard
 * terminology an AND node is not a collection of nodes but is rather
 * itself part of a collection of nodes each of which must be solved in
 * order to make the parent of these nodes solved and any other node is
 * an OR node.
 */

/*
 * The constructor initializes the node, without creating room
 * for any possible successors because at this point this
 * information is not known.
 */
AndNode::AndNode()
    :succlist(0, DontManage)
{
}


/*
 * The alternative constructor creates room for n-successors and
 * initializes n_left (the number of successor nodes left to be solved)
 * accordingly.
 */
AndNode::AndNode(int num)
    :AONode(num), succlist(0, DontManage)
{
   succlist.grow(num);
}


TypeOfNode AndNode::gettype() const
{
    return(type);
}


NodeStat AndNode::getstatus() const
{
    return(status);
}


void AndNode::setstatus(NodeStat stat)
{
    status = stat;
}


/*
 * setsolved() checks if the node can be labelled solved and if so it
 * labels the node solved. This routine is called by the solved labelling
 * procedure when one of the successor nodes of this node has been solved.
 * Accordingly, the number of successor nodes left to be solved is decreased
 * by one and if there are no more successors left to be solved the node
 * itself is labelled solved.
 * setsolved() returns 1 if the node can be labelled solved and 0
 * otherwise.
 */
int AndNode::setsolved(AONode *)
{
    n_left--;
    if (n_left == 0)
    {
	status = Solved;
	return(1);
    }
    return(0);
}


/*
 * setunsolvable() checks if the node can be labelled unsolvable and if
 * so it labels the node unsolvable. This routine is called by the unsolvable
 * labelling procedure when one of the successor nodes of the node has been
 * labelled unsolvable. An AND node automatically becomes unsolvable
 * if any of its successor is unsolvable. Therefore, this routine always
 * labels the node unsolvable and returns 1.
 */
int AndNode::setunsolvable()
{
    status = Unsolvable;
    return(1);
}


/*
 * addsucc adds a node to the successor list of this node and increases
 * the number of nodes left to be solved by one.
 */
void AndNode::addsucc(AONode *node)
{
    succlist += node;
    n_left++;
}


/*
 * setsucc() adds a node to the successor list of this node and increases
 * the number of nodes left to be solved by one. The difference between
 * this routine and addsucc() is that room for the node must already
 * be present before calling this routine.
 */
void AndNode::setsucc(int index, AONode *node)
{
    succlist[index] = node;
}


int AndNode::get_nsucc() const
{
    return(succlist.getsize());
}


AONode *AndNode::getsucc(int index) const
{
    return((AONode *)succlist[index]);
}


/*
 * display() should never be called since AndNode only has
 * 'administrative' value. Still, it must be implemented because
 * display() is defined pure virtual in class Node, from which AndNode
 * is ultimately derived.
 */
void AndNode::display() const
{
    puts("display() function of class AndNode not implemented - should never be called");
    exit(0);
}


/*
 * For the same reason as display() the equality operator must also
 * be implemented.
 */
int AndNode::operator==(const Node &) const
{
    puts("operator==() function of class AndNode not implemented - should never be called");
    exit(0);
    return(0);
}
