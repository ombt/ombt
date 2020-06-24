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

#ifndef _AINODES_H_
#define _AINODES_H_

#include <stdio.h>
#include <stdlib.h>
#include "xarray.h"
#include "xlist.h"

/*
 * Class Node is derived from class ListNode and serves as a definition
 * of the states or nodes that are generated during the search.
 *
 * Application-level classes that are derived from class Node must
 * implement the following functions:
 *
 * - do_operator(): generates and returns one successor, i.e., a new state,
 *                  when operator n is applied or 0 (NULL) if operator n is
 *		    not applicable.
 *   or expand():   expands the node by generating all of its successors.
 *                  The successors are stored in a list object. This function
 *                  serves as an alternative for do_operator(), either one of
 *                  of these functions must be implemented.
 * - operator==:    determines if 2 objects are the same,
 *                  1: yes, 0: no.
 * - display():     displays the object.
 */
class Node : public ListNode
{
    public:
        Node();
	Node *clone() const;
        void setparent(Node *);
        Node *getparent() const;

	virtual int operator==(const Node &) const = 0;
	virtual int operator<(const Node &) const;
	virtual void display() const = 0;
        virtual Node *do_operator(int);
	virtual IntrList<Node> *expand(int);
    private:
        Node *parent;		// used to trace solution path
};


/*
 * Class DepthNode is a deritvative of class Node and is used by class
 * DepthTree and DepthGraph to perform a depth-bounded depth-first
 * search.
 */
class DepthNode : public Node
{
    public:
	DepthNode();
	unsigned getdepth() const;
	void setdepth(unsigned);
    private:
	unsigned depth;		// depth of node
};


/*
 * Class UniNode is derived from class Node and defines nodes that
 * are generated during a uniform cost search. Nodes of this type
 * are placed in an ordered list based on the G-value of the node.
 * G is the cost of getting from the start node to the current node,
 * i.e. the cost of the path generated so far. G is computed by
 * compute_g().
 */
class UniNode : public Node
{
    public:
        UniNode();
        int operator<(const Node &) const;
        int get_g() const;
        void set_g(int);
    private:
        int g;		// cost of getting from the start node to this node
};


/*
 * Class BestNode is derived from class UniNode and defines nodes
 * that are generated during a best-first search (more specifically:
 * by the A* algorithm). These nodes are placed in an ordered list
 * based on the F-value of the node. F is the sum of two costs: G, the
 * cost of getting from the start node to this node (see UniNode)
 * and H, a heuristic estimate of getting from this node to the goal
 * node. G is computed by compute_g() and H is computed by compute_h().
 */
class BestNode : public UniNode
{
    public:
        BestNode();
        int operator<(const Node &) const;
        int get_f() const;
        void set_f(int);
    private:
        int
            f;          // F: G + H
};


/*
 * Class AONode is derived from class Node and defines nodes that
 * are generated during an AND-OR search. AONode is a base class for
 * class ORNode and ANDNode and must never be used for direct derivation
 * by application-level classes. The meaning of n_left depends on the
 * type of the node: if the node is an AND node n_left means the number of
 * successor nodes that must be solved before the AND node itself becomes
 * solvable; in case of an OR node n_left means the number of successor
 * nodes that may still be solved to make the OR node itself solvable.
 */

enum TypeOfNode { OR, AND };
enum NodeStat { Unsolvable, Solved, Undef };

class AONode : public Node
{
    public:
	AONode();
	AONode(int);
	AONode *clone() const;
	void incn_left();

        virtual TypeOfNode gettype() const = 0;
        virtual int get_nsucc() const = 0; 
	virtual AONode *getsucc(int = 0) const = 0;
	virtual void setstatus(NodeStat) = 0;
        virtual NodeStat getstatus() const = 0;
	virtual int setsolved(AONode *) = 0;
	virtual int setunsolvable() = 0;
    protected:
        NodeStat status;
        int n_left;
};


/*
 * Class AndNode is derived from class AONode and represents AND nodes
 * of an AND-OR search. Note that the the terminology for AND and OR nodes
 * used here differs from standard terminology. Here, an AND node is a
 * collection of other nodes, each of which must be solved in order for the
 * AND node to be solved. Any other node is an OR node. In standard
 * terminology an AND node is a node that is part of a collection
 * of nodes each of which must be solved in order to make their
 * parent node solved and any other node is an OR node.
 * An object of class AndNode is created by the application if a set
 * of sub-problems, i.e. a set of successors, is generated each of which must
 * be solved to make the parent node solved. After an AndNode has been
 * created each of these successor nodes must be passed to the AndNode
 * by calling addsucc().
 * Class AndNode must never be used for derivation by the application.
 */
class AndNode : public AONode
{
    public:
	AndNode();
	AndNode(int n_nodes);

	TypeOfNode gettype() const;
	void setstatus(NodeStat);
        NodeStat getstatus() const;
	int setsolved(AONode *);
	int setunsolvable();
	AONode *getsucc(int) const;     // get successor n from succlist
        int get_nsucc() const;
	void setsucc(int, AONode *);    // set successor n in succlist
	void addsucc(AONode *);         // add a successor to succlist
	void display() const;
        int operator==(const Node &) const;
    private:
        static TypeOfNode type;

	PtrArray<AONode> succlist;	// this node's successors
};


/*
 * Class OrNode is derived from class AONode and defines OR nodes of
 * an AND-OR search. This class must be used for derivation by the
 * application (and not class AONode or class ANDNode).
 */
class OrNode : public AONode
{
    public:
	OrNode();

        TypeOfNode gettype() const;
        int get_nsucc() const;
	AONode *getsucc(int n) const;
	void setstatus(NodeStat);
        NodeStat getstatus() const;
	int setsolved(AONode *);
	int setunsolvable();
    private:
        static TypeOfNode type;

	AONode *succ;			// this node's successor
};


/*
 * Class BackNode is derived from ListNode and defines nodes that 
 * are generated by a backtrack search.
 *
 * Application-level classes that are derived from class Node must
 * implement the following functions:
 *
 * - do_operator(): generates and returns one successor, i.e., a new state,
 *                  when operator n is applied or 0 (NULL) if operator n
 *		    is not applicable.
 * or expand_one(): expands the node by generating the next successor.
 *                  This function serves as an alternative for do_operator(),
 *		    either one of of these functions must be implemented.
 * - operator==:    determines if 2 objects are the same,
 *                  1: yes, 0: no.
 * - display():     displays the object.
 */
class BackNode : public ListNode
{
    public:
        BackNode();
        virtual int operator==(const BackNode &) const = 0;
        virtual BackNode *do_operator(int);
        virtual BackNode *expand_one(int);
        virtual void display() const = 0;
        BackNode *clone() const;
    private:
        int last_op;		// operator that was applied last
};
 
#endif


