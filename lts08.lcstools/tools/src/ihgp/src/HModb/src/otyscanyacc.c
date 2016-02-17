
# line 3 "otyscan.y"
// parser for oty files

// headers
#include "otyscan.h"

// externs
extern int optind;
extern char *optarg;

// globals
int verbose = 0;
int syntaxflag = 0;


# line 20 "otyscan.y"
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif

{
	char cvalue[128];
} YYSTYPE;
# define NEWLINE 257
# define MSGID 258
# define TIME 259
# define CLASS 260
# define ACTION 261
# define OUTPRIOR 262
# define FREQ 263
# define PROTOTYPE 264
# define FIELD 265
# define FIELDUP 266
# define NPVAL 267
# define WIDTH 268
# define ENUM 269
# define OUTPUT 270
# define LEFTBRACE 271
# define RIGHTBRACE 272

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

# line 140 "otyscan.y"


// report yacc errors
int
yyerror(char *s)
{
	if (syntaxflag)
	{
		fprintf(stdout, "%s.\n", s);
	}
	return(0);
}

// usage msg
void
usage(const char *cmd)
{
	fprintf(stderr, 
		"usage: %s [-?V] \n\n"
		"where\n\n"
		"-? = help flag.\n"
		"-V = verbose flag.\n"
		"%s reads stdin and writes to stdout.\n", 
		cmd, cmd);
	return;
}

// main entry point
main(int argc, char **argv)
{
	int c;
#ifdef YYDEBUG
	extern int yydebug;
	syntaxflag = 1;
	yydebug = 1;
	setbuf(stdout, (char *)0);
#endif

	// check options
	while ((c = getopt(argc, argv, "?V")) != EOF)
	{
		switch (c)
		{
		case 'V':
			// turn on  verbose option
			verbose = 1;
			syntaxflag = 1;
			break;

		case '?':
			usage(argv[0]);
			exit(0);

		default:
			errno = EINVAL;
			ERROR("invalid command line option", errno);
			exit(2);
		}
	}

	// scan oty files
	VERBOSE(stderr, "yyparse start ...\n");
	yyparse();
	VERBOSE(stderr, "yyparse finish ...\n");

	// all done
	exit(0);
}

yytabelem yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 28
# define YYLAST 53
yytabelem yyact[]={

    28,    29,    13,    14,    15,    16,    17,    18,    19,    20,
    21,    22,    23,    24,     7,    40,    32,     9,    10,    13,
    14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
    24,     4,     3,    25,    39,    38,    37,    36,    35,    34,
    33,    26,    12,    11,    27,     6,     8,     5,     2,     1,
     0,    31,    30 };
yytabelem yypact[]={

-10000000,  -225,-10000000,-10000000,  -258,  -239,  -238,  -216,  -256,  -217,
-10000000,  -218,  -219,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,  -220,-10000000,-10000000,  -221,-10000000,
  -222,  -223,  -242,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000 };
yytabelem yypgo[]={

     0,    49,    48,    47,    46,    44,    43,    42 };
yytabelem yyr1[]={

     0,     1,     1,     1,     1,     2,     3,     5,     4,     4,
     4,     4,     4,     4,     4,     4,     6,     6,     6,     6,
     6,     6,     6,     7,     7,     7,     7,     7 };
yytabelem yyr2[]={

     0,     0,     4,     4,     9,     6,     7,     5,     5,     7,
     2,     4,     4,     6,     4,     6,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3 };
yytabelem yychk[]={

-10000000,    -1,    -2,   257,   256,    -3,   270,   272,    -4,   256,
   257,    -6,    -7,   258,   259,   260,   261,   262,   263,   264,
   265,   266,   267,   268,   269,   271,   257,    -5,   256,   257,
    -6,    -7,   272,   257,   257,   257,   257,   257,   257,   257,
   257 };
yytabelem yydef[]={

     1,    -2,     2,     3,     0,     0,     0,     0,     0,     0,
    10,     0,     0,    16,    17,    18,    19,    20,    21,    22,
    23,    24,    25,    26,    27,     0,     4,     5,     0,    11,
     0,     0,     0,     8,    12,    14,     6,     9,    13,    15,
     7 };
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
	"MSGID",	258,
	"TIME",	259,
	"CLASS",	260,
	"ACTION",	261,
	"OUTPRIOR",	262,
	"FREQ",	263,
	"PROTOTYPE",	264,
	"FIELD",	265,
	"FIELDUP",	266,
	"NPVAL",	267,
	"WIDTH",	268,
	"ENUM",	269,
	"OUTPUT",	270,
	"LEFTBRACE",	271,
	"RIGHTBRACE",	272,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"begin : /* empty */",
	"begin : begin msg",
	"begin : begin NEWLINE",
	"begin : begin error RIGHTBRACE NEWLINE",
	"msg : msgstart msgbody msgend",
	"msgstart : OUTPUT LEFTBRACE NEWLINE",
	"msgend : RIGHTBRACE NEWLINE",
	"msgbody : error NEWLINE",
	"msgbody : msgbody error NEWLINE",
	"msgbody : NEWLINE",
	"msgbody : msgbody NEWLINE",
	"msgbody : requiredbody NEWLINE",
	"msgbody : msgbody requiredbody NEWLINE",
	"msgbody : optionalbody NEWLINE",
	"msgbody : msgbody optionalbody NEWLINE",
	"requiredbody : MSGID",
	"requiredbody : TIME",
	"requiredbody : CLASS",
	"requiredbody : ACTION",
	"requiredbody : OUTPRIOR",
	"requiredbody : FREQ",
	"requiredbody : PROTOTYPE",
	"optionalbody : FIELD",
	"optionalbody : FIELDUP",
	"optionalbody : NPVAL",
	"optionalbody : WIDTH",
	"optionalbody : ENUM",
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
		
case 4:
# line 48 "otyscan.y"
{
			yyerrok;
		} break;
case 6:
# line 60 "otyscan.y"
{
			msgstartSemantics();
		} break;
case 7:
# line 66 "otyscan.y"
{
			msgendSemantics();
		} break;
case 8:
# line 72 "otyscan.y"
{
			yyerrok;
		} break;
case 9:
# line 76 "otyscan.y"
{
			yyerrok;
		} break;
case 16:
# line 88 "otyscan.y"
{
			msgidSemantics(yypvt[-0].cvalue);
		} break;
case 17:
# line 92 "otyscan.y"
{
			timeSemantics(yypvt[-0].cvalue);
		} break;
case 18:
# line 96 "otyscan.y"
{
			classSemantics(yypvt[-0].cvalue);
		} break;
case 19:
# line 100 "otyscan.y"
{
			actionSemantics(yypvt[-0].cvalue);
		} break;
case 20:
# line 104 "otyscan.y"
{
			outpriorSemantics(yypvt[-0].cvalue);
		} break;
case 21:
# line 108 "otyscan.y"
{
			freqSemantics(yypvt[-0].cvalue);
		} break;
case 22:
# line 112 "otyscan.y"
{
			prototypeSemantics(yypvt[-0].cvalue);
		} break;
case 23:
# line 119 "otyscan.y"
{
			fieldSemantics(yypvt[-0].cvalue);
		} break;
case 24:
# line 123 "otyscan.y"
{
			fieldupSemantics(yypvt[-0].cvalue);
		} break;
case 25:
# line 127 "otyscan.y"
{
			npvalSemantics(yypvt[-0].cvalue);
		} break;
case 26:
# line 131 "otyscan.y"
{
			widthSemantics(yypvt[-0].cvalue);
		} break;
case 27:
# line 135 "otyscan.y"
{
			enumSemantics(yypvt[-0].cvalue);
		} break;
# line	532 "/usr/ccs/bin/yaccpar"
	}
	goto yystack;		/* reset registers in driver code */
}

