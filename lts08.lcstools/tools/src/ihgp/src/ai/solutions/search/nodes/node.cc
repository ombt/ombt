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
 * The constructor initializes the node's parent pointer to 0.
 */
Node::Node()
{
    parent = 0;
}


/*
 * operator< is a dummy function that is implemented here because
 * it has been defined virtual and must only be implemented by the
 * class derived from Node when a uniform-cost or best-first search
 * is performed.
 */
int Node::operator<(const Node &other) const
{
    puts("Node::operator< not implemented - should never be called");
    exit(0);
    return(0);
}


/*
 * expand() expands the node by generating all of its successors by
 * calling do_operator() n times, where n is the number of operators.
 * The successor nodes are stored in an object of IntrList.
 * Note that since expand() is defined virtual the class that is derived
 * from Node may override this implementation.
 */
IntrList<Node> *Node::expand(int max_op)
{
    IntrList<Node> *ret = new IntrList<Node>(0);
    Node *succ;
    int op;

    for (op = 0; op < max_op; op++)
    {
	succ = do_operator(op);
	if (succ)
            ret->addtotail(succ);
    }

    if (ret->is_empty())
    {
	delete(ret);
	ret = NULL;
    }

    return(ret);
}


void Node::setparent(Node *par)
{
    parent = par;
}


Node *Node::getparent() const
{
        return(parent);
}


/*
 * do_operator() is a dummy function that is implemented here because
 * either one of do_operator() and expand_one() is implemented by the
 * class derived from Node.
 */
Node *Node::do_operator(int )
{
    puts("Wrong do_operator() function called - did you forget to implement it (or expand)!?");
    exit(0);
    return(0);
}


/*
 * Because Node objects are stored in an object of class IntrList
 * clone() must be implemented. But because the elements in this list
 * are actually never copied clone() is made a dummy routine.
 */
Node *Node::clone() const
{
    puts("Node::clone() not implemented - should not be called");
    exit(0);
    return(0);
}
