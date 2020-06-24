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

#include <stdarg.h>
#include "clause.h"

Clause::Clause()
{
}

Clause::Clause(const char *string)
{
    parse_string(string);
}

int Clause::operator==(const Clause &other) const
{
    return(terms == other.terms);
}

Clause *Clause::clone() const
{
    return(new Clause(*this));
}

Clause &Clause::operator=(const Clause &other)
{
    if (this != &other)
	terms = other.terms;
    return(*this);
}

Clause &Clause::operator=(const char *string)
{
    terms.clear(DoDel);
    parse_string(string);
    return(*this);
}

void Clause::display() const
{
    ROIntrListIterator<Literal> iter(terms);
    const Literal *lit;

    cout << '[';
    if ((lit = iter.getfirst()))
        lit->display();
    while ((lit = iter.getnext()))
    {
        cout << ", ";
        lit->display();
    }
    cout << ']';
}

/*
 * is_tautology() determines if the clause is a tautology.
 */
int Clause::is_tautology()
{
    IntrListIterator<Literal> iter(terms);
    Literal *plit;

    /*
     * To check if a clause is a tautology it must be checked
     * if the clause contains a pair of literals that are complementary
     * to each other (one positive, the other negative). Framed literals
     * are not taken into account.
     */
    for (plit = iter.getfirst(); plit; plit = iter.getnext())
    {
	/* Skip framed literals. */
	if (!plit->is_framed())
	{
	    /*
	     * Get the next literal of the clause and determine if its
	     * negation is also part of the clause. If so the clause
	     * is a tautology.
	     */
            Literal lit = *plit;
            lit.negate();
            if (terms.lookup(&lit))
                return(1);
	}
    }
    return(0);
}

int Clause::is_empty() const
{
    return(terms.is_empty());
}

/*
 * merge_except() merges the current clause with another clause,
 * except for the specified literal of the other clause (this
 * will usually be one of the literals resolved upon, specifically
 * the literal in the other clause that is the negation of the last
 * literal of the current clause). This is called merging left by
 * Chang and Lee. Unlike Chang and Lee's algorithm merging left is
 * also done for framed literals.
 */
void Clause::merge_except(int pos, Clause &other)
{
    IntrListIterator<Literal> iter(other.terms);
    Literal *lit;
    int i;

    for (i = 0, lit = iter.getfirst(); lit; lit = iter.getnext())
    {
        if (pos != i++)
	    /*
	     * Add (a copy of) the literal to the current clause, taking
	     * care not to produce any duplicates.
	     */
	    if (!terms.lookup(lit))
		terms.addtotail(lit->clone());
    }
}

/*
 * frame_last() makes the last literal of the clause framed. Note that
 * the literal is not marked framed if it already occurs as framed in
 * the clause. In this case the literal is simply removed. This is to
 * prevent the occurence of duplicate framed literals in the clause.
 */
void Clause::frame_last()
{
    Literal lit = *terms.gettail();
    lit.make_framed();

    if (!terms.lookup(&lit))
        terms.gettail()->make_framed();
    else
        terms.remove_tail(DoDel);
}

/*
 * delete_framed() removes every framed literal from the clause that
 * is not followed by any unframed literal. By definition these literals
 * must be the last literals of the clause.
 */
void Clause::delete_framed()
{
    Literal *lit;
    while ((lit = terms.gettail()) && lit->is_framed())
        terms.remove_tail(DoDel);
}

/*
 * reduce_order() implements the reduce-order operation.
 * Note that we keep reducing the clause as long as possible: if
 * the result of a reduced order clause is reduceable itself, it
 * is reduced again. It is not really clear from Chang and Lee's
 * description if this should be done, but this seems like a reasonable
 * thing to do and does not cause any problems as far as we know.
 */
void Clause::reduce_order()
{
    for (;;)
    {
	if (terms.is_empty())
	    break;

	/*
	 * Check if the negation of the last literal of the clause
	 * appears as a framed literal in the clause. If so reduce
	 * the clause (turn it into a reduced ordered clause) by
	 * removing the last literal. Next, remove every framed literal
	 * not followed by any unframed literal.
	 */
        Literal lit = *terms.gettail(); 
        lit.negate();
        lit.make_framed();

        if (terms.lookup(&lit))
            terms.remove_tail(DoDel);
        else
            break;

	/* Delete framed literals not followed by any unframed literal. */
        delete_framed();
    }
}


/*
 * resolve() resolves the two clauses according to the OL-resolution
 * scheme. If this procedure succeeds the resulting resolvent is
 * returned, otherwise NULL is returned.
 */
Clause* Clause::resolve(Clause &other)
{
    Clause *result = NULL;
    int pos;

    /*
     * Create the negation of the last literal of the clause:
     * get a copy of this literal and negate it.
     */
    Literal neglast = *terms.gettail();
    neglast.negate();

    /*
     * Determine if the negated literal appears in the other clause.
     * If so the clauses can be resolved.
     */
    if ((pos = other.terms.find(&neglast)) >= 0)
    {
	/*
	 * Create the resolvent:
         * Get a copy of the current clause. Frame the last literal. Merge
	 * with the other clause, exempting the negated literal found in
	 * the other clause. Delete every framed literal not followed by
	 * any unframed literal. Apply the reduce order operation.
	 */
        result = clone();
        result->frame_last();
        result->merge_except(pos, other);
        result->delete_framed();
        result->reduce_order();
    }
    return(result);
}

/*
 * parse_string() parses the supplied string. This routine is used to
 * create Clause objects. No error checking is performed.
 */
void Clause::parse_string(const char *string) 
{
    char *p, *end, *buf;
 
    if (!(buf = strdup(string)))
    {
	puts("Clause::Clause() out of memory");
	exit(0);
    }
    p = buf + 1;
    while (*p)
    {
	int neg;
	Literal *lit;

	neg = 0;
        while (*p == ' ')
	    p++;
        if (*p == '~')
        {
	    neg = 1;
	    p++;
        }
        if (end = strchr(p, ','))
	{
	    *end = '\0';
	    end++;
	}
	else
	{
	    end = strchr(p, ']');
	    *end = '\0';
	}
        lit = new Literal(neg, p);
	if (!terms.lookup(lit))
	    terms.addtotail(lit);
	else
	    delete lit;
	p = end;
    }
    delete(buf);
}
