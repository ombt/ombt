// theorem prover code

// unix headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>

// headers 
#include "pr.h"

// counter for clauses
static int nextClause = 1;
static unsigned int currentDepth = 0;

// add extra axioms for equality, etc.
int
addextraaxioms()
{
	// check if program uses equality
	if (equality)
	{
		// check if paramodulation is in use
		if (paramodulation)
		{
			// add functionally reflexive axioms
			if (addreflexiveaxioms() != OK)
			{
				ERROR("add reflex failed.", errno);
				return(NOTOK);
			}
		}
		else
		{
			// axiomatize equality
			if (addequalityaxioms() != OK)
			{
				ERROR("add reflex failed.", errno);
				return(NOTOK);
			}
		}
	}

	// all done
	return(OK);
}

// add functionally-reflexive axioms for paramodulation
int
addreflexiveaxioms(int iarg, int argnum, const String &fname, 
		const Array<String> &argnames, Semantic *&psem)
{
	// check which case
	if (iarg < argnum)
	{
		// still counting arguments
		if (addreflexiveaxioms(
			iarg+1, argnum, fname, argnames, psem) != OK)
		{
			return(NOTOK);
		}

		// create a universal quantifier record
		psem = new Semantic(Expression::Universal, 
				argnames[iarg], psem);
		MustBeTrue(psem != NULL);
	}
	else if (iarg == argnum)
	{
		// create f(x,...) = f(x,...) record
		// first, create two argument lists
		List<Semantic * > *pargs1 = new List<Semantic * >;
		MustBeTrue(pargs1 != NULL);
		List<Semantic * > *pargs2 = new List<Semantic * >;
		MustBeTrue(pargs1 != NULL);
		for (int arg=0; arg<argnum; arg++)
		{
			Semantic *pvar1 = 
				new Semantic(Term::Variable, argnames[arg]);
			MustBeTrue(pvar1 != NULL);
			MustBeTrue(pargs1->insertAtEnd(pvar1) == OK);
			Semantic *pvar2 = 
				new Semantic(Term::Variable, argnames[arg]);
			MustBeTrue(pvar2 != NULL);
			MustBeTrue(pargs2->insertAtEnd(pvar2) == OK);
		}

		// create two function records
		Semantic *pfunc1 = 
			new Semantic(Term::Function, fname, pargs1, argnum);
		MustBeTrue(pfunc1 != NULL);
		Semantic *pfunc2 = 
			new Semantic(Term::Function, fname, pargs2, argnum);
		MustBeTrue(pfunc2 != NULL);

		// create an equality record
		List<Semantic * > *pargs = new List<Semantic *>;
		MustBeTrue(pargs != NULL);
		MustBeTrue(pargs->insertAtEnd(pfunc1) == OK);
		MustBeTrue(pargs->insertAtEnd(pfunc2) == OK);
		psem = new Semantic(Predicate::Equal, String("="), pargs, 2);
		MustBeTrue(psem != NULL);
	}
	else
	{
		ERROR("arg count > number of args.", EINVAL);
		return(NOTOK);
	}

	// all done
	return(OK);
}

int 
addreflexiveaxioms()
{
	// add x = x axiom
	String varname = uniqueName(String("_V"));
	Semantic *pvar1 = new Semantic(Term::Variable, varname);
	MustBeTrue(pvar1 != NULL);
	Semantic *pvar2 = new Semantic(Term::Variable, varname);
	MustBeTrue(pvar2 != NULL);
	List<Semantic * > *pargs = new List<Semantic * >;
	MustBeTrue(pargs != NULL);
	MustBeTrue(pargs->insertAtEnd(pvar1) == OK);
	MustBeTrue(pargs->insertAtEnd(pvar2) == OK);
	Semantic *peq = new Semantic(Predicate::Equal, String("="), pargs, 2);
	MustBeTrue(peq != NULL);
	Semantic *pu = new Semantic(Expression::Universal, varname, peq);
	MustBeTrue(pu != NULL);
	MustBeTrue(ptrees.insertAtFront(pu) == OK);

	// add reflexive axioms for each function
	BinaryTree_AVL_Iterator_InOrder<Symbol> symbolsIter(symbols);
	for ( ; !symbolsIter.done(); symbolsIter++)
	{
		// check if symbol is a function
		Symbol symbol = symbolsIter();
		if (symbol.getType() != Symbol::Function)
			continue;

		// we have a function, get number of arguments
		int argnum = symbol.getArgs();
		String fname = symbol.getName();

		// generate list of variable names
		Array<String> argnames(argnum);
		for (int arg = 0; arg < argnum; arg++)
		{
			argnames[arg] = uniqueName(String("_V"));
		}

		// build record recursively
		int iarg = 0;
		Semantic *psem = NULL;
		if (addreflexiveaxioms(
			iarg, argnum, fname, argnames, psem) != OK)
		{
			ERROR("failed to build semantic record.", errno);
			return(NOTOK);
		}

		// add to set of axioms
		MustBeTrue(ptrees.insertAtFront(psem) == OK);
	}

	// all done
	return(OK);
}

// axiomatize equality
int
addeqreflexiveaxiom()
{
	// get variable name
	String x = uniqueName(String("_V"));

	// get two variable records
	Semantic *pvar1 = new Semantic(Term::Variable, x);
	MustBeTrue(pvar1 != NULL);
	Semantic *pvar2 = new Semantic(Term::Variable, x);
	MustBeTrue(pvar2 != NULL);

	// create a equal record
	List<Semantic * > *pargs = new List<Semantic * >;
	MustBeTrue(pargs != NULL);
	MustBeTrue(pargs->insertAtEnd(pvar1) == OK);
	MustBeTrue(pargs->insertAtEnd(pvar2) == OK);
	Semantic *peq = new Semantic(Predicate::Equal, String("="), pargs, 2);
	MustBeTrue(peq != NULL);

	// create a universal record
	Semantic *pu = new Semantic(Expression::Universal, x, peq);
	MustBeTrue(pu != NULL);

	// save record, place at front of program.
	MustBeTrue(ptrees.insertAtFront(pu) == OK);

	// all done
	return(OK);
}

int
addeqsymmetryaxiom()
{
	// get variable names
	String x = uniqueName(String("_V"));
	String y = uniqueName(String("_V"));

	// create x!=y record
	Semantic *px = new Semantic(Term::Variable, x);
	MustBeTrue(px != NULL);
	Semantic *py = new Semantic(Term::Variable, y);
	MustBeTrue(py != NULL);
	List<Semantic * > *pargs = new List<Semantic * >;
	MustBeTrue(pargs != NULL);
	MustBeTrue(pargs->insertAtEnd(px) == OK);
	MustBeTrue(pargs->insertAtEnd(py) == OK);
	Semantic *peq = new Semantic(Predicate::Equal, String("="), pargs, 2);
	MustBeTrue(peq != NULL);
	Semantic *pxney = new Semantic(Expression::Negation, NULL, peq);
	MustBeTrue(pxney != NULL);

	// create y==x record
	py = new Semantic(Term::Variable, y);
	MustBeTrue(py != NULL);
	px = new Semantic(Term::Variable, x);
	MustBeTrue(px != NULL);
	pargs = new List<Semantic * >;
	MustBeTrue(pargs != NULL);
	MustBeTrue(pargs->insertAtEnd(py) == OK);
	MustBeTrue(pargs->insertAtEnd(px) == OK);
	Semantic *pyeqx = new Semantic(Predicate::Equal, String("="), pargs, 2);
	MustBeTrue(pyeqx != NULL);

	// create x!=y || y==x record
	Semantic *por = new Semantic(Expression::Or, pxney, pyeqx);
	MustBeTrue(por != NULL);

	// create a universal record
	Semantic *pu = new Semantic(Expression::Universal, y, por);
	MustBeTrue(pu != NULL);
	Semantic *pu2 = new Semantic(Expression::Universal, x, pu);
	MustBeTrue(pu2 != NULL);

	// save record, place a front of program.
	MustBeTrue(ptrees.insertAtFront(pu2) == OK);

	// all done
	return(OK);
}

int
addeqtransitiveaxiom()
{
	// get variable names
	String x = uniqueName(String("_V"));
	String y = uniqueName(String("_V"));
	String z = uniqueName(String("_V"));

	// create x!=y record
	Semantic *px = new Semantic(Term::Variable, x);
	MustBeTrue(px != NULL);
	Semantic *py = new Semantic(Term::Variable, y);
	MustBeTrue(py != NULL);
	List<Semantic * > *pargs = new List<Semantic * >;
	MustBeTrue(pargs != NULL);
	MustBeTrue(pargs->insertAtEnd(px) == OK);
	MustBeTrue(pargs->insertAtEnd(py) == OK);
	Semantic *peq = new Semantic(Predicate::Equal, String("="), pargs, 2);
	MustBeTrue(peq != NULL);
	Semantic *pxney = new Semantic(Expression::Negation, NULL, peq);
	MustBeTrue(pxney != NULL);

	// create y!=z record
	py = new Semantic(Term::Variable, y);
	MustBeTrue(py != NULL);
	Semantic *pz = new Semantic(Term::Variable, z);
	MustBeTrue(pz != NULL);
	pargs = new List<Semantic * >;
	MustBeTrue(pargs != NULL);
	MustBeTrue(pargs->insertAtEnd(py) == OK);
	MustBeTrue(pargs->insertAtEnd(pz) == OK);
	peq = new Semantic(Predicate::Equal, String("="), pargs, 2);
	MustBeTrue(peq != NULL);
	Semantic *pynez = new Semantic(Expression::Negation, NULL, peq);
	MustBeTrue(pynez != NULL);

	// create x==z record
	px = new Semantic(Term::Variable, x);
	MustBeTrue(px != NULL);
	pz = new Semantic(Term::Variable, z);
	MustBeTrue(pz != NULL);
	pargs = new List<Semantic * >;
	MustBeTrue(pargs != NULL);
	MustBeTrue(pargs->insertAtEnd(px) == OK);
	MustBeTrue(pargs->insertAtEnd(pz) == OK);
	Semantic *pxeqz = new Semantic(Predicate::Equal, String("="), pargs, 2);
	MustBeTrue(pxeqz != NULL);

	// create x!=y || y!=z || x==z record
	Semantic *por = new Semantic(Expression::Or, pxney, pynez);
	MustBeTrue(por != NULL);
	Semantic *por2 = new Semantic(Expression::Or, por, pxeqz);
	MustBeTrue(por2 != NULL);

	// create a universal record
	Semantic *pu = new Semantic(Expression::Universal, z, por2);
	MustBeTrue(pu != NULL);
	Semantic *pu2 = new Semantic(Expression::Universal, y, pu);
	MustBeTrue(pu2 != NULL);
	Semantic *pu3 = new Semantic(Expression::Universal, x, pu2);
	MustBeTrue(pu3 != NULL);

	// save record, place a front of program.
	MustBeTrue(ptrees.insertAtFront(pu3) == OK);

	// all done
	return(OK);
}

int
addpredicateaxioms(int j, int iarg, int argnum, const String &pname,
			const Array<String> &argnames, Semantic *&psem)
{
	// check which case
	if (iarg <= argnum)
	{
		// still counting arguments
		if (addpredicateaxioms(
			j, iarg+1, argnum, pname, argnames, psem) != OK)
		{
			return(NOTOK);
		}

		// create a universal quantifier record
		psem = new Semantic(Expression::Universal, 
				argnames[iarg], psem);
		MustBeTrue(psem != NULL);
	}
	else if (iarg > argnum)
	{
		// create xj != x0 record
		Semantic *pxj = new Semantic(Term::Variable, argnames[j]);
		MustBeTrue(pxj != NULL);
		Semantic *px0 = new Semantic(Term::Variable, argnames[0]);
		MustBeTrue(px0 != NULL);
		List<Semantic * > *pargs = new List<Semantic *>;
		MustBeTrue(pargs != NULL);
		MustBeTrue(pargs->insertAtEnd(pxj) == OK);
		MustBeTrue(pargs->insertAtEnd(px0) == OK);
		Semantic *peq = new Semantic(Predicate::Equal, 
				String("="), pargs, 2);
		MustBeTrue(peq != NULL);
		Semantic *pxjnex0 = 
			new Semantic(Expression::Negation, NULL, peq);
		MustBeTrue(pxjnex0 != NULL);

		// create ~P(...xj...) and 
		pargs = new List<Semantic * >;
		MustBeTrue(pargs != NULL);
		for (int arg=1; arg<=argnum; arg++)
		{
			Semantic *px = 
				new Semantic(Term::Variable, argnames[arg]);
			MustBeTrue(px != NULL);
			MustBeTrue(pargs->insertAtEnd(px) == OK);
		}
		Semantic *pp = new Semantic(Predicate::Function, 
					pname, pargs, argnum);
		MustBeTrue(pp != NULL);
		Semantic *pnegp = new Semantic(Expression::Negation, NULL, pp);
		MustBeTrue(pnegp != NULL);

		// create P(...x0...) and 
		pargs = new List<Semantic * >;
		MustBeTrue(pargs != NULL);
		for (int arg=1; arg<=argnum; arg++)
		{
			Semantic *px = NULL;
			if (arg != j)
				px = new Semantic(Term::Variable, 
					argnames[arg]);
			else
				px = new Semantic(Term::Variable, 
					argnames[0]);
			MustBeTrue(px != NULL);
			MustBeTrue(pargs->insertAtEnd(px) == OK);
		}
		pp = new Semantic(Predicate::Function, 
					pname, pargs, argnum);
		MustBeTrue(pp != NULL);

		// create xj!=x0 || ~P(...xj...) || P(...x0...)
		Semantic *por = new Semantic(Expression::Or, pxjnex0, pnegp);
		MustBeTrue(por != NULL);
		psem = new Semantic(Expression::Or, por, pp);
		MustBeTrue(psem != NULL);
	}

	// all done
	return(OK);
}

int
addpredicateaxioms()
{
	// add axioms for every predicate
	BinaryTree_AVL_Iterator_InOrder<Symbol> symbolsIter(symbols);
	for ( ; !symbolsIter.done(); symbolsIter++)
	{
		// check if symbol is a predicate
		Symbol symbol = symbolsIter();
		if (symbol.getType() != Symbol::PredicateFunction)
			continue;

		// we have a predicate function, get number of arguments
		int argnum = symbol.getArgs();
		String pname = symbol.getName();

		// generate list of variable names
		Array<String> argnames(argnum+1);
		for (int arg = 0; arg <= argnum; arg++)
		{
			argnames[arg] = uniqueName(String("_V"));
		}

		// build record recursively
		for (int j = 1; j <= argnum; j++)
		{
			// build records
			int iarg = 0;
			Semantic *psem = NULL;
			if (addpredicateaxioms(
				j, iarg, argnum, pname, argnames, psem) != OK)
			{
				ERROR("failed to build semantic record.", 
					errno);
				return(NOTOK);
			}

			// add to set of axioms
			MustBeTrue(ptrees.insertAtFront(psem) == OK);
		}
	}

	// all done
	return(OK);
}

int
addfunctionaxioms(int j, int iarg, int argnum, const String &fname,
			const Array<String> &argnames, Semantic *&psem)
{
	// check which case
	if (iarg <= argnum)
	{
		// still counting arguments
		if (addfunctionaxioms(
			j, iarg+1, argnum, fname, argnames, psem) != OK)
		{
			return(NOTOK);
		}

		// create a universal quantifier record
		psem = new Semantic(Expression::Universal, 
				argnames[iarg], psem);
		MustBeTrue(psem != NULL);
	}
	else if (iarg > argnum)
	{
		// create xj != x0 record
		Semantic *pxj = new Semantic(Term::Variable, argnames[j]);
		MustBeTrue(pxj != NULL);
		Semantic *px0 = new Semantic(Term::Variable, argnames[0]);
		MustBeTrue(px0 != NULL);
		List<Semantic * > *pargs = new List<Semantic *>;
		MustBeTrue(pargs != NULL);
		MustBeTrue(pargs->insertAtEnd(pxj) == OK);
		MustBeTrue(pargs->insertAtEnd(px0) == OK);
		Semantic *peq = new Semantic(Predicate::Equal, 
				String("="), pargs, 2);
		MustBeTrue(peq != NULL);
		Semantic *pxjnex0 = 
			new Semantic(Expression::Negation, NULL, peq);
		MustBeTrue(pxjnex0 != NULL);

		// create f(...xj...) and 
		pargs = new List<Semantic * >;
		MustBeTrue(pargs != NULL);
		for (int arg=1; arg<=argnum; arg++)
		{
			Semantic *px = 
				new Semantic(Term::Variable, argnames[arg]);
			MustBeTrue(px != NULL);
			MustBeTrue(pargs->insertAtEnd(px) == OK);
		}
		Semantic *pf = new Semantic(Term::Function, 
					fname, pargs, argnum);
		MustBeTrue(pf != NULL);

		// create f(...x0...) and 
		pargs = new List<Semantic * >;
		MustBeTrue(pargs != NULL);
		for (int arg=1; arg<=argnum; arg++)
		{
			Semantic *px = NULL;
			if (arg != j)
				px = new Semantic(Term::Variable, argnames[arg]);
			else
				px = new Semantic(Term::Variable, argnames[0]);
			MustBeTrue(px != NULL);
			MustBeTrue(pargs->insertAtEnd(px) == OK);
		}
		Semantic *pf2 = new Semantic(Term::Function, 
					fname, pargs, argnum);
		MustBeTrue(pf2 != NULL);

		// create equal record
		pargs = new List<Semantic * >;
		MustBeTrue(pargs != NULL);
		MustBeTrue(pargs->insertAtEnd(pf) == OK);
		MustBeTrue(pargs->insertAtEnd(pf2) == OK);
		peq = new Semantic(Predicate::Equal, String("="), pargs, 2);
		MustBeTrue(peq != NULL);

		// create xj!=x0 || f(...xj...) = f(...x0...)
		psem = new Semantic(Expression::Or, pxjnex0, peq);
		MustBeTrue(psem != NULL);
	}

	// all done
	return(OK);
}
int
addfunctionaxioms()
{
	// add axioms for every function
	BinaryTree_AVL_Iterator_InOrder<Symbol> symbolsIter(symbols);
	for ( ; !symbolsIter.done(); symbolsIter++)
	{
		// check if symbol is a function
		Symbol symbol = symbolsIter();
		if (symbol.getType() != Symbol::Function)
			continue;

		// we have a function, get number of arguments
		int argnum = symbol.getArgs();
		String pname = symbol.getName();

		// generate list of variable names
		Array<String> argnames(argnum+1);
		for (int arg = 0; arg <= argnum; arg++)
		{
			argnames[arg] = uniqueName(String("_V"));
		}

		// build record recursively
		for (int j = 1; j <= argnum; j++)
		{
			// build records
			int iarg = 0;
			Semantic *psem = NULL;
			if (addfunctionaxioms(
				j, iarg, argnum, pname, argnames, psem) != OK)
			{
				ERROR("failed to build semantic record.", 
					errno);
				return(NOTOK);
			}

			// add to set of axioms
			MustBeTrue(ptrees.insertAtFront(psem) == OK);
		}
	}

	// all done
	return(OK);
}

int
addequalityaxioms()
{
	// add (x = x) axiom
	if (addeqreflexiveaxiom() != OK)
	{
		ERROR("failed to add x=x axiom.", errno);
		return(NOTOK);
	}

	// add x!=y || y==x axiom
	if (addeqsymmetryaxiom() != OK)
	{
		ERROR("failed to add x!=y || y==x axiom.", errno);
		return(NOTOK);
	}

	// add x!=y || y!=z || x==z axiom
	if (addeqtransitiveaxiom() != OK)
	{
		ERROR("failed to add x!=y || y!=z || x==z axiom.", errno);
		return(NOTOK);
	}

	// add xj!=x0 || ~P(...xj...) || P(...x0...) axioms
	if (addpredicateaxioms() != OK)
	{
		ERROR("failed to xj!=x0 || ~P(...xj...) || P(...xj...) axiom.", errno);
		return(NOTOK);
	}

	// add xj!=x0 || f(...xj...) = f(...x0...) axioms
	if (addfunctionaxioms() != OK)
	{
		ERROR("failed to xj!=x0 || f(...xj...) = f(...xj...) axiom.", errno);
		return(NOTOK);
	}

	// all done
	return(OK);
}

// remove subsumed clauses
int
removedSubsumed(List<Clause> &cl1, List<Clause> &cl2, List<Clause> &cl2rmv)
{
	// check if any clauses in clause list 1 subsume any
	// clause in clause set 2.
	ListIterator<Clause> cl2Iter(cl2);
	for ( ; !cl2Iter.done(); cl2Iter++)
	{
		ListIterator<Clause> cl1Iter(cl1);
		for ( ; !cl1Iter.done(); cl1Iter++)
		{
			Substitutions s;
			Clause c1(cl1Iter());
			Clause c2(cl2Iter());
			if (subsumes(c1, c2, s) == OK)
			{
				if (cl2rmv.insertAtEnd(cl2Iter()) != OK)
					return(NOTOK);
			}
		}
	}
	return(OK);
}

int
removeSubsumed(Array<OrderedSet<Clause> > &clarray, unsigned int curdepth)
{
	// list of tautologies to remove
	List<Clause> clausesToRemove;

	// check if any clause at the current depth is subsumed
	// by a clause at a lower level.
	//
	OrderedSetIterator<Clause> cdIter(clarray[curdepth]);
	for ( ; !cdIter.done(); cdIter++)
	{
		// check if the current clause is subsumed
		for (int idepth = 0; idepth <= curdepth; idepth++)
		{
			// get previous depth clauses
			OrderedSetIterator<Clause> idIter(clarray[idepth]);
			for ( ; !idIter.done(); idIter++)
			{
				// check if a clause is a subset of 
				// another. comparison is done
				// comparing equivalent sets.
				//
				if (subset(idIter.key(), cdIter.key()) != OK)
					continue;

				// check each set for subsumed clauses
				int status = 
					removedSubsumed(idIter.data(), 
						cdIter.data(), clausesToRemove);
				if (status != OK)
					return(status);
			}
		}
	}
	
	
	// remove any clauses
	ListIterator<Clause> cltrIter(clausesToRemove);
	for ( ; !cltrIter.done(); cltrIter++)
	{
		Clause clause(cltrIter());
		if (clarray[curdepth].remove(clause) != OK)
			return(NOTOK);
	}

	// all done
	return(OK);
}

// remove tautologies
int
removeTautologies(Array<OrderedSet<Clause> > &clarray, unsigned int curdepth)
{
	// list of tautologies to remove
	List<Clause> clausesToRemove;

	// scan each equivalence set for tautologies
	OrderedSetIterator<Clause> clIter(clarray[curdepth]);
	for ( ; !clIter.done(); clIter++)
	{
		// check each member in an equivalence set.
		ListIterator<Clause> dataIter(clIter.data());
		for ( ; !dataIter.done(); dataIter++)
		{
			Clause clause(dataIter());
			if (tautology(clause) == OK)
			{
				// we have a tautology, remove it.
				if (clausesToRemove.insert(clause) != OK)
					return(NOTOK);
			}
		}
	}

	// remove any clauses
	ListIterator<Clause> cltrIter(clausesToRemove);
	for ( ; !cltrIter.done(); cltrIter++)
	{
		Clause clause(cltrIter());
		if (clarray[curdepth].remove(clause) != OK)
			return(NOTOK);
	}

	// all done
	return(OK);
}

// dump any new clause
void
dumpnewclause(const Clause &cl1, const Clause &cl2, const Clause &newcl)
{
	cout << endl;
	cout << "==========================================" << endl;
	cout << "clause 1: " << cl1 << endl;
	cout << "clause 2: " << cl2 << endl;
	cout << "resolvent clause 2: " << newcl << endl;
	cout << "==========================================" << endl;
	return;
}

// resolve two clauses, if possible.
int
resolveClauses(Array<OrderedSet<Clause> > &clausesArray, 
	Clause &cl1, Clause &cl2, int &clausesAdded, unsigned int currentDepth)
{
	// clear this flag
	clausesAdded = 0;

	// check if any of the clauses are empty
	if (cl1.isEmpty() || cl2.isEmpty())
	{
		return(VALID);
	}

	// check if clauses can be resolved
	if (!cl1.getSOS() && cl2.getSOS())
	{
		// one clause must be in the set-of-support
		return(NOMATCH);
	}
	if ((cl1.getType() == Clause::Negative && 
	     cl2.getType() == Clause::Negative) ||
	    (cl1.getType() == Clause::Positive && 
	     cl2.getType() == Clause::Positive))
	{
		// two positive clauses or two negative clauses
		// can not be resolved.
		return(NOMATCH);
	}

	// attempt to resolve two clauses. use A-ordering resolution,
	// try to resolve maximal literals.
	//
	Literal maxlit1;
	if (cl1.getMaximalLiteral(maxlit1) != OK)
	{
		return(NOTOK);
	}
	Literal maxlit2;
	if (cl2.getMaximalLiteral(maxlit2) != OK)
	{
		return(NOTOK);
	}
	if (maxlit1 != ~maxlit2)
	{
		return(NOMATCH);
	}

	// factor clauses
	if (factor(maxlit1, cl1) == NOTOK)
	{
		return(NOTOK);
	}
	if (factor(maxlit2, cl2) == NOTOK)
	{
		return(NOTOK);
	}

	// attempt to unify the clauses
	Substitutions subs;
	if (unify(maxlit1, ~maxlit2, subs) != OK)
	{
		return(NOMATCH);
	}

	// can resolve, remove literals from clauses
	Clause newcl1(cl1);
	if (newcl1.remove(maxlit1, 1) != OK)
	{
		return(NOTOK);
	}
	if (subs.applyTo(newcl1) != OK)
	{
		return(NOTOK);
	}
	Clause newcl2(cl2);
	if (newcl2.remove(maxlit2, 1) != OK)
	{
		return(NOTOK);
	}
	if (subs.applyTo(newcl2) != OK)
	{
		return(NOTOK);
	}

	// store new clause and update flag
	Clause newcl = newcl1+newcl2;
	if (newcl.renameVariables() != OK)
	{
		return(NOTOK);
	}
	newcl.setDepth(currentDepth+1);
	newcl.setNumber(nextClause++);
	if (clausesArray[currentDepth+1].insert(newcl) != OK)
	{
		return(NOTOK);
	}
	clausesAdded = 1;

	// indicate the clauses that were resolved
	dumpnewclause(cl1, cl2, newcl);

	// check if we found an empty clause
	if (newcl.isEmpty())
	{
		return(VALID);
	}
	else
	{
		return(OK);
	}
}

// resolve clauses at a given depth
int
resolveDepth(Array<OrderedSet<Clause> > &clausesArray, 
		int &clausesAdded, unsigned int currentDepth)
{
	// resolve the clauses at the current depth
	// with clauses at previous depths
	//
	OrderedSetIterator<Clause> cdIter(clausesArray[currentDepth]);
	for ( ; !cdIter.done(); cdIter++)
	{
		// scan the clauses in the equivalence set
		ListIterator<Clause> cdclIter(cdIter.data());
		for ( ; !cdclIter.done(); cdclIter++)
		{
			// scan the clauses at each depth
			for (int idepth = 0; idepth <= currentDepth; idepth++)
			{
				// scan each equivalent set.
				OrderedSetIterator<Clause> 
					idIter(clausesArray[idepth]);
				for ( ; !idIter.done(); idIter++)
				{
					// scan the clauses in the 
					// equivalence set
					ListIterator<Clause> 
						idclIter(idIter.data());
					for ( ; !idclIter.done(); idclIter++)
					{
						// try to resolve the clauses
						int status = 
							resolveClauses(
							clausesArray,
							cdclIter(),
							idclIter(),
							clausesAdded,
							currentDepth);

						// check for success, etc.
						switch (status)
						{
						case OK:
						case NOMATCH:
						case CONTINUE:
							break;
						default:
							return(status);
						}
					}
				}
			}
		}
		
	}

	// all done
	return(OK);
}

// run provers
int
runprover(List<Clause> &clist)
{
	// track current depth in proof
	unsigned int currentDepth = 0;

	// array of clauses for each depth
	Array<OrderedSet<Clause> > clausesArray(maxdepth+1);
	ListIterator<Clause> clIter(clist);
	for ( ; !clIter.done(); clIter++)
	{
		// insert clauses at current deph
		Clause clause(clIter());
		clause.setDepth(currentDepth);
		clause.setNumber(nextClause++);
		if (clausesArray[currentDepth].insert(clause) != OK)
			return(NOTOK);
	}

	// filter clauses
	if (removeTautologies(clausesArray, currentDepth) != OK)
		return(NOTOK);
	if (removeSubsumed(clausesArray, currentDepth) != OK)
		return(NOTOK);

	// start breadth-first resolution process
	int clausesAdded = 1; 
	for ( ; currentDepth <= maxdepth && clausesAdded; currentDepth++)
	{
		// reset clauses-added to false. it is reset to true
		// by resolveClauses if any clauses are added.
		//
		clausesAdded = 0;

		// resolve clauses at each level
		clausesAdded = 0;
		int status = 
			resolveDepth(clausesArray, clausesAdded, currentDepth);
		switch (status)
		{
		case OK:
		case NOMATCH:
		case CONTINUE:
			// continue to next depth
			break;
		case VALID:
			// valid program
			cout << "VALID program." << endl;
			return(VALID);
		case INVALID:
			// invalid program
			cout << "INVALID program." << endl;
			return(INVALID);
		default:
			// some type of error
			cout << "ERROR from resolveDepth." << endl;
			return(status);
		}

		// remove redundant clauses
		if (clausesAdded)
		{
			if (removeTautologies(
				clausesArray, currentDepth+1) != OK)
				return(NOTOK);
			if (removeSubsumed(
				clausesArray, currentDepth+1) != OK)
				return(NOTOK);
		}
	}

	// all done
	if (currentDepth > maxdepth)
	{
		return(NOTPROVEN);
	}
	else if (!clausesAdded)
	{
		return(INVALID);
	}
	else
	{
		return(NOTOK);
	}
}

int
runprover()
{
	// check if anything to prove
	if (clauses.isEmpty())
	{
		ERROR("clauses program is empty.", EINVAL);
		return(NOTOK);
	}

	// list of clauses
	List<Clause> clauselist;

	// convert strings to actual clauses for proving
	ListIterator<List<String > > clausesIter(clauses);
	for (int icl=1; !clausesIter.done(); clausesIter++, icl++)
	{
		Clause clause(clausesIter());
		if (clauselist.insertAtEnd(clause) != OK)
			return(NOTOK);
	}

	// list clauses
	cout << "===============================================" << endl;
	cout << "clauses of literals are: " << endl;
	ListIterator<Clause> clIter(clauselist);
	for ( ; !clIter.done(); clIter++)
	{
		cout << "clause is ... " << endl << clIter() << endl;
	}
	cout << "===============================================" << endl;

	// run real prover on list of clauses
	return(runprover(clauselist));
}
