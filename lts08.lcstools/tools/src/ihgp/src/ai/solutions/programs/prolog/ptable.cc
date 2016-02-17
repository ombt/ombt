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

#include "ptable.h"

PrologTable::PrologTable()
{
    index = 0;
}

PrologTable::PrologTable(const char **arrstr)
{
    index = 0;
    addclauses(arrstr);
}

const PrologBase *PrologTable::operator[](int i) const
{
    return(table[i]);
}

PrologBase *&PrologTable::operator[](int i)
{
    return(table[i]);
}

/*
 * addclause() adds one clause to the Prolog DB. The routine decides
 * itself whether an object of type PrologRule or PrologFact should
 * be created, based on the information supplied in the string.
 */
void PrologTable::addclause(const char *str)
{
    PrologBase *add;

    if (strstr(str, ":-"))
	add = new PrologRule(str);
    else
	add = new PrologFact(str);
    if (index == table.getsize())
	table.grow(GROWSIZE);
    table[index] = add;
    index++;
}

/*
 * addclauses() processes an array of strings and adds new clauses
 * to the Prolog DB.
 */
void PrologTable::addclauses(const char **str)
{
    while (*str)
	addclause(*str++);
}

void PrologTable::listclauses() const
{
    for (int i = 0; i < index; i++)
    {
 	table[i]->display();
	cout << '\n';
    }
}

/*
 * findmatch() scans the Prolog DB for a clause that matches the
 * specified Complex object, starting at the specified entry.
 * If a match is found the position of this clause in the DB, the
 * substitution needed by the match and any new goals that were
 * introduced are returned. -1 means a match could not be found.
 */
int PrologTable::findmatch(int startclause, const Complex &findme,
PtrList<Complex> &newgoals, Substitution &subst)
{
    for (int i = startclause; i < index; i++)
	if (table[i]->match(findme, newgoals, subst))
	    return(i);
    return(-1);
}
