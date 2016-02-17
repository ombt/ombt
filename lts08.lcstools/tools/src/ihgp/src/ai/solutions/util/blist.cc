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

#include "blist.h"

BasList::BasList(const BasList &other)
{
    found = 0;
    copylist(other);
}

BasList &BasList::operator=(const BasList &other)
{
    if (this == &other)
	return(*this);

    clear();
    copylist(other);

    return(*this);
}

void BasList::copylist(const BasList &other)
{
    ListNode *node, *prevnode, *newnode;

    if (!(node = other.head))
	return;

    newnode = copynode(node);
    newnode->prev = 0;
    head = newnode;
    prevnode = newnode;

    while ((node = node->next))
    {
	newnode = copynode(node);
	newnode->prev = prevnode;
	prevnode->next = newnode;
	prevnode = newnode;
    }
    newnode->next = 0;
    tail = newnode;
    nodecount = other.nodecount;
}

int BasList::operator==(const BasList &other) const
{
    ListNode *n, *p;

    if (nodecount != other.nodecount)
	return(0);

    for (n = head, p = other.head; n; n = n->next, p = p->next)
	if (!compareeq(n, p))
	    return(0);
    return(1);
}

ListNode *BasList::operator[](int index) const
{
    ListNode *p;

    if (index < 0 || index >= nodecount)
	return(0);

    p = head;
    for (int i = 0; i < index; i++)
	p = p->next;

    return(p);
}


void BasList::addtohead(ListNode *node)
{
    nodecount++;

    node->prev = 0;
    node->next = head;
    if (head)
	head->prev = node;
    else
	tail = node;
    head = node;
}


void BasList::addtotail(ListNode *node)
{
    nodecount++;

    node->next = 0;
    node->prev = tail;
    if (tail)
	tail->next = node;
    else
	head = node;
    tail = node;
}


void BasList::remove_node(ListNode *node, int clr, int del)
{
    if (!node)
	return;

    nodecount--;

    if (node == found)
	found = 0;

    if (node == head)
    {
	if ((head = head->next) != 0)
	    head->prev = 0;
	if (tail == node)
	    tail = head;
    }
    else if (node == tail)
    {
	tail = tail->prev;
	tail->next = 0;
    }
    else
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    if (del)
    {
	if (clr)
	    clearnode(node);
	delete node;
    }
}


void BasList::clear(int clr, int del)
{
    ListNode *p;

    if (del)
    {
	if (clr)
	{
	    while(head)
	    {
		p = head;
		head = head->next;
		clearnode(p);
		delete p;
	    }
	}
	else
	{
	    while(head)
	    {
		p = head;
		head = head->next;
		delete p;
	    }
	}
    }
    head = tail = found = 0;
    nodecount = 0;
}


int BasList::find(ListNode *findme)
{
    ListNode *p;
    int i = 0;

    for (p = head; p; p = p->next)
    {
	if (compareeq(findme, p))
        {
            found = p;
            return(i);
	}
        i++;
    }

    return(-1);
}


ListNode *BasList::lookup(ListNode *findme)
{
    ListNode *p;

    for (p = head; p; p = p->next)
    {
        if (compareeq(findme, p))
        {
            found = p;
            return(p);
	}
    }

    return(0);
}


void BasList::insert(ListNode *node)
{
    ListNode *cur, *tmp;

    if (!head || comparelt(node, head))
        addtohead(node);
    else
    {
        for (cur = head->next; cur; cur = cur->next)
            if (comparelt(node, cur))
            {
                node->next = cur;
                node->prev = cur->prev;
                cur->prev = node;
                node->prev->next = node;
		nodecount++;
                break;
	    }
        if (!cur)
            addtotail(node);
     }
}
