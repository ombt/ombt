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

#include "literal.h"

/*
 * This routine creates a literal object. By default a new literal
 * is not framed.
 */
Literal::Literal(int negval, const char *txt)
{
    negated = negval;
    prop = txt;
    framed = 0;
}

int Literal::operator==(const Literal &other) const
{
    if (negated != other.negated)
        return(0);
    if (framed != other.framed)
        return(0);
    return(prop == other.prop);
}

Literal *Literal::clone() const
{
    return(new Literal(*this));
}

void Literal::display() const
{
    cout << (framed ? "|" : "") << (negated ? "~" : "") << prop << (framed ? "|" : "");
}

void Literal::negate()
{
    negated = (negated == 0) ? 1 : 0;
}

void Literal::make_framed()
{
    framed = 1;
}

int Literal::is_framed() const
{
    return(framed);
}
