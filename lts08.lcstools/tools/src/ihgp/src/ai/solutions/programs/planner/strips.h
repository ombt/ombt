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

#ifndef _STRIPS_H_
#define _STRIPS_H_

#include <stdio.h>
#include "xlist.h"
#include "logic.h"
#include "ainodes.h"
#include "aitree.h"

/*
 * StackItemType is used to differentiate stack objects. This is
 * useful when comparing stack objects - because the stack contains
 * objects of different types the StackItemType tells us if these objects
 * can be compared at all.
 */
enum StackItemType { T_Fact, T_FactList, T_Action };

/* Forward declaration of class StripsObject */
class StripsObject;

/*
 * BasFact is the base class for every class that can be put on the
 * stack. Three classes are derived from this class: Fact, FactList
 * and Action.
 * Since stack objects are stored on a heterogeneous list a type
 * is used to differentiate objects of one type from another. The type
 * of an object is determined by getstacktype().
 *
 * Member function process_strips() generates the successors of
 * an object of class StripsObject, so this is where the real work
 * is done.
 */
class BasFact
{
    public:
	virtual ~BasFact();
	virtual int operator==(const BasFact &) const = 0;
	virtual StackItemType getstacktype() const = 0;
	virtual BasFact *basdup() const = 0;
	BasFact *clone() const;
	virtual void display() const = 0;
	virtual void process_strips(StripsObject *, IntrList<Node> *) = 0;
	virtual void apply_subst(Substitution &) = 0;
};

/*
 * Class Fact represents a predicate: either a fact in the world or
 * a goal on the stack. Examples are: on(a, b), on(a, X1).
 */
class Fact : public BasFact, public Complex
{
    public:
	Fact(const char *str);
	int operator==(const BasFact &) const;
	StackItemType getstacktype() const;
	BasFact *basdup() const;
	Fact *clone() const;
	void display() const;
	void process_strips(StripsObject *, IntrList<Node> *);
	void apply_subst(Substitution &s);
    private:
	static StackItemType TypeId;
};

/*
 * Class FactList represents a conjunction of precidates:
 * either a collection of facts in the world, i.e. a state description,
 * or a compound goal on the stack. For example: on(a, b) & on(b, c).
 */
class FactList : public BasFact, public PtrList<Fact>
{
    public:
	FactList(int do_manage = 1);
	FactList &operator=(const FactList &);
	FactList &operator=(const char *);
	int operator==(const BasFact &) const;
	StackItemType getstacktype() const;
	FactList(const char *);
	BasFact *basdup() const;
	FactList *clone() const;
	void display() const;
	void process_strips(StripsObject *, IntrList<Node> *);
	void apply_subst(Substitution &);
    private:
	void process(const char *);

	static StackItemType TypeId;
};

class Frule;

/*
 * Class Action represents an action that changes one state desription
 * into another. For example: pickup(a).
 * The changes introduced by the Action object are represented by
 * an object of class F-rule. 
 */
class Action : public BasFact
{
    public:
	Action(Complex &, Frule *);
	int operator==(const BasFact &other) const;
	StackItemType getstacktype() const;
	void display() const;
	void process_strips(StripsObject *, IntrList<Node> *);
	void apply_subst(Substitution &s);
	BasFact *basdup() const;
	Action *clone() const;
	FactList* get_addlist() const;
	FactList* get_prelist() const;
	FactList* get_dellist() const;
    private:
	FactList *get_list(const FactList &) const;
	static StackItemType TypeId;

	Complex action;
	Frule *rule;
};

/*
 * Class F-rule represents the so called F-rules of STRIPS. It consists
 * of three components: a precondition formula, an add formula and a
 * delete list. The precondition formula consists of facts that must be
 * true in the world before the action can the F-rule can be applied.
 * The add formula contains facts that are added to the world and the
 * delete list contains facts that are removed from the world when the F-rule
 * is applied to the world.
 */
class Frule
{
    public:
	friend Action;
        Frule(char *, char *, char *, char *);
        void display() const;
	Action *create_action(Fact &);
    private:
        Complex name;
        FactList pre,
                 del,
                 add;
};

/*
 * Array of F-rule objects.
 */
#define MAXFRULE	4
extern Frule frulearr[MAXFRULE];

/*
 * Class StripsObject represents nodes in the search tree. StripsObject
 * contains the description of the world, the goal stack and the action
 * object that has last been applied. The solution path is composed of
 * these action objects.
 * The definition of the destructor ~StripsObject() with an empty
 * body is necessary to fix a gcc 2.7.2. problem.
 */
class StripsObject : public Node
{
    public:
        StripsObject();
	~StripsObject() { }	/* needed to fix a gcc 2.7.2 problem */
 	StripsObject(const char *, const char *);
        int operator==(const Node &) const;
	void display() const;
	IntrList<Node> *expand(int);
        StripsObject *dup() const;

        FactList world;
        PtrList<BasFact> stack;
	Complex action;
};

/*
 * Class Strips represents the search method that is applied to the
 * STRIPS problem. In this case depth-first search is selected.
 */
class Strips : public DepthTree
{
    public:
	Strips();
	Strips(StripsObject *);
	int is_goal(const Node *);
};

#endif
