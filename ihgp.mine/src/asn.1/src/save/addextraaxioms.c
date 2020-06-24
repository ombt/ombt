// add extra axioms for equality
#include "folp.h"

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
				ERROR("add reflexive axioms failed.", errno);
				return(NOTOK);
			}
		}
		else
		{
			// axiomatize equality
			if (addequalityaxioms() != OK)
			{
				ERROR("add equality axioms failed.", errno);
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
			ERROR("addreflexiveaxioms failed.", errno);
			return(NOTOK);
		}

		// create a universal quantifier record
		psem = new Semantic(Universal, 
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
				new Semantic(Variable, argnames[arg]);
			MustBeTrue(pvar1 != NULL);
			MustBeTrue(pargs1->insertAtEnd(pvar1) == OK);
			Semantic *pvar2 = 
				new Semantic(Variable, argnames[arg]);
			MustBeTrue(pvar2 != NULL);
			MustBeTrue(pargs2->insertAtEnd(pvar2) == OK);
		}

		// create two function records
		Semantic *pfunc1 = 
			new Semantic(Function, fname, pargs1, argnum);
		MustBeTrue(pfunc1 != NULL);
		Semantic *pfunc2 = 
			new Semantic(Function, fname, pargs2, argnum);
		MustBeTrue(pfunc2 != NULL);

		// create an equality record
		List<Semantic * > *pargs = new List<Semantic *>;
		MustBeTrue(pargs != NULL);
		MustBeTrue(pargs->insertAtEnd(pfunc1) == OK);
		MustBeTrue(pargs->insertAtEnd(pfunc2) == OK);
		psem = new Semantic(PredicateEqual, String("="), pargs, 2);
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
	Semantic *pvar1 = new Semantic(Variable, varname);
	MustBeTrue(pvar1 != NULL);
	Semantic *pvar2 = new Semantic(Variable, varname);
	MustBeTrue(pvar2 != NULL);
	List<Semantic * > *pargs = new List<Semantic * >;
	MustBeTrue(pargs != NULL);
	MustBeTrue(pargs->insertAtEnd(pvar1) == OK);
	MustBeTrue(pargs->insertAtEnd(pvar2) == OK);
	Semantic *peq = new Semantic(PredicateEqual, String("="), pargs, 2);
	MustBeTrue(peq != NULL);
	Semantic *pu = new Semantic(Universal, varname, peq);
	MustBeTrue(pu != NULL);
	MustBeTrue(extraaxioms.insertAtFront(pu) == OK);

	// add reflexive axioms for each function
	BinaryTreeIterator_InOrder<Symbol> symbolsIter(symbols);
	for ( ; !symbolsIter.done(); symbolsIter++)
	{
		// check if symbol is a function
		Symbol symbol = symbolsIter();
		if (symbol.getType() != Function)
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
		MustBeTrue(extraaxioms.insertAtFront(psem) == OK);
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
	Semantic *pvar1 = new Semantic(Variable, x);
	MustBeTrue(pvar1 != NULL);
	Semantic *pvar2 = new Semantic(Variable, x);
	MustBeTrue(pvar2 != NULL);

	// create a equal record
	List<Semantic * > *pargs = new List<Semantic * >;
	MustBeTrue(pargs != NULL);
	MustBeTrue(pargs->insertAtEnd(pvar1) == OK);
	MustBeTrue(pargs->insertAtEnd(pvar2) == OK);
	Semantic *peq = new Semantic(PredicateEqual, String("="), pargs, 2);
	MustBeTrue(peq != NULL);

	// create a universal record
	Semantic *pu = new Semantic(Universal, x, peq);
	MustBeTrue(pu != NULL);

	// save record, place at front of program.
	MustBeTrue(extraaxioms.insertAtFront(pu) == OK);

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
	Semantic *px = new Semantic(Variable, x);
	MustBeTrue(px != NULL);
	Semantic *py = new Semantic(Variable, y);
	MustBeTrue(py != NULL);
	List<Semantic * > *pargs = new List<Semantic * >;
	MustBeTrue(pargs != NULL);
	MustBeTrue(pargs->insertAtEnd(px) == OK);
	MustBeTrue(pargs->insertAtEnd(py) == OK);
	Semantic *peq = new Semantic(PredicateEqual, String("="), pargs, 2);
	MustBeTrue(peq != NULL);
	Semantic *pxney = new Semantic(Negation, NULL, peq);
	MustBeTrue(pxney != NULL);

	// create y==x record
	py = new Semantic(Variable, y);
	MustBeTrue(py != NULL);
	px = new Semantic(Variable, x);
	MustBeTrue(px != NULL);
	pargs = new List<Semantic * >;
	MustBeTrue(pargs != NULL);
	MustBeTrue(pargs->insertAtEnd(py) == OK);
	MustBeTrue(pargs->insertAtEnd(px) == OK);
	Semantic *pyeqx = new Semantic(PredicateEqual, String("="), pargs, 2);
	MustBeTrue(pyeqx != NULL);

	// create x!=y || y==x record
	Semantic *por = new Semantic(Or, pxney, pyeqx);
	MustBeTrue(por != NULL);

	// create a universal record
	Semantic *pu = new Semantic(Universal, y, por);
	MustBeTrue(pu != NULL);
	Semantic *pu2 = new Semantic(Universal, x, pu);
	MustBeTrue(pu2 != NULL);

	// save record, place a front of program.
	MustBeTrue(extraaxioms.insertAtFront(pu2) == OK);

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
	Semantic *px = new Semantic(Variable, x);
	MustBeTrue(px != NULL);
	Semantic *py = new Semantic(Variable, y);
	MustBeTrue(py != NULL);
	List<Semantic * > *pargs = new List<Semantic * >;
	MustBeTrue(pargs != NULL);
	MustBeTrue(pargs->insertAtEnd(px) == OK);
	MustBeTrue(pargs->insertAtEnd(py) == OK);
	Semantic *peq = new Semantic(PredicateEqual, String("="), pargs, 2);
	MustBeTrue(peq != NULL);
	Semantic *pxney = new Semantic(Negation, NULL, peq);
	MustBeTrue(pxney != NULL);

	// create y!=z record
	py = new Semantic(Variable, y);
	MustBeTrue(py != NULL);
	Semantic *pz = new Semantic(Variable, z);
	MustBeTrue(pz != NULL);
	pargs = new List<Semantic * >;
	MustBeTrue(pargs != NULL);
	MustBeTrue(pargs->insertAtEnd(py) == OK);
	MustBeTrue(pargs->insertAtEnd(pz) == OK);
	peq = new Semantic(PredicateEqual, String("="), pargs, 2);
	MustBeTrue(peq != NULL);
	Semantic *pynez = new Semantic(Negation, NULL, peq);
	MustBeTrue(pynez != NULL);

	// create x==z record
	px = new Semantic(Variable, x);
	MustBeTrue(px != NULL);
	pz = new Semantic(Variable, z);
	MustBeTrue(pz != NULL);
	pargs = new List<Semantic * >;
	MustBeTrue(pargs != NULL);
	MustBeTrue(pargs->insertAtEnd(px) == OK);
	MustBeTrue(pargs->insertAtEnd(pz) == OK);
	Semantic *pxeqz = new Semantic(PredicateEqual, String("="), pargs, 2);
	MustBeTrue(pxeqz != NULL);

	// create x!=y || y!=z || x==z record
	Semantic *por = new Semantic(Or, pxney, pynez);
	MustBeTrue(por != NULL);
	Semantic *por2 = new Semantic(Or, por, pxeqz);
	MustBeTrue(por2 != NULL);

	// create a universal record
	Semantic *pu = new Semantic(Universal, z, por2);
	MustBeTrue(pu != NULL);
	Semantic *pu2 = new Semantic(Universal, y, pu);
	MustBeTrue(pu2 != NULL);
	Semantic *pu3 = new Semantic(Universal, x, pu2);
	MustBeTrue(pu3 != NULL);

	// save record, place a front of program.
	MustBeTrue(extraaxioms.insertAtFront(pu3) == OK);

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
		psem = new Semantic(Universal, 
				argnames[iarg], psem);
		MustBeTrue(psem != NULL);
	}
	else if (iarg > argnum)
	{
		int arg;

		// create xj != x0 record
		Semantic *pxj = new Semantic(Variable, argnames[j]);
		MustBeTrue(pxj != NULL);
		Semantic *px0 = new Semantic(Variable, argnames[0]);
		MustBeTrue(px0 != NULL);
		List<Semantic * > *pargs = new List<Semantic *>;
		MustBeTrue(pargs != NULL);
		MustBeTrue(pargs->insertAtEnd(pxj) == OK);
		MustBeTrue(pargs->insertAtEnd(px0) == OK);
		Semantic *peq = new Semantic(PredicateEqual, 
				String("="), pargs, 2);
		MustBeTrue(peq != NULL);
		Semantic *pxjnex0 = 
			new Semantic(Negation, NULL, peq);
		MustBeTrue(pxjnex0 != NULL);

		// create ~P(...xj...) and 
		pargs = new List<Semantic * >;
		MustBeTrue(pargs != NULL);
		for (arg=1; arg<=argnum; arg++)
		{
			Semantic *px = 
				new Semantic(Variable, argnames[arg]);
			MustBeTrue(px != NULL);
			MustBeTrue(pargs->insertAtEnd(px) == OK);
		}
		Semantic *pp = new Semantic(PredicateFunction, 
					pname, pargs, argnum);
		MustBeTrue(pp != NULL);
		Semantic *pnegp = new Semantic(Negation, NULL, pp);
		MustBeTrue(pnegp != NULL);

		// create P(...x0...) and 
		pargs = new List<Semantic * >;
		MustBeTrue(pargs != NULL);
		for (arg=1; arg<=argnum; arg++)
		{
			Semantic *px = NULL;
			if (arg != j)
				px = new Semantic(Variable, 
					argnames[arg]);
			else
				px = new Semantic(Variable, 
					argnames[0]);
			MustBeTrue(px != NULL);
			MustBeTrue(pargs->insertAtEnd(px) == OK);
		}
		pp = new Semantic(PredicateFunction, 
					pname, pargs, argnum);
		MustBeTrue(pp != NULL);

		// create xj!=x0 || ~P(...xj...) || P(...x0...)
		Semantic *por = new Semantic(Or, pxjnex0, pnegp);
		MustBeTrue(por != NULL);
		psem = new Semantic(Or, por, pp);
		MustBeTrue(psem != NULL);
	}

	// all done
	return(OK);
}

int
addpredicateaxioms()
{
	// add axioms for every predicate
	BinaryTreeIterator_InOrder<Symbol> symbolsIter(symbols);
	for ( ; !symbolsIter.done(); symbolsIter++)
	{
		// check if symbol is a predicate
		Symbol symbol = symbolsIter();
		if (symbol.getType() != PredicateFunction)
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
			MustBeTrue(extraaxioms.insertAtFront(psem) == OK);
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
		psem = new Semantic(Universal, 
				argnames[iarg], psem);
		MustBeTrue(psem != NULL);
	}
	else if (iarg > argnum)
	{
		int arg;

		// create xj != x0 record
		Semantic *pxj = new Semantic(Variable, argnames[j]);
		MustBeTrue(pxj != NULL);
		Semantic *px0 = new Semantic(Variable, argnames[0]);
		MustBeTrue(px0 != NULL);
		List<Semantic * > *pargs = new List<Semantic *>;
		MustBeTrue(pargs != NULL);
		MustBeTrue(pargs->insertAtEnd(pxj) == OK);
		MustBeTrue(pargs->insertAtEnd(px0) == OK);
		Semantic *peq = new Semantic(PredicateEqual, 
				String("="), pargs, 2);
		MustBeTrue(peq != NULL);
		Semantic *pxjnex0 = 
			new Semantic(Negation, NULL, peq);
		MustBeTrue(pxjnex0 != NULL);

		// create f(...xj...) and 
		pargs = new List<Semantic * >;
		MustBeTrue(pargs != NULL);
		for (arg=1; arg<=argnum; arg++)
		{
			Semantic *px = 
				new Semantic(Variable, argnames[arg]);
			MustBeTrue(px != NULL);
			MustBeTrue(pargs->insertAtEnd(px) == OK);
		}
		Semantic *pf = new Semantic(Function, 
					fname, pargs, argnum);
		MustBeTrue(pf != NULL);

		// create f(...x0...) and 
		pargs = new List<Semantic * >;
		MustBeTrue(pargs != NULL);
		for (arg=1; arg<=argnum; arg++)
		{
			Semantic *px = NULL;
			if (arg != j)
				px = new Semantic(Variable, argnames[arg]);
			else
				px = new Semantic(Variable, argnames[0]);
			MustBeTrue(px != NULL);
			MustBeTrue(pargs->insertAtEnd(px) == OK);
		}
		Semantic *pf2 = new Semantic(Function, 
					fname, pargs, argnum);
		MustBeTrue(pf2 != NULL);

		// create equal record
		pargs = new List<Semantic * >;
		MustBeTrue(pargs != NULL);
		MustBeTrue(pargs->insertAtEnd(pf) == OK);
		MustBeTrue(pargs->insertAtEnd(pf2) == OK);
		peq = new Semantic(PredicateEqual, String("="), pargs, 2);
		MustBeTrue(peq != NULL);

		// create xj!=x0 || f(...xj...) = f(...x0...)
		psem = new Semantic(Or, pxjnex0, peq);
		MustBeTrue(psem != NULL);
	}

	// all done
	return(OK);
}
int
addfunctionaxioms()
{
	// add axioms for every function
	BinaryTreeIterator_InOrder<Symbol> symbolsIter(symbols);
	for ( ; !symbolsIter.done(); symbolsIter++)
	{
		// check if symbol is a function
		Symbol symbol = symbolsIter();
		if (symbol.getType() != Function)
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
			MustBeTrue(extraaxioms.insertAtFront(psem) == OK);
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

