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

#include "aosearch.h"

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
 * The constructor puts the start node on OPEN and sets the number
 * of operators to the specified value.
 */
AOSearch::AOSearch(int num, AONode *start)
{
    if (start)
        open.addtohead(start);
    startnode = start;            // used to print solution afterwards
    num_op = num;
}


/*
 * The destructor does nothing. The destruction of OPEN and CLOSED is
 * taken care of automatically by the destructors of the list objects.
 */
AOSearch::~AOSearch()
{
}


/*
 * generate() starts the search.
 * generate() returns 1 if a solution is found and 0 otherwise.
 */
int AOSearch::generate()
{
    if (!solve())
    {
	startnode = 0;
	return(0);
    }
    return(1);
}


/*
 * display() prints the solution path. Note that all nodes that are
 * on the solution path are printed and not just terminal nodes.
 */
void AOSearch::display()
{
    if (startnode)
        print_sol(startnode);
}


/*
 * set_startnode() adds the node that will be used to start the search on
 * OPEN. This function can be called if passing the start node through the
 * constructor of class AOSearch is for some reason not feasible or if
 * the search space has been cleared by clear() and a new search must
 * be started.
 */
void AOSearch::set_startnode(AONode *node)
{
    open.addtohead(node);
    startnode = node;
}


/*
 * clear() clears the state space so that the object can be reused for a
 * new problem.
 */
void AOSearch::clear()
{
    closed.clear(DoDel);
    open.clear(DoDel);
    startnode = 0;
}


/*
 * print_sol() traverses the solution path from the start and prints every
 * node it finds on its way. When a node of type AND is encountered print_sol()
 * calls itself recursively on each of the successor nodes of the AND-node.
 */
void AOSearch::print_sol(AONode *node)
{
    AONode
        *cur = node;

    while (1)
    {
        if (cur->gettype() == AND)
        {
	    int i, num;

            num = cur->get_nsucc();
            for (i = 0; i < num; i++)
                print_sol(cur->getsucc(i));    // recurse
            break;
        }
        else
        {
	    /*
	     * Assumption: all nodes must be printed and not just
	     * terminal nodes.
	     */
            cur->display();

	    /*
	     * Get the next successor and stop if the end of the 
	     * branch has been reached.
	     */
            if (!(cur = cur->getsucc()))
                break;
        }
    }
}


/*
 * get_sol() returns the solution path: it stores the nodes that belong
 * to the solution path in an object of type IntrList<Node> which
 * is allocated dynamically. This routine removes these nodes from the
 * the search tree and adds them to the solution list. This means that
 * after get_sol() has been called the state space is in an
 * inconsistent state and the search object can no longer be used (to
 * do any further searches). This is not a problem because the search
 * algorithm wasn't designed this way way anyway: generate() can be
 * called only once to generate one solution (a search algorithm
 * that generates multiple solutions can be found in class BackTrack
 * which implements a backtracking algorithm). For this reason get_sol()
 * calls clear() after it's done.
 */
IntrList<Node> *AOSearch::get_sol()
{
    IntrList<Node> *ret;

    if (!startnode)
	return(0);

    ret = new IntrList<Node>;
    addnodetosol(ret, startnode);
    clear();
    return(ret);
}


/*
 * addnodetosol() adds a node and all of it's successor nodes to
 * the solution path. When an AND-node is encountered it calls itself
 * recursively on each of the successor nodes of the AND-node.
 */
void AOSearch::addnodetosol(IntrList<Node> *list, AONode *node)
{
    AONode
        *cur = node;

    while (1)
    {
        if (cur->gettype() == AND)
        {
	    int i, num;

            num = cur->get_nsucc();
            for (i = 0; i < num; i++)
                addnodetosol(list, cur->getsucc(i));   // recurse
            break;
        }
        else
        {
	    closed.remove_node(cur, NoDel);
	    list->addtotail(cur);
            if (!(cur = cur->getsucc()))               // end of branch
                break;
        }
    }
}


/*
 * solvable() applies the solved labeling procedure to the search tree.
 * First, the (terminal) node that it gets passed is labelled solved. Next,
 * the search tree is traversed upwards through all of the node's ancestors
 * to see if they can be labelled solved. There are two cases:
 * - An AND node can be labelled solved if and only if all of its successors
 *   are labelled solved, i.e., n_left is 0.
 * - An OR node can be labelled solved if any of its successors is
 *   labelled solved.
 * Both of these cases are handled transparently by calling the virtual
 * function setsolved() on each node. The extra node argument of setsolved()
 * is used to make OR-nodes point to their successor so that the solution
 * can be traced afterwards (this is not necessary for AND-nodes because
 * they already point to all of their successors).
 *
 * Solvable() returns 1 if the start node is labelled solved, meaning that
 * the search process ends with success, and 0 otherwise.
 */
int AOSearch::solvable(AONode *node)
{
    AONode
        *parent;

    node->setstatus(Solved);

    while ((parent = (AONode *)node->getparent()))
    {
	/*
         * Label the node solved, if possible. If the node is not solved
         * yet the solved labelling procedure stops because none of the 
         * node's ancestors can be labelled solved as far as their
         * dependency on this node is concerned.
	 */
	if (!parent->setsolved(node))
	    break;
	node = parent;
    }
    return(!parent);
}


/*
 * unsolvable() applies the unsolvable labeling procedure to the search
 * tree. First, the (terminal) node that it gets passed is labelled
 * unsolvable. Next, the search tree is traversed upwards through all of
 * the node's ancestors to see if they can be labelled unsolvable. There
 * are two cases:
 * - An AND node can be labelled unsolvable if any of its successors is
 *   labelled unsolvable.
 * - An OR node can be labelled unsolvable if and only if all of its
 *   successors are labelled unsolvable.
 * Both of these cases are handled transparently by calling the virtual
 * function setunsolvable() on each node.
 *
 * Unsolvable() returns 1 if the start node is labelled unsolvable, meaning
 * that the search process ends with failure, and 0 otherwise.
 */
int AOSearch::unsolvable(AONode *node)
{
    node->setstatus(Unsolvable);

    while ((node = (AONode *)node->getparent()))
    {
	/*
         * Label the node unsolvable, if possible. If the node it not
         * unsolvable, i.e. the node is solved, the unsolvable labelling
	 * procedure stops because none of the node's ancestors can be
	 * labelled unsolvable as far as their dependency on this node
         * is concerned.
	 */
	if (!node->setunsolvable())
	    break;
    }
    return(!node);
}


/*
 * deletable() decides if a node can be pruned. A node can be
 * pruned if itself or any of its ancestors has the specified
 * status: solved or unsolvable, depending on what kind of nodes must
 * be pruned. To check for this condition the tree is traversed upwards
 * trough the parent pointers.
 * deletable() returns 1 if the node can be pruned and 0 otherwise.
 */
int AOSearch::deletable(AONode *node, NodeStat stat)
{
    AONode
        *parent;

    for (parent = node; parent; parent = (AONode *)parent->getparent())
        if (parent->getstatus() == stat)
            return(1);
    return(0);
}


/*
 * prune() searches nodes on OPEN which have the specified status,
 * solved or unsolvable, or which have an ancestor with the specified
 * status. If such a node is found it is removed from OPEN.
 */
void AOSearch::prune(NodeStat stat)
{
    AONode
        *node;
    IntrListIterator<AONode>
        iter(open);

    node = iter.getfirst();
    while (node)
    {
	/*
	 * Check if the node can be pruned and if so, remove it from
         * the list.
	 */
        if (deletable(node, stat))
        {
            iter.remove_current(DoDel);
            node = iter.getcurrent();
        }
        else
            node = iter.getnext();
    }
}


/*
 * solve() implements the actual search engine. The steps taken by this
 * routine are:
 * 
 * 1. Get the first node from OPEN. If OPEN is empty the search ends 
 *    with failure. Otherwise move the node to CLOSED.
 * 2. Check if the node is a terminal node. If true, apply the solved
 *    labelling procedure to the search tree; if the root node was labelled
 *    solved by this procedure the search ends with success. If the node
 *    was not a terminal node prune all nodes from OPEN that are labelled
 *    solved or that have an ancestor that is labelled solved. Go to step 1.
 * 3. Expand the node by generating its successor nodes. If there are 
 *    no successors the node cannot be solved and the unsolvable labelling
 *    procedure is applied to the search tree; if the root node was labelled
 *    solved by this procedure the search ends with failure. If the node
 *    does have have successors all nodes that are labelled unsolvable or
 *    that have an unsolvable ancestor are pruned from OPEN. Go to step 1.
 *    For every successor do the following:
 *    4. Set successor to point to node (which is its parent node) so
 *       that the search tree can be traversed upwards by the solved
 *       and unsolvable labelling procedures.  
 *    5. Increase the value of the number of nodes left to be solved
 *       (or that can still be solved, depending on the type of the parent
 *       node) of the parent node of the successor.
 *    6. Pass the node to add() for further processing. If add() returns
 *       0 the node is already part of the search graph and must be
 *       done away with. Add() is a virtual function implemented by
 *       a derivative of AOSearch. The implementation of add() defines
 *       the type of search: depth-first, breadth-first, etc.
 *
 * Solve() returns 1 if the root node is solved and 0 if the root node is
 * unsolvable or if there are no more nodes left on OPEN.
 */
int AOSearch::solve()
{
    IntrList<Node>
        *succlist;
     AONode
        *father,
        *child;

    while((father = open.gethead()) != 0)
    {                                // get first node from open
        open.remove_head(NoDel);
        closed.addtohead(father);       // move it to closed

        if (is_terminal(father))        // is this a terminal node?
        {
            if (solvable(father))        // apply solved-labelling procedure
                return(1);               // start node is solved: success
            prune(Solved);               // prune solved nodes from open
            continue;
        }

        succlist = father->expand(num_op);	// expand the node

        if (!succlist || succlist->is_empty()) 
        {			    // node has no sucessors
            if (unsolvable(father))    // apply unsolvable-labelling procedure
	    {
		delete(succlist);
                return(0);             // start node unsolvable: fail
	    }
            prune(Unsolvable);         // prune unsolvable nodes from open
            continue;
        }

        while ((child = (AONode *)succlist->gethead()) != 0)
        {
            succlist->remove_head(NoDel);

            child->setparent(father);   // set successor's parent pointer
            father->incn_left();        // increase number of successors left

	    /*
	     * Because in this implementation an AND node is just a container
	     * of other nodes it has no real value in itself and can be
	     * moved to CLOSED. But since each of its successors must be
             * processed every AND node is passed to add() too.
	     */
            if (child->gettype() == AND)
                closed.addtohead(child);

	    if (!add(child))		// add successor to graph
		delete(child);		// successor already in graph
#ifdef DEBUG
            else
	    {
                puts("adding node:");
                child->display();
                putchar('\n');
	    }
#endif
        }
	delete(succlist);
    }
    return(0);
}
