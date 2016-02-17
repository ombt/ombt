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

#ifndef _Clause_H_
#define _Clause_H_

#include <stdio.h>
#include "literal.h"
#include "xlist.h"

/*
 * Class clause represents clauses as used in resolution. A Clause
 * object is created by calling Clause("[p, q]"). Member function resolve()
 * can be called to resolve a clause with another clause. If successfull
 * this function returns the resolvent of these two clauses. The reduce
 * order operation as defined by Chang and Lee for OL-Resolution is
 * implemented by private member function reduce_order().
 */
class Clause
{
    public:
        Clause();
        Clause(const char *);
        Clause *clone() const;
	Clause &operator=(const Clause &other);
	Clause &operator=(const char *);
	int operator==(const Clause &) const;
        Clause *resolve(Clause &);
        int is_empty() const;
        void display() const;
	int is_tautology();
    private:
        void merge_except(int, Clause &);
        void frame_last();
        void delete_framed();
        void reduce_order();
	void parse_string(const char *);

        IntrList<Literal> terms;
};

#endif
