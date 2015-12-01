%{
// required headers
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

// other headers
#include "pr.h"

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
List<Semantic * > program;
List<List<String> > clauses;

// other globals
char errormsg[BUFSIZ];
int syntaxErrors = 0;
int equality = 0;

// what phases to run
Map<String, int> phases;

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
%token <cstring> EXISTENTIAL
%token <cstring> LEFTEXISTENTIAL
%token <cstring> LEFTUNIVERSAL
%token <cstring> UNIVERSAL
%token <cstring> LPAREN
%token <cstring> NEGATION
%token <cstring> NUMBER
%token <cstring> NEGNUMBER
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
%token <cstring> QUERY
%token <cstring> TRUE
%token <cstring> LBRACE
%token <cstring> RBRACE
%token <cstring> OPTION

%type <cstring> existential
%type <flag> sos_option
%type <cstring> universal
%type <parguments> arglist
%type <psemantic> and
%type <psemantic> arg
%type <psemantic> atom
%type <psemantic> biconditional
%type <psemantic> conclusiontype
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
	| start argument sosconclusionlist rbrace
	| start argument expressionlist rbrace
	| start argument expressionlist sosconclusionlist rbrace
	| start error RBRACE
	{
		cout << endl << "Syntax error detected ..." << endl;
		scope.clear();
		symbols.clear();
		ptrees.clear();
		variableNames.clear();
		yyerrok;
	}
	| start QUIT linesynch
	{
		cout << endl << "Quitting ..." << endl;
		scope.clear();
		symbols.clear();
		ptrees.clear();
		variableNames.clear();
		YYACCEPT;
	}
	| start options RBRACE
	| start options optionslist RBRACE
	;

options:
	OPTION LBRACE;
	;

optionslist:
	IDENTIFIER EQUAL STRING linesynch
	{
		options[String($1)] = String($3);
	}
	| IDENTIFIER EQUAL NUMBER linesynch
	{
		options[String($1)] = String($3);
	}
	| IDENTIFIER EQUAL NEGNUMBER linesynch
	{
		options[String($1)] = String($3);
	}
	| optionslist IDENTIFIER EQUAL STRING linesynch
	{
		options[String($2)] = String($4);
	}
	| optionslist IDENTIFIER EQUAL NUMBER linesynch
	{
		options[String($2)] = String($4);
	}
	| optionslist IDENTIFIER EQUAL NEGNUMBER linesynch
	{
		options[String($2)] = String($4);
	}
	;

rbrace:	RBRACE
	{
		/* syntax has been accepted */
		cout << endl << "Argument syntax accepted ..." << endl;

		// dump parse tree
		if (verbose)
		{
			cout << endl << "Dumping parse tree ..." << endl;
			dumpTree();
		}

		// check if we need to add any extra axioms
		if (addextraaxioms() != OK)
		{
			ERROR("unable to add extra axioms.", EINVAL);
			scope.clear();
			symbols.clear();
			ptrees.clear();
			variableNames.clear();
			YYABORT;
		}

		// convert to conjunctive normal form
		if (convert2cnf() != OK)
		{
			ERROR("conversion to CNF failed.", EINVAL);
			scope.clear();
			symbols.clear();
			ptrees.clear();
			variableNames.clear();
			YYABORT;
		}

		// run prover on the problem
		switch (runprover())
		{
		case OK:
			break;
		case NOTPROVEN:
			cout << "NOT PROVEN PROGRAM." << endl;
			break;
		case VALID:
			cout << "VALID PROGRAM." << endl;
			break;
		case NOTOK:
			ERROR("PROVER FAILED", EINVAL);
			scope.clear();
			symbols.clear();
			ptrees.clear();
			variableNames.clear();
			YYABORT;
		default:
			ERROR("UNEXPECTED RETURN.", EINVAL);
			scope.clear();
			symbols.clear();
			ptrees.clear();
			variableNames.clear();
			YYABORT;
		}

		// clear out semantic structures
		scope.clear();
		symbols.clear();
		ptrees.clear();
		variableNames.clear();
	}
	;

argument:
	ARGUMENT LBRACE
	{
		equality = 0;
	}
	;

sosconclusionlist:
	sos_option conclusiontype linesynch
	{
		/* negate conclusion for refutation proof */
		Semantic *pnegs = new Semantic(Expression::Negation, NULL, $2);
		MustBeTrue(pnegs != NULL);
		
		/* set of support, conclusion and query */
		pnegs->setSOS($1);
		pnegs->setConclusion($2->getConclusion());
		pnegs->setQuery($2->getQuery());

		// clear expression flags
		$2->setConclusion(0);
		$2->setQuery(0);

		/* save expression */
		MustBeTrue(ptrees.insertAtEnd(pnegs) == OK);
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
		MustBeTrue(ptrees.insertAtEnd($2) == OK);
	}
	| separator_expressionlist separator sos_option expression
	{
		/* is this a set of support */
		$4->setSOS($3);

		/* save expression */
		MustBeTrue(ptrees.insertAtEnd($4) == OK);
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
		/* create a UNIVERSAL record */
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
		/* create an EXISTENTIAL record */
		$$ = new Semantic(Expression::Existential, String($1), $2);
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
					"type changed for %s.\n", $2);
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into symbol table */
			MustBeTrue(symbols.insert(Symbol(String($2), 
				Symbol::Variable)) == OK);
		}

		/* insert name into scope */
		MustBeTrue(scope.insertAtFront(Symbol(String($2), 
			Symbol::UniversalVariable)) == OK);
	}
	| UNIVERSAL LPAREN IDENTIFIER RPAREN
	{
		/* return string name */
		strcpy($$, $3);

		/* check if symbol exists in global symbol table */
		Symbol newqvsym(String($3), Symbol::UniversalVariable);
		if (symbols.retrieve(newqvsym) == OK)
		{
			/* symbol was found */
			if (newqvsym.getType() != Symbol::Variable)
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
				Symbol::Variable)) == OK);
		}

		/* insert name into scope */
		MustBeTrue(scope.insertAtFront(Symbol(String($3), 
			Symbol::UniversalVariable)) == OK);
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
					"type changed for %s.\n", $2);
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into symbol table */
			MustBeTrue(symbols.insert(Symbol(String($2), 
				Symbol::Variable)) == OK);
		}

		/* insert name into scope */
		MustBeTrue(scope.insertAtFront(Symbol(String($2),
			Symbol::ExistentialVariable)) == OK);
	}
	| EXISTENTIAL LPAREN IDENTIFIER RPAREN
	{
		/* return string name */
		strcpy($$, $3);

		/* check if symbol exists in global symbol table */
		Symbol newqvsym(String($3), Symbol::ExistentialVariable);
		if (symbols.retrieve(newqvsym) == OK)
		{
			/* symbol was found */
			if (newqvsym.getType() != Symbol::Variable)
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
				Symbol::Variable)) == OK);
		}

		/* insert name into scope */
		MustBeTrue(scope.insertAtFront(Symbol(String($3),
			Symbol::ExistentialVariable)) == OK);
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
			MustBeTrue(symbols.insert(newsym) == OK);
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
			MustBeTrue(symbols.insert(newsym) == OK);
		}

		/* create a predicate record */
		$$ = new Semantic(Predicate::Function, String($1), $3, nargs);
		MustBeTrue($$ != NULL);
	}
	| term EQUAL term
	{
		/* turn on equal flag */
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
		$$ = new Semantic(Predicate::Equal, String($2), pargs, nargs);
		MustBeTrue($$ != NULL);
	}
	| term NOTEQUAL term
	{
		/* turn on equal flag */
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
		$$ = new Semantic(Predicate::Equal, String("="), pargs, nargs);
		MustBeTrue($$ != NULL);

		/* create a NEGATION record */
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
				MustBeTrue(symbols.insert(Symbol(String($1), 
					Symbol::Constant)) == OK);
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
			MustBeTrue(symbols.insert(newsym) == OK);
		}

		/* create a function record */
		$$ = new Semantic(Term::Function, String($1), $3, nargs);
		MustBeTrue($$ != NULL);
	}
	;
	
separator:
	COMMA 
	;

conclusiontype:
	THEREFORE expression
	{
		$2->setConclusion(1);
		$2->setQuery(0);
		$$ = $2;
	}
	| QUERY expression
	{
		$2->setConclusion(0);
		$2->setQuery(1);
		$$ = $2;
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

// clear program structures
void
clearProgram()
{
	if (!program.isEmpty())
	{
		ListIterator<Semantic * > programIter(program);
		for ( ; !programIter.done(); programIter++)
		{
			if (programIter() != NULL)
				delete programIter();
		}
		program.clear();
	}
	return;
}

void
clearClauses()
{
	clauses.clear();
	return;
}

// print program data structures
void
dumpTree()
{
	cout << endl << "START OF DUMP TREE ..." << endl;
	ListIterator<Semantic * > ptreesIter(ptrees);
	for ( ; !ptreesIter.done(); ptreesIter++)
	{
		cout << *ptreesIter() << ";" << endl;
	}
	cout << endl << "END OF DUMP TREE ..." << endl;
	return;
}

void
dumpProgram()
{
	ListIterator<Semantic * > programIter(program);
	for ( ; !programIter.done(); programIter++)
	{
		cout << *programIter() << ";" << endl;
	}
	return;
}

void
dumpClauses()
{
	ListIterator<List<String> > clausesIter(clauses);
	for ( ; !clausesIter.done(); clausesIter++)
	{
		ListIterator<String> clauseIter(clausesIter());
		for (int first=1; !clauseIter.done(); first=0, clauseIter++)
		{
			if (!first) cout << "|| ";
			cout << clauseIter();
		}
		cout << endl;
	}
	return;
}

// convert statements to conjunctive normal form
int
convert2cnf()
{
	if (!phases[EXPANDFILE] || !phases[PARSEFILE] || 
	    !phases[ADDEXTRAAXIOMS] || !phases[CONVERT2CNF])
	{
		cout << endl;
		cout << "Skipping conversion to CNF ..." << endl;
		return(OK);
	}
	// clear program structure
	clearProgram();
	clearClauses();

	// loop over all expressions and convert
	ListIterator<Semantic * > ptreesIter(ptrees);
	for ( ; !ptreesIter.done(); ptreesIter++)
	{
		// convert biconditionals to implications
		if (ptreesIter()->removeBiconditionals() != OK)
		{
			ERROR("removeBiconditionals failed.", EINVAL);
			return(NOTOK);
		}

		// convert implications to ORs and NOTs
		if (ptreesIter()->removeConditionals() != OK)
		{
			ERROR("removeConditionals failed.", EINVAL);
			return(NOTOK);
		}

		// apply demorgan's laws to push negation down
		if (ptreesIter()->demorgans() != OK)
		{
			ERROR("demorgans failed.", EINVAL);
			return(NOTOK);
		}

		// rename all variables to unique names
		if (ptreesIter()->renameVariables() != OK)
		{
			ERROR("renameVariables failed.", EINVAL);
			return(NOTOK);
		}

		// remove existential quantifier with skolem functions
		if (ptreesIter()->skolemize() != OK)
		{
			ERROR("skolemize failed.", EINVAL);
			return(NOTOK);
		}

		// remove universal quantifiers
		if (ptreesIter()->removeUniversals() != OK)
		{
			ERROR("removeUniversals failed.", EINVAL);
			return(NOTOK);
		}

		// distribution law for ORs and ANDs.
		if (ptreesIter()->distribution() != OK)
		{
			ERROR("distribution failed.", EINVAL);
			return(NOTOK);
		}

		// get clauses in programs
		if (ptreesIter()->getClauses(program) != OK)
		{
			ERROR("getClauses failed.", EINVAL);
			return(NOTOK);
		}
	}

	// rename variables in clauses
	ListIterator<Semantic *> programIter(program);
	for ( ; !programIter.done(); programIter++)
	{
		// rename all variables to unique names
		if (programIter()->renameAgain() != OK)
		{
			ERROR("renameAgain failed.", EINVAL);
			return(NOTOK);
		}

		// get clauses as lists
		if (programIter()->getClausesAsLists(clauses) != OK)
		{
			ERROR("getClausesAsList failed.", EINVAL);
			return(NOTOK);
		}
	}
	// dump data
	if (verbose)
	{
		cout << endl;
		cout << "Dumping program ..." << endl;
		dumpProgram();
		cout << endl;
		cout << "Dumping program clauses ..." << endl;
		dumpClauses();
	}

	// all done 
	return(OK);
}

