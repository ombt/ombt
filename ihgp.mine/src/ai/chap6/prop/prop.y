%{
// required headers
#include <stdio.h>
#include <string.h>
#include <limits.h>

// other headers
#include "prop.h"

// externs
extern int yylineno;
extern int optind;
extern char *optarg;

// globals 
int allflag = 1;
int supportsetflag = 1;
int tautologyflag = 1;
int subsumeflag = 1;
int chasteflag = 1;
int dbflag = 1;
int maxdepth = 0;
int maxbreadth = INT_MAX;
int nextclause = 0;
int verbose = 0;

// constants
const int MaxDepth = 100;

// pointer to program tree
List<Semantic * > ptrees;
List<List<Semantic * > > programs;
List<Set_List<Clause> > clausesPrograms;
BinaryTree_AVL<Clause> clausesDB;

%}

%start start

%union {
	char string[32];
	Semantic *precord;
};

%token <string> NEWLINE
%token <string> COMMA
%token <string> SEMICOLON
%token <string> THEREFORE
%token <string> BICONDITIONAL
%token <string> IMPLICATION
%token <string> OR
%token <string> AND
%token <string> NEGATION
%token <string> LPAREN
%token <string> RPAREN
%token <string> IDENTIFIER
%token <string> QUIT
%token <string> TRUE
%token <string> FALSE

%type <precord> conclusion
%type <precord> expression
%type <precord> biconditional
%type <precord> implication
%type <precord> and
%type <precord> or
%type <precord> unary
%type <precord> primary

%%

start:
	/* empty */
	| start conclusion linesynch
	{
		fprintf(stdout, "argument syntax accepted ...\n");
	}
	| start expressionlist conclusion linesynch
	{
		fprintf(stdout, "argument syntax accepted ...\n");
	}
	| start newline
	| start error newline
	{
		fprintf(stdout, "syntax error detected ...\n");
		yyerrok;
	}
	| start QUIT linesynch
	{
		fprintf(stdout, "quitting ...\n");
		YYACCEPT;
	}
	| start QUIT newline
	{
		fprintf(stdout, "quitting ...\n");
		YYACCEPT;
	}
	;

expressionlist:
	separator_expressionlist linesynch
	| expressionlist separator_expressionlist linesynch
	;

separator_expressionlist:
	expression
	{
		ptrees.insertAtEnd($1);
	}
	| separator_expressionlist separator expression
	{
		ptrees.insertAtEnd($3);
	}
	;

expression:
	biconditional
	{
		$$ = $1;
	}
	;

biconditional:
	implication
	{
		$$ = $1;
	}
	| biconditional BICONDITIONAL implication
	{
		$$ = new Semantic(Expression::Biconditional, $1, $3);
		MustBeTrue($$ != NULL);
	}
	;

implication:
	or
	{
		$$ = $1;
	}
	| implication IMPLICATION or
	{
		$$ = new Semantic(Expression::Conditional, $1, $3);
		MustBeTrue($$ != NULL);
	}
	;
or:
	and
	{
		$$ = $1;
	}
	| or OR and
	{
		$$ = new Semantic(Expression::Or, $1, $3);
		MustBeTrue($$ != NULL);
	}
	;

and:
	unary
	{
		$$ = $1;
	}
	| and AND unary
	{
		$$ = new Semantic(Expression::And, $1, $3);
		MustBeTrue($$ != NULL);
	}
	;

unary:
	primary
	{
		$$ = $1;
	}
	| NEGATION unary
	{
		$$ = new Semantic(Expression::Negation, NULL, $2);
		MustBeTrue($$ != NULL);
	}
	;

primary:
	IDENTIFIER
	{
		$$ = new Semantic(Atomic::Variable, $1, "");
		MustBeTrue($$ != NULL);
	}
	| TRUE
	{
		$$ = new Semantic(Atomic::Constant, $1, "true");
		MustBeTrue($$ != NULL);
	}
	| FALSE
	{
		$$ = new Semantic(Atomic::Constant, $1, "false");
		MustBeTrue($$ != NULL);
	}
	| LPAREN expression RPAREN
	{
		$$ = $2;
	}
	;

separator:
	COMMA 
	;

conclusion:
	THEREFORE expression
	{
		// negate conclusion for resolution
		$$ = new Semantic(Expression::Negation, NULL, $2, 1);
		MustBeTrue($$ != NULL);
		ptrees.insertAtEnd($$);
	}
	;

linesynch:
	SEMICOLON newline
	;

newline:
	NEWLINE
	;
%%

// report syntax errors
void
yyerror(char *s)
{
	fprintf(stdout, " <<==\nlineno %04d: %s\n", yylineno, s);
}

// print program
void
dumpPtrees()
{
	// check if any program to print
	if (ptrees.isEmpty())
		return;

	// iterate over all parsetrees, and print them.
	int progno = 1;
	ListIterator<Semantic * > ptreesIter(ptrees);
	for (int lnno = 1; !ptreesIter.done(); ptreesIter++, lnno++)
	{
		if (lnno == 1)
			cout << endl << "PARSE TREE: " << progno++ << endl;
		cout << "Line " << lnno << ": ";
		if (ptreesIter()->getConclusion())
		{
			cout << "conclusion: ";
			lnno = 0;
		}
		else
			cout << "assumption: ";
		cout << *ptreesIter() << endl;
	}
	return;
}

void
dumpPrograms()
{
	// check if any program to print
	if (programs.isEmpty())
		return;

	// iterate over all programs, and print them.
	ListIterator<List<Semantic *> > programsIter(programs);
	for (int progno = 1; !programsIter.done(); programsIter++, progno++)
	{
		// iterate over all clauses in program
		cout << endl << "PROGRAM: " << progno << endl;
		ListIterator<Semantic *> programIter(programsIter());
		for (int lnno = 1; !programIter.done(); programIter++, lnno++)
		{
			cout << "Clause " << lnno << ": ";
			cout << *programIter() << endl;
		}
	}
	return;
}

void
dumpClausesPrograms()
{
	// check if any program to print
	if (clausesPrograms.isEmpty())
		return;

	// iterate over all programs, and print them.
	ListIterator<Set_List<Clause> > cprogsIter(clausesPrograms);
	for (int progno = 1; !cprogsIter.done(); cprogsIter++, progno++)
	{
		// iterate over all clauses in program
		cout << endl << "CLAUSE PROGRAM: " << progno << endl;
		Set_List_Iterator<Clause> clauseIter(cprogsIter());
		for (int lnno = 1; !clauseIter.done(); clauseIter++, lnno++)
		{
			cout << "Clause " << lnno << ": ";
			ClauseIterator atomIter(clauseIter());
			for (int first = 1; !atomIter.done(); atomIter++)
			{
				if (first)
					cout << atomIter();
				else
					cout << " || " << atomIter();
				first = 0;
			}
			cout << endl;
		}
	}
	return;
}

// remove tautologies from clauses programs
void
removeTautologies(Set_List<Clause> &clausesProgram)
{
	Clause clause;
	Set_List<Clause> newClausesProgram;
	Set_List_Iterator<Clause> clprogIter(clausesProgram);
	for ( ; !clprogIter.done(); clprogIter++)
	{
		clause = clprogIter();
		if (clause.isPartOfConclusion())
		{
			newClausesProgram.insert(clause);
		}
		else if (!tautologyflag || !clause.isATautology())
		{
			newClausesProgram.insert(clause);
		}
		else if (verbose)
		{
			cout << "TAUTOLOGY, removing " << clause << endl;
		}
	}
	clausesProgram = newClausesProgram;
}

// remove subsumed clauses from clauses programs
void
removeSubsumedClauses(Set_List<Clause> &clausesProgram)
{
	Set_List<Clause> deletedClauses;
	Set_List_Iterator<Clause> clprogIter(clausesProgram);
	Set_List_Iterator<Clause> clprogIter2(clausesProgram);
	for( ; !clprogIter.done(); clprogIter++)
	{
		for (clprogIter2.reset(); !clprogIter2.done(); clprogIter2++)
		{
			if (clprogIter().subsumes(clprogIter2()))
			{
				deletedClauses.insert(clprogIter2());
			}
		}
	}
	Set_List_Iterator<Clause> deletedIter(deletedClauses);
	for( ; !deletedIter.done(); deletedIter++)
	{
		clausesProgram.remove(deletedIter());
		if (verbose)
		{
			cout << "SUBSUMED, removing " << deletedIter() << endl;
		}
	}
}

// remove chaste clauses from clauses programs
void
removeChasteClauses(Set_List<Clause> &clausesProgram)
{
	// generate list of all atoms in program
	Clause allAtoms;
	Set_List_Iterator<Clause> clprogIter(clausesProgram);
	for ( ; !clprogIter.done(); clprogIter++)
	{
		allAtoms |= clprogIter();
	}

	// now make sure every atom in program has a complement
	Clause clause;
	Set_List<Clause> newClausesProgram;
	for (clprogIter.reset() ; !clprogIter.done(); clprogIter++)
	{
		// include ALL clauses from conclusions
		clause = clprogIter();
		if (clause.isPartOfConclusion())
		{
			newClausesProgram.insert(clause);
			continue;
		}

		// check for atoms without a complement 
		ClauseIterator clauseIter(clause);
		for (int includeit = 1 ; !clauseIter.done(); clauseIter++)
		{
			Atom negatedAtom = ~clauseIter();
			if (!allAtoms.isInClause(negatedAtom))
			{
				includeit = 0;
				break;
			}
				
		}
		if (includeit)
		{
			newClausesProgram.insert(clause);
		}
		else if (verbose)
		{
			cout << "CHASTE, removing " << clause << endl;
		}
	}
	clausesProgram = newClausesProgram;
	return;
}

// convert program to conjunctive normal form
int
convert2cnf()
{
	// list for holding clauses
	List<Semantic *> program;
	Set_List<Clause> clausesProgram;
	Clause clause;

	// loop over all expressions and convert
	ListIterator<Semantic * > ptreesIter(ptrees);
	for ( ; !ptreesIter.done(); ptreesIter++)
	{
		// convert biconditionals to implications
		if (ptreesIter()->removeBiconditionals() != OK)
			return(NOTOK);

		// convert implications to ORs and NOTs
		if (ptreesIter()->removeConditionals() != OK)
			return(NOTOK);

		// apply demorgan's laws to push negation down
		if (ptreesIter()->demorgans() != OK)
			return(NOTOK);

		// distribution law for ORs and ANDs.
		if (ptreesIter()->distribution() != OK)
			return(NOTOK);

		// get clauses in programs
		if (ptreesIter()->getClauses(program) != OK)
			return(NOTOK);
		if (ptreesIter()->getClauses(clausesProgram, clause) != OK)
			return(NOTOK);

		// check for end of program
		if (ptreesIter()->getConclusion())
		{
			// save current program
			programs.insertAtEnd(program);
			if (tautologyflag)
				removeTautologies(clausesProgram);
			if (chasteflag)
				removeChasteClauses(clausesProgram);
			if (subsumeflag)
				removeSubsumedClauses(clausesProgram);
			clausesPrograms.insertAtEnd(clausesProgram);

			// clear old program
			program.clear();
			clausesProgram.clear();
		}
	}

	// all done 
	return(OK);
}

// clean up program data
void
clearPrograms()
{
	// check if any program to print
	if (ptrees.isEmpty())
		return;

	// iterate over all parsetrees, and delete them
	ListIterator<Semantic * > ptreesIter(ptrees);
	for ( ; !ptreesIter.done(); ptreesIter++)
	{
		if (ptreesIter() != NULL)
			delete ptreesIter();
	}
	ptrees.clear();
	return;
}

// print atoms in clause
void
dumpClause(char *searchType, int currentDepth, int icl1, int icl2, 
		int nextClause, Clause &clause)
{
	cout << "st(" << searchType << "), ";
	cout << "cl(" << nextClause << "), ";
	cout << "con(" << clause.isPartOfConclusion() << "), ";
	cout << "res(" << icl1 << "," << icl2 << "), ";
	cout << "dep(" << currentDepth << ") = ";
	ClauseIterator atomIter(clause);
	for (int first = 1; !atomIter.done(); atomIter++)
	{
		if (first)
			cout << atomIter();
		else
			cout << " || " << atomIter();
		first = 0;
	}
	cout << endl;
}

// resolve two clauses if possible
int
resolveClauses(Array<BinaryTree_AVL<Clause> > &clausesArray, 
	Clause &clause1, Clause &clause2, int &clausesAdded, int currentDepth)
{
	// scan one clause, atom by atom, and search the other clause
	// for the negation of the atom.
	//
	Atom cl1atom;
	ClauseIterator cl1Iter(clause1);
	for ( ; !cl1Iter.done(); cl1Iter++)
	{
		// negated the current atom
		cl1atom = cl1Iter();
		Atom negatedAtom = ~cl1atom;

		// check if negated atom exists in clause2
		if (clause2.isInClause(negatedAtom))
		{
			// it exists !!! we can resolve these
			// two clauses together. resolve them
			// and check if the null string results.
			//
			// resolve clause1 and clause2
			//
			Clause tmpcl1 = clause1;
			Clause tmpcl2 = clause2;
			tmpcl1.remove(cl1atom);
			tmpcl2.remove(negatedAtom);
			Clause newcl = tmpcl1 | tmpcl2;

			// set conclusion bit if either clause is
			// part of a conclusion
			//
			if (tmpcl1.isPartOfConclusion() ||
			    tmpcl2.isPartOfConclusion())
			{
				newcl.setPartOfConclusion(1);
			}

			// set clause depth and number
			newcl.setDepth(currentDepth+1);
			newcl.setNumber(nextclause);

			// add new clause to clause array,
			// but first check if empty.
			//
			if (newcl.isEmpty())
			{
				// we have generated a contradiction.
				dumpClause("BREADTH", currentDepth+1, 
					clause1.getNumber(),
					clause2.getNumber(),
					nextclause, newcl);
				return(VALID);
			}

			// a new clause was generated. check if it 
			// already exists.
			//
			if ((!dbflag || !clausesDB.isInTree(newcl)) && 
			   ((!tautologyflag) || !newcl.isATautology()))
			{
				// insert into data base
				clausesDB.insert(newcl);

				// update flag to indicate that a new
				// clause was resolved. added it to 
				// clauses array.
				//
				clausesAdded = 1;
				if ((currentDepth+1) >= MaxDepth)
				{
					ERROR("(currentDepth+1) >= MaxDepth", 
						NOTOK);
					return(NOTOK);
				}
				clausesArray[currentDepth+1].insert(newcl);

				// print new clause
				dumpClause("BREADTH", currentDepth+1, 
					clause1.getNumber(),
					clause2.getNumber(),
					nextclause, newcl);
				nextclause++;
				break;
			}
		}
	}

	// all done
	return(OK);
}

// run a depth-first search using fewest-literals strategy at a depth
int 
linearSearch(Array<BinaryTree_AVL<Clause> > &clausesArray, int currentDepth)
{
#if 0
	// first, sort clauses by length
	List<LClause> lset;
	for (int idep = 0; idep <= currentDepth; idep++)
	{
		BinaryTree_AVL_Iterator_InOrder<Clause>
			clIter(clausesArray[idep]);
		for ( ; !clIter.done(); clIter++)
		{
			lset.insertByValue(LClause(clIter()));
		}
	}

	// get conclusions for initial set
	BinaryTree_AVL_Iterator_InOrder<Clause> initIter(clausesArray[0]);
	for( ; !initIter.done(); initIter++)
	{
		// skip if clause is not part of conclusion
		if (!initIter().isPartOfConclusion())
			continue;

		// get clause from conclusion
		Clause clause1(initIter();

		// try to resolve by length
		List_Iterator<LClause> lsetIter(lset);
		for ( ; !lsetIter.done(); lsetIter++)
		{
			// get clause to resolve
			Clause clause2(lsetIter());
		}
	}
#endif

#if 0
	// get start and end of clauses for the current depth
	int startClause = start[currentDepth];
	int endClause = end[currentDepth];

	// use the conclusions as the starting point for each linear search.
	for (int icl1 = startClause; icl1 <= endClause; icl1++)
	{
		// get starting clause for linear resolution
		Clause clause1(clausesArray[icl1]);
		int isaveicl1 = icl1;

		// skip clauses that are not descendant from the conclusion
		if (supportsetflag && (!clause1.isPartOfConclusion()))
			continue;

		// limit the number of cycles in the linear search
		for (int idepth = 1; idepth <= maxdepth; idepth++)
		{
			// loop over existing clauses
			int saveNextClause = nextClause;
			for (int icl2 = startClause; icl2 < saveNextClause; icl2++)
			{
				// current clause to check
				Clause clause2(clausesArray[icl2]);

				// check if clauses are resolvable
				ClauseIterator cl1Iter(clause1);
				for ( ; !cl1Iter.done(); cl1Iter++)
				{
					// negate the current atom
					Atom cl1Atom = cl1Iter();
					Atom negatedCl1Atom = ~cl1Atom;

					// does negated atom exist in clause2
					if (clause2.isInClause(negatedCl1Atom))
					{
						// resolve clause1 and clause2
						Clause tmpcl1 = clause1;
						Clause tmpcl2 = clause2;
						tmpcl1.remove(cl1Atom);
						tmpcl2.remove(negatedCl1Atom);
						Clause newcl = tmpcl1 | tmpcl2;

						// add new clause to clause array,
						// but first check if empty.
						if (newcl.isEmpty())
						{
							// we have generated a 
							// contradiction.
							dumpClause("DEPTH", 
								++currentDepth, 
								isaveicl1, icl2, 
								nextClause, newcl);
							return(VALID);
						}

						// a new clause was generated. 
						if (nextClause >= MaxClauses)
						{
							ERROR("nextClause >= MaxClauses", NOTOK);
							return(NOTOK);
						}
						clausesArray[nextClause] = newcl;

						// print new clause
						dumpClause("DEPTH", 
							++currentDepth, 
							isaveicl1, icl2, 
							nextClause, newcl);

						// point to next clause
						clause1 = newcl;
						isaveicl1 = nextClause++;
						break;
					}
				}
			}
		}
	}
#endif
	return(OK);
}

// resolve all clauses at the current depth
int
resolveDepth(Array<BinaryTree_AVL<Clause> > &clausesArray, 
	int &clausesAdded, int currentDepth)
{
	// scan over clauses and resolve, when possible
	BinaryTree_AVL_Iterator_InOrder<Clause>
		cdIter(clausesArray[currentDepth]);
	for ( ; !cdIter.done(); cdIter++)
	{
		// resolve with clause at lower or current level;
		for (int isd = 0; isd <= currentDepth; isd++)
		{
			BinaryTree_AVL_Iterator_InOrder<Clause>
				sdIter(clausesArray[isd]);
			for ( ; !sdIter.done(); sdIter++)
			{
				// only resolve clauses where at least
				// one of them is part of conclusion.
				// this is the set of support approach.
				//
				if (supportsetflag &&
				   (!cdIter().isPartOfConclusion() &&
				    !sdIter().isPartOfConclusion()))
					continue;

				// resolve the clauses
				int status = resolveClauses(clausesArray,
					cdIter(), sdIter(),
					clausesAdded, currentDepth);

				// check return value
				switch (status)
				{
				case OK:
				case CONTINUE:
					break;
				case NOTOK:
					ERROR("resolveClauses failed.", status);
					return(NOTOK);
				case VALID:
					cout << "Valid program." << endl;
					return(VALID);
				case INVALID:
					cout << "Invalid program." << endl;
					return(INVALID);
				default:
					MustBeTrue(0);
					return(NOTOK);
				}
			}
		}
	}
	return(OK);
}

// remove subsumed clauses at current depth
void
removeSubsumedClauses(
	Array<BinaryTree_AVL<Clause> > &clausesArray, int currentDepth)
{
	Set_List<Clause> deletedClauses;
	for (int idep = 0; idep <= currentDepth; idep++)
	{
		BinaryTree_AVL_Iterator_InOrder<Clause>
			clIter(clausesArray[idep]);
		for ( ; !clIter.done(); clIter++)
		{
			for (int idep2 = 0; idep2 <= currentDepth; idep2++)
			{
				BinaryTree_AVL_Iterator_InOrder<Clause>
					clIter2(clausesArray[idep2]);
				for ( ; !clIter2.done(); clIter2++)
				{
					if (clIter().subsumes(clIter2()))
					{
						deletedClauses.insert(clIter2());
					}
				}
			}
		}
	}
	Set_List_Iterator<Clause> deletedIter(deletedClauses);
	for( ; !deletedIter.done(); deletedIter++)
	{
		for (int idep = 0; idep <= currentDepth; idep++)
		{
			clausesArray[idep].remove(deletedIter());
			if (verbose)
			{
				cout << "SUBSUMED, removing " << deletedIter() << endl;
			}
		}
	}
}

// resolve programs, for now, use a breadth-first approach.
int
resolveProgram(const Set_List<Clause> &clauses)
{
	// create an array of trees to track clauses at each depth
	int currentDepth = 0;
	Array<BinaryTree_AVL<Clause> > clausesArray(MaxDepth);

	// copy over initial set of clauses
	Set_List_Iterator<Clause> clausesIter(clauses);
	for (nextclause = 0; !clausesIter.done(); nextclause++, clausesIter++)
	{
		// copy clause to clauses tree
		Clause clause(clausesIter());
		clause.setDepth(currentDepth);
		clause.setNumber(nextclause);
		clausesArray[currentDepth].insert(clause);

		// save in database for later checks for duplicates
		clausesDB.insert(clause);

		// print clause data
		dumpClause("BREADTH", currentDepth, -1, -1, nextclause, clause);
	}

	// loop until all resolutions are done
	for (int clausesAdded = 1; 
	    (currentDepth < maxbreadth && currentDepth < MaxDepth) && 
	     clausesAdded; currentDepth++)
	{
		// turn off flag. it will be reset if any new clauses
		// were added by the function resolveClauses().
		//
		clausesAdded = 0;

		// check if we should do a linear-search at each depth
		if (maxdepth > 0)
		{
			// perform linear search
			int status = 
				linearSearch(clausesArray, currentDepth);

			// check return value
			switch (status)
			{
			case OK:
			case CONTINUE:
				break;
			case NOTOK:
				ERROR("linearSearch failed.", status);
				return(NOTOK);
			case VALID:
				cout << "Valid program." << endl;
				return(OK);
			case INVALID:
				cout << "Invalid program." << endl;
				return(OK);
			default:
				MustBeTrue(0);
				return(NOTOK);
			}
		}

		// resolve all clauses for the current depth.
		int status = 
			resolveDepth(clausesArray, clausesAdded, currentDepth);

		// check return value
		switch (status)
		{
		case OK:
		case CONTINUE:
			break;
		case NOTOK:
			ERROR("resolveDepth failed.", status);
			return(NOTOK);
		case VALID:
			cout << "Valid program." << endl;
			return(OK);
		case INVALID:
			cout << "Invalid program." << endl;
			return(OK);
		default:
			MustBeTrue(0);
			return(NOTOK);
		}

		// remove subsumed clauses
		if (subsumeflag && clausesAdded)
			removeSubsumedClauses(clausesArray, currentDepth+1);
	}

	// invalid programs terminate without producing the empty clause
	cout << "Invalid program." << endl;

	// all done
	return(OK);
}

void
resolvePrograms()
{
	// check if any programs to resolve
	if (clausesPrograms.isEmpty())
	{
		cout << "No programs to resolve. Returning." << endl;
		return;
	}

	// make sure database is all clear
	clausesDB.clear();

	// iterate over all programs, and print them.
	ListIterator<Set_List<Clause> > cprogsIter(clausesPrograms);
	for (int progno = 1; !cprogsIter.done(); cprogsIter++, progno++)
	{
		cout << endl << "RESOLVING CLAUSE PROGRAM: " << progno << endl;
		if (resolveProgram(cprogsIter()) == OK)
			cout << "PROGRAM " << progno << " SUCCEEDED." << endl;
		else
			cout << "PROGRAM " << progno << " FAILED." << endl;
		clausesDB.clear();
	}
	return;
}


// usage message
void
usage(const char *cmd)
{
	cout << endl;
	cout << "usage: " << cmd << " [-?V] [-A [on|off]]" << endl;
	cout << "	[-C[on|off]] [-D[on|off] " << endl;
	cout << "	[-S[on|off]] [-T[on|off]] " << endl;
	cout << "	[-d maxlevel] [-b maxlevel] " << endl;
	cout << "	input_file ..." << endl;
	cout << endl;
	cout << "-V = turn verbose on," << endl;
	cout << "-A = turn ALL filtering on/off," << endl;
	cout << "-C = turn chaste clause filtering on/off," << endl;
	cout << "-D = turn database filtering on/off," << endl;
	cout << "-S = turn set-of-support filtering on/off," << endl;
	cout << "-T = turn tautology filtering on/off," << endl;
	cout << "-U = turn subsumption filtering on/off," << endl;
	cout << "-d = depth-first levels to search (not implemented)," << endl;
	cout << "-b = breadth-first levels to search," << endl;
	cout << endl;
	cout << "input file contains the clauses to prove." << endl;
	cout << "by default, the flags -A,-C,-D,-S,-T,-U are all on." << endl;
}

// main entry point
main(int argc, char **argv)
{
#ifdef YYDEBUG
	extern int yydebug;
	yydebug = 1;
#endif
	extern FILE *yyin;

	// get command line options
	for (int c = 0; (c = getopt(argc, argv, "?Vb:d:C:D:S:T:U:A:")) != EOF; )
	{
		switch (c)
		{
		case 'V':
			verbose = 1;
			break;

		case 'b':
			maxbreadth = atoi(optarg);
			break;

		case 'd':
			maxdepth = atoi(optarg);
			break;

		case 'A':
			if (String(optarg) == String("on"))
			{
				allflag = 1;
				supportsetflag = 1;
				tautologyflag = 1;
				subsumeflag = 1;
				chasteflag = 1;
				dbflag = 1;
			}
			else if (String(optarg) == String("off"))
			{
				allflag = 0;
				supportsetflag = 0;
				tautologyflag = 0;
				subsumeflag = 0;
				chasteflag = 0;
				dbflag = 0;
			}
			break;

		case 'C':
			if (String(optarg) == String("on"))
				chasteflag = 1;
			else if (String(optarg) == String("off"))
				chasteflag = 0;
			else
				chasteflag = 0;
			break;

		case 'D':
			if (String(optarg) == String("on"))
				dbflag = 1;
			else if (String(optarg) == String("off"))
				dbflag = 0;
			else
				dbflag = 0;
			break;

		case 'S':
			if (String(optarg) == String("on"))
				supportsetflag = 1;
			else if (String(optarg) == String("off"))
				supportsetflag = 0;
			else
				supportsetflag = 0;
			break;

		case 'T':
			if (String(optarg) == String("on"))
				tautologyflag = 1;
			else if (String(optarg) == String("off"))
				tautologyflag = 0;
			else
				tautologyflag = 0;
			break;

		case 'U':
			if (String(optarg) == String("on"))
				subsumeflag = 1;
			else if (String(optarg) == String("off"))
				subsumeflag = 0;
			else
				subsumeflag = 0;
			break;

		case '?':
			usage(argv[0]);
			return(0);

		default:
			fprintf(stdout, "invalid option.\n");
			usage(argv[0]);
			return(2);
		}
	}

	// turn off buffering
	setbuf(stdout, NULL);

	// scan files
	for (int arg = optind; arg < argc; arg++)
	{
		// tell user which file is scanned
		fprintf(stdout, "\nscanning file %s ...\n", 
			argv[arg]);

		// open file to read
		if ((yyin = fopen(argv[arg], "r")) == NULL)
		{
			fprintf(stdout, "unable to read %s.\n", 
				argv[arg]);
			continue;
		}

		// check syntax of program
		if (yyparse() != OK)
		{
			fprintf(stdout, "yyparse failed.\n");
			goto cleanup;
		}
		fprintf(stdout, "yyparse passed.\n");
		dumpPtrees();

		// convert to conjunctive normal form
		if (convert2cnf() != OK)
		{
			fprintf(stdout, "convert2cnf failed.\n");
			goto cleanup;
		}
		fprintf(stdout, "convert2cnf passed.\n");
		dumpPtrees();
		dumpPrograms();
		dumpClausesPrograms();

		// resolve all programs
		resolvePrograms();

		// clean up: delete syntax tree and close file.
cleanup:
		fclose(yyin);
		yyin = NULL;
		clearPrograms();
	}
	return(0);
}

