%{
// required headers
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

// other headers
#include "pred.h"

// local definitions
#define IfDebug(DebugLevel, DebugMsg, StuffToDump) \
	if (DebugLevel <= currentDebugLevel) \
	{ \
		cout << DebugMsg << endl; \
		cout << StuffToDump << endl; \
	}

#define IfDebugCall(DebugLevel, Called) \
	if (DebugLevel <= currentDebugLevel) \
	{ \
		Called; \
	}

// phases for semantic processing
#define PHASE1 1
#define PHASE2 (PHASE1+1)
#define PHASE3 (PHASE2+1)
#define PHASE4 (PHASE3+1)
#define PHASE5 (PHASE4+1)
#define PHASE6 (PHASE5+1)
#define PHASE7 (PHASE6+1)
#define PHASE8 (PHASE7+1)
#define PHASE9 (PHASE8+1)
#define PHASE10 (PHASE9+1)
#define MAXPHASE PHASE10

// externs
extern int yylineno;
extern int optind;
extern char *optarg;
extern int yylex();
extern void yyerror(const char *s);

// symbol table
BinaryTree_AVL<Symbol> symbols;

// program data
List<Symbol> scope;
List<Semantic * > ptrees;
List<List<Semantic * > > programs;
List<List<IRClause> > clausesPrograms;

// other globals
int currentDebugLevel = 0;
int maxPhase = MAXPHASE;
char errormsg[BUFSIZ];
int syntaxErrors = 0;

%}

%start start

%union {
	char cstring[32];
	Semantic *psemantic;
	List<Semantic *> *parguments;
};

%token <cstring> AND
%token <cstring> BICONDITIONAL
%token <cstring> COMMA
%token <cstring> FALSE
%token <cstring> IDENTIFIER
%token <cstring> IMPLICATION
%token <cstring> LEFTEXISTENTIAL
%token <cstring> LEFTUNIVERSAL
%token <cstring> LPAREN
%token <cstring> NEGATION
%token <cstring> NEWLINE
%token <cstring> NUMBER
%token <cstring> OR
%token <cstring> PIDENTIFIER
%token <cstring> QUIT
%token <cstring> RIGHTEXISTENTIAL
%token <cstring> RIGHTUNIVERSAL
%token <cstring> RPAREN
%token <cstring> SEMICOLON
%token <cstring> STRING
%token <cstring> THEREFORE
%token <cstring> TRUE

%type <parguments> arglist
%type <psemantic> arg
%type <psemantic> constant
%type <psemantic> function
%type <psemantic> term
%type <psemantic> variable
%type <psemantic> and
%type <psemantic> atom
%type <psemantic> biconditional
%type <psemantic> conclusion
%type <psemantic> expression
%type <psemantic> implication
%type <psemantic> or
%type <psemantic> predicate
%type <psemantic> unary
%type <cstring> universal
%type <cstring> existential

%%

start:
	/* empty */
	| start conclusion linesynch
	{
		fprintf(stdout, "argument syntax accepted ...\n");
		scope.clear();
		symbols.clear();
	}
	| start expressionlist conclusion linesynch
	{
		fprintf(stdout, "argument syntax accepted ...\n");
		scope.clear();
		symbols.clear();
	}
	| start newline
	{
		scope.clear();
		symbols.clear();
	}
	| start error newline
	{
		fprintf(stdout, "syntax error detected ...\n");
		scope.clear();
		symbols.clear();
		yyerrok;
	}
	| start QUIT linesynch
	{
		fprintf(stdout, "quitting ...\n");
		scope.clear();
		symbols.clear();
		YYACCEPT;
	}
	| start QUIT newline
	{
		fprintf(stdout, "quitting ...\n");
		scope.clear();
		symbols.clear();
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
		/* save expression parse tree */
		ptrees.insertAtEnd($1);
	}
	| separator_expressionlist separator expression
	{
		/* save expression parse tree */
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
		/* create a biconditional record */
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
		/* create a conditional record */
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
		/* create an OR record */
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
		/* create an AND record */
		$$ = new Semantic(Expression::And, $1, $3);
		MustBeTrue($$ != NULL);
	}
	;

unary:
	atom
	{
		$$ = $1;
	}
	| NEGATION unary
	{
		/* create an NEGATION record */
		$$ = new Semantic(Expression::Negation, NULL, $2);
		MustBeTrue($$ != NULL);
	}
	| universal unary
	{
		/* create an NEGATION record */
		$$ = new Semantic(Expression::Universal, String($1), $2);
		MustBeTrue($$ != NULL);

		/* end scope */
		Symbol uvarsym;
		MustBeTrue(scope.removeAtFront(uvarsym) == OK);
		IfDebug(5, "ending a universal scope ...", scope);

		/* check if variable was used */
		if (uvarsym.getUsed() <= 0)
		{
			sprintf(errormsg, 
				"variable %s not used.\n", 
				(char *)uvarsym.getName());
			yyerror(errormsg);
			YYERROR;
		}
	}
	| existential unary
	{
		/* create an NEGATION record */
		$$ = new Semantic(Expression::Existential, $1, $2);
		MustBeTrue($$ != NULL);

		/* end scope */
		Symbol uvarsym;
		MustBeTrue(scope.removeAtFront(uvarsym) == OK);
		IfDebug(5, "ending an existential scope ...", scope);

		/* check if variable was used */
		if (uvarsym.getUsed() <= 0)
		{
			sprintf(errormsg, 
				"variable %s not used.\n", 
				(char *)uvarsym.getName());
			yyerror(errormsg);
			YYERROR;
		}
	}
	;

universal:
	LEFTUNIVERSAL IDENTIFIER RIGHTUNIVERSAL
	{
		/* return string name */
		strcpy($$, $2);

		/* check if symbol exists in global symbol table */
		Symbol newqvsym(String($2), Symbol::UniversalVariable);
		if (symbols.retrieve(newqvsym) == OK)
		{
			/* symbol was found */
			if (newqvsym.getType() != Symbol::Variable)
			{
				/* type of symbol changed */
				sprintf(errormsg, 
					"type changed for %s.\n", $1);
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into symbol table */
			symbols.insert(Symbol(String($2), 
				Symbol::Variable));
		}

		/* insert name into scope */
		scope.insertAtFront(Symbol(String($2), 
			Symbol::UniversalVariable));
		IfDebug(5, "starting a new universal scope ...", scope);
	}
	;

existential:
	LEFTEXISTENTIAL IDENTIFIER RIGHTEXISTENTIAL
	{
		/* return string name */
		strcpy($$, $2);

		/* check if symbol exists in global symbol table */
		Symbol newqvsym(String($2), Symbol::ExistentialVariable);
		if (symbols.retrieve(newqvsym) == OK)
		{
			/* symbol was found */
			if (newqvsym.getType() != Symbol::Variable)
			{
				/* type of symbol changed */
				sprintf(errormsg, 
					"type changed for %s.\n", $1);
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into symbol table */
			symbols.insert(Symbol(String($2), 
				Symbol::Variable));
		}

		/* insert name into scope */
		scope.insertAtFront(Symbol(String($2),
			Symbol::ExistentialVariable));
		IfDebug(5, "starting a new existential scope ...", scope);
	}
	;

atom:
	predicate
	{
		$$ = $1;
	}
	| TRUE
	{
		/* create a predicate logical record */
		$$ = new Semantic(Predicate::LogicalConstant, 
				String(""), String("True"));
		MustBeTrue($$ != NULL);
	}
	| FALSE
	{
		/* create a logical constant record */
		$$ = new Semantic(Predicate::LogicalConstant, 
				String(""), String("False"));
		MustBeTrue($$ != NULL);
	}
	| LPAREN expression RPAREN
	{
		$$ = $2;
	}
	;

predicate:
	PIDENTIFIER
	{
		/* check if symbol exists */
		Symbol newsym(String($1), Symbol::PredicateConstant);
		if (symbols.retrieve(newsym) == OK)
		{
			/* symbol was found */
			if (newsym.getType() != Symbol::PredicateConstant)
			{
				/* type of symbol changed */
				sprintf(errormsg, 
					"type changed for %s.\n", $1);
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into table */
			symbols.insert(newsym);
		}

		/* create a predicate constant record */
		$$ = new Semantic(Predicate::Constant, String($1), String(""));
		MustBeTrue($$ != NULL);
	}
	| PIDENTIFIER LPAREN arglist RPAREN
	{
		/* count the number of arguments */
		int nargs;
		ListIterator<Semantic * > argsIter(*$3);
		for (nargs = 0; !argsIter.done(); argsIter++, nargs++);

		/* check if symbol exists */
		Symbol newsym(String($1), Symbol::PredicateFunction, nargs);
		if (symbols.retrieve(newsym) == OK)
		{
			/* symbol was found, type should not change */
			if (newsym.getType() != Symbol::PredicateFunction)
			{
				sprintf(errormsg, "type changed for %s.\n", $1);
				yyerror(errormsg);
				YYERROR;
			}
			if (newsym.getArgs() != nargs)
			{
				char buffer[BUFSIZ];
				sprintf(buffer, "number of args changed for %s.\n", $1);
				strcpy(errormsg, buffer);
				sprintf(buffer, "old number of args = %d.\n", newsym.getArgs());
				strcat(errormsg, buffer);
				sprintf(buffer, "new number of args = %d.\n", nargs);
				strcat(errormsg, buffer);
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into table */
			symbols.insert(newsym);
		}

		/* create a predicate record */
		$$ = new Semantic(Predicate::Function, String($1), $3, nargs);
		MustBeTrue($$ != NULL);
	}
	;

arglist:
	arg
	{
		/* create an argument list */
		$$ = new List<Semantic *>;
		MustBeTrue($$ != NULL);

		/* insert an argument in the list */
		$$->insertAtEnd($1);
	}
	| arglist separator arg
	{
		/* insert an argument in the list */
		$$->insertAtEnd($3);
	}
	;

arg:
	term
	{
		$$ = $1;
	}
	;

term:
	constant
	{
		$$ = $1;
	}
	| variable
	{
		$$ = $1;
	}
	| function
	{
		$$ = $1;
	}
	;

constant:
	STRING
	{
		/* create a string record */
		$$ = new Semantic(Argument::QuotedString, String($1));
		MustBeTrue($$ != NULL);
	}
	| NUMBER
	{
		/* create a number record */
		$$ = new Semantic(Argument::Number, String($1));
		MustBeTrue($$ != NULL);
	}
	;

variable:
	IDENTIFIER
	{
		/* search if name is in the scope */
		Symbol varsym(String($1));
		if (scope.retrieve(varsym) == OK)
		{
			/* increment usage */
			IfDebug(5, "varsym incremented (before) ...", varsym);
			varsym++;
			IfDebug(5, "varsym incremented (after) ...", varsym);
			IfDebug(5, "scope updated (before) ...", scope);
			scope.update(varsym);
			IfDebug(5, "scope updated (after) ...", scope);

			/* we have a variable, verify type did not change */
			Symbol sym(String($1));
			if (symbols.retrieve(sym) != OK)
			{
				sprintf(errormsg, 
					"variable %s not in symbol table.\n", 
					$1);
				yyerror(errormsg);
				YYERROR;
			}
			if (sym.getType() != Symbol::Variable)
			{
				sprintf(errormsg, 
					"symbol %s not a variable.\n", 
					$1);
				yyerror(errormsg);
				YYERROR;
			}

			/* create a variable semantic record */
			$$ = new Semantic(Argument::Variable, String($1));
			MustBeTrue($$ != NULL);
		}
		else
		{
			/* we have a constant, verify type did not change */
			Symbol sym(String($1));
			if (symbols.retrieve(sym) == OK)
			{
				if (sym.getType() != Symbol::Constant)
				{
					sprintf(errormsg, 
						"symbol %s not a constant.\n", 
						$1);
					yyerror(errormsg);
					YYERROR;
				}
			}
			else
			{
				/* insert new constant symbol entry */
				symbols.insert(Symbol(String($1), 
					Symbol::Constant));
			}

			/* create a constant semantic record */
			$$ = new Semantic(Argument::Constant, String($1));
			MustBeTrue($$ != NULL);
		}
	}
	;

function:
	IDENTIFIER LPAREN arglist RPAREN
	{
		/* get number of arguments */
		int nargs;
		ListIterator<Semantic * > argsIter(*$3);
		for (nargs = 0; !argsIter.done(); argsIter++, nargs++);

		/* check if symbol exists */
		Symbol newsym(String($1), Symbol::Function, nargs);
		if (symbols.retrieve(newsym) == OK)
		{
			/* symbol was found, type should not change */
			if (newsym.getType() != Symbol::Function)
			{
				sprintf(errormsg, "type changed for %s.\n", $1);
				yyerror(errormsg);
				YYERROR;
			}
			if (newsym.getArgs() != nargs)
			{
				char buffer[BUFSIZ];
				sprintf(buffer, "number of args changed for %s.\n", $1);
				strcpy(errormsg, buffer);
				sprintf(buffer, "old number of args = %d.\n", newsym.getArgs());
				strcat(errormsg, buffer);
				sprintf(buffer, "new number of args = %d.\n", nargs);
				strcat(errormsg, buffer);
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into table */
			symbols.insert(newsym);
		}

		/* create a function record */
		$$ = new Semantic(Argument::Function, String($1), $3, nargs);
		MustBeTrue($$ != NULL);
	}
	;
	
separator:
	COMMA 
	;

conclusion:
	THEREFORE expression
	{
		/* negate conclusion */
		$$ = new Semantic(Expression::Negation, NULL, $2, 1);
		MustBeTrue($$ != NULL);

		/* save expression */
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
	syntaxErrors++;
	fprintf(stdout, " <<==\nlineno %04d: %s\n", yylineno, s);
}

// usage message
void
usage(const char *cmd)
{
	cout << "usage: " << cmd << " [-?d:p:] [-o outfile] input_file" << endl;
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
dumpIRClausesPrograms()
{
	// check if any program to print
	if (clausesPrograms.isEmpty())
		return;

	// iterate over all programs, and print them.
	ListIterator<List<IRClause> > cprogsIter(clausesPrograms);
	for (int progno = 1; !cprogsIter.done(); cprogsIter++, progno++)
	{
		// iterate over all clauses in program
		cout << endl << "CLAUSE PROGRAM: " << progno << endl;
		ListIterator<IRClause> clauseIter(cprogsIter());
		for (int lnno = 1; !clauseIter.done(); clauseIter++, lnno++)
		{
			cout << "Clause " << lnno << ": " << clauseIter();
			if (clauseIter().getPartOfConclusion())
				cout << "(part of conclusion)";
			cout << endl;
		}
	}
	return;
}

void
dumpIRClausesPrograms(const char *ofile, ofstream &outfile, int &emptyFile)
{
	// check if any program to print
	if (clausesPrograms.isEmpty())
		return;

	// iterate over all programs, and print them.
	emptyFile = 0;
	outfile << "file " << ofile << ":" << endl;
	ListIterator<List<IRClause> > cprogsIter(clausesPrograms);
	for (int progno = 1; !cprogsIter.done(); cprogsIter++, progno++)
	{
		// iterate over all clauses in program
		outfile << "program " << progno << ":" << endl;
		ListIterator<IRClause> clauseIter(cprogsIter());
		for (int lnno = 1; !clauseIter.done(); clauseIter++, lnno++)
		{
			if (clauseIter().getPartOfConclusion())
				outfile << "conclusion " << lnno << ": " << clauseIter();
			else
				outfile << "assumption " << lnno << ": " << clauseIter();
			outfile << endl;
		}
	}
	return;
}

// convert statements to conjunctive normal form
int
convert2cnf()
{
	// list for holding individual programs
	List<Semantic *> program;
	List<IRClause> clausesProgram;

	// loop over all expressions and convert
	ListIterator<Semantic * > ptreesIter(ptrees);
	for ( ; !ptreesIter.done(); ptreesIter++)
	{
		// initial tree
		IfDebug(2, "starting with ...", *ptreesIter());

		// convert biconditionals to implications
		if (maxPhase < PHASE1) continue;
		if (ptreesIter()->removeBiconditionals() != OK)
			return(NOTOK);
		IfDebug(2, "removeBiconditionals (after) ...", *ptreesIter());

		// convert implications to ORs and NOTs
		if (maxPhase < PHASE2) continue;
		if (ptreesIter()->removeConditionals() != OK)
			return(NOTOK);
		IfDebug(2, "removeConditionals (after) ...", *ptreesIter());

		// apply demorgan's laws to push negation down
		if (maxPhase < PHASE3) continue;
		if (ptreesIter()->demorgans() != OK)
			return(NOTOK);
		IfDebug(2, "demorgans (after) ...", *ptreesIter());

		// rename all variables to unique names
		if (maxPhase < PHASE4) continue;
		if (ptreesIter()->renameVariables() != OK)
			return(NOTOK);
		IfDebug(2, "renameVariables (after) ...", *ptreesIter());

		// remove existential quantifier with skolem functions
		if (maxPhase < PHASE5) continue;
		if (ptreesIter()->skolemize() != OK)
			return(NOTOK);
		IfDebug(2, "skolemize (after) ...", *ptreesIter());

		// remove universal quantifiers
		if (maxPhase < PHASE6) continue;
		if (ptreesIter()->removeUniversals() != OK)
			return(NOTOK);
		IfDebug(2, "removeUniversals (after) ...", *ptreesIter());

		// distribution law for ORs and ANDs.
		if (maxPhase < PHASE7) continue;
		if (ptreesIter()->distribution() != OK)
			return(NOTOK);
		IfDebug(2, "distribution (after) ...", *ptreesIter());

		// get clauses in programs
		if (maxPhase < PHASE8) continue;
		if (ptreesIter()->getClauses(program) != OK)
			return(NOTOK);
		IfDebug(2, "getClauses (after) ...", *ptreesIter());
		IfDebug(2, "getClauses (after) ...", program);

		// check for end of program
		if (ptreesIter()->getConclusion())
		{
			// rename variables in clauses
			ListIterator<Semantic *> programIter(program);
			for (int ic=1 ; !programIter.done(); ic++,programIter++)
			{
				// rename all variables to unique names
				if (maxPhase < PHASE9) continue;
				IfDebug(2, "renameAgain (before) ...", 
					*programIter());
				if (programIter()->renameAgain() != OK)
					return(NOTOK);
				IfDebug(2, "renameAgain (after) ...", 
					*programIter());

				// convert semantic clauses to a 
				// list representation
				if (maxPhase < PHASE10) continue;
				if (programIter()->getClausesAsLists(
					clausesProgram) != OK)
					return(NOTOK);
				IfDebug(2, "getClausesAsLists (after) ...", 
					*programIter());
				IfDebug(2, "getClausesAsLists (after) ...", 
					clausesProgram);
			}

			// save current program
			programs.insertAtEnd(program);
			clausesPrograms.insertAtEnd(clausesProgram);

			// clear old program
			program.clear();
			clausesProgram.clear();
		}
	}

	// all done 
	return(OK);
}

// delete everything
void
clearPtrees()
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

void
clearPrograms()
{
	// check if any program to print
	if (programs.isEmpty())
		return;
 
	// iterate over all programs, and delete them
	ListIterator<List<Semantic * > > programsIter(programs);
	for ( ; !programsIter.done(); programsIter++)
	{
		ListIterator<Semantic * > programIter(programsIter());
		for ( ; !programIter.done(); programIter++)
		{
			if (programIter() != NULL)
				delete programIter();
		}
	}
	programs.clear();
	return;
}

void
clearIRClausesPrograms()
{
	clausesPrograms.clear();
	return;
}


// main entry point
main(int argc, char **argv)
{
#ifdef YYDEBUG
	extern int yydebug;
	yydebug = 1;
#endif
	extern FILE *yyin;
	char *outfilename = NULL;
	int emptyFile = 1;

	// get command line options
	for (int c = 0; (c = getopt(argc, argv, "?d:p:o:")) != EOF; )
	{
		switch (c)
		{
		case 'd':
			// debug level: 0, 1, 2, 5 (as of now).
			currentDebugLevel = atoi(optarg);
			break;

		case 'p':
			// what conversion phases to run
			maxPhase = atoi(optarg);
			break;

		case 'o':
			// output file for intermediate representations
			outfilename = optarg;
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

	// check if an output file was given. also, do not write out
	// a file if a full conversion is NOT done.
	//
	ofstream outfile;
	if (outfilename != NULL && maxPhase >= MAXPHASE)
	{
		outfile.open(outfilename);
		if (!outfile)
		{
			fprintf(stdout, "unable to open %s.\n", optarg);
			return(2);
		}
	}

	// scan files
	for (int arg = optind; arg < argc; arg++)
	{
		// tell user which file is scanned
		fprintf(stdout, "\nscanning file %s ...\n", argv[arg]);

		// open file to read
		if ((yyin = fopen(argv[arg], "r")) == NULL)
		{
			fprintf(stdout, "unable to read %s.\n", argv[arg]);
			continue;
		}

		// check syntax of program
		if (yyparse() != OK || syntaxErrors > 0)
		{
			fprintf(stdout, "yyparse failed. [errors=%d]\n",
				syntaxErrors);
		}
		else
		{
			// tell user that program passed
			fprintf(stdout, "yyparse passed.\n");

			// print out parse tree
			IfDebugCall(1, dumpPtrees());

			// convert to conjunctive normal form
			if (convert2cnf() != OK)
			{
				fprintf(stdout, "convert2cnf failed.\n");
			}
			else
			{
				fprintf(stdout, "convert2cnf passed.\n");
				IfDebugCall(1, dumpPtrees());
				IfDebugCall(1, dumpPrograms());
				if (maxPhase >= MAXPHASE)
					dumpIRClausesPrograms();
				if (outfilename != NULL)
					dumpIRClausesPrograms(argv[arg], outfile, emptyFile);
			}
		}

		// clear parser data
		clearPtrees();
		clearPrograms();
		clearIRClausesPrograms();
		scope.clear();
		symbols.clear();
		syntaxErrors = 0;

		// close file.
		fclose(yyin);
		yyin = NULL;
	}

	// close output file, if any.
	if (outfilename != NULL)
		outfile.close();

	// check if anything was written
	if (emptyFile)
		unlink(outfilename);

	// all done
	return(0);
}

