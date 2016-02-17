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
    Complex a("f(X)"), a2("f(g(Y))"),
	    a3("f(Y)"), a4("f(f(X))"), a5("f(f(a))");
    Substitution s, s2, s3;

    a.unify(a2, s);
    cout << s << "\n";

    a3.unify(a4, s2);
    cout << s2 << "\n" << s.compose(s2) << " " << s << "\n";

    a3.unify(a5, s3);
    cout << s3 << "\n" << s.compose(s3) << "\n" << s << "\n";

    a.apply_subst(s);
    cout << a << "\n";

    return(1);
}
    
