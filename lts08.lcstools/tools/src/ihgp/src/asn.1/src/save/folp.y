%{
// headers
#include "folp.h"

// externs
extern int yylineno;
extern int optind;
extern char *optarg;

// globals
char errormsg[BUFSIZ];
int syntaxErrors = 0;
int echo = 1;
int equality = 0;
int paramodulation = 0;
int postorder = 0;
String reductionType("none");
String tmpdir("pp_tmp");
String suffix("i");;
String fname;
String ppfname;
String dname;

// symbol table and semantic data
List<Symbol> scope;
BinaryTree<Symbol> symbols;
List<Semantic * > premises;
List<Semantic * > conclusions;
List<Semantic * > extraaxioms;
List<Semantic * > clpremises;
List<Semantic * > clconclusions;
List<Semantic * > clextraaxioms;
List<String> opts;

// options for prover
Map<String, String> options;

%}

%start start

%union {
	char cstring[32];
	Semantic *psemantic;
	List<Semantic * > *parguments;
};

%token <cstring> AND
%token <cstring> ARGUMENT
%token <cstring> BICONDITIONAL
%token <cstring> COLON
%token <cstring> COMMA
%token <cstring> EQUAL
%token <cstring> EXISTENTIAL
%token <cstring> FALSE
%token <cstring> IDENTIFIER
%token <cstring> IMPLICATION
%token <cstring> LBRACE
%token <cstring> LEFTEXISTENTIAL
%token <cstring> LEFTUNIVERSAL
%token <cstring> LPAREN
%token <cstring> MINUS
%token <cstring> NEGATION
%token <cstring> NOTEQUAL
%token <cstring> NUMBER
%token <cstring> OPTION
%token <cstring> OR
%token <cstring> PIDENTIFIER
%token <cstring> QUERY
%token <cstring> QUIT
%token <cstring> RBRACE
%token <cstring> RIGHTEXISTENTIAL
%token <cstring> RIGHTUNIVERSAL
%token <cstring> RPAREN
%token <cstring> SEMICOLON
%token <cstring> SOS
%token <cstring> STRING
%token <cstring> THEREFORE
%token <cstring> TRUE
%token <cstring> UNIVERSAL

%type <cstring> copt
%type <cstring> existential
%type <cstring> universal
%type <cstring> xopt
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
	{
		clearAll();
	}
	| start argument expressionlist conclusionlist RBRACE
	{
		cout << endl << "Argument syntax ACCEPTED ..." << endl;
		execute();
		clearAll();
	}
	| start error RBRACE
	{
		cout << endl << "Argument syntax ERROR ..." << endl;
		clearAll();
		yyerrok;
	}
	| start QUIT linesynch
	{
		cout << endl << "Quitting ..." << endl;
		clearAll();
		YYACCEPT;
	}
	| start options optionslist RBRACE
	;

options:
	OPTION LBRACE;
	;

optionslist:
	/* empty */
	| optionslist option
	;

option:
	IDENTIFIER EQUAL STRING linesynch
	{
		options[$1] = $3;
	}
	| IDENTIFIER EQUAL NUMBER linesynch
	{
		options[$1] = $3;
	}
	;

argument:
	ARGUMENT LBRACE
	{
		equality = 0;
	}
	;

conclusionlist:
	conclusion linesynch
	{
		/* save expression in conclusion list */
		MustBeTrue(conclusions.insertAtEnd($1) == OK);
	}
	| conclusionlist conclusion linesynch
	{
		/* save expression in conclusion list */
		MustBeTrue(conclusions.insertAtEnd($2) == OK);
	}
	;

conclusion:
	THEREFORE coptionlist expression
	{
		$3->setConclusion(1);
		$3->setQuery(0);
		$3->setOptions(opts);
		$$ = $3;
		opts.clear();
	}
	| QUERY coptionlist expression
	{
		$3->setConclusion(0);
		$3->setQuery(1);
		$3->setOptions(opts);
		$$ = $3;
		opts.clear();
	}
	;

coptionlist:
	/* empty */
	{
		opts.clear();
	}
	| coptionlist copt
	{
		MustBeTrue(opts.insert($2) == OK);
	}
	;

copt:
	SOS
	{
		strcpy($$, $1);
	}
	;

expressionlist:
	/* empty */
	| expressionlist xoptionlist expression linesynch
	{
		/* set options, SOS, conclusion or query */
		$3->setOptions(opts);
		opts.clear();

		/* save expression in conclusion list */
		MustBeTrue(premises.insertAtEnd($3) == OK);
	}
	;

xoptionlist:
	/* empty */
	{
		opts.clear();
	}
	| xoptionlist xopt
	{
		MustBeTrue(opts.insert($2) == OK);
	}
	;

xopt:
	SOS
	{
		strcpy($$, $1);
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
		$$ = new Semantic(Biconditional, $1, $3);
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
		$$ = new Semantic(Conditional, $1, $3);
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
		$$ = new Semantic(Or, $1, $3);
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
		$$ = new Semantic(And, $1, $3);
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
		$$ = new Semantic(Negation, NULL, $2);
		MustBeTrue($$ != NULL);
	}
	| universal unary
	{
		/* create a UNIVERSAL record */
		$$ = new Semantic(Universal, String($1), $2);
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
		/* create an EXISTENTIAL record */
		$$ = new Semantic(Existential, String($1), $2);
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
		Symbol newqvsym(String($2));
		if (symbols.retrieve(newqvsym) == OK)
		{
			/* symbol was found */
			if (newqvsym.getType() != Variable)
			{
				/* type of symbol changed */
				sprintf(errormsg, 
					"type changed for %s.\n", $2);
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into symbol table */
			MustBeTrue(symbols.insert(Symbol(String($2), 
				Variable)) == OK);
		}

		/* insert name into scope */
		MustBeTrue(scope.insertAtFront(Symbol(String($2), 
			UniversalVariable)) == OK);
	}
	| UNIVERSAL LPAREN IDENTIFIER RPAREN
	{
		/* return string name */
		strcpy($$, $3);

		/* check if symbol exists in global symbol table */
		Symbol newqvsym(String($3));
		if (symbols.retrieve(newqvsym) == OK)
		{
			/* symbol was found */
			if (newqvsym.getType() != Variable)
			{
				/* type of symbol changed */
				sprintf(errormsg, 
					"type changed for %s.\n", $3);
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into symbol table */
			MustBeTrue(symbols.insert(Symbol(String($3), 
				Variable)) == OK);
		}

		/* insert name into scope */
		MustBeTrue(scope.insertAtFront(Symbol(String($3), 
			UniversalVariable)) == OK);
	}
	;

existential:
	LEFTEXISTENTIAL IDENTIFIER RIGHTEXISTENTIAL
	{
		/* return string name */
		strcpy($$, $2);

		/* check if symbol exists in global symbol table */
		Symbol newqvsym(String($2));
		if (symbols.retrieve(newqvsym) == OK)
		{
			/* symbol was found */
			if (newqvsym.getType() != Variable)
			{
				/* type of symbol changed */
				sprintf(errormsg, 
					"type changed for %s.\n", $2);
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into symbol table */
			MustBeTrue(symbols.insert(Symbol(String($2), 
				Variable)) == OK);
		}

		/* insert name into scope */
		MustBeTrue(scope.insertAtFront(Symbol(String($2),
			ExistentialVariable)) == OK);
	}
	| EXISTENTIAL LPAREN IDENTIFIER RPAREN
	{
		/* return string name */
		strcpy($$, $3);

		/* check if symbol exists in global symbol table */
		Symbol newqvsym(String($3));
		if (symbols.retrieve(newqvsym) == OK)
		{
			/* symbol was found */
			if (newqvsym.getType() != Variable)
			{
				/* type of symbol changed */
				sprintf(errormsg, 
					"type changed for %s.\n", $3);
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into symbol table */
			MustBeTrue(symbols.insert(Symbol(String($3), 
				Variable)) == OK);
		}

		/* insert name into scope */
		MustBeTrue(scope.insertAtFront(Symbol(String($3),
			ExistentialVariable)) == OK);
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
		$$ = new Semantic(LogicalConstant, String(""), String("True"));
		MustBeTrue($$ != NULL);
	}
	| FALSE
	{
		/* create a logical constant record */
		$$ = new Semantic(LogicalConstant, String(""), String("False"));
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
		Symbol newsym(String($1), PredicateConstant);
		if (symbols.retrieve(newsym) == OK)
		{
			/* symbol was found */
			if (newsym.getType() != PredicateConstant)
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
			MustBeTrue(symbols.insert(newsym) == OK);
		}

		/* create a predicate constant record */
		$$ = new Semantic(PredicateConstant, String($1), String(""));
		MustBeTrue($$ != NULL);
	}
	| PIDENTIFIER LPAREN arglist RPAREN
	{
		/* count the number of arguments */
		int nargs = $3->getCount();

		/* check if symbol exists */
		Symbol newsym(String($1), PredicateFunction, nargs);
		if (symbols.retrieve(newsym) == OK)
		{
			/* symbol was found, type should not change */
			if (newsym.getType() != PredicateFunction)
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
			MustBeTrue(symbols.insert(newsym) == OK);
		}

		/* create a predicate record */
		$$ = new Semantic(PredicateFunction, String($1), $3, nargs);
		MustBeTrue($$ != NULL);
	}
	| term EQUAL term
	{
		/* turn on uses-equality flag */
		equality = 1;

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
		$$ = new Semantic(PredicateEqual, String($2), pargs, nargs);
		MustBeTrue($$ != NULL);
	}
	| term NOTEQUAL term
	{
		/* turn on uses-equality flag */
		equality = 1;

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
		$$ = new Semantic(PredicateEqual, String("="), pargs, nargs);
		MustBeTrue($$ != NULL);

		/* create a NEGATION record */
		$$ = new Semantic(Negation, NULL, $$);
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
		MustBeTrue($$->insertAtEnd($1) == OK);
	}
	| arglist separator arg
	{
		/* insert an argument in the list */
		MustBeTrue($$->insertAtEnd($3) == OK);
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
		$$ = new Semantic(QuotedString, String($1));
		MustBeTrue($$ != NULL);
	}
	| NUMBER
	{
		/* create a number record */
		$$ = new Semantic(Number, String($1));
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
			if (sym.getType() != Variable)
			{
				sprintf(errormsg, 
					"symbol %s not a variable.\n", 
					$1);
				yyerror(errormsg);
				YYERROR;
			}

			/* create a variable semantic record */
			$$ = new Semantic(Variable, String($1));
			MustBeTrue($$ != NULL);
		}
		else
		{
			/* we have a constant, verify type did not change */
			Symbol sym(String($1));
			if (symbols.retrieve(sym) == OK)
			{
				if (sym.getType() != Constant)
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
				MustBeTrue(symbols.insert(Symbol(String($1), 
					Constant)) == OK);
			}

			/* create a constant semantic record */
			$$ = new Semantic(Constant, String($1));
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
		Symbol newsym(String($1), Function, nargs);
		if (symbols.retrieve(newsym) == OK)
		{
			/* symbol was found, type should not change */
			if (newsym.getType() != Function)
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
			MustBeTrue(symbols.insert(newsym) == OK);
		}

		/* create a function record */
		$$ = new Semantic(Function, String($1), $3, nargs);
		MustBeTrue($$ != NULL);
	}
	;

separator:
	COMMA 
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

// print memory usage
void
dumpMemoryUsage()
{
	cout << endl;
	cout << "Memory usage data:" << endl;
	cout << "Symbol Constructor calls = " << Symbol::Ccount << endl;
	cout << "Symbol Destructor calls  = " << Symbol::Dcount << endl;
	cout << "Symbol Call Difference   = " << Symbol::Ccount - Symbol::Dcount << endl;
	cout << endl;
	cout << "Semantic Constructor calls = " << Semantic::Ccount << endl;
	cout << "Semantic Destructor calls  = " << Semantic::Dcount << endl;
	cout << "Semantic Call Difference   = " << Semantic::Ccount - Semantic::Dcount << endl;
	cout << endl;
	return;
}

// main entry for testing
main(int argc, char **argv)
{
	extern FILE *yyin;

	setbuf(stdout, NULL);

	for (int c = 0; (c = getopt(argc, argv, "?ptr:T:S:")) != EOF; )
	{
		switch (c)
		{
		case 'p':
			// use paramodu;ation 
			paramodulation = 1;
			break;

		case 't':
			// show post-order tree traversal
			postorder = 1;
			break;

		case 'r':
			// problem reduction type
			reductionType = String(optarg);
			if ((reductionType != String("bledsoe")) &&
			    (reductionType != String("schaum")) &&
			    (reductionType != String("cp")) &&
			    (reductionType != String("none")))
			{
				ERRORD("invalid reduction type.", 
					optarg, EINVAL);
				return(2);
			}
			break;

		case 'T':
			// temp directory
			tmpdir = String(optarg);
			break;

		case 'S':
			// suffix for output file
			suffix = String(optarg);
			break;

		case '?':
			cout << "usage: " << argv[0] << " ";
			cout << "[-?pt] ";
			cout << "[-r bledsoe|schaum|cp|none] ";
			cout << "[-T tmp_directory] ";
			cout << "[-S suffix] ";
			cout << "files ..." << endl;
			return(0);

		default:
			ERRORD("invalid command option.", c, EINVAL);
			return(2);
		}
	}

	for (int arg = optind; arg < argc; arg++)
	{
		// which file are we processing
		cout << argv[0] << ": processing file ... ";
		cout << argv[arg] << endl;

		// save file name in a global;
		fname = String(argv[arg]);
		dname = tmpdir + String("/") + fname + String(".d");
		ppfname = dname + String("/") + fname + String(".") + suffix;

		// open file for read
		if ((yyin = fopen((const char *)ppfname, "r")) == NULL)
		{
			ERRORD("unable to open file.", ppfname, errno);
			return(2);
		}

		// scan current rop file
		yyparse();

		// close rop file
		fclose(yyin);
		yyin = NULL;
	}

	dumpMemoryUsage();

	return(0);
}

