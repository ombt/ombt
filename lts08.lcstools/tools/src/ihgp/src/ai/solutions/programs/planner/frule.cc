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

#include "strips.h"

/*
 * Note that the prelist and the dellist of each F-rule are exactly
 * the same, meaning that either of them could be removed. But for
 * the sake of completeness both of them are kept.
 */
Frule frulearr[MAXFRULE] = {
		     Frule("stack(X, Y)",
			"clear(Y) & holding(X)",
			"clear(Y) & holding(X)",
			"arm(empty) & on(X, Y) & clear(X)"),

		     Frule("unstack(X, Y)",
			"on(X, Y) & clear(X) & arm(empty)",
			"on(X, Y) & clear(X) & arm(empty)",
                        "holding(X) & clear(Y)"),

		     Frule("pickup(X)",
			"ontable(X) & clear(X) & arm(empty)",
			"ontable(X) & clear(X) & arm(empty)",
			"holding(X)"),

		     Frule("putdown(X)",
			"holding(X)",
			"holding(X)",
			"ontable(X) & arm(empty) & clear(X)")
};
