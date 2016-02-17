#include <stdio.h>
# define U(x) x
# define NLSTATE yyprevious=YYNEWLINE
# define BEGIN yybgin = yysvec + 1 +
# define INITIAL 0
# define YYLERR yysvec
# define YYSTATE (yyestate-yysvec-1)
# define YYOPTIM 1
# define YYLMAX BUFSIZ
#ifndef __cplusplus
# define output(c) (void)putc(c,yyout)
#else
# define lex_output(c) (void)putc(c,yyout)
#endif

#if defined(__cplusplus) || defined(__STDC__)

#if defined(__cplusplus) && defined(__EXTERN_C__)
extern "C" {
#endif
	int yyback(int *, int);
	int yyinput(void);
	int yylook(void);
	void yyoutput(int);
	int yyracc(int);
	int yyreject(void);
	void yyunput(int);
	int yylex(void);
#ifdef YYLEX_E
	void yywoutput(wchar_t);
	wchar_t yywinput(void);
#endif
#ifndef yyless
	int yyless(int);
#endif
#ifndef yywrap
	int yywrap(void);
#endif
#ifdef LEXDEBUG
	void allprint(char);
	void sprint(char *);
#endif
#if defined(__cplusplus) && defined(__EXTERN_C__)
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
	void exit(int);
#ifdef __cplusplus
}
#endif

#endif
# define unput(c) {yytchar= (c);if(yytchar=='\n')yylineno--;*yysptr++=yytchar;}
# define yymore() (yymorfg=1)
#ifndef __cplusplus
# define input() (((yytchar=yysptr>yysbuf?U(*--yysptr):getc(yyin))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
#else
# define lex_input() (((yytchar=yysptr>yysbuf?U(*--yysptr):getc(yyin))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
#endif
#define ECHO fprintf(yyout, "%s",yytext)
# define REJECT { nstr = yyreject(); goto yyfussy;}
int yyleng;
char yytext[YYLMAX];
int yymorfg;
extern char *yysptr, yysbuf[];
int yytchar;
FILE *yyin = {stdin}, *yyout = {stdout};
extern int yylineno;
struct yysvf { 
	struct yywork *yystoff;
	struct yysvf *yyother;
	int *yystops;};
struct yysvf *yyestate;
extern struct yysvf yysvec[], *yybgin;

# line 3 "mybc.l"
/* standard unix headers */
#include <stdio.h>
#include <string.h>


# line 7 "mybc.l"
/* other headers */
#include "mybc.h"
#include "mybcyacc.h"

#ifdef YYDEBUG
#define YYDUMP() fprintf(stderr, "token is <%s>.\n", yytext)
#else
#define YYDUMP()
#endif

#define GETTOKEN(tokenId) { \
	YYDUMP(); \
	strcpy(yylval.string, yytext); \
	return(tokenId); \
}

# define YYNEWLINE 10
yylex(){
int nstr; extern int yyprevious;
#ifdef __cplusplus
/* to avoid CC and lint complaining yyfussy not being used ...*/
static int __lex_hack = 0;
if (__lex_hack) goto yyfussy;
#endif
while((nstr = yylook()) >= 0)
yyfussy: switch(nstr){
case 0:
if(yywrap()) return(0); break;
case 1:

# line 33 "mybc.l"
;
break;
case 2:

# line 34 "mybc.l"
{
		YYDUMP();
		yylval.number.value.lvalue = strtol(yytext, (char **)0, 10);
		yylval.number.type = LONG;
		return(LCONST);
		}
break;
case 3:

# line 40 "mybc.l"
{
		YYDUMP();
		yylval.number.value.ulvalue = strtol(yytext+1, (char **)0, 16);
		yylval.number.type = ULONG;
		return(ULCONST);
		}
break;
case 4:

# line 46 "mybc.l"
	{
		YYDUMP();
		yylval.number.value.ulvalue = strtol(yytext+1, (char **)0, 8);
		yylval.number.type = ULONG;
		return(ULCONST);
		}
break;
case 5:

# line 52 "mybc.l"
{
		YYDUMP();
		yylval.number.value.ulvalue = strtol(yytext+1, (char **)0, 2);
		yylval.number.type = ULONG;
		return(ULCONST);
		}
break;
case 6:

# line 58 "mybc.l"
	{
		YYDUMP();
#ifdef SUN
		sscanf(yytext, "%le", &yylval.number.value.dvalue);
#else
		sscanf(yytext, "%e", &yylval.number.value.dvalue);
#endif
		yylval.number.type = DOUBLE;
		return(DCONST);
		}
break;
case 7:

# line 68 "mybc.l"
	GETTOKEN(OR);
break;
case 8:

# line 69 "mybc.l"
	GETTOKEN(AND);
break;
case 9:

# line 70 "mybc.l"
	GETTOKEN(BITOR);
break;
case 10:

# line 71 "mybc.l"
	GETTOKEN(BITXOR);
break;
case 11:

# line 72 "mybc.l"
	GETTOKEN(BITAND);
break;
case 12:

# line 73 "mybc.l"
	GETTOKEN(EQUAL);
break;
case 13:

# line 74 "mybc.l"
	GETTOKEN(NOTEQUAL);
break;
case 14:

# line 75 "mybc.l"
	GETTOKEN(LT);
break;
case 15:

# line 76 "mybc.l"
	GETTOKEN(GT);
break;
case 16:

# line 77 "mybc.l"
	GETTOKEN(LE);
break;
case 17:

# line 78 "mybc.l"
	GETTOKEN(GE);
break;
case 18:

# line 79 "mybc.l"
	GETTOKEN(LSHIFT);
break;
case 19:

# line 80 "mybc.l"
	GETTOKEN(RSHIFT);
break;
case 20:

# line 81 "mybc.l"
	GETTOKEN(PLUS);
break;
case 21:

# line 82 "mybc.l"
	GETTOKEN(MINUS);
break;
case 22:

# line 83 "mybc.l"
	GETTOKEN(STAR);
break;
case 23:

# line 84 "mybc.l"
	GETTOKEN(SLASH);
break;
case 24:

# line 85 "mybc.l"
	GETTOKEN(PERCENT);
break;
case 25:

# line 86 "mybc.l"
	GETTOKEN(LPAREN);
break;
case 26:

# line 87 "mybc.l"
	GETTOKEN(RPAREN);
break;
case 27:

# line 88 "mybc.l"
	GETTOKEN(TILDE);
break;
case 28:

# line 89 "mybc.l"
	GETTOKEN(NOT);
break;
case 29:

# line 90 "mybc.l"
	GETTOKEN(COMMA);
break;
case 30:

# line 91 "mybc.l"
	GETTOKEN(HELP);
break;
case 31:

# line 92 "mybc.l"
{
		YYDUMP();
		if (strcmp(yytext, "l") == 0)
		{
			yylval.type = LONG;
			return(LCAST);
		}
		else if (strcmp(yytext, "ul") == 0)
		{
			yylval.type = ULONG;
			return(ULCAST);
		}
		else if (strcmp(yytext, "d") == 0)
		{
			yylval.type = DOUBLE;
			return(DCAST);
		}
		else if ((strcmp(yytext, "q") == 0) ||
			 (strcmp(yytext, "quit") == 0))
		{
			GETTOKEN(QUIT);
		}
		else if (strcmp(yytext, "ob") == 0)
		{
			GETTOKEN(OUTBASE);
		}
		else if (strcmp(yytext, "exp") == 0)
		{
			GETTOKEN(EXP);
		}
		else if (strcmp(yytext, "log10") == 0)
		{
			GETTOKEN(LOG10);
		}
		else if (strcmp(yytext, "log") == 0)
		{
			GETTOKEN(LOG);
		}
		else if (strcmp(yytext, "pow") == 0)
		{
			GETTOKEN(POW);
		}
		else if (strcmp(yytext, "sqrt") == 0)
		{
			GETTOKEN(SQRT);
		}
		else if (strcmp(yytext, "sin") == 0)
		{
			GETTOKEN(SIN);
		}
		else if (strcmp(yytext, "cos") == 0)
		{
			GETTOKEN(COS);
		}
		else if (strcmp(yytext, "tan") == 0)
		{
			GETTOKEN(TAN);
		}
		else if (strcmp(yytext, "asin") == 0)
		{
			GETTOKEN(ASIN);
		}
		else if (strcmp(yytext, "acos") == 0)
		{
			GETTOKEN(ACOS);
		}
		else if (strcmp(yytext, "atan") == 0)
		{
			GETTOKEN(ATAN);
		}
		else if (strcmp(yytext, "help") == 0)
		{
			GETTOKEN(HELP);
		}
		GETTOKEN(STRING);
		}
break;
case 32:

# line 168 "mybc.l"
GETTOKEN(NEWLINE);
break;
case 33:

# line 169 "mybc.l"
	{
		YYDUMP();
		strcpy(yylval.string, yytext);
		return(*yytext);
		}
break;
case -1:
break;
default:
(void)fprintf(yyout,"bad switch yylook %d",nstr);
} return(0); }
/* end of yylex */

/*
main()
{
	while (yylex() != 0);
}
 */
int yyvstop[] = {
0,

33,
0, 

1,
33,
0, 

32,
0, 

28,
33,
0, 

24,
33,
0, 

11,
33,
0, 

25,
33,
0, 

26,
33,
0, 

22,
33,
0, 

20,
33,
0, 

29,
33,
0, 

21,
33,
0, 

23,
33,
0, 

2,
33,
0, 

14,
33,
0, 

33,
0, 

15,
33,
0, 

30,
33,
0, 

31,
33,
0, 

31,
33,
0, 

31,
33,
0, 

31,
33,
0, 

10,
33,
0, 

9,
33,
0, 

27,
33,
0, 

1,
0, 

13,
0, 

8,
0, 

2,
0, 

18,
0, 

16,
0, 

12,
0, 

17,
0, 

19,
0, 

31,
0, 

5,
31,
0, 

3,
31,
0, 

4,
31,
0, 

7,
0, 

6,
0, 

6,
0, 
0};
# define YYTYPE unsigned char
struct yywork { YYTYPE verify, advance; } yycrank[] = {
0,0,	0,0,	1,3,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,4,	1,5,	
4,28,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,6,	4,28,	
0,0,	0,0,	1,7,	1,8,	
8,30,	1,9,	1,10,	1,11,	
1,12,	1,13,	1,14,	0,0,	
1,15,	1,16,	0,0,	1,16,	
22,39,	22,39,	39,39,	39,39,	
0,0,	1,16,	0,0,	0,0,	
0,0,	1,17,	1,18,	1,19,	
1,20,	6,29,	1,21,	1,22,	
17,33,	17,34,	1,21,	18,35,	
1,21,	1,23,	2,6,	19,36,	
19,37,	0,0,	2,7,	2,8,	
1,24,	2,9,	2,10,	2,11,	
0,0,	2,13,	2,14,	0,0,	
2,15,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	1,25,	
43,44,	0,0,	0,0,	0,0,	
0,0,	2,17,	2,18,	2,19,	
2,20,	16,31,	0,0,	16,32,	
16,32,	16,32,	16,32,	16,32,	
16,32,	16,32,	16,32,	16,32,	
16,32,	24,41,	24,41,	24,41,	
24,41,	24,41,	24,41,	24,41,	
24,41,	1,26,	26,42,	1,27,	
43,44,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	2,25,	
31,43,	31,43,	31,43,	31,43,	
31,43,	31,43,	31,43,	31,43,	
31,43,	31,43,	0,0,	0,0,	
0,0,	0,0,	0,0,	21,38,	
21,38,	21,38,	21,38,	21,38,	
21,38,	21,38,	21,38,	21,38,	
21,38,	0,0,	0,0,	0,0,	
0,0,	2,26,	0,0,	2,27,	
21,38,	21,38,	21,38,	21,38,	
21,38,	21,38,	21,38,	21,38,	
21,38,	21,38,	21,38,	21,38,	
21,38,	21,38,	21,38,	21,38,	
21,38,	21,38,	21,38,	21,38,	
21,38,	21,38,	21,38,	21,38,	
21,38,	21,38,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
21,38,	21,38,	21,38,	21,38,	
21,38,	21,38,	21,38,	21,38,	
21,38,	21,38,	21,38,	21,38,	
21,38,	21,38,	21,38,	21,38,	
21,38,	21,38,	21,38,	21,38,	
21,38,	21,38,	21,38,	21,38,	
21,38,	21,38,	23,40,	23,40,	
23,40,	23,40,	23,40,	23,40,	
23,40,	23,40,	23,40,	23,40,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	23,40,	
23,40,	23,40,	23,40,	23,40,	
23,40,	40,40,	40,40,	40,40,	
40,40,	40,40,	40,40,	40,40,	
40,40,	40,40,	40,40,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	40,40,	40,40,	
40,40,	40,40,	40,40,	40,40,	
0,0,	0,0,	0,0,	23,40,	
23,40,	23,40,	23,40,	23,40,	
23,40,	41,41,	41,41,	41,41,	
41,41,	41,41,	41,41,	41,41,	
41,41,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	40,40,	40,40,	
40,40,	40,40,	40,40,	40,40,	
44,45,	0,0,	44,45,	0,0,	
0,0,	44,46,	44,46,	44,46,	
44,46,	44,46,	44,46,	44,46,	
44,46,	44,46,	44,46,	45,46,	
45,46,	45,46,	45,46,	45,46,	
45,46,	45,46,	45,46,	45,46,	
45,46,	0,0,	0,0,	0,0,	
0,0};
struct yysvf yysvec[] = {
0,	0,	0,
yycrank+-1,	0,		0,	
yycrank+-41,	yysvec+1,	0,	
yycrank+0,	0,		yyvstop+1,
yycrank+3,	0,		yyvstop+3,
yycrank+0,	0,		yyvstop+6,
yycrank+4,	0,		yyvstop+8,
yycrank+0,	0,		yyvstop+11,
yycrank+2,	0,		yyvstop+14,
yycrank+0,	0,		yyvstop+17,
yycrank+0,	0,		yyvstop+20,
yycrank+0,	0,		yyvstop+23,
yycrank+0,	0,		yyvstop+26,
yycrank+0,	0,		yyvstop+29,
yycrank+0,	0,		yyvstop+32,
yycrank+0,	0,		yyvstop+35,
yycrank+59,	0,		yyvstop+38,
yycrank+8,	0,		yyvstop+41,
yycrank+10,	0,		yyvstop+44,
yycrank+14,	0,		yyvstop+46,
yycrank+0,	0,		yyvstop+49,
yycrank+103,	0,		yyvstop+52,
yycrank+4,	yysvec+21,	yyvstop+55,
yycrank+178,	yysvec+21,	yyvstop+58,
yycrank+69,	yysvec+21,	yyvstop+61,
yycrank+0,	0,		yyvstop+64,
yycrank+2,	0,		yyvstop+67,
yycrank+0,	0,		yyvstop+70,
yycrank+0,	yysvec+4,	yyvstop+73,
yycrank+0,	0,		yyvstop+75,
yycrank+0,	0,		yyvstop+77,
yycrank+88,	0,		0,	
yycrank+0,	yysvec+16,	yyvstop+79,
yycrank+0,	0,		yyvstop+81,
yycrank+0,	0,		yyvstop+83,
yycrank+0,	0,		yyvstop+85,
yycrank+0,	0,		yyvstop+87,
yycrank+0,	0,		yyvstop+89,
yycrank+0,	yysvec+21,	yyvstop+91,
yycrank+6,	yysvec+21,	yyvstop+93,
yycrank+201,	yysvec+21,	yyvstop+96,
yycrank+233,	yysvec+21,	yyvstop+99,
yycrank+0,	0,		yyvstop+102,
yycrank+27,	yysvec+31,	yyvstop+104,
yycrank+261,	0,		0,	
yycrank+271,	0,		0,	
yycrank+0,	yysvec+45,	yyvstop+106,
0,	0,	0};
struct yywork *yytop = yycrank+328;
struct yysvf *yybgin = yysvec+1;
char yymatch[] = {
  0,   1,   1,   1,   1,   1,   1,   1, 
  1,   9,  10,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  9,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,  43,   1,  43,   1,   1, 
 48,  48,  50,  50,  50,  50,  50,  50, 
 56,  56,   1,   1,   1,   1,   1,   1, 
  1,  65,  66,  65,  65,  69,  65,  71, 
 72,  71,  71,  71,  71,  71,  71,  79, 
 71,  71,  71,  71,  71,  71,  71,  71, 
 72,  71,  71,   1,   1,   1,   1,   1, 
  1,  65,  66,  65,  65,  69,  65,  71, 
 72,  71,  71,  71,  71,  71,  71,  79, 
 71,  71,  71,  71,  71,  71,  71,  71, 
 72,  71,  71,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
0};
char yyextra[] = {
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0};
/*	Copyright (c) 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#pragma ident	"@(#)ncform	6.8	95/02/11 SMI"

int yylineno =1;
# define YYU(x) x
# define NLSTATE yyprevious=YYNEWLINE
struct yysvf *yylstate [YYLMAX], **yylsp, **yyolsp;
char yysbuf[YYLMAX];
char *yysptr = yysbuf;
int *yyfnd;
extern struct yysvf *yyestate;
int yyprevious = YYNEWLINE;
#if defined(__cplusplus) || defined(__STDC__)
int yylook(void)
#else
yylook()
#endif
{
	register struct yysvf *yystate, **lsp;
	register struct yywork *yyt;
	struct yysvf *yyz;
	int yych, yyfirst;
	struct yywork *yyr;
# ifdef LEXDEBUG
	int debug;
# endif
	char *yylastch;
	/* start off machines */
# ifdef LEXDEBUG
	debug = 0;
# endif
	yyfirst=1;
	if (!yymorfg)
		yylastch = yytext;
	else {
		yymorfg=0;
		yylastch = yytext+yyleng;
		}
	for(;;){
		lsp = yylstate;
		yyestate = yystate = yybgin;
		if (yyprevious==YYNEWLINE) yystate++;
		for (;;){
# ifdef LEXDEBUG
			if(debug)fprintf(yyout,"state %d\n",yystate-yysvec-1);
# endif
			yyt = yystate->yystoff;
			if(yyt == yycrank && !yyfirst){  /* may not be any transitions */
				yyz = yystate->yyother;
				if(yyz == 0)break;
				if(yyz->yystoff == yycrank)break;
				}
#ifndef __cplusplus
			*yylastch++ = yych = input();
#else
			*yylastch++ = yych = lex_input();
#endif
			if(yylastch > &yytext[YYLMAX]) {
				fprintf(yyout,"Input string too long, limit %d\n",YYLMAX);
				exit(1);
			}
			yyfirst=0;
		tryagain:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"char ");
				allprint(yych);
				putchar('\n');
				}
# endif
			yyr = yyt;
			if ( (int)yyt > (int)yycrank){
				yyt = yyr + yych;
				if (yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					if(lsp > &yylstate[YYLMAX]) {
						fprintf(yyout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				}
# ifdef YYOPTIM
			else if((int)yyt < (int)yycrank) {		/* r < yycrank */
				yyt = yyr = yycrank+(yycrank-yyt);
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"compressed state\n");
# endif
				yyt = yyt + yych;
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					if(lsp > &yylstate[YYLMAX]) {
						fprintf(yyout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				yyt = yyr + YYU(yymatch[yych]);
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"try fall back character ");
					allprint(YYU(yymatch[yych]));
					putchar('\n');
					}
# endif
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transition */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					if(lsp > &yylstate[YYLMAX]) {
						fprintf(yyout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				}
			if ((yystate = yystate->yyother) && (yyt= yystate->yystoff) != yycrank){
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"fall back to state %d\n",yystate-yysvec-1);
# endif
				goto tryagain;
				}
# endif
			else
				{unput(*--yylastch);break;}
		contin:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"state %d char ",yystate-yysvec-1);
				allprint(yych);
				putchar('\n');
				}
# endif
			;
			}
# ifdef LEXDEBUG
		if(debug){
			fprintf(yyout,"stopped at %d with ",*(lsp-1)-yysvec-1);
			allprint(yych);
			putchar('\n');
			}
# endif
		while (lsp-- > yylstate){
			*yylastch-- = 0;
			if (*lsp != 0 && (yyfnd= (*lsp)->yystops) && *yyfnd > 0){
				yyolsp = lsp;
				if(yyextra[*yyfnd]){		/* must backup */
					while(yyback((*lsp)->yystops,-*yyfnd) != 1 && lsp > yylstate){
						lsp--;
						unput(*yylastch--);
						}
					}
				yyprevious = YYU(*yylastch);
				yylsp = lsp;
				yyleng = yylastch-yytext+1;
				yytext[yyleng] = 0;
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"\nmatch ");
					sprint(yytext);
					fprintf(yyout," action %d\n",*yyfnd);
					}
# endif
				return(*yyfnd++);
				}
			unput(*yylastch);
			}
		if (yytext[0] == 0  /* && feof(yyin) */)
			{
			yysptr=yysbuf;
			return(0);
			}
#ifndef __cplusplus
		yyprevious = yytext[0] = input();
		if (yyprevious>0)
			output(yyprevious);
#else
		yyprevious = yytext[0] = lex_input();
		if (yyprevious>0)
			lex_output(yyprevious);
#endif
		yylastch=yytext;
# ifdef LEXDEBUG
		if(debug)putchar('\n');
# endif
		}
	}
#if defined(__cplusplus) || defined(__STDC__)
int yyback(int *p, int m)
#else
yyback(p, m)
	int *p;
#endif
{
	if (p==0) return(0);
	while (*p) {
		if (*p++ == m)
			return(1);
	}
	return(0);
}
	/* the following are only used in the lex library */
#if defined(__cplusplus) || defined(__STDC__)
int yyinput(void)
#else
yyinput()
#endif
{
#ifndef __cplusplus
	return(input());
#else
	return(lex_input());
#endif
	}
#if defined(__cplusplus) || defined(__STDC__)
void yyoutput(int c)
#else
yyoutput(c)
  int c; 
#endif
{
#ifndef __cplusplus
	output(c);
#else
	lex_output(c);
#endif
	}
#if defined(__cplusplus) || defined(__STDC__)
void yyunput(int c)
#else
yyunput(c)
   int c; 
#endif
{
	unput(c);
	}
