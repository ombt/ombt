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

#include "strips.h"

StackItemType Fact::TypeId = T_Fact;
StackItemType FactList::TypeId = T_FactList;
StackItemType Action::TypeId = T_Action;

BasFact::~BasFact()
{
}

BasFact *BasFact::clone() const
{
    return(basdup());
}

Fact::Fact(const char *str)
    : Complex(str)
{
}

int Fact::operator==(const BasFact &other) const
{
    if (other.getstacktype() != T_Fact)
	return(0);
    return(Complex::operator==((Fact &)other));
}

StackItemType Fact::getstacktype() const
{
    return(TypeId);
}

BasFact *Fact::basdup() const
{
    return(clone());
}

Fact *Fact::clone() const
{
    return(new Fact(*this));
}

void Fact::display() const
{
    Complex::display();
}

void Fact::apply_subst(Substitution &s)
{
    Complex::apply_subst(s);
}

void Fact::process_strips(StripsObject *parent, IntrList<Node> *succs)
{
    PtrListIterator<Fact> iter(parent->world);
    Fact *f;

    StripsObject *kid = 0;

    /*
     * Check if the fact is true in the current configuration of
     * the world. There are two possibilities: the fact matches
     * another fact in the world without needing a substitution,
     * in this case we're through immediately because the fact
     * is a ground term. If a subsitution is needed by the match
     * other substitutions may be possible as well; all of these must
     * be regarded because each may lead to a (or the) solution.
     */
    for (f = iter.getfirst(); f; f = iter.getnext())
    {
        Substitution subst;

	/* Empty the substitution. */
	subst.clear();

	/* Compare the fact with a fact that is in the world. */
	if (unify(*f, subst))
	{
	    /*
	     * Create a successor of the parent node: copy the
	     * parent node and pop off the first item which is
	     * now true from the goal stack of the child.
	     */
	    kid = parent->dup();
	    kid->stack.remove_head(DoDel);

	    /* Add successor to the other successors of the parent. */
	    succs->addtotail(kid);
	
	    /*
	     * If the substition is empty the term is ground,
	     * in which case we're through at once.
	     */
	    if (subst.is_empty())
	        break;
	    /*
	     * If the term was not ground the goal stack of the
	     * successor must be updated with the information contained
	     * in the substitution.
	     */
	    else
	    {
		PtrListIterator<BasFact> it(kid->stack);

		for (BasFact *b = it.getfirst(); b; b = it.getnext())
		    b->apply_subst(subst);
	    }
	}
    }

    /*
     * If the fact could not be unified with any fact in
     * the world find out which actions will make the fact true.
     */
    if (!kid)
    {
        Action *act;

	/* Check all Frules to see if they can give us the needed action. */
        for (int i = 0; i < MAXFRULE; i ++)
        {
	    /*
	     * create_action() checks wether the fact can be unified
	     * with any of the facts in the add-list of the Frule.
	     * If it does, an Action object is returned. For the creation
	     * of this Action object create_action() uses the binding that
	     * resulted from the unification of both facts.
	     */
            act = frulearr[i].create_action(*this);

	    if (act)
	    {
		/*
		 * Check for repeated actions. If the action is already
		 * on the stack it must not be pushed again. Without this
		 * check the planning process could end up in a cycle.
		 */
		if (parent->stack.lookup(act) != 0)
		    delete act;
		else
		{
		    FactList *precond;

		    /* Create a successor of the parent node. */
		    kid = parent->dup();

		    /*
		     * Add the action and the preconditions of the action
		     * to the goal stack of the successor. The preconditions
		     * will be added as a compound goal, i.e. as a FactList.
		     */
		    kid->stack.addtohead(act);
		    precond = act->get_prelist();
		    kid->stack.addtohead(precond);

		    /* Add successor to the other successors of the parent. */
		    succs->addtotail(kid);
		}
	    }
	}
    }
}

    
FactList::FactList(int do_manage)
    : PtrList<Fact>(do_manage)
{
}

FactList::FactList(const char *str)
{
    process(str);
}

FactList &FactList::operator=(const FactList &other)
{
    PtrList<Fact>::operator=(other);
    return(*this);
}

FactList &FactList::operator=(const char *str)
{
    process(str);
    return(*this);
}

StackItemType FactList::getstacktype() const
{
    return(TypeId);
}

BasFact *FactList::basdup() const
{
    return(clone());
}

FactList *FactList::clone() const
{
    FactList *ret = new FactList;
    *ret = *this;
    return(ret);
}

int FactList::operator==(const BasFact &other) const
{
    if (other.getstacktype() != T_FactList)
	return(0);
    return(PtrList<Fact>::operator==((PtrList<Fact> &)other));
}

/*
 * Process a string consisting of expressions like "on(a, b) & on(c, d)"
 * and turn it into a FactList object.
 */
void FactList::process(const char *str)
{
    char foo[256];
    const char *a = str;
    Fact *f;

    while (1)
    {
	if (a = strchr(str, '&'))
	{
	    strncpy(foo, str, a - str);
	    *(foo + (a - str)) = '\0';
	    f = new Fact(foo);
	    addtotail(f);
	    while (isspace(*++a))
		;
	    str = a;
	}
	else if (a = strchr(str, '\0'))
	{
	    f = new Fact(str);
	    addtotail(f);
	    break;
	}
    }
}

void FactList::display() const
{
    const Fact *f;
    ROPtrListIterator<Fact> iter(*this);

    if (is_empty())
	return;

    f = iter.getfirst();
    f->display();

    while (f = iter.getnext())
    {
	cout << " & ";
	f->display();
    }
}

void FactList::process_strips(StripsObject *parent, IntrList<Node> *succs)
{
    PtrListIterator<Fact> iter(*this);
    PtrList<Fact> nottrue(DontManage);
    StripsObject *kid;

    /*
     * A compound goal is true if each of its subgoals is true. If
     * a subgoal is found that is not true in the current configuration
     * of the world it will be reinserted on the stack.
     *
     * Note that we take a "shortcut" here. To determine if a compound
     * goal is true all of its subgoals must unify with a fact in the world
     * and all mgu's (the subsitutions that are introduced by the unifications
     * of these subgoals with a fact in the world) must be
     * compatible. Because there may be several mgu's we would
     * have to check a lot of possible combinations. That's why
     * a different solution has been chosen: we check if there is
     * a direct match between the subgoal and a fact in the world (i.e
     * variables are not taken into account, so no unification takes
     * place). Every subgoal that can't be matched will be placed on
     * the stack. Any variables that the subgoal might contain will be
     * bound as soon as the subgoal is popped off the stack or when
     * the stack is updated with a new binding.
     */
    for (Fact *f = iter.getfirst(); f; f = iter.getnext())
	if (!parent->world.lookup(f))
	   nottrue.addtohead(f->clone());

    /*
     * If there were any unsolved subgoals create a successor of the
     * parent and update the stack of this successor with these subgoals.
     *
     * We take another shortcut: normally n successors must be
     * generated where n is the number of permutations of the list of
     * unsolved subgoals. But usually we can get away with this without
     * any problems, so we generate only one successor.
     */
    if (!nottrue.is_empty())
    {
	Fact *f;

	kid = parent->dup();

	succs->addtotail(kid);

	iter.setlist(nottrue);
	for (f = iter.getfirst(); f; f = iter.getnext())
	    kid->stack.addtohead(f);
    }
    /*
     * If there were no unsolved goals left create a new successor
     * and pop off the first item of its stack because the compound
     * goal is now solved.
     */
    else
    {
	kid = parent->dup();

	kid->stack.remove_head(DoDel);
	succs->addtotail(kid);
    }
}

void FactList::apply_subst(Substitution &subst)
{
    PtrListIterator<Fact> iter(*this);

    for (Fact *f = iter.getfirst(); f; f = iter.getnext())
	f->apply_subst(subst);
}

Frule::Frule(char *rule, char *prelist, char *dellist, char *addlist)
{
    name = rule;
    pre = prelist;
    del = dellist;
    add = addlist;
}

void Frule::display() const
{
    name.display();
    cout << "\n\tP: ";
    pre.display();
    cout << "\n\tD: ";
    del.display();
    cout << "\n\tA: ";
    add.display();
    cout << '\n';
}

Action *Frule::create_action(Fact &findme)
{
    PtrListIterator<Fact> iter(add);
    Substitution subst;
    Action *act = 0;

    /*
     * Determine if the fact unifies with any facts in the add-list
     * of the F-rule. 
     *
     * The order of the terms in the unification is important.
     * By unifying a fact in the add-list with the supplied fact instead
     * of the other way around we make sure that we get the correct binding,
     * e.g. Y/Y5 instead of Y5/Y. This is important because the binding will
     * be applied to the Action object that is created and if there are any
     * variables in the supplied fact that were left unbound these same
     * variables must return in the Action object, so that if the stack is
     * updated with new binding information the variables in the Action object
     * will get bound to correct values, i.e. to the same values as those of
     * the variables that were left unbound in the fact.
     */
    for (Fact *f = iter.getfirst(); f; f = iter.getnext())
	if (f->unify(findme, subst))
	{
	    /*
	     * Create an action object. The action object must be
	     * passed an updated copy of the 'name' of the F-rule. The
	     * variables of this copy must be bound to the values
	     * specified in the substitution. Furthermore, any variables
	     * that are left unbound must be replaced by fresh variable
	     * names so that each time a new variable is introduced on the
	     * goal stack it is given a distinct name from any other
             * variables already on the stack. update_vars() will perform
	     * both of these tasks in one step.
	     */
	    Complex copy = name;

	    copy.update_vars(subst);
	    act = new Action(copy, this);

	    break;
	}
    return(act);
}

Action::Action(Complex &c, Frule *f)
{
    action = c;
    rule = f;
}

int Action::operator==(const BasFact &other) const
{
    if (other.getstacktype() != T_Action)
	return(0);

    Substitution s;
    Action &alter = (Action &)other;

    return(action.unify(alter.action, s));
}

StackItemType Action::getstacktype() const
{
    return(TypeId);
}

void Action::display() const
{
    action.display();
}

BasFact *Action::basdup() const
{
    return(clone());
}

Action *Action::clone() const
{
    return(new Action(*this));
}

void Action::process_strips(StripsObject *parent, IntrList<Node> *succs)
{
    StripsObject *kid;
    PtrListIterator<Fact> iter;
    Substitution subst;
    Fact *f;

    /*
     * If an Action object is found on top of the stack it is true
     * by definition because all of its preconditions have been popped
     * off already. The action object must be applied to current
     * configuration of the world. This means that a successor will be
     * generated and the facts in the del-list of the Action must be removed
     * from the world of the successor and the facts in the add-list must be
     * added to it. Also the Action itself must be recorded with the parent
     * node because it is part of the solution path.
     */

    /* Record the action. */
    parent->action = action;

    /* Create successor node and add it to the successor list. */
    kid = parent->dup();
    succs->addtotail(kid);

    /* Pop the Action object off the stack of the successor. */
    kid->stack.remove_head(DoDel);

    /*
     * The variables in both the add-list and the del-list of the
     * F-rule that the Action object points to are unbound variables.
     * To determine the actual values of the variables we must unify
     * the Complex object of the Action with the Complex object
     * (the name) of the F-rule and next apply the resulting unification
     * to the terms in the add-list and del-list.
     *
     * Note that the order of the terms in the unification is important.
     * See comment at create_action().
     */
    rule->name.unify(action, subst);

    iter.setlist(rule->del);

    /*
     * Apply the unification to the terms in the del-list and remove
     * these facts from the world of the successor.
     */
    for (f = iter.getfirst(); f; f = iter.getnext())
    {
	/*
	 * The F-rule itself must not be updated, so create
	 * a copy of each fact and apply the substitution to this copy.
	 */
	Fact c = *f;
	c.apply_subst(subst);

	/* Remove the fact from the world. */
	kid->world.lookup(&c);
	kid->world.remove_found(DoDel);
    }

    /*
     * Apply the unification to the terms in the add-list and add
     * these facts to the world of the successor.
     */
    iter.setlist(rule->add);
    for (f = iter.getfirst(); f; f = iter.getnext())
    {
	/*
	 * The F-rule itself must not be updated, so create
	 * a copy of each fact and apply the substitution to this copy.
	 */
	Fact *c = f->clone();
	c->apply_subst(subst);

	/*
	 * Add the fact to the world but only if it does not already
	 * exist yet in the world.
	 */
	if (!kid->world.lookup(c))
	    kid->world.addtohead(c);
	else
	    delete c;
    }

}

void Action::apply_subst(Substitution &s)
{
    action.apply_subst(s);
}

/*
 * get_addlist() returns the delete list of the Action object.
 */
FactList* Action::get_addlist() const
{
    return(get_list(rule->add));
}

/*
 * get_prelist() returns the delete list of the Action object.
 */
FactList* Action::get_prelist() const
{
    return(get_list(rule->pre));
}

/*
 * get_dellist() returns the delete list of the Action object.
 */
FactList* Action::get_dellist() const
{
    return(get_list(rule->del));
}

/*
 * get_list() is called by get_addlist(), get_dellist() and get_prelist()
 * to return the add list, delete list of precondition list of the Action
 * object respectively. Because the Action object points to a generic F-rule
 * object, we must first bind the variables in the list to their correct
 * values. This is done by unifying the "name" of the action with the name
 * of the F-rule and next applying the resulting unification to a copy
 * of the facts in the F-rule.
 */
FactList *Action::get_list(const FactList &list) const
{
    ROPtrListIterator<Fact> iter(list);
    Substitution subst;
    FactList *ret = new FactList;
    const Fact *f;

   /*
    * Unify the action with the F-rule.
    * Note that the order of the terms in the unification is important.
    * See comment at create_action().
    */
    rule->name.unify(action, subst);

    /*
     * Copy each fact of the list in the F-rule object and apply
     * to it the substitution just obtained.
     */
    for (f = iter.getfirst(); f; f = iter.getnext())
    {
	Fact *c;

	c = f->clone();
	c->apply_subst(subst);
	ret->addtotail(c);
    }

    return(ret);
}

