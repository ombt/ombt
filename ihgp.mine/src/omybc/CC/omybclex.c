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

# line 3 "omybc.l"
/* standard unix headers */
#include <stdio.h>
#include <string.h>
#include <math.h>


# line 8 "omybc.l"
/* other headers */
#include "omybc.h"
#include "omybcyacc.h"


# line 12 "omybc.l"
/* externs */
int yywrap(void);

#ifdef YYDEBUG
#define YYDUMP() fprintf(stderr, "token is <%s>.\n", yytext)
#define MYECHO ECHO
#else
#define YYDUMP()
#define MYECHO
#endif

#define GETTOKEN(tokenId) { \
	MYECHO; \
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

# line 42 "omybc.l"
MYECHO;
break;
case 2:

# line 43 "omybc.l"
	MYECHO;
break;
case 3:

# line 44 "omybc.l"
{
		MYECHO;
		YYDUMP();
		if (yytext[0] != '0')
			yylval.number.value.ulvalue = 
				strtol(yytext+1, (char **)0, 16);
		else
			yylval.number.value.ulvalue = 
				strtol(yytext+2, (char **)0, 16);
		yylval.number.type = ULONG;
		return(ULCONST);
		}
break;
case 4:

# line 56 "omybc.l"
	{
		MYECHO;
		YYDUMP();
		yylval.number.value.ulvalue = strtol(yytext+1, (char **)0, 8);
		yylval.number.type = ULONG;
		return(ULCONST);
		}
break;
case 5:

# line 63 "omybc.l"
{
		MYECHO;
		YYDUMP();
		yylval.number.value.ulvalue = strtol(yytext+1, (char **)0, 2);
		yylval.number.type = ULONG;
		return(ULCONST);
		}
break;
case 6:

# line 70 "omybc.l"
{
		MYECHO;
		YYDUMP();
		yylval.number.value.lvalue = strtol(yytext, (char **)0, 10);
		yylval.number.type = LONG;
		return(LCONST);
		}
break;
case 7:

# line 77 "omybc.l"
	{
		MYECHO;
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
case 8:

# line 88 "omybc.l"
	GETTOKEN(OR);
break;
case 9:

# line 89 "omybc.l"
	GETTOKEN(AND);
break;
case 10:

# line 90 "omybc.l"
	GETTOKEN(BITOR);
break;
case 11:

# line 91 "omybc.l"
	GETTOKEN(BITXOR);
break;
case 12:

# line 92 "omybc.l"
	GETTOKEN(BITAND);
break;
case 13:

# line 93 "omybc.l"
	GETTOKEN(EQUAL);
break;
case 14:

# line 94 "omybc.l"
	GETTOKEN(NOTEQUAL);
break;
case 15:

# line 95 "omybc.l"
	GETTOKEN(LT);
break;
case 16:

# line 96 "omybc.l"
	GETTOKEN(GT);
break;
case 17:

# line 97 "omybc.l"
	GETTOKEN(LE);
break;
case 18:

# line 98 "omybc.l"
	GETTOKEN(GE);
break;
case 19:

# line 99 "omybc.l"
	GETTOKEN(LSHIFT);
break;
case 20:

# line 100 "omybc.l"
	GETTOKEN(RSHIFT);
break;
case 21:

# line 101 "omybc.l"
	GETTOKEN(PLUS);
break;
case 22:

# line 102 "omybc.l"
	GETTOKEN(MINUS);
break;
case 23:

# line 103 "omybc.l"
	GETTOKEN(STAR);
break;
case 24:

# line 104 "omybc.l"
	GETTOKEN(SLASH);
break;
case 25:

# line 105 "omybc.l"
	GETTOKEN(PERCENT);
break;
case 26:

# line 106 "omybc.l"
	GETTOKEN(LPAREN);
break;
case 27:

# line 107 "omybc.l"
	GETTOKEN(RPAREN);
break;
case 28:

# line 108 "omybc.l"
	GETTOKEN(TILDE);
break;
case 29:

# line 109 "omybc.l"
	GETTOKEN(NOT);
break;
case 30:

# line 110 "omybc.l"
	GETTOKEN(COMMA);
break;
case 31:

# line 111 "omybc.l"
	GETTOKEN(HELP);
break;
case 32:

# line 112 "omybc.l"
	GETTOKEN(HELP);
break;
case 33:

# line 113 "omybc.l"
{
		MYECHO;
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
		else if (strcmp(yytext, "e") == 0)
		{
			YYDUMP();
			yylval.number.value.dvalue = M_E;
			yylval.number.type = DOUBLE;
			return(DCONST);
		}
		else if (strcmp(yytext, "pi") == 0)
		{
			YYDUMP();
			yylval.number.value.dvalue = M_PI;
			yylval.number.type = DOUBLE;
			return(DCONST);
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
		else if (strcmp(yytext, "graph") == 0)
		{
			GETTOKEN(GRAPH);
		}
		GETTOKEN(STRING);
		}
break;
case 34:

# line 208 "omybc.l"
GETTOKEN(NEWLINE);
break;
case 35:

# line 209 "omybc.l"
GETTOKEN(SEMICOLON);
break;
case 36:

# line 210 "omybc.l"
	{
		MYECHO;
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

int yywrap(void) { return(1); }

#if 0
main()
{
	while (yylex() != 0);
}
#endif
int yyvstop[] = {
0,

36,
0, 

1,
36,
0, 

34,
0, 

29,
36,
0, 

36,
0, 

25,
36,
0, 

12,
36,
0, 

26,
36,
0, 

27,
36,
0, 

23,
36,
0, 

21,
36,
0, 

30,
36,
0, 

22,
36,
0, 

24,
36,
0, 

6,
36,
0, 

6,
36,
0, 

35,
36,
0, 

15,
36,
0, 

36,
0, 

16,
36,
0, 

31,
36,
0, 

33,
36,
0, 

33,
36,
0, 

33,
36,
0, 

33,
36,
0, 

11,
36,
0, 

10,
36,
0, 

28,
36,
0, 

1,
0, 

14,
0, 

2,
0, 

9,
0, 

32,
0, 

6,
0, 

19,
0, 

17,
0, 

13,
0, 

18,
0, 

20,
0, 

33,
0, 

5,
33,
0, 

3,
33,
0, 

4,
33,
0, 

8,
0, 

7,
0, 

3,
0, 

7,
0, 
0};
# define YYTYPE unsigned char
struct yywork { YYTYPE verify, advance; } yycrank[] = {
0,0,	0,0,	1,3,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,4,	1,5,	
4,31,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,6,	4,31,	
1,7,	0,0,	1,8,	1,9,	
9,35,	1,10,	1,11,	1,12,	
1,13,	1,14,	1,15,	0,0,	
1,16,	1,17,	1,18,	1,18,	
25,46,	25,46,	0,0,	0,0,	
0,0,	1,18,	0,0,	0,0,	
1,19,	1,20,	1,21,	1,22,	
1,23,	6,32,	1,24,	1,25,	
15,36,	21,42,	1,24,	0,0,	
1,24,	1,26,	2,6,	0,0,	
2,7,	0,0,	2,8,	2,9,	
1,27,	2,10,	2,11,	2,12,	
7,33,	2,14,	2,15,	0,0,	
2,16,	50,52,	2,18,	0,0,	
7,33,	7,34,	0,0,	1,28,	
20,40,	20,41,	22,43,	22,44,	
2,19,	2,20,	2,21,	2,22,	
2,23,	46,46,	46,46,	0,0,	
17,37,	0,0,	17,38,	17,38,	
17,38,	17,38,	17,38,	17,38,	
17,38,	17,38,	17,38,	17,38,	
0,0,	50,52,	0,0,	0,0,	
0,0,	1,29,	7,33,	1,30,	
0,0,	0,0,	0,0,	7,33,	
0,0,	7,33,	17,39,	2,28,	
0,0,	29,49,	0,0,	7,33,	
27,48,	27,48,	27,48,	27,48,	
27,48,	27,48,	27,48,	27,48,	
7,33,	7,33,	17,39,	0,0,	
7,33,	0,0,	7,33,	7,33,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	7,33,	0,0,	
0,0,	2,29,	17,39,	2,30,	
18,37,	0,0,	18,38,	18,38,	
18,38,	18,38,	18,38,	18,38,	
18,38,	18,38,	18,38,	18,38,	
0,0,	0,0,	17,39,	24,45,	
24,45,	24,45,	24,45,	24,45,	
24,45,	24,45,	24,45,	24,45,	
24,45,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
24,45,	24,45,	24,45,	24,45,	
24,45,	24,45,	24,45,	24,45,	
24,45,	24,45,	24,45,	24,45,	
24,45,	24,45,	24,45,	24,45,	
24,45,	24,45,	24,45,	24,45,	
24,45,	24,45,	24,45,	24,45,	
24,45,	24,45,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
24,45,	24,45,	24,45,	24,45,	
24,45,	24,45,	24,45,	24,45,	
24,45,	24,45,	24,45,	24,45,	
24,45,	24,45,	24,45,	24,45,	
24,45,	24,45,	24,45,	24,45,	
24,45,	24,45,	24,45,	24,45,	
24,45,	24,45,	26,47,	26,47,	
26,47,	26,47,	26,47,	26,47,	
26,47,	26,47,	26,47,	26,47,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	26,47,	
26,47,	26,47,	26,47,	26,47,	
26,47,	37,50,	37,50,	37,50,	
37,50,	37,50,	37,50,	37,50,	
37,50,	37,50,	37,50,	0,0,	
0,0,	0,0,	0,0,	0,0,	
39,51,	39,51,	39,51,	39,51,	
39,51,	39,51,	39,51,	39,51,	
39,51,	39,51,	0,0,	26,47,	
26,47,	26,47,	26,47,	26,47,	
26,47,	39,51,	39,51,	39,51,	
39,51,	39,51,	39,51,	47,47,	
47,47,	47,47,	47,47,	47,47,	
47,47,	47,47,	47,47,	47,47,	
47,47,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
47,47,	47,47,	47,47,	47,47,	
47,47,	47,47,	0,0,	0,0,	
0,0,	39,51,	39,51,	39,51,	
39,51,	39,51,	39,51,	48,48,	
48,48,	48,48,	48,48,	48,48,	
48,48,	48,48,	48,48,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
47,47,	47,47,	47,47,	47,47,	
47,47,	47,47,	52,53,	0,0,	
52,53,	0,0,	0,0,	52,54,	
52,54,	52,54,	52,54,	52,54,	
52,54,	52,54,	52,54,	52,54,	
52,54,	53,54,	53,54,	53,54,	
53,54,	53,54,	53,54,	53,54,	
53,54,	53,54,	53,54,	0,0,	
0,0};
struct yysvf yysvec[] = {
0,	0,	0,
yycrank+-1,	0,		0,	
yycrank+-41,	yysvec+1,	0,	
yycrank+0,	0,		yyvstop+1,
yycrank+3,	0,		yyvstop+3,
yycrank+0,	0,		yyvstop+6,
yycrank+4,	0,		yyvstop+8,
yycrank+-83,	0,		yyvstop+11,
yycrank+0,	0,		yyvstop+13,
yycrank+2,	0,		yyvstop+16,
yycrank+0,	0,		yyvstop+19,
yycrank+0,	0,		yyvstop+22,
yycrank+0,	0,		yyvstop+25,
yycrank+0,	0,		yyvstop+28,
yycrank+0,	0,		yyvstop+31,
yycrank+5,	0,		yyvstop+34,
yycrank+0,	0,		yyvstop+37,
yycrank+62,	0,		yyvstop+40,
yycrank+122,	0,		yyvstop+43,
yycrank+0,	0,		yyvstop+46,
yycrank+36,	0,		yyvstop+49,
yycrank+8,	0,		yyvstop+52,
yycrank+37,	0,		yyvstop+54,
yycrank+0,	0,		yyvstop+57,
yycrank+135,	0,		yyvstop+60,
yycrank+4,	yysvec+24,	yyvstop+63,
yycrank+210,	yysvec+24,	yyvstop+66,
yycrank+92,	yysvec+24,	yyvstop+69,
yycrank+0,	0,		yyvstop+72,
yycrank+13,	0,		yyvstop+75,
yycrank+0,	0,		yyvstop+78,
yycrank+0,	yysvec+4,	yyvstop+81,
yycrank+0,	0,		yyvstop+83,
yycrank+0,	yysvec+7,	0,	
yycrank+0,	0,		yyvstop+85,
yycrank+0,	0,		yyvstop+87,
yycrank+0,	0,		yyvstop+89,
yycrank+233,	0,		0,	
yycrank+0,	yysvec+18,	yyvstop+91,
yycrank+248,	0,		0,	
yycrank+0,	0,		yyvstop+93,
yycrank+0,	0,		yyvstop+95,
yycrank+0,	0,		yyvstop+97,
yycrank+0,	0,		yyvstop+99,
yycrank+0,	0,		yyvstop+101,
yycrank+0,	yysvec+24,	yyvstop+103,
yycrank+57,	yysvec+24,	yyvstop+105,
yycrank+271,	yysvec+24,	yyvstop+108,
yycrank+303,	yysvec+24,	yyvstop+111,
yycrank+0,	0,		yyvstop+114,
yycrank+20,	yysvec+37,	yyvstop+116,
yycrank+0,	yysvec+39,	yyvstop+118,
yycrank+331,	0,		0,	
yycrank+341,	0,		0,	
yycrank+0,	yysvec+53,	yyvstop+120,
0,	0,	0};
struct yywork *yytop = yycrank+398;
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
