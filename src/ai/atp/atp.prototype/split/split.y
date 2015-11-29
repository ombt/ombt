%{
// required headers
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <iostream.h>
#include <fstream.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

// other headers
#include "split.h"

// definitions
#define ATPTEMP "atp_temp"
#define ATPPPSUFFIX ".i"

// externs
extern int yylineno;
extern int optind;
extern char *optarg;

// symbol table
BinaryTree_AVL<Symbol> symbols;

// program data
List<Symbol> scope;
List<Semantic * > ptrees;
List<List<Semantic * > > programs;
List<String> filelist;
List<String> strategylist;

// other globals
char errormsg[BUFSIZ];
int syntaxErrors = 0;
int echoflag = 0;
int fileflag = 0;
int ofileflag = 0;
int verbose = 0;
int problem = 1;
char filename[BUFSIZ];
char tmpdir[BUFSIZ];
char problemdir[BUFSIZ];

%}

%start start

%union {
	int flag;
	char cstring[32];
	Semantic *psemantic;
	List<Semantic *> *parguments;
};

%token <cstring> AND
%token <cstring> ARGUMENT
%token <cstring> BICONDITIONAL
%token <cstring> COMMA
%token <cstring> FALSE
%token <cstring> IDENTIFIER
%token <cstring> EQUAL
%token <cstring> NOTEQUAL
%token <cstring> IMPLICATION
%token <cstring> LEFTEXISTENTIAL
%token <cstring> LEFTUNIVERSAL
%token <cstring> LPAREN
%token <cstring> NEGATION
%token <cstring> NUMBER
%token <cstring> OR
%token <cstring> PIDENTIFIER
%token <cstring> QUIT
%token <cstring> RIGHTEXISTENTIAL
%token <cstring> RIGHTUNIVERSAL
%token <cstring> RPAREN
%token <cstring> SEMICOLON
%token <cstring> SOS
%token <cstring> STRING
%token <cstring> THEREFORE
%token <cstring> TRUE
%token <cstring> LBRACE
%token <cstring> RBRACE

%type <cstring> existential
%type <flag> sos_option
%type <cstring> universal
%type <parguments> arglist
%type <psemantic> and
%type <psemantic> arg
%type <psemantic> atom
%type <psemantic> biconditional
%type <psemantic> conclusion
%type <psemantic> constant
%type <psemantic> expression
%type <psemantic> function
%type <psemantic> implication
%type <psemantic> or
%type <psemantic> predicate
%type <psemantic> term
%type <psemantic> unary
%type <psemantic> variable

%%

start:
	/* empty */
	| start ARGUMENT LBRACE conclusionlist RBRACE
	{
		/* syntax has been accepted */
		fprintf(stdout, "argument syntax accepted ...\n");

		/* print out results */
		if (verbose)
		{
			ListIterator<Semantic * > ptreesIter(ptrees);
			for ( ; !ptreesIter.done(); ptreesIter++)
			{
				cout << *ptreesIter() << ";" << endl;
			}
		}

		// create directories for each problem
		if (createSubProblems() != OK)
		{
			ERROR("createSubProblems failed.", errno);
		}

		/* clear out semantic structures */
		scope.clear();
		symbols.clear();
		ptrees.clear();
	}
	| start ARGUMENT LBRACE expressionlist conclusionlist RBRACE
	{
		/* syntax has been accepted */
		fprintf(stdout, "argument syntax accepted ...\n");

		/* print out results */
		if (verbose)
		{
			ListIterator<Semantic * > ptreesIter(ptrees);
			for ( ; !ptreesIter.done(); ptreesIter++)
			{
				cout << *ptreesIter() << ";" << endl;
			}
		}

		// create directories for each problem
		if (createSubProblems() != OK)
		{
			ERROR("createProblems failed.", errno);
		}

		/* clear out semantic structures */
		scope.clear();
		symbols.clear();
		ptrees.clear();
	}
	| start error RBRACE
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
	;

conclusionlist:
	sos_option conclusion linesynch
	{
		/* is this a set of support */
		$2->setSOS($1);

		/* save expression */
		ptrees.insertAtEnd($2);
	}
	;
	| conclusionlist sos_option conclusion linesynch
	{
		/* is this a set of support */
		$3->setSOS($2);

		/* save expression */
		ptrees.insertAtEnd($3);
	}
	;

expressionlist:
	separator_expressionlist linesynch
	| expressionlist separator_expressionlist linesynch
	;

separator_expressionlist:
	sos_option expression
	{
		/* is this a set of support */
		$2->setSOS($1);

		/* save expression */
		ptrees.insertAtEnd($2);
	}
	| separator_expressionlist separator sos_option expression
	{
		/* is this a set of support */
		$4->setSOS($3);

		/* save expression */
		ptrees.insertAtEnd($4);
	}
	;

sos_option:
	/* empty */
	{
		$$ = 0;
	}
	| SOS
	{
		$$ = 1;
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
		int nargs = $3->getCount();

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
	| term EQUAL term
	{
		/* create a list */
		List<Semantic *> *pargs = new List<Semantic *>;
		MustBeTrue(pargs != NULL);

		/* insert LHS and RHS into list */
		MustBeTrue(pargs->insertAtEnd($1) == OK);
		MustBeTrue(pargs->insertAtEnd($3) == OK);

		/* TWO arguments */
		int nargs = pargs->getCount();
		MustBeTrue(nargs == 2);

		/* create a new semantic record */
		$$ = new Semantic(Predicate::Equal, String($2), pargs, nargs);
		MustBeTrue($$ != NULL);
	}
	| term NOTEQUAL term
	{
		/* create a list */
		List<Semantic *> *pargs = new List<Semantic *>;
		MustBeTrue(pargs != NULL);

		/* insert LHS and RHS into list */
		MustBeTrue(pargs->insertAtEnd($1) == OK);
		MustBeTrue(pargs->insertAtEnd($3) == OK);

		/* TWO arguments */
		int nargs = pargs->getCount();
		MustBeTrue(nargs == 2);

		/* create a new semantic record */
		$$ = new Semantic(Predicate::Equal, String($2), pargs, nargs);
		MustBeTrue($$ != NULL);

		/* create an NEGATION record */
		$$ = new Semantic(Expression::Negation, NULL, $$);
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
		$$ = new Semantic(Term::QuotedString, String($1));
		MustBeTrue($$ != NULL);
	}
	| NUMBER
	{
		/* create a number record */
		$$ = new Semantic(Term::Number, String($1));
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
			varsym++;
			scope.update(varsym);

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
			$$ = new Semantic(Term::Variable, String($1));
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
			$$ = new Semantic(Term::Constant, String($1));
			MustBeTrue($$ != NULL);
		}
	}
	;

function:
	IDENTIFIER LPAREN arglist RPAREN
	{
		/* get number of arguments */
		int nargs = $3->getCount();

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
		$$ = new Semantic(Term::Function, String($1), $3, nargs);
		MustBeTrue($$ != NULL);
	}
	;
	
separator:
	COMMA 
	;

conclusion:
	THEREFORE expression
	{
		/* set conclusion bit to true */
		$$ = $2;
		$$->setConclusion(1);
	}
	;

linesynch:
	SEMICOLON 
	;
%%

// report syntax errors
void
yyerror(const char *s)
{
	syntaxErrors++;
	fprintf(stdout, " <<==\nlineno %04d: %s\n", yylineno, s);
}

// usage message
void
usage(const char *cmd)
{
	cout << "usage: " << cmd << " [-?EV] [-S strategy, ...] problem_dir ... " << endl;
	cout << "or" << endl;
	cout << "usage: " << cmd << " [-?EV] [-S strategy, ...] -F filename ... " << endl;
	cout << "or" << endl;
	cout << "usage: " << cmd << " [-?EV] [-S strategy, ...] -O original_filename ... " << endl;
	cout << endl;
	cout << "\t\t-S - a choice of strategy as how the problem is split." << endl;
	cout << "\t\tthe default is all. the choices are: schaum," << endl;
	cout << "\t\tbledsoe, none, all." << endl;
}

// expand directory to file names
void
expandfiles(const char *parg)
{
	// check input parameters
	MustBeTrue(parg != NULL && *parg != 0);

	// remove old files
	filelist.clear();

	// check if original file name option is on
	if (!ofileflag)
	{
		// just save the file name
		MustBeTrue(filelist.insertAtEnd(String(parg)) == OK);
		return;
	}

	// generate a list of directories for the file
	for (int iprob = 1; ; iprob++)
	{	
		// generate a file name
		char dname[BUFSIZ];
		sprintf(dname, "%s/%s.d/axioms_conclusion%d",
			ATPTEMP, parg, iprob);

		// check if directory exists
		if (access(dname, R_OK|W_OK|X_OK) != 0)
		{
			// all done, no more problem directories
			break;
		}

		// save directory name
		MustBeTrue(filelist.insertAtEnd(String(dname)) == OK);
	}

	// all done
	return;
}

// figure out the file name
void
setfname(char *parg)
{
	// check input parameters
	MustBeTrue(parg != NULL && *parg != 0);

	// check if file name option is on
	if (fileflag)
	{
		// file names are given, get directory
		strcpy(problemdir, parg);
		char *pdir = findlast(problemdir, '/');
		if (pdir != NULL)
			*pdir = 0;
		else
			strcpy(problemdir, ".");
		strcpy(filename, parg);
	}
	else
	{
		// a directory was given, use standard file name
		strcpy(problemdir, parg);
		sprintf(filename, "%s/axioms_conclusion", problemdir);
	}

	// all done
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

	// default list of strategies to try
	MustBeTrue(strategylist.insertAtEnd(String("none")) == OK);
	MustBeTrue(strategylist.insertAtEnd(String("schaum")) == OK);
	MustBeTrue(strategylist.insertAtEnd(String("bledsoe")) == OK);

	// get command line options
	for (int c = 0; (c = getopt(argc, argv, "?S:OEVF")) != EOF; )
	{
		switch (c)
		{
		case 'E':
			// echo to stdout
			echoflag = 1;
			break;

		case 'F':
			// file names were given
			ofileflag = 0;
			fileflag = 1;
			break;

		case 'O':
			// original file names were given
			fileflag = 0;
			ofileflag = 1;
			break;

		case 'V':
			// verbose
			verbose = 1;
			break;

		case 'S':
		{
			// list of strategies to try
			strategylist.clear();
			String string(optarg);
			StringTokens token(string, String(","));
			for ( ; !token.done(); token++)
			{
				// check if all strategies are requested
				if (token() == String("all"))
				{
					strategylist.clear();
					MustBeTrue(strategylist.insertAtEnd(
						String("none")) == OK);
					MustBeTrue(strategylist.insertAtEnd(
						String("schaum")) == OK);
					MustBeTrue(strategylist.insertAtEnd(
						String("bledsoe")) == OK);
					break;
				}
				MustBeTrue(strategylist.insertAtEnd(token()) == OK);
			}
		}
			break;

		case '?':
			// help message
			usage(argv[0]);
			return(0);

		default:
			ERROR("invalid option.", EINVAL);
			usage(argv[0]);
			return(2);
		}
	}

	// turn off buffering
	setbuf(stdout, NULL);

	// scan directories for problem files
	for (int arg = optind; arg < argc; arg++)
	{
		// tell user which file is scanned
		if (fileflag)
			cout << "scanning file ... " << argv[arg] << endl;
		else
			cout << "scanning directory ... " << argv[arg] << endl;

		// get list of files to scan
		expandfiles(argv[arg]);

		// scan list of files
		ListIterator<String> fliter(filelist);
		for ( ; !fliter.done(); fliter++)
		{
			// set file name and temp directory
			setfname(fliter());

			// reset syntax errors counter for this file
			syntaxErrors = 0;

			// open file to read
			if ((yyin = fopen(filename, "r")) == NULL)
			{
				ERRORD("unable to read.", filename, errno);
				continue;
			}

			// check syntax of program
			if (yyparse() != OK || syntaxErrors > 0)
			{
				ERRORD("yyparse failed.", syntaxErrors, errno);
			}

			// close file.
			fclose(yyin);
			yyin = NULL;
		}
	}

	// all done
	return(0);
}

