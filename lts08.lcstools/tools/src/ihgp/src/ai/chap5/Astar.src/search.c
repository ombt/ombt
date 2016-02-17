// A* best-first search for state-space graphs

// local headers
#include "search.h"

// print solution by tracing parent pointers
template <class NodeType>
void
PrintSolution(NodeType *pnode)
{
	// need to reverse direction of list before printing
	List<NodeType *> solution;
	for ( ; pnode != NULL; pnode = pnode->getParent())
	{
		solution.insertAtFront(pnode);
	}

	// now print the solution starting from the start node
	ListIterator<NodeType *> solutionIter(solution);
	for (int i = 1; !solutionIter.done(); i++, solutionIter++)
	{
		cout << "STEP " << i << " ..." << endl;
		cout << *solutionIter() << endl;
	}
	return;
}

// best first search routine with multiple goal states but without
// checking for redundant states.
//
template <class NodeType>
int
BestFirst_Astar_WOCheck(const NodeType &start, const List<NodeType> &goals, 
	int &uniquenodes, int &expandednodes)
{
	uniquenodes = expandednodes = 0;

	// copy start state
	NodePtr<NodeType> pstart(start);

	// calculate the heuristic value for this node
	pstart->heuristic(*pstart, goals);

	// create open priority queue
	List<NodePtr<NodeType> > openpq;
	openpq.insertByValue(pstart);
	uniquenodes++;

	// create closed set
	List<NodePtr<NodeType> > closedset;

	// start search loop
	for (NodePtr<NodeType> pnode; ! openpq.isEmpty(); )
	{
		// remove next node from priority open queue
		openpq.removeAtFront(pnode);

		// check if we have a goal node or not
		if (goals.isInList(*pnode))
		{
			// goal node, print solution
			PrintSolution((NodeType *)pnode);
			return(OK);
		}

		// generate the children of the current node
		if (pnode->expand() != OK)
		{
			// unable to expand current node
			return(NOTOK);
		}
		if (((++expandednodes%1000) == 0) && (expandednodes > 0))
		{
			cout << expandednodes << " nodes expanded." << endl;
			cout << "current node is ... " << *pnode << endl;
		}

		// set up links to parent and calculate the heuristic value
		ListIterator<NodePtr<NodeType> > 
			childrenIter(*pnode->getChildren());
		for ( ; !childrenIter.done(); childrenIter++)
		{
			// set up link to parent
			childrenIter()->setParent((NodeType *)pnode);

			// calculate the heuristic value
			childrenIter()->heuristic(*pstart, goals);

			// insert into open queue
			openpq.insertByValue(childrenIter());
			if (((++uniquenodes%1000) == 0) && (uniquenodes > 0))
			{
				cout << uniquenodes << " unique nodes." << endl;
			}
		}

		// store node in closed set
		closedset.insertAtFront(pnode);
	}

	// finished with error
	return(NOTOK);
}

// best first search routine with a termination-detection function 
// but without checking for redundant states. 
//
template <class NodeType>
int
BestFirst_Astar_WOCheck(const NodeType &start, 
	int &uniquenodes, int &expandednodes)
{
	uniquenodes = expandednodes = 0;

	// copy start state
	NodePtr<NodeType> pstart(start);

	// calculate the heuristic value for this node
	pstart->heuristic(*pstart);

	// create open priority queue
	List<NodePtr<NodeType> > openpq;
	openpq.insertByValue(pstart);
	uniquenodes++;

	// create closed set
	List<NodePtr<NodeType> > closedset;

	// start search loop
	for (NodePtr<NodeType> pnode; ! openpq.isEmpty(); )
	{
		// remove next node from priority open queue
		openpq.removeAtFront(pnode);

		// check if we have a goal node or not
		if (pnode->isGoal())
		{
			// goal node, print solution
			PrintSolution((NodeType *)pnode);
			return(OK);
		}

		// generate the children of the current node
		if (pnode->expand() != OK)
		{
			// unable to expand current node
			return(NOTOK);
		}
		if (((++expandednodes%1000) == 0) && (expandednodes > 0))
		{
			cout << expandednodes << " nodes expanded." << endl;
			cout << "current node is ... " << *pnode << endl;
		}

		// set up links to parent and calculate the heuristic value
		ListIterator<NodePtr<NodeType> > 
			childrenIter(*pnode->getChildren());
		for ( ; !childrenIter.done(); childrenIter++)
		{
			// set up link to parent
			childrenIter()->setParent((NodeType *)pnode);

			// calculate the heuristic value
			childrenIter()->heuristic(*pstart);

			// insert into open queue
			openpq.insertByValue(childrenIter());
			if (((++uniquenodes%1000) == 0) && (uniquenodes > 0))
			{
				cout << uniquenodes << " unique nodes." << endl;
			}
		}

		// store node in closed set
		closedset.insertAtFront(pnode);
	}

	// finished with error
	return(NOTOK);
}

// best first search routine with multiple goal states and with
// checking for redundant states. redundant states are removed.
//
template <class NodeType>
int
BestFirst_Astar_BasicCheck(const NodeType &start, const List<NodeType> &goals, 
	int &uniquenodes, int &expandednodes)
{
	uniquenodes = expandednodes = 0;

	// copy start state
	NodePtr<NodeType> pstart(start);

	// calculate the heuristic value for this node
	pstart->heuristic(*pstart, goals);

	// create open priority queue
	List<NodePtr<NodeType> > openpq;
	openpq.insertByValue(pstart);
	uniquenodes++;

	// create closed set
	List<NodePtr<NodeType> > closedset;

	// start search loop
	for (NodePtr<NodeType> pnode; ! openpq.isEmpty(); )
	{
		// remove next node from priority open queue
		openpq.removeAtFront(pnode);

		// check if we have a goal node or not
		if (goals.isInList(*pnode))
		{
			// goal node, print solution
			PrintSolution((NodeType *)pnode);
			return(OK);
		}

		// generate the children of the current node
		if (pnode->expand() != OK)
		{
			// unable to expand current node
			return(NOTOK);
		}
		if (((++expandednodes%1000) == 0) && (expandednodes > 0))
		{
			cout << expandednodes << " nodes expanded." << endl;
			cout << "current node is ... " << *pnode << endl;
		}

		// set up links to parent and calculate the heuristic value
		ListIterator<NodePtr<NodeType> > 
			childrenIter(*pnode->getChildren());
		for ( ; !childrenIter.done(); childrenIter++)
		{
			// set up link to parent
			childrenIter()->setParent((NodeType *)pnode);

			// calculate the heuristic value
			childrenIter()->heuristic(*pstart, goals);

			// check if node was already generated
			if ( ! openpq.isInList(childrenIter()) &&
			     ! closedset.isInList(childrenIter()))
			{
				// insert into open queue
				openpq.insertByValue(childrenIter());
				if (((++uniquenodes%1000) == 0) && (uniquenodes > 0))
				{
					cout << uniquenodes << " unique nodes." << endl;
				}
			}
		}

		// store node in closed set
		closedset.insertAtFront(pnode);
	}

	// finished with error
	return(NOTOK);
}

// best first search routine with a termination-detection function 
// and with checking for redundant states. redundant states are removed.
//
template <class NodeType>
int
BestFirst_Astar_BasicCheck(const NodeType &start, 
	int &uniquenodes, int &expandednodes)
{
	uniquenodes = expandednodes = 0;

	// copy start state
	NodePtr<NodeType> pstart(start);

	// calculate the heuristic value for this node
	pstart->heuristic(*pstart);

	// create open priority queue
	List<NodePtr<NodeType> > openpq;
	openpq.insertByValue(pstart);
	uniquenodes++;

	// create closed set
	List<NodePtr<NodeType> > closedset;

	// start search loop
	for (NodePtr<NodeType> pnode; ! openpq.isEmpty(); )
	{
		// remove next node from priority open queue
		openpq.removeAtFront(pnode);

		// check if we have a goal node or not
		if (pnode->isGoal())
		{
			// goal node, print solution
			PrintSolution((NodeType *)pnode);
			return(OK);
		}

		// generate the children of the current node
		if (pnode->expand() != OK)
		{
			// unable to expand current node
			return(NOTOK);
		}
		if (((++expandednodes%1000) == 0) && (expandednodes > 0))
		{
			cout << expandednodes << " nodes expanded." << endl;
			cout << "current node is ... " << *pnode << endl;
		}

		// set up links to parent and calculate the heuristic value
		ListIterator<NodePtr<NodeType> > 
			childrenIter(*pnode->getChildren());
		for ( ; !childrenIter.done(); childrenIter++)
		{
			// set up link to parent
			childrenIter()->setParent((NodeType *)pnode);

			// calculate the heuristic value
			childrenIter()->heuristic(*pstart);

			// check if node was already generated
			if ( ! openpq.isInList(childrenIter()) &&
			     ! closedset.isInList(childrenIter()))
			{
				// insert into open queue
				openpq.insertByValue(childrenIter());
				if (((++uniquenodes%1000) == 0) && (uniquenodes > 0))
				{
					cout << uniquenodes << " unique nodes." << endl;
				}
			}
		}

		// store node in closed set
		closedset.insertAtFront(pnode);
	}

	// finished with error
	return(NOTOK);
}

// best first search routine with multiple goal states and with
// checking for redundant states.
//
template <class NodeType>
int
BestFirst_Astar_Check(const NodeType &start, const List<NodeType> &goals, 
	int &uniquenodes, int &expandednodes)
{
	uniquenodes = expandednodes = 0;

	// copy start state
	NodePtr<NodeType> pstart(start);

	// calculate the heuristic value for this node
	pstart->heuristic(*pstart, goals);

	// create open priority queue
	List<NodePtr<NodeType> > openpq;
	openpq.insertByValue(pstart);
	uniquenodes++;

	// create closed set
	List<NodePtr<NodeType> > closedset;

	// start search loop
	for (NodePtr<NodeType> pnode; ! openpq.isEmpty(); )
	{
		// remove next node from priority open queue
		openpq.removeAtFront(pnode);

		// check if we have a goal node or not
		if (goals.isInList(*pnode))
		{
			// goal node, print solution
			PrintSolution((NodeType *)pnode);
			return(OK);
		}

		// generate the children of the current node
		if (pnode->expand() != OK)
		{
			// unable to expand current node
			return(NOTOK);
		}
		if (((++expandednodes%1000) == 0) && (expandednodes > 0))
		{
			cout << expandednodes << " nodes expanded." << endl;
			cout << "current node is ... " << *pnode << endl;
		}

		// the following two lists are REQUIRED since the
		// list iterator used below does NOT allow nodes
		// to be deleted or inserted into the list while
		// the iterator is traversing the list. the solution
		// is to save the nodes that must be deleted and
		// inserted in two separate lists, and after the
		// iterator is done, then remove and add nodes to
		// pnodes children list.
		//
		// list of nodes to delete from pnode children
		List<NodePtr<NodeType> > nodesToRemove;
		nodesToRemove.clear();

		// list of nodes to add to pnode children
		List<NodePtr<NodeType> > nodesToInsert;
		nodesToInsert.clear();

		// scan children and determine if they already exist.
		ListIterator<NodePtr<NodeType> > 
			childrenIter(*pnode->getChildren());
		for ( ; !childrenIter.done(); childrenIter++)
		{
			// set up link to parent
			childrenIter()->setParent((NodeType *)pnode);

			// calculate the heuristic value
			childrenIter()->heuristic(*pstart, goals);

			// check if node already exists
			NodePtr<NodeType> pchild(childrenIter());
			NodePtr<NodeType> popen(childrenIter());
			NodePtr<NodeType> pclosed(childrenIter());

			// check if node was already generated
			int inopen = NOMATCH;
			int inclosed = NOMATCH;
			if (( ! openpq.isEmpty()) && 
			    (inopen = openpq.retrieveByValue(popen)) == OK)
			{
				// check which path is better, the new
				// path thru pnode, or the old one
				// thru popen.
				//
				int childGvalue = pchild->getGvalue();
				int oldGvalue = popen->getGvalue();
				if (childGvalue < oldGvalue)
				{
					// reset old node parent pointer
					popen->setParent(pchild->getParent());
					popen->heuristic(*pstart);
				}

				// delete copy of node, and insert
				// old node, popen, into children
				// list of pnode.
				//
				nodesToRemove.insertAtFront(pchild);
				nodesToInsert.insertAtFront(popen);
			}
			else if (( ! closedset.isEmpty()) && 
				 (inclosed = closedset.retrieveByValue(pclosed)) == OK)
			{
				// check which path is better, the new
				// path thru pnode, or the old one
				// thru pclosed.
				//
				int childGvalue = pchild->getGvalue();
				int oldGvalue = pclosed->getGvalue();
				if (childGvalue < oldGvalue)
				{
					// reset parent pointer
					pclosed->setParent(pchild->getParent());
					pclosed->heuristic(*pstart, goals);

					// check parent pointers for
					// children of pclosed.
					//
					ListIterator<NodePtr<NodeType> > closedChildrenIter(*pclosed->getChildren());
					for ( ; ! closedChildrenIter.done(); closedChildrenIter++)
					{
						// first we need to check if this child has 
						// the closed node as a parent. if it does,
						// then do nothing. if it does not, then we
						// have to compare g-values. if the existing
						// g-value is smaller, then leave child node
						// alone. if the the existing g-value is greater,
						// then we have to reset the closed child parent
						// pointer to point to the closed node since this
						// path is now better.
						//
						NodePtr<NodeType> pclosedChild(closedChildrenIter());
						NodePtr<NodeType> pclosedChildParent = 
							pclosedChild->getParent();
						if ((NodeType *)pclosedChildParent == (NodeType *)pclosed)
						{
							// parent of closed child is the closed
							// node, so just skip it.
							continue;
						}

						// compare g-values to determine if the new path
						// is better (cheaper) than the old path to 
						// the closed child node.
						//
						int oldClosedChildGvalue = pclosedChild->getGvalue();
						pclosedChild->setParent(pclosed);
						pclosedChild->heuristic(*pstart, goals);
						int newClosedChildGvalue = pclosedChild->getGvalue();
						if (newClosedChildGvalue > oldClosedChildGvalue)
						{
							// set parent to old value
							pclosedChild->setParent(pclosedChildParent);
							pclosedChild->heuristic(*pstart, goals);
						}
					}
				}

				// delete copy of node, and insert
				// old node, pclosed, into children
				// list of pnode.
				//
				nodesToRemove.insertAtFront(pchild);
				nodesToInsert.insertAtFront(pclosed);
			}
			else if (inopen == NOMATCH && inclosed == NOMATCH)
			{
				// new node, place in open queue
				openpq.insertByValue(pchild);
				if (((++uniquenodes%1000) == 0) && (uniquenodes > 0))
				{
					cout << uniquenodes << " unique nodes." << endl;
				}
			}
			else
			{
				// an error of some type
				return(NOTOK);
			}
		}

		// get pointer to nodes children list
		List<NodePtr<NodeType> > 
			*pchildren = pnode->getChildren();

		// remove children nodes
		ListIterator<NodePtr<NodeType> > 
			nodesToRemoveIter(nodesToRemove);
		for ( ; !nodesToRemoveIter.done(); nodesToRemoveIter++)
		{
			pchildren->removeByValue(nodesToRemoveIter());
		}

		// add children nodes
		ListIterator<NodePtr<NodeType> > 
			nodesToInsertIter(nodesToInsert);
		for ( ; !nodesToInsertIter.done(); nodesToInsertIter++)
		{
			pchildren->insertAtFront(nodesToInsertIter());
		}

		// store node in closed set
		closedset.insertAtFront(pnode);
	}

	// finished with error
	return(NOTOK);
}

// best first search routine with multiple goal states and with
// checking for redundant states. this function calls a predicate
// function to determine if a goal has been reached.
//
template <class NodeType>
int
BestFirst_Astar_Check(const NodeType &start, 
	int &uniquenodes, int &expandednodes)
{
	uniquenodes = expandednodes = 0;

	// copy start state
	NodePtr<NodeType> pstart(start);

	// calculate the heuristic value for this node
	pstart->heuristic(*pstart);

	// create open priority queue
	List<NodePtr<NodeType> > openpq;
	openpq.insertByValue(pstart);
	uniquenodes++;

	// create closed set
	List<NodePtr<NodeType> > closedset;

	// start search loop
	for (NodePtr<NodeType> pnode; ! openpq.isEmpty(); )
	{
		// remove next node from priority open queue
		openpq.removeAtFront(pnode);

		// check if we have a goal node or not
		if (pnode->isGoal())
		{
			// goal node, print solution
			PrintSolution((NodeType *)pnode);
			return(OK);
		}

		// generate the children of the current node
		if (pnode->expand() != OK)
		{
			// unable to expand current node
			return(NOTOK);
		}
		if (((++expandednodes%1000) == 0) && (expandednodes > 0))
		{
			cout << expandednodes << " nodes expanded." << endl;
			cout << "current node is ... " << *pnode << endl;
		}

		// the following two lists are REQUIRED since the
		// list iterator used below does NOT allow nodes
		// to be deleted or inserted into the list while
		// the iterator is traversing the list. the solution
		// is to save the nodes that must be deleted and
		// inserted in two separate lists, and after the
		// iterator is done, then remove and add nodes to
		// pnodes children list.
		//
		// list of nodes to delete from pnode children
		List<NodePtr<NodeType> > nodesToRemove;
		nodesToRemove.clear();

		// list of nodes to add to pnode children
		List<NodePtr<NodeType> > nodesToInsert;
		nodesToInsert.clear();

		// scan children and determine if they already exist.
		ListIterator<NodePtr<NodeType> > 
			childrenIter(*pnode->getChildren());
		for ( ; !childrenIter.done(); childrenIter++)
		{
			// set up link to parent
			childrenIter()->setParent((NodeType *)pnode);

			// calculate the heuristic value
			childrenIter()->heuristic(*pstart);

			// check if node already exists
			NodePtr<NodeType> pchild(childrenIter());
			NodePtr<NodeType> popen(childrenIter());
			NodePtr<NodeType> pclosed(childrenIter());

			// check if node was already generated
			int inopen = NOMATCH;
			int inclosed = NOMATCH;
			if (( ! openpq.isEmpty()) && 
			    (inopen = openpq.retrieveByValue(popen)) == OK)
			{
				// check which path is better, the new
				// path thru pnode, or the old one
				// thru popen.
				//
				int childGvalue = pchild->getGvalue();
				int oldGvalue = popen->getGvalue();
				if (childGvalue < oldGvalue)
				{
					// reset old node parent pointer
					popen->setParent(pchild->getParent());
					popen->heuristic(*pstart);
				}

				// delete copy of node, and insert
				// old node, popen, into children
				// list of pnode.
				//
				nodesToRemove.insertAtFront(pchild);
				nodesToInsert.insertAtFront(popen);
			}
			else if (( ! closedset.isEmpty()) && 
				 (inclosed = closedset.retrieveByValue(pclosed)) == OK)
			{
				// check which path is better, the new
				// path thru pnode, or the old one
				// thru pclosed.
				//
				int childGvalue = pchild->getGvalue();
				int oldGvalue = pclosed->getGvalue();
				if (childGvalue < oldGvalue)
				{
					// reset parent pointer
					pclosed->setParent(pchild->getParent());
					pclosed->heuristic(*pstart);

					// check parent pointers for
					// children of pclosed.
					//
					ListIterator<NodePtr<NodeType> > closedChildrenIter(*pclosed->getChildren());
					for ( ; ! closedChildrenIter.done(); closedChildrenIter++)
					{
						// first we need to check if this child has 
						// the closed node as a parent. if it does,
						// then do nothing. if it does not, then we
						// have to compare g-values. if the existing
						// g-value is smaller, then leave child node
						// alone. if the the existing g-value is greater,
						// then we have to reset the closed child parent
						// pointer to point to the closed node since this
						// path is now better.
						//
						NodePtr<NodeType> pclosedChild(closedChildrenIter());
						NodePtr<NodeType> pclosedChildParent = 
							pclosedChild->getParent();
						if ((NodeType *)pclosedChildParent == (NodeType *)pclosed)
						{
							// parent of closed child is the closed
							// node, so just skip it.
							continue;
						}
TRACE();

						// compare g-values to determine if the new path
						// is better (cheaper) than the old path to 
						// the closed child node.
						//
						int oldClosedChildGvalue = pclosedChild->getGvalue();
						pclosedChild->setParent(pclosed);
						pclosedChild->heuristic(*pstart);
						int newClosedChildGvalue = pclosedChild->getGvalue();
						if (newClosedChildGvalue > oldClosedChildGvalue)
						{
							// set parent to old value
							pclosedChild->setParent(pclosedChildParent);
							pclosedChild->heuristic(*pstart);
						}
					}
				}

				// delete copy of node, and insert
				// old node, pclosed, into children
				// list of pnode.
				//
				nodesToRemove.insertAtFront(pchild);
				nodesToInsert.insertAtFront(pclosed);
			}
			else if (inopen == NOMATCH && inclosed == NOMATCH)
			{
				// new node, place in open queue
				openpq.insertByValue(pchild);
				if (((++uniquenodes%1000) == 0) && (uniquenodes > 0))
				{
					cout << uniquenodes << " unique nodes." << endl;
				}
			}
			else
			{
				// an error of some type
				return(NOTOK);
			}
		}

		// get pointer to nodes children list
		List<NodePtr<NodeType> > 
			*pchildren = pnode->getChildren();

		// remove children nodes
		ListIterator<NodePtr<NodeType> > 
			nodesToRemoveIter(nodesToRemove);
		for ( ; !nodesToRemoveIter.done(); nodesToRemoveIter++)
		{
			pchildren->removeByValue(nodesToRemoveIter());
		}

		// add children nodes
		ListIterator<NodePtr<NodeType> > 
			nodesToInsertIter(nodesToInsert);
		for ( ; !nodesToInsertIter.done(); nodesToInsertIter++)
		{
			pchildren->insertAtFront(nodesToInsertIter());
		}

		// store node in closed set
		closedset.insertAtFront(pnode);
	}

	// finished with error
	return(NOTOK);
}

