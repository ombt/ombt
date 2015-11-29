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
		return(1);

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
	if (l1.type != Literal::Function)
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
	// check for tautologies, compare class by class
	PositiveClauseIterator posIter(c);
	NegativeClauseIterator negIter(c);
	while (!posIter.done() && !negIter.done())
	{
		if (posIter().key < ~(negIter().key))
		{
			posIter++;
		}
		else if (posIter().key > ~(negIter().key))
		{
			negIter++;
		}
		else
		{
			// we have a class and a counterpart class
			// that is negated. check each literal.
			//
			ListIterator<Literal> plIter(posIter().data);
			for ( ; !plIter.done(); plIter++)
			{
				// check each literal against the other.
				ListIterator<Literal> nlIter(negIter().data);
				for ( ; !nlIter.done(); nlIter++)
				{
					if (equal(plIter(), ~nlIter()))
					{
						// we have a tautology
						return(OK);
					}
				}
			}
			posIter++;
			negIter++;
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
	if (cl1.getTotalMembers() >= cl2.getTotalMembers())
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

	// start checking class by class. every class in clause 1 
	// should be in clause2
	//
	for (ClauseIterator cl1Iter(cl1); !cl1Iter.done(); cl1Iter++)
	{
		if (cl2.getMembersInSet(cl1Iter().key) <= 0)
		{
			// this class does not exist in the
			// clause 2. therefore clause 1
			// can not be a subset of clause 2.
			//
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
		return(NOMATCH);

	// search for a possible unifying literal
	int a = j;
	Literal Li(C[i]);
	if (theta.applyTo(Li) != OK)
		return(NOTOK);
	Substitutions mu;
	for ( ; a <= D.getSize(); a++)
	{
		mu.clear();
		Literal Ka(D[a]);
		if (unify(Li, Ka, mu) == OK)
			break;
	}
	if (a > D.getSize())
		return(NOMATCH);

	// attempt to extend the matching
	if (i == C.getSize() || ST(C, D, i+1, 1, theta*mu) == OK)
		return(OK);
	else
		return(ST(C, D, i, a+1, theta));
}

int
subsumes(const Clause &cl1, const Clause &cl2, Substitutions &s)
{
	// clear substitution list
	s.clear();

	// check that every class in clause 1 exists in clause 2. 
	if (subset(cl1, cl2) != OK)
		return(NOMATCH);

	// now start the actual subsumption algorithm.
	Substitutions subs;
	Clause clause2(cl2);
	if (groundSubstitutions(clause2, subs) != OK)
		return(NOTOK);

	// convert second clause to a ground clause
	if (subs.applyTo(clause2) != OK)
		return(NOTOK);

	// copy clauses to arrays
	Array<Literal> clarray1(1, cl1.getTotalMembers());
	ClauseIterator cl1Iter(cl1);
	for (int i=1; !cl1Iter.done(); cl1Iter++)
	{
		ListIterator<Literal> dataIter(cl1Iter().data);
		for ( ; !dataIter.done(); i++, dataIter++)
		{
			clarray1[i] = dataIter();
		}
	}
	Array<Literal> clarray2(1, cl2.getTotalMembers());
	ClauseIterator cl2Iter(clause2);
	for (int i=1; !cl2Iter.done(); cl2Iter++)
	{
		ListIterator<Literal> dataIter(cl2Iter().data);
		for ( ; !dataIter.done(); i++, dataIter++)
		{
			clarray2[i] = dataIter();
		}
	}


	// use Stillman's algorithm
	return(ST(clarray1, clarray2, 1, 1, s));
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
factor(Literal &fl, Clause &cl)
{
	// get equivalence set for this literal
	List<Literal> flset;
	if (cl.retrieveEqSet(fl, flset) != OK)
		return(NOMATCH);
 
	// check if there is more than one literal in the set
	if (flset.getCount() <= 1)
		return(NOMATCH);

	// copy to an array for easier handling
	Array<Literal> ca(flset.getCount());
	ListIterator<Literal> flsetIter(flset);
	for (int ia=0; !flsetIter.done(); ia++, flsetIter++)
	{
		ca[ia] = flsetIter();
	}

	// now try to factor 
	List<int> idx;
	int keyfound = 0;
	int factoridx = -1;
	Substitutions subs;
	for (int ia=0; ia < ca.getSize(); ia++)
	{
		// we found the key clause
		if (equal(fl, ca[ia]))
		{
			// don't save this
			keyfound = 1;
			continue;
		}
		else if (idx.insertAtEnd(ia) != OK)
		{
			return(NOTOK);
		}

		// check for unification
		if (factoridx != -1)
		{
			// clear the substitutions
			subs.clear();

			// unify, if possible
			if (unify(Literal(fl), Literal(ca[ia]), subs) == OK)
			{
				factoridx = ia;
			}
		}
	}

	// check how many potentially unifiable literals were found
	if (factoridx == -1)
	{
		// no literals to unify
		return(NOMATCH);
	}
	if (!keyfound)
	{
		// key literal was NOT found. not possible.
		return(NOTOK);
	}

	// delete equivalent set
	List<Literal> data;
	if (cl.removeEqSet(fl, data) != OK)
		return(NOTOK);

	// insert new data
	ListIterator<int> idxIter(idx);
	for ( ; !idxIter.done(); idxIter++)
	{
		if (cl.insert(ca[idxIter()]) != OK)
			return(NOTOK);
	}

	// apply substitutions to entire clause
	if (subs.applyTo(cl) != OK)
		return(NOTOK);

	// all done
	return(OK);
}

