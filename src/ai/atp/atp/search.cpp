// A* best-first search for state-space graphs

// local headers
#include "globals.h"
#include "search.h"
#include "trace.h"

// print solution by tracing parent pointers
template <class NodeType>
void
PrintRenameVariables(Proxy<NodeType> &pnode)
{
	// check if we have a query
	if (!pnode->getQuery())
		return;

	// we have a query, dump renamed variable data
	cout << endl;
	cout << "RENAMED VARIABLES ARE: " << endl;
	cout << variableNames << endl;
	return;
}

template <class NodeType>
void
PrintSolution(Proxy<NodeType> &pnode)
{
	// is reporting turned on
	if (reporttype != ReportParent && 
	    reporttype != ReportBoth) 
		return;

	// need to reverse direction of list before printing
	List<Proxy<NodeType> > solution;
	for ( ; (NodeType *)pnode != NULL; pnode = pnode->getParent())
	{
		solution.insertAtFront(pnode);
	}

	// now print the solution starting from the start node
	cout << "SOLUTION OUTPUT (1):" << endl;
	ListIterator<Proxy<NodeType> > solutionIter(solution);
	for (int i = 1; !solutionIter.done(); i++, solutionIter++)
	{
		cout << "STEP " << i << " ..." << endl;
		cout << *solutionIter() << endl;
	}
	return;
}

template <class NodeType>
void
PrintSolution(List<Proxy<NodeType> > &solution)
{
	// is reporting turned on
	if (reporttype != ReportStack && reporttype != ReportBoth) 
		return;

	// now print the solution starting from the start node
	cout << "SOLUTION OUTPUT (2):" << endl;
	ListIterator_Reverse<Proxy<NodeType> > solutionIter(solution);
	for (int i = 1; !solutionIter.done(); i++, solutionIter++)
	{
		cout << "STEP " << i << " ..." << endl;
		cout << *solutionIter() << endl;
	}
	return;
}

template <class NodeType>
void
PrintBriefSolution(List<Proxy<NodeType> > &solution)
{
	// now print the solution starting from the start node
	cout << endl << "BRIEF SOLUTION OUTPUT (2):" << endl;
	ListIterator_Reverse<Proxy<NodeType> > solutionIter(solution);
	for (int i = 1; !solutionIter.done(); i++, solutionIter++)
	{
		solutionIter()->dumpMinData(cout);
	}
	cout << endl;
	return;
}

// dump a queue out
template <class NodeType>
void
dumpqueue(const List<Proxy<NodeType> > &q)
{
	ListIterator<Proxy<NodeType> > qit(q);
	for (int i=1; !qit.done(); i++, qit++)
	{
		cout << "fvalue[" << i << "] = " << qit()->getFvalue() << endl;
		cout << "gvalue[" << i << "] = " << qit()->getGvalue() << endl;
		cout << "hvalue[" << i << "] = " << qit()->getHvalue() << endl;
	}
	return;
}

// best first search routine with a termination-detection function 
//
template <class NodeType, class DataType>
int
BestFirstSearch(List<NodeType> &startstatelist, 
		BinaryTree_AVL<DataType> &startlist, 
		BinaryTree_AVL<DataType> &otherlist)
{
	int status;

	// track all states created
	BinaryTree_AVL<String> allstates;

	// create open priority queue
	List<Proxy<NodeType> > openpq;

	// copy list of start states
	int nodedepth = 1;
	ListIterator<NodeType> startstateIter(startstatelist);
	for ( ; !startstateIter.done(); startstateIter++)
	{
		// copy start state
		Proxy<NodeType> pstart = &startstateIter();

		// set start node depth
		pstart->setDepth(nodedepth);

		// calculate the heuristic value for this node
		if ((status = pstart->heuristic(startlist, otherlist)) != OK)
		{
			ERRORD("heuristic() failed.", status, errno);
			return(status);
		}

		// insert start state into open queue
		if ((status = openpq.insertOrdered(pstart)) != OK)
		{
			ERRORD("insertOrdered() failed.", status, errno);
			return(status);
		}
	}

	// create closed set
	List<Proxy<NodeType> > closedset;

	// start search loop
	for (Proxy<NodeType> pnode; ! openpq.isEmpty(); )
	{
		// dump queue if verbose mode
		if (verbose)
			dumpqueue(openpq);

		// remove next node from priority open queue
		if ((status = openpq.removeAtFront(pnode)) != OK)
		{
			ERRORD("removeAtFront() failed.", status, errno);
			return(status);
		}

		// get current node depth
		nodedepth = pnode->getDepth();

		// check if we have a goal node or not
		status = pnode->isGoal(startlist, otherlist);
		switch (status)
		{
		case OK:
			break;

		case NOMATCH:
		case MAXDEPTHEXCEEDED:
			// no clauses were generated. skip further
			// processing of this node.
			continue;

		case VALID:
			// goal node, print solution
			PrintSolution(pnode);
			PrintRenameVariables(pnode);

			// check if more than one solutions is required
			solutionsfound += 1;
			statistics[SolutionsFound] += 1;
			totalstatistics[TotalSolutionsFound] += 1;

			if (solutionsfound >= solutionsrequired)
				return(VALID);

			// store node in closed set
			if ((status = closedset.insertAtFront(pnode)) != OK)
			{
				ERRORD("insertAtFront() failed.", 
				status, errno);
				return(status);
			}
			continue;

		case MAXCLAUSEEXCEEDED:
			// check if any solutions were found
			if (solutionsfound > 0)
				return(VALID);
			else
				return(NOTPROVEN);

		default:
			// some type of error
			ERRORD("isGoal() failed.", status, errno);
			return(status);
		}

		// generate the children of the current node
		if ((status = pnode->expand(startlist, otherlist)) != OK)
		{
			ERRORD("expand() failed.", status, errno);
			return(status);
		}

		// set up links to parent and calculate the heuristic value
		ListIterator<Proxy<NodeType> > 
			childrenIter(*pnode->getChildren());
		for ( ; !childrenIter.done(); childrenIter++)
		{
			// pointer to child
			Proxy<NodeType> pchild(childrenIter());

			// set up link to parent
			if (reporttype == ReportParent ||
			    reporttype == ReportBoth)
				pchild->setParent(pnode);

			// calculate the heuristic value
			if ((status = pchild->heuristic(
				startlist, otherlist)) != OK)
			{
				ERRORD("heuristic() failed.", status, errno);
				return(status);
			}

			// insert into open queue
			if (!bfswithchecks)
			{
				pchild->setDepth(nodedepth+1);
				if ((status = openpq.insertOrdered(
					pchild)) != OK)
				{
					ERRORD("insertOrdered() failed.", 
						status, errno);
					return(status);
				}
			}
			else
			{
				statistics[RedundantClauseTestsAttempted] += 1;
				totalstatistics[TotalRedundantClauseTestsAttempted] += 1;
				String newnode(pchild->getNormalizedClauses());
				if (allstates.retrieve(newnode) == NOMATCH)
				{
					pchild->setDepth(nodedepth+1);
					if ((status = openpq.insertOrdered(
							pchild)) != OK)
					{
						ERRORD("insertOrdered() failed.", 
							status, errno);
						return(status);
					}
				}
				else
				{
					statistics[RedundantClausesRejected] += 1;
					totalstatistics[TotalRedundantClausesRejected] += 1;
				}
			}
		}

		// store node in closed set. if the clause has no children,
		// then no reason to save it. just release the memory
		//
		if (!(pnode->getChildren()->isEmpty()))
		{
			if ((status = closedset.insertAtFront(pnode)) != OK)
			{
				ERRORD("insertAtFront() failed.", status, errno);
				return(status);
			}
		}
		if (bfswithchecks)
		{
			if (allstates.insert(pnode->getNormalizedClauses()) != OK)
			{
				ERRORD("insert() failed.", status, errno);
				return(status);
			}
		}
	}

	// check if any solutions were found
	if (solutionsfound > 0)
		return(VALID);
	else
		return(NOTPROVEN);
}

// update current path to active node
template <class NodeType>
int
updatepath(int &nodedepth, int &olddepth, Proxy<NodeType> &pnode,
		List<Proxy<NodeType> > &currentpath)
{
	int status;

	// get new node depth
	nodedepth = pnode->getDepth();

	// update path if reporting is on
	if (reporttype != ReportStack && reporttype != ReportBoth)
		return(OK);

	// update path in stack
	if (currentpath.isEmpty())
	{
		if ((status = currentpath.insertAtFront(pnode)) != OK)
		{
			ERRORD("insertAtFront() failed.", 
				status, errno);
			return(status);
		}
		olddepth = nodedepth;
	}
	else if (nodedepth <= olddepth)
	{
		int nodestopop = olddepth - nodedepth + 1;
		for (int inode = 1; inode <= nodestopop; inode++)
		{
			Proxy<NodeType> ppathnode;
			if ((status = currentpath.removeAtFront(
				ppathnode)) != OK)
			{
				ERRORD("removeAtFront() failed.", 
					status, errno);
				return(status);
			}
		}
		if ((status = currentpath.insertAtFront(pnode)) != OK)
		{
			ERRORD("insertAtFront() failed.", 
				status, errno);
			return(status);
		}
		olddepth = nodedepth;
	}
	else if (nodedepth > olddepth)
	{
		if ((status = currentpath.insertAtFront(pnode)) != OK)
		{
			ERRORD("insertAtFront() failed.", 
				status, errno);
			return(status);
		}
		olddepth = nodedepth;
	}
	else
	{
		MustBeTrue(0);
	}

	// all done
	return(OK);
}

// depth-first search routine with a termination-detection function 
//
template <class NodeType, class DataType>
int
DepthFirstSearch(List<NodeType> &startstatelist, 
		BinaryTree_AVL<DataType> &startlist, 
		BinaryTree_AVL<DataType> &otherlist)
{
	int status;

	// dump data info
	if (reportmemoryusage)
	{
		SETFINALEDATA();
		DUMPDATA(cout);
	}

	// track all states created
	BinaryTree_AVL<String> allstates;

	// create open priority queue
	List<Proxy<NodeType> > openpq;

	// keep track of current path here instead of 
	// using parent pointers. parent pointers cause
	// cycles and reference-counted pointers cannot
	// deal with cyclic data structures. they cause
	// major memory leaks.
	//
	int nodedepth = 1;
	List<Proxy<NodeType> > currentpath;

	// copy list of start states
	ListIterator<NodeType> startstateIter(startstatelist);
	for ( ; !startstateIter.done(); startstateIter++)
	{
		// copy start state
		Proxy<NodeType> pstart(startstateIter());

		// set start node depth
		pstart->setDepth(nodedepth);

		// insert start state into open queue
		if ((status = openpq.insertAtFront(pstart)) != OK)
		{
			ERRORD("insertAtFront() failed.", status, errno);
			return(status);
		}
	}

	// start search loop
	int olddepth = nodedepth;
	Proxy<NodeType> pchild;
	for (Proxy<NodeType> pnode; ! openpq.isEmpty(); )
	{
		// remove next node from priority open queue
		if ((status = openpq.removeAtFront(pnode)) != OK)
		{
			ERRORD("removeAtFront() failed.", status, errno);
			return(status);
		}

		// check if we have a goal node or not
		status = pnode->isGoal(startlist, otherlist);
		switch (status)
		{
		case OK:
			// update current path
			if ((status = updatepath(nodedepth, 
				olddepth, pnode, currentpath)) != OK)
				return(status);
			break;

		case NOMATCH:
		case MAXDEPTHEXCEEDED:
			// no clauses were generated. skip further
			// processing of this node.
			continue;

		case VALID:
			// update current path
			if ((status = updatepath(nodedepth, 
				olddepth, pnode, currentpath)) != OK)
				return(status);

			// goal node, print solution
			PrintSolution(pnode);
			PrintSolution(currentpath);
			PrintRenameVariables(pnode);

			// check if more than one solutions is required
			solutionsfound += 1;
			statistics[SolutionsFound] += 1;
			totalstatistics[TotalSolutionsFound] += 1;
			if (solutionsfound >= solutionsrequired)
			{
				return(VALID);
			}
			continue;

		case MAXCLAUSEEXCEEDED:
			// check if any solutions were found
			if (solutionsfound > 0)
			{
				return(VALID);
			}
			else
			{
				return(NOTPROVEN);
			}

		default:
			// some type of error
			ERRORD("isGoal() failed.", status, errno);
			return(status);
		}

		// generate the children of the current node
		if ((status = pnode->expand(startlist, otherlist)) != OK)
		{
			ERRORD("expand() failed.", status, errno);
			return(status);
		}

		// set up links to parent and calculate the heuristic value
		ListIterator<Proxy<NodeType> > 
			childrenIter(*pnode->getChildren());
		for ( ; !childrenIter.done(); childrenIter++)
		{
			// pointer to child
			pchild = childrenIter();

			// set up link to parent
			if (reporttype == ReportParent ||
			    reporttype == ReportBoth)
				pchild->setParent(pnode);

			// insert into stack
			if (!bfswithchecks)
			{
				pchild->setDepth(nodedepth+1);
				if ((status = openpq.insertAtFront(
					pchild)) != OK)
				{
					ERRORD("insertAtFront() failed.", 
						status, errno);
					return(status);
				}
			}
			else
			{
				statistics[RedundantClauseTestsAttempted] += 1;
				totalstatistics[TotalRedundantClauseTestsAttempted] += 1;
				String newnode(pchild->getNormalizedClauses());
				if (allstates.retrieve(newnode) == NOMATCH)
				{
					pchild->setDepth(nodedepth+1);
					if ((status = openpq.insertAtFront(
							pchild)) != OK)
					{
						ERRORD("insertAtFront() failed.", 
							status, errno);
						return(status);
					}
				}
				else
				{
					statistics[RedundantClausesRejected] += 1;
					totalstatistics[TotalRedundantClausesRejected] += 1;
				}
			}
		}

		// children are now in the queue. release pointers to
		// children stored in node.
		//
		pnode->getChildren()->clear();
		if (bfswithchecks)
		{
			if (allstates.insert(pnode->getNormalizedClauses()) != OK)
			{
				ERRORD("insert() failed.", status, errno);
				return(status);
			}
		}
	}

	// dump data info
	if (reportmemoryusage)
	{
		SETFINALEDATA();
		DUMPDATA(cout);
	}

	// check if any solutions were found
	if (solutionsfound > 0)
	{
		return(VALID);
	}
	else
	{
		return(NOTPROVEN);
	}
}

// depth-first with hill-climb search routine with a 
// termination-detection function 
//
template <class NodeType, class DataType>
int
DepthFirstHillClimbSearch(List<NodeType> &startstatelist, 
		BinaryTree_AVL<DataType> &startlist, 
		BinaryTree_AVL<DataType> &otherlist)
{
	int status;

	// track all states created
	BinaryTree_AVL<String> allstates;

	// create open stack for traversal
	List<Proxy<NodeType> > openstack;

	// keep track of current path here instead of 
	// using parent pointers. parent pointers cause
	// cycles and reference-counted pointers cannot
	// deal with cyclic data structures. they cause
	// major memory leaks.
	//
	int nodedepth = 1;
	List<Proxy<NodeType> > currentpath;

	// copy list of start states
	ListIterator<NodeType> startstateIter(startstatelist);
	for ( ; !startstateIter.done(); startstateIter++)
	{
		// copy start state
		Proxy<NodeType> pstart(startstateIter());

		// calculate the heuristic value for this node
		if ((status = pstart->heuristic(startlist, otherlist)) != OK)
		{
			ERRORD("heuristic() failed.", status, errno);
			return(status);
		}

		// set start node depth
		pstart->setDepth(nodedepth);

		// insert start state into open queue
		if ((status = openstack.insertOrdered(pstart)) != OK)
		{
			ERRORD("insertOrdered() failed.", status, errno);
			return(status);
		}
	}

	// children priority queue
	List<Proxy<NodeType> > childpq;
	
	// start search loop
	int olddepth = nodedepth;
	Proxy<NodeType> pchild;
	for (Proxy<NodeType> pnode; ! openstack.isEmpty(); )
	{
		// remove next node from priority open queue
		if ((status = openstack.removeAtFront(pnode)) != OK)
		{
			ERRORD("removeAtFront() failed.", status, errno);
			return(status);
		}

		// check if we have a goal node or not
		status = pnode->isGoal(startlist, otherlist);
		switch (status)
		{
		case OK:
			// update current path
			if ((status = updatepath(nodedepth, 
				olddepth, pnode, currentpath)) != OK)
				return(status);
			break;

		case NOMATCH:
		case MAXDEPTHEXCEEDED:
			// no clauses were generated. skip further
			// processing of this node.
			continue;

		case VALID:
			// update current path
			if ((status = updatepath(nodedepth, 
				olddepth, pnode, currentpath)) != OK)
				return(status);

			// goal node, print solution
			PrintSolution(pnode);
			PrintSolution(currentpath);
			PrintRenameVariables(pnode);

			// check if more than one solutions is required
			solutionsfound += 1;
			statistics[SolutionsFound] += 1;
			totalstatistics[TotalSolutionsFound] += 1;
			if (solutionsfound >= solutionsrequired)
				return(VALID);
			continue;

		case MAXCLAUSEEXCEEDED:
			// check if any solutions were found
			if (solutionsfound > 0)
				return(VALID);
			else
				return(NOTPROVEN);

		default:
			// some type of error
			ERRORD("isGoal() failed.", status, errno);
			return(status);
		}

		// generate the children of the current node
		if ((status = pnode->expand(startlist, otherlist)) != OK)
		{
			ERRORD("expand() failed.", status, errno);
			return(status);
		}

		// clear children priority queue
		childpq.clear();

		// set up links to parent and calculate the heuristic value
		ListIterator<Proxy<NodeType> > 
			childrenIter(*pnode->getChildren());
		for ( ; !childrenIter.done(); childrenIter++)
		{
			// pointer to child
			pchild = childrenIter();

			// set up link to parent
			if (reporttype == ReportParent ||
			    reporttype == ReportBoth)
				pchild->setParent(pnode);

			// calculate the heuristic value
			if ((status = pchild->heuristic(
				startlist, otherlist)) != OK)
			{
				ERRORD("heuristic() failed.", status, errno);
				return(status);
			}

			// insert child into a priority queue to order
			pchild->setDepth(nodedepth+1);
			if ((status = childpq.insertOrdered(pchild)) != OK)
			{
				ERRORD("insertOrdered() failed.", 
					status, errno);
				return(status);
			}
		}

		// insert nodes into stack ordered by heuristic value
		while (!childpq.isEmpty())
		{
			// remove next node from priority open queue
			if ((status = childpq.removeAtEnd(pchild)) != OK)
			{
				ERRORD("removeAtEnd() failed.", 
					status, errno);
				return(status);
			}

			// insert node into a stack
			if (!bfswithchecks)
			{
				if ((status = openstack.insertAtFront(
					pchild)) != OK)
				{
					ERRORD("insertAtFront() failed.", 
						status, errno);
					return(status);
				}
			}
			else
			{
				statistics[RedundantClauseTestsAttempted] += 1;
				totalstatistics[TotalRedundantClauseTestsAttempted] += 1;
				String newnode(pchild->getNormalizedClauses());
				if (allstates.retrieve(newnode) == NOMATCH)
				{
					if ((status = openstack.insertAtFront(
							pchild)) != OK)
					{
						ERRORD("insertAtFront() failed.", 
							status, errno);
						return(status);
					}
				}
				else
				{
					statistics[RedundantClausesRejected] += 1;
					totalstatistics[TotalRedundantClausesRejected] += 1;
				}
			}
		}

		// children are now in the queue. release pointers to
		// children stored in node.
		//
		pnode->getChildren()->clear();
		if (bfswithchecks)
		{
			if (allstates.insert(pnode->getNormalizedClauses()) != OK)
			{
				ERRORD("insert() failed.", status, errno);
				return(status);
			}
		}
	}

	// check if any solutions were found
	if (solutionsfound > 0)
		return(VALID);
	else
		return(NOTPROVEN);
}

// breadth-first search routine with a termination-detection function 
//
template <class NodeType, class DataType>
int
BreadthFirstSearch(List<NodeType> &startstatelist, 
		BinaryTree_AVL<DataType> &startlist, 
		BinaryTree_AVL<DataType> &otherlist)
{
	int status;

	// track all states created
	BinaryTree_AVL<String> allstates;

	// create open priority queue
	List<Proxy<NodeType> > openpq;

	// copy list of start states
	int nodedepth = 1;
	ListIterator<NodeType> startstateIter(startstatelist);
	for ( ; !startstateIter.done(); startstateIter++)
	{
		// copy start state
		Proxy<NodeType> pstart(startstateIter());

		// set start node depth
		pstart->setDepth(nodedepth);

		// insert start state into open queue
		if ((status = openpq.insertAtEnd(pstart)) != OK)
		{
			ERRORD("insertAtFront() failed.", status, errno);
			return(status);
		}
	}

	// start search loop
	for (Proxy<NodeType> pnode; ! openpq.isEmpty(); )
	{
		// remove next node from priority open queue
		if ((status = openpq.removeAtFront(pnode)) != OK)
		{
			ERRORD("removeAtFront() failed.", status, errno);
			return(status);
		}

		// set current node depth
		nodedepth = pnode->getDepth();

		// check if we have a goal node or not
		status = pnode->isGoal(startlist, otherlist);
		switch (status)
		{
		case OK:
			break;

		case NOMATCH:
		case MAXDEPTHEXCEEDED:
			// no clauses were generated. skip further
			// processing of this node.
			continue;

		case VALID:
			// goal node, print solution
			PrintSolution(pnode);
			PrintRenameVariables(pnode);

			// check if more than one solutions is required
			solutionsfound += 1;
			statistics[SolutionsFound] += 1;
			totalstatistics[TotalSolutionsFound] += 1;
			if (solutionsfound >= solutionsrequired)
				return(VALID);
			continue;

		case MAXCLAUSEEXCEEDED:
			// check if any solutions were found
			if (solutionsfound > 0)
				return(VALID);
			else
				return(NOTPROVEN);

		default:
			// some type of error
			ERRORD("isGoal() failed.", status, errno);
			return(status);
		}

		// generate the children of the current node
		if ((status = pnode->expand(startlist, otherlist)) != OK)
		{
			ERRORD("expand() failed.", status, errno);
			return(status);
		}

		// set up links to parent and calculate the heuristic value
		ListIterator<Proxy<NodeType> > 
			childrenIter(*pnode->getChildren());
		for ( ; !childrenIter.done(); childrenIter++)
		{
			// pointer to child
			Proxy<NodeType> pchild(childrenIter());

			// set up link to parent
			if (reporttype == ReportParent ||
			    reporttype == ReportBoth)
				pchild->setParent(pnode);

			// insert into queue
			if (!bfswithchecks)
			{
				pchild->setDepth(nodedepth+1);
				if ((status = openpq.insertAtEnd(
					pchild)) != OK)
				{
					ERRORD("insertAtEnd() failed.", 
						status, errno);
					return(status);
				}
			}
			else
			{
				statistics[RedundantClauseTestsAttempted] += 1;
				totalstatistics[TotalRedundantClauseTestsAttempted] += 1;
				String newnode(pchild->getNormalizedClauses());
				if (allstates.retrieve(newnode) == NOMATCH)
				{
					pchild->setDepth(nodedepth+1);
					if ((status = openpq.insertAtEnd(
							pchild)) != OK)
					{
						ERRORD("insertAtEnd() failed.", 
							status, errno);
						return(status);
					}
				}
				else
				{
					statistics[RedundantClausesRejected] += 1;
					totalstatistics[TotalRedundantClausesRejected] += 1;
				}
			}
		}

		if (bfswithchecks)
		{
			if (allstates.insert(pnode->getNormalizedClauses()) != OK)
			{
				ERRORD("insert() failed.", status, errno);
				return(status);
			}
		}
	}

	// check if any solutions were found
	if (solutionsfound > 0)
		return(VALID);
	else
		return(NOTPROVEN);
}


// iterative deeping search routine with a termination-detection function 
//
template <class NodeType, class DataType>
int
dfs(Proxy<NodeType> &pnode, int &threshold, int &next_threshold, 
	BinaryTree_AVL<DataType> &startlist, 
	BinaryTree_AVL<DataType> &otherlist)
{
	int status;

	// expand the current node's children
	if ((status = pnode->expand(startlist, otherlist)) != OK)
	{
		// some type of error
		ERRORD("expand() failed.", status, errno);
		return(status);
	}

	// scan children for goal nodes 
	ListIterator<Proxy<NodeType> > childrenIter(*pnode->getChildren());
	for ( ; !childrenIter.done(); childrenIter++)
	{
		// get pointer to child
		Proxy<NodeType> pchild(childrenIter());

		// set up link to parent
		if (reporttype == ReportParent ||
		    reporttype == ReportBoth)
			pchild->setParent(pnode);

		// calculate the heuristic value
		if ((status = pchild->heuristic(startlist, otherlist)) != OK)
		{
			ERRORD("heuristic() failed.", status, errno);
			return(status);
		}

		// check if we have a goal node
		status = pchild->isGoal(startlist, otherlist);
		switch (status)
		{
		case OK:
			break;

		case NOMATCH:
		case MAXDEPTHEXCEEDED:
			continue;

		case MAXCLAUSEEXCEEDED:
			return(status);

		case VALID:
			// goal node, print solution
			PrintSolution(pnode);
			PrintRenameVariables(pnode);
			return(VALID);

		default:
			// some type of error
			ERRORD("isGoal() failed.", status, errno);
			return(status);
		}

		// check if we continue, is threshold exceeded?
		int childcost = pchild->getFvalue();
		if (childcost <= threshold)
		{
			// continue depth-first search
			status = dfs(pchild, threshold, next_threshold,
					startlist, otherlist);
			switch (status)
			{
			case OK:
			case NOMATCH:
			case MAXDEPTHEXCEEDED:
			case NOTPROVEN:
				break;
	
			case MAXCLAUSEEXCEEDED:
				return(MAXCLAUSEEXCEEDED);

			case VALID:
				// goal node, print solution
				return(VALID);

			default:
				// some type of error
				ERRORD("dfs() failed.", status, errno);
				return(status);
			}
		}
		else if (childcost < next_threshold)
		{
			next_threshold = childcost;
		}
	}

	// goal was not not proven
	return(NOTPROVEN);
}

template <class NodeType, class DataType>
int
IterativeDeepeningSearch(Proxy<NodeType> &proot, int &threshold, 
	int &next_threshold, BinaryTree_AVL<DataType> &startlist, 
	BinaryTree_AVL<DataType> &otherlist)
{
	int status;

	// check if we have a goal node or not
	status = proot->isGoal(startlist, otherlist);
	switch (status)
	{
	case OK:
		break;

	case NOMATCH:
		return(NOTPROVEN);

	case VALID:
		// goal node, print solution
		PrintSolution(proot);
		PrintRenameVariables(proot);
		return(VALID);

	default:
		// some type of error
		ERRORD("isGoal() failed.", status, errno);
		return(status);
	}

	// calculate the heuristic value for root node
	if ((status = proot->heuristic(startlist, otherlist)) != OK)
	{
		ERRORD("heuristic() failed.", status, errno);
		return(status);
	}

	// set initial threshold values
	threshold = proot->getFvalue();
	next_threshold = INT_MAX;

	// start interative deepening, probe ahead
	while ( 1 )
	{
		status = dfs(proot, threshold, next_threshold,
				startlist, otherlist);
		switch (status)
		{
		case OK:
		case NOMATCH:
		case NOTPROVEN:
		case MAXDEPTHEXCEEDED:
			// reset thresholds
			if (next_threshold == INT_MAX)
			{
				ERROR("next_threshold is STILL INT_MAX.", 
					errno);
				return(NOTOK);
			}
			threshold = next_threshold;
			next_threshold = INT_MAX;
			break;

		case MAXCLAUSEEXCEEDED:
			return(NOTPROVEN);

		case VALID:
			return(status);

		default:
			// some type of error
			ERRORD("interative deepening dfs() failed.", 
				status, errno);
			return(status);
		}
	}
}

template <class NodeType, class DataType>
int
IterativeDeepeningSearch(List<NodeType> &startstatelist, 
		BinaryTree_AVL<DataType> &startlist, 
		BinaryTree_AVL<DataType> &otherlist)
{
	int status;

	// create start node queue
	List<Proxy<NodeType> > startpq;

	// copy list of start states
	ListIterator<NodeType> startstateIter(startstatelist);
	for ( ; !startstateIter.done(); startstateIter++)
	{
		// copy start state
		Proxy<NodeType> pstart(startstateIter());

		// calculate the heuristic value for this node
		if ((status = pstart->heuristic(startlist, otherlist)) != OK)
		{
			ERRORD("heuristic() failed.", status, errno);
			return(status);
		}

		// insert start state into open queue
		if ((status = startpq.insertOrdered(pstart)) != OK)
		{
			ERRORD("insertOrdered() failed.", status, errno);
			return(status);
		}
	}

	// threshold cutoff for iterative deepening search
	int threshold, next_threshold;

	// start search loop
	for (Proxy<NodeType> pnode; !startpq.isEmpty();)
	{
		// reset next clause to 1;
		nextClause = 1;

		// remove next node from start queue
		if ((status = startpq.removeAtFront(pnode)) != OK)
		{
			ERRORD("removeAtFront() failed.", status, errno);
			return(status);
		}

		// call iterative deepening routine with root node
		status = IterativeDeepeningSearch(pnode, threshold, 
				next_threshold, startlist, otherlist);
		switch (status)
		{
		case VALID:
			// check if more than one solutions is required
			solutionsfound += 1;
			statistics[SolutionsFound] += 1;
			totalstatistics[TotalSolutionsFound] += 1;
			if (solutionsfound >= solutionsrequired)
			{
				return(VALID);
			}
			break;

		case NOTPROVEN:
			// go on to next root node
			break;

		default:
			// some type of error
			ERRORD("IterativeDeepingSearch() failed.", 
				status, errno);
			return(status);
		}
	}

	// check if any solutions were found
	if (solutionsfound > 0)
	{
		return(VALID);
	}
	else
	{
		return(NOTPROVEN);
	}
}

