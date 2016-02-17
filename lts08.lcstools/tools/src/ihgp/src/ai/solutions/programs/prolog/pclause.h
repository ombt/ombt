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

#ifndef _PCLAUSE_H_
#define _PCLAUSE_H_

#include <stdio.h>
#include <iostream.h>
#include "logic.h"
#include "xlist.h"

/*
 * PrologBase defines a common base class for objects of class PrologFact
 * and PrologRule so that they can be stored in the same container class.
 * Also, a pure virtual function match() is defined which makes matching
 * of a Complex object and an object of one of these classes transparent.
 * If the match succeeds the substitution that was needed to unify both
 * predicates is returned together with a set of clauses, if any, that
 * represent new goals that must be satisfied.
 */
class PrologBase
{
    public:
	virtual ~PrologBase() { };
	int operator==(const PrologBase &) const;
	virtual PrologBase *clone() const = 0;
	virtual void display() const = 0;
	virtual int match(const Complex &matchme, PtrList<Complex> &newgoals,
			  Substitution &subst) = 0;
};

/*
 * Class PrologFact represents facts that may be stored in the Prolog DB.
 * It is little more than a specialization of class Complex.
 */
class PrologFact : public PrologBase, public Complex
{
    public:
	PrologFact(const char *);
	PrologFact(const PrologFact &);
	PrologBase *clone() const;
	void display() const;
	int match(const Complex &, PtrList<Complex> &, Substitution &);
};

ostream &operator<<(ostream &, const PrologFact &);

/*
 * Class PrologRule represents rules that may be stored in the Prolog DB.
 * Rules consist of a head, an object of type Complex, and a body,
 * a list of Complex objects.
 */
class PrologRule : public PrologBase
{
    friend ostream &operator<<(ostream &, const PrologRule &);
    public:
	PrologRule(const char *);
    	PrologRule(const PrologRule &);
	PrologBase *clone() const;
	void display() const;
	int match(const Complex &, PtrList<Complex> &, Substitution &);
    private:
	Complex head;
	PtrList<Complex> body;
};

#endif
