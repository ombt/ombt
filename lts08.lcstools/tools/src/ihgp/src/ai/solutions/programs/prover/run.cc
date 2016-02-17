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

#include <stdio.h>
#include <new.h>
#include "resnode.h"

#ifdef _MSC_VER
int no_mem(size_t size)
{
    fprintf(stderr, "Out of memory\n");
    exit(1);
    return(0);
}
#else
void no_mem()
{
    fprintf(stderr, "Out of memory\n");
    exit(1);
}
#endif

const char
    *clauses[] = {
                   "[p, q, r]",
                   "[p, q, ~r]",
                   "[p, ~q, r]",
                   "[p, ~q, ~r]",
                   "[~p, q, r]",
                   "[~p, q, ~r]",
                   "[~p, ~q, r]",
                   "[~p, ~q, ~r]",
                   NULL
                 };

int main()
{
    /* Install new handler to catch out of memory errors. */
#ifdef _MSC_VER
    _set_new_handler(no_mem);
#else
    set_new_handler(no_mem);
#endif
 
    ClauseTable
	table;
    table.addclauses(clauses);
    Clause
	topclause = clauses[0];
    ResGraph
	prover(new ResNode(topclause.clone(), NULL), table);

    if (prover.generate())
	prover.display();
    else
	puts("no solution found");
    putchar('\n');

    return(1);
}
