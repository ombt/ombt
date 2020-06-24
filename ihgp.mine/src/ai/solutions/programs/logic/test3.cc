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

#include "logic.h"

int main()
{
    Substitution s, s2, s3;
    Complex c("f(X, g(X, f(g(X2, X))), X2, X)"),
	    c2("f(X, X2, X4)"),
	    c3("f(X, X2, a)"),
	    a("f(X)"),
            b("f(a)");

    cout << c << "\n";
    c.update_vars(s);
    cout << c << "  " << s << "\n";
    cout << c2 << "\n";
    c2.unify(c3, s);
    c2.update_vars(s);
    cout << c2 << "\n";

    a.unify(b, s2);
    s3 = s2;
    cout << s3 << "\n" << s2.compose(s3) << "  " << s2 << "\n";

    return(1);
}
