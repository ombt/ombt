
# line 2 "folp.y"
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


# line 42 "folp.y"
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif
 {
	char cstring[32];
	Semantic *psemantic;
	List<Semantic * > *parguments;
} YYSTYPE;
# define AND 257
# define ARGUMENT 258
# define BICONDITIONAL 259
# define COLON 260
# define COMMA 261
# define EQUAL 262
# define EXISTENTIAL 263
# define FALSE 264
# define IDENTIFIER 265
# define IMPLICATION 266
# define LBRACE 267
# define LEFTEXISTENTIAL 268
# define LEFTUNIVERSAL 269
# define LPAREN 270
# define MINUS 271
# define NEGATION 272
# define NOTEQUAL 273
# define NUMBER 274
# define OPTION 275
# define OR 276
# define PIDENTIFIER 277
# define QUERY 278
# define QUIT 279
# define RBRACE 280
# define RIGHTEXISTENTIAL 281
# define RIGHTUNIVERSAL 282
# define RPAREN 283
# define SEMICOLON 284
# define SOS 285
# define STRING 286
# define THEREFORE 287
# define TRUE 288
# define UNIVERSAL 289

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

# line 779 "folp.y"


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

static YYCONST yytabelem yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 61
# define YYLAST 186
static YYCONST yytabelem yyact[]={

    44,    39,    52,    52,    19,    43,    41,    40,    19,    34,
    23,    51,    51,    18,    45,   102,    11,    18,    80,    99,
   102,    86,    77,    50,    50,    38,    42,    44,    39,    52,
    79,    98,    43,    41,    40,    85,    34,   103,    51,    88,
     9,    45,   100,     3,    61,     6,    74,    72,    71,    28,
    50,    70,    38,    42,    44,    39,    52,    22,    73,    43,
    41,    40,     7,    34,    68,    51,     4,    14,    45,    13,
    60,    89,    20,    87,    69,    67,    56,    50,    59,    38,
    42,    62,    46,    91,    10,    90,    32,    30,    31,    25,
    17,    29,    54,   101,    16,    21,    12,     5,    15,     8,
     2,    48,    53,    37,    49,    47,    24,    27,    33,    57,
    58,    26,    55,    35,    36,    76,     1,     0,     0,     0,
     0,    63,    64,    65,     0,     0,     0,     0,     0,     0,
    66,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    75,    78,     0,     0,    82,    84,
    83,    81,     0,     0,    92,    93,    94,    92,     0,     0,
    95,     0,     0,     0,    96,    97,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    92,   104 };
static YYCONST yytabelem yypact[]={

-10000000,  -213,-10000000,  -240,  -268,-10000000,  -198,  -200,  -274,-10000000,
-10000000,-10000000,  -208,-10000000,-10000000,  -270,  -236,  -268,-10000000,-10000000,
-10000000,-10000000,  -186,-10000000,  -268,  -268,-10000000,  -181,-10000000,  -196,
  -232,  -176,-10000000,-10000000,  -209,  -209,  -209,-10000000,-10000000,-10000000,
  -209,  -190,  -206,  -191,  -219,  -222,  -215,-10000000,-10000000,-10000000,
-10000000,-10000000,  -224,-10000000,  -263,  -263,  -256,-10000000,-10000000,  -209,
  -209,  -209,  -209,-10000000,-10000000,-10000000,  -248,  -261,  -192,  -242,
  -194,  -262,  -262,  -262,  -262,-10000000,-10000000,-10000000,-10000000,  -268,
  -268,  -196,  -232,  -176,-10000000,-10000000,-10000000,  -252,-10000000,  -264,
  -241,-10000000,-10000000,-10000000,-10000000,  -246,-10000000,-10000000,-10000000,-10000000,
-10000000,  -262,-10000000,-10000000,-10000000 };
static YYCONST yytabelem yypgo[]={

     0,   116,   115,   114,   113,   111,    85,    88,    83,   108,
   107,    90,   105,    89,   104,    91,    87,   103,    82,    86,
   101,   100,    99,    98,    84,    97,    96,    95,    92,    94,
    93 };
static YYCONST yytabelem yyr1[]={

     0,     1,     1,     1,     1,     1,    25,    26,    26,    27,
    27,    21,    23,    23,    11,    11,    28,    28,     2,    22,
    22,    29,    29,     5,    13,    10,    10,    15,    15,    16,
    16,     7,     7,    19,    19,    19,    19,     4,     4,     3,
     3,     9,     9,     9,     9,    17,    17,    17,    17,     6,
     6,     8,    18,    18,    18,    12,    12,    20,    14,    30,
    24 };
static YYCONST yytabelem yyr2[]={

     0,     1,    11,     7,     7,     8,     4,     0,     4,     9,
     9,     5,     5,     7,     7,     7,     1,     5,     3,     0,
     9,     1,     5,     3,     3,     3,     7,     3,     7,     3,
     7,     3,     7,     3,     5,     5,     5,     7,     9,     7,
     9,     3,     3,     3,     7,     3,     9,     7,     7,     3,
     7,     3,     3,     3,     3,     3,     3,     3,     9,     2,
     2 };
static YYCONST yytabelem yychk[]={

-10000000,    -1,   -21,   256,   279,   -25,   258,   275,   -22,   280,
   -24,   284,   -26,   267,   267,   -23,   -29,   -11,   287,   278,
   280,   -27,   265,   280,   -11,   -13,    -5,   -10,   285,   -15,
   -16,    -7,   -19,    -9,   272,    -4,    -3,   -17,   288,   264,
   270,   269,   289,   268,   263,   277,   -18,   -12,   -20,   -14,
   286,   274,   265,   -24,   -28,   -28,   262,   -24,   -24,   259,
   266,   276,   257,   -19,   -19,   -19,   -13,   265,   270,   265,
   270,   270,   262,   273,   270,   -13,    -2,   285,   -13,   286,
   274,   -15,   -16,    -7,   -19,   283,   282,   265,   281,   265,
    -6,    -8,   -18,   -18,   -18,    -6,   -24,   -24,   283,   283,
   283,   -30,   261,   283,    -8 };
static YYCONST yytabelem yydef[]={

     1,    -2,    19,     0,     0,     7,     0,     0,    21,     3,
     4,    60,     0,    11,     6,     0,     0,     0,    16,    16,
     5,     8,     0,     2,     0,     0,    22,    24,    23,    25,
    27,    29,    31,    33,     0,     0,     0,    41,    42,    43,
     0,     0,     0,     0,     0,    45,     0,    52,    53,    54,
    55,    56,    57,    12,     0,     0,     0,    13,    20,     0,
     0,     0,     0,    34,    35,    36,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    14,    17,    18,    15,     0,
     0,    26,    28,    30,    32,    44,    37,     0,    39,     0,
     0,    49,    51,    47,    48,     0,     9,    10,    38,    40,
    46,     0,    59,    58,    50 };
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
	"COLON",	260,
	"COMMA",	261,
	"EQUAL",	262,
	"EXISTENTIAL",	263,
	"FALSE",	264,
	"IDENTIFIER",	265,
	"IMPLICATION",	266,
	"LBRACE",	267,
	"LEFTEXISTENTIAL",	268,
	"LEFTUNIVERSAL",	269,
	"LPAREN",	270,
	"MINUS",	271,
	"NEGATION",	272,
	"NOTEQUAL",	273,
	"NUMBER",	274,
	"OPTION",	275,
	"OR",	276,
	"PIDENTIFIER",	277,
	"QUERY",	278,
	"QUIT",	279,
	"RBRACE",	280,
	"RIGHTEXISTENTIAL",	281,
	"RIGHTUNIVERSAL",	282,
	"RPAREN",	283,
	"SEMICOLON",	284,
	"SOS",	285,
	"STRING",	286,
	"THEREFORE",	287,
	"TRUE",	288,
	"UNIVERSAL",	289,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"start : /* empty */",
	"start : start argument expressionlist conclusionlist RBRACE",
	"start : start error RBRACE",
	"start : start QUIT linesynch",
	"start : start options optionslist RBRACE",
	"options : OPTION LBRACE",
	"optionslist : /* empty */",
	"optionslist : optionslist option",
	"option : IDENTIFIER EQUAL STRING linesynch",
	"option : IDENTIFIER EQUAL NUMBER linesynch",
	"argument : ARGUMENT LBRACE",
	"conclusionlist : conclusion linesynch",
	"conclusionlist : conclusionlist conclusion linesynch",
	"conclusion : THEREFORE coptionlist expression",
	"conclusion : QUERY coptionlist expression",
	"coptionlist : /* empty */",
	"coptionlist : coptionlist copt",
	"copt : SOS",
	"expressionlist : /* empty */",
	"expressionlist : expressionlist xoptionlist expression linesynch",
	"xoptionlist : /* empty */",
	"xoptionlist : xoptionlist xopt",
	"xopt : SOS",
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
		
case 1:
# line 106 "folp.y"
{
		clearAll();
	} break;
case 2:
# line 110 "folp.y"
{
		cout << endl << "Argument syntax ACCEPTED ..." << endl;
		execute();
		clearAll();
	} break;
case 3:
# line 116 "folp.y"
{
		cout << endl << "Argument syntax ERROR ..." << endl;
		clearAll();
		yyerrok;
	} break;
case 4:
# line 122 "folp.y"
{
		cout << endl << "Quitting ..." << endl;
		clearAll();
		YYACCEPT;
	} break;
case 9:
# line 141 "folp.y"
{
		options[yypvt[-3].cstring] = yypvt[-1].cstring;
	} break;
case 10:
# line 145 "folp.y"
{
		options[yypvt[-3].cstring] = yypvt[-1].cstring;
	} break;
case 11:
# line 152 "folp.y"
{
		equality = 0;
	} break;
case 12:
# line 159 "folp.y"
{
		/* save expression in conclusion list */
		MustBeTrue(conclusions.insertAtEnd(yypvt[-1].psemantic) == OK);
	} break;
case 13:
# line 164 "folp.y"
{
		/* save expression in conclusion list */
		MustBeTrue(conclusions.insertAtEnd(yypvt[-1].psemantic) == OK);
	} break;
case 14:
# line 172 "folp.y"
{
		yypvt[-0].psemantic->setConclusion(1);
		yypvt[-0].psemantic->setQuery(0);
		yypvt[-0].psemantic->setOptions(opts);
		yyval.psemantic = yypvt[-0].psemantic;
		opts.clear();
	} break;
case 15:
# line 180 "folp.y"
{
		yypvt[-0].psemantic->setConclusion(0);
		yypvt[-0].psemantic->setQuery(1);
		yypvt[-0].psemantic->setOptions(opts);
		yyval.psemantic = yypvt[-0].psemantic;
		opts.clear();
	} break;
case 16:
# line 191 "folp.y"
{
		opts.clear();
	} break;
case 17:
# line 195 "folp.y"
{
		MustBeTrue(opts.insert(yypvt[-0].cstring) == OK);
	} break;
case 18:
# line 202 "folp.y"
{
		strcpy(yyval.cstring, yypvt[-0].cstring);
	} break;
case 20:
# line 210 "folp.y"
{
		/* set options, SOS, conclusion or query */
		yypvt[-1].psemantic->setOptions(opts);
		opts.clear();

		/* save expression in conclusion list */
		MustBeTrue(premises.insertAtEnd(yypvt[-1].psemantic) == OK);
	} break;
case 21:
# line 222 "folp.y"
{
		opts.clear();
	} break;
case 22:
# line 226 "folp.y"
{
		MustBeTrue(opts.insert(yypvt[-0].cstring) == OK);
	} break;
case 23:
# line 233 "folp.y"
{
		strcpy(yyval.cstring, yypvt[-0].cstring);
	} break;
case 24:
# line 240 "folp.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 25:
# line 247 "folp.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 26:
# line 251 "folp.y"
{
		/* create a biconditional record */
		yyval.psemantic = new Semantic(Biconditional, yypvt[-2].psemantic, yypvt[-0].psemantic);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 27:
# line 260 "folp.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 28:
# line 264 "folp.y"
{
		/* create a conditional record */
		yyval.psemantic = new Semantic(Conditional, yypvt[-2].psemantic, yypvt[-0].psemantic);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 29:
# line 273 "folp.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 30:
# line 277 "folp.y"
{
		/* create an OR record */
		yyval.psemantic = new Semantic(Or, yypvt[-2].psemantic, yypvt[-0].psemantic);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 31:
# line 286 "folp.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 32:
# line 290 "folp.y"
{
		/* create an AND record */
		yyval.psemantic = new Semantic(And, yypvt[-2].psemantic, yypvt[-0].psemantic);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 33:
# line 299 "folp.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 34:
# line 303 "folp.y"
{
		/* create an NEGATION record */
		yyval.psemantic = new Semantic(Negation, NULL, yypvt[-0].psemantic);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 35:
# line 309 "folp.y"
{
		/* create a UNIVERSAL record */
		yyval.psemantic = new Semantic(Universal, String(yypvt[-1].cstring), yypvt[-0].psemantic);
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
case 36:
# line 329 "folp.y"
{
		/* create an EXISTENTIAL record */
		yyval.psemantic = new Semantic(Existential, String(yypvt[-1].cstring), yypvt[-0].psemantic);
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
# line 352 "folp.y"
{
		/* return string name */
		strcpy(yyval.cstring, yypvt[-1].cstring);

		/* check if symbol exists in global symbol table */
		Symbol newqvsym(String(yypvt[-1].cstring));
		if (symbols.retrieve(newqvsym) == OK)
		{
			/* symbol was found */
			if (newqvsym.getType() != Variable)
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
				Variable)) == OK);
		}

		/* insert name into scope */
		MustBeTrue(scope.insertAtFront(Symbol(String(yypvt[-1].cstring), 
			UniversalVariable)) == OK);
	} break;
case 38:
# line 382 "folp.y"
{
		/* return string name */
		strcpy(yyval.cstring, yypvt[-1].cstring);

		/* check if symbol exists in global symbol table */
		Symbol newqvsym(String(yypvt[-1].cstring));
		if (symbols.retrieve(newqvsym) == OK)
		{
			/* symbol was found */
			if (newqvsym.getType() != Variable)
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
				Variable)) == OK);
		}

		/* insert name into scope */
		MustBeTrue(scope.insertAtFront(Symbol(String(yypvt[-1].cstring), 
			UniversalVariable)) == OK);
	} break;
case 39:
# line 415 "folp.y"
{
		/* return string name */
		strcpy(yyval.cstring, yypvt[-1].cstring);

		/* check if symbol exists in global symbol table */
		Symbol newqvsym(String(yypvt[-1].cstring));
		if (symbols.retrieve(newqvsym) == OK)
		{
			/* symbol was found */
			if (newqvsym.getType() != Variable)
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
				Variable)) == OK);
		}

		/* insert name into scope */
		MustBeTrue(scope.insertAtFront(Symbol(String(yypvt[-1].cstring),
			ExistentialVariable)) == OK);
	} break;
case 40:
# line 445 "folp.y"
{
		/* return string name */
		strcpy(yyval.cstring, yypvt[-1].cstring);

		/* check if symbol exists in global symbol table */
		Symbol newqvsym(String(yypvt[-1].cstring));
		if (symbols.retrieve(newqvsym) == OK)
		{
			/* symbol was found */
			if (newqvsym.getType() != Variable)
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
				Variable)) == OK);
		}

		/* insert name into scope */
		MustBeTrue(scope.insertAtFront(Symbol(String(yypvt[-1].cstring),
			ExistentialVariable)) == OK);
	} break;
case 41:
# line 478 "folp.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 42:
# line 482 "folp.y"
{
		/* create a predicate logical record */
		yyval.psemantic = new Semantic(LogicalConstant, String(""), String("True"));
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 43:
# line 488 "folp.y"
{
		/* create a logical constant record */
		yyval.psemantic = new Semantic(LogicalConstant, String(""), String("False"));
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 44:
# line 494 "folp.y"
{
		yyval.psemantic = yypvt[-1].psemantic;
	} break;
case 45:
# line 501 "folp.y"
{
		/* check if symbol exists */
		Symbol newsym(String(yypvt[-0].cstring), PredicateConstant);
		if (symbols.retrieve(newsym) == OK)
		{
			/* symbol was found */
			if (newsym.getType() != PredicateConstant)
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
		yyval.psemantic = new Semantic(PredicateConstant, String(yypvt[-0].cstring), String(""));
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 46:
# line 527 "folp.y"
{
		/* count the number of arguments */
		int nargs = yypvt[-1].parguments->getCount();

		/* check if symbol exists */
		Symbol newsym(String(yypvt[-3].cstring), PredicateFunction, nargs);
		if (symbols.retrieve(newsym) == OK)
		{
			/* symbol was found, type should not change */
			if (newsym.getType() != PredicateFunction)
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
		yyval.psemantic = new Semantic(PredicateFunction, String(yypvt[-3].cstring), yypvt[-1].parguments, nargs);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 47:
# line 566 "folp.y"
{
		/* turn on uses-equality flag */
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
		yyval.psemantic = new Semantic(PredicateEqual, String(yypvt[-1].cstring), pargs, nargs);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 48:
# line 587 "folp.y"
{
		/* turn on uses-equality flag */
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
		yyval.psemantic = new Semantic(PredicateEqual, String("="), pargs, nargs);
		MustBeTrue(yyval.psemantic != NULL);

		/* create a NEGATION record */
		yyval.psemantic = new Semantic(Negation, NULL, yyval.psemantic);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 49:
# line 615 "folp.y"
{
		/* create an argument list */
		yyval.parguments = new List<Semantic *>;
		MustBeTrue(yyval.parguments != NULL);

		/* insert an argument in the list */
		MustBeTrue(yyval.parguments->insertAtEnd(yypvt[-0].psemantic) == OK);
	} break;
case 50:
# line 624 "folp.y"
{
		/* insert an argument in the list */
		MustBeTrue(yyval.parguments->insertAtEnd(yypvt[-0].psemantic) == OK);
	} break;
case 51:
# line 632 "folp.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 52:
# line 639 "folp.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 53:
# line 643 "folp.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 54:
# line 647 "folp.y"
{
		yyval.psemantic = yypvt[-0].psemantic;
	} break;
case 55:
# line 654 "folp.y"
{
		/* create a string record */
		yyval.psemantic = new Semantic(QuotedString, String(yypvt[-0].cstring));
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 56:
# line 660 "folp.y"
{
		/* create a number record */
		yyval.psemantic = new Semantic(Number, String(yypvt[-0].cstring));
		MustBeTrue(yyval.psemantic != NULL);
	} break;
case 57:
# line 669 "folp.y"
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
			if (sym.getType() != Variable)
			{
				sprintf(errormsg, 
					"symbol %s not a variable.\n", 
					yypvt[-0].cstring);
				yyerror(errormsg);
				YYERROR;
			}

			/* create a variable semantic record */
			yyval.psemantic = new Semantic(Variable, String(yypvt[-0].cstring));
			MustBeTrue(yyval.psemantic != NULL);
		}
		else
		{
			/* we have a constant, verify type did not change */
			Symbol sym(String(yypvt[-0].cstring));
			if (symbols.retrieve(sym) == OK)
			{
				if (sym.getType() != Constant)
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
					Constant)) == OK);
			}

			/* create a constant semantic record */
			yyval.psemantic = new Semantic(Constant, String(yypvt[-0].cstring));
			MustBeTrue(yyval.psemantic != NULL);
		}
	} break;
case 58:
# line 732 "folp.y"
{
		/* get number of arguments */
		int nargs = yypvt[-1].parguments->getCount();

		/* check if symbol exists */
		Symbol newsym(String(yypvt[-3].cstring), Function, nargs);
		if (symbols.retrieve(newsym) == OK)
		{
			/* symbol was found, type should not change */
			if (newsym.getType() != Function)
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
		yyval.psemantic = new Semantic(Function, String(yypvt[-3].cstring), yypvt[-1].parguments, nargs);
		MustBeTrue(yyval.psemantic != NULL);
	} break;
# line	531 "/usr/ccs/bin/yaccpar"
	}
	goto yystack;		/* reset registers in driver code */
}

