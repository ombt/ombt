
# line 2 "prop.y"
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


# line 40 "prop.y"
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif
 {
	char string[32];
	Semantic *precord;
} YYSTYPE;
# define NEWLINE 257
# define COMMA 258
# define SEMICOLON 259
# define THEREFORE 260
# define BICONDITIONAL 261
# define IMPLICATION 262
# define OR 263
# define AND 264
# define NEGATION 265
# define LPAREN 266
# define RPAREN 267
# define IDENTIFIER 268
# define QUIT 269
# define TRUE 270
# define FALSE 271

#ifdef __STDC__
#include <stdlib.h>
#include <string.h>
#else
#include <malloc.h>
#include <memory.h>
#endif

#include <values.h>

#ifdef __cplusplus

#ifndef yyerror
	void yyerror(const char *);
#endif

#ifndef yylex
#ifdef __EXTERN_C__
	extern "C" { int yylex(void); }
#else
	int yylex(void);
#endif
#endif
	int yyparse(void);

#endif
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
YYSTYPE yylval;
YYSTYPE yyval;
typedef int yytabelem;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#if YYMAXDEPTH > 0
int yy_yys[YYMAXDEPTH], *yys = yy_yys;
YYSTYPE yy_yyv[YYMAXDEPTH], *yyv = yy_yyv;
#else	/* user does initial allocation */
int *yys;
YYSTYPE *yyv;
#endif
static int yymaxdepth = YYMAXDEPTH;
# define YYERRCODE 256

# line 225 "prop.y"


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

yytabelem yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 31
# define YYLAST 71
yytabelem yyact[]={

     5,     9,    17,    21,     7,    18,    47,    19,    20,    17,
    21,     7,    18,     6,    19,    20,    17,    21,    36,    18,
    35,    19,    20,    34,    33,    32,    23,     9,    23,    23,
     9,    15,    13,    14,    10,     8,    12,    22,    31,    25,
     3,     4,    29,    16,    27,    11,    30,    26,    28,    37,
     2,     1,     0,     0,    24,     0,    38,     0,     0,     0,
     0,     0,    40,    41,     0,    39,    42,    44,    46,    45,
    43 };
yytabelem yypact[]={

-10000000,  -256,  -231,  -249,-10000000,  -227,  -230,  -263,  -233,-10000000,
-10000000,  -237,  -239,  -243,  -246,-10000000,-10000000,  -263,-10000000,-10000000,
-10000000,  -263,-10000000,  -227,  -231,  -233,-10000000,-10000000,-10000000,-10000000,
-10000000,  -263,-10000000,  -263,  -263,  -263,  -263,-10000000,  -261,-10000000,
-10000000,-10000000,-10000000,  -239,  -243,  -246,-10000000,-10000000 };
yytabelem yypgo[]={

     0,    51,    50,    34,    45,    36,    33,    32,    31,    43,
    37,    40,    41,    35,    38 };
yytabelem yyr1[]={

     0,     1,     1,     1,     1,     1,     1,     1,    11,    11,
    13,    13,     3,     4,     4,     5,     5,     7,     7,     6,
     6,     8,     8,     9,     9,     9,     9,    14,     2,    10,
    12 };
yytabelem yyr2[]={

     0,     0,     7,     9,     4,     7,     7,     7,     4,     6,
     3,     7,     3,     3,     7,     3,     7,     3,     7,     3,
     7,     3,     5,     3,     3,     3,     7,     2,     5,     4,
     2 };
yytabelem yychk[]={

-10000000,    -1,    -2,   -11,   -12,   256,   269,   260,   -13,   257,
    -3,    -4,    -5,    -7,    -6,    -8,    -9,   265,   268,   270,
   271,   266,   -10,   259,    -2,   -13,   -12,   -10,   -12,    -3,
   -10,   -14,   258,   261,   262,   263,   264,    -8,    -3,   -12,
   -10,   -10,    -3,    -5,    -7,    -6,    -8,   267 };
yytabelem yydef[]={

     1,    -2,     0,     0,     4,     0,     0,     0,     0,    30,
    10,    12,    13,    15,    17,    19,    21,     0,    23,    24,
    25,     0,     2,     0,     0,     0,     5,     6,     7,    28,
     8,     0,    27,     0,     0,     0,     0,    22,     0,    29,
     3,     9,    11,    14,    16,    18,    20,    26 };
typedef struct
#ifdef __cplusplus
	yytoktype
#endif
{ char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] =
{
	"NEWLINE",	257,
	"COMMA",	258,
	"SEMICOLON",	259,
	"THEREFORE",	260,
	"BICONDITIONAL",	261,
	"IMPLICATION",	262,
	"OR",	263,
	"AND",	264,
	"NEGATION",	265,
	"LPAREN",	266,
	"RPAREN",	267,
	"IDENTIFIER",	268,
	"QUIT",	269,
	"TRUE",	270,
	"FALSE",	271,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"start : /* empty */",
	"start : start conclusion linesynch",
	"start : start expressionlist conclusion linesynch",
	"start : start newline",
	"start : start error newline",
	"start : start QUIT linesynch",
	"start : start QUIT newline",
	"expressionlist : separator_expressionlist linesynch",
	"expressionlist : expressionlist separator_expressionlist linesynch",
	"separator_expressionlist : expression",
	"separator_expressionlist : separator_expressionlist separator expression",
	"expression : biconditional",
	"biconditional : implication",
	"biconditional : biconditional BICONDITIONAL implication",
	"implication : or",
	"implication : implication IMPLICATION or",
	"or : and",
	"or : or OR and",
	"and : unary",
	"and : and AND unary",
	"unary : primary",
	"unary : NEGATION unary",
	"primary : IDENTIFIER",
	"primary : TRUE",
	"primary : FALSE",
	"primary : LPAREN expression RPAREN",
	"separator : COMMA",
	"conclusion : THEREFORE expression",
	"linesynch : SEMICOLON newline",
	"newline : NEWLINE",
};
#endif /* YYDEBUG */
# line	1 "/usr/ccs/bin/yaccpar"
/*
 * Copyright (c) 1993 by Sun Microsystems, Inc.
 */

#pragma ident	"@(#)yaccpar	6.12	93/06/07 SMI"

/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#define YYERROR		goto yyerrlab
#define YYACCEPT	return(0)
#define YYABORT		return(1)
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
	{\
		yyerror( "syntax error - cannot backup" );\
		goto yyerrlab;\
	}\
	yychar = newtoken;\
	yystate = *yyps;\
	yylval = newvalue;\
	goto yynewstate;\
}
#define YYRECOVERING()	(!!yyerrflag)
#define YYNEW(type)	malloc(sizeof(type) * yynewmax)
#define YYCOPY(to, from, type) \
	(type *) memcpy(to, (char *) from, yynewmax * sizeof(type))
#define YYENLARGE( from, type) \
	(type *) realloc((char *) from, yynewmax * sizeof(type))
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int yydebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-10000000)

/*
** global variables used by the parser
*/
YYSTYPE *yypv;			/* top of value stack */
int *yyps;			/* top of state stack */

int yystate;			/* current state */
int yytmp;			/* extra var (lasts between blocks) */

int yynerrs;			/* number of errors */
int yyerrflag;			/* error recovery flag */
int yychar;			/* current input token number */



#ifdef YYNMBCHARS
#define YYLEX()		yycvtok(yylex())
/*
** yycvtok - return a token if i is a wchar_t value that exceeds 255.
**	If i<255, i itself is the token.  If i>255 but the neither 
**	of the 30th or 31st bit is on, i is already a token.
*/
#if defined(__STDC__) || defined(__cplusplus)
int yycvtok(int i)
#else
int yycvtok(i) int i;
#endif
{
	int first = 0;
	int last = YYNMBCHARS - 1;
	int mid;
	wchar_t j;

	if(i&0x60000000){/*Must convert to a token. */
		if( yymbchars[last].character < i ){
			return i;/*Giving up*/
		}
		while ((last>=first)&&(first>=0)) {/*Binary search loop*/
			mid = (first+last)/2;
			j = yymbchars[mid].character;
			if( j==i ){/*Found*/ 
				return yymbchars[mid].tvalue;
			}else if( j<i ){
				first = mid + 1;
			}else{
				last = mid -1;
			}
		}
		/*No entry in the table.*/
		return i;/* Giving up.*/
	}else{/* i is already a token. */
		return i;
	}
}
#else/*!YYNMBCHARS*/
#define YYLEX()		yylex()
#endif/*!YYNMBCHARS*/

/*
** yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
#if defined(__STDC__) || defined(__cplusplus)
int yyparse(void)
#else
int yyparse()
#endif
{
	register YYSTYPE *yypvt;	/* top of value stack for $vars */

#if defined(__cplusplus) || defined(lint)
/*
	hacks to please C++ and lint - goto's inside switch should never be
	executed; yypvt is set to 0 to avoid "used before set" warning.
*/
	static int __yaccpar_lint_hack__ = 0;
	switch (__yaccpar_lint_hack__)
	{
		case 1: goto yyerrlab;
		case 2: goto yynewstate;
	}
	yypvt = 0;
#endif

	/*
	** Initialize externals - yyparse may be called more than once
	*/
	yypv = &yyv[-1];
	yyps = &yys[-1];
	yystate = 0;
	yytmp = 0;
	yynerrs = 0;
	yyerrflag = 0;
	yychar = -1;

#if YYMAXDEPTH <= 0
	if (yymaxdepth <= 0)
	{
		if ((yymaxdepth = YYEXPAND(0)) <= 0)
		{
			yyerror("yacc initialization error");
			YYABORT;
		}
	}
#endif

	{
		register YYSTYPE *yy_pv;	/* top of value stack */
		register int *yy_ps;		/* top of state stack */
		register int yy_state;		/* current state */
		register int  yy_n;		/* internal state number info */
	goto yystack;	/* moved from 6 lines above to here to please C++ */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	yynewstate:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;
		goto yy_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	yystack:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	yy_stack:
		/*
		** put a state and value onto the stacks
		*/
#if YYDEBUG
		/*
		** if debugging, look up token value in list of value vs.
		** name pairs.  0 and negative (-1) are special values.
		** Note: linear search is used since time is not a real
		** consideration while debugging.
		*/
		if ( yydebug )
		{
			register int yy_i;

			printf( "State %d, token ", yy_state );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++yy_ps >= &yys[ yymaxdepth ] )	/* room on stack? */
		{
			/*
			** reallocate and recover.  Note that pointers
			** have to be reset, or bad things will happen
			*/
			int yyps_index = (yy_ps - yys);
			int yypv_index = (yy_pv - yyv);
			int yypvt_index = (yypvt - yyv);
			int yynewmax;
#ifdef YYEXPAND
			yynewmax = YYEXPAND(yymaxdepth);
#else
			yynewmax = 2 * yymaxdepth;	/* double table size */
			if (yymaxdepth == YYMAXDEPTH)	/* first time growth */
			{
				char *newyys = (char *)YYNEW(int);
				char *newyyv = (char *)YYNEW(YYSTYPE);
				if (newyys != 0 && newyyv != 0)
				{
					yys = YYCOPY(newyys, yys, int);
					yyv = YYCOPY(newyyv, yyv, YYSTYPE);
				}
				else
					yynewmax = 0;	/* failed */
			}
			else				/* not first time */
			{
				yys = YYENLARGE(yys, int);
				yyv = YYENLARGE(yyv, YYSTYPE);
				if (yys == 0 || yyv == 0)
					yynewmax = 0;	/* failed */
			}
#endif
			if (yynewmax <= yymaxdepth)	/* tables not expanded */
			{
				yyerror( "yacc stack overflow" );
				YYABORT;
			}
			yymaxdepth = yynewmax;

			yy_ps = yys + yyps_index;
			yy_pv = yyv + yypv_index;
			yypvt = yyv + yypvt_index;
		}
		*yy_ps = yy_state;
		*++yy_pv = yyval;

		/*
		** we have a new state - find out what to do
		*/
	yy_newstate:
		if ( ( yy_n = yypact[ yy_state ] ) <= YYFLAG )
			goto yydefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		yytmp = yychar < 0;
#endif
		if ( ( yychar < 0 ) && ( ( yychar = YYLEX() ) < 0 ) )
			yychar = 0;		/* reached EOF */
#if YYDEBUG
		if ( yydebug && yytmp )
		{
			register int yy_i;

			printf( "Received token " );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( yy_n += yychar ) < 0 ) || ( yy_n >= YYLAST ) )
			goto yydefault;
		if ( yychk[ yy_n = yyact[ yy_n ] ] == yychar )	/*valid shift*/
		{
			yychar = -1;
			yyval = yylval;
			yy_state = yy_n;
			if ( yyerrflag > 0 )
				yyerrflag--;
			goto yy_stack;
		}

	yydefault:
		if ( ( yy_n = yydef[ yy_state ] ) == -2 )
		{
#if YYDEBUG
			yytmp = yychar < 0;
#endif
			if ( ( yychar < 0 ) && ( ( yychar = YYLEX() ) < 0 ) )
				yychar = 0;		/* reached EOF */
#if YYDEBUG
			if ( yydebug && yytmp )
			{
				register int yy_i;

				printf( "Received token " );
				if ( yychar == 0 )
					printf( "end-of-file\n" );
				else if ( yychar < 0 )
					printf( "-none-\n" );
				else
				{
					for ( yy_i = 0;
						yytoks[yy_i].t_val >= 0;
						yy_i++ )
					{
						if ( yytoks[yy_i].t_val
							== yychar )
						{
							break;
						}
					}
					printf( "%s\n", yytoks[yy_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register int *yyxi = yyexca;

				while ( ( *yyxi != -1 ) ||
					( yyxi[1] != yy_state ) )
				{
					yyxi += 2;
				}
				while ( ( *(yyxi += 2) >= 0 ) &&
					( *yyxi != yychar ) )
					;
				if ( ( yy_n = yyxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( yy_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( yyerrflag )
			{
			case 0:		/* new error */
				yyerror( "syntax error" );
				goto skip_init;
			yyerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				yy_pv = yypv;
				yy_ps = yyps;
				yy_state = yystate;
			skip_init:
				yynerrs++;
				/* FALLTHRU */
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				yyerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( yy_ps >= yys )
				{
					yy_n = yypact[ *yy_ps ] + YYERRCODE;
					if ( yy_n >= 0 && yy_n < YYLAST &&
						yychk[yyact[yy_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						yy_state = yyact[ yy_n ];
						goto yy_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( yydebug )
						printf( _POP_, *yy_ps,
							yy_ps[-1] );
#	undef _POP_
#endif
					yy_ps--;
					yy_pv--;
				}
				/*
				** there is no state on stack with "error" as
				** a valid shift.  give up.
				*/
				YYABORT;
			case 3:		/* no shift yet; eat a token */
#if YYDEBUG
				/*
				** if debugging, look up token in list of
				** pairs.  0 and negative shouldn't occur,
				** but since timing doesn't matter when
				** debugging, it doesn't hurt to leave the
				** tests here.
				*/
				if ( yydebug )
				{
					register int yy_i;

					printf( "Error recovery discards " );
					if ( yychar == 0 )
						printf( "token end-of-file\n" );
					else if ( yychar < 0 )
						printf( "token -none-\n" );
					else
					{
						for ( yy_i = 0;
							yytoks[yy_i].t_val >= 0;
							yy_i++ )
						{
							if ( yytoks[yy_i].t_val
								== yychar )
							{
								break;
							}
						}
						printf( "token %s\n",
							yytoks[yy_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( yychar == 0 )	/* reached EOF. quit */
					YYABORT;
				yychar = -1;
				goto yy_newstate;
			}
		}/* end if ( yy_n == 0 ) */
		/*
		** reduction by production yy_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( yydebug )
			printf( "Reduce by (%d) \"%s\"\n",
				yy_n, yyreds[ yy_n ] );
#endif
		yytmp = yy_n;			/* value to switch over */
		yypvt = yy_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using yy_state here as temporary
		** register variable, but why not, if it works...
		** If yyr2[ yy_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto yy_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int yy_len = yyr2[ yy_n ];

			if ( !( yy_len & 01 ) )
			{
				yy_len >>= 1;
				yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
				yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
					*( yy_ps -= yy_len ) + 1;
				if ( yy_state >= YYLAST ||
					yychk[ yy_state =
					yyact[ yy_state ] ] != -yy_n )
				{
					yy_state = yyact[ yypgo[ yy_n ] ];
				}
				goto yy_stack;
			}
			yy_len >>= 1;
			yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
			yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
				*( yy_ps -= yy_len ) + 1;
			if ( yy_state >= YYLAST ||
				yychk[ yy_state = yyact[ yy_state ] ] != -yy_n )
			{
				yy_state = yyact[ yypgo[ yy_n ] ];
			}
		}
					/* save until reenter driver code */
		yystate = yy_state;
		yyps = yy_ps;
		yypv = yy_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( yytmp )
	{
		
case 2:
# line 75 "prop.y"
{
		fprintf(stdout, "argument syntax accepted ...\n");
	} break;
case 3:
# line 79 "prop.y"
{
		fprintf(stdout, "argument syntax accepted ...\n");
	} break;
case 5:
# line 84 "prop.y"
{
		fprintf(stdout, "syntax error detected ...\n");
		yyerrok;
	} break;
case 6:
# line 89 "prop.y"
{
		fprintf(stdout, "quitting ...\n");
		YYACCEPT;
	} break;
case 7:
# line 94 "prop.y"
{
		fprintf(stdout, "quitting ...\n");
		YYACCEPT;
	} break;
case 10:
# line 107 "prop.y"
{
		ptrees.insertAtEnd(yypvt[-0].precord);
	} break;
case 11:
# line 111 "prop.y"
{
		ptrees.insertAtEnd(yypvt[-0].precord);
	} break;
case 12:
# line 118 "prop.y"
{
		yyval.precord = yypvt[-0].precord;
	} break;
case 13:
# line 125 "prop.y"
{
		yyval.precord = yypvt[-0].precord;
	} break;
case 14:
# line 129 "prop.y"
{
		yyval.precord = new Semantic(Expression::Biconditional, yypvt[-2].precord, yypvt[-0].precord);
		MustBeTrue(yyval.precord != NULL);
	} break;
case 15:
# line 137 "prop.y"
{
		yyval.precord = yypvt[-0].precord;
	} break;
case 16:
# line 141 "prop.y"
{
		yyval.precord = new Semantic(Expression::Conditional, yypvt[-2].precord, yypvt[-0].precord);
		MustBeTrue(yyval.precord != NULL);
	} break;
case 17:
# line 148 "prop.y"
{
		yyval.precord = yypvt[-0].precord;
	} break;
case 18:
# line 152 "prop.y"
{
		yyval.precord = new Semantic(Expression::Or, yypvt[-2].precord, yypvt[-0].precord);
		MustBeTrue(yyval.precord != NULL);
	} break;
case 19:
# line 160 "prop.y"
{
		yyval.precord = yypvt[-0].precord;
	} break;
case 20:
# line 164 "prop.y"
{
		yyval.precord = new Semantic(Expression::And, yypvt[-2].precord, yypvt[-0].precord);
		MustBeTrue(yyval.precord != NULL);
	} break;
case 21:
# line 172 "prop.y"
{
		yyval.precord = yypvt[-0].precord;
	} break;
case 22:
# line 176 "prop.y"
{
		yyval.precord = new Semantic(Expression::Negation, NULL, yypvt[-0].precord);
		MustBeTrue(yyval.precord != NULL);
	} break;
case 23:
# line 184 "prop.y"
{
		yyval.precord = new Semantic(Atomic::Variable, yypvt[-0].string, "");
		MustBeTrue(yyval.precord != NULL);
	} break;
case 24:
# line 189 "prop.y"
{
		yyval.precord = new Semantic(Atomic::Constant, yypvt[-0].string, "true");
		MustBeTrue(yyval.precord != NULL);
	} break;
case 25:
# line 194 "prop.y"
{
		yyval.precord = new Semantic(Atomic::Constant, yypvt[-0].string, "false");
		MustBeTrue(yyval.precord != NULL);
	} break;
case 26:
# line 199 "prop.y"
{
		yyval.precord = yypvt[-1].precord;
	} break;
case 28:
# line 210 "prop.y"
{
		// negate conclusion for resolution
		yyval.precord = new Semantic(Expression::Negation, NULL, yypvt[-0].precord, 1);
		MustBeTrue(yyval.precord != NULL);
		ptrees.insertAtEnd(yyval.precord);
	} break;
# line	532 "/usr/ccs/bin/yaccpar"
	}
	goto yystack;		/* reset registers in driver code */
}

