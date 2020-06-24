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

#ifndef _BLIST_H_
#define _BLIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>

#define NoDel 0
#define DoDel 1
#define DontManage 0
#define DoManage 1

class ListNode
{
    friend class BasList;
    friend class SortedBasList;
    friend class BasListIterator;
    friend class ROBasListIterator;
    public:
	virtual ~ListNode()		{ }
    private:
        ListNode *next,
                 *prev;
};


class BasList
{
    friend class BasListIterator;
    friend class ROBasListIterator;
    public:
        BasList()			{ head = tail = found = 0; nodecount = 0; }
        virtual ~BasList()		{ clear(); }
	BasList(const BasList &other);
	BasList &operator=(const BasList &);
	int operator==(const BasList &) const;

	virtual int compareeq(ListNode *, ListNode *) const = 0;
	virtual int comparelt(ListNode *, ListNode *) const { return(0); }
	virtual void clearnode(ListNode *) { }
	virtual ListNode *copynode(ListNode *) const = 0;

	ListNode *operator[](int) const;

        void addtohead(ListNode *);
        void addtotail(ListNode *);
	void insert(ListNode *);

        ListNode *gethead() const	{ return(head); }
        ListNode *gettail() const	{ return(tail); }
	ListNode *getfound() const	{ return(found); }

        int getcount() const		{ return(nodecount); }
        int is_empty() const		{ return(head == 0); }

	int find(ListNode *);
	ListNode *lookup(ListNode *);

        void remove_head(int clr = 0, int del = 1)
         { remove_node(head, del, clr); }
        void remove_tail(int clr = 0, int del = 1)
         { remove_node(tail, del, clr); }
	void remove_found(int clr = 0, int del = 1)
         { remove_node(found, del, clr); }

        void clear(int clr = 0, int del = 1);
 
    protected:
        void remove_node(ListNode *, int, int);
	void copylist(const BasList &);
	ListNode *head,
                 *tail,
		 *found;
        int nodecount;
};


class SortedBasList : public BasList
{
    public:
	SortedBasList &operator=(const SortedBasList &);
};

inline
SortedBasList &SortedBasList::operator=(const SortedBasList &other)
{
    BasList::operator=(other);
    return(*this);
}


class BasListIterator
{
    public:
	BasListIterator(BasList &li)	{ mine = &li; current = li.head; }
	BasListIterator()		{ mine = 0; current = 0; }
	void setlist(BasList &li)   	{ mine = &li; current = li.head; }

	ListNode *getfirst()    { current = mine->head; return(current); } 
	ListNode *getlast()	{ current = mine->tail; return(current); }
	ListNode *getnext()     { return(current ? current = current->next : 0); }
	ListNode *getprev()	{ return(current ? current = current->prev : 0); }
	ListNode *getcurrent()	{ return(current); }

	int notempty() const		{ return(current != 0); }
	
	void remove_current(int clr = 0, int del = 1);
    private:
	BasList *mine;
	ListNode *current;
};

class ROBasListIterator
{
    public:
	ROBasListIterator(const BasList &li) { mine = &li; current = li.head; }
	ROBasListIterator()		     { mine = 0; current = 0; }
	void setlist(const BasList &li)      { mine = &li; current = li.head; }

	const ListNode *getfirst()	{ current = mine->head; return(current); } 
	const ListNode *getlast()	{ current = mine->tail; return(current); }
	const ListNode *getnext()	{ return(current ? current = current->next : 0); }
	const ListNode *getprev()	{ return(current ? current = current->prev : 0); }
	const ListNode *getcurrent()	{ return(current); }

	int notempty() const		{ return(current != 0); }
	
    private:
	const BasList *mine;
	const ListNode *current;
};

#endif
