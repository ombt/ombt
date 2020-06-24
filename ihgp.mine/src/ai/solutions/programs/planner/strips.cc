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

StripsObject::StripsObject()
{
}

StripsObject::StripsObject(const char *wrld, const char *stck)
{
    FactList *fl = new FactList(stck);

    world = wrld;
    stack.addtohead(fl);
}

int StripsObject::operator==(const Node &other) const
{
    if (!(world == ((StripsObject &)other).world))
	return(0);
    if (!(stack == ((StripsObject &)other).stack))
	return(0);
    return(action == ((StripsObject &)other).action);
}

void StripsObject::display() const
{
    if (action.notempty())
    {
	action.display();
	cout << '\n';
    }
}

/*
 * dup() copies the complete StripsObject except for the action.
 */
StripsObject *StripsObject::dup() const
{
    StripsObject *ret = new StripsObject;
    ret->world = world;
    ret->stack = stack;
    return(ret);
}

/*
 * expand() generates the successors of the StripsObject. In reality
 * the real work is done in process_strips() defined in class BasFact
 * and the classes derived from it. expand() takes the first item
 * from the stack and calls process_strips() on it to process
 * "itself".
 */
IntrList<Node> *StripsObject::expand(int)
{
    BasFact *object;
    IntrList<Node> *succs = new IntrList<Node>;

    /*
     * Get the first item of the stack. If the stack is empty
     * we're through.
     */
    object = stack.gethead();

    if (!object)
    {
	/*
	 * Create a successor. The stack of this successor will be
	 * empty which will serve as the stop condition: when is_goal()
	 * is called on this object it will return true.
	 */
	StripsObject *kid = new StripsObject;
	succs->addtohead(kid);
    }

    /*
     * Use the item that was on top of the stack to process the
     * current StripsObject. This will update the StripsObject and generate
     * its successors.
     */
    object->process_strips(this, succs);

    /*
     * Once we've expanded the object the stack and world are no longer
     * needed (this would be different if we were doing a graph search
     * instead of a tree search), so we call clear() to free the memory
     * used by them.
     */
    stack.clear(DoDel);
    world.clear(DoDel);

#ifdef DEBUG
    IntrListIterator<Node> iter(*succs);
    StripsObject *b;

    cout << "\nSuccessors are:\n";
    for (b = (StripsObject *)iter.getfirst(); b; b = (StripsObject *)iter.getnext())
    {
        PtrListIterator<BasFact> it(b->stack);
	BasFact *bf;

        cout << "world is:\n";
        b->world.display();

        cout << "\nstack is:\n";
        for (bf = it.getfirst(); bf; bf = it.getnext())
        {
            bf->display();
	    cout << '\t';
        }
        cout << '\n';
    }

    cout << "Action object is:\n";
    action.display();
    cout << '\n';
#endif

    return(succs);
}

Strips::Strips()
    : DepthTree(0)
{
}

Strips::Strips(StripsObject *start)
    : DepthTree(0, start)
{
}

int Strips::is_goal(const Node *g)
{
    const StripsObject *s = (StripsObject *)g;

    /*
     * The node is a goal node if the goal stack of the node
     * is empty.
     */
    return(s->stack.is_empty());
}
