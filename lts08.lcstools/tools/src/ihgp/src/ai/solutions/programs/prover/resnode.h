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

#include <stdio.h>
#include "clause.h"
#include "ctable.h"
#include "literal.h"
#include "aigraph.h"

class ResGraph;

/*
 * Class ResNode represents the nodes in the search graph. It contains
 * the resolvent and the side clause used with the resolvent to create
 * the next resolvent. If the node is the root node the resolvent
 * will be the top clause and the side clause will be empty. If the node
 * is a goal clause the resolvent will be the empty clause: [].
 */
class ResNode : public Node
{
    friend ResGraph;
    public:
	ResNode(Clause *, Clause *);
	~ResNode();

	int operator==(const Node &) const;
	void display() const;
	IntrList<Node> *expand(int);
    private:
	Clause *resolvent,
	       *side;
};

/*
 * Class ResGraph implements the search algorithm. In this case
 * the depth first algorithm is chosen. ResGraph also contains the
 * table consisting of the axioms (the theory) to be used in the
 * resolution proof.
 */
class ResGraph : public DepthGraph
{
    friend ResNode;
    public:
	ResGraph(ResNode *, ClauseTable &);
	void settable(ClauseTable &);
	int is_goal(const Node *);
    private:
	static ClauseTable *table;
};
