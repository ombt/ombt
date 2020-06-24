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

#include "resnode.h"

ClauseTable *ResGraph::table;

ResNode::ResNode(Clause *resolv, Clause *sid)
{
    resolvent = resolv;
    side = sid;
}

ResNode::~ResNode()
{
    delete(resolvent);
}

/*
 * Since the resolution procedure centers on resolvents it is sufficient
 * to check if the resolvents are equal when comparing two ResNode objects.
 * The idea is that if B is the resolvent of A & C and also of D & E
 * B has been generated twice by seperate procedures, i.e. by seperate
 * search paths. But only the resolvent is of interest and since B is
 * really the same resolvent, the ResNode objects that contain B are
 * effectively the same also.
 */
int ResNode::operator==(const Node &other) const
{
    const ResNode &resother = (ResNode &)other;
    return(*resolvent == *(resother.resolvent));
}

void ResNode::display() const
{
    if (side)
    {
	side->display();
	cout << '\n';
    }
    resolvent->display();
    cout << '\t';
}

/*
 * Expanding() a ResNode means finding side clauses in the DB that can be
 * used with the current clause to produce resolvent clauses. For each
 * side clause and new resolvent that is found a successor ResNode object
 * is created.
 */
IntrList<Node> *ResNode::expand(int)
{
    Clause
	*newclause;
    ResNode
	*tmp;
    IntrList<Node>
	*ret = new IntrList<Node>;

   int max = ResGraph::table->getsize();
   for (int i = 0; i < max; i++)
   {
	/*
	 * Try to resolve the current clause with the next clause in
	 * the DB. If this succeeds check if the resolvent clause is
	 * a tautology. If so, ignore it. If not, add the clause to the
	 * successor list. Tautologies are not taken into account in the
         * resolution process because a tautology is always true and does
         * not add any new information to the resolution process.
	 */
	newclause = resolvent->resolve(*ResGraph::table->getclause(i));
        if (newclause)
	{
	    if (!newclause->is_tautology())
	    {
	        tmp = new ResNode(newclause, ResGraph::table->getclause(i));
	        ret->addtotail(tmp);
	    }
	    else
		delete(newclause);
	}
   }
   return(ret);
}

ResGraph::ResGraph(ResNode *start, ClauseTable &ct)
    :DepthGraph(0, start)
{
    table = &ct;
}

void ResGraph::settable(ClauseTable &ct)
{
    table = &ct;
}

/*
 * If the resolvent represents the empty clause, [], the node 
 * is a goal node.
 */
int ResGraph::is_goal(const Node *n)
{
    ResNode *node = (ResNode *)n;
    return(node->resolvent->is_empty());
}

