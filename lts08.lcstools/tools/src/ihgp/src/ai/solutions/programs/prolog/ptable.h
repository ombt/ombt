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

#include "xarray.h"
#include "pclause.h"

#define GROWSIZE 1000

/*
 * Class PrologTable represents the Prolog database in which objects
 * of class PrologFact and class PrologRule are stored. New facts and
 * rules can be created by passing strings that represent either rules
 * or facts. The parsing routines of this class will create new
 * objects of class PrologFact or PrologRule accordingly and add them to
 * the DB. Member function findmatch() scans the DB, starting at a given
 * clause, for a clause that matches the specified predicate. If a match
 * is found the resulting binding information and the new goals that were
 * introduced, if any, are returned. The position of the clause is
 * also returned, -1 meaning that a match could not be found.
 */
class PrologTable
{
    public:
	PrologTable();
	PrologTable(const char **);
	const PrologBase *operator[](int) const;
	PrologBase *&operator[](int);
	void addclause(const char *);
	void addclauses(const char **);
	void listclauses() const;
	int findmatch(int startclause, const Complex &findme,
	              PtrList<Complex> &newgoals, Substitution &subst); 
    private:
	PtrArray<PrologBase> table;
	int index;
};
