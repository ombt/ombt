// functions to prepare expresssions for running.

// headers
#include "folp.h"
#include "problem.h"
#include "miscutils.h"

// list of problems and subproblems
List<Problem * > problems;

// clear out semantic data structures
template <class DataType >
void
clearPointerList(List<DataType * > &pl)
{
	ListIterator<DataType * > pli(pl);
	for ( ; !pli.done(); pli++)
	{
		if (pli() != NULL)
			delete pli();
	}
	pl.clear();
	return;
}

// print out semantic data structures
template <class DataType >
void
printPointerList(const String &label, const List<DataType * > &pl)
{
	cout << endl << label << endl;
	ListIterator<DataType * > pli(pl);
	for (int p=1; !pli.done(); p++, pli++)
	{
		cout << p << ": " << *pli() << endl;
	}
	return;
}

// print data 
void
printOptions()
{
	cout << endl << "Program Options  ..." << endl;
	cout << options << endl;
	return;
}

void
printTreeTraversals()
{
	int p;

	cout << endl << "Post Order Tree Traversal for Premises ..." << endl;
	ListIterator<Semantic * > pi(premises);
	for (p=1; !pi.done(); p++, pi++)
	{
		cout << p << ": ";
		pi()->postOrder(cout);
		cout << endl;
	}

	cout << endl << "Post Order Tree Traversal for Extra Axioms ..." << endl;
	ListIterator<Semantic * > xi(extraaxioms);
	for (p=1; !xi.done(); p++, xi++)
	{
		cout << p << ": ";
		xi()->postOrder(cout);
		cout << endl;
	}

	cout << endl << "Post Order Tree Traversal for Conclusions ..." << endl;
	ListIterator<Semantic * > ci(conclusions);
	for (p=1; !ci.done(); p++, ci++)
	{
		cout << p << ": ";
		ci()->postOrder(cout);
		cout << endl;
	}
	return;
}

// clear all
void
clearAll()
{
	opts.clear();
	scope.clear();
	symbols.clear();
	clearPointerList(premises);
	clearPointerList(clpremises);
	clearPointerList(extraaxioms);
	clearPointerList(clextraaxioms);
	clearPointerList(conclusions);
	clearPointerList(clconclusions);
	clearPointerList(problems);
	return;
}

// write intermediate representations
int
writeIRs()
{
	ListIterator<Problem * > pi(problems);
	for (int i=1; !pi.done(); i++, pi++)
	{
		String pname = String("prob") + itoa(i);
		if (pi()->writeIR(options, dname, pname) != OK)
		{
			ERRORD("writing IR failed.", dname, errno);
			return(NOTOK);
		}
	}
	return(OK);
}

// main entry for preparing and executing program
int
execute()
{
	// print program options
	printOptions();
	if (postorder)
		printTreeTraversals();

	// add any extra axioms for equality
	if (addextraaxioms() != OK)
	{
		ERROR("addextraaxioms() failed.", NOTOK);
		return(NOTOK);
	}

	// print what we have now
	printPointerList(String("Premises Parse Tree ..."), premises);
	printPointerList(String("Extra Axioms Parse Tree ..."), extraaxioms);
	printPointerList(String("Conclusion Parse Tree ..."), conclusions);

	// convert premises and extra axioms to CNF
	if (convert2cnf(PremiseClause, premises, clpremises) != OK)
	{
		ERROR("convert2cnf() failed.", NOTOK);
		return(NOTOK);
	}
	if (convert2cnf(ExtraAxiomClause, extraaxioms, clextraaxioms) != OK)
	{
		ERROR("convert2cnf() failed.", NOTOK);
		return(NOTOK);
	}

	// print out CNF forms for premises and extra-axioms
	printPointerList(String("Premises CNF Parse Tree ..."), premises);
	printPointerList(String("Premises CNF Clauses ..."), clpremises);
	printPointerList(String("Extra Axioms CNF Parse Tree ..."), 
			extraaxioms);
	printPointerList(String("Extra Axioms CNF Clauses ..."), 
			clextraaxioms);

	// split conclusions/problems into subproblems
	ListIterator<Semantic * > ci(conclusions);
	for (int ip=1; !ci.done(); ip++, ci++)
	{
		// problem for each conclusion
		Problem *p = new Problem(clpremises, 
				clextraaxioms, ci(), reductionType);
		MustBeTrue(p != NULL);
		cout << endl << "new problem: " << ip << endl;
		cout << *p << endl;

		// split the problem
		if (p->split() != OK)
		{
			ERROR("split problem failed.", NOTOK);
			return(NOTOK);
		}
		cout << endl << "new split problem: " << ip << endl;
		cout << *p << endl;

		// negate split problems
		if (p->negate() != OK)
		{
			ERROR("negating split problem failed.", NOTOK);
			return(NOTOK);
		}
		cout << endl << "new negated split problem: " << ip << endl;
		cout << *p << endl;

		// convert to CNF
		if (p->convert2cnf() != OK)
		{
			ERROR("convert2cnf problem failed.", NOTOK);
			return(NOTOK);
		}
		cout << endl << "new negated split CNF problem: " << ip << endl;
		cout << *p << endl;

		// save problem
		if (problems.insertAtEnd(p) != OK)
		{
			ERROR("problem insert failed.", NOTOK)
			return(NOTOK);
		}
	}

	// write out intermediate forms
	if (writeIRs() != OK)
	{
		ERROR("unable to write IRs.", NOTOK)
		return(NOTOK);
	}

	// all done
	return(OK);
}


