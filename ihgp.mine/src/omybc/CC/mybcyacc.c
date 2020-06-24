
# line 2 "mybc.y"
/* required headers */
#include <stdio.h>
#include <string.h>
#include <math.h>

/* other headers */
#include "mybc.h"

/* base for output */
long outbase = 10;

/* local definitions */
#undef YES
#define YES 1
#undef NO
#define NO 0


# line 23 "mybc.y"
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif
 {
	TYPE type;
	VALUE number; 
	char string[32];
} YYSTYPE;
# define NEWLINE 257
# define OR 258
# define AND 259
# define BITOR 260
# define BITXOR 261
# define BITAND 262
# define EQUAL 263
# define NOTEQUAL 264
# define LT 265
# define GT 266
# define LE 267
# define GE 268
# define LSHIFT 269
# define RSHIFT 270
# define PLUS 271
# define MINUS 272
# define STAR 273
# define SLASH 274
# define PERCENT 275
# define LPAREN 276
# define RPAREN 277
# define TILDE 278
# define NOT 279
# define STRING 280
# define QUIT 281
# define OUTBASE 282
# define HELP 283
# define EXP 284
# define LOG 285
# define LOG10 286
# define POW 287
# define SQRT 288
# define SIN 289
# define COS 290
# define TAN 291
# define ASIN 292
# define ACOS 293
# define ATAN 294
# define COMMA 295
# define ULCONST 296
# define LCONST 297
# define DCONST 298
# define ULCAST 299
# define LCAST 300
# define DCAST 301

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

# line 1928 "mybc.y"


VALUE
number2double(number)
VALUE number;
{
	switch (number.type)
	{
	case LONG:
		number.type = DOUBLE;
		number.value.dvalue = (double)number.value.lvalue;
		break;
	case ULONG:
		number.type = DOUBLE;
		number.value.dvalue = (double)number.value.ulvalue;
		break;
	case DOUBLE:
		break;
	default:
		number.type = UNKNOWN;
		break;
	}
	return(number);
}

char *
num2base(base, issigned, number)
unsigned long base;
int issigned;
unsigned long number;
{
	long ic, sign, quotient, remainder;
	static char buf[64];

	if (issigned == YES)
	{
		if ((long)number < 0)
		{
			sign = -1;
			number = -1*(long)number;
		}
		else
		{
			sign = 1;
		}
	}
	ic = sizeof(buf);
	buf[--ic] = 0;
	do {
		quotient = number/base;
		remainder = number % base;
		if (remainder < 10)
		{
			buf[--ic] = '0' + remainder;
		}
		else
		{
			buf[--ic] = 'A' + remainder - 10;
		}
	} while ((number = quotient) != 0);
	if (issigned == YES && sign == -1)
	{
		buf[--ic] = '-';
	}
	return(buf+ic);
}

#ifdef SUN
void
yyerror(s)
char *s;
{
	fprintf(stderr, "%s\n", s);
}
#endif

void
mybcerror(msg, lnno)
char *msg;
int lnno;
{
	char buf[BUFSIZ];
	sprintf(buf, "%d: %s", lnno, msg);
	yyerror(buf);
	return;
}

main()
{
#ifdef YYDEBUG
	extern int yydebug;
	yydebug = 1;
#endif
	yyparse();
	exit(0);
}
yytabelem yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 76
# define YYLAST 257
yytabelem yyact[]={

    23,    24,    46,   131,   140,    21,    95,    25,    26,   138,
   137,   136,   135,    28,    29,    30,    31,    32,    33,    34,
    35,    36,    37,    38,   134,    39,    40,    41,    78,    79,
    80,     4,     5,    58,   133,    47,    48,    49,    50,    51,
    52,    53,    54,    55,    56,    57,    23,    24,   132,   130,
   129,    21,    94,    25,    26,   128,     7,     8,     9,    28,
    29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
   115,    39,    40,    41,    23,    24,   114,    93,    92,    21,
    91,    25,    26,    73,    74,    75,    62,    28,    29,    30,
    31,    32,    33,    34,    35,    36,    37,    38,    90,    39,
    40,    41,    89,    88,    87,    86,    85,    71,    72,    69,
    70,    65,    66,    67,    68,    63,    64,    61,    60,    59,
    45,    44,    43,    42,     2,    19,    17,    15,    13,    11,
    18,    14,    12,    10,    76,    27,    22,    20,    16,     6,
     3,     1,     0,     0,     0,     0,    77,     0,     0,    81,
    82,    83,    84,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    97,
    99,   101,   102,    98,   100,     0,   107,   108,     0,   111,
   112,   113,   109,   110,   103,   104,   105,   106,     0,     0,
   116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
   126,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   127,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   139 };
yytabelem yypact[]={

-10000000,  -225,  -134,  -135,  -136,-10000000,  -138,-10000000,  -295,  -249,
  -140,  -142,  -144,  -176,  -148,  -154,  -160,  -164,  -190,-10000000,
-10000000,  -271,-10000000,  -197,  -197,  -197,  -197,-10000000,  -170,  -171,
  -172,  -173,  -174,  -178,  -196,  -198,  -199,  -224,  -270,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,  -197,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,  -197,
  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,  -197,
  -197,  -197,  -197,  -197,  -197,  -197,  -201,  -207,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,  -197,  -197,  -197,  -197,  -197,
  -197,  -197,  -197,  -197,  -197,  -197,  -140,  -142,  -144,  -176,
  -148,  -154,  -154,  -160,  -160,  -160,  -160,  -164,  -164,  -190,
  -190,-10000000,-10000000,-10000000,  -197,-10000000,  -222,  -227,  -228,  -292,
  -229,  -243,  -253,  -265,  -266,  -267,  -268,-10000000,-10000000,-10000000,
-10000000,  -197,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,  -273,
-10000000 };
yytabelem yypgo[]={

     0,   141,   124,   140,   139,   133,   129,   132,   128,   131,
   127,   138,   126,   130,   125,   137,   136,   135,   134 };
yytabelem yyr1[]={

     0,     1,     1,     1,     1,     1,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     2,     4,     4,     5,     5,     6,     6,     7,     7,
     8,     8,     9,     9,     9,    10,    10,    10,    10,    10,
    11,    11,    11,    12,    12,    12,    13,    13,    13,    13,
    14,    14,    15,    15,    15,    15,    15,    16,    16,    16,
    16,    16,    16,    16,    16,    16,    16,    16,    16,    16,
    17,    17,    17,    18,    18,    18 };
yytabelem yyr2[]={

     0,     0,     7,     7,     7,     5,     3,     5,     3,     5,
     5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
     5,     3,     3,     7,     3,     7,     3,     7,     3,     7,
     3,     7,     3,     7,     7,     3,     7,     7,     7,     7,
     3,     7,     7,     3,     7,     7,     3,     7,     7,     7,
     3,     9,     3,     5,     5,     5,     5,     3,     9,     9,
     9,    13,     9,     9,     9,     9,     9,     9,     9,     7,
     3,     3,     3,     3,     3,     3 };
yytabelem yychk[]={

-10000000,    -1,    -2,    -3,   256,   257,    -4,   281,   282,   283,
    -5,    -6,    -7,    -8,    -9,   -10,   -11,   -12,   -13,   -14,
   -15,   276,   -16,   271,   272,   278,   279,   -17,   284,   285,
   286,   287,   288,   289,   290,   291,   292,   293,   294,   296,
   297,   298,   257,   257,   257,   258,   297,   284,   285,   286,
   287,   288,   289,   290,   291,   292,   293,   294,   282,   259,
   260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
   270,   271,   272,   273,   274,   275,   -18,    -2,   299,   300,
   301,   -14,   -14,   -14,   -14,   276,   276,   276,   276,   276,
   276,   276,   276,   276,   276,   276,    -5,    -6,    -7,    -8,
    -9,   -10,   -10,   -11,   -11,   -11,   -11,   -12,   -12,   -13,
   -13,   -14,   -14,   -14,   277,   277,    -2,    -2,    -2,    -2,
    -2,    -2,    -2,    -2,    -2,    -2,    -2,   -14,   277,   277,
   277,   295,   277,   277,   277,   277,   277,   277,   277,    -2,
   277 };
yytabelem yydef[]={

     1,    -2,     0,     0,     0,     5,    21,     6,     0,     8,
    22,    24,    26,    28,    30,    32,    35,    40,    43,    46,
    50,     0,    52,     0,     0,     0,     0,    57,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    70,
    71,    72,     2,     3,     4,     0,     7,     9,    10,    11,
    12,    13,    14,    15,    16,    17,    18,    19,    20,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    73,    74,
    75,    53,    54,    55,    56,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    23,    25,    27,    29,
    31,    33,    34,    36,    37,    38,    39,    41,    42,    44,
    45,    47,    48,    49,     0,    69,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    51,    58,    59,
    60,     0,    62,    63,    64,    65,    66,    67,    68,     0,
    61 };
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
	"OR",	258,
	"AND",	259,
	"BITOR",	260,
	"BITXOR",	261,
	"BITAND",	262,
	"EQUAL",	263,
	"NOTEQUAL",	264,
	"LT",	265,
	"GT",	266,
	"LE",	267,
	"GE",	268,
	"LSHIFT",	269,
	"RSHIFT",	270,
	"PLUS",	271,
	"MINUS",	272,
	"STAR",	273,
	"SLASH",	274,
	"PERCENT",	275,
	"LPAREN",	276,
	"RPAREN",	277,
	"TILDE",	278,
	"NOT",	279,
	"STRING",	280,
	"QUIT",	281,
	"OUTBASE",	282,
	"HELP",	283,
	"EXP",	284,
	"LOG",	285,
	"LOG10",	286,
	"POW",	287,
	"SQRT",	288,
	"SIN",	289,
	"COS",	290,
	"TAN",	291,
	"ASIN",	292,
	"ACOS",	293,
	"ATAN",	294,
	"COMMA",	295,
	"ULCONST",	296,
	"LCONST",	297,
	"DCONST",	298,
	"ULCAST",	299,
	"LCAST",	300,
	"DCAST",	301,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"start : /* empty */",
	"start : start expr NEWLINE",
	"start : start cmd NEWLINE",
	"start : start error NEWLINE",
	"start : start NEWLINE",
	"cmd : QUIT",
	"cmd : OUTBASE LCONST",
	"cmd : HELP",
	"cmd : HELP EXP",
	"cmd : HELP LOG",
	"cmd : HELP LOG10",
	"cmd : HELP POW",
	"cmd : HELP SQRT",
	"cmd : HELP SIN",
	"cmd : HELP COS",
	"cmd : HELP TAN",
	"cmd : HELP ASIN",
	"cmd : HELP ACOS",
	"cmd : HELP ATAN",
	"cmd : HELP OUTBASE",
	"expr : or",
	"or : and",
	"or : or OR and",
	"and : bitor",
	"and : and AND bitor",
	"bitor : bitxor",
	"bitor : bitor BITOR bitxor",
	"bitxor : bitand",
	"bitxor : bitxor BITXOR bitand",
	"bitand : equality",
	"bitand : bitand BITAND equality",
	"equality : rel",
	"equality : equality EQUAL rel",
	"equality : equality NOTEQUAL rel",
	"rel : shift",
	"rel : rel LT shift",
	"rel : rel GT shift",
	"rel : rel LE shift",
	"rel : rel GE shift",
	"shift : add",
	"shift : shift LSHIFT add",
	"shift : shift RSHIFT add",
	"add : mult",
	"add : add PLUS mult",
	"add : add MINUS mult",
	"mult : cast",
	"mult : mult STAR cast",
	"mult : mult SLASH cast",
	"mult : mult PERCENT cast",
	"cast : unary",
	"cast : LPAREN type RPAREN cast",
	"unary : primary",
	"unary : PLUS cast",
	"unary : MINUS cast",
	"unary : TILDE cast",
	"unary : NOT cast",
	"primary : const",
	"primary : EXP LPAREN expr RPAREN",
	"primary : LOG LPAREN expr RPAREN",
	"primary : LOG10 LPAREN expr RPAREN",
	"primary : POW LPAREN expr COMMA expr RPAREN",
	"primary : SQRT LPAREN expr RPAREN",
	"primary : SIN LPAREN expr RPAREN",
	"primary : COS LPAREN expr RPAREN",
	"primary : TAN LPAREN expr RPAREN",
	"primary : ASIN LPAREN expr RPAREN",
	"primary : ACOS LPAREN expr RPAREN",
	"primary : ATAN LPAREN expr RPAREN",
	"primary : LPAREN expr RPAREN",
	"const : ULCONST",
	"const : LCONST",
	"const : DCONST",
	"type : ULCAST",
	"type : LCAST",
	"type : DCAST",
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
# line 98 "mybc.y"
{
		switch (yypvt[-1].number.type)
		{
		case ULONG:
#if 0
			switch (outbase)
			{
			case 8:
				printf("%lo\n", yypvt[-1].number.value.ulvalue);
				break;
			case 10:
				printf("%lu\n", yypvt[-1].number.value.ulvalue);
				break;
			case 16:
				printf("%lx\n", yypvt[-1].number.value.ulvalue);
				break;
			default:
				printf("%d: default case.\n", __LINE__);
				YYERROR;
			}
#else
			printf("%s\n", num2base(outbase, NO, yypvt[-1].number.value.ulvalue));
#endif
			break;
		case LONG:
#if 0
			switch (outbase)
			{
			case 8:
				printf("%lo\n", yypvt[-1].number.value.lvalue);
				break;
			case 10:
				printf("%ld\n", yypvt[-1].number.value.lvalue);
				break;
			case 16:
				printf("%lx\n", yypvt[-1].number.value.lvalue);
				break;
			default:
				printf("%d: default case.\n", __LINE__);
				YYERROR;
			}
#else
			printf("%s\n", num2base(outbase, YES, yypvt[-1].number.value.lvalue));
#endif
			break;
		case DOUBLE:
			switch (outbase)
			{
			case 2:
			case 8:
			case 16:
				printf("base = 10 for double.\n");
			case 10:
				printf("%le\n", yypvt[-1].number.value.dvalue);
				break;
			default:
				printf("%d: default case.\n", __LINE__);
				YYERROR;
			}
			break;
		default:
			printf("%d: default case.\n", __LINE__);
			YYERROR;
		}
	} break;
case 3:
# line 164 "mybc.y"
{
		/* do nothing */
	} break;
case 4:
# line 168 "mybc.y"
{
		yyerrok;
	} break;
case 5:
# line 172 "mybc.y"
{
		yyerrok;
	} break;
case 6:
# line 179 "mybc.y"
{
		YYACCEPT;
	} break;
case 7:
# line 183 "mybc.y"
{
		switch (yypvt[-0].number.value.lvalue)
		{
		case 2:
		case 8:
		case 10:
		case 16:
			outbase = yypvt[-0].number.value.lvalue;
			break;
		default:
			yyerror("allowed bases are 2, 8, 10, 16.");
			YYERROR;
		}
		strcpy(yyval.string, yypvt[-1].string);
	} break;
case 8:
# line 199 "mybc.y"
{
		printf("mybc supports the following operators and math\n");
		printf("functions.\n");
		printf("\n");
		printf("operators:\n");
		printf("|| && | ^ & == != < > <= >= << >> - * /\n");
		printf("%% ( ) ~ !\n");
		printf("\n");
		printf("math functions:\n");
		printf("exp log log10 pow sqrt sin cos tan\n");
		printf("asin acos atan\n");
		printf("\n");
		printf("miscellaneous:\n");
		printf("(l) - long typecast\n");
		printf("(ul) - unsigned long typecast\n");
		printf("(d) - double typecast\n");
		printf("[xh][0-9a-z]* - hex numbers\n");
		printf("o[0-7]* - octal numbers\n");
		printf("b[01]* - binary numbers\n");
		printf("[+|-]?[0-9]+.[0-9]+([eE][+-]?[0-9]+)? - real numbers\n");
		printf("ob [2|8|10|16] - set output base\n");
		printf("? - help\n");
		strcpy(yyval.string, yypvt[-0].string);
	} break;
case 9:
# line 224 "mybc.y"
{
		printf("usage: exp(expr)\n");
	} break;
case 10:
# line 228 "mybc.y"
{
		printf("usage: log(expr)\n");
	} break;
case 11:
# line 232 "mybc.y"
{
		printf("usage: log10(expr)\n");
	} break;
case 12:
# line 236 "mybc.y"
{
		printf("usage: pow(base_expr, power_expr)\n");
	} break;
case 13:
# line 240 "mybc.y"
{
		printf("usage: sqrt(expr)\n");
	} break;
case 14:
# line 244 "mybc.y"
{
		printf("usage: sin(expr)\n");
	} break;
case 15:
# line 248 "mybc.y"
{
		printf("usage: cos(expr)\n");
	} break;
case 16:
# line 252 "mybc.y"
{
		printf("usage: tan(expr)\n");
	} break;
case 17:
# line 256 "mybc.y"
{
		printf("usage: asin(expr)\n");
	} break;
case 18:
# line 260 "mybc.y"
{
		printf("usage: acos(expr)\n");
	} break;
case 19:
# line 264 "mybc.y"
{
		printf("usage: atan(expr)\n");
	} break;
case 20:
# line 268 "mybc.y"
{
		printf("usage: ob [2|8|10|16]\n");
	} break;
case 21:
# line 275 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 22:
# line 282 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 23:
# line 286 "mybc.y"
{
		yyval.number.type = LONG;
		switch (yypvt[-2].number.type)
		{
		case ULONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.ulvalue || yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.ulvalue || yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.ulvalue || yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue || yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue || yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue || yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.dvalue || yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.dvalue || yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.dvalue || yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
	} break;
case 24:
# line 359 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 25:
# line 363 "mybc.y"
{
		yyval.number.type = LONG;
		switch (yypvt[-2].number.type)
		{
		case ULONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.ulvalue && yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.ulvalue && yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.ulvalue && yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue && yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue && yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue && yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.dvalue && yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.dvalue && yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.dvalue && yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
	} break;
case 26:
# line 436 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 27:
# line 440 "mybc.y"
{
		switch (yypvt[-2].number.type)
		{
		case ULONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.type = ULONG;
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.ulvalue | yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = ULONG;
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.ulvalue | yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyerror("| cannot be applied to double.");
				YYERROR;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.type = ULONG;
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.lvalue | yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = LONG;
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue | yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyerror("| cannot be applied to double.");
				YYERROR;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror("| cannot be applied to double.");
			YYERROR;
		default:
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
	} break;
case 28:
# line 497 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 29:
# line 501 "mybc.y"
{
		switch (yypvt[-2].number.type)
		{
		case ULONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.type = ULONG;
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.ulvalue ^ yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = ULONG;
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.ulvalue ^ yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyerror("^ cannot be applied to double.");
				YYERROR;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.type = ULONG;
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.lvalue ^ yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = LONG;
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue ^ yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyerror("^ cannot be applied to double.");
				YYERROR;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror("^ cannot be applied to double.");
			YYERROR;
		default:
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
	} break;
case 30:
# line 558 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 31:
# line 562 "mybc.y"
{
		switch (yypvt[-2].number.type)
		{
		case ULONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.type = ULONG;
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.ulvalue & yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = ULONG;
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.ulvalue & yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyerror("& cannot be applied to double.");
				YYERROR;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.type = ULONG;
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.lvalue & yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = LONG;
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue & yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyerror("& cannot be applied to double.");
				YYERROR;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror("& cannot be applied to double.");
			YYERROR;
		default:
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
	} break;
case 32:
# line 619 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 33:
# line 623 "mybc.y"
{
		yyval.number.type = LONG;
		switch (yypvt[-2].number.type)
		{
		case ULONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.ulvalue == yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.ulvalue == yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.ulvalue == yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue == yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue == yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue == yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.dvalue == yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.dvalue == yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.dvalue == yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
	} break;
case 34:
# line 693 "mybc.y"
{
		yyval.number.type = LONG;
		switch (yypvt[-2].number.type)
		{
		case ULONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.ulvalue != yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.ulvalue != yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.ulvalue != yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue != yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue != yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue != yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.dvalue != yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.dvalue != yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.dvalue != yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
	} break;
case 35:
# line 766 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 36:
# line 770 "mybc.y"
{
		yyval.number.type = LONG;
		switch (yypvt[-2].number.type)
		{
		case ULONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.ulvalue < yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.ulvalue < yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.ulvalue < yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue < yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue < yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue < yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.dvalue < yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.dvalue < yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.dvalue < yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
	} break;
case 37:
# line 840 "mybc.y"
{
		yyval.number.type = LONG;
		switch (yypvt[-2].number.type)
		{
		case ULONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.ulvalue > yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.ulvalue > yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.ulvalue > yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue > yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue > yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue > yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.dvalue > yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.dvalue > yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.dvalue > yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
	} break;
case 38:
# line 910 "mybc.y"
{
		yyval.number.type = LONG;
		switch (yypvt[-2].number.type)
		{
		case ULONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.ulvalue <= yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.ulvalue <= yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.ulvalue <= yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue <= yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue <= yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue <= yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.dvalue <= yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.dvalue <= yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.dvalue <= yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
	} break;
case 39:
# line 980 "mybc.y"
{
		yyval.number.type = LONG;
		switch (yypvt[-2].number.type)
		{
		case ULONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.ulvalue >= yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.ulvalue >= yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.ulvalue >= yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue >= yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue >= yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue >= yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.dvalue >= yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.dvalue >= yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.dvalue >= yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
	} break;
case 40:
# line 1053 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 41:
# line 1057 "mybc.y"
{
		switch (yypvt[-2].number.type)
		{
		case ULONG:
			yyval.number.type = ULONG;
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.ulvalue << yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.ulvalue << yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyerror("<< cannot be applied to double.");
				YYERROR;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			yyval.number.type = LONG;
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue << yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue << yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyerror("<< cannot be applied to double.");
				YYERROR;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror("<< cannot be applied to double.");
			YYERROR;
		default:
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
	} break;
case 42:
# line 1109 "mybc.y"
{
		switch (yypvt[-2].number.type)
		{
		case ULONG:
			yyval.number.type = ULONG;
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.ulvalue >> yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.ulvalue >> yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyerror(">> cannot be applied to double.");
				YYERROR;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			yyval.number.type = LONG;
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue >> yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue >> yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyerror(">> cannot be applied to double.");
				YYERROR;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror(">> cannot be applied to double.");
			YYERROR;
		default:
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
	} break;
case 43:
# line 1164 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 44:
# line 1168 "mybc.y"
{
		switch (yypvt[-2].number.type)
		{
		case ULONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.type = ULONG;
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.ulvalue + yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = ULONG;
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.ulvalue + yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.type = DOUBLE;
				yyval.number.value.dvalue =
					yypvt[-2].number.value.ulvalue + yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.type = ULONG;
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.lvalue + yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = LONG;
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue + yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.type = DOUBLE;
				yyval.number.value.dvalue =
					yypvt[-2].number.value.lvalue + yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.type = DOUBLE;
				yyval.number.value.dvalue =
					yypvt[-2].number.value.dvalue + yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = DOUBLE;
				yyval.number.value.dvalue =
					yypvt[-2].number.value.dvalue + yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.type = DOUBLE;
				yyval.number.value.dvalue =
					yypvt[-2].number.value.dvalue + yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
	} break;
case 45:
# line 1246 "mybc.y"
{
		switch (yypvt[-2].number.type)
		{
		case ULONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.type = ULONG;
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.ulvalue - yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = ULONG;
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.ulvalue - yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.type = DOUBLE;
				yyval.number.value.dvalue =
					yypvt[-2].number.value.ulvalue - yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.type = ULONG;
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.lvalue - yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = LONG;
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue - yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.type = DOUBLE;
				yyval.number.value.dvalue =
					yypvt[-2].number.value.lvalue - yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.type = DOUBLE;
				yyval.number.value.dvalue =
					yypvt[-2].number.value.dvalue - yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = DOUBLE;
				yyval.number.value.dvalue =
					yypvt[-2].number.value.dvalue - yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.type = DOUBLE;
				yyval.number.value.dvalue =
					yypvt[-2].number.value.dvalue - yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
	} break;
case 46:
# line 1327 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 47:
# line 1331 "mybc.y"
{
		switch (yypvt[-2].number.type)
		{
		case ULONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.type = ULONG;
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.ulvalue * yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = ULONG;
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.ulvalue * yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.type = DOUBLE;
				yyval.number.value.dvalue =
					yypvt[-2].number.value.ulvalue * yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.type = ULONG;
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.lvalue * yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = LONG;
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue * yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.type = DOUBLE;
				yyval.number.value.dvalue =
					yypvt[-2].number.value.lvalue * yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.type = DOUBLE;
				yyval.number.value.dvalue =
					yypvt[-2].number.value.dvalue * yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = DOUBLE;
				yyval.number.value.dvalue =
					yypvt[-2].number.value.dvalue * yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.type = DOUBLE;
				yyval.number.value.dvalue =
					yypvt[-2].number.value.dvalue * yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
	} break;
case 48:
# line 1409 "mybc.y"
{
		switch (yypvt[-2].number.type)
		{
		case ULONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.type = ULONG;
				if (yypvt[-0].number.value.ulvalue == 0)
				{
					mybcerror("divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.ulvalue / yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = ULONG;
				if (yypvt[-0].number.value.lvalue == 0)
				{
					mybcerror("divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.ulvalue / yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.type = DOUBLE;
				if (yypvt[-0].number.value.dvalue == 0)
				{
					mybcerror("divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.dvalue =
					yypvt[-2].number.value.ulvalue / yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.type = ULONG;
				if (yypvt[-0].number.value.ulvalue == 0)
				{
					mybcerror("divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.lvalue / yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = LONG;
				if (yypvt[-0].number.value.lvalue == 0)
				{
					mybcerror("divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue / yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.type = DOUBLE;
				if (yypvt[-0].number.value.dvalue == 0)
				{
					mybcerror("divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.dvalue =
					yypvt[-2].number.value.lvalue / yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.type = DOUBLE;
				if (yypvt[-0].number.value.ulvalue == 0)
				{
					mybcerror("divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.dvalue =
					yypvt[-2].number.value.dvalue / yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = DOUBLE;
				if (yypvt[-0].number.value.lvalue == 0)
				{
					mybcerror("divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.dvalue =
					yypvt[-2].number.value.dvalue / yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.type = DOUBLE;
				if (yypvt[-0].number.value.dvalue == 0)
				{
					mybcerror("divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.dvalue =
					yypvt[-2].number.value.dvalue / yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
	} break;
case 49:
# line 1532 "mybc.y"
{
		switch (yypvt[-2].number.type)
		{
		case ULONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.type = ULONG;
				if (yypvt[-0].number.value.ulvalue == 0)
				{
					mybcerror("divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.ulvalue % yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = ULONG;
				if (yypvt[-0].number.value.lvalue == 0)
				{
					mybcerror("divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.ulvalue % yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyerror("% cannot be applied to double.");
				YYERROR;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.type = ULONG;
				if (yypvt[-0].number.value.ulvalue == 0)
				{
					mybcerror("divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.lvalue % yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = LONG;
				if (yypvt[-0].number.value.lvalue == 0)
				{
					mybcerror("divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue % yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyerror("% cannot be applied to double.");
				YYERROR;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror("% cannot be applied to double.");
			YYERROR;
		default:
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
	} break;
case 50:
# line 1609 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 51:
# line 1613 "mybc.y"
{
		switch (yypvt[-2].type)
		{
		case ULONG:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number = yypvt[-0].number;
				break;
			case LONG:
				yyval.number.type = ULONG;
				yyval.number.value.ulvalue = 
					(unsigned long)yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.type = ULONG;
				yyval.number.value.ulvalue = 
					(unsigned long)yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch (yypvt[-0].number.type)
			{
			case LONG:
				yyval.number = yypvt[-0].number;
				break;
			case ULONG:
				yyval.number.type = LONG;
				yyval.number.value.lvalue = (long)yypvt[-0].number.value.ulvalue;
				break;
			case DOUBLE:
				yyval.number.type = LONG;
				yyval.number.value.lvalue = (long)yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.type = DOUBLE;
				yyval.number.value.dvalue = (double)yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = DOUBLE;
				yyval.number.value.dvalue = (double)yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number = yypvt[-0].number;
				break;
			default:
				mybcerror("default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
	} break;
case 52:
# line 1684 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 53:
# line 1688 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 54:
# line 1692 "mybc.y"
{
		switch (yypvt[-0].number.type)
		{
		case ULONG:
			yypvt[-0].number.value.ulvalue *= -1;
			break;
		case DOUBLE:
			yypvt[-0].number.value.dvalue *= -1.0;
			break;
		case LONG:
			yypvt[-0].number.value.lvalue *= -1;
			break;
		default:
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
		yyval.number = yypvt[-0].number;
	} break;
case 55:
# line 1711 "mybc.y"
{
		switch (yypvt[-0].number.type)
		{
		case ULONG:
			yypvt[-0].number.value.ulvalue = ~(yypvt[-0].number.value.ulvalue);
			break;
		case DOUBLE:
			yyerror("~ cannot be applied to double.");
			YYERROR;
		case LONG:
			yypvt[-0].number.value.lvalue = ~(yypvt[-0].number.value.lvalue);
			break;
		default:
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
		yyval.number = yypvt[-0].number;
	} break;
case 56:
# line 1730 "mybc.y"
{
		switch (yypvt[-0].number.type)
		{
		case ULONG:
			yypvt[-0].number.value.ulvalue = !(yypvt[-0].number.value.ulvalue);
			break;
		case DOUBLE:
			yypvt[-0].number.value.dvalue = !(yypvt[-0].number.value.dvalue);
			break;
		case LONG:
			yypvt[-0].number.value.lvalue = !(yypvt[-0].number.value.lvalue);
			break;
		default:
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
		yyval.number = yypvt[-0].number;
	} break;
case 57:
# line 1752 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 58:
# line 1756 "mybc.y"
{
		yypvt[-1].number = number2double(yypvt[-1].number);
		if (yypvt[-1].number.type != DOUBLE)
		{
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = exp(yypvt[-1].number.value.dvalue);
	} break;
case 59:
# line 1767 "mybc.y"
{
		yypvt[-1].number = number2double(yypvt[-1].number);
		if (yypvt[-1].number.type != DOUBLE)
		{
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
		if (yypvt[-1].number.value.dvalue < 0)
		{
			mybcerror("log of negative number.", __LINE__);
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = log(yypvt[-1].number.value.dvalue);
	} break;
case 60:
# line 1783 "mybc.y"
{
		yypvt[-1].number = number2double(yypvt[-1].number);
		if (yypvt[-1].number.type != DOUBLE)
		{
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
		if (yypvt[-1].number.value.dvalue < 0)
		{
			mybcerror("log10 of negative number.", __LINE__);
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = log10(yypvt[-1].number.value.dvalue);
	} break;
case 61:
# line 1799 "mybc.y"
{
		yypvt[-3].number = number2double(yypvt[-3].number);
		if (yypvt[-3].number.type != DOUBLE)
		{
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
		yypvt[-1].number = number2double(yypvt[-1].number);
		if (yypvt[-1].number.type != DOUBLE)
		{
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = pow(yypvt[-3].number.value.dvalue, yypvt[-1].number.value.dvalue);
	} break;
case 62:
# line 1816 "mybc.y"
{
		yypvt[-1].number = number2double(yypvt[-1].number);
		if (yypvt[-1].number.type != DOUBLE)
		{
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = sqrt(yypvt[-1].number.value.dvalue);
	} break;
case 63:
# line 1827 "mybc.y"
{
		yypvt[-1].number = number2double(yypvt[-1].number);
		if (yypvt[-1].number.type != DOUBLE)
		{
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = sin(yypvt[-1].number.value.dvalue);
	} break;
case 64:
# line 1838 "mybc.y"
{
		yypvt[-1].number = number2double(yypvt[-1].number);
		if (yypvt[-1].number.type != DOUBLE)
		{
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = cos(yypvt[-1].number.value.dvalue);
	} break;
case 65:
# line 1849 "mybc.y"
{
		yypvt[-1].number = number2double(yypvt[-1].number);
		if (yypvt[-1].number.type != DOUBLE)
		{
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = tan(yypvt[-1].number.value.dvalue);
	} break;
case 66:
# line 1860 "mybc.y"
{
		yypvt[-1].number = number2double(yypvt[-1].number);
		if (yypvt[-1].number.type != DOUBLE)
		{
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = asin(yypvt[-1].number.value.dvalue);
	} break;
case 67:
# line 1871 "mybc.y"
{
		yypvt[-1].number = number2double(yypvt[-1].number);
		if (yypvt[-1].number.type != DOUBLE)
		{
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = acos(yypvt[-1].number.value.dvalue);
	} break;
case 68:
# line 1882 "mybc.y"
{
		yypvt[-1].number = number2double(yypvt[-1].number);
		if (yypvt[-1].number.type != DOUBLE)
		{
			mybcerror("default case.", __LINE__);
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = atan(yypvt[-1].number.value.dvalue);
	} break;
case 69:
# line 1893 "mybc.y"
{
		yyval.number = yypvt[-1].number;
	} break;
case 70:
# line 1900 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 71:
# line 1904 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 72:
# line 1908 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 73:
# line 1915 "mybc.y"
{
		yyval.type = ULONG;
	} break;
case 74:
# line 1919 "mybc.y"
{
		yyval.type = LONG;
	} break;
case 75:
# line 1923 "mybc.y"
{
		yyval.type = DOUBLE;
	} break;
# line	532 "/usr/ccs/bin/yaccpar"
	}
	goto yystack;		/* reset registers in driver code */
}

