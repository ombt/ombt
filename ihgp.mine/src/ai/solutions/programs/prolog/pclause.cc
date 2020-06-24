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

#include "pclause.h"

/*
 * operator== is here just to keep PtrArray<PrologBase>::find() happy.
 * It is not actually used anywhere (if it were it should be made pure
 * virtual and the real implementation of this function should be in
 * PrologFact and PrologRule).
 */
int PrologBase::operator==(const PrologBase &) const
{
    return(0);
}

PrologFact::PrologFact(const PrologFact &other)
	: Complex(other)
{
}

PrologFact::PrologFact(const char *str)
    : Complex(str)
{
}

/*
 * match() determines if the Prolog fact matches the specified Complex
 * by unifying the two of them. If they do, the resulting substitution is
 * stored in subst. Before the actual match is done a variant of the Prolog
 * fact is created to ensure "fresh" variables will be used.
 */
int PrologFact::match(const Complex &matchme, PtrList<Complex> &outgoals,
Substitution &subst)
{
    Complex variant = *this;
    variant.update_vars();

    if (matchme.unify(variant, subst))
	return(1);
    return(0);
}

PrologBase *PrologFact::clone() const
{
    return(new PrologFact(*this));
}

void PrologFact::display() const
{
    cout << *(Complex *)this << ".";
}

ostream &operator<<(ostream &stream, const PrologFact &f)
{
    return(stream << f << ".");
}

PrologRule::PrologRule(const PrologRule &other)
{
    head = other.head;
    body = other.body;
}

PrologBase *PrologRule::clone() const
{
    return(new PrologRule(*this));
}

/*
 * PrologRule's must be of the form:
 * 
 * a(X, Y) :-
 *	b(X, Y) &
 *	c(X, Y).
 *
 * Note the occurenceof '&' instead of ','. This is because a delimiter
 * is needed other than '&'. Also, don't forget the '.' at the end.
 */
PrologRule::PrologRule(const char *str)
{
    char buf[256];
    char const *p = str;

    p = strstr(str, ":-");
    strncpy(buf, str, p - str);
    *(buf + (p - str)) = '\0';

    head = buf;

    ++p;
    while (isspace(*++p))
	;
    str = p;

    while ((p = strchr(str, '&')) || (p = strchr(str, '.')))
    {
	Complex *cplex;

	strncpy(buf, str, p - str);
	*(buf + (p - str)) = '\0';
        cplex = new Complex(buf);
	body.addtotail(cplex);
	if (*p != '.')
	    while (isspace(*++p))
		;
	else
	    break;
	str = p;
    }
}

/*
 * match() determines if the Prolog rule matches the specified Complex
 * object by unifying the head of the rule with the Complex. If the unification
 * succeeds, the resulting substitution is stored in 'subst' and the
 * goals in the body of the rule are updated with the substitution and
 * copied to 'outgoals'. Before unification takes place a variant of the
 * rule is created to ensure "fresh" variables will be used.
 */
int PrologRule::match(const Complex &matchme, PtrList<Complex> &outgoals,
Substitution &subst)
{
    Substitution s;

    /*
     * Create variant of the head only, the terms in the body will
     * be updated only if the unification succeeds.
     */
    Complex variant = head;
    variant.update_vars(s);

    if (matchme.unify(variant, subst))
    {
        Complex *c;
	PtrListIterator<Complex> iter(outgoals);

	/*
	 * Copy the terms in the body to outgoals and update them with
	 * the substitution that was used in creating the variant and in
         * the unification.
	 */
	outgoals = body;
	for (c = iter.getfirst(); c; c = iter.getnext())
	{
	    c->update_vars(s);
	    c->apply_subst(subst);
	}
        return(1);
    }
    return(0);
}


void PrologRule::display() const
{
    cout << *this;
}

ostream &operator<<(ostream &stream, const PrologRule &rule)
{
    ROPtrListIterator<Complex> iter(rule.body);
    const Complex *c;

    stream << rule.head << ":-\n";

    stream << "\t" << *iter.getfirst();
    while ((c = iter.getnext()))
	cout << ",\n\t" << *c;

    cout << ".";

    return(stream);
}
