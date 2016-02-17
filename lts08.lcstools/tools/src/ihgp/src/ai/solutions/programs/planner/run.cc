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
#include "strips.h"

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

    StripsObject *s1 = new StripsObject(
	"clear(a) & clear(b) & clear(c) & ontable(a) & ontable(b) & ontable(c) & arm(empty)",
	"on(a, b) & on(c, a)");

    StripsObject *s2 = new StripsObject(
	"clear(b) & clear(c) & ontable(a) & ontable(b) & on(c, a) & arm(empty)",
	"on(a, b) & on(b, c)");

    StripsObject *s3 = new StripsObject(
	"clear(a) & clear(b) & clear(c) & ontable(a) & ontable(b) & ontable(c) & arm(empty)",
	"holding(a) & on(b, c)");

    StripsObject *s4 = new StripsObject(
	"clear(c) & clear(b) & ontable(a) & ontable(b) & arm(empty) & on(c, a)",
	"ontable(c) & on(b, c) & on(a, b)");

     StripsObject *s5 = new StripsObject(
	"clear(c) & ontable(a) & on(b, a) & on(c, b) & arm(empty)",
	"on(a, b) & on(b, c)");

    StripsObject *s6 = new StripsObject(
	"on(b, a) & ontable(a) & ontable(c) & ontable(d) & clear(b) & clear(c) & clear(d) & arm(empty)",
	"on(b, c) & on(c, a) & ontable(a) & ontable(d)");

    Strips planner;

    cout << "** Plan for problem 1: **\n";
    planner.set_startnode(s1);
    if (planner.generate())
	planner.display();

    cout << "\nPress enter to coninue.\n";
    cin.get();

    cout << "** Plan for problem 2: **\n";
    planner.set_startnode(s2);
    if (planner.generate())
	planner.display();

    cout << "\nPress enter to coninue.\n";
    cin.get();

    cout << "** Plan for problem 3: **\n";
    planner.set_startnode(s3);
    if (planner.generate())
	planner.display();

    cout << "\nPress enter to coninue.\n";
    cin.get();

    cout << "** Plan for problem 4: **\n";
    planner.set_startnode(s4);
    if (planner.generate())
	planner.display();

    cout << "\nPress enter to coninue.\n";
    cin.get();

    cout << "** Plan for problem 5: **\n";
    planner.set_startnode(s5);
    if (planner.generate())
	planner.display();

    cout << "\nPress enter to coninue.\n";
    cin.get();

    cout << "** Plan for problem 6: **\n";
    planner.set_startnode(s6);
    if (planner.generate())
	planner.display();

    return(1);
}

