
# line 2 "pr.y"
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
#include "pr.h"

// externs
extern int yylineno;
extern int optind;
extern char *optarg;

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
int verbose = 0;
int echoflag = 0;
int paramodulation = 0;
int equality = 0;
unsigned int maxdepth = 100;
char filename[BUFSIZ];
int finalstatus = 0;


# line 44 "pr.y"
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif
 {
	int flag;
	char cstring[32];
	Semantic *psemantic;
	List<Semantic *> *parguments;
} YYSTYPE;
# define AND 257
# define ARGUMENT 258
# define BICONDITIONAL 259
# define COMMA 260
# define FALSE 261
# define IDENTIFIER 262
# define EQUAL 263
# define NOTEQUAL 264
# define IMPLICATION 265
# define LEFTEXISTENTIAL 266
# define LEFTUNIVERSAL 267
# define LPAREN 268
# define NEGATION 269
# define NUMBER 270
# define OR 271
# define PIDENTIFIER 272
# define QUIT 273
# define RIGHTEXISTENTIAL 274
# define RIGHTUNIVERSAL 275
# define RPAREN 276
# define SEMICOLON 277
# define SOS 278
# define STRING 279
# define THEREFORE 280
# define TRUE 281
# define LBRACE 282
# define RBRACE 283

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

# line 696 "pr.y"


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
	cout << "usage: " << cmd << " [-?V] file ... " << endl;
}

// clear program structures
static void
clearProgram()
{
	if (program.isEmpty())
		return;
	ListIterator<Semantic * > programIter(program);
	for ( ; !programIter.done(); programIter++)
	{
		if (programIter() != NULL)
			delete programIter();
	}
	program.clear();
	return;
}

static void
clearClauses()
{
	clauses.clear();
	return;
}

// print program data structures
void
dumpTree()
{
	ListIterator<Semantic * > ptreesIter(ptrees);
	for ( ; !ptreesIter.done(); ptreesIter++)
	{
		cout << *ptreesIter() << ";" << endl;
	}
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
	// clear program structure
	clearProgram();
	clearClauses();

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

		// rename all variables to unique names
		if (ptreesIter()->renameVariables() != OK)
			return(NOTOK);

		// remove existential quantifier with skolem functions
		if (ptreesIter()->skolemize() != OK)
			return(NOTOK);

		// remove universal quantifiers
		if (ptreesIter()->removeUniversals() != OK)
			return(NOTOK);

		// distribution law for ORs and ANDs.
		if (ptreesIter()->distribution() != OK)
			return(NOTOK);

		// get clauses in programs
		if (ptreesIter()->getClauses(program) != OK)
			return(NOTOK);

		// check for end of program
		if (ptreesIter()->getConclusion())
		{
			// rename variables in clauses
			ListIterator<Semantic *> programIter(program);
			for ( ; !programIter.done(); programIter++)
			{
				// rename all variables to unique names
				if (programIter()->renameAgain() != OK)
					return(NOTOK);

				// get clauses as lists
				if (programIter()->getClausesAsLists(clauses) != OK)
					return(NOTOK);
			}
		}
	}

	// all done 
	return(OK);
}

// convert to CNF and try to prove problem */
int
runatp()
{
	// print out results
	if (verbose)
	{
		cout << "BEFORE conversion to CNF ..." << endl;
		dumpTree();
	}

	// check if we need to add any extra axioms
	if (addextraaxioms() != OK)
	{
		sprintf(errormsg, "adding extra axioms.");
		return(NOTOK);
	}
	if (verbose)
	{
		cout << "AFTER adding additional axioms ..." << endl;
		dumpTree();
	}

	// convert to conjunctive normal form
	if (convert2cnf() != OK)
	{
		sprintf(errormsg, "conversion to CNF failed.");
		return(NOTOK);
	}

	// print out results
	if (verbose)
	{
		cout << "AFTER conversion to CNF ..." << endl;
		dumpTree();
		cout << "FINAL PROGRAM ..." << endl;
		dumpProgram();
		cout << "FINAL CLAUSES PROGRAM ..." << endl;
		dumpClauses();
	}

	// run prover on the problem
	switch (runprover())
	{
	case NOTPROVEN:
		cout << "NOT PROVEN PROGRAM." << endl;
		break;
	case VALID:
		cout << "VALID PROGRAM." << endl;
		break;
	case INVALID:
		cout << "INVALID PROGRAM." << endl;
		break;
	case NOTOK:
		cout << "PROVER FAILED." << endl;
		sprintf(errormsg, "prover failed.");
		return(NOTOK);
	default:
		cout << "UNEXPECTED RETURN." << endl;
		sprintf(errormsg, "prover unexpected return.");
		return(NOTOK);
	}

	// clear out semantic structures
	scope.clear();
	symbols.clear();
	ptrees.clear();

	// all done
	return(OK);
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
	for (int c = 0; (c = getopt(argc, argv, "?VEPM:")) != EOF; )
	{
		switch (c)
		{
		case 'P':
			// use paramodulation
			paramodulation = 1;
			break;

		case 'E':
			// echo code
			echoflag = 1;
			break;

		case 'V':
			// verbose is on
			verbose = 1;
			break;

		case 'M':
			// set maximum depth
			maxdepth = atoi(optarg);
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
		strcpy(filename, argv[arg]);
		cout << "scanning file ... " << filename << endl;

		// reset syntax errors counter for this file
		finalstatus = syntaxErrors = 0;

		// open file to read
		if ((yyin = fopen(filename, "r")) == NULL)
		{
			finalstatus = -1;
			ERRORD("unable to read.", filename, errno);
			break;
		}

		// check syntax of program
		if (yyparse() != OK || syntaxErrors > 0)
		{
			finalstatus = -1;
			ERRORD("yyparse failed.", syntaxErrors, errno);
			break;
		}

		// close file.
		fclose(yyin);
		yyin = NULL;
	}

	// all done
DUMP(finalstatus);
	if (finalstatus == VALID)
		return(0);
	else if (finalstatus == NOTOK)
		return(-1);
	else
		return(1);
}

yytabelem yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 51
# define YYLAST 142
yytabelem yyact[]={

    33,    44,    11,    14,    21,    36,    35,    34,    28,    43,
    10,    37,    10,    47,    13,    49,    33,    44,    42,    21,
    32,    36,    35,    34,    28,    43,    10,    37,    47,    44,
    13,    15,    47,    73,    42,    74,    32,    43,     3,    75,
     5,    55,    66,    63,    85,    54,    42,    62,    83,    64,
    65,    61,    53,    56,    38,     4,    77,    20,    46,    76,
    26,    24,    12,    19,     7,     9,    25,     8,     6,    23,
     2,    40,    45,    18,    16,    31,    17,    41,    39,    52,
    48,    50,    51,    22,    27,    16,    29,    30,     1,    57,
    58,    59,    60,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    68,     0,     0,    67,     0,    70,    72,    78,    79,
    80,    78,    71,    69,     0,    82,    81,     0,     0,     0,
     0,     0,     0,     0,     0,    84,     0,     0,     0,    78,
    84,    86 };
yytabelem yypact[]={

-10000000,  -218,  -266,  -281,  -263,  -279,  -252,  -266,  -261,  -247,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,  -276,  -268,  -247,  -263,
-10000000,  -245,  -207,  -220,  -230,  -204,-10000000,-10000000,  -245,  -245,
  -245,-10000000,-10000000,-10000000,  -245,  -211,  -215,  -225,  -214,-10000000,
-10000000,-10000000,-10000000,-10000000,  -226,-10000000,  -266,-10000000,  -263,-10000000,
-10000000,-10000000,-10000000,  -245,  -245,  -245,  -245,-10000000,-10000000,-10000000,
  -243,  -240,  -235,  -233,  -233,  -233,  -233,  -245,-10000000,  -220,
  -230,  -204,-10000000,-10000000,-10000000,-10000000,  -228,-10000000,-10000000,-10000000,
-10000000,  -232,-10000000,-10000000,  -233,-10000000,-10000000 };
yytabelem yypgo[]={

     0,    88,    87,    67,    86,    59,    66,    56,    84,    83,
    63,    78,    57,    77,    69,    61,    75,    54,    60,    71,
    70,    68,    64,    62,    65,    58 };
yytabelem yyr1[]={

     0,     1,     1,     1,     1,     1,    20,    21,    21,    22,
    22,    24,    24,     3,     3,    12,     9,     9,    14,    14,
    15,    15,     6,     6,    18,    18,    18,    18,     4,     2,
     8,     8,     8,     8,    16,    16,    16,    16,     5,     5,
     7,    17,    17,    17,    11,    11,    19,    13,    25,    10,
    23 };
yytabelem yyr2[]={

     0,     0,     9,    11,     7,     7,     5,     7,     9,     4,
     6,     5,     9,     1,     3,     3,     3,     7,     3,     7,
     3,     7,     3,     7,     3,     5,     5,     5,     7,     7,
     3,     3,     3,     7,     3,     9,     7,     7,     3,     7,
     3,     3,     3,     3,     3,     3,     3,     9,     2,     5,
     2 };
yytabelem yychk[]={

-10000000,    -1,   -20,   256,   273,   258,   -21,   -22,    -3,   -24,
   278,   283,   -23,   277,   282,   283,    -3,   -21,   -24,   -10,
   -12,   280,    -9,   -14,   -15,    -6,   -18,    -8,   269,    -4,
    -2,   -16,   281,   261,   268,   267,   266,   272,   -17,   -11,
   -19,   -13,   279,   270,   262,   -23,   -25,   260,   -10,   283,
   -23,   -23,   -12,   259,   265,   271,   257,   -18,   -18,   -18,
   -12,   262,   262,   268,   263,   264,   268,    -3,   -23,   -14,
   -15,    -6,   -18,   276,   275,   274,    -5,    -7,   -17,   -17,
   -17,    -5,   -12,   276,   -25,   276,    -7 };
yytabelem yydef[]={

     1,    -2,    13,     0,     0,     0,    13,    13,     0,     0,
    14,     4,     5,    50,     6,     2,     0,    13,     0,     0,
    11,     0,    15,    16,    18,    20,    22,    24,     0,     0,
     0,    30,    31,    32,     0,     0,     0,    34,     0,    41,
    42,    43,    44,    45,    46,     9,    13,    48,     0,     3,
    10,     7,    49,     0,     0,     0,     0,    25,    26,    27,
     0,     0,     0,     0,     0,     0,     0,     0,     8,    17,
    19,    21,    23,    33,    28,    29,     0,    38,    40,    36,
    37,     0,    12,    35,     0,    47,    39 };
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
	"ARGUMENT",	258,
	"BICONDITIONAL",	259,
	"COMMA",	260,
	"FALSE",	261,
	"IDENTIFIER",	262,
	"EQUAL",	263,
	"NOTEQUAL",	264,
	"IMPLICATION",	265,
	"LEFTEXISTENTIAL",	266,
	"LEFTUNIVERSAL",	267,
	"LPAREN",	268,
	"NEGATION",	269,
	"NUMBER",	270,
	"OR",	271,
	"PIDENTIFIER",	272,
	"QUIT",	273,
	"RIGHTEXISTENTIAL",	274,
	"RIGHTUNIVERSAL",	275,
	"RPAREN",	276,
	"SEMICOLON",	277,
	"SOS",	278,
	"STRING",	279,
	"THEREFORE",	280,
	"TRUE",	281,
	"LBRACE",	282,
	"RBRACE",	283,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"start : /* empty */",
	"start : start argument conclusionlist RBRACE",
	"start : start argument expressionlist conclusionlist RBRACE",
	"start : start error RBRACE",
	"start : start QUIT linesynch",
	"argument : ARGUMENT LBRACE",
	"conclusionlist : sos_option conclusion linesynch",
	"conclusionlist : conclusionlist sos_option conclusion linesynch",
	"expressionlist : separator_expressionlist linesynch",
	"expressionlist : expressionlist separator_expressionlist linesynch",
	"separator_expressionlist : sos_option expression",
	"separator_expressionlist : separator_expressionlist separator sos_option expression",
	"sos_option : /* empty */",
	"sos_option : SOS",
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
	"predicate : term EQUAL term",
	"predicate : term NOTEQUAL term",
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
	"linesynch : SEMICOLON",
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
# line 103 "pr.y"
{
		/* syntax has been accepted */
		cout << "argument syntax accepted ..." << endl;

		/* run automated theorem proving program */
		finalstatus = runatp();
	} break;
case 3:
# line 111 "pr.y"
{
		/* syntax has been accepted */
		cout << "argument syntax accepted ..." << endl;

		/* run automated theorem proving program */
		finalstatus = runatp();
	} break;
case 4:
# line 119 "pr.y"
{
		fprintf(stdout, "syntax error detected ...\n");
		scope.clear();
		symbols.clear();
		yyerrok;
	} break;
case 5:
# line 126 "pr.y"
{
		fprintf(stdout, "quitting ...\n");
		scope.clear();
		symbols.clear();
		YYACCEPT;
	} break;
case 6:
# line 136 "pr.y"
{
		equality = 0;
	} break;
case 7:
# line 143 "pr.y"
{
		/* negate conclusion for refutation proof */
		Semantic *pnegs = new Semantic(Expression::Negation, NULL, yypvt[-1].psemantic);
		MustBeTrue(pnegs != NULL);
		
		/* set of support and conclusion */
		pnegs->setSOS(yypvt[-2].flag);
		pnegs->setConclusion(1);

		/* save expression */
		MustBeTrue(ptrees.insertAtEnd(pnegs) == OK);
	} break;
case 8:
# line 157 "pr.y"
{
		/* negate conclusion for refutation proof */
		Semantic *pnegs = new Semantic(Expression::Negation, NULL, yypvt[-1].psemantic);
		MustBeTrue(pnegs != NULL);
		
		/* set of support and conclusion */
		pnegs->setSOS(yypvt[-2].flag);
		pnegs->setConclusion(1);

		/* save expression */
		MustBeTrue(ptrees.insertAtEnd(pnegs) == OK);
	} break;
case 11:
# line 178 "pr.y"
{
		/* is this a set of support */
		yypvt[-0].psemantic->setSOS(yypvt[-1].flag);

		/* save expression */
		MustBeTrue(ptrees.insertAtEnd(yypvt[-0].psemantic) == OK);
	} break;
case 12:
# line 186 "pr.y"
{
		/* is this a set of support */
		yypvt[-0].psemantic->setSOS(yypvt[-1].flag);

		/* save expression */
		MustBeTrue(ptrees.insertAtEnd(yypvt[-0].psemantic) == OK);
	} break;
case 13:
# line 197 "pr.y"
{
		yyval.flag = 0;
	} break;
case 14:
# line 201 "pr.y"
{
		yyval.flag = 1;
	} break;
case 15:
# line 208 "pr.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 16:
# line 215 "pr.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 17:
# line 219 "pr.y"
{
		/* create a biconditional record */
		yyval.psemantic = new Semantic(Expression::Biconditional, yypvt[-2].psemantic, yypvt[-0].psemantic);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 18:
# line 228 "pr.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 19:
# line 232 "pr.y"
{
		/* create a conditional record */
		yyval.psemantic = new Semantic(Expression::Conditional, yypvt[-2].psemantic, yypvt[-0].psemantic);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 20:
# line 241 "pr.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 21:
# line 245 "pr.y"
{
		/* create an OR record */
		yyval.psemantic = new Semantic(Expression::Or, yypvt[-2].psemantic, yypvt[-0].psemantic);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 22:
# line 254 "pr.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 23:
# line 258 "pr.y"
{
		/* create an AND record */
		yyval.psemantic = new Semantic(Expression::And, yypvt[-2].psemantic, yypvt[-0].psemantic);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 24:
# line 267 "pr.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 25:
# line 271 "pr.y"
{
		/* create an NEGATION record */
		yyval.psemantic = new Semantic(Expression::Negation, NULL, yypvt[-0].psemantic);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 26:
# line 277 "pr.y"
{
		/* create a UNIVERSAL record */
		yyval.psemantic = new Semantic(Expression::Universal, String(yypvt[-1].cstring), yypvt[-0].psemantic);
		MustBeTrue(yyval.psemantic != NULL);

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
	} break;
case 27:
# line 297 "pr.y"
{
		/* create an EXISTENTIAL record */
		yyval.psemantic = new Semantic(Expression::Existential, String(yypvt[-1].cstring), yypvt[-0].psemantic);
		MustBeTrue(yyval.psemantic != NULL);

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
	} break;
case 28:
# line 320 "pr.y"
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
	} break;
case 29:
# line 353 "pr.y"
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
	} break;
case 30:
# line 386 "pr.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 31:
# line 390 "pr.y"
{
		/* create a predicate logical record */
		yyval.psemantic = new Semantic(Predicate::LogicalConstant, 
				String(""), String("True"));
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 32:
# line 397 "pr.y"
{
		/* create a logical constant record */
		yyval.psemantic = new Semantic(Predicate::LogicalConstant, 
				String(""), String("False"));
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 33:
# line 404 "pr.y"
{
		yyval.psemantic = yypvt[-1].psemantic;
	} break;
case 34:
# line 411 "pr.y"
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
case 35:
# line 437 "pr.y"
{
		/* count the number of arguments */
		int nargs = yypvt[-1].parguments->getCount();

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
case 36:
# line 476 "pr.y"
{
		/* turn on equal flag */
		equality = 1;

		/* create a list */
		List<Semantic *> *pargs = new List<Semantic *>;
		MustBeTrue(pargs != NULL);

		/* insert LHS and RHS into list */
		MustBeTrue(pargs->insertAtEnd(yypvt[-2].psemantic) == OK);
		MustBeTrue(pargs->insertAtEnd(yypvt[-0].psemantic) == OK);

		/* TWO arguments */
		int nargs = pargs->getCount();
		MustBeTrue(nargs == 2);

		/* create a new semantic record */
		yyval.psemantic = new Semantic(Predicate::Equal, String(yypvt[-1].cstring), pargs, nargs);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 37:
# line 497 "pr.y"
{
		/* turn on equal flag */
		equality = 1;

		/* create a list */
		List<Semantic *> *pargs = new List<Semantic *>;
		MustBeTrue(pargs != NULL);

		/* insert LHS and RHS into list */
		MustBeTrue(pargs->insertAtEnd(yypvt[-2].psemantic) == OK);
		MustBeTrue(pargs->insertAtEnd(yypvt[-0].psemantic) == OK);

		/* TWO arguments */
		int nargs = pargs->getCount();
		MustBeTrue(nargs == 2);

		/* create a new semantic record */
		yyval.psemantic = new Semantic(Predicate::Equal, String(yypvt[-1].cstring), pargs, nargs);
		MustBeTrue(yyval.psemantic != NULL);

		/* create an NEGATION record */
		yyval.psemantic = new Semantic(Expression::Negation, NULL, yyval.psemantic);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 38:
# line 525 "pr.y"
{
		/* create an argument list */
		yyval.parguments = new List<Semantic *>;
		MustBeTrue(yyval.parguments != NULL);

		/* insert an argument in the list */
		yyval.parguments->insertAtEnd(yypvt[-0].psemantic);
	} break;
case 39:
# line 534 "pr.y"
{
		/* insert an argument in the list */
		yyval.parguments->insertAtEnd(yypvt[-0].psemantic);
	} break;
case 40:
# line 542 "pr.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 41:
# line 549 "pr.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 42:
# line 553 "pr.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 43:
# line 557 "pr.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 44:
# line 564 "pr.y"
{
		/* create a string record */
		yyval.psemantic = new Semantic(Term::QuotedString, String(yypvt[-0].cstring));
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 45:
# line 570 "pr.y"
{
		/* create a number record */
		yyval.psemantic = new Semantic(Term::Number, String(yypvt[-0].cstring));
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 46:
# line 579 "pr.y"
{
		/* search if name is in the scope */
		Symbol varsym(String(yypvt[-0].cstring));
		if (scope.retrieve(varsym) == OK)
		{
			/* increment usage */
			varsym++;
			scope.update(varsym);

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
			yyval.psemantic = new Semantic(Term::Variable, String(yypvt[-0].cstring));
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
			yyval.psemantic = new Semantic(Term::Constant, String(yypvt[-0].cstring));
			MustBeTrue(yyval.psemantic != NULL);
		}
	} break;
case 47:
# line 642 "pr.y"
{
		/* get number of arguments */
		int nargs = yypvt[-1].parguments->getCount();

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
		yyval.psemantic = new Semantic(Term::Function, String(yypvt[-3].cstring), yypvt[-1].parguments, nargs);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 49:
# line 688 "pr.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
# line	532 "/usr/ccs/bin/yaccpar"
	}
	goto yystack;		/* reset registers in driver code */
}

