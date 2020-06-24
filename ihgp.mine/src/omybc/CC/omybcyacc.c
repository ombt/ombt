
# line 2 "omybc.y"
/* required headers */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

/* other headers */
#include "omybc.h"

/* globals */
long outbase = 10;
double xy[NUMBEROFPOINTS][2];
int xyno;

/* externs */
void yyerror(const char *);

/* local definitions */
#undef YES
#define YES 1
#undef NO
#define NO 0


# line 29 "omybc.y"
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif
 {
	TYPE type;
	VALUE number; 
	char string[32];
} YYSTYPE;
# define DCONST 257
# define LCONST 258
# define ULCONST 259
# define ACOS 260
# define AND 261
# define ASIN 262
# define ATAN 263
# define BITAND 264
# define BITOR 265
# define BITXOR 266
# define COMMA 267
# define COS 268
# define EQUAL 269
# define EXP 270
# define GE 271
# define GRAPH 272
# define GT 273
# define HELP 274
# define LE 275
# define LOG 276
# define LOG10 277
# define LPAREN 278
# define LSHIFT 279
# define LT 280
# define MINUS 281
# define NEWLINE 282
# define NOT 283
# define NOTEQUAL 284
# define OR 285
# define OUTBASE 286
# define PERCENT 287
# define PLUS 288
# define POW 289
# define QUIT 290
# define RPAREN 291
# define RSHIFT 292
# define SEMICOLON 293
# define SIN 294
# define SLASH 295
# define SQRT 296
# define STAR 297
# define STRING 298
# define TAN 299
# define TILDE 300
# define DCAST 301
# define LCAST 302
# define ULCAST 303

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

# line 1963 "omybc.y"


void
initgraph()
{
	xyno = 0;
}

void
storexy(VALUE x, VALUE y)
{
	if (xyno < NUMBEROFPOINTS)
	{
		xy[xyno][0] = x.value.dvalue;
		xy[xyno][1] = y.value.dvalue;
		xyno++;
	}
	else
	{
		fprintf(stderr, "ERROR: no room for storing (x,y) point.\n");
	}
	return;
}

void
graph()
{
	int ixy, ix, iy, px, py;
	char xymap[XPIXNUM][YPIXNUM+1];
	double dx, dy, xmin, ymin, xmax, ymax;
	int ixmin, iymin, ixmax, iymax;

	/* dump data for now */
	for (ixy = 0; ixy < xyno; ixy++)
	{
		printf("%03d: (%e, %e)\n", ixy, xy[ixy][0], xy[ixy][1]);
	}

	/* initialize graph */	
	for (ix = 0; ix < XPIXNUM; ix++)
	{
		for (iy = 0; iy < YPIXNUM; iy++)
		{
			xymap[ix][iy] = ' ';
		}
		xymap[ix][iy] = 0;
	}

	/* get min and max */
	xmin = xy[0][0];
	ymin = xy[0][1];
	xmax = xy[0][0];
	ymax = xy[0][1];
	for (ix = 1; ix < xyno; ix++)
	{
		if (xmin > xy[ix][0])
			xmin = xy[ix][0];
		if (ymin > xy[ix][1])
			ymin = xy[ix][1];
		if (xmax < xy[ix][0])
			xmax = xy[ix][0];
		if (ymax < xy[ix][1])
			ymax = xy[ix][1];
	}

	/* calculate the deltas */
	dx = (xmax - xmin)/((double)XPIXNUM);
	dy = (ymax - ymin)/((double)YPIXNUM);

	/* plot points in map */
	for (ix = 0; ix < xyno; ix++)
	{
		px = (xy[ix][0] - xmin)/dx;
		py = (xy[ix][1] - ymin)/dy;
		if (px < 0) px = 0;
		if (px >= XPIXNUM) px = XPIXNUM-1;
		if (py < 0) py = 0;
		if (py >= YPIXNUM) py = YPIXNUM-1;
		xymap[px][py] = '*';
	}

	/* print graph */
	fprintf(stdout, "\n");
	for (iy = 0; iy < (YPIXNUM+5); iy++)
	{
		fprintf(stdout, "-");
	}
	fprintf(stdout, "\n");
	for (ix = 0; ix < XPIXNUM; ix++)
	{
		fprintf(stdout, "%03d| %s\n", ix, &xymap[ix][0]);
	}
	for (iy = 0; iy < (YPIXNUM+5); iy++)
	{
		fprintf(stdout, "-");
	}
	fprintf(stdout, "\n");

	/* all done */
	return;
}

VALUE
number2double(VALUE number)
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
num2base(unsigned long base, int issigned, unsigned long number)
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
yyerror(const char *s)
{
	fprintf(stderr, "%s\n", s);
}
#endif

void
mybcerror(const char *msg, int lnno)
{
	char buf[BUFSIZ];
	sprintf(buf, "%d: %s", lnno, msg);
	yyerror((const char *)buf);
	return;
}

main(int argc, char **argv)
{
#ifdef YYDEBUG
	extern int yydebug;
#endif
	extern FILE *yyin;
	char inbuf[10000];	/* it works, so don't complain !!! */
	FILE fakefd;

	/* check if used in command line mode */
#ifdef YYDEBUG
	yydebug = 1;
#endif
	if (argc > 1)
	{
		/* concatenate the arguments into one string */
		int arg = 1;
		char *pib = inbuf;
		for (*pib = 0; arg < argc; arg++)
		{
			char *parg = argv[arg];
			for ( ; *parg != 0; parg++, pib++)
			{
				*pib = *parg;
			}
			*pib++ = ' ';
		}

		/* add quit to command line */
		*pib++ = ';';
		*pib++ = 'q';
		*pib++ = ';';
		*pib = 0;

		/* set up a fake FILE structure */
		fakefd._flag = _IOREAD ;
		fakefd._ptr = fakefd._base = (unsigned char *)inbuf;
		fakefd._cnt = pib - inbuf;
		fakefd._file = 0;

		/* set yyin to pointer to fake FILE structure */
		yyin = &fakefd;
	}

	/* call parser */
	yyparse();

	exit(0);
}
yytabelem yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 82
# define YYLAST 269
yytabelem yyact[]={

     4,    44,    43,    42,    40,    79,    39,    41,    11,    48,
    73,   152,    37,    78,    31,    77,    10,   147,     9,    12,
    32,    33,    24,    74,   145,    27,    11,    29,   144,   143,
     8,   142,    26,    34,     7,   101,   141,    12,    36,   140,
    35,   146,   139,    38,    28,    44,    43,    42,    40,   137,
    39,    41,   136,   135,   120,   119,    37,    65,    31,    67,
    76,    99,    98,    97,    32,    33,    24,    75,    66,    27,
    96,    29,    95,    94,    68,    93,    26,    34,    92,    91,
    90,    89,    36,   153,    35,   148,   138,    38,    28,    84,
    83,    82,    44,    43,    42,    40,    64,    39,    41,    63,
    72,    49,    70,    37,    71,    31,    21,    19,    17,    69,
    20,    32,    33,    24,     2,    22,    27,    14,    29,    18,
    16,    13,   132,    26,    34,    15,    62,    80,     3,    36,
    59,    35,    58,    60,    38,    28,    23,    25,    56,    81,
    50,     6,    85,    86,    87,    88,    51,    52,     5,    30,
     1,    45,    46,    47,     0,     0,    61,     0,     0,    53,
     0,     0,     0,     0,    55,     0,    54,     0,     0,    57,
   100,     0,     0,     0,     0,   105,     0,   108,   109,   110,
   111,   102,   114,   115,   112,   113,   104,   106,   107,     0,
   103,     0,     0,   116,   117,   118,     0,     0,     0,     0,
     0,     0,     0,     0,   121,   122,   123,   124,   125,   126,
   127,   128,   129,   130,   131,     0,   133,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   134,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   149,     0,     0,     0,     0,     0,     0,
     0,     0,   150,   151,     0,     0,     0,     0,   154 };
yytabelem yypact[]={

-10000000,  -256,  -274,  -274,  -274,-10000000,  -276,-10000000,  -157,  -130,
-10000000,-10000000,-10000000,  -162,  -169,  -209,  -196,  -210,  -171,  -269,
  -221,  -282,-10000000,-10000000,  -212,-10000000,  -165,  -165,  -165,  -165,
-10000000,  -197,  -198,  -199,  -200,  -203,  -205,  -206,  -208,  -215,
  -216,  -217,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,  -165,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,  -243,  -165,  -165,  -165,  -165,  -165,  -165,  -165,
  -165,  -165,  -165,  -165,  -165,  -165,  -165,  -165,  -165,  -165,
  -236,  -237,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,  -165,
  -165,  -165,  -165,  -165,  -165,  -165,  -165,  -165,  -165,  -165,
  -162,  -165,  -169,  -209,  -196,  -210,  -171,  -171,  -269,  -269,
  -269,  -269,  -221,  -221,  -282,  -282,-10000000,-10000000,-10000000,  -165,
-10000000,  -238,  -239,  -242,  -181,  -249,  -252,  -255,  -260,  -262,
  -263,  -267,  -250,  -182,-10000000,-10000000,-10000000,-10000000,  -165,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,  -165,  -165,  -280,
  -184,-10000000,-10000000,  -165,-10000000 };
yytabelem yypgo[]={

     0,   150,   110,   121,   120,   117,   125,   115,   149,   108,
   114,   106,   141,   137,   119,   107,   136,   128,   127,   148,
   126,   122 };
yytabelem yyr1[]={

     0,     1,     1,     1,     1,     1,    19,    19,    17,    17,
    17,    17,    17,    17,    17,    17,    17,    17,    17,    17,
    17,    17,    17,    20,    17,    21,    21,    10,    12,    12,
     3,     3,     5,     5,     6,     6,     4,     4,     9,     9,
     9,    14,    14,    14,    14,    14,    15,    15,    15,     2,
     2,     2,    11,    11,    11,    11,     7,     7,    16,    16,
    16,    16,    16,    13,    13,    13,    13,    13,    13,    13,
    13,    13,    13,    13,    13,    13,     8,     8,     8,    18,
    18,    18 };
yytabelem yyr2[]={

     0,     0,     7,     7,     7,     5,     2,     2,     3,     5,
     3,     5,     5,     5,     5,     5,     5,     5,     5,     5,
     5,     5,     5,     1,    11,     7,    11,     3,     3,     7,
     3,     7,     3,     7,     3,     7,     3,     7,     3,     7,
     7,     3,     7,     7,     7,     7,     3,     7,     7,     3,
     7,     7,     3,     7,     7,     7,     3,     9,     3,     5,
     5,     5,     5,     3,     9,     9,     9,    13,     9,     9,
     9,     9,     9,     9,     9,     7,     3,     3,     3,     3,
     3,     3 };
yytabelem yychk[]={

-10000000,    -1,   -10,   -17,   256,   -19,   -12,   290,   286,   274,
   272,   282,   293,    -3,    -5,    -6,    -4,    -9,   -14,   -15,
    -2,   -11,    -7,   -16,   278,   -13,   288,   281,   300,   283,
    -8,   270,   276,   277,   289,   296,   294,   268,   299,   262,
   260,   263,   259,   258,   257,   -19,   -19,   -19,   285,   258,
   270,   276,   277,   289,   296,   294,   268,   299,   262,   260,
   263,   286,   -20,   261,   265,   266,   264,   269,   284,   280,
   273,   275,   271,   279,   292,   288,   281,   297,   295,   287,
   -18,   -10,   303,   302,   301,    -7,    -7,    -7,    -7,   278,
   278,   278,   278,   278,   278,   278,   278,   278,   278,   278,
    -3,   278,    -5,    -6,    -4,    -9,   -14,   -14,   -15,   -15,
   -15,   -15,    -2,    -2,   -11,   -11,    -7,    -7,    -7,   291,
   291,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,
   -10,   -10,   -21,   -10,    -7,   291,   291,   291,   267,   291,
   291,   291,   291,   291,   291,   291,   291,   267,   267,   -10,
   -10,   -10,   291,   267,   -10 };
yytabelem yydef[]={

     1,    -2,     0,     0,     0,     5,    27,     8,     0,    10,
    23,     6,     7,    28,    30,    32,    34,    36,    38,    41,
    46,    49,    52,    56,     0,    58,     0,     0,     0,     0,
    63,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    76,    77,    78,     2,     3,     4,     0,     9,
    11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
    21,    22,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    79,    80,    81,    59,    60,    61,    62,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    29,     0,    31,    33,    35,    37,    39,    40,    42,    43,
    44,    45,    47,    48,    50,    51,    53,    54,    55,     0,
    75,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    57,    64,    65,    66,     0,    68,
    69,    70,    71,    72,    73,    74,    24,     0,     0,     0,
     0,    25,    67,     0,    26 };
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
	"DCONST",	257,
	"LCONST",	258,
	"ULCONST",	259,
	"ACOS",	260,
	"AND",	261,
	"ASIN",	262,
	"ATAN",	263,
	"BITAND",	264,
	"BITOR",	265,
	"BITXOR",	266,
	"COMMA",	267,
	"COS",	268,
	"EQUAL",	269,
	"EXP",	270,
	"GE",	271,
	"GRAPH",	272,
	"GT",	273,
	"HELP",	274,
	"LE",	275,
	"LOG",	276,
	"LOG10",	277,
	"LPAREN",	278,
	"LSHIFT",	279,
	"LT",	280,
	"MINUS",	281,
	"NEWLINE",	282,
	"NOT",	283,
	"NOTEQUAL",	284,
	"OR",	285,
	"OUTBASE",	286,
	"PERCENT",	287,
	"PLUS",	288,
	"POW",	289,
	"QUIT",	290,
	"RPAREN",	291,
	"RSHIFT",	292,
	"SEMICOLON",	293,
	"SIN",	294,
	"SLASH",	295,
	"SQRT",	296,
	"STAR",	297,
	"STRING",	298,
	"TAN",	299,
	"TILDE",	300,
	"DCAST",	301,
	"LCAST",	302,
	"ULCAST",	303,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"start : /* empty */",
	"start : start expr newline",
	"start : start cmd newline",
	"start : start error newline",
	"start : start newline",
	"newline : NEWLINE",
	"newline : SEMICOLON",
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
	"cmd : GRAPH",
	"cmd : GRAPH LPAREN exprlist RPAREN",
	"exprlist : expr COMMA expr",
	"exprlist : exprlist COMMA expr COMMA expr",
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
# line 106 "omybc.y"
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
				printf("%e\n", yypvt[-1].number.value.dvalue);
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
# line 172 "omybc.y"
{
		/* do nothing */
	} break;
case 4:
# line 176 "omybc.y"
{
		yyerrok;
	} break;
case 5:
# line 180 "omybc.y"
{
		yyerrok;
	} break;
case 8:
# line 192 "omybc.y"
{
		YYACCEPT;
	} break;
case 9:
# line 196 "omybc.y"
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
			yyerror((const char *)"allowed bases are 2, 8, 10, 16.");
			YYERROR;
		}
		strcpy(yyval.string, yypvt[-1].string);
	} break;
case 10:
# line 212 "omybc.y"
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
		printf("math constants:\n");
		printf("pi, e\n");
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
case 11:
# line 240 "omybc.y"
{
		printf("usage: exp(expr)\n");
	} break;
case 12:
# line 244 "omybc.y"
{
		printf("usage: log(expr)\n");
	} break;
case 13:
# line 248 "omybc.y"
{
		printf("usage: log10(expr)\n");
	} break;
case 14:
# line 252 "omybc.y"
{
		printf("usage: pow(base_expr, power_expr)\n");
	} break;
case 15:
# line 256 "omybc.y"
{
		printf("usage: sqrt(expr)\n");
	} break;
case 16:
# line 260 "omybc.y"
{
		printf("usage: sin(expr)\n");
	} break;
case 17:
# line 264 "omybc.y"
{
		printf("usage: cos(expr)\n");
	} break;
case 18:
# line 268 "omybc.y"
{
		printf("usage: tan(expr)\n");
	} break;
case 19:
# line 272 "omybc.y"
{
		printf("usage: asin(expr)\n");
	} break;
case 20:
# line 276 "omybc.y"
{
		printf("usage: acos(expr)\n");
	} break;
case 21:
# line 280 "omybc.y"
{
		printf("usage: atan(expr)\n");
	} break;
case 22:
# line 284 "omybc.y"
{
		printf("usage: ob [2|8|10|16]\n");
	} break;
case 23:
# line 288 "omybc.y"
{
		initgraph();
	} break;
case 24:
# line 292 "omybc.y"
{
		graph();
	} break;
case 25:
# line 299 "omybc.y"
{
		storexy(number2double(yypvt[-2].number), number2double(yypvt[-0].number));
	} break;
case 26:
# line 303 "omybc.y"
{
		storexy(number2double(yypvt[-2].number), number2double(yypvt[-0].number));
	} break;
case 27:
# line 310 "omybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 28:
# line 317 "omybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 29:
# line 321 "omybc.y"
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
				mybcerror((const char *)"default case.", __LINE__);
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
				mybcerror((const char *)"default case.", __LINE__);
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
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 30:
# line 394 "omybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 31:
# line 398 "omybc.y"
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
				mybcerror((const char *)"default case.", __LINE__);
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
				mybcerror((const char *)"default case.", __LINE__);
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
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 32:
# line 471 "omybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 33:
# line 475 "omybc.y"
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
				yyerror((const char *)"| cannot be applied to double.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
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
				yyerror((const char *)"| cannot be applied to double.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror((const char *)"| cannot be applied to double.");
			YYERROR;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 34:
# line 532 "omybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 35:
# line 536 "omybc.y"
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
				yyerror((const char *)"^ cannot be applied to double.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
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
				yyerror((const char *)"^ cannot be applied to double.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror((const char *)"^ cannot be applied to double.");
			YYERROR;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 36:
# line 593 "omybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 37:
# line 597 "omybc.y"
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
				yyerror((const char *)"& cannot be applied to double.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
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
				yyerror((const char *)"& cannot be applied to double.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror((const char *)"& cannot be applied to double.");
			YYERROR;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 38:
# line 654 "omybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 39:
# line 658 "omybc.y"
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
				mybcerror((const char *)"default case.", __LINE__);
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
				mybcerror((const char *)"default case.", __LINE__);
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
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 40:
# line 728 "omybc.y"
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
				mybcerror((const char *)"default case.", __LINE__);
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
				mybcerror((const char *)"default case.", __LINE__);
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
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 41:
# line 801 "omybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 42:
# line 805 "omybc.y"
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
				mybcerror((const char *)"default case.", __LINE__);
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
				mybcerror((const char *)"default case.", __LINE__);
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
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 43:
# line 875 "omybc.y"
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
				mybcerror((const char *)"default case.", __LINE__);
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
				mybcerror((const char *)"default case.", __LINE__);
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
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 44:
# line 945 "omybc.y"
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
				mybcerror((const char *)"default case.", __LINE__);
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
				mybcerror((const char *)"default case.", __LINE__);
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
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 45:
# line 1015 "omybc.y"
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
				mybcerror((const char *)"default case.", __LINE__);
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
				mybcerror((const char *)"default case.", __LINE__);
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
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 46:
# line 1088 "omybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 47:
# line 1092 "omybc.y"
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
				yyerror((const char *)"<< cannot be applied to double.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
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
				yyerror((const char *)"<< cannot be applied to double.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror((const char *)"<< cannot be applied to double.");
			YYERROR;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 48:
# line 1144 "omybc.y"
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
				yyerror((const char *)">> cannot be applied to double.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
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
				yyerror((const char *)">> cannot be applied to double.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror((const char *)">> cannot be applied to double.");
			YYERROR;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 49:
# line 1199 "omybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 50:
# line 1203 "omybc.y"
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
				mybcerror((const char *)"default case.", __LINE__);
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
				mybcerror((const char *)"default case.", __LINE__);
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
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 51:
# line 1281 "omybc.y"
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
				mybcerror((const char *)"default case.", __LINE__);
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
				mybcerror((const char *)"default case.", __LINE__);
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
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 52:
# line 1362 "omybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 53:
# line 1366 "omybc.y"
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
				mybcerror((const char *)"default case.", __LINE__);
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
				mybcerror((const char *)"default case.", __LINE__);
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
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 54:
# line 1444 "omybc.y"
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
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.ulvalue / yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = ULONG;
				if (yypvt[-0].number.value.lvalue == 0)
				{
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.ulvalue / yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.type = DOUBLE;
				if (yypvt[-0].number.value.dvalue == 0)
				{
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.dvalue =
					yypvt[-2].number.value.ulvalue / yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
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
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.lvalue / yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = LONG;
				if (yypvt[-0].number.value.lvalue == 0)
				{
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue / yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.type = DOUBLE;
				if (yypvt[-0].number.value.dvalue == 0)
				{
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.dvalue =
					yypvt[-2].number.value.lvalue / yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
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
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.dvalue =
					yypvt[-2].number.value.dvalue / yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = DOUBLE;
				if (yypvt[-0].number.value.lvalue == 0)
				{
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.dvalue =
					yypvt[-2].number.value.dvalue / yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyval.number.type = DOUBLE;
				if (yypvt[-0].number.value.dvalue == 0)
				{
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.dvalue =
					yypvt[-2].number.value.dvalue / yypvt[-0].number.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 55:
# line 1567 "omybc.y"
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
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.ulvalue % yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = ULONG;
				if (yypvt[-0].number.value.lvalue == 0)
				{
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.ulvalue % yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyerror((const char *)"% cannot be applied to double.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
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
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.ulvalue =
					yypvt[-2].number.value.lvalue % yypvt[-0].number.value.ulvalue;
				break;
			case LONG:
				yyval.number.type = LONG;
				if (yypvt[-0].number.value.lvalue == 0)
				{
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				yyval.number.value.lvalue =
					yypvt[-2].number.value.lvalue % yypvt[-0].number.value.lvalue;
				break;
			case DOUBLE:
				yyerror((const char *)"% cannot be applied to double.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror((const char *)"% cannot be applied to double.");
			YYERROR;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 56:
# line 1644 "omybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 57:
# line 1648 "omybc.y"
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
				mybcerror((const char *)"default case.", __LINE__);
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
				mybcerror((const char *)"default case.", __LINE__);
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
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 58:
# line 1719 "omybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 59:
# line 1723 "omybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 60:
# line 1727 "omybc.y"
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
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		yyval.number = yypvt[-0].number;
	} break;
case 61:
# line 1746 "omybc.y"
{
		switch (yypvt[-0].number.type)
		{
		case ULONG:
			yypvt[-0].number.value.ulvalue = ~(yypvt[-0].number.value.ulvalue);
			break;
		case DOUBLE:
			yyerror((const char *)"~ cannot be applied to double.");
			YYERROR;
		case LONG:
			yypvt[-0].number.value.lvalue = ~(yypvt[-0].number.value.lvalue);
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		yyval.number = yypvt[-0].number;
	} break;
case 62:
# line 1765 "omybc.y"
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
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		yyval.number = yypvt[-0].number;
	} break;
case 63:
# line 1787 "omybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 64:
# line 1791 "omybc.y"
{
		yypvt[-1].number = number2double(yypvt[-1].number);
		if (yypvt[-1].number.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = exp(yypvt[-1].number.value.dvalue);
	} break;
case 65:
# line 1802 "omybc.y"
{
		yypvt[-1].number = number2double(yypvt[-1].number);
		if (yypvt[-1].number.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		if (yypvt[-1].number.value.dvalue < 0)
		{
			mybcerror((const char *)"log of negative number.", __LINE__);
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = log(yypvt[-1].number.value.dvalue);
	} break;
case 66:
# line 1818 "omybc.y"
{
		yypvt[-1].number = number2double(yypvt[-1].number);
		if (yypvt[-1].number.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		if (yypvt[-1].number.value.dvalue < 0)
		{
			mybcerror((const char *)"log10 of negative number.", __LINE__);
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = log10(yypvt[-1].number.value.dvalue);
	} break;
case 67:
# line 1834 "omybc.y"
{
		yypvt[-3].number = number2double(yypvt[-3].number);
		if (yypvt[-3].number.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		yypvt[-1].number = number2double(yypvt[-1].number);
		if (yypvt[-1].number.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = pow(yypvt[-3].number.value.dvalue, yypvt[-1].number.value.dvalue);
	} break;
case 68:
# line 1851 "omybc.y"
{
		yypvt[-1].number = number2double(yypvt[-1].number);
		if (yypvt[-1].number.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = sqrt(yypvt[-1].number.value.dvalue);
	} break;
case 69:
# line 1862 "omybc.y"
{
		yypvt[-1].number = number2double(yypvt[-1].number);
		if (yypvt[-1].number.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = sin(yypvt[-1].number.value.dvalue);
	} break;
case 70:
# line 1873 "omybc.y"
{
		yypvt[-1].number = number2double(yypvt[-1].number);
		if (yypvt[-1].number.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = cos(yypvt[-1].number.value.dvalue);
	} break;
case 71:
# line 1884 "omybc.y"
{
		yypvt[-1].number = number2double(yypvt[-1].number);
		if (yypvt[-1].number.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = tan(yypvt[-1].number.value.dvalue);
	} break;
case 72:
# line 1895 "omybc.y"
{
		yypvt[-1].number = number2double(yypvt[-1].number);
		if (yypvt[-1].number.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = asin(yypvt[-1].number.value.dvalue);
	} break;
case 73:
# line 1906 "omybc.y"
{
		yypvt[-1].number = number2double(yypvt[-1].number);
		if (yypvt[-1].number.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = acos(yypvt[-1].number.value.dvalue);
	} break;
case 74:
# line 1917 "omybc.y"
{
		yypvt[-1].number = number2double(yypvt[-1].number);
		if (yypvt[-1].number.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = atan(yypvt[-1].number.value.dvalue);
	} break;
case 75:
# line 1928 "omybc.y"
{
		yyval.number = yypvt[-1].number;
	} break;
case 76:
# line 1935 "omybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 77:
# line 1939 "omybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 78:
# line 1943 "omybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 79:
# line 1950 "omybc.y"
{
		yyval.type = ULONG;
	} break;
case 80:
# line 1954 "omybc.y"
{
		yyval.type = LONG;
	} break;
case 81:
# line 1958 "omybc.y"
{
		yyval.type = DOUBLE;
	} break;
# line	532 "/usr/ccs/bin/yaccpar"
	}
	goto yystack;		/* reset registers in driver code */
}

