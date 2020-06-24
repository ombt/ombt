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

#ifndef _Literal_H_
#define _Literal_H_

#include <stdio.h>
#include "xstring.h"
#include "xlist.h"

/*
 * Class Literal represents literals that are part of a propositional
 * statement or clause. A Literal object consists of a string,
 * a flag indicating whether the literal is negated or not and a flag
 * indicating whether the literal is framed or not.
 * Literals must be expressions of the form "q", "~raining",
 *"|p|", "|~q|", where '| |' means that the literal is framed and
 * '~' means that the literal is negated.
 */
class Literal : public ListNode
{
    public:
        Literal(int, const char *);
        operator==(const Literal &) const;
	Literal *clone() const;
        void display() const;
        void negate();
        void make_framed();
        int is_framed() const;
    private:
        int negated,
            framed;
	String prop;
};

#endif
