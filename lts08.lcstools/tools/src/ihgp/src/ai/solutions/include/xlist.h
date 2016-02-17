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

#ifndef _LIST_H_
#define _LIST_H_

#include <string.h>
#include "blist.h"

/*
 * It would be cleaner to make 'data' private and the classes access
 * access it friend. However, BCC 3.1 doesn't handle forward declaration
 * of template classes very well and without these it doesn't work either.
 */
template <class T>
class TListNode : public ListNode
{
    public:
	TListNode(const T &obj)	: data(obj) { }
	T data;
};

/*
 *  Note the definition of the copy constructor. We can't call BasList's
 *  copy constructor directly because it calls virtual function copynode().
 *  This function is (still) pure virtual when BasList is being constructed
 *  so this would hopelessly fail (believe me, I tried).
 */
template <class T>
class List : private BasList
{
    public:
	List() : BasList() { }
#ifdef NEED_CAST
	List(const List<T> &other) { BasList::copylist((BasList &)other); }
	int operator==(const List<T> &other) const { return(BasList::operator==((BasList &)other)); }
#else
	List(const List<T> &other) { BasList::copylist(other); }
#endif
	List<T> &operator=(const List<T> &other);

	int compareeq(ListNode *n1, ListNode *n2) const;
	ListNode *copynode(ListNode *) const;

        void addtohead(const T &a) { BasList::addtohead(new TListNode<T>(a)); }
        void addtotail(const T &a) { BasList::addtotail(new TListNode<T>(a)); }

	T &operator[](int);
	const T &operator[](int) const;
        T &gethead() const;
        T &gettail() const;
	T &getfound() const;
	int getcount() const	   { return(BasList::getcount()); }
	int is_empty() const	   { return(BasList::is_empty()); }

        int find(const T &n)  { TListNode<T> tmp(n); return(BasList::find(&tmp)); }
	T *lookup(const T &n);

	void remove_head()	   { BasList::remove_head(); }
	void remove_tail()	   { BasList::remove_tail(); }
	void remove_found()	   { BasList::remove_found(); }
	void clear()		   { BasList::clear(); }
    private:
	void error(const char *str) const;
    protected:
	void insert(const T &a)	    { BasList::insert(new TListNode<T>(a)); }
};

template <class T>
inline List<T> &List<T>::operator=(const List<T> &other)
{
    BasList::operator=(other);
    return(*this);
}

template <class T>
inline ListNode *List<T>::copynode(ListNode *node) const
{
    return(new TListNode<T>(((TListNode<T> *)node)->data));
}

template <class T>
inline int List<T>::compareeq(ListNode *n1, ListNode *n2) const
{
    return(((TListNode<T> *)n1)->data == ((TListNode<T> *)n2)->data);
}


template <class T>
void List<T>::error(const char *str) const
{
    puts(str);
    exit(0);
}

template <class T>
inline T &List<T>::operator[](int index)
{
    TListNode<T> *node = (TListNode<T> *)BasList::operator[](index);

    if (!node)
	error("subscript [] out of range");
    return(node->data);
}

template <class T>
inline const T &List<T>::operator[](int index) const
{
    TListNode<T> *node = (TListNode<T> *)BasList::operator[](index);

    if (!node)
	error("subscript [] out of range");
    return(node->data);
}

template <class T>
inline T &List<T>::gethead() const
{
    if (!head)
	error("gethead() from empty list");
    return(((TListNode<T> *)head)->data);
}

template <class T>
inline T &List<T>::gettail() const
{
    if (!tail)
	error("gettail() from empty list");
    return(((TListNode<T> *)tail)->data);
}

template <class T>
inline T &List<T>::getfound() const
{
    if (!found)
	error("found not initialized");
    return(((TListNode<T> *)found)->data);
}

template <class T>
inline T *List<T>::lookup(const T &n)
{
    TListNode<T> tmp(n), *t;
    t = (TListNode<T> *)BasList::lookup(&tmp);
    return(t ? &t->data : 0);
}

template <class T>
class SortedList : public List<T>
{
    public:
	SortedList<T> &operator=(const SortedList<T> &);
	int comparelt(ListNode *, ListNode *) const;

	void insert(const T &a)		{ List<T>::insert(a); }
};

template <class T>
inline SortedList<T> &SortedList<T>::operator=(const SortedList<T> &other)
{
    List<T>::operator=(other);
    return(*this);
}

template <class T>
inline int SortedList<T>::comparelt(ListNode *n1, ListNode *n2) const
{
    return(((TListNode<T> *)n1)->data < ((TListNode<T> *)n2)->data);
}

template <class T>
class PtrList : private BasList
{
    public:
	PtrList(int do_manage = 1)		{ manage = do_manage; }
#ifdef NEED_CAST
	PtrList(const PtrList<T> &other) { BasList::copylist((BasList &)other);
					   manage = other.manage; }
	int operator==(const PtrList<T> &other) const { return(BasList::operator==((BasList &)other)); }
#else
	PtrList(const PtrList<T> &other) { BasList::copylist(other);
					   manage = other.manage; }
#endif
	~PtrList()				{ clear(manage); }
	PtrList<T> &operator=(const PtrList<T> &other);

	int compareeq(ListNode *, ListNode *) const;
	void clearnode(ListNode *);
	ListNode *copynode(ListNode *) const;

	void addtohead(T *a)	{ BasList::addtohead(new TListNode<T *>(a)); }
	void addtotail(T *a)	{ BasList::addtotail(new TListNode<T *>(a)); }

	T *operator[](int);
	const T *operator[](int) const;
	T *gethead() const;
	T *gettail() const;
	T *getfound() const;
	int getcount() const		{ return(BasList::getcount()); }
	int is_empty() const		{ return(BasList::is_empty()); }
	int getmanage() const		{ return(manage); }
	void setmanage(int m)		{ manage = m; }

	int find(T *);
	T *lookup(T *);

	void remove_head(int clr)	{ BasList::remove_head(clr); }
	void remove_tail(int clr)	{ BasList::remove_tail(clr); }
	void remove_found(int clr)	{ BasList::remove_found(clr); }

	void clear(int clr)		{ BasList::clear(clr); }
    private:
	int manage;
    protected:
	void insert(T *a)	{ BasList::insert(new TListNode<T *>(a)); }
};

template <class T>
inline ListNode *PtrList<T>::copynode(ListNode *n) const
{
    if (manage)
	return(new TListNode<T *>(((TListNode<T *> *)n)->data->clone()));
    else
	return(new TListNode<T *>(((TListNode<T *> *)n)->data));
}

template <class T>
inline void PtrList<T>::clearnode(ListNode *n)
{
    delete ((TListNode<T *> *)n)->data;
}

template <class T>
inline int PtrList<T>::compareeq(ListNode *n1, ListNode *n2) const
{
    return(*((TListNode<T *> *)n1)->data == *((TListNode<T *> *)n2)->data);
}

template <class T>
PtrList<T> &PtrList<T>::operator=(const PtrList<T>&other)
{
    if (this == &other)
	return(*this);

    BasList::clear(manage);
    manage = other.manage;
    BasList::operator=(other);
    return(*this);
}

template <class T>
inline T *PtrList<T>::gethead() const
{
    return(head? ((TListNode<T *> *)head)->data : 0);
}

template <class T>
inline T *PtrList<T>::gettail() const
{
    return(tail? ((TListNode<T *> *)tail)->data : 0);
}

template <class T>
inline T *PtrList<T>::getfound() const
{
    return(found? ((TListNode<T *> *)found)->data : 0);
}

template <class T>
inline T *PtrList<T>::operator[](int index)
{
    TListNode<T *> *n = (TListNode<T *> *)BasList::operator[](index);
    return(n ? n->data : 0);
}

template <class T>
inline const T *PtrList<T>::operator[](int index) const
{
    TListNode<T *> *n = (TListNode<T *> *)BasList::operator[](index);
    return(n ? n->data : 0);
}

template <class T>
inline int PtrList<T>::find(T *s)
{
    TListNode<T *> tmp(s);
    return(BasList::find(&tmp));
}

template <class T>
inline T *PtrList<T>::lookup(T *s)
{
    TListNode<T *> tmp(s), *n;
    n = (TListNode<T *> *)BasList::lookup(&tmp);
    return(n ? n->data : 0);
}


template <class T>
class SortedPtrList : private PtrList<T>
{
    public:
	SortedPtrList<T> &operator=(const SortedPtrList<T> &);
	int comparelt(ListNode *, ListNode *) const;

	void insert(T *a)	{ PtrList<T>::insert(a); }
};

template <class T>
inline SortedPtrList<T> &SortedPtrList<T>::operator=(const SortedPtrList<T> &other)
{
    PtrList<T>::operator=(other);
    return(*this);
}

template <class T>
inline int SortedPtrList<T>::comparelt(ListNode *n1, ListNode *n2) const
{
    return(*((TListNode<T> *)n1)->data < *((TListNode<T> *)n2)->data);
}

template <class T>
class IntrList : private BasList
{
    public:
	IntrList(int do_manage = 1)	   { manage = do_manage; }
#ifdef NEED_CAST
	IntrList(const IntrList<T> &other) { BasList::copylist((BasList &)other);
					     manage = other.manage; }
	int operator==(const IntrList<T> &other) const { return(BasList::operator==((BasList &)other)); }
#else
	IntrList(const IntrList<T> &other) { BasList::copylist(other);
					     manage = other.manage; }
#endif
	~IntrList()			{ clear(manage); }
	IntrList<T> &operator=(const IntrList<T> &other);

	int compareeq(ListNode *, ListNode *) const;
	ListNode *copynode(ListNode *) const;

	T *operator[](int n)      { return((T *)BasList::operator[](n)); }
	const T *operator[](int n) const      { return((T *)BasList::operator[](n)); }

	void addtohead(T *n)		{ BasList::addtohead(n); }
	void addtotail(T *n)		{ BasList::addtotail(n); }

	T *gethead() const		{ return((T *)BasList::gethead()); }
	T *gettail() const		{ return((T *)BasList::gettail()); }
	T *getfound() const		{ return((T *)BasList::getfound()); }
	int getcount() const		{ return(BasList::getcount()); }
	int is_empty() const		{ return(BasList::is_empty()); }
	int getmanage() const		{ return(manage); }
	void setmanage(int m)		{ manage = m; }

	int find(T* n)			{ return(BasList::find(n)); }
	T* lookup(T *n)			{ return((T *)BasList::lookup(n)); }

	void remove_head(int del) 	{ BasList::remove_head(0, del); }
	void remove_tail(int del)	{ BasList::remove_tail(0, del); }
	void remove_found(int del)	{ BasList::remove_found(0, del); }
/*
 * IntrList is the only list class that has remove_node() as a member
 * funtion. This is because IntrList stores data objects that are derived
 * from ListNode (instead of the data being contained in a ListNode object).
 * Hence, the data is a direct part of IntrList (i.e. if we access the
 * data object we also access the list object).
 */
	void remove_node(T *n, int del) { BasList::remove_node(n, 0, del); }

	void clear(int del)		{ BasList::clear(0, del); }
    private:
	int manage;
    protected:
	void insert(T *n)		{ BasList::insert(n); }
};

template <class T>
IntrList<T> &IntrList<T>::operator=(const IntrList<T> &other)
{
    if (this == &other)
	return(*this);

    clear(manage);
    manage = other.manage;
    BasList::operator=(other);
    return(*this);
}

template <class T>
inline int IntrList<T>::compareeq(ListNode *n1, ListNode *n2) const
{
    return(*(T*)n1 == *(T*)n2);
}

template <class T>
inline ListNode *IntrList<T>::copynode(ListNode *n) const
{
    return(((T *)n)->clone());
}

template <class T>
class SortedIntrList : public IntrList<T>
{
    public:
	SortedIntrList(int do_manage = 1) : IntrList<T>(do_manage) { }
	SortedIntrList<T> &operator=(const SortedIntrList<T> &other);

	int comparelt(ListNode *, ListNode *) const;

	void insert(T *n)		{ IntrList<T>::insert(n); }
};

template <class T>
inline SortedIntrList<T> &SortedIntrList<T>::operator=(const SortedIntrList<T> &other)
{
    IntrList<T>::operator=(other);
    return(*this);
}

template <class T>
inline int SortedIntrList<T>::comparelt(ListNode *n1, ListNode *n2) const
{
    return(*(T*)n1 < *(T*)n2);
}


template <class T>
class ListIterator : private BasListIterator
{
    public:
        ListIterator() : BasListIterator() { }
#ifdef NEED_CAST
        ListIterator(List<T> &li) : BasListIterator((BasList &)li) { }
	void setlist(List<T> &li)	{ BasListIterator::setlist((BasList &)li); }
#else
        ListIterator(List<T> &li) : BasListIterator(li) { }
	void setlist(List<T> &li)	{ BasListIterator::setlist(li); }
#endif

	T &getfirst();
	T &getlast();
	T &getnext();
	T &getprev();
	T &getcurrent();
	int notempty() const		{ return(BasListIterator::notempty()); }

	void remove_current()		{ BasListIterator::remove_current(); }
    private:
	void error(const char *) const;
};

template <class T>
T &ListIterator<T>::getfirst()
{
    TListNode<T> *tmp = (TListNode<T> *)BasListIterator::getfirst();
    if (!tmp)
	error("Iter - getfirst() from empty list");
    return(tmp->data);
}

template <class T>
T &ListIterator<T>::getlast()
{
    TListNode<T> *tmp = (TListNode<T> *)BasListIterator::getlast();
    if (!tmp)
	error("Iter - getlast() from empty list");
    return(tmp->data);
}

template <class T>
T &ListIterator<T>::getnext()
{
    TListNode<T> *tmp = (TListNode<T> *)BasListIterator::getnext();
    if (!tmp)
	error("Iter - getnext() is empty");
    return(tmp->data);
}

template <class T>
T &ListIterator<T>::getprev()
{
    TListNode<T> *tmp = (TListNode<T> *)BasListIterator::getprev();
    if (!tmp)
	error("Iter - getprev() is empty");
    return(tmp->data);
}

template <class T>
T &ListIterator<T>::getcurrent()
{
    TListNode<T> *tmp = (TListNode<T> *)BasListIterator::getcurrent();
    if (!tmp)
	error("Iter - getitem(): no current item");
    return(tmp->data);
}

template <class T>
void ListIterator<T>::error(const char *str) const
{
    puts(str);
    exit(0);
}

template <class T>
class ROListIterator : private ROBasListIterator
{
    public:
        ROListIterator() : ROBasListIterator() { }
#ifdef NEED_CAST
        ROListIterator(const List<T> &li) : ROBasListIterator((BasList &)li) { }
	void setlist(const List<T> &li)	{ ROBasListIterator::setlist((BasList &)li); }
#else
        ROListIterator(const List<T> &li) : ROBasListIterator(li) { }
	void setlist(const List<T> &li)	{ ROBasListIterator::setlist(li); }
#endif

	const T &getfirst();
	const T &getlast();
	const T &getnext();
	const T &getprev();
	const T &getcurrent();
	int notempty() const	{ return(ROBasListIterator::notempty()); }
    private:
	void error(const char *) const;
};

template <class T>
const T &ROListIterator<T>::getfirst()
{
    TListNode<T> *tmp = (TListNode<T> *)ROBasListIterator::getfirst();
    if (!tmp)
	error("Iter - getfirst() from empty list");
    return(tmp->data);
}

template <class T>
const T &ROListIterator<T>::getlast()
{
    TListNode<T> *tmp = (TListNode<T> *)ROBasListIterator::getlast();
    if (!tmp)
	error("Iter - getlast() from empty list");
    return(tmp->data);
}

template <class T>
const T &ROListIterator<T>::getnext()
{
    TListNode<T> *tmp = (TListNode<T> *)ROBasListIterator::getnext();
    if (!tmp)
	error("Iter - getnext() is empty");
    return(tmp->data);
}

template <class T>
const T &ROListIterator<T>::getprev()
{
    TListNode<T> *tmp = (TListNode<T> *)ROBasListIterator::getprev();
    if (!tmp)
	error("Iter - getprev() is empty");
    return(tmp->data);
}

template <class T>
const T &ROListIterator<T>::getcurrent()
{
    TListNode<T> *tmp = (TListNode<T> *)ROBasListIterator::getcurrent();
    if (!tmp)
	error("Iter - getitem(): no current item");
    return(tmp->data);
}

template <class T>
void ROListIterator<T>::error(const char *str) const
{
    puts(str);
    exit(0);
}


template <class T>
class PtrListIterator : private BasListIterator
{
    public:
        PtrListIterator() : BasListIterator() { }
#ifdef NEED_CAST
        PtrListIterator(PtrList<T> &li) : BasListIterator((BasList &)li) { }
	void setlist(PtrList<T> &li)	{ BasListIterator::setlist((BasList &)li); }
#else
        PtrListIterator(PtrList<T> &li) : BasListIterator(li) { }
	void setlist(PtrList<T> &li)	{ BasListIterator::setlist(li); }
#endif

	T *getfirst();
	T *getlast();
	T *getnext();
	T *getprev();
	T *getcurrent();
	int notempty() const		{ return(BasListIterator::notempty()); }

	void remove_current(int clr)	{ BasListIterator::remove_current(clr); }
};

template <class T>
T *PtrListIterator<T>::getfirst()
{
    TListNode<T *> *n = (TListNode<T *> *)BasListIterator::getfirst();
    return(n ? n->data : 0);
}

template <class T>
T *PtrListIterator<T>::getlast()
{
    TListNode<T *> *n = (TListNode<T *> *)BasListIterator::getlast();
    return(n ? n->data : 0);
}

template <class T>
T *PtrListIterator<T>::getnext()
{
    TListNode<T *> *n = (TListNode<T *> *)BasListIterator::getnext();
    return(n ? n->data : 0);
}

template <class T>
T *PtrListIterator<T>::getprev()
{
    TListNode<T *> *n = (TListNode<T *> *)BasListIterator::getprev();
    return(n ? n->data : 0);
}

template <class T>
T *PtrListIterator<T>::getcurrent()
{
    TListNode<T *> *n = (TListNode<T *> *)BasListIterator::getcurrent();
    return(n ? n->data : 0);
}


template <class T>
class ROPtrListIterator : private ROBasListIterator
{
    public:
        ROPtrListIterator() : ROBasListIterator() { }
#ifdef NEED_CAST
        ROPtrListIterator(const PtrList<T> &li) : ROBasListIterator((BasList &)li) { }
	void setlist(const PtrList<T> &li)	{ ROBasListIterator::setlist((BasList &)li); }
#else
        ROPtrListIterator(const PtrList<T> &li) : ROBasListIterator(li) { }
	void setlist(const PtrList<T> &li)	{ ROBasListIterator::setlist(li); }
#endif

	const T *getfirst();
	const T *getlast();
	const T *getnext();
	const T *getprev();
	const T *getcurrent();
	int notempty() const		  { return(ROBasListIterator::notempty()); }
};

template <class T>
const T *ROPtrListIterator<T>::getfirst()
{
    TListNode<T *> *n = (TListNode<T *> *)ROBasListIterator::getfirst();
    return(n ? n->data : 0);
}

template <class T>
const T *ROPtrListIterator<T>::getlast()
{
    TListNode<T *> *n = (TListNode<T *> *)ROBasListIterator::getlast();
    return(n ? n->data : 0);
}

template <class T>
const T *ROPtrListIterator<T>::getnext()
{
    TListNode<T *> *n = (TListNode<T *> *)ROBasListIterator::getnext();
    return(n ? n->data : 0);
}

template <class T>
const T *ROPtrListIterator<T>::getprev()
{
    TListNode<T *> *n = (TListNode<T *> *)ROBasListIterator::getprev();
    return(n ? n->data : 0);
}

template <class T>
const T *ROPtrListIterator<T>::getcurrent()
{
    TListNode<T *> *n = (TListNode<T *> *)ROBasListIterator::getcurrent();
    return(n ? n->data : 0);
}


template <class T>
class IntrListIterator : private BasListIterator
{
    public:
        IntrListIterator() : BasListIterator() { }
#ifdef NEED_CAST
        IntrListIterator(IntrList<T> &li) : BasListIterator((BasList &)li) { }
	void setlist(IntrList<T> &li)	{ BasListIterator::setlist((BasList &)li); }
#else
        IntrListIterator(IntrList<T> &li) : BasListIterator(li) { }
	void setlist(IntrList<T> &li)	{ BasListIterator::setlist(li); }
#endif

	T *getfirst()		{ return((T*)BasListIterator::getfirst()); }
	T *getlast()		{ return((T*)BasListIterator::getlast()); }
	T *getnext()		{ return((T*)BasListIterator::getnext()); }
	T *getprev()		{ return((T*)BasListIterator::getprev()); }
	T *getcurrent()		{ return((T*)BasListIterator::getcurrent()); }
	int notempty() const	{ return(BasListIterator::notempty()); }

	void remove_current(int del)  { BasListIterator::remove_current(0, del); }
};

template <class T>
class ROIntrListIterator : private ROBasListIterator
{
    public:
        ROIntrListIterator() : ROBasListIterator() { }
#ifdef NEED_CAST
        ROIntrListIterator(const IntrList<T> &li) : ROBasListIterator((BasList &)li) { }
	void setlist(const IntrList<T> &li)	{ ROBasListIterator::setlist((BasList &)li); }
#else
        ROIntrListIterator(const IntrList<T> &li) : ROBasListIterator(li) { }
	void setlist(const IntrList<T> &li)	{ ROBasListIterator::setlist(li); }
#endif

	const T *getfirst()	{ return((T*)ROBasListIterator::getfirst()); }
	const T *getlast()	{ return((T*)ROBasListIterator::getlast()); }
	const T *getnext()	{ return((T*)ROBasListIterator::getnext()); }
	const T *getprev()	{ return((T*)ROBasListIterator::getprev()); }
	const T *getcurrent()	{ return((T*)ROBasListIterator::getcurrent()); }
	int notempty() const	{ return(ROBasListIterator::notempty()); }
};

#endif
