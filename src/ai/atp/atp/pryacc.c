
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
int equality = 0;

// what phases to run
Map<String, int> phases;


# line 41 "pr.y"
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
# define EXISTENTIAL 266
# define LEFTEXISTENTIAL 267
# define LEFTUNIVERSAL 268
# define UNIVERSAL 269
# define LPAREN 270
# define NEGATION 271
# define NUMBER 272
# define NEGNUMBER 273
# define OR 274
# define PIDENTIFIER 275
# define QUIT 276
# define RIGHTEXISTENTIAL 277
# define RIGHTUNIVERSAL 278
# define RPAREN 279
# define SEMICOLON 280
# define SOS 281
# define STRING 282
# define THEREFORE 283
# define QUERY 284
# define TRUE 285
# define LBRACE 286
# define RBRACE 287
# define OPTION 288

#include <inttypes.h>

#ifdef __STDC__
#include <stdlib.h>
#include <string.h>
#define	YYCONST	const
#else
#include <malloc.h>
#include <memory.h>
#define	YYCONST
#endif

#include <values.h>

#if defined(__cplusplus) || defined(__STDC__)

#if defined(__cplusplus) && defined(__EXTERN_C__)
extern "C" {
#endif
#ifndef yyerror
#if defined(__cplusplus)
	void yyerror(YYCONST char *);
#endif
#endif
#ifndef yylex
	int yylex(void);
#endif
	int yyparse(void);
#if defined(__cplusplus) && defined(__EXTERN_C__)
}
#endif

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

# line 850 "pr.y"


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

static YYCONST yytabelem yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 64
# define YYLAST 194
static YYCONST yytabelem yyact[]={

    41,    54,    59,    18,    22,    46,    45,    43,    44,    42,
    36,    53,    13,    12,    47,    20,    19,    12,     3,    22,
     6,    52,    28,    29,    40,    41,    54,    58,    16,    15,
    46,    45,    43,    44,    42,    36,    53,    54,     4,    47,
    57,   104,   105,    85,    86,    57,    52,    53,    57,    40,
     7,   103,   110,    84,    83,   109,    91,    52,    67,    92,
    15,    94,    81,    68,   113,    78,    77,   111,    75,    79,
    80,    60,    95,    93,    76,    74,    66,    69,    48,    14,
    97,    96,    34,    32,    56,    27,    33,    31,    10,    11,
     8,    55,    21,    17,     5,     9,     2,    50,    39,    25,
    24,    51,    23,    49,    26,    62,    63,    30,    35,    37,
    38,     1,     0,     0,    64,    65,     0,    61,     0,    70,
    71,    72,     0,     0,     0,     0,     0,     0,    73,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    82,     0,     0,     0,     0,
     0,    88,    90,     0,    87,    89,     0,    98,    99,   100,
    98,     0,     0,   101,   106,   107,   108,     0,   102,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   112,     0,   114,   115,   116,   112,     0,     0,     0,
     0,    98,     0,   117 };
static YYCONST yytabelem yypact[]={

-10000000,  -238,  -264,  -275,  -251,  -259,  -270,  -271,  -283,  -268,
  -261,  -220,-10000000,-10000000,-10000000,-10000000,-10000000,  -260,  -192,-10000000,
-10000000,-10000000,-10000000,-10000000,  -283,  -220,  -251,-10000000,  -236,  -236,
  -183,  -207,  -211,  -180,-10000000,-10000000,  -236,  -236,  -236,-10000000,
-10000000,-10000000,  -236,  -187,  -202,  -188,  -204,  -205,  -194,-10000000,
-10000000,-10000000,-10000000,-10000000,  -208,-10000000,  -264,-10000000,-10000000,  -209,
  -229,-10000000,-10000000,-10000000,-10000000,-10000000,  -236,  -236,  -236,  -236,
-10000000,-10000000,-10000000,  -223,  -219,  -189,  -216,  -190,  -225,  -225,
  -225,  -225,  -236,  -231,  -251,  -251,  -251,  -207,  -211,  -180,
-10000000,-10000000,-10000000,  -224,-10000000,  -227,  -212,-10000000,-10000000,-10000000,
-10000000,  -215,-10000000,  -251,  -251,  -251,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,  -225,-10000000,-10000000,-10000000,-10000000,-10000000 };
static YYCONST yytabelem yypgo[]={

     0,   111,   110,    88,   109,    81,    86,    80,   108,   107,
   104,   103,    85,   101,    87,    83,    98,    78,    82,    97,
    96,    90,    92,    95,    79,    94,    93,    89,    84 };
static YYCONST yytabelem yyr1[]={

     0,     1,     1,     1,     1,     1,     1,     1,     1,    25,
    26,    26,    26,    26,    26,    26,    22,    20,    21,    23,
    23,    27,    27,     3,     3,    12,     9,     9,    14,    14,
    15,    15,     6,     6,    18,    18,    18,    18,     4,     4,
     2,     2,     8,     8,     8,     8,    16,    16,    16,    16,
     5,     5,     7,    17,    17,    17,    11,    11,    19,    13,
    28,    10,    10,    24 };
static YYCONST yytabelem yyr2[]={

     0,     0,     8,     8,    10,     7,     7,     6,     8,     4,
     9,     9,     9,    11,    11,    11,     3,     5,     7,     4,
     6,     5,     9,     1,     3,     3,     3,     7,     3,     7,
     3,     7,     3,     7,     3,     5,     5,     5,     7,     9,
     7,     9,     3,     3,     3,     7,     3,     9,     7,     7,
     3,     7,     3,     3,     3,     3,     3,     3,     3,     9,
     2,     5,     5,     2 };
static YYCONST yytabelem yychk[]={

-10000000,    -1,   -20,   256,   276,   -25,   258,   288,   -21,   -23,
    -3,   -27,   281,   287,   -24,   280,   287,   -26,   262,   286,
   286,   -22,   287,   -22,   -21,   -27,   -10,   -12,   283,   284,
    -9,   -14,   -15,    -6,   -18,    -8,   271,    -4,    -2,   -16,
   285,   261,   270,   268,   269,   267,   266,   275,   -17,   -11,
   -19,   -13,   282,   272,   262,   -24,   -28,   260,   287,   262,
   263,   -22,   -24,   -24,   -12,   -12,   259,   265,   274,   257,
   -18,   -18,   -18,   -12,   262,   270,   262,   270,   270,   263,
   264,   270,    -3,   263,   282,   272,   273,   -14,   -15,    -6,
   -18,   279,   278,   262,   277,   262,    -5,    -7,   -17,   -17,
   -17,    -5,   -12,   282,   272,   273,   -24,   -24,   -24,   279,
   279,   279,   -28,   279,   -24,   -24,   -24,    -7 };
static YYCONST yytabelem yydef[]={

     1,    -2,    23,     0,     0,     0,     0,     0,     0,    23,
     0,     0,    24,     5,     6,    63,     7,     0,     0,    17,
     9,     2,    16,     3,     0,     0,     0,    21,     0,     0,
    25,    26,    28,    30,    32,    34,     0,     0,     0,    42,
    43,    44,     0,     0,     0,     0,     0,    46,     0,    53,
    54,    55,    56,    57,    58,    19,    23,    60,     8,     0,
     0,     4,    20,    18,    61,    62,     0,     0,     0,     0,
    35,    36,    37,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    27,    29,    31,
    33,    45,    38,     0,    40,     0,     0,    50,    52,    48,
    49,     0,    22,     0,     0,     0,    10,    11,    12,    39,
    41,    47,     0,    59,    13,    14,    15,    51 };
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
	"EXISTENTIAL",	266,
	"LEFTEXISTENTIAL",	267,
	"LEFTUNIVERSAL",	268,
	"UNIVERSAL",	269,
	"LPAREN",	270,
	"NEGATION",	271,
	"NUMBER",	272,
	"NEGNUMBER",	273,
	"OR",	274,
	"PIDENTIFIER",	275,
	"QUIT",	276,
	"RIGHTEXISTENTIAL",	277,
	"RIGHTUNIVERSAL",	278,
	"RPAREN",	279,
	"SEMICOLON",	280,
	"SOS",	281,
	"STRING",	282,
	"THEREFORE",	283,
	"QUERY",	284,
	"TRUE",	285,
	"LBRACE",	286,
	"RBRACE",	287,
	"OPTION",	288,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"start : /* empty */",
	"start : start argument sosconclusionlist rbrace",
	"start : start argument expressionlist rbrace",
	"start : start argument expressionlist sosconclusionlist rbrace",
	"start : start error RBRACE",
	"start : start QUIT linesynch",
	"start : start options RBRACE",
	"start : start options optionslist RBRACE",
	"options : OPTION LBRACE",
	"optionslist : IDENTIFIER EQUAL STRING linesynch",
	"optionslist : IDENTIFIER EQUAL NUMBER linesynch",
	"optionslist : IDENTIFIER EQUAL NEGNUMBER linesynch",
	"optionslist : optionslist IDENTIFIER EQUAL STRING linesynch",
	"optionslist : optionslist IDENTIFIER EQUAL NUMBER linesynch",
	"optionslist : optionslist IDENTIFIER EQUAL NEGNUMBER linesynch",
	"rbrace : RBRACE",
	"argument : ARGUMENT LBRACE",
	"sosconclusionlist : sos_option conclusiontype linesynch",
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
	"universal : UNIVERSAL LPAREN IDENTIFIER RPAREN",
	"existential : LEFTEXISTENTIAL IDENTIFIER RIGHTEXISTENTIAL",
	"existential : EXISTENTIAL LPAREN IDENTIFIER RPAREN",
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
	"conclusiontype : THEREFORE expression",
	"conclusiontype : QUERY expression",
	"linesynch : SEMICOLON",
};
#endif /* YYDEBUG */
# line	1 "/usr/ccs/bin/yaccpar"
/*
 * Copyright (c) 1993 by Sun Microsystems, Inc.
 */

#pragma ident	"@(#)yaccpar	6.16	99/01/20 SMI"

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
	(type *) memcpy(to, (char *) from, yymaxdepth * sizeof (type))
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
	register YYSTYPE *yypvt = 0;	/* top of value stack for $vars */

#if defined(__cplusplus) || defined(lint)
/*
	hacks to please C++ and lint - goto's inside
	switch should never be executed
*/
	static int __yaccpar_lint_hack__ = 0;
	switch (__yaccpar_lint_hack__)
	{
		case 1: goto yyerrlab;
		case 2: goto yynewstate;
	}
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
			long yyps_index = (yy_ps - yys);
			long yypv_index = (yy_pv - yyv);
			long yypvt_index = (yypvt - yyv);
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
				register YYCONST int *yyxi = yyexca;

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
		
case 5:
# line 108 "pr.y"
{
		cout << endl << "Syntax error detected ..." << endl;
		scope.clear();
		symbols.clear();
		ptrees.clear();
		variableNames.clear();
		yyerrok;
	} break;
case 6:
# line 117 "pr.y"
{
		cout << endl << "Quitting ..." << endl;
		scope.clear();
		symbols.clear();
		ptrees.clear();
		variableNames.clear();
		YYACCEPT;
	} break;
case 10:
# line 135 "pr.y"
{
		options[String(yypvt[-3].cstring)] = String(yypvt[-1].cstring);
	} break;
case 11:
# line 139 "pr.y"
{
		options[String(yypvt[-3].cstring)] = String(yypvt[-1].cstring);
	} break;
case 12:
# line 143 "pr.y"
{
		options[String(yypvt[-3].cstring)] = String(yypvt[-1].cstring);
	} break;
case 13:
# line 147 "pr.y"
{
		options[String(yypvt[-3].cstring)] = String(yypvt[-1].cstring);
	} break;
case 14:
# line 151 "pr.y"
{
		options[String(yypvt[-3].cstring)] = String(yypvt[-1].cstring);
	} break;
case 15:
# line 155 "pr.y"
{
		options[String(yypvt[-3].cstring)] = String(yypvt[-1].cstring);
	} break;
case 16:
# line 161 "pr.y"
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
	} break;
case 17:
# line 231 "pr.y"
{
		equality = 0;
	} break;
case 18:
# line 238 "pr.y"
{
		/* negate conclusion for refutation proof */
		Semantic *pnegs = new Semantic(Expression::Negation, NULL, yypvt[-1].psemantic);
		MustBeTrue(pnegs != NULL);
		
		/* set of support, conclusion and query */
		pnegs->setSOS(yypvt[-2].flag);
		pnegs->setConclusion(yypvt[-1].psemantic->getConclusion());
		pnegs->setQuery(yypvt[-1].psemantic->getQuery());

		// clear expression flags
		yypvt[-1].psemantic->setConclusion(0);
		yypvt[-1].psemantic->setQuery(0);

		/* save expression */
		MustBeTrue(ptrees.insertAtEnd(pnegs) == OK);
	} break;
case 21:
# line 264 "pr.y"
{
		/* is this a set of support */
		yypvt[-0].psemantic->setSOS(yypvt[-1].flag);

		/* save expression */
		MustBeTrue(ptrees.insertAtEnd(yypvt[-0].psemantic) == OK);
	} break;
case 22:
# line 272 "pr.y"
{
		/* is this a set of support */
		yypvt[-0].psemantic->setSOS(yypvt[-1].flag);

		/* save expression */
		MustBeTrue(ptrees.insertAtEnd(yypvt[-0].psemantic) == OK);
	} break;
case 23:
# line 283 "pr.y"
{
		yyval.flag = 0;
	} break;
case 24:
# line 287 "pr.y"
{
		yyval.flag = 1;
	} break;
case 25:
# line 294 "pr.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 26:
# line 301 "pr.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 27:
# line 305 "pr.y"
{
		/* create a biconditional record */
		yyval.psemantic = new Semantic(Expression::Biconditional, yypvt[-2].psemantic, yypvt[-0].psemantic);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 28:
# line 314 "pr.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 29:
# line 318 "pr.y"
{
		/* create a conditional record */
		yyval.psemantic = new Semantic(Expression::Conditional, yypvt[-2].psemantic, yypvt[-0].psemantic);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 30:
# line 327 "pr.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 31:
# line 331 "pr.y"
{
		/* create an OR record */
		yyval.psemantic = new Semantic(Expression::Or, yypvt[-2].psemantic, yypvt[-0].psemantic);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 32:
# line 340 "pr.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 33:
# line 344 "pr.y"
{
		/* create an AND record */
		yyval.psemantic = new Semantic(Expression::And, yypvt[-2].psemantic, yypvt[-0].psemantic);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 34:
# line 353 "pr.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 35:
# line 357 "pr.y"
{
		/* create an NEGATION record */
		yyval.psemantic = new Semantic(Expression::Negation, NULL, yypvt[-0].psemantic);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 36:
# line 363 "pr.y"
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
case 37:
# line 383 "pr.y"
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
case 38:
# line 406 "pr.y"
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
					"type changed for %s.\n", yypvt[-1].cstring);
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into symbol table */
			MustBeTrue(symbols.insert(Symbol(String(yypvt[-1].cstring), 
				Symbol::Variable)) == OK);
		}

		/* insert name into scope */
		MustBeTrue(scope.insertAtFront(Symbol(String(yypvt[-1].cstring), 
			Symbol::UniversalVariable)) == OK);
	} break;
case 39:
# line 436 "pr.y"
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
					"type changed for %s.\n", yypvt[-1].cstring);
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into symbol table */
			MustBeTrue(symbols.insert(Symbol(String(yypvt[-1].cstring), 
				Symbol::Variable)) == OK);
		}

		/* insert name into scope */
		MustBeTrue(scope.insertAtFront(Symbol(String(yypvt[-1].cstring), 
			Symbol::UniversalVariable)) == OK);
	} break;
case 40:
# line 469 "pr.y"
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
					"type changed for %s.\n", yypvt[-1].cstring);
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into symbol table */
			MustBeTrue(symbols.insert(Symbol(String(yypvt[-1].cstring), 
				Symbol::Variable)) == OK);
		}

		/* insert name into scope */
		MustBeTrue(scope.insertAtFront(Symbol(String(yypvt[-1].cstring),
			Symbol::ExistentialVariable)) == OK);
	} break;
case 41:
# line 499 "pr.y"
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
					"type changed for %s.\n", yypvt[-1].cstring);
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into symbol table */
			MustBeTrue(symbols.insert(Symbol(String(yypvt[-1].cstring), 
				Symbol::Variable)) == OK);
		}

		/* insert name into scope */
		MustBeTrue(scope.insertAtFront(Symbol(String(yypvt[-1].cstring),
			Symbol::ExistentialVariable)) == OK);
	} break;
case 42:
# line 532 "pr.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 43:
# line 536 "pr.y"
{
		/* create a predicate logical record */
		yyval.psemantic = new Semantic(Predicate::LogicalConstant, 
				String(""), String("True"));
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 44:
# line 543 "pr.y"
{
		/* create a logical constant record */
		yyval.psemantic = new Semantic(Predicate::LogicalConstant, 
				String(""), String("False"));
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 45:
# line 550 "pr.y"
{
		yyval.psemantic = yypvt[-1].psemantic;
	} break;
case 46:
# line 557 "pr.y"
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
			MustBeTrue(symbols.insert(newsym) == OK);
		}

		/* create a predicate constant record */
		yyval.psemantic = new Semantic(Predicate::Constant, String(yypvt[-0].cstring), String(""));
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 47:
# line 583 "pr.y"
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
			MustBeTrue(symbols.insert(newsym) == OK);
		}

		/* create a predicate record */
		yyval.psemantic = new Semantic(Predicate::Function, String(yypvt[-3].cstring), yypvt[-1].parguments, nargs);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 48:
# line 622 "pr.y"
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
case 49:
# line 643 "pr.y"
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
		yyval.psemantic = new Semantic(Predicate::Equal, String("="), pargs, nargs);
		MustBeTrue(yyval.psemantic != NULL);

		/* create a NEGATION record */
		yyval.psemantic = new Semantic(Expression::Negation, NULL, yyval.psemantic);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 50:
# line 671 "pr.y"
{
		/* create an argument list */
		yyval.parguments = new List<Semantic *>;
		MustBeTrue(yyval.parguments != NULL);

		/* insert an argument in the list */
		MustBeTrue(yyval.parguments->insertAtEnd(yypvt[-0].psemantic) == OK);
	} break;
case 51:
# line 680 "pr.y"
{
		/* insert an argument in the list */
		MustBeTrue(yyval.parguments->insertAtEnd(yypvt[-0].psemantic) == OK);
	} break;
case 52:
# line 688 "pr.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 53:
# line 695 "pr.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 54:
# line 699 "pr.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 55:
# line 703 "pr.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 56:
# line 710 "pr.y"
{
		/* create a string record */
		yyval.psemantic = new Semantic(Term::QuotedString, String(yypvt[-0].cstring));
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 57:
# line 716 "pr.y"
{
		/* create a number record */
		yyval.psemantic = new Semantic(Term::Number, String(yypvt[-0].cstring));
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 58:
# line 725 "pr.y"
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
				MustBeTrue(symbols.insert(Symbol(String(yypvt[-0].cstring), 
					Symbol::Constant)) == OK);
			}

			/* create a constant semantic record */
			yyval.psemantic = new Semantic(Term::Constant, String(yypvt[-0].cstring));
			MustBeTrue(yyval.psemantic != NULL);
		}
	} break;
case 59:
# line 788 "pr.y"
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
			MustBeTrue(symbols.insert(newsym) == OK);
		}

		/* create a function record */
		yyval.psemantic = new Semantic(Term::Function, String(yypvt[-3].cstring), yypvt[-1].parguments, nargs);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 61:
# line 834 "pr.y"
{
		yypvt[-0].psemantic->setConclusion(1);
		yypvt[-0].psemantic->setQuery(0);
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 62:
# line 840 "pr.y"
{
		yypvt[-0].psemantic->setConclusion(0);
		yypvt[-0].psemantic->setQuery(1);
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
# line	531 "/usr/ccs/bin/yaccpar"
	}
	goto yystack;		/* reset registers in driver code */
}

