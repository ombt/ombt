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
#include "prolog.h"

const char *clauses[] =
{
	"has(robbie, darkspots).",
	"has(robbie, tawnycolor).",
	"eats(robbie, meat).",
	"has(robbie, hair).",
	"has(suzie, feathers).",
	"flieswell(suzie).",
	"isa(X, mammal):- \
			has(X, hair).",
	"isa(X, bird):- \
			has(X, feathers).",
	"isa(X, carnivore):- \
			eats(X, meat).",
	"isa(X, cheetah):- \
			isa(X, mammal) & \
			isa(X, carnivore) & \
			has(X, tawnycolor) & \
			has(X, darkspots).",
	"isa(X, albatross):- \
			isa(X, bird) & \
			flieswell(X).",
	NULL
};

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

int main()
{
    /* Install new handler to catch out of memory errors. */
#ifdef _MSC_VER
    _set_new_handler(no_mem);
#else
    set_new_handler(no_mem);
#endif
 
    Substitution subst;

    /* Create Prolog database. */
    PrologTable table;

    /* Create search object, passing it the Prolog DB. */
    Prolog prolog(table);

    /* Fill the Prolog DB. */
    table.addclauses(clauses);

    /* Enter a query and start the search. */
    prolog.set_query("isa(X, Y)");
    prolog.get_result(subst);
    cout << subst << "\n";

    /* Important: clear the substitution before restarting the search. */
    subst.clear();
    while (prolog.get_next_result(subst))
    {
	cout << subst << "\n";
	subst.clear();
    }
    return(1);
}
