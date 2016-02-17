
# line 2 "pred.y"
// required headers
#include <stdio.h>
#include <string.h>
#include <limits.h>

// other headers
#include "pred.h"

// local definitions
#define IfDebug(DebugLevel, DebugMsg, StuffToDump) \
	if (DebugLevel <= currentDebugLevel) \
	{ \
		cout << DebugMsg << endl; \
		cout << StuffToDump << endl; \
	}

// phases for semantic processing
#define PHASE1 1
#define PHASE2 2
#define PHASE3 3
#define PHASE4 4
#define PHASE5 5
#define PHASE6 6
#define PHASE7 7
#define PHASE8 8
#define PHASE9 9
#define PHASE10 10
#define PHASE11 11
#define PHASE12 12
#define PHASE13 13
#define PHASE14 14
#define PHASE15 15

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
List<Set_List<Clause> > clausesPrograms;

// other globals
int currentDebugLevel = 0;
int maxPhase = PHASE15;
char errormsg[BUFSIZ];


# line 58 "pred.y"
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif
 {
	char cstring[32];
	Semantic *psemantic;
	List<Semantic *> *parguments;
} YYSTYPE;
# define AND 257
# define BICONDITIONAL 258
# define COMMA 259
# define FALSE 260
# define IDENTIFIER 261
# define IMPLICATION 262
# define LEFTEXISTENTIAL 263
# define LEFTUNIVERSAL 264
# define LPAREN 265
# define NEGATION 266
# define NEWLINE 267
# define NUMBER 268
# define OR 269
# define PIDENTIFIER 270
# define QUIT 271
# define RIGHTEXISTENTIAL 272
# define RIGHTUNIVERSAL 273
# define RPAREN 274
# define SEMICOLON 275
# define STRING 276
# define THEREFORE 277
# define TRUE 278

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

# line 630 "pred.y"


// report syntax errors
void
yyerror(char *s)
{
	fprintf(stdout, " <<==\nlineno %04d: %s\n", yylineno, s);
}

// usage message
void
usage(const char *cmd)
{
	cout << "usage: " << cmd << " [-?D:P:] input_file" << endl;
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
#if 0
			cout << "Clause " << lnno << ": ";
			ClauseIterator termIter(clauseIter());
			for (int first = 1; !termIter.done(); termIter++)
			{
				if (first)
					cout << termIter();
				else
					cout << "|| " << termIter();
				first = 0;
			}
			cout << endl;
#else
			cout << "Clause " << lnno << ": " << clauseIter() << endl;
#endif
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
	Set_List<Clause> clausesProgram;

	// loop over all expressions and convert
	ListIterator<Semantic * > ptreesIter(ptrees);
	for ( ; !ptreesIter.done(); ptreesIter++)
	{
		// initial tree
		IfDebug(1, "starting with ...", *ptreesIter());

		// convert biconditionals to implications
		if (maxPhase < PHASE1) continue;
		if (ptreesIter()->removeBiconditionals() != OK)
			return(NOTOK);
		IfDebug(1, "removeBiconditionals (after) ...", *ptreesIter());

		// convert implications to ORs and NOTs
		if (maxPhase < PHASE2) continue;
		if (ptreesIter()->removeConditionals() != OK)
			return(NOTOK);
		IfDebug(1, "removeConditionals (after) ...", *ptreesIter());

		// apply demorgan's laws to push negation down
		if (maxPhase < PHASE3) continue;
		if (ptreesIter()->demorgans() != OK)
			return(NOTOK);
		IfDebug(1, "demorgans (after) ...", *ptreesIter());

		// rename all variables to unique names
		if (maxPhase < PHASE4) continue;
		if (ptreesIter()->renameVariables() != OK)
			return(NOTOK);
		IfDebug(1, "renameVariables (after) ...", *ptreesIter());

		// remove existential quantifier with skolem functions
		if (maxPhase < PHASE5) continue;
		if (ptreesIter()->skolemize() != OK)
			return(NOTOK);
		IfDebug(1, "skolemize (after) ...", *ptreesIter());

		// remove universal quantifiers
		if (maxPhase < PHASE6) continue;
		if (ptreesIter()->removeUniversals() != OK)
			return(NOTOK);
		IfDebug(1, "removeUniversals (after) ...", *ptreesIter());

		// distribution law for ORs and ANDs.
		if (maxPhase < PHASE7) continue;
		if (ptreesIter()->distribution() != OK)
			return(NOTOK);
		IfDebug(1, "distribution (after) ...", *ptreesIter());

		// get clauses in programs
		if (maxPhase < PHASE8) continue;
		if (ptreesIter()->getClauses(program) != OK)
			return(NOTOK);
		IfDebug(1, "getClauses (after) ...", *ptreesIter());
		if (1 <= currentDebugLevel)
		{
			cout << "getClauses (after) ..." << endl;
			ListIterator<Semantic *> programIter(program);
			for (int ic=1 ; !programIter.done(); ic++, programIter++)
			{
				cout << "CLAUSE " << ic << " :";
				cout << *programIter() << endl;
			}
		}

		// check for end of program
		if (ptreesIter()->getConclusion())
		{
			// rename variables in clauses
			ListIterator<Semantic *> programIter(program);
			for (int ic=1 ; !programIter.done(); ic++,programIter++)
			{
				// rename all variables to unique names
				if (maxPhase < PHASE9) continue;
				IfDebug(1, "renameAgain (before) ...", 
					*programIter());
				if (programIter()->renameAgain() != OK)
					return(NOTOK);
				IfDebug(1, "renameAgain (after) ...", 
					*programIter());

				// convert semantic clauses to a 
				// list representation
				if (maxPhase < PHASE10) continue;
				if (programIter()->getClausesAsLists(
					clausesProgram) != OK)
					return(NOTOK);
				IfDebug(1, "getClausesAsLists (after) ...", 
					*programIter());
				if (1 <= currentDebugLevel)
				{
					cout << "getClausesAsLists (after) ..." << endl;
					Set_List_Iterator<Clause> 
						clprogIter(clausesProgram);
					for (int ic=1 ; !clprogIter.done(); 
						ic++, clprogIter++)
					{
						cout << "LIST CLAUSE " << ic << " :";
						cout << clprogIter() << endl;
					}
				}
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
clearClausesPrograms()
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

	// get command line options
	for (int c = 0; (c = getopt(argc, argv, "?D:P:")) != EOF; )
	{
		switch (c)
		{
		case 'D':
			currentDebugLevel = atoi(optarg);
			break;

		case 'P':
			maxPhase = atoi(optarg);;
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
		fprintf(stdout, "\nscanning file %s ...\n", argv[arg]);

		// open file to read
		if ((yyin = fopen(argv[arg], "r")) == NULL)
		{
			fprintf(stdout, "unable to read %s.\n", argv[arg]);
			continue;
		}

		// check syntax of program
		if (yyparse() != OK)
			fprintf(stdout, "yyparse failed.\n");
		else
		{
			// tell user that program passed
			fprintf(stdout, "yyparse passed.\n");

			// print out parse tree
			dumpPtrees();

			// convert to conjunctive normal form
			if (convert2cnf() != OK)
			{
				fprintf(stdout, "convert2cnf failed.\n");
			}
			else
			{
				fprintf(stdout, "convert2cnf passed.\n");
				dumpPtrees();
				dumpPrograms();
				dumpClausesPrograms();
			}
		}

		// clear parser data
		clearPtrees();
		clearPrograms();
		clearClausesPrograms();
		scope.clear();
		symbols.clear();

		// close file.
		fclose(yyin);
		yyin = NULL;
	}
	return(0);
}

yytabelem yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 47
# define YYLAST 125
yytabelem yyact[]={

     5,    28,    57,    58,    22,    59,    40,    25,    24,    23,
    17,     9,     9,    22,    26,     6,    25,    24,    23,    17,
    28,     7,    21,    26,    37,     9,    37,    37,    71,    22,
     7,    21,    25,    24,    23,    17,    68,    48,    39,    26,
    28,    74,    69,    67,    47,    46,    38,    21,    41,    36,
    60,    66,    15,    61,    14,    10,    13,    27,    12,     3,
    19,     4,    18,    34,    32,     8,    35,    31,    33,    30,
    42,    43,    44,     2,    20,    11,    16,    29,    64,    45,
    62,    65,    63,     1,     0,     0,     0,    50,    51,     0,
    49,     0,    52,     0,    56,    55,    54,    53,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    70,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    73,    70,    72 };
yytabelem yypact[]={

-10000000,  -256,  -274,  -247,-10000000,  -242,  -255,  -231,  -235,-10000000,
-10000000,  -212,  -224,  -263,  -209,-10000000,-10000000,  -231,  -231,  -231,
-10000000,-10000000,-10000000,  -231,  -216,  -217,  -228,-10000000,  -242,  -274,
  -235,-10000000,-10000000,-10000000,-10000000,-10000000,  -231,-10000000,  -231,  -231,
  -231,  -231,-10000000,-10000000,-10000000,  -272,  -270,  -267,  -225,-10000000,
-10000000,-10000000,-10000000,  -224,  -263,  -209,-10000000,-10000000,-10000000,-10000000,
  -232,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,  -237,-10000000,
  -225,  -225,-10000000,  -233,-10000000 };
yytabelem yypgo[]={

     0,    83,    50,    53,    82,    81,    80,    78,    54,    76,
    75,    73,    55,    58,    56,    74,    52,    62,    60,    57,
    59,    61,    65,    49 };
yytabelem yyr1[]={

     0,     1,     1,     1,     1,     1,     1,     1,    20,    20,
    22,    22,    12,    10,    10,    13,    13,    14,    14,     8,
     8,    16,    16,    16,    16,    17,    18,     9,     9,     9,
     9,    15,    15,     2,     2,     3,     6,     6,     6,     4,
     4,     7,     5,    23,    11,    19,    21 };
yytabelem yyr2[]={

     0,     0,     7,     9,     5,     7,     7,     7,     4,     6,
     3,     7,     3,     3,     7,     3,     7,     3,     7,     3,
     7,     3,     5,     5,     5,     7,     7,     3,     3,     3,
     7,     3,     9,     3,     7,     3,     3,     3,     3,     3,
     3,     3,     9,     2,     5,     4,     2 };
yytabelem yychk[]={

-10000000,    -1,   -11,   -20,   -21,   256,   271,   277,   -22,   267,
   -12,   -10,   -13,   -14,    -8,   -16,    -9,   266,   -17,   -18,
   -15,   278,   260,   265,   264,   263,   270,   -19,   275,   -11,
   -22,   -21,   -19,   -21,   -12,   -19,   -23,   259,   258,   262,
   269,   257,   -16,   -16,   -16,   -12,   261,   261,   265,   -21,
   -19,   -19,   -12,   -13,   -14,    -8,   -16,   274,   273,   272,
    -2,    -3,    -6,    -4,    -7,    -5,   276,   268,   261,   274,
   -23,   265,    -3,    -2,   274 };
yytabelem yydef[]={

     1,    -2,     0,     0,     4,     0,     0,     0,     0,    46,
    10,    12,    13,    15,    17,    19,    21,     0,     0,     0,
    27,    28,    29,     0,     0,     0,    31,     2,     0,     0,
     0,     5,     6,     7,    44,     8,     0,    43,     0,     0,
     0,     0,    22,    23,    24,     0,     0,     0,     0,    45,
     3,     9,    11,    14,    16,    18,    20,    30,    25,    26,
     0,    33,    35,    36,    37,    38,    39,    40,    41,    32,
     0,     0,    34,     0,    42 };
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
	"AND",	257,
	"BICONDITIONAL",	258,
	"COMMA",	259,
	"FALSE",	260,
	"IDENTIFIER",	261,
	"IMPLICATION",	262,
	"LEFTEXISTENTIAL",	263,
	"LEFTUNIVERSAL",	264,
	"LPAREN",	265,
	"NEGATION",	266,
	"NEWLINE",	267,
	"NUMBER",	268,
	"OR",	269,
	"PIDENTIFIER",	270,
	"QUIT",	271,
	"RIGHTEXISTENTIAL",	272,
	"RIGHTUNIVERSAL",	273,
	"RPAREN",	274,
	"SEMICOLON",	275,
	"STRING",	276,
	"THEREFORE",	277,
	"TRUE",	278,
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
	"unary : atom",
	"unary : NEGATION unary",
	"unary : universal unary",
	"unary : existential unary",
	"universal : LEFTUNIVERSAL IDENTIFIER RIGHTUNIVERSAL",
	"existential : LEFTEXISTENTIAL IDENTIFIER RIGHTEXISTENTIAL",
	"atom : predicate",
	"atom : TRUE",
	"atom : FALSE",
	"atom : LPAREN expression RPAREN",
	"predicate : PIDENTIFIER",
	"predicate : PIDENTIFIER LPAREN arglist RPAREN",
	"arglist : arg",
	"arglist : arglist separator arg",
	"arg : term",
	"term : constant",
	"term : variable",
	"term : function",
	"constant : STRING",
	"constant : NUMBER",
	"variable : IDENTIFIER",
	"function : IDENTIFIER LPAREN arglist RPAREN",
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
# line 110 "pred.y"
{
		fprintf(stdout, "argument syntax accepted ...\n");
		scope.clear();
		symbols.clear();
	} break;
case 3:
# line 116 "pred.y"
{
		fprintf(stdout, "argument syntax accepted ...\n");
		scope.clear();
		symbols.clear();
	} break;
case 4:
# line 122 "pred.y"
{
		scope.clear();
		symbols.clear();
	} break;
case 5:
# line 127 "pred.y"
{
		fprintf(stdout, "syntax error detected ...\n");
		scope.clear();
		symbols.clear();
		yyerrok;
	} break;
case 6:
# line 134 "pred.y"
{
		fprintf(stdout, "quitting ...\n");
		scope.clear();
		symbols.clear();
		YYACCEPT;
	} break;
case 7:
# line 141 "pred.y"
{
		fprintf(stdout, "quitting ...\n");
		scope.clear();
		symbols.clear();
		YYACCEPT;
	} break;
case 10:
# line 156 "pred.y"
{
		/* save expression parse tree */
		ptrees.insertAtEnd(yypvt[-0].psemantic);
	} break;
case 11:
# line 161 "pred.y"
{
		/* save expression parse tree */
		ptrees.insertAtEnd(yypvt[-0].psemantic);
	} break;
case 12:
# line 169 "pred.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 13:
# line 176 "pred.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 14:
# line 180 "pred.y"
{
		/* create a biconditional record */
		yyval.psemantic = new Semantic(Expression::Biconditional, yypvt[-2].psemantic, yypvt[-0].psemantic);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 15:
# line 189 "pred.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 16:
# line 193 "pred.y"
{
		/* create a conditional record */
		yyval.psemantic = new Semantic(Expression::Conditional, yypvt[-2].psemantic, yypvt[-0].psemantic);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 17:
# line 202 "pred.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 18:
# line 206 "pred.y"
{
		/* create an OR record */
		yyval.psemantic = new Semantic(Expression::Or, yypvt[-2].psemantic, yypvt[-0].psemantic);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 19:
# line 215 "pred.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 20:
# line 219 "pred.y"
{
		/* create an AND record */
		yyval.psemantic = new Semantic(Expression::And, yypvt[-2].psemantic, yypvt[-0].psemantic);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 21:
# line 228 "pred.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 22:
# line 232 "pred.y"
{
		/* create an NEGATION record */
		yyval.psemantic = new Semantic(Expression::Negation, NULL, yypvt[-0].psemantic);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 23:
# line 238 "pred.y"
{
		/* create an NEGATION record */
		yyval.psemantic = new Semantic(Expression::Universal, String(yypvt[-1].cstring), yypvt[-0].psemantic);
		MustBeTrue(yyval.psemantic != NULL);

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
	} break;
case 24:
# line 259 "pred.y"
{
		/* create an NEGATION record */
		yyval.psemantic = new Semantic(Expression::Existential, yypvt[-1].cstring, yypvt[-0].psemantic);
		MustBeTrue(yyval.psemantic != NULL);

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
	} break;
case 25:
# line 283 "pred.y"
{
		/* return string name */
		strcpy(yyval.cstring, yypvt[-1].cstring);

		/* check if symbol exists in global symbol table */
		Symbol newqvsym(String(yypvt[-1].cstring), Symbol::UniversalVariable);
		if (symbols.retrieve(newqvsym) == OK)
		{
			/* symbol was found */
			if (newqvsym.getType() != Symbol::Variable)
			{
				/* type of symbol changed */
				sprintf(errormsg, 
					"type changed for %s.\n", yypvt[-2].cstring);
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into symbol table */
			symbols.insert(Symbol(String(yypvt[-1].cstring), 
				Symbol::Variable));
		}

		/* insert name into scope */
		scope.insertAtFront(Symbol(String(yypvt[-1].cstring), 
			Symbol::UniversalVariable));
		IfDebug(5, "starting a new universal scope ...", scope);
	} break;
case 26:
# line 317 "pred.y"
{
		/* return string name */
		strcpy(yyval.cstring, yypvt[-1].cstring);

		/* check if symbol exists in global symbol table */
		Symbol newqvsym(String(yypvt[-1].cstring), Symbol::ExistentialVariable);
		if (symbols.retrieve(newqvsym) == OK)
		{
			/* symbol was found */
			if (newqvsym.getType() != Symbol::Variable)
			{
				/* type of symbol changed */
				sprintf(errormsg, 
					"type changed for %s.\n", yypvt[-2].cstring);
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into symbol table */
			symbols.insert(Symbol(String(yypvt[-1].cstring), 
				Symbol::Variable));
		}

		/* insert name into scope */
		scope.insertAtFront(Symbol(String(yypvt[-1].cstring),
			Symbol::ExistentialVariable));
		IfDebug(5, "starting a new existential scope ...", scope);
	} break;
case 27:
# line 351 "pred.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 28:
# line 355 "pred.y"
{
		/* create a predicate logical record */
		yyval.psemantic = new Semantic(Predicate::LogicalConstant, 
				String(""), String("true"));
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 29:
# line 362 "pred.y"
{
		/* create a logical constant record */
		yyval.psemantic = new Semantic(Predicate::LogicalConstant, 
				String(""), String("false"));
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 30:
# line 369 "pred.y"
{
		yyval.psemantic = yypvt[-1].psemantic;
	} break;
case 31:
# line 376 "pred.y"
{
		/* check if symbol exists */
		Symbol newsym(String(yypvt[-0].cstring), Symbol::PredicateConstant);
		if (symbols.retrieve(newsym) == OK)
		{
			/* symbol was found */
			if (newsym.getType() != Symbol::PredicateConstant)
			{
				/* type of symbol changed */
				sprintf(errormsg, 
					"type changed for %s.\n", yypvt[-0].cstring);
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
		yyval.psemantic = new Semantic(Predicate::Constant, String(yypvt[-0].cstring), String(""));
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 32:
# line 402 "pred.y"
{
		/* count the number of arguments */
		ListIterator<Semantic * > argsIter(*yypvt[-1].parguments);
		for (int nargs = 0; !argsIter.done(); argsIter++, nargs++);

		/* check if symbol exists */
		Symbol newsym(String(yypvt[-3].cstring), Symbol::PredicateFunction, nargs);
		if (symbols.retrieve(newsym) == OK)
		{
			/* symbol was found, type should not change */
			if (newsym.getType() != Symbol::PredicateFunction)
			{
				sprintf(errormsg, "type changed for %s.\n", yypvt[-3].cstring);
				yyerror(errormsg);
				YYERROR;
			}
			if (newsym.getArgs() != nargs)
			{
				char buffer[BUFSIZ];
				sprintf(buffer, "number of args changed for %s.\n", yypvt[-3].cstring);
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
		yyval.psemantic = new Semantic(Predicate::Function, String(yypvt[-3].cstring), yypvt[-1].parguments, nargs);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 33:
# line 445 "pred.y"
{
		/* create an argument list */
		yyval.parguments = new List<Semantic *>;
		MustBeTrue(yyval.parguments != NULL);

		/* insert an argument in the list */
		yyval.parguments->insertAtEnd(yypvt[-0].psemantic);
	} break;
case 34:
# line 454 "pred.y"
{
		/* insert an argument in the list */
		yyval.parguments->insertAtEnd(yypvt[-0].psemantic);
	} break;
case 35:
# line 462 "pred.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 36:
# line 469 "pred.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 37:
# line 473 "pred.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 38:
# line 477 "pred.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 39:
# line 484 "pred.y"
{
		/* create a string record */
		yyval.psemantic = new Semantic(Argument::QuotedString, String(yypvt[-0].cstring));
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 40:
# line 490 "pred.y"
{
		/* create a number record */
		yyval.psemantic = new Semantic(Argument::Number, String(yypvt[-0].cstring));
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 41:
# line 499 "pred.y"
{
		/* search if name is in the scope */
		Symbol varsym(String(yypvt[-0].cstring));
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
			Symbol sym(String(yypvt[-0].cstring));
			if (symbols.retrieve(sym) != OK)
			{
				sprintf(errormsg, 
					"variable %s not in symbol table.\n", 
					yypvt[-0].cstring);
				yyerror(errormsg);
				YYERROR;
			}
			if (sym.getType() != Symbol::Variable)
			{
				sprintf(errormsg, 
					"symbol %s not a variable.\n", 
					yypvt[-0].cstring);
				yyerror(errormsg);
				YYERROR;
			}

			/* create a variable semantic record */
			yyval.psemantic = new Semantic(Argument::Variable, String(yypvt[-0].cstring));
			MustBeTrue(yyval.psemantic != NULL);
		}
		else
		{
			/* we have a constant, verify type did not change */
			Symbol sym(String(yypvt[-0].cstring));
			if (symbols.retrieve(sym) == OK)
			{
				if (sym.getType() != Symbol::Constant)
				{
					sprintf(errormsg, 
						"symbol %s not a constant.\n", 
						yypvt[-0].cstring);
					yyerror(errormsg);
					YYERROR;
				}
			}
			else
			{
				/* insert new constant symbol entry */
				symbols.insert(Symbol(String(yypvt[-0].cstring), 
					Symbol::Constant));
			}

			/* create a constant semantic record */
			yyval.psemantic = new Semantic(Argument::Constant, String(yypvt[-0].cstring));
			MustBeTrue(yyval.psemantic != NULL);
		}
	} break;
case 42:
# line 566 "pred.y"
{
		/* get number of arguments */
		ListIterator<Semantic * > argsIter(*yypvt[-1].parguments);
		for (int nargs = 0; !argsIter.done(); argsIter++, nargs++);

		/* check if symbol exists */
		Symbol newsym(String(yypvt[-3].cstring), Symbol::Function, nargs);
		if (symbols.retrieve(newsym) == OK)
		{
			/* symbol was found, type should not change */
			if (newsym.getType() != Symbol::Function)
			{
				sprintf(errormsg, "type changed for %s.\n", yypvt[-3].cstring);
				yyerror(errormsg);
				YYERROR;
			}
			if (newsym.getArgs() != nargs)
			{
				char buffer[BUFSIZ];
				sprintf(buffer, "number of args changed for %s.\n", yypvt[-3].cstring);
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
		yyval.psemantic = new Semantic(Argument::Function, String(yypvt[-3].cstring), yypvt[-1].parguments, nargs);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 44:
# line 613 "pred.y"
{
		/* negate conclusion */
		yyval.psemantic = new Semantic(Expression::Negation, NULL, yypvt[-0].psemantic, 1);
		MustBeTrue(yyval.psemantic != NULL);

		/* save expression */
		ptrees.insertAtEnd(yyval.psemantic);
	} break;
# line	532 "/usr/ccs/bin/yaccpar"
	}
	goto yystack;		/* reset registers in driver code */
}

