
# line 2 "mybc.y"
/* required headers */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <values.h>

/* other headers */
#include "mybc.h"

/* globals */
long outbase = 10;
double xy[NUMBEROFPOINTS][2];
int xyno;
Complex<double> result(0.0,0.0);
double deps = 0.0;

/* externs */
void yyerror(const char *);

/* local definitions */
#undef YES
#define YES 1
#undef NO
#define NO 0


# line 32 "mybc.y"
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif
 {
	TYPE type;
	VALUE number; 
	char string[32];
} YYSTYPE;
# define CCONST 257
# define DCONST 258
# define LCONST 259
# define ULCONST 260
# define ABS 261
# define ACOS 262
# define ACOSH 263
# define AND 264
# define ARG 265
# define ASIN 266
# define ASINH 267
# define ATAN 268
# define ATANH 269
# define BITAND 270
# define BITOR 271
# define BITXOR 272
# define COMB 273
# define COMMA 274
# define COS 275
# define COSH 276
# define COT 277
# define CSC 278
# define EQUAL 279
# define EXP 280
# define FACT 281
# define GE 282
# define GRAPH 283
# define GT 284
# define HELP 285
# define IMAG 286
# define LE 287
# define LOG 288
# define LOG10 289
# define LPAREN 290
# define LBRACKET 291
# define LSHIFT 292
# define LT 293
# define MINUS 294
# define NEWLINE 295
# define NORM 296
# define NOT 297
# define NOTEQUAL 298
# define OR 299
# define OUTBASE 300
# define PERCENT 301
# define PERM 302
# define PLUS 303
# define POW 304
# define QUIT 305
# define REAL 306
# define RPAREN 307
# define RBRACKET 308
# define RSHIFT 309
# define SEMICOLON 310
# define SIN 311
# define SINH 312
# define SLASH 313
# define SQRT 314
# define SSEC 315
# define STAR 316
# define STRING 317
# define TAN 318
# define TANH 319
# define TILDE 320
# define USURY 321
# define CCAST 322
# define DCAST 323
# define LCAST 324
# define ULCAST 325

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

# line 4613 "mybc.y"


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
		printf("%03d: (%le, %le)\n", ixy, xy[ixy][0], xy[ixy][1]);
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

Complex<double>
number2complex(VALUE number)
{
	switch (number.type)
	{
	case LONG:
		return(Complex<double>((double)number.value.lvalue, 0.0));
	case ULONG:
		return(Complex<double>((double)number.value.ulvalue, 0.0));
	case DOUBLE:
		return(Complex<double>((double)number.value.dvalue, 0.0));
	case COMPLEX:
		break;
	default:
		MustBeTrue(0);
		break;
	}
	return(Complex<double>( 
			number.value.cvalue.r,
			number.value.cvalue.i));
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

void
yyerror(const char *s)
{
	fprintf(stderr, "%s\n", s);
}

void
mybcerror(const char *msg, int lnno)
{
	char buf[BUFSIZ];
	sprintf(buf, "%d: %s", lnno, msg);
	yyerror((const char *)buf);
	return;
}

unsigned long 
perm(unsigned long n, unsigned long r)
{
	unsigned long total = 1;
	for (unsigned long i=(n-r+1); i<=n; i++)
	{
		total = total*i;
	}
	return(total);
}

unsigned long 
comb(unsigned long n, unsigned long r)
{
	return(perm(n,r)/fact(r));
}

unsigned long 
fact(unsigned long n)
{
	if (n == 0 || n == 1)
		return(1L);
	else if (n == 2)
		return(2L);
	else
	{
		unsigned long total = 1;
		for ( ; n > 1; n--)
		{
			total *= n;
		}
		return(total);
	}
}

double
perm(double n, double r)
{
	return(fact(n)/fact(n-r));
}

double
comb(double n, double r)
{
	return(fact(n)/(fact(r)*fact(n-r)));
}

double
dabs(double d)
{
	if (d < 0.0)
		return(-d);
	else
		return(d);
}

double 
fact(double n)
{
	/* use stirling's approximation */
#if 0
	return(pow(n, n)*exp(-n)*sqrt(2.0*M_PI*n)*(1.0+1.0/(12.0*n)));
#else
	return(pow(n, n)*sqrt(2.0*M_PI*n)*exp(-n+1.0/(12.0*n)));
#endif
}

void
usury(double principal, double interest, double length)
{
	length *= 12.0; 		// convert to months
	interest /= 100.0*12.0;		// convert to fraction/month
	double payment = 
		principal*interest/(1.0-pow((1.0+interest), -length));
	double totalpayment = length*payment;
	printf("\n");
	printf("monthly payment: %-10.2lf\n", payment);
	printf("total payment  : %-10.2lf\n", totalpayment);
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

	/* error tolerance for doubles */
	deps = calcEpsilon(double(0));

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
static YYCONST yytabelem yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 121
# define YYLAST 459
static YYCONST yytabelem yyact[]={

    64,    63,    62,    61,    55,    47,    53,   249,    56,    46,
    52,    48,    54,   117,   259,    11,    35,   254,    41,    50,
    45,    43,   253,    31,    38,   116,   111,   243,   115,    59,
    12,    32,    33,    24,    60,   252,   240,    27,   239,    57,
    29,   114,   238,   112,   237,    36,    26,    34,   236,    58,
   113,   235,   234,   233,    40,    49,   232,    39,    44,   231,
   242,    42,    51,    28,    37,   123,   122,   121,   120,     4,
    64,    63,    62,    61,    55,    47,    53,   230,    56,    46,
    52,    48,    54,   229,   228,   227,    35,   226,    41,    50,
    45,    43,   225,    31,    38,   224,    10,   223,     9,    59,
   222,    32,    33,    24,    60,   221,   220,    27,    11,    57,
    29,   219,   214,     8,   213,    36,    26,    34,     7,    58,
    68,   212,   178,    12,    40,    49,   177,    39,    44,   105,
   159,    42,    51,    28,    37,    64,    63,    62,    61,    55,
    47,    53,   156,    56,    46,    52,    48,    54,   106,   155,
   154,    35,     2,    41,    50,    45,    43,   153,    31,    38,
   152,   151,   150,   149,    59,   148,    32,    33,    24,    60,
   147,   146,    27,   145,    57,    29,   144,   119,   143,   142,
    36,    26,    34,   110,    58,   108,   141,   140,   109,    40,
    49,   256,    39,    44,   107,   139,    42,    51,    28,    37,
    71,    88,    94,   138,    72,    87,    93,    89,    95,   137,
   136,   135,    97,   134,    82,    91,    86,    84,   133,    76,
    99,   132,   131,   130,   129,    75,   128,    77,    78,   255,
   244,   218,   217,   216,   215,    73,   208,   103,   102,    96,
   104,    98,   101,    79,    69,    74,    21,    19,    17,    22,
    81,    90,    14,    80,    85,    16,    18,    83,    92,    13,
    70,    20,     5,   209,   100,    65,    66,    67,   118,    15,
     3,    23,    25,     6,    30,     1,   124,   125,   126,   127,
     0,   179,   180,   181,   182,   183,   184,   185,   186,   187,
   188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
   198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
   157,     0,   210,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   158,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   163,   160,   166,   167,   168,   169,   162,
   172,   173,   164,   165,     0,   174,   175,   176,   245,   246,
   247,   248,   161,   170,   171,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   250,   251,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   257,   258,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   211,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,   241 };
static YYCONST yytabelem yypact[]={

-10000000,  -187,  -280,  -280,  -280,-10000000,  -179,-10000000,   -15,   -61,
-10000000,-10000000,-10000000,   -22,   -33,   -35,   -30,  -150,   -99,  -266,
  -253,  -288,-10000000,-10000000,  -257,-10000000,  -122,  -122,  -122,  -122,
-10000000,   -64,   -66,   -67,   -68,   -69,   -72,   -77,   -79,   -80,
   -81,   -87,   -95,  -103,  -104,  -111,  -112,  -114,  -117,  -119,
  -120,  -125,  -127,  -128,  -129,  -130,  -133,  -140,  -141,  -148,
  -122,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,  -122,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
  -160,  -122,  -122,  -122,  -122,  -122,  -122,  -122,  -122,  -122,
  -122,  -122,  -122,  -122,  -122,  -122,  -122,  -122,  -181,  -185,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,  -122,  -122,
  -122,  -122,  -122,  -122,  -122,  -122,  -122,  -122,  -122,  -122,
  -122,  -122,  -122,  -122,  -122,  -122,  -122,  -122,  -122,  -122,
  -122,  -122,  -122,  -122,  -122,  -122,  -122,   -38,   -22,  -122,
   -33,   -35,   -30,  -150,   -99,   -99,  -266,  -266,  -266,  -266,
  -253,  -253,  -288,  -288,-10000000,-10000000,-10000000,  -122,-10000000,  -186,
  -193,  -195,   -40,   -41,   -42,   -43,  -196,  -201,  -202,  -207,
  -210,  -212,  -215,  -220,  -222,  -223,  -224,  -230,  -248,  -251,
  -254,  -255,  -256,  -259,  -263,  -265,  -269,  -271,  -122,  -247,
   -44,-10000000,-10000000,-10000000,-10000000,  -122,  -122,  -122,  -122,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,  -301,-10000000,  -122,  -122,  -272,  -285,  -290,   -45,-10000000,
   -83,-10000000,-10000000,-10000000,-10000000,  -122,  -122,  -293,-10000000,-10000000 };
static YYCONST yytabelem yypgo[]={

     0,   275,   261,   259,   255,   252,   269,   249,   274,   248,
   152,   246,   273,   272,   256,   247,   271,   270,   268,   262,
   264,   263 };
static YYCONST yytabelem yyr1[]={

     0,     1,     1,     1,     1,     1,    19,    19,    17,    17,
    17,    17,    17,    17,    17,    17,    17,    17,    17,    17,
    17,    17,    17,    17,    17,    17,    17,    17,    17,    17,
    17,    17,    17,    17,    17,    17,    17,    17,    17,    17,
    17,    20,    17,    21,    21,    10,    12,    12,     3,     3,
     5,     5,     6,     6,     4,     4,     9,     9,     9,    14,
    14,    14,    14,    14,    15,    15,    15,     2,     2,     2,
    11,    11,    11,    11,     7,     7,    16,    16,    16,    16,
    16,    13,    13,    13,    13,    13,    13,    13,    13,    13,
    13,    13,    13,    13,    13,    13,    13,    13,    13,    13,
    13,    13,    13,    13,    13,    13,    13,    13,    13,    13,
    13,    13,    13,     8,     8,     8,     8,    18,    18,    18,
    18 };
static YYCONST yytabelem yyr2[]={

     0,     0,     7,     7,     7,     5,     2,     2,     3,     5,
     3,     5,     5,     5,     5,     5,     5,     5,     5,     5,
     5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
     5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
     5,     1,    11,     7,    11,     3,     3,     7,     3,     7,
     3,     7,     3,     7,     3,     7,     3,     7,     7,     3,
     7,     7,     7,     7,     3,     7,     7,     3,     7,     7,
     3,     7,     7,     7,     3,     9,     3,     5,     5,     5,
     5,     3,     9,     9,     9,    13,    13,    13,    17,     9,
     9,     9,     9,     9,     9,     9,     9,     9,     9,     9,
     9,     9,     9,     9,     9,     9,     9,     9,     9,     9,
     9,    11,     7,     3,     3,     3,     3,     3,     3,     3,
     3 };
static YYCONST yytabelem yychk[]={

-10000000,    -1,   -10,   -17,   256,   -19,   -12,   305,   300,   285,
   283,   295,   310,    -3,    -5,    -6,    -4,    -9,   -14,   -15,
    -2,   -11,    -7,   -16,   290,   -13,   303,   294,   320,   297,
    -8,   280,   288,   289,   304,   273,   302,   321,   281,   314,
   311,   275,   318,   278,   315,   277,   266,   262,   268,   312,
   276,   319,   267,   263,   269,   261,   265,   296,   306,   286,
   291,   260,   259,   258,   257,   -19,   -19,   -19,   299,   259,
   321,   261,   265,   296,   306,   286,   280,   288,   289,   304,
   314,   311,   275,   318,   278,   315,   277,   266,   262,   268,
   312,   276,   319,   267,   263,   269,   300,   273,   302,   281,
   -20,   264,   271,   272,   270,   279,   298,   293,   284,   287,
   282,   292,   309,   303,   294,   316,   313,   301,   -18,   -10,
   325,   324,   323,   322,    -7,    -7,    -7,    -7,   290,   290,
   290,   290,   290,   290,   290,   290,   290,   290,   290,   290,
   290,   290,   290,   290,   290,   290,   290,   290,   290,   290,
   290,   290,   290,   290,   290,   290,   290,    -7,    -3,   290,
    -5,    -6,    -4,    -9,   -14,   -14,   -15,   -15,   -15,   -15,
    -2,    -2,   -11,   -11,    -7,    -7,    -7,   307,   307,   -10,
   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,
   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,
   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   274,   -21,
   -10,    -7,   307,   307,   307,   274,   274,   274,   274,   307,
   307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
   307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
   307,    -7,   307,   274,   274,   -10,   -10,   -10,   -10,   308,
   -10,   -10,   307,   307,   307,   274,   274,   -10,   -10,   307 };
static YYCONST yytabelem yydef[]={

     1,    -2,     0,     0,     0,     5,    45,     8,     0,    10,
    41,     6,     7,    46,    48,    50,    52,    54,    56,    59,
    64,    67,    70,    74,     0,    76,     0,     0,     0,     0,
    81,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   113,   114,   115,   116,     2,     3,     4,     0,     9,
    11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
    21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
    31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   117,   118,   119,   120,    77,    78,    79,    80,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    47,     0,
    49,    51,    53,    55,    57,    58,    60,    61,    62,    63,
    65,    66,    68,    69,    71,    72,    73,     0,   112,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    75,    82,    83,    84,     0,     0,     0,     0,    89,
    90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
   100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
   110,     0,    42,     0,     0,     0,     0,     0,     0,   111,
     0,    43,    85,    86,    87,     0,     0,     0,    44,    88 };
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
	"CCONST",	257,
	"DCONST",	258,
	"LCONST",	259,
	"ULCONST",	260,
	"ABS",	261,
	"ACOS",	262,
	"ACOSH",	263,
	"AND",	264,
	"ARG",	265,
	"ASIN",	266,
	"ASINH",	267,
	"ATAN",	268,
	"ATANH",	269,
	"BITAND",	270,
	"BITOR",	271,
	"BITXOR",	272,
	"COMB",	273,
	"COMMA",	274,
	"COS",	275,
	"COSH",	276,
	"COT",	277,
	"CSC",	278,
	"EQUAL",	279,
	"EXP",	280,
	"FACT",	281,
	"GE",	282,
	"GRAPH",	283,
	"GT",	284,
	"HELP",	285,
	"IMAG",	286,
	"LE",	287,
	"LOG",	288,
	"LOG10",	289,
	"LPAREN",	290,
	"LBRACKET",	291,
	"LSHIFT",	292,
	"LT",	293,
	"MINUS",	294,
	"NEWLINE",	295,
	"NORM",	296,
	"NOT",	297,
	"NOTEQUAL",	298,
	"OR",	299,
	"OUTBASE",	300,
	"PERCENT",	301,
	"PERM",	302,
	"PLUS",	303,
	"POW",	304,
	"QUIT",	305,
	"REAL",	306,
	"RPAREN",	307,
	"RBRACKET",	308,
	"RSHIFT",	309,
	"SEMICOLON",	310,
	"SIN",	311,
	"SINH",	312,
	"SLASH",	313,
	"SQRT",	314,
	"SSEC",	315,
	"STAR",	316,
	"STRING",	317,
	"TAN",	318,
	"TANH",	319,
	"TILDE",	320,
	"USURY",	321,
	"CCAST",	322,
	"DCAST",	323,
	"LCAST",	324,
	"ULCAST",	325,
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
	"cmd : HELP USURY",
	"cmd : HELP ABS",
	"cmd : HELP ARG",
	"cmd : HELP NORM",
	"cmd : HELP REAL",
	"cmd : HELP IMAG",
	"cmd : HELP EXP",
	"cmd : HELP LOG",
	"cmd : HELP LOG10",
	"cmd : HELP POW",
	"cmd : HELP SQRT",
	"cmd : HELP SIN",
	"cmd : HELP COS",
	"cmd : HELP TAN",
	"cmd : HELP CSC",
	"cmd : HELP SSEC",
	"cmd : HELP COT",
	"cmd : HELP ASIN",
	"cmd : HELP ACOS",
	"cmd : HELP ATAN",
	"cmd : HELP SINH",
	"cmd : HELP COSH",
	"cmd : HELP TANH",
	"cmd : HELP ASINH",
	"cmd : HELP ACOSH",
	"cmd : HELP ATANH",
	"cmd : HELP OUTBASE",
	"cmd : HELP COMB",
	"cmd : HELP PERM",
	"cmd : HELP FACT",
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
	"primary : COMB LPAREN expr COMMA expr RPAREN",
	"primary : PERM LPAREN expr COMMA expr RPAREN",
	"primary : USURY LPAREN expr COMMA expr COMMA expr RPAREN",
	"primary : FACT LPAREN expr RPAREN",
	"primary : SQRT LPAREN expr RPAREN",
	"primary : SIN LPAREN expr RPAREN",
	"primary : COS LPAREN expr RPAREN",
	"primary : TAN LPAREN expr RPAREN",
	"primary : CSC LPAREN expr RPAREN",
	"primary : SSEC LPAREN expr RPAREN",
	"primary : COT LPAREN expr RPAREN",
	"primary : ASIN LPAREN expr RPAREN",
	"primary : ACOS LPAREN expr RPAREN",
	"primary : ATAN LPAREN expr RPAREN",
	"primary : SINH LPAREN expr RPAREN",
	"primary : COSH LPAREN expr RPAREN",
	"primary : TANH LPAREN expr RPAREN",
	"primary : ASINH LPAREN expr RPAREN",
	"primary : ACOSH LPAREN expr RPAREN",
	"primary : ATANH LPAREN expr RPAREN",
	"primary : ABS LPAREN expr RPAREN",
	"primary : ARG LPAREN expr RPAREN",
	"primary : NORM LPAREN expr RPAREN",
	"primary : REAL LPAREN expr RPAREN",
	"primary : IMAG LPAREN expr RPAREN",
	"primary : LBRACKET cast COMMA cast RBRACKET",
	"primary : LPAREN expr RPAREN",
	"const : ULCONST",
	"const : LCONST",
	"const : DCONST",
	"const : CCONST",
	"type : ULCAST",
	"type : LCAST",
	"type : DCAST",
	"type : CCAST",
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
		
case 2:
# line 131 "mybc.y"
{
		switch (yypvt[-1].number.type)
		{
		case ULONG:
			printf("%s\n", num2base(outbase, NO, yypvt[-1].number.value.ulvalue));
			break;
		case LONG:
			printf("%s\n", num2base(outbase, YES, yypvt[-1].number.value.lvalue));
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
		case COMPLEX:
			switch (outbase)
			{
			case 2:
			case 8:
			case 16:
				printf("base = 10 for complex.\n");
			case 10:
				printf("[%le,%le]\n", 
					yypvt[-1].number.value.cvalue.r, yypvt[-1].number.value.cvalue.i);
				break;
			default:
				printf("%d: default case.\n", __LINE__);
				YYERROR;
			}
			break;
		case VOID:
			break;
		default:
			printf("%d: default case.\n", __LINE__);
			YYERROR;
		}
	} break;
case 3:
# line 179 "mybc.y"
{
		/* do nothing */
	} break;
case 4:
# line 183 "mybc.y"
{
		yyerrok;
	} break;
case 5:
# line 187 "mybc.y"
{
		yyerrok;
	} break;
case 8:
# line 199 "mybc.y"
{
		YYACCEPT;
	} break;
case 9:
# line 203 "mybc.y"
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
# line 219 "mybc.y"
{
		printf("mybc supports the following operators and math ");
		printf("functions.\n");
		printf("\n");
		printf("operators:\n");
		printf("|| && | ^ & == != < > <= >= << >> - * / ");
		printf("%% ( ) ~ !\n");
		printf("\n");
		printf("math functions:\n");
		printf("exp log log10 pow sqrt\n");
		printf("sin cos tan asin acos atan\n");
		printf("sinh cosh tanh asinh acosh atanh\n");
		printf("abs arg norm real imag\n");
		printf("\n");
		printf("counting functions:\n");
		printf("C(ombinations), P(ermutations), fact(orial)\n");
		printf("\n");
		printf("math constants:\n");
		printf("pi, e, i\n");
		printf("\n");
		printf("finance functions:\n");
		printf("usury(principal, %% interest, loan period in years)\n");
		printf("\n");
		printf("miscellaneous:\n");
		printf("(l) - long typecast\n");
		printf("(ul) - unsigned long typecast\n");
		printf("(d) - double typecast\n");
		printf("(c) - complex typecast\n");
		printf("[xh][0-9a-z]* - hex numbers\n");
		printf("o[0-7]* - octal numbers\n");
		printf("b[01]* - binary numbers\n");
		printf("'c' - ascii character\n");
		printf("[+|-]?[0-9]+.[0-9]+([eE][+-]?[0-9]+)? - real numbers\n");
		printf("[real,real] - complex numbers\n");
		printf("ob [2|8|10|16] - set output base\n");
		printf("? - help\n");
		strcpy(yyval.string, yypvt[-0].string);
	} break;
case 11:
# line 258 "mybc.y"
{
		printf("usage: usury(p, i, lp)\n");
		printf("where p = principal, i = %% interest, and "); 
		printf("lp = loan period in years.\n");
	} break;
case 12:
# line 264 "mybc.y"
{
		printf("usage: abs(expr)\n");
	} break;
case 13:
# line 268 "mybc.y"
{
		printf("usage: arg(expr)\n");
	} break;
case 14:
# line 272 "mybc.y"
{
		printf("usage: norm(expr)\n");
	} break;
case 15:
# line 276 "mybc.y"
{
		printf("usage: real(expr)\n");
	} break;
case 16:
# line 280 "mybc.y"
{
		printf("usage: imag(expr)\n");
	} break;
case 17:
# line 284 "mybc.y"
{
		printf("usage: exp(expr)\n");
	} break;
case 18:
# line 288 "mybc.y"
{
		printf("usage: log(expr)\n");
	} break;
case 19:
# line 292 "mybc.y"
{
		printf("usage: log10(expr)\n");
	} break;
case 20:
# line 296 "mybc.y"
{
		printf("usage: pow(base_expr, power_expr)\n");
	} break;
case 21:
# line 300 "mybc.y"
{
		printf("usage: sqrt(expr)\n");
	} break;
case 22:
# line 304 "mybc.y"
{
		printf("usage: sin(expr)\n");
	} break;
case 23:
# line 308 "mybc.y"
{
		printf("usage: cos(expr)\n");
	} break;
case 24:
# line 312 "mybc.y"
{
		printf("usage: tan(expr)\n");
	} break;
case 25:
# line 316 "mybc.y"
{
		printf("usage: csc(expr)\n");
	} break;
case 26:
# line 320 "mybc.y"
{
		printf("usage: sec(expr)\n");
	} break;
case 27:
# line 324 "mybc.y"
{
		printf("usage: cot(expr)\n");
	} break;
case 28:
# line 328 "mybc.y"
{
		printf("usage: asin(expr)\n");
	} break;
case 29:
# line 332 "mybc.y"
{
		printf("usage: acos(expr)\n");
	} break;
case 30:
# line 336 "mybc.y"
{
		printf("usage: atan(expr)\n");
	} break;
case 31:
# line 340 "mybc.y"
{
		printf("usage: sinh(expr)\n");
	} break;
case 32:
# line 344 "mybc.y"
{
		printf("usage: cosh(expr)\n");
	} break;
case 33:
# line 348 "mybc.y"
{
		printf("usage: tanh(expr)\n");
	} break;
case 34:
# line 352 "mybc.y"
{
		printf("usage: asinh(expr)\n");
	} break;
case 35:
# line 356 "mybc.y"
{
		printf("usage: acosh(expr)\n");
	} break;
case 36:
# line 360 "mybc.y"
{
		printf("usage: atanh(expr)\n");
	} break;
case 37:
# line 364 "mybc.y"
{
		printf("usage: ob [2|8|10|16]\n");
	} break;
case 38:
# line 368 "mybc.y"
{
		printf("usage: C(n, r)\n");
	} break;
case 39:
# line 372 "mybc.y"
{
		printf("usage: P(n, r)\n");
	} break;
case 40:
# line 376 "mybc.y"
{
		printf("usage: fact(n)\n");
	} break;
case 41:
# line 380 "mybc.y"
{
		initgraph();
	} break;
case 42:
# line 384 "mybc.y"
{
		graph();
	} break;
case 43:
# line 391 "mybc.y"
{
		storexy(number2double(yypvt[-2].number), number2double(yypvt[-0].number));
	} break;
case 44:
# line 395 "mybc.y"
{
		storexy(number2double(yypvt[-2].number), number2double(yypvt[-0].number));
	} break;
case 45:
# line 402 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 46:
# line 409 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 47:
# line 413 "mybc.y"
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)yypvt[-2].number.value.ulvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 || c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex || failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)yypvt[-2].number.value.lvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 || c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex || failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.dvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 || c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex || failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.ulvalue);
				try {
					yyval.number.value.lvalue = (c1 || c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex || failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case LONG:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.lvalue);
				try {
					yyval.number.value.lvalue = (c1 || c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex || failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case DOUBLE:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3(yypvt[-0].number.value.dvalue);
				try {
					yyval.number.value.lvalue = (c1 || c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex || failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case COMPLEX:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 || c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex || failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
case 48:
# line 606 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 49:
# line 610 "mybc.y"
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)yypvt[-2].number.value.ulvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 && c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex && failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)yypvt[-2].number.value.lvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 && c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex && failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.dvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 && c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex && failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.ulvalue);
				try {
					yyval.number.value.lvalue = (c1 && c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex && failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case LONG:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.lvalue);
				try {
					yyval.number.value.lvalue = (c1 && c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex && failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case DOUBLE:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3(yypvt[-0].number.value.dvalue);
				try {
					yyval.number.value.lvalue = (c1 && c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex && failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case COMPLEX:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 && c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex && failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
case 50:
# line 803 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 51:
# line 807 "mybc.y"
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
			case COMPLEX:
				yyerror((const char *)"| cannot be applied to complex.");
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
			case COMPLEX:
				yyerror((const char *)"| cannot be applied to complex.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror((const char *)"| cannot be applied to double.");
			YYERROR;
		case COMPLEX:
			yyerror((const char *)"| cannot be applied to complex.");
			YYERROR;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 52:
# line 873 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 53:
# line 877 "mybc.y"
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
			case COMPLEX:
				yyerror((const char *)"^ cannot be applied to complex.");
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
			case COMPLEX:
				yyerror((const char *)"^ cannot be applied to complex.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror((const char *)"^ cannot be applied to double.");
			YYERROR;
		case COMPLEX:
			yyerror((const char *)"^ cannot be applied to complex.");
			YYERROR;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 54:
# line 943 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 55:
# line 947 "mybc.y"
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
			case COMPLEX:
				yyerror((const char *)"& cannot be applied to complex.");
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
			case COMPLEX:
				yyerror((const char *)"& cannot be applied to complex.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror((const char *)"& cannot be applied to double.");
			YYERROR;
		case COMPLEX:
			yyerror((const char *)"& cannot be applied to complex.");
			YYERROR;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 56:
# line 1013 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 57:
# line 1017 "mybc.y"
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)yypvt[-2].number.value.ulvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 == c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex == failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)yypvt[-2].number.value.lvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 == c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex == failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.dvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 == c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex == failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.ulvalue);
				try {
					yyval.number.value.lvalue = (c1 == c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex == failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case LONG:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.lvalue);
				try {
					yyval.number.value.lvalue = (c1 == c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex == failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case DOUBLE:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3(yypvt[-0].number.value.dvalue);
				try {
					yyval.number.value.lvalue = (c1 == c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex == failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case COMPLEX:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 == c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex == failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
# line 1207 "mybc.y"
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)yypvt[-2].number.value.ulvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 != c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex != failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)yypvt[-2].number.value.lvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 != c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex != failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.dvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 != c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex != failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.ulvalue);
				try {
					yyval.number.value.lvalue = (c1 != c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex != failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case LONG:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.lvalue);
				try {
					yyval.number.value.lvalue = (c1 != c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex != failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case DOUBLE:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3(yypvt[-0].number.value.dvalue);
				try {
					yyval.number.value.lvalue = (c1 != c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex != failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case COMPLEX:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 != c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex != failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
case 59:
# line 1400 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 60:
# line 1404 "mybc.y"
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)yypvt[-2].number.value.ulvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 < c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex < failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)yypvt[-2].number.value.lvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 < c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex < failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.dvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 < c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex < failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.ulvalue);
				try {
					yyval.number.value.lvalue = (c1 < c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex < failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case LONG:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.lvalue);
				try {
					yyval.number.value.lvalue = (c1 < c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex < failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case DOUBLE:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3(yypvt[-0].number.value.dvalue);
				try {
					yyval.number.value.lvalue = (c1 < c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex < failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case COMPLEX:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 < c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex < failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
case 61:
# line 1594 "mybc.y"
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)yypvt[-2].number.value.ulvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 > c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex > failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)yypvt[-2].number.value.lvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 > c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex > failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.dvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 > c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex > failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.ulvalue);
				try {
					yyval.number.value.lvalue = (c1 > c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex > failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case LONG:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.lvalue);
				try {
					yyval.number.value.lvalue = (c1 > c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex > failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case DOUBLE:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3(yypvt[-0].number.value.dvalue);
				try {
					yyval.number.value.lvalue = (c1 > c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex > failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case COMPLEX:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 > c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex > failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
case 62:
# line 1784 "mybc.y"
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)yypvt[-2].number.value.ulvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 <= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex <= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)yypvt[-2].number.value.lvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 <= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex <= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)yypvt[-2].number.value.dvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 <= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex <= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.ulvalue);
				try {
					yyval.number.value.lvalue = (c1 <= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex <= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case LONG:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.lvalue);
				try {
					yyval.number.value.lvalue = (c1 <= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex <= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case DOUBLE:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.dvalue);
				try {
					yyval.number.value.lvalue = (c1 <= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex <= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case COMPLEX:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 <= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex <= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
case 63:
# line 1974 "mybc.y"
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)yypvt[-2].number.value.ulvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 >= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex >= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)yypvt[-2].number.value.lvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 >= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex >= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)yypvt[-2].number.value.dvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 >= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex >= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.ulvalue);
				try {
					yyval.number.value.lvalue = (c1 >= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex >= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case LONG:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.lvalue);
				try {
					yyval.number.value.lvalue = (c1 >= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex >= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case DOUBLE:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.dvalue);
				try {
					yyval.number.value.lvalue = (c1 >= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex >= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case COMPLEX:
			{
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					yyval.number.value.lvalue = (c1 >= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex >= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
case 64:
# line 2167 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 65:
# line 2171 "mybc.y"
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
			case COMPLEX:
				yyerror((const char *)"<< cannot be applied to complex.");
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
			case COMPLEX:
				yyerror((const char *)"<< cannot be applied to complex.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror((const char *)"<< cannot be applied to double.");
			YYERROR;
		case COMPLEX:
			yyerror((const char *)"<< cannot be applied to complex.");
			YYERROR;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 66:
# line 2232 "mybc.y"
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
			case COMPLEX:
				yyerror((const char *)">> cannot be applied to complex.");
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
			case COMPLEX:
				yyerror((const char *)">> cannot be applied to complex.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror((const char *)">> cannot be applied to double.");
			YYERROR;
		case COMPLEX:
			yyerror((const char *)">> cannot be applied to complex.");
			YYERROR;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 67:
# line 2296 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 68:
# line 2300 "mybc.y"
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
			case COMPLEX:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1((double)yypvt[-2].number.value.ulvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					result = c1+c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex addition failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
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
			case COMPLEX:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1((double)yypvt[-2].number.value.lvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					result = c1+c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex addition failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
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
			case COMPLEX:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1(yypvt[-2].number.value.dvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					result = c1+c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex addition failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.ulvalue);
				try {
					result = c1+c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex addition failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
			case LONG:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.lvalue);
				try {
					result = c1+c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex addition failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
			case DOUBLE:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3(yypvt[-0].number.value.dvalue);
				try {
					result = c1+c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex addition failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
			case COMPLEX:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					result = c1+c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex addition failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
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
case 69:
# line 2519 "mybc.y"
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
			case COMPLEX:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1((double)yypvt[-2].number.value.ulvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					result = c1-c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex subtraction failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
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
			case COMPLEX:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1((double)yypvt[-2].number.value.lvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					result = c1-c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex subtraction failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
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
			case COMPLEX:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1(yypvt[-2].number.value.dvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					result = c1-c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex subtraction failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.ulvalue);
				try {
					result = c1-c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex subtraction failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
			case LONG:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.lvalue);
				try {
					result = c1-c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex subtraction failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
			case DOUBLE:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3(yypvt[-0].number.value.dvalue);
				try {
					result = c1-c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex subtraction failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
			case COMPLEX:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					result = c1-c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex subtraction failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
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
case 70:
# line 2741 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 71:
# line 2745 "mybc.y"
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
			case COMPLEX:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1((double)yypvt[-2].number.value.ulvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					result = c1*c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex multiply failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
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
			case COMPLEX:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1((double)yypvt[-2].number.value.lvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					result = c1*c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex multiply failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
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
			case COMPLEX:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1(yypvt[-2].number.value.dvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					result = c1*c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex multiply failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.ulvalue);
				try {
					result = c1*c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex multiply failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
			case LONG:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.lvalue);
				try {
					result = c1*c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex multiply failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
			case DOUBLE:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3(yypvt[-0].number.value.dvalue);
				try {
					result = c1*c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex multiply failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
			case COMPLEX:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					result = c1*c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex multiply failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
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
case 72:
# line 2964 "mybc.y"
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
			case COMPLEX:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1((double)yypvt[-2].number.value.ulvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					result = c1/c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex divide failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
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
			case COMPLEX:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1((double)yypvt[-2].number.value.lvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					result = c1/c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex divide failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
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
			case COMPLEX:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1(yypvt[-2].number.value.dvalue);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					result = c1/c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex divide failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.ulvalue);
				try {
					result = c1/c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex divide failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
			case LONG:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3((double)yypvt[-0].number.value.lvalue);
				try {
					result = c1/c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex divide failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
			case DOUBLE:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3(yypvt[-0].number.value.dvalue);
				try {
					result = c1/c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex divide failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
			case COMPLEX:
			{
				yyval.number.type = COMPLEX;
				Complex<double> 
					c1(yypvt[-2].number.value.cvalue.r,yypvt[-2].number.value.cvalue.i);
				Complex<double> 
					c3(yypvt[-0].number.value.cvalue.r,yypvt[-0].number.value.cvalue.i);
				try {
					result = c1/c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex divide failed.", __LINE__);
					YYERROR;
				}
				yyval.number.value.cvalue.r = real(result);
				yyval.number.value.cvalue.i = imag(result);
				break;
			}
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
case 73:
# line 3228 "mybc.y"
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
			case COMPLEX:
				yyerror((const char *)"% cannot be applied to complex.");
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
			case COMPLEX:
				yyerror((const char *)"% cannot be applied to complex.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror((const char *)"% cannot be applied to double.");
			YYERROR;
		case COMPLEX:
			yyerror((const char *)"% cannot be applied to complex.");
			YYERROR;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	} break;
case 74:
# line 3314 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 75:
# line 3318 "mybc.y"
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
			case COMPLEX:
				yyerror((const char *)"cannot typecast complex.");
				YYERROR;
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
			case COMPLEX:
				yyerror((const char *)"cannot typecast complex.");
				YYERROR;
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
			case COMPLEX:
				yyerror((const char *)"cannot typecast complex.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch (yypvt[-0].number.type)
			{
			case ULONG:
				yyval.number.type = COMPLEX;
				yyval.number.value.cvalue.r = (double)yypvt[-0].number.value.ulvalue;
				yyval.number.value.cvalue.i = 0;
				break;
			case LONG:
				yyval.number.type = COMPLEX;
				yyval.number.value.cvalue.r = (double)yypvt[-0].number.value.lvalue;
				yyval.number.value.cvalue.i = 0;
				break;
			case DOUBLE:
				yyval.number.type = COMPLEX;
				yyval.number.value.cvalue.r = yypvt[-0].number.value.dvalue;
				yyval.number.value.cvalue.i = 0;
				break;
			case COMPLEX:
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
case 76:
# line 3424 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 77:
# line 3428 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 78:
# line 3432 "mybc.y"
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
		case COMPLEX:
			yypvt[-0].number.value.cvalue.r *= -1.0;
			yypvt[-0].number.value.cvalue.i *= -1.0;
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		yyval.number = yypvt[-0].number;
	} break;
case 79:
# line 3455 "mybc.y"
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
		case COMPLEX:
			yyerror((const char *)"~ cannot be applied to complex.");
			YYERROR;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		yyval.number = yypvt[-0].number;
	} break;
case 80:
# line 3477 "mybc.y"
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
		case COMPLEX:
			if (yypvt[-0].number.value.cvalue.r != 0 || yypvt[-0].number.value.cvalue.i != 0)
			{
				yypvt[-0].number.value.cvalue.r = 0;
				yypvt[-0].number.value.cvalue.i = 0;
			}
			else
			{
				yypvt[-0].number.value.cvalue.r = 1;
				yypvt[-0].number.value.cvalue.i = 0;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		yyval.number = yypvt[-0].number;
	} break;
case 81:
# line 3511 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 82:
# line 3515 "mybc.y"
{
		if (yypvt[-1].number.type != COMPLEX)
		{
			yypvt[-1].number = number2double(yypvt[-1].number);
			if (yypvt[-1].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			yyval.number.type = DOUBLE;
			yyval.number.value.dvalue = exp(yypvt[-1].number.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex(yypvt[-1].number);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = exp(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex exp failed.");
				YYERROR;
			}
			yyval.number.type = COMPLEX;
			yyval.number.value.cvalue.r = real(c2);
			yyval.number.value.cvalue.i = imag(c2);
		}
	} break;
case 83:
# line 3546 "mybc.y"
{
		if (yypvt[-1].number.type != COMPLEX)
		{
			yypvt[-1].number = number2double(yypvt[-1].number);
			if (yypvt[-1].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			if (yypvt[-1].number.value.dvalue < 0)
			{
				mybcerror((const char *)"log of negative number.", 
						__LINE__);
				YYERROR;
			}
			yyval.number.type = DOUBLE;
			yyval.number.value.dvalue = log(yypvt[-1].number.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex(yypvt[-1].number);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = log(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex log failed.");
				YYERROR;
			}
			yyval.number.type = COMPLEX;
			yyval.number.value.cvalue.r = real(c2);
			yyval.number.value.cvalue.i = imag(c2);
		}
	} break;
case 84:
# line 3583 "mybc.y"
{
		if (yypvt[-1].number.type != COMPLEX)
		{
			yypvt[-1].number = number2double(yypvt[-1].number);
			if (yypvt[-1].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			if (yypvt[-1].number.value.dvalue < 0)
			{
				mybcerror((const char *)"log10 of negative number.", 
					__LINE__);
				YYERROR;
			}
			yyval.number.type = DOUBLE;
			yyval.number.value.dvalue = log10(yypvt[-1].number.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex(yypvt[-1].number);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = log10(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex log10 failed.");
				YYERROR;
			}
			yyval.number.type = COMPLEX;
			yyval.number.value.cvalue.r = real(c2);
			yyval.number.value.cvalue.i = imag(c2);
		}
	} break;
case 85:
# line 3620 "mybc.y"
{
		if (yypvt[-3].number.type != COMPLEX)
		{
			yypvt[-3].number = number2double(yypvt[-3].number);
			if (yypvt[-3].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			if (yypvt[-3].number.value.dvalue < 0)
			{
				mybcerror((const char *)"log of negative number.", 
						__LINE__);
				YYERROR;
			}
			yyval.number.type = DOUBLE;
			yyval.number.value.dvalue = log(yypvt[-3].number.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex(yypvt[-3].number);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = log(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex log failed.");
				YYERROR;
			}
			yyval.number.type = COMPLEX;
			yyval.number.value.cvalue.r = real(c2);
			yyval.number.value.cvalue.i = imag(c2);
		}
		if (yypvt[-3].number.type != COMPLEX && yypvt[-1].number.type != COMPLEX)
		{
			yypvt[-3].number = number2double(yypvt[-3].number);
			if (yypvt[-3].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			yypvt[-1].number = number2double(yypvt[-1].number);
			if (yypvt[-1].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			yyval.number.type = DOUBLE;
			yyval.number.value.dvalue = pow(yypvt[-3].number.value.dvalue, yypvt[-1].number.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex(yypvt[-3].number);
			Complex<double> c2 = number2complex(yypvt[-1].number);
			Complex<double> c3(0.0, 0.0);
			try {
				c3 = pow(c1, c2);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex pow failed.");
				YYERROR;
			}
			yyval.number.type = COMPLEX;
			yyval.number.value.cvalue.r = real(c3);
			yyval.number.value.cvalue.i = imag(c3);
		}
	} break;
case 86:
# line 3693 "mybc.y"
{
		if (yypvt[-3].number.type == COMPLEX || yypvt[-1].number.type == COMPLEX)
		{
			mybcerror((const char *)"C(n,r) not supported for complex.", __LINE__);
			YYERROR;
		}
		if ((yypvt[-3].number.type == ULONG || yypvt[-3].number.type == LONG) &&
		    (yypvt[-1].number.type == ULONG || yypvt[-1].number.type == LONG))
		{
			if (yypvt[-3].number.type == LONG)
			{
				if (yypvt[-3].number.value.lvalue < 0)
				{
					mybcerror((const char *)"n < 0 is not allowed.", __LINE__);
					YYERROR;
				}
				yypvt[-3].number.type = ULONG;
				yypvt[-3].number.value.ulvalue = yypvt[-3].number.value.lvalue;
			}
			if (yypvt[-1].number.type == LONG)
			{
				if (yypvt[-1].number.value.lvalue < 0)
				{
					mybcerror((const char *)"r < 0 is not allowed.", __LINE__);
					YYERROR;
				}
				yypvt[-1].number.type = ULONG;
				yypvt[-1].number.value.ulvalue = yypvt[-1].number.value.lvalue;
			}
			if (yypvt[-3].number.value.ulvalue < yypvt[-1].number.value.ulvalue)
			{
				mybcerror((const char *)"n < r not allowed.", __LINE__);
				YYERROR;
			}
			yyval.number.type = ULONG;
			yyval.number.value.ulvalue = comb(yypvt[-3].number.value.ulvalue, yypvt[-1].number.value.ulvalue);
		}
		else
		{
			yypvt[-3].number = number2double(yypvt[-3].number);
			if (yypvt[-3].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			yypvt[-1].number = number2double(yypvt[-1].number);
			if (yypvt[-1].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			if (yypvt[-3].number.value.dvalue < yypvt[-1].number.value.dvalue)
			{
				mybcerror((const char *)"n < r not allowed.", __LINE__);
				YYERROR;
			}
			yyval.number.type = DOUBLE;
			yyval.number.value.dvalue = comb(yypvt[-3].number.value.dvalue, yypvt[-1].number.value.dvalue);
		}
	} break;
case 87:
# line 3756 "mybc.y"
{
		if (yypvt[-3].number.type == COMPLEX || yypvt[-1].number.type == COMPLEX)
		{
			mybcerror((const char *)"P(n,r) not supported for complex.", __LINE__);
			YYERROR;
		}
		if ((yypvt[-3].number.type == ULONG || yypvt[-3].number.type == LONG) &&
		    (yypvt[-1].number.type == ULONG || yypvt[-1].number.type == LONG))
		{
			if (yypvt[-3].number.type == LONG)
			{
				if (yypvt[-3].number.value.lvalue < 0)
				{
					mybcerror((const char *)"n < 0 is not allowed.", __LINE__);
					YYERROR;
				}
				yypvt[-3].number.type = ULONG;
				yypvt[-3].number.value.ulvalue = yypvt[-3].number.value.lvalue;
			}
			if (yypvt[-1].number.type == LONG)
			{
				if (yypvt[-1].number.value.lvalue < 0)
				{
					mybcerror((const char *)"r < 0 is not allowed.", __LINE__);
					YYERROR;
				}
				yypvt[-1].number.type = ULONG;
				yypvt[-1].number.value.ulvalue = yypvt[-1].number.value.lvalue;
			}
			if (yypvt[-3].number.value.ulvalue < yypvt[-1].number.value.ulvalue)
			{
				mybcerror((const char *)"n < r not allowed.", __LINE__);
				YYERROR;
			}
			yyval.number.type = ULONG;
			yyval.number.value.ulvalue = perm(yypvt[-3].number.value.ulvalue, yypvt[-1].number.value.ulvalue);
		}
		else
		{
			yypvt[-3].number = number2double(yypvt[-3].number);
			if (yypvt[-3].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			yypvt[-1].number = number2double(yypvt[-1].number);
			if (yypvt[-1].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			if (yypvt[-3].number.value.dvalue < yypvt[-1].number.value.dvalue)
			{
				mybcerror((const char *)"n < r not allowed.", __LINE__);
				YYERROR;
			}
			yyval.number.type = DOUBLE;
			yyval.number.value.dvalue = perm(yypvt[-3].number.value.dvalue, yypvt[-1].number.value.dvalue);
		}
	} break;
case 88:
# line 3819 "mybc.y"
{
		if (yypvt[-5].number.type == COMPLEX || yypvt[-3].number.type == COMPLEX || yypvt[-1].number.type == COMPLEX)
		{
			mybcerror((const char *)"usury(p,i,lp) not supported for complex.", __LINE__);
			YYERROR;
		}
		yypvt[-5].number = number2double(yypvt[-5].number);
		if (yypvt[-5].number.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", 
					__LINE__);
			YYERROR;
		}
		yypvt[-3].number = number2double(yypvt[-3].number);
		if (yypvt[-3].number.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", 
					__LINE__);
			YYERROR;
		}
		yypvt[-1].number = number2double(yypvt[-1].number);
		if (yypvt[-1].number.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", 
					__LINE__);
			YYERROR;
		}
		try {
			yyval.number.type = VOID;
			MustBeTrue(yypvt[-5].number.value.dvalue > 0.0);
			MustBeTrue(yypvt[-3].number.value.dvalue > 0.0);
			MustBeTrue(yypvt[-1].number.value.dvalue > 0.0);
			usury(yypvt[-5].number.value.dvalue, yypvt[-3].number.value.dvalue, 
				yypvt[-1].number.value.dvalue);
		}
		catch (const char *pe)
		{
			yyerror((const char *)"usury failed.");
			YYERROR;
		}
	} break;
case 89:
# line 3861 "mybc.y"
{
		if (yypvt[-1].number.type == COMPLEX)
		{
			mybcerror((const char *)"fact(n) not supported for complex.", __LINE__);
			YYERROR;
		}
		if (yypvt[-1].number.type == ULONG || yypvt[-1].number.type == LONG)
		{
			if (yypvt[-1].number.type == LONG)
			{
				if (yypvt[-1].number.value.lvalue < 0)
				{
					mybcerror((const char *)"n < 0 is not allowed.", __LINE__);
					YYERROR;
				}
				yypvt[-1].number.type = ULONG;
				yypvt[-1].number.value.ulvalue = yypvt[-1].number.value.lvalue;
			}
			if (yypvt[-1].number.value.ulvalue <= 12)
			{
				/* ulong overflows with n > 12 */
				yyval.number.type = ULONG;
				yyval.number.value.ulvalue = fact(yypvt[-1].number.value.ulvalue);
			}
			else
			{
				/* ulong overflows with n > 12 */
				yypvt[-1].number = number2double(yypvt[-1].number);
				if (yypvt[-1].number.type != DOUBLE)
				{
					mybcerror((const char *)"default case.", 
							__LINE__);
					YYERROR;
				}
				yyval.number.type = DOUBLE;
				yyval.number.value.dvalue = fact(yypvt[-1].number.value.dvalue);
			}
		}
		else
		{
			yypvt[-1].number = number2double(yypvt[-1].number);
			if (yypvt[-1].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			yyval.number.type = DOUBLE;
			yyval.number.value.dvalue = fact(yypvt[-1].number.value.dvalue);
		}
	} break;
case 90:
# line 3913 "mybc.y"
{
		if (yypvt[-1].number.type != COMPLEX)
		{
			yypvt[-1].number = number2double(yypvt[-1].number);
			if (yypvt[-1].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			yyval.number.type = DOUBLE;
			yyval.number.value.dvalue = sqrt(yypvt[-1].number.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex(yypvt[-1].number);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = sqrt(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex sqrt failed.");
				YYERROR;
			}
			yyval.number.type = COMPLEX;
			yyval.number.value.cvalue.r = real(c2);
			yyval.number.value.cvalue.i = imag(c2);
		}
	} break;
case 91:
# line 3944 "mybc.y"
{
		if (yypvt[-1].number.type != COMPLEX)
		{
			yypvt[-1].number = number2double(yypvt[-1].number);
			if (yypvt[-1].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			yyval.number.type = DOUBLE;
			yyval.number.value.dvalue = sin(yypvt[-1].number.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex(yypvt[-1].number);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = sin(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex sin failed.");
				YYERROR;
			}
			yyval.number.type = COMPLEX;
			yyval.number.value.cvalue.r = real(c2);
			yyval.number.value.cvalue.i = imag(c2);
		}
	} break;
case 92:
# line 3974 "mybc.y"
{
		if (yypvt[-1].number.type != COMPLEX)
		{
			yypvt[-1].number = number2double(yypvt[-1].number);
			if (yypvt[-1].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			yyval.number.type = DOUBLE;
			yyval.number.value.dvalue = cos(yypvt[-1].number.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex(yypvt[-1].number);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = cos(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex cos failed.");
				YYERROR;
			}
			yyval.number.type = COMPLEX;
			yyval.number.value.cvalue.r = real(c2);
			yyval.number.value.cvalue.i = imag(c2);
		}
	} break;
case 93:
# line 4005 "mybc.y"
{
		if (yypvt[-1].number.type != COMPLEX)
		{
			yypvt[-1].number = number2double(yypvt[-1].number);
			if (yypvt[-1].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			yyval.number.type = DOUBLE;
			yyval.number.value.dvalue = tan(yypvt[-1].number.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex(yypvt[-1].number);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = tan(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex tan failed.");
				YYERROR;
			}
			yyval.number.type = COMPLEX;
			yyval.number.value.cvalue.r = real(c2);
			yyval.number.value.cvalue.i = imag(c2);
		}
	} break;
case 94:
# line 4035 "mybc.y"
{
		if (yypvt[-1].number.type != COMPLEX)
		{
			yypvt[-1].number = number2double(yypvt[-1].number);
			if (yypvt[-1].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			yyval.number.type = DOUBLE;
			yyval.number.value.dvalue = sin(yypvt[-1].number.value.dvalue);
			if (yyval.number.value.dvalue != 0.0)
			{
				yyval.number.value.dvalue /= 1.0;
			}
			else
			{
				yyerror((const char *)"csc division by zero.");
				YYERROR;
			}
		}
		else
		{
			Complex<double> c1 = number2complex(yypvt[-1].number);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = sin(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex sin failed.");
				YYERROR;
			}
			try {
				c2 /= 1.0;
			}
			catch (const char *pe)
			{
				yyerror((const char *)"csc complex division by zero.");
				YYERROR;
			}
			yyval.number.type = COMPLEX;
			yyval.number.value.cvalue.r = real(c2);
			yyval.number.value.cvalue.i = imag(c2);
		}
	} break;
case 95:
# line 4082 "mybc.y"
{
		if (yypvt[-1].number.type != COMPLEX)
		{
			yypvt[-1].number = number2double(yypvt[-1].number);
			if (yypvt[-1].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			yyval.number.type = DOUBLE;
			yyval.number.value.dvalue = cos(yypvt[-1].number.value.dvalue);
			if (yyval.number.value.dvalue != 0.0)
			{
				yyval.number.value.dvalue /= 1.0;
			}
			else
			{
				yyerror((const char *)"sec division by zero.");
				YYERROR;
			}
		}
		else
		{
			Complex<double> c1 = number2complex(yypvt[-1].number);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = cos(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex cos failed.");
				YYERROR;
			}
			try {
				c2 /= 1.0;
			}
			catch (const char *pe)
			{
				yyerror((const char *)"sec complex division by zero.");
				YYERROR;
			}
			yyval.number.type = COMPLEX;
			yyval.number.value.cvalue.r = real(c2);
			yyval.number.value.cvalue.i = imag(c2);
		}
	} break;
case 96:
# line 4129 "mybc.y"
{
		if (yypvt[-1].number.type != COMPLEX)
		{
			yypvt[-1].number = number2double(yypvt[-1].number);
			if (yypvt[-1].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			yyval.number.type = DOUBLE;
			yyval.number.value.dvalue = tan(yypvt[-1].number.value.dvalue);
			if (yyval.number.value.dvalue != 0.0)
			{
				yyval.number.value.dvalue /= 1.0;
			}
			else
			{
				yyerror((const char *)"cot division by zero.");
				YYERROR;
			}
		}
		else
		{
			Complex<double> c1 = number2complex(yypvt[-1].number);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = tan(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex tan failed.");
				YYERROR;
			}
			try {
				c2 /= 1.0;
			}
			catch (const char *pe)
			{
				yyerror((const char *)"cot complex division by zero.");
				YYERROR;
			}
			yyval.number.type = COMPLEX;
			yyval.number.value.cvalue.r = real(c2);
			yyval.number.value.cvalue.i = imag(c2);
		}
	} break;
case 97:
# line 4176 "mybc.y"
{
		if (yypvt[-1].number.type != COMPLEX)
		{
			yypvt[-1].number = number2double(yypvt[-1].number);
			if (yypvt[-1].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			yyval.number.type = DOUBLE;
			yyval.number.value.dvalue = asin(yypvt[-1].number.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex(yypvt[-1].number);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = asin(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex asin failed.");
				YYERROR;
			}
			yyval.number.type = COMPLEX;
			yyval.number.value.cvalue.r = real(c2);
			yyval.number.value.cvalue.i = imag(c2);
		}
	} break;
case 98:
# line 4206 "mybc.y"
{
		if (yypvt[-1].number.type != COMPLEX)
		{
			yypvt[-1].number = number2double(yypvt[-1].number);
			if (yypvt[-1].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			yyval.number.type = DOUBLE;
			yyval.number.value.dvalue = acos(yypvt[-1].number.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex(yypvt[-1].number);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = acos(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex acos failed.");
				YYERROR;
			}
			yyval.number.type = COMPLEX;
			yyval.number.value.cvalue.r = real(c2);
			yyval.number.value.cvalue.i = imag(c2);
		}
	} break;
case 99:
# line 4236 "mybc.y"
{
		if (yypvt[-1].number.type != COMPLEX)
		{
			yypvt[-1].number = number2double(yypvt[-1].number);
			if (yypvt[-1].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			yyval.number.type = DOUBLE;
			yyval.number.value.dvalue = atan(yypvt[-1].number.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex(yypvt[-1].number);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = atan(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex atan failed.");
				YYERROR;
			}
			yyval.number.type = COMPLEX;
			yyval.number.value.cvalue.r = real(c2);
			yyval.number.value.cvalue.i = imag(c2);
		}
	} break;
case 100:
# line 4266 "mybc.y"
{
		if (yypvt[-1].number.type != COMPLEX)
		{
			yypvt[-1].number = number2double(yypvt[-1].number);
			if (yypvt[-1].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			yyval.number.type = DOUBLE;
			yyval.number.value.dvalue = sinh(yypvt[-1].number.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex(yypvt[-1].number);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = sinh(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex sinh failed.");
				YYERROR;
			}
			yyval.number.type = COMPLEX;
			yyval.number.value.cvalue.r = real(c2);
			yyval.number.value.cvalue.i = imag(c2);
		}
	} break;
case 101:
# line 4296 "mybc.y"
{
		if (yypvt[-1].number.type != COMPLEX)
		{
			yypvt[-1].number = number2double(yypvt[-1].number);
			if (yypvt[-1].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			yyval.number.type = DOUBLE;
			yyval.number.value.dvalue = cosh(yypvt[-1].number.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex(yypvt[-1].number);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = cosh(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex cosh failed.");
				YYERROR;
			}
			yyval.number.type = COMPLEX;
			yyval.number.value.cvalue.r = real(c2);
			yyval.number.value.cvalue.i = imag(c2);
		}
	} break;
case 102:
# line 4327 "mybc.y"
{
		if (yypvt[-1].number.type != COMPLEX)
		{
			yypvt[-1].number = number2double(yypvt[-1].number);
			if (yypvt[-1].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			yyval.number.type = DOUBLE;
			yyval.number.value.dvalue = tanh(yypvt[-1].number.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex(yypvt[-1].number);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = tanh(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex tanh failed.");
				YYERROR;
			}
			yyval.number.type = COMPLEX;
			yyval.number.value.cvalue.r = real(c2);
			yyval.number.value.cvalue.i = imag(c2);
		}
	} break;
case 103:
# line 4357 "mybc.y"
{
		if (yypvt[-1].number.type != COMPLEX)
		{
			yypvt[-1].number = number2double(yypvt[-1].number);
			if (yypvt[-1].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			yyval.number.type = DOUBLE;
			yyval.number.value.dvalue = asinh(yypvt[-1].number.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex(yypvt[-1].number);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = asinh(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex asinh failed.");
				YYERROR;
			}
			yyval.number.type = COMPLEX;
			yyval.number.value.cvalue.r = real(c2);
			yyval.number.value.cvalue.i = imag(c2);
		}
	} break;
case 104:
# line 4387 "mybc.y"
{
		if (yypvt[-1].number.type != COMPLEX)
		{
			yypvt[-1].number = number2double(yypvt[-1].number);
			if (yypvt[-1].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			yyval.number.type = DOUBLE;
			yyval.number.value.dvalue = acosh(yypvt[-1].number.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex(yypvt[-1].number);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = acosh(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex acosh failed.");
				YYERROR;
			}
			yyval.number.type = COMPLEX;
			yyval.number.value.cvalue.r = real(c2);
			yyval.number.value.cvalue.i = imag(c2);
		}
	} break;
case 105:
# line 4417 "mybc.y"
{
		if (yypvt[-1].number.type != COMPLEX)
		{
			yypvt[-1].number = number2double(yypvt[-1].number);
			if (yypvt[-1].number.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			yyval.number.type = DOUBLE;
			yyval.number.value.dvalue = atanh(yypvt[-1].number.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex(yypvt[-1].number);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = atanh(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex atanh failed.");
				YYERROR;
			}
			yyval.number.type = COMPLEX;
			yyval.number.value.cvalue.r = real(c2);
			yyval.number.value.cvalue.i = imag(c2);
		}
	} break;
case 106:
# line 4447 "mybc.y"
{
		switch (yypvt[-1].number.type)
		{
		case ULONG:
			break;
		case DOUBLE:
			if (yypvt[-1].number.value.dvalue < 0.0)
				yypvt[-1].number.value.dvalue *= -1.0;
			break;
		case LONG:
			if (yypvt[-1].number.value.lvalue < 0)
				yypvt[-1].number.value.lvalue *= -1;
			break;
		case COMPLEX:
		{
			Complex<double> c1 = number2complex(yypvt[-1].number);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = abs(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex abs failed.");
				YYERROR;
			}
			yypvt[-1].number.type = COMPLEX;
			yypvt[-1].number.value.cvalue.r = real(c2);
			yypvt[-1].number.value.cvalue.i = imag(c2);
			break;
		}
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		yyval.number = yypvt[-1].number;
	} break;
case 107:
# line 4484 "mybc.y"
{
		Complex<double> c1 = number2complex(yypvt[-1].number);
		Complex<double> c2(0.0, 0.0);
		try {
			c2 = arg(c1);
		}
		catch (const char *pe)
		{
			yyerror((const char *)"complex arg failed.");
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = real(c2);
	} break;
case 108:
# line 4499 "mybc.y"
{
		Complex<double> c1 = number2complex(yypvt[-1].number);
		Complex<double> c2(0.0, 0.0);
		try {
			c2 = norm(c1);
		}
		catch (const char *pe)
		{
			yyerror((const char *)"complex norm failed.");
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = real(c2);
	} break;
case 109:
# line 4514 "mybc.y"
{
		Complex<double> c1 = number2complex(yypvt[-1].number);
		Complex<double> c2(0.0, 0.0);
		try {
			c2 = real(c1);
		}
		catch (const char *pe)
		{
			yyerror((const char *)"complex real failed.");
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = real(c2);
	} break;
case 110:
# line 4529 "mybc.y"
{
		Complex<double> c1 = number2complex(yypvt[-1].number);
		Complex<double> c2(0.0, 0.0);
		try {
			c2 = imag(c1);
		}
		catch (const char *pe)
		{
			yyerror((const char *)"complex imag failed.");
			YYERROR;
		}
		yyval.number.type = DOUBLE;
		yyval.number.value.dvalue = real(c2);
	} break;
case 111:
# line 4544 "mybc.y"
{
		if (yypvt[-3].number.type != COMPLEX && yypvt[-1].number.type != COMPLEX)
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
			yyval.number.value.cvalue.r = yypvt[-3].number.value.dvalue;
			yyval.number.value.cvalue.i = yypvt[-1].number.value.dvalue;
			yyval.number.type = COMPLEX;
		}
		else
		{
			yyerror((const char *)"cannot include complex in a complex.");
			YYERROR;
		}
	} break;
case 112:
# line 4570 "mybc.y"
{
		yyval.number = yypvt[-1].number;
	} break;
case 113:
# line 4577 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 114:
# line 4581 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 115:
# line 4585 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 116:
# line 4589 "mybc.y"
{
		yyval.number = yypvt[-0].number;
	} break;
case 117:
# line 4596 "mybc.y"
{
		yyval.type = ULONG;
	} break;
case 118:
# line 4600 "mybc.y"
{
		yyval.type = LONG;
	} break;
case 119:
# line 4604 "mybc.y"
{
		yyval.type = DOUBLE;
	} break;
case 120:
# line 4608 "mybc.y"
{
		yyval.type = COMPLEX;
	} break;
# line	531 "/usr/ccs/bin/yaccpar"
	}
	goto yystack;		/* reset registers in driver code */
}

