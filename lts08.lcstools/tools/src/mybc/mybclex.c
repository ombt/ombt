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
#include <math.h>


# line 8 "mybc.l"
/* other headers */
#include "mybc.h"
#include "mybcyacc.h"


# line 12 "mybc.l"
/* externs */
#if 0
extern "C" {
int yywrap(void);

# line 16 "mybc.l"
/*
int yylex(void);
 */
};
#endif

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

# line 51 "mybc.l"
MYECHO;
break;
case 2:

# line 52 "mybc.l"
	MYECHO;
break;
case 3:

# line 53 "mybc.l"
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

# line 65 "mybc.l"
	{
		MYECHO;
		YYDUMP();
		yylval.number.value.ulvalue = strtol(yytext+1, (char **)0, 8);
		yylval.number.type = ULONG;
		return(ULCONST);
		}
break;
case 5:

# line 72 "mybc.l"
{
		MYECHO;
		YYDUMP();
		yylval.number.value.ulvalue = strtol(yytext+1, (char **)0, 2);
		yylval.number.type = ULONG;
		return(ULCONST);
		}
break;
case 6:

# line 79 "mybc.l"
{
		MYECHO;
		YYDUMP();
		yylval.number.value.lvalue = strtol(yytext, (char **)0, 10);
		yylval.number.type = LONG;
		return(LCONST);
		}
break;
case 7:

# line 86 "mybc.l"
{
		MYECHO;
		YYDUMP();
		yylval.number.value.lvalue = (long)yytext[1];
		yylval.number.type = LONG;
		return(LCONST);
		}
break;
case 8:

# line 93 "mybc.l"
	{
		MYECHO;
		YYDUMP();
		sscanf(yytext, "%le", &yylval.number.value.dvalue);
		yylval.number.type = DOUBLE;
		return(DCONST);
		}
break;
case 9:

# line 100 "mybc.l"
	GETTOKEN(OR);
break;
case 10:

# line 101 "mybc.l"
	GETTOKEN(AND);
break;
case 11:

# line 102 "mybc.l"
	GETTOKEN(BITOR);
break;
case 12:

# line 103 "mybc.l"
	GETTOKEN(BITXOR);
break;
case 13:

# line 104 "mybc.l"
	GETTOKEN(BITAND);
break;
case 14:

# line 105 "mybc.l"
	GETTOKEN(EQUAL);
break;
case 15:

# line 106 "mybc.l"
	GETTOKEN(NOTEQUAL);
break;
case 16:

# line 107 "mybc.l"
	GETTOKEN(LT);
break;
case 17:

# line 108 "mybc.l"
	GETTOKEN(GT);
break;
case 18:

# line 109 "mybc.l"
	GETTOKEN(LE);
break;
case 19:

# line 110 "mybc.l"
	GETTOKEN(GE);
break;
case 20:

# line 111 "mybc.l"
	GETTOKEN(LSHIFT);
break;
case 21:

# line 112 "mybc.l"
	GETTOKEN(RSHIFT);
break;
case 22:

# line 113 "mybc.l"
	GETTOKEN(PLUS);
break;
case 23:

# line 114 "mybc.l"
	GETTOKEN(MINUS);
break;
case 24:

# line 115 "mybc.l"
	GETTOKEN(STAR);
break;
case 25:

# line 116 "mybc.l"
	GETTOKEN(SLASH);
break;
case 26:

# line 117 "mybc.l"
	GETTOKEN(PERCENT);
break;
case 27:

# line 118 "mybc.l"
	GETTOKEN(LPAREN);
break;
case 28:

# line 119 "mybc.l"
	GETTOKEN(RPAREN);
break;
case 29:

# line 120 "mybc.l"
	GETTOKEN(LBRACKET);
break;
case 30:

# line 121 "mybc.l"
	GETTOKEN(RBRACKET);
break;
case 31:

# line 122 "mybc.l"
	GETTOKEN(TILDE);
break;
case 32:

# line 123 "mybc.l"
	GETTOKEN(NOT);
break;
case 33:

# line 124 "mybc.l"
	GETTOKEN(COMMA);
break;
case 34:

# line 125 "mybc.l"
	GETTOKEN(HELP);
break;
case 35:

# line 126 "mybc.l"
	GETTOKEN(HELP);
break;
case 36:

# line 127 "mybc.l"
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
		else if (strcmp(yytext, "c") == 0)
		{
			yylval.type = COMPLEX;
			return(CCAST);
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
		else if (strcmp(yytext, "i") == 0)
		{
			YYDUMP();
			yylval.number.value.cvalue.r = 0.0;
			yylval.number.value.cvalue.i = 1.0;
			yylval.number.type = COMPLEX;
			return(CCONST);
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
		else if (strcmp(yytext, "sinh") == 0)
		{
			GETTOKEN(SINH);
		}
		else if (strcmp(yytext, "cosh") == 0)
		{
			GETTOKEN(COSH);
		}
		else if (strcmp(yytext, "tanh") == 0)
		{
			GETTOKEN(TANH);
		}
		else if (strcmp(yytext, "asinh") == 0)
		{
			GETTOKEN(ASINH);
		}
		else if (strcmp(yytext, "acosh") == 0)
		{
			GETTOKEN(ACOSH);
		}
		else if (strcmp(yytext, "atanh") == 0)
		{
			GETTOKEN(ATANH);
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
		else if (strcmp(yytext, "csc") == 0)
		{
			GETTOKEN(CSC);
		}
		else if (strcmp(yytext, "sec") == 0)
		{
			GETTOKEN(SSEC);
		}
		else if (strcmp(yytext, "cot") == 0)
		{
			GETTOKEN(COT);
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
		else if (strcmp(yytext, "abs") == 0)
		{
			GETTOKEN(ABS);
		}
		else if (strcmp(yytext, "arg") == 0)
		{
			GETTOKEN(ARG);
		}
		else if (strcmp(yytext, "norm") == 0)
		{
			GETTOKEN(NORM);
		}
		else if (strcmp(yytext, "real") == 0)
		{
			GETTOKEN(REAL);
		}
		else if (strcmp(yytext, "imag") == 0)
		{
			GETTOKEN(IMAG);
		}
		else if (strcmp(yytext, "C") == 0)
		{
			GETTOKEN(COMB);
		}
		else if (strcmp(yytext, "P") == 0)
		{
			GETTOKEN(PERM);
		}
		else if (strcmp(yytext, "fact") == 0)
		{
			GETTOKEN(FACT);
		}
		else if (strcmp(yytext, "usury") == 0)
		{
			GETTOKEN(USURY);
		}
		GETTOKEN(STRING);
		}
break;
case 37:

# line 307 "mybc.l"
GETTOKEN(NEWLINE);
break;
case 38:

# line 308 "mybc.l"
GETTOKEN(SEMICOLON);
break;
case 39:

# line 309 "mybc.l"
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

39,
0, 

1,
39,
0, 

37,
0, 

32,
39,
0, 

39,
0, 

26,
39,
0, 

13,
39,
0, 

39,
0, 

27,
39,
0, 

28,
39,
0, 

24,
39,
0, 

22,
39,
0, 

33,
39,
0, 

23,
39,
0, 

25,
39,
0, 

6,
39,
0, 

6,
39,
0, 

38,
39,
0, 

16,
39,
0, 

39,
0, 

17,
39,
0, 

34,
39,
0, 

36,
39,
0, 

36,
39,
0, 

36,
39,
0, 

36,
39,
0, 

29,
39,
0, 

30,
39,
0, 

12,
39,
0, 

11,
39,
0, 

31,
39,
0, 

1,
0, 

15,
0, 

2,
0, 

10,
0, 

35,
0, 

6,
0, 

20,
0, 

18,
0, 

14,
0, 

19,
0, 

21,
0, 

36,
0, 

5,
36,
0, 

3,
36,
0, 

4,
36,
0, 

9,
0, 

7,
0, 

8,
0, 

3,
0, 

8,
0, 
0};
# define YYTYPE unsigned char
struct yywork { YYTYPE verify, advance; } yycrank[] = {
0,0,	0,0,	1,3,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,4,	1,5,	
4,34,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,6,	4,34,	
1,7,	0,0,	1,8,	1,9,	
1,10,	1,11,	1,12,	1,13,	
1,14,	1,15,	1,16,	9,38,	
1,17,	1,18,	1,19,	1,19,	
26,50,	26,50,	39,54,	50,50,	
50,50,	1,19,	0,0,	0,0,	
1,20,	1,21,	1,22,	1,23,	
1,24,	6,35,	1,25,	1,26,	
16,40,	22,46,	1,25,	0,0,	
1,25,	1,27,	21,44,	21,45,	
55,57,	2,6,	0,0,	2,7,	
1,28,	2,8,	2,9,	2,10,	
2,11,	2,12,	2,13,	0,0,	
2,15,	2,16,	0,0,	2,17,	
1,29,	2,19,	1,30,	1,31,	
23,47,	23,48,	0,0,	0,0,	
0,0,	7,36,	0,0,	2,20,	
2,21,	2,22,	2,23,	2,24,	
55,57,	7,36,	7,37,	0,0,	
10,39,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
10,39,	10,0,	0,0,	0,0,	
0,0,	1,32,	0,0,	1,33,	
32,53,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	2,29,	
0,0,	2,30,	2,31,	0,0,	
0,0,	0,0,	0,0,	7,36,	
0,0,	0,0,	0,0,	0,0,	
7,36,	0,0,	7,36,	0,0,	
0,0,	0,0,	10,39,	0,0,	
7,36,	0,0,	0,0,	10,39,	
0,0,	10,39,	0,0,	0,0,	
0,0,	7,36,	7,36,	10,39,	
2,32,	7,36,	2,33,	7,36,	
7,36,	0,0,	0,0,	0,0,	
10,39,	10,39,	0,0,	7,36,	
10,39,	0,0,	10,39,	10,39,	
0,0,	0,0,	0,0,	0,0,	
0,0,	18,41,	10,39,	18,42,	
18,42,	18,42,	18,42,	18,42,	
18,42,	18,42,	18,42,	18,42,	
18,42,	19,41,	0,0,	19,42,	
19,42,	19,42,	19,42,	19,42,	
19,42,	19,42,	19,42,	19,42,	
19,42,	0,0,	0,0,	18,43,	
28,52,	28,52,	28,52,	28,52,	
28,52,	28,52,	28,52,	28,52,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	18,43,	
41,55,	41,55,	41,55,	41,55,	
41,55,	41,55,	41,55,	41,55,	
41,55,	41,55,	0,0,	0,0,	
0,0,	0,0,	0,0,	18,43,	
25,49,	25,49,	25,49,	25,49,	
25,49,	25,49,	25,49,	25,49,	
25,49,	25,49,	0,0,	0,0,	
0,0,	0,0,	0,0,	18,43,	
0,0,	25,49,	25,49,	25,49,	
25,49,	25,49,	25,49,	25,49,	
25,49,	25,49,	25,49,	25,49,	
25,49,	25,49,	25,49,	25,49,	
25,49,	25,49,	25,49,	25,49,	
25,49,	25,49,	25,49,	25,49,	
25,49,	25,49,	25,49,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	25,49,	25,49,	25,49,	
25,49,	25,49,	25,49,	25,49,	
25,49,	25,49,	25,49,	25,49,	
25,49,	25,49,	25,49,	25,49,	
25,49,	25,49,	25,49,	25,49,	
25,49,	25,49,	25,49,	25,49,	
25,49,	25,49,	25,49,	27,51,	
27,51,	27,51,	27,51,	27,51,	
27,51,	27,51,	27,51,	27,51,	
27,51,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
27,51,	27,51,	27,51,	27,51,	
27,51,	27,51,	43,56,	43,56,	
43,56,	43,56,	43,56,	43,56,	
43,56,	43,56,	43,56,	43,56,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	43,56,	
43,56,	43,56,	43,56,	43,56,	
43,56,	0,0,	0,0,	0,0,	
27,51,	27,51,	27,51,	27,51,	
27,51,	27,51,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
51,51,	51,51,	51,51,	51,51,	
51,51,	51,51,	51,51,	51,51,	
51,51,	51,51,	0,0,	43,56,	
43,56,	43,56,	43,56,	43,56,	
43,56,	51,51,	51,51,	51,51,	
51,51,	51,51,	51,51,	52,52,	
52,52,	52,52,	52,52,	52,52,	
52,52,	52,52,	52,52,	57,58,	
0,0,	57,58,	0,0,	0,0,	
57,59,	57,59,	57,59,	57,59,	
57,59,	57,59,	57,59,	57,59,	
57,59,	57,59,	0,0,	0,0,	
0,0,	51,51,	51,51,	51,51,	
51,51,	51,51,	51,51,	58,59,	
58,59,	58,59,	58,59,	58,59,	
58,59,	58,59,	58,59,	58,59,	
58,59,	0,0,	0,0,	0,0,	
0,0};
struct yysvf yysvec[] = {
0,	0,	0,
yycrank+-1,	0,		0,	
yycrank+-44,	yysvec+1,	0,	
yycrank+0,	0,		yyvstop+1,
yycrank+3,	0,		yyvstop+3,
yycrank+0,	0,		yyvstop+6,
yycrank+4,	0,		yyvstop+8,
yycrank+-100,	0,		yyvstop+11,
yycrank+0,	0,		yyvstop+13,
yycrank+9,	0,		yyvstop+16,
yycrank+-111,	0,		yyvstop+19,
yycrank+0,	0,		yyvstop+21,
yycrank+0,	0,		yyvstop+24,
yycrank+0,	0,		yyvstop+27,
yycrank+0,	0,		yyvstop+30,
yycrank+0,	0,		yyvstop+33,
yycrank+5,	0,		yyvstop+36,
yycrank+0,	0,		yyvstop+39,
yycrank+143,	0,		yyvstop+42,
yycrank+155,	0,		yyvstop+45,
yycrank+0,	0,		yyvstop+48,
yycrank+14,	0,		yyvstop+51,
yycrank+8,	0,		yyvstop+54,
yycrank+35,	0,		yyvstop+56,
yycrank+0,	0,		yyvstop+59,
yycrank+200,	0,		yyvstop+62,
yycrank+4,	yysvec+25,	yyvstop+65,
yycrank+275,	yysvec+25,	yyvstop+68,
yycrank+168,	yysvec+25,	yyvstop+71,
yycrank+0,	0,		yyvstop+74,
yycrank+0,	0,		yyvstop+77,
yycrank+0,	0,		yyvstop+80,
yycrank+4,	0,		yyvstop+83,
yycrank+0,	0,		yyvstop+86,
yycrank+0,	yysvec+4,	yyvstop+89,
yycrank+0,	0,		yyvstop+91,
yycrank+0,	yysvec+7,	0,	
yycrank+0,	0,		yyvstop+93,
yycrank+0,	0,		yyvstop+95,
yycrank+15,	0,		0,	
yycrank+0,	0,		yyvstop+97,
yycrank+184,	0,		0,	
yycrank+0,	yysvec+19,	yyvstop+99,
yycrank+298,	0,		0,	
yycrank+0,	0,		yyvstop+101,
yycrank+0,	0,		yyvstop+103,
yycrank+0,	0,		yyvstop+105,
yycrank+0,	0,		yyvstop+107,
yycrank+0,	0,		yyvstop+109,
yycrank+0,	yysvec+25,	yyvstop+111,
yycrank+7,	yysvec+25,	yyvstop+113,
yycrank+336,	yysvec+25,	yyvstop+116,
yycrank+359,	yysvec+25,	yyvstop+119,
yycrank+0,	0,		yyvstop+122,
yycrank+0,	0,		yyvstop+124,
yycrank+7,	yysvec+41,	yyvstop+126,
yycrank+0,	yysvec+43,	yyvstop+128,
yycrank+372,	0,		0,	
yycrank+391,	0,		0,	
yycrank+0,	yysvec+58,	yyvstop+130,
0,	0,	0};
struct yywork *yytop = yycrank+448;
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
