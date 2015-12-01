// deletion strategy functions

// headers
#include "deletion.h"

// check if two literals or two terms are identical.
int
equal(const Terms &t1, const Terms &t2)
{
	// check if the same term
	if ((t1.type != t2.type) || (t1.value != t2.value) || 
	    (t1.argnum != t2.argnum))
		return(0);

	// if not a function, we have a match already.
	if (t1.type != Terms::Function)
	{
		return(1);
	}

	// check if the parameters are the same
	ListIterator<Terms * > t1paIter(*t1.pargs);
	ListIterator<Terms * > t2paIter(*t2.pargs);
	for ( ; !t1paIter.done() && !t2paIter.done(); t1paIter++, t2paIter++)
	{
		if (!equal(*t1paIter(), *t2paIter()))
			return(0);
	}

	// the terms are the same
	return(1);
}

int
equal(const Literal &l1, const Literal &l2)
{
	// check if the same literal
	if ((l1.negated != l2.negated) || (l1.type != l2.type) ||
	    (l1.value != l2.value) || (l1.argnum != l2.argnum))
		return(0);

	// if not a function, we have a match already.
	if (l1.type != Literal::Function && l1.type != Literal::Equal)
		return(1);

	// check if the parameters are the same
	ListIterator<Terms * > l1paIter(*l1.pargs);
	ListIterator<Terms * > l2paIter(*l2.pargs);
	for ( ; !l1paIter.done() && !l2paIter.done(); l1paIter++, l2paIter++)
	{
		if (!equal(*l1paIter(), *l2paIter()))
			return(0);
	}

	// the literals are the same
	return(1);
}

// check if a clause is a tautology
int
tautology(const Clause &c)
{
	// copy clause
	Clause clause(c);

	// check if the clause is positive, negative or mixed
	if (clause.getType() != Clause::Mixed)
	{
		// can not be a tautology
		return(NOMATCH);
	}

	// we have a mixed clause; it contains atoms and
	// negated atoms. check if A and ~A both occur
	// in the clause, then we have a tautology.
	//
	// check for tautologies. these comparisons are done
	// without the use of an index. this means that
	// we will use the comparison functions eq, ne, lt, le,
	// gt, ge, instead of the operator versions, ==, !=, etc.
	//
	PositiveClauseIterator posIter(c);
	NegativeClauseIterator negIter(c);
	while (!posIter.done() && !negIter.done())
	{
		if (posIter().lt(~negIter()))
		{
			posIter++;
		}
		else if (posIter().gt(~negIter()))
		{
			negIter++;
		}
		else
		{
			// get positive literals in equivalence class
			Literal plit = posIter();
			List<Literal> plist;
			MustBeTrue(plist.insertAtEnd(posIter()) == OK);
			for (posIter++; !posIter.done() && plit.eq(posIter()); 
			     posIter++)
			{
				MustBeTrue(plist.insertAtEnd(posIter()) == OK);
			}

			// get negative literals in equivalence class
			Literal nlit = negIter();
			List<Literal> nlist;
			MustBeTrue(nlist.insertAtEnd(negIter()) == OK);
			for (negIter++; !negIter.done() && nlit.eq(negIter()); 
			     negIter++)
			{
				MustBeTrue(nlist.insertAtEnd(negIter()) == OK);
			}

			// we have a class and a counterpart class
			// that is negated. check each literal.
			//
			ListIterator<Literal> plIter(plist);
			for ( ; !plIter.done(); plIter++)
			{
				// check each literal against the other.
				ListIterator<Literal> nlIter(nlist);
				for ( ; !nlIter.done(); nlIter++)
				{
					if (equal(plIter(), ~nlIter()))
					{
						// we have a tautology
						if (verbose)
						{
							cout << endl;
							cout << "clause is a tautology ..." << endl;
							cout << "clause: " << c << endl;
						}
						return(OK);
					}
				}
			}
		}
	}

	// not a tautology
	return(NOMATCH);
}

// check if a clause is a subset of another. only compares 
// equivalent sets.
//
int
subset(const Clause &cl1, const Clause &cl2)
{
	// verify smaller clause subsumes larger clause
	if (cl1.getTotalMembers() > cl2.getTotalMembers())
	{
		return(NOMATCH);
	}

	// certain types of clauses cannot subsume others,
	// p = positive, n = negative, m = mixed.
	// 
	//	clause 1	clause 2	can 
	//	type		type		subsume?
	//	-------------------------------------------
	//	P		P		yes
	//	P		N		no
	//	P		M		yes
	//	N		P		no
	//	N		N		yes
	//	N		M		yes
	//	M		P		yes
	//	M		N		yes
	//	M		M		yes
	//
	if (((cl1.getType() == Clause::Positive) &&
	    (cl2.getType() == Clause::Negative)) ||
	    ((cl1.getType() == Clause::Negative) &&
	    (cl2.getType() == Clause::Positive)))
	{
		return(NOMATCH);
	}

	// start checking every literal equivalent class. every equivalent 
	// class in clause 1 should be in clause2. absolute equality
	// is checked with unification.
	//
	ClauseIterator cl1Iter(cl1);
	ClauseIterator cl2Iter(cl2);
	for ( ; !cl1Iter.done(); cl1Iter++)
	{
		// search for literal in clause 2
		int found = 0;
		while (!cl2Iter.done() && !found)
		{
			if (cl2Iter().lt(cl1Iter()))
			{
				cl2Iter++;
			}
			else if (cl2Iter().gt(cl1Iter()))
			{
				return(NOMATCH);
			}
			else
			{
				found = 1;
			}
		}
		if (!found)
		{
			return(NOMATCH);
		}
	}

	// all done
	return(OK);
}

// check if a clause subsumes another clause. subsumed clause
// must have MORE elements than the subsuming clause.
//
int
ST(Array<Literal> &C, Array<Literal> &D, int i, int j, Substitutions &theta)
{
	// check if clause D literals are exhausted
	if (j > D.getSize())
	{
		return(NOMATCH);
	}

	// search for a possible unifying literal
	int a = j;
	Literal Li(C[i]);
	if (theta.applyTo(Li) != OK)
	{
		ERROR("applyTo failed.", errno);
		return(NOTOK);
	}
	Substitutions mu;
	for ( ; a <= D.getSize(); a++)
	{
		mu.clear();
		Literal Ka(D[a]);
		if (unify(Li, Ka, mu) == OK)
			break;
	}
	if (a > D.getSize())
	{
		return(NOMATCH);
	}

	// attempt to extend the matching
	Substitutions theta_mu(theta*mu);
	// if (i == C.getSize() || ST(C, D, i+1, 1, theta*mu) == OK)
	if (i == C.getSize() || ST(C, D, i+1, 1, theta_mu) == OK)
	{
		return(OK);
	}
	else
		return(ST(C, D, i, a+1, theta));
}

int
subsumes(const Clause &cl1, const Clause &cl2, Substitutions &s)
{
	// clear substitution list
	s.clear();

	// make sure the clauses are not the same one.
	// this check works since the index in literals
	// are never reassigned.
	//
	if (cl1 == cl2)
	{
		return(NOMATCH);
	}

	// check that every class in clause 1 exists in clause 2. 
	if (subset(cl1, cl2) != OK)
	{
		return(NOMATCH);
	}

	// now start the actual subsumption algorithm.
	Substitutions subs;
	Clause clause2(cl2);
	if (groundSubstitutions(clause2, subs) != OK)
	{
		ERROR("groundSubstitutions failed.", errno);
		return(NOTOK);
	}

	// convert second clause to a ground clause
	if (subs.applyTo(clause2) != OK)
	{
		ERROR("applyTo failed.", errno);
		return(NOTOK);
	}

	// copy clauses to arrays
	Array<Literal> clarray1(1, cl1.getTotalMembers());
	ClauseIterator cl1Iter(cl1);
	for (int i=1; !cl1Iter.done(); i++, cl1Iter++)
	{
		clarray1[i] = cl1Iter();
	}
	Array<Literal> clarray2(1, cl2.getTotalMembers());
	ClauseIterator cl2Iter(clause2);
#ifdef SC42
	for (i=1; !cl2Iter.done(); i++, cl2Iter++)
#else
	for (int i=1; !cl2Iter.done(); i++, cl2Iter++)
#endif
	{
		clarray2[i] = cl2Iter();
	}

	// use Stillman's algorithm
	statistics[AttemptedStillmanSubsumptionTests] += 1;
	totalstatistics[TotalAttemptedStillmanSubsumptionTests] += 1;
	int status = ST(clarray1, clarray2, 1, 1, s);
	if (status == OK && verbose)
	{
		cout << endl;
		cout << "clause1 subsumes clause2 ..." << endl;
		cout << "clause1: " << cl1 << endl;
		cout << "clause2: " << cl2 << endl;
	}
	return(status);
}

// generate a ground substitution for a clause
int
groundSubstitutions(const Clause &clause, Substitutions &subs)
{
	// clear substitutions
	subs.clear();

	// get a string representation
	String clstring = (String)clause;

	// list of variables
	Map<String, String> variables;

	// find variables in clause
	StringTokens clst(clstring, " \t");
	for ( ; !clst.done(); clst++)
	{
		if ((clst()(0,2) == String("_V")) ||
		    (clst()(0,3) == String("_RV")))
		{
			String variable(clst());
			if (!variables.isInMap(variable))
			{
				variables[variable] = 
					uniqueName("_CONST");
			}
		}
	}

	// generate substitutions now
	MapIterator<String, String> varsIter(variables);
	for ( ; !varsIter.done(); varsIter++)
	{
		Substitution sub(Terms(varsIter.data()), varsIter.key());
		subs = subs*sub;
	}

	// all done
	return(OK);
}

// factor the literals in a clause. the current implementation
// attempts to unify only two terms. 
//
int
factor(Literal &fl, Clause &cl, Substitutions &flsubs)
{
	// clear substitutions
	flsubs.clear();

	// scan clause for possible literals to unify
	List<Literal> flset;
	ClauseIterator clIter(cl);
	for ( ; !clIter.done() && clIter().lt(fl); clIter++)
	{
		// do nothing
	}
	for ( ; !clIter.done() && clIter().eq(fl); clIter++)
	{
		MustBeTrue(flset.insertAtEnd(clIter()) == OK);
	}

	// check if there is more than one literal in the set
	if (flset.getCount() < 2)
	{
		return(NOMATCH);
	}

	// try to find a factor
	int factorfound = 0;
	Substitutions subs;
	ListIterator<Literal> flsetIter(flset);
	for ( ; !flsetIter.done() && !factorfound; flsetIter++)
	{
		// we found the key literal
		if (fl == flsetIter())
		{
			// skip key literal
			continue;
		}

		// try to unify
		Literal tmp_fl(fl);
		Literal tmp_flit(flsetIter());
		subs.clear();
		if (unify(tmp_fl, tmp_flit, subs) == OK)
		{
			// literals unified
			factorfound++;
			break;
		}
	}
	if (!factorfound)
	{
		return(NOMATCH);
	}

	// key literal should be in the clause
	if (cl.retrieve(fl) != OK)
	{
		// key literal was NOT found. not possible.
		ERROR("key literal was not found.", errno);
		return(NOTOK);
	}

	// remove factor literal
	if (verbose)
	{
		cout << endl;
		cout << "clause before factoring ... " << cl << endl;
	}
	Literal fl2(flsetIter());
	if (cl.remove(fl2) != OK)
	{
		// factor literal was NOT found.
		ERROR("unable to remove factor key literal.", errno);
		return(NOTOK);
	}

	// apply substitutions to entire clause
	if (subs.applyTo(cl) != OK)
	{
		ERROR("applyTo failed.", errno);
		return(NOTOK);
	}
	if (verbose)
	{
		cout << "factored literal ... " << fl2 << endl;
		cout << "clause after factoring ... " << cl << endl;
	}

	// return substitutions
	flsubs = subs;

	// all done
	return(OK);
}

