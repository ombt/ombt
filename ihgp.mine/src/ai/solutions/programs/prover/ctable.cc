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

#include "ctable.h"

ClauseTable::ClauseTable()
{
    index = 0;
}

ClauseTable::ClauseTable(const char **arrstr)
{
    index = 0;
    addclauses(arrstr);
}

const Clause *ClauseTable::operator[](int i) const
{
    return(table[i]);
}

Clause *&ClauseTable::operator[](int i)
{
    return(table[i]);
}

Clause *&ClauseTable::getclause(int i)
{
    return(table[i]);
}

void ClauseTable::addclause(const char *str)
{
    Clause *add;

    add = new Clause(str);
    if (index == table.getsize())
	table.grow(GROWSIZE);
    table[index] = add;
    index++;
}

void ClauseTable::addclauses(const char **str)
{
    while (*str)
	addclause(*str++);
}

int ClauseTable::getsize() const
{
    return(index);
}
