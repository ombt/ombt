// theorem prover code

// unix headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// headers 
#include "pr.h"
#include "trace.h"
#include "deletion.h"
#include "search.h"

// add extra axioms for equality, etc.
int
addextraaxioms()
{
	// check phases to run
	if (!phases[EXPANDFILE] || !phases[PARSEFILE] || 
	    !phases[ADDEXTRAAXIOMS])
	{
		cout << endl;
		cout << "Skipping adding extra axioms." << endl;
		return(OK);
	}

	// check if program uses equality
	if (equality)
	{
		// verbose message
		if (verbose)
		{
			cout << endl;
			cout << "Equality found in program ..." << endl;
		}

		// check if paramodulation is in use
		if (paramodulation)
		{
			// verbose message
			if (verbose)
			{
				cout << endl;
				cout << "Paramodulation is active ..." << endl;
			}

			// add functionally reflexive axioms
			if (addreflexiveaxioms() != OK)
			{
				ERROR("add reflexive axioms failed.", errno);
				return(NOTOK);
			}
		}
		else
		{
			// verbose message
			if (verbose)
			{
				cout << endl;
				cout << "Axiomatizing equality ..." << endl;
			}

			// axiomatize equality
			if (addequalityaxioms() != OK)
			{
				ERROR("add equality axioms failed.", errno);
				return(NOTOK);
			}
		}
		if (verbose)
		{
			cout << endl;
			cout << "Dumping parse tree after adding extra axioms ..." << endl;
			dumpTree();
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
			ERROR("addreflexiveaxioms failed.", errno);
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
			ERROR("addpredicateaxioms failed.", errno);
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
#ifdef SC42
		for (arg=1; arg<=argnum; arg++)
#else
		for (int arg=1; arg<=argnum; arg++)
#endif
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
			ERROR("addfunctionaxioms failed.", errno);
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
#ifdef SC42
		for (arg=1; arg<=argnum; arg++)
#else
		for (int arg=1; arg<=argnum; arg++)
#endif
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
removedSubsumed(const Clause &cl1, const Clause &cl2, List<Clause> &cl2rmv)
{
	// check if clause 1 subsumes clause 2.
	Substitutions s;
	Clause c1(cl1);
	Clause c2(cl2);
	statistics[AttemptedSubsumptionTests] += 1;
	totalstatistics[TotalAttemptedSubsumptionTests] += 1;
	if (subsumes(c1, c2, s) == OK)
	{
		statistics[SubsumedClausesRemoved] += 1;
		totalstatistics[TotalSubsumedClausesRemoved] += 1;
		if (cl2rmv.insertAtEnd(c2) != OK)
		{
			ERROR("insertAtEnd failed.", errno);
			return(NOTOK);
		}
	}
	return(OK);
}

int
removeSubsumed(Array<BinaryTree_AVL<Clause> > &clarray, unsigned int curdepth)
{
	// list of tautologies to remove
	List<Clause> clausesToRemove;

	// check if these tests should be run
	if (!subsumptiontest)
		return(OK);

	// check if any clause at the current depth is subsumed
	// by a clause at a lower level.
	//
	BinaryTree_AVL_Iterator_InOrder<Clause> cdIter(clarray[curdepth]);
	for ( ; !cdIter.done(); cdIter++)
	{
		// check if the current clause is subsumed
		for (int idepth = 0; idepth < curdepth; idepth++)
		{
			// get previous depth clauses
			BinaryTree_AVL_Iterator_InOrder<Clause> 
				idIter(clarray[idepth]);
			for ( ; !idIter.done(); idIter++)
			{
				// check if a clause is a subset of 
				// another. comparison is done by
				// comparing equivalent sets.
				//
				int status = 
					removedSubsumed(idIter(), 
						cdIter(), clausesToRemove);
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
		int status = clarray[curdepth].remove(clause);
		if (status != OK && status != NOMATCH)
		{
			ERROR("remove failed.", errno);
			return(NOTOK);
		}
	}
	// all done
	return(OK);
}

int
initialRemoveSubsumed(BinaryTree_AVL<Clause> &ctree)
{
	List<Clause> clausesToRemove;

	// check if these tests should be run
	if (!subsumptiontest)
		return(OK);

	// check if current depth is empty
	if (ctree.isEmpty())
		return(OK);

	// check for subsumed clauses
	BinaryTree_AVL_Iterator_InOrder<Clause> cdIter1(ctree);
	for ( ; !cdIter1.done(); cdIter1++)
	{
		BinaryTree_AVL_Iterator_InOrder<Clause> cdIter2(cdIter1);
		for (cdIter2++; !cdIter2.done(); cdIter2++)
		{
			int status = removedSubsumed(
				cdIter1(), cdIter2(), clausesToRemove);
			if (status != OK)
				return(status);
		}
	}

	// remove any clauses
	ListIterator<Clause> cltrIter(clausesToRemove);
	for ( ; !cltrIter.done(); cltrIter++)
	{
		Clause clause(cltrIter());
		int status = ctree.remove(clause);
		if (status != OK && status != NOMATCH)
		{
			ERROR("remove failed.", errno);
			return(NOTOK);
		}
	}

	// all done
	return(OK);
}

int
initialRemoveSubsumed(Array<BinaryTree_AVL<Clause> > &clarray, 
	unsigned int curdepth)
{
	List<Clause> clausesToRemove;

	// check if these tests should be run
	if (!subsumptiontest)
		return(OK);

	// check if current depth is empty
	if (clarray[curdepth].isEmpty())
		return(OK);

	// check for subsumed clauses
	BinaryTree_AVL_Iterator_InOrder<Clause> cdIter1(clarray[curdepth]);
	for ( ; !cdIter1.done(); cdIter1++)
	{
		BinaryTree_AVL_Iterator_InOrder<Clause> cdIter2(cdIter1);
		for (cdIter2++; !cdIter2.done(); cdIter2++)
		{
			int status = removedSubsumed(
				cdIter1(), cdIter2(), clausesToRemove);
			if (status != OK)
				return(status);
		}
	}

	// remove any clauses
	ListIterator<Clause> cltrIter(clausesToRemove);
	for ( ; !cltrIter.done(); cltrIter++)
	{
		Clause clause(cltrIter());
		int status = clarray[curdepth].remove(clause);
		if (status != OK && status != NOMATCH)
		{
			ERROR("remove failed.", errno);
			return(NOTOK);
		}
	}

	// all done
	return(OK);
}

// remove tautologies
int
removeTautologies(BinaryTree_AVL<Clause> &ctree)
{
	// list of tautologies to remove
	List<Clause> clausesToRemove;

	// check if these tests should be run
	if (!tautologytest)
		return(OK);

	// scan each equivalence set for tautologies
	BinaryTree_AVL_Iterator_InOrder<Clause> clIter(ctree);
	for ( ; !clIter.done(); clIter++)
	{
		// check each member for being a tautolgy
		Clause clause(clIter());
		statistics[AttemptedTautologyTests] += 1;
		totalstatistics[TotalAttemptedTautologyTests] += 1;
		if (tautology(clause) == OK)
		{
			// we have a tautology, remove it.
			statistics[TautologiesRemoved] += 1;
			totalstatistics[TotalTautologiesRemoved] += 1;
			if (clausesToRemove.insert(clause) != OK)
			{
				ERROR("insert failed.", errno);
				return(NOTOK);
			}
		}
	}

	// remove any clauses
	ListIterator<Clause> cltrIter(clausesToRemove);
	for ( ; !cltrIter.done(); cltrIter++)
	{
		Clause clause(cltrIter());
		int status = ctree.remove(clause);
		if (status != OK && status != NOMATCH)
		{
			ERROR("remove failed.", errno);
			return(NOTOK);
		}
	}

	// all done
	return(OK);
}

int
removeTautologies(Array<BinaryTree_AVL<Clause> > &clarray, 
	unsigned int curdepth)
{
	// list of tautologies to remove
	List<Clause> clausesToRemove;

	// check if these tests should be run
	if (!tautologytest)
		return(OK);

	// scan each equivalence set for tautologies
	BinaryTree_AVL_Iterator_InOrder<Clause> clIter(clarray[curdepth]);
	for ( ; !clIter.done(); clIter++)
	{
		// check each member for being a tautolgy
		Clause clause(clIter());
		statistics[AttemptedTautologyTests] += 1;
		totalstatistics[TotalAttemptedTautologyTests] += 1;
		if (tautology(clause) == OK)
		{
			// we have a tautology, remove it.
			statistics[TautologiesRemoved] += 1;
			totalstatistics[TotalTautologiesRemoved] += 1;
			if (clausesToRemove.insert(clause) != OK)
			{
				ERROR("insert failed.", errno);
				return(NOTOK);
			}
		}
	}

	// remove any clauses
	ListIterator<Clause> cltrIter(clausesToRemove);
	for ( ; !cltrIter.done(); cltrIter++)
	{
		Clause clause(cltrIter());
		int status = clarray[curdepth].remove(clause);
		if (status != OK && status != NOMATCH)
		{
			ERROR("remove failed.", errno);
			return(NOTOK);
		}
	}

	// all done
	return(OK);
}

// dump any new clause
void
dumpnewclause(const Clause &cl1, const Clause &cl2, const Clause &newcl)
{
	cout << "((p1,l1), (p2,l2)) = ((";
	cout << cl1.getNumber() << "," << cl1.getTotalMembers() << "),(";
	cout << cl2.getNumber() << "," << cl2.getTotalMembers() << ")) = ";
	cout << newcl << endl;
	return;
}

// resolve two clauses, if possible.
int
resolveClauses(Array<BinaryTree_AVL<Clause> > &clausesArray, 
	const Clause &cl1, const Clause &cl2, 
	int &clausesAdded, unsigned int currentDepth)
{
	// check if any of the clauses are empty
	statistics[ResolutionsAttempted] += 1;
	totalstatistics[TotalResolutionsAttempted] += 1;
	if (cl1.isEmpty() || cl2.isEmpty())
	{
		return(VALID);
	}

	// check if clauses can be resolved
	if (!cl1.getSOS() && !cl2.getSOS())
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
		ERROR("getMaximalLiteral failed.", errno);
		return(NOTOK);
	}
	Literal maxlit2;
	if (cl2.getMaximalLiteral(maxlit2) != OK)
	{
		ERROR("getMaximalLiteral failed.", errno);
		return(NOTOK);
	}
	if ((cl1.getTotalMembers() > maxliterals) ||
	    (cl2.getTotalMembers() > maxliterals))
	{
		statistics[MaximumLiteralsClausesRejected] += 1;
		totalstatistics[TotalMaximumLiteralsClausesRejected] += 1;
		return(NOMATCH);
	}
	if (maxlit1.unify_ne(~maxlit2))
	{
		return(NOMATCH);
	}

	// factor clauses
	Substitutions subs;
	Clause tmp_cl1(cl1);
	if (factor(maxlit1, tmp_cl1, subs) == NOTOK)
	{
		ERROR("factor failed.", errno);
		return(NOTOK);
	}
	Clause tmp_cl2(cl2);
	if (factor(maxlit2, tmp_cl2, subs) == NOTOK)
	{
		ERROR("factor failed.", errno);
		return(NOTOK);
	}

	// attempt to unify the clauses
	subs.clear();
	int status = unify(maxlit1, ~maxlit2, subs);
	switch (status)
	{
	case OK:
		if (verbose)
		{
			cout << endl;
			Literal ml1(maxlit1);
			cout << "max literal 1 (before subs): " << ml1 << endl;
			subs.applyTo(ml1);
			cout << "max literal 1 (after subs): " << ml1 << endl;
			Literal ml2(maxlit2);
			cout << "max literal 2 (before subs): " << ml2 << endl;
			subs.applyTo(ml2);
			cout << "max literal 2 (after subs): " << ml2 << endl;
			MustBeTrue(equal(ml1, ~ml2));
		}
		break;
	case NOMATCH:
		return(NOMATCH);
	default:
		ERROR("unify failed.", errno);
		return(status);
	}

	// can resolve, remove literals from clauses
	Clause newcl1(cl1);
	if (newcl1.remove(maxlit1) != OK)
	{
		ERROR("remove failed.", errno);
		return(NOTOK);
	}
	if (subs.applyTo(newcl1) != OK)
	{
		ERROR("applyTo failed.", errno);
		return(NOTOK);
	}
	Clause newcl2(cl2);
	if (newcl2.remove(maxlit2) != OK)
	{
		ERROR("remove failed.", errno);
		return(NOTOK);
	}
	if (subs.applyTo(newcl2) != OK)
	{
		ERROR("applyTo failed.", errno);
		return(NOTOK);
	}

	// store new clause and update flag
	Clause newcl = newcl1+newcl2;
	if (newcl.renameVariables() != OK)
	{
		ERROR("renameVariables failed.", errno);
		return(NOTOK);
	}
	newcl.setDepth(currentDepth+1);
	newcl.setNumber(nextClause++);
	if (clausesArray[currentDepth+1].insert(newcl) != OK)
	{
		ERROR("insert failed.", errno);
		return(NOTOK);
	}
	clausesAdded = 1;

	// indicate the clauses that were resolved
	statistics[ClausesGenerated] += 1;
	totalstatistics[TotalClausesGenerated] += 1;
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
resolveDepth(Array<BinaryTree_AVL<Clause> > &clausesArray, 
		int &clausesAdded, unsigned int currentDepth)
{
	// check if resolving clauses at different depths
	// or just within one depth.
	//
	if (currentDepth == 1)
	{
		// resolving at initial depth only
		BinaryTree_AVL_Iterator_InOrder<Clause> 
			cdclIter(clausesArray[currentDepth]);
		for ( ; nextClause <= maxclause && !cdclIter.done(); cdclIter++)
		{
			// scan each equivalent set.
			BinaryTree_AVL_Iterator_InOrder<Clause> 
				idclIter(cdclIter);
			for (idclIter++ ; 
			     nextClause <= maxclause && !idclIter.done(); 
			     idclIter++)
			{
				// try to resolve the clauses
				int status = resolveClauses(
					clausesArray, cdclIter(), idclIter(), 
					clausesAdded, currentDepth);

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

	// resolve the clauses at the current depth
	// with clauses at previous depths
	//
	BinaryTree_AVL_Iterator_InOrder<Clause> 
		cdclIter(clausesArray[currentDepth]);
	for ( ; nextClause <= maxclause && !cdclIter.done(); cdclIter++)
	{
		// scan the clauses at each depth
		for (int idepth = 0; nextClause <= maxclause && 
			idepth <  currentDepth; idepth++)
		{
			// scan each equivalent set.
			BinaryTree_AVL_Iterator_InOrder<Clause> 
				idclIter(clausesArray[idepth]);
			for ( ; nextClause <= maxclause && 
				!idclIter.done(); idclIter++)
			{
				// try to resolve the clauses
				int status = resolveClauses(
					clausesArray, cdclIter(), idclIter(), 
					clausesAdded, currentDepth);

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

	// all done
	return(OK);
}

// run provers
int
runprover(List<Clause> &clist)
{
	// track current depth in proof
	unsigned int currentDepth = 1;

	// array of clauses for each depth
	Array<BinaryTree_AVL<Clause> > clausesArray(maxdepth+2);
	ListIterator<Clause> clIter(clist);
	for (nextClause=1; !clIter.done(); clIter++)
	{
		// insert clauses at current deph
		Clause clause(clIter());
		clause.setDepth(currentDepth);
		clause.setNumber(nextClause++);
		if (clausesArray[currentDepth].insert(clause) != OK)
		{
			ERROR("insert failed.", errno);
			return(NOTOK);
		}
	}

	// list clauses
	cout << endl;
	cout << "===============================================" << endl;
	cout << "initial list of clauses: " << endl;
	BinaryTree_AVL_Iterator_InOrder<Clause>
		 cdclIter(clausesArray[currentDepth]);
	for ( ; !cdclIter.done(); cdclIter++)
	{
		cout << cdclIter() << endl;
	}
	cout << "===============================================" << endl;

	// filter clauses
	if (removeTautologies(clausesArray, currentDepth) != OK)
	{
		ERROR("removeTautologies failed.", errno);
		return(NOTOK);
	}
	if (initialRemoveSubsumed(clausesArray, currentDepth) != OK)
	{
		ERROR("removeSubsumed failed.", errno);
		return(NOTOK);
	}

	// start breadth-first resolution process
	int clausesAdded = 1; 
	for ( ; (currentDepth <= maxdepth) && 
		(nextClause <= maxclause) && clausesAdded; currentDepth++)
	{
		// reset clauses-added to false. it is reset to true
		// by resolveClauses if any clauses are added.
		//
		clausesAdded = 0;

		// resolve clauses at each level
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
			programstatistics[TotalValidPrograms] += 1;
			cout << endl;
			cout << "Run Time Statistics ..." << endl;
			cout << statistics << endl;
			cout << endl;
			cout << "VALID program." << endl;
			return(VALID);
		case NOTPROVEN:
			// not proven program
			if (nextClause > maxclause)
			{
				programstatistics[TotalMaximumClauseExceededPrograms] += 1;
				programstatistics[TotalNotProvenPrograms] += 1;
				ERROR("maxclause exceeded !!!", EINVAL);
			}
			else if (currentDepth > maxdepth)
			{
				programstatistics[TotalMaximumDepthExceededPrograms] += 1;
				programstatistics[TotalNotProvenPrograms] += 1;
				ERROR("maxdepth exceeded !!!", EINVAL);
			}
			cout << endl;
			cout << "Run Time Statistics ..." << endl;
			cout << statistics << endl;
			cout << endl;
			cout << "NOTPROVEN program." << endl;
			return(NOTPROVEN);
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
			{
				ERROR("removeTautologies failed.", errno);
				return(NOTOK);
			}
			if (removeSubsumed(
				clausesArray, currentDepth+1) != OK)
			{
				ERROR("removeSubsumed failed.", errno);
				return(NOTOK);
			}
		}
	}

	// dump out current statistics 
	cout << endl;
	cout << "Run Time Statistics ..." << endl;
	cout << statistics << endl;

	// all done
	programstatistics[TotalNotProvenPrograms] += 1;
	if (nextClause > maxclause)
	{
		programstatistics[TotalMaximumClauseExceededPrograms] += 1;
		ERROR("maxclause exceeded !!!", EINVAL);
		return(NOTPROVEN);
	}
	else if (currentDepth > maxdepth)
	{
		programstatistics[TotalMaximumDepthExceededPrograms] += 1;
		ERROR("maxdepth exceeded !!!", EINVAL);
		return(NOTPROVEN);
	}
	else if (!clausesAdded)
	{
		cout << endl;
		cout << "NOTPROVEN program." << endl;
		return(NOTPROVEN);
	}
	else
	{
		ERROR("runprover failed.", errno);
		return(NOTOK);
	}
}

// use a best-first search theorem prover
int
runbfsprover(List<Clause> &clist)
{
	int status;

	// dump data info
	if (reportmemoryusage)
	{
		SETFINALEDATA();
		DUMPDATA(cout);
	}

	// copy clauses to a tree for better performance
	BinaryTree_AVL<Clause> ctree;
	ListIterator<Clause> clIter(clist);
	for (nextClause=1; !clIter.done(); clIter++)
	{
		// insert clauses at current deph
		Clause clause(clIter());
		clause.setDepth(1);
		clause.setNumber(nextClause++);
		if (ctree.insert(clause) != OK)
		{
			ERROR("insert failed.", errno);
			return(NOTOK);
		}
	}

	// dump data info
	if (reportmemoryusage)
	{
		SETFINALEDATA();
		DUMPDATA(cout);
	}

	// list clauses
	cout << endl;
	cout << "===============================================" << endl;
	cout << "initial list of clauses: " << endl;
	BinaryTree_AVL_Iterator_InOrder<Clause> ctreeIter(ctree);
	for ( ; !ctreeIter.done(); ctreeIter++)
	{
		cout << ctreeIter() << endl;
	}
	cout << "===============================================" << endl;

	// dump data info
	if (reportmemoryusage)
	{
		SETFINALEDATA();
		DUMPDATA(cout);
	}

	// filter clauses
	if (removeTautologies(ctree) != OK)
	{
		ERROR("removeTautologies failed.", errno);
		return(NOTOK);
	}
	if (initialRemoveSubsumed(ctree) != OK)
	{
		ERROR("removeSubsumed failed.", errno);
		return(NOTOK);
	}

	// dump data info
	if (reportmemoryusage)
	{
		SETFINALEDATA();
		DUMPDATA(cout);
	}

	// separate clauses into SOS and axioms
	BinaryTree_AVL<Clause> soslist;
	BinaryTree_AVL<Clause> axiomlist;
	for (ctreeIter.reset(); !ctreeIter.done(); ctreeIter++)
	{
		if (ctreeIter().getSOS())
		{
			if ((status = soslist.insert(ctreeIter())) != OK)
			{
				ERROR("insert failed.", errno);
				return(status);
			}
		}
		else
		{
			if ((status = axiomlist.insert(ctreeIter())) != OK)
			{
				ERROR("insert failed.", errno);
				return(status);
			}
		}
	}

	// dump data info
	if (reportmemoryusage)
	{
		SETFINALEDATA();
		DUMPDATA(cout);
	}

	// generate a list of possible resolution states
	List<BFSNode> bfsnodelist;
	BinaryTree_AVL_Iterator_InOrder<Clause> sosIter1(soslist);
	for ( ; !sosIter1.done(); sosIter1++)
	{
		if (sosIter1().getTotalMembers() > maxliterals)
			continue;
		Literal maxlit1;
		if (sosIter1().getMaximalLiteral(maxlit1) != OK)
		{
			ERROR("get maximal literal failed.", errno);
			return(NOTOK);
		}
		BinaryTree_AVL_Iterator_InOrder<Clause> sosIter2(sosIter1);
		for (sosIter2++ ; !sosIter2.done(); sosIter2++)
		{
			// generate possible resolution states
			Literal maxlit2;
			if (sosIter2().getMaximalLiteral(maxlit2) != OK)
			{
				ERROR("get maximal literal failed.", errno);
				return(NOTOK);
			}
			if (sosIter2().getTotalMembers() > maxliterals)
			{
				statistics[MaximumLiteralsClausesRejected] += 1;
				totalstatistics[TotalMaximumLiteralsClausesRejected] += 1;
				continue;
			}
			if (maxlit1.unify_ne(~maxlit2))
				continue;
			BFSNode bfsnode(maxlit1, maxlit2, 
					sosIter1(), sosIter2());
			if (bfsnodelist.insertAtEnd(bfsnode) != OK)
			{
				ERROR("insert failed.", errno);
				return(status);
			}
		}
		BinaryTree_AVL_Iterator_InOrder<Clause> axiomIter(axiomlist);
		for ( ; !axiomIter.done(); axiomIter++)
		{
			// generate possible resolution states
			Literal maxlit2;
			if (axiomIter().getMaximalLiteral(maxlit2) != OK)
			{
				ERROR("get maximal literal failed.", errno);
				return(NOTOK);
			}
			if (axiomIter().getTotalMembers() > maxliterals)
			{
				statistics[MaximumLiteralsClausesRejected] += 1;
				totalstatistics[TotalMaximumLiteralsClausesRejected] += 1;
				continue;
			}
			if (maxlit1.unify_ne(~maxlit2))
				continue;
			BFSNode bfsnode(maxlit1, maxlit2, 
					sosIter1(), axiomIter());
			if (bfsnodelist.insertAtEnd(bfsnode) != OK)
			{
				ERROR("insert failed.", errno);
				return(status);
			}
		}
	}

	// dump data info
	if (reportmemoryusage)
	{
		SETFINALEDATA();
		DUMPDATA(cout);
	}

	// call search routines 
	switch (searchtype)
	{
	case BestFirst:
		status = BestFirstSearch(bfsnodelist, soslist, axiomlist);
		break;
	case DepthFirstHillClimb:
		status = DepthFirstHillClimbSearch(bfsnodelist, soslist, axiomlist);
		break;
	case DepthFirst:
		status = DepthFirstSearch(bfsnodelist, soslist, axiomlist);
		break;
	case BreadthFirst:
		status = BreadthFirstSearch(bfsnodelist, soslist, axiomlist);
		break;
	case IterativeDeepening:
		status = IterativeDeepeningSearch(bfsnodelist, soslist, axiomlist);
		break;
	default:
		ERRORD("unknown search type", searchtype, EINVAL);
		return(NOTOK);
	}

	// dump data info
	if (reportmemoryusage)
	{
		SETFINALEDATA();
		DUMPDATA(cout);
	}

	// report results of provers
	switch (status)
	{
	case OK:
	case VALID:
		// valid program
		programstatistics[TotalValidPrograms] += 1;
		cout << endl;
		cout << "Run Time Statistics ..." << endl;
		cout << statistics << endl;
		cout << endl;
		cout << "VALID program." << endl;

		// dump data info
		if (reportmemoryusage)
		{
			SETFINALEDATA();
			DUMPDATA(cout);
		}
		return(VALID);
	case NOTPROVEN:
		if (nextClause > maxclause)
		{
			programstatistics[TotalMaximumClauseExceededPrograms] += 1;
			ERROR("maxclause exceeded !!!", EINVAL);
		}
		else if (currentBFSDepth > maxdepth)
		{
			programstatistics[TotalMaximumDepthExceededPrograms] += 1;
			ERROR("maxdepth exceeded !!!", EINVAL);
		}
		programstatistics[TotalNotProvenPrograms] += 1;
		cout << endl;
		cout << "Run Time Statistics ..." << endl;
		cout << statistics << endl;
		cout << endl;
		cout << "NOTPROVEN program." << endl;

		// dump data info
		if (reportmemoryusage)
		{
			SETFINALEDATA();
			DUMPDATA(cout);
		}
		return(NOTPROVEN);
	default:
		// some type of error
		ERRORD("unexpected return from best first search !!!", 
			status, EINVAL);

		// dump data info
		if (reportmemoryusage)
		{
			SETFINALEDATA();
			DUMPDATA(cout);
		}
		return(status);
	}
}

int
runprover()
{
	// dump data info
	if (reportmemoryusage)
	{
		SETFINALEDATA();
		DUMPDATA(cout);
	}

	// check phases
	if (!phases[EXPANDFILE] || !phases[PARSEFILE] || 
	    !phases[ADDEXTRAAXIOMS] || !phases[CONVERT2CNF] ||
	    !phases[RUNPROVER])
	{
		cout << endl;
		cout << "Skipping running prover ..." << endl;
		return(OK);
	}

	// count number of programs run
	programstatistics[TotalProgramsExecuted] += 1;

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
		{
			ERROR("insertAtEnd.", EINVAL);
			return(NOTOK);
		}
	}

	// initialize statistics ...
	statistics[AttemptedSubsumptionTests] = 0;
	statistics[SubsumedClausesRemoved] = 0;
	statistics[AttemptedTautologyTests] = 0;
	statistics[TautologiesRemoved] = 0;
	statistics[ClausesGenerated] = 0;
	statistics[ResolutionsAttempted] = 0;
	statistics[RedundantClauseTestsAttempted] = 0;
	statistics[RedundantClausesRejected] = 0;
	statistics[SolutionsFound] = 0;
	statistics[MaximumLiteralsClausesRejected] = 0;

	// initialize the number of solutions found
	solutionsfound = 0;

	// dump data info
	if (reportmemoryusage)
	{
		SETFINALEDATA();
		DUMPDATA(cout);
	}

	// run real prover on list of clauses
	if (searchtype != Saturation)
		return(runbfsprover(clauselist));
	else
		return(runprover(clauselist));
}
