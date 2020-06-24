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
    Complex a,
	    b("f(g(Y), f(g(a), Z))"),
	    c("f(X11, g(b))"),
            d("f(a, Y1234)"),
	    e("g(a)"),
	    f(b);	// copy constructor

    Substitution s, s2, s3;

    cout << a << "  " << f << "\n";
    a = "f(X, f(X, Y))";

    cout << a << "\n" << b << "\n" << a.unify(b, s) << "\n";
    cout << s << "\n";

    a.apply_subst(s);
    cout << a << "\n";
    cout << (a == b) << " " << (a == a) << "\n";

    c.unify(d, s2);
    s3 = s2;
    cout << s2 << " " << (s == s) << " " << " " << (s == s2) << " " << s3 << "\n";

    cout << e << "\n";
    e = d;
    cout << e << " " << d << "\n";

    return(1);
}
