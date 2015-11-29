// A* best-first search for state-space graphs

// local headers
#include "search_AVL.h"

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
	PriorityQueue_AVLTree<NodePtr<NodeType> > openpq;
	openpq.enqueue(pstart);
	uniquenodes++;

	// create closed set
	List<NodePtr<NodeType> > closedset;

	// start search loop
	for (NodePtr<NodeType> pnode; ! openpq.isEmpty(); )
	{
		// remove next node from priority open queue
		openpq.dequeue(pnode);

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
			if ( ! openpq.includes(childrenIter()) &&
			     ! closedset.isInList(childrenIter()))
			{
				// insert into open queue
				openpq.enqueue(childrenIter());
				if (((++uniquenodes%1000) == 0) && (uniquenodes > 0))
				{
					cout << uniquenodes << " unique nodes." << endl;
					cout << "current node is ... " << *pnode << endl;
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
	PriorityQueue_AVLTree<NodePtr<NodeType> > openpq;
	openpq.enqueue(pstart);
	uniquenodes++;

	// create closed set
	List<NodePtr<NodeType> > closedset;

	// start search loop
	for (NodePtr<NodeType> pnode; ! openpq.isEmpty(); )
	{
		// remove next node from priority open queue
		openpq.dequeue(pnode);

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
			if ( ! openpq.includes(childrenIter()) &&
			     ! closedset.isInList(childrenIter()))
			{
				// insert into open queue
				openpq.enqueue(childrenIter());
				if (((++uniquenodes%1000) == 0) && (uniquenodes > 0))
				{
					cout << uniquenodes << " unique nodes." << endl;
					cout << "current node is ... " << *pnode << endl;
				}
			}
		}

		// store node in closed set
		closedset.insertAtFront(pnode);
	}

	// finished with error
	return(NOTOK);
}

