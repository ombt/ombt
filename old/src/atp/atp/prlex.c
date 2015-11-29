#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
# define U(x) x
# define NLSTATE yyprevious=YYNEWLINE
# define BEGIN yybgin = yysvec + 1 +
# define INITIAL 0
# define YYLERR yysvec
# define YYSTATE (yyestate-yysvec-1)
# define YYOPTIM 1
# ifndef YYLMAX 
# define YYLMAX BUFSIZ
# endif 
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
#define YYISARRAY
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
// standard unix headers
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

// other headers
#include "pr.h"
#include "pryacc.h"

// other macros
#ifdef YYDEBUG
#define YYDUMP() fprintf(stdout, "token is <%s>.\n", yytext)
#else
#define YYDUMP()
#endif

#define IFECHO if (echo) ECHO

#define GETTOKEN(tokenId) { \
	IFECHO; \
	YYDUMP(); \
	strcpy(yylval.cstring, yytext); \
	return(tokenId); \
}

// externs
extern int echo;

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

# line 43 "pr.l"
IFECHO;
break;
case 2:

# line 44 "pr.l"
	IFECHO;
break;
case 3:

# line 45 "pr.l"
IFECHO;
break;
case 4:

# line 46 "pr.l"
	GETTOKEN(THEREFORE);
break;
case 5:

# line 47 "pr.l"
	GETTOKEN(QUERY);
break;
case 6:

# line 48 "pr.l"
	GETTOKEN(BICONDITIONAL);
break;
case 7:

# line 49 "pr.l"
	GETTOKEN(IMPLICATION);
break;
case 8:

# line 50 "pr.l"
	GETTOKEN(OR);
break;
case 9:

# line 51 "pr.l"
	GETTOKEN(AND);
break;
case 10:

# line 52 "pr.l"
	GETTOKEN(NEGATION);
break;
case 11:

# line 53 "pr.l"
	GETTOKEN(LPAREN);
break;
case 12:

# line 54 "pr.l"
	GETTOKEN(RPAREN);
break;
case 13:

# line 55 "pr.l"
	GETTOKEN(COMMA);
break;
case 14:

# line 56 "pr.l"
	GETTOKEN(SEMICOLON);
break;
case 15:

# line 57 "pr.l"
GETTOKEN(UNIVERSAL);
break;
case 16:

# line 58 "pr.l"
	GETTOKEN(LEFTUNIVERSAL);
break;
case 17:

# line 59 "pr.l"
	GETTOKEN(RIGHTUNIVERSAL);
break;
case 18:

# line 60 "pr.l"
GETTOKEN(EXISTENTIAL);
break;
case 19:

# line 61 "pr.l"
	GETTOKEN(LEFTEXISTENTIAL);
break;
case 20:

# line 62 "pr.l"
	GETTOKEN(RIGHTEXISTENTIAL);
break;
case 21:

# line 63 "pr.l"
	GETTOKEN(LBRACE);
break;
case 22:

# line 64 "pr.l"
	GETTOKEN(RBRACE);
break;
case 23:

# line 65 "pr.l"
	GETTOKEN(EQUAL);
break;
case 24:

# line 66 "pr.l"
	GETTOKEN(NOTEQUAL);
break;
case 25:

# line 67 "pr.l"
GETTOKEN(ARGUMENT);
break;
case 26:

# line 68 "pr.l"
	GETTOKEN(QUIT);
break;
case 27:

# line 69 "pr.l"
	GETTOKEN(TRUE);
break;
case 28:

# line 70 "pr.l"
	GETTOKEN(FALSE);
break;
case 29:

# line 71 "pr.l"
	GETTOKEN(SOS);
break;
case 30:

# line 72 "pr.l"
GETTOKEN(OPTION);
break;
case 31:

# line 73 "pr.l"
GETTOKEN(NUMBER);
break;
case 32:

# line 74 "pr.l"
GETTOKEN(NEGNUMBER);
break;
case 33:

# line 75 "pr.l"
{
		IFECHO;
		YYDUMP();
		yytext[strlen(yytext)-1] = 0;
		strcpy(yylval.cstring, yytext+1);
		return(STRING);
		}
break;
case 34:

# line 82 "pr.l"
GETTOKEN(IDENTIFIER);
break;
case 35:

# line 83 "pr.l"
GETTOKEN(PIDENTIFIER);
break;
case 36:

# line 84 "pr.l"
	{
		IFECHO;
		YYDUMP();
		strcpy(yylval.cstring, yytext);
		return(*yytext);
		}
break;
case -1:
break;
default:
(void)fprintf(yyout,"bad switch yylook %d",nstr);
} return(0); }
/* end of yylex */

# line 92 "pr.l"

// useless function
int
yywrap(void)
{
	return(1);
}

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

3,
0, 

36,
0, 

36,
0, 

36,
0, 

36,
0, 

11,
36,
0, 

12,
36,
0, 

13,
36,
0, 

36,
0, 

31,
32,
36,
0, 

14,
36,
0, 

16,
36,
0, 

23,
36,
0, 

17,
36,
0, 

36,
0, 

35,
36,
0, 

35,
36,
0, 

19,
36,
0, 

20,
36,
0, 

34,
36,
0, 

34,
36,
0, 

34,
36,
0, 

34,
36,
0, 

34,
36,
0, 

34,
36,
0, 

21,
36,
0, 

36,
0, 

22,
36,
0, 

10,
36,
0, 

1,
0, 

24,
0, 

33,
0, 

2,
0, 

9,
0, 

31,
32,
0, 

5,
0, 

35,
0, 

35,
0, 

34,
0, 

34,
0, 

34,
0, 

34,
0, 

34,
0, 

34,
0, 

34,
0, 

4,
0, 

8,
0, 

7,
0, 

29,
35,
0, 

34,
0, 

34,
0, 

34,
0, 

34,
0, 

34,
0, 

34,
0, 

6,
0, 

34,
0, 

34,
0, 

34,
0, 

34,
0, 

34,
0, 

26,
34,
0, 

27,
34,
0, 

34,
0, 

28,
34,
0, 

34,
0, 

34,
0, 

34,
0, 

34,
0, 

15,
34,
0, 

34,
0, 

34,
0, 

34,
0, 

18,
34,
0, 

30,
34,
0, 

25,
34,
0, 
0};
# define YYTYPE unsigned char
struct yywork { YYTYPE verify, advance; } yycrank[] = {
0,0,	0,0,	1,3,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,4,	1,5,	
0,0,	0,0,	4,34,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,6,	1,7,	
1,8,	4,34,	0,0,	1,9,	
9,40,	1,10,	1,11,	0,0,	
0,0,	1,12,	1,13,	13,41,	
16,43,	1,14,	14,42,	14,42,	
14,42,	14,42,	14,42,	14,42,	
14,42,	14,42,	14,42,	14,42,	
1,15,	1,16,	1,17,	1,18,	
1,19,	6,35,	1,20,	19,44,	
31,54,	41,56,	43,57,	57,65,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
21,46,	0,0,	0,0,	0,0,	
1,21,	46,58,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
1,22,	0,0,	1,23,	0,0,	
1,3,	0,0,	1,24,	1,25,	
1,25,	1,25,	1,25,	1,26,	
1,25,	1,25,	1,25,	1,25,	
1,25,	1,25,	1,25,	1,25,	
1,27,	1,25,	1,28,	1,25,	
1,25,	1,29,	1,25,	1,25,	
1,25,	1,25,	1,25,	1,25,	
1,30,	1,31,	1,32,	1,33,	
2,6,	25,47,	2,8,	26,49,	
27,51,	2,9,	27,47,	2,10,	
2,11,	29,53,	28,47,	2,12,	
2,13,	28,52,	47,47,	50,61,	
49,60,	26,50,	71,47,	31,55,	
26,47,	48,59,	49,47,	51,47,	
52,63,	51,62,	2,15,	2,16,	
2,17,	2,18,	2,19,	53,47,	
48,47,	52,47,	53,64,	59,47,	
60,47,	60,67,	59,66,	61,68,	
62,70,	63,47,	64,72,	63,71,	
72,47,	74,47,	66,73,	68,75,	
67,74,	62,47,	2,21,	66,47,	
79,47,	68,47,	83,47,	64,47,	
61,47,	61,69,	2,22,	69,76,	
2,23,	67,47,	69,47,	81,47,	
81,84,	2,25,	2,25,	2,25,	
2,25,	2,26,	2,25,	2,25,	
2,25,	2,25,	2,25,	2,25,	
2,25,	2,25,	2,27,	2,25,	
2,28,	2,25,	2,25,	2,29,	
2,25,	2,25,	2,25,	2,25,	
2,25,	2,25,	2,30,	2,31,	
2,32,	2,33,	7,36,	70,77,	
73,78,	8,38,	70,47,	76,80,	
80,83,	75,79,	7,36,	7,36,	
76,47,	8,38,	8,39,	75,47,	
77,81,	73,47,	78,82,	84,47,	
77,47,	80,47,	78,47,	82,47,	
85,47,	82,85,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	7,37,	
0,0,	0,0,	8,38,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	7,36,	0,0,	0,0,	
8,38,	20,45,	20,45,	20,45,	
20,45,	20,45,	20,45,	20,45,	
20,45,	20,45,	20,45,	0,0,	
0,0,	0,0,	7,36,	0,0,	
0,0,	8,38,	20,45,	20,45,	
20,45,	20,45,	20,45,	20,45,	
20,45,	20,45,	20,45,	20,45,	
20,45,	20,45,	20,45,	20,45,	
20,45,	20,45,	20,45,	20,45,	
20,45,	20,45,	20,45,	20,45,	
20,45,	20,45,	20,45,	20,45,	
7,36,	0,0,	7,36,	8,38,	
20,45,	8,38,	20,45,	20,45,	
20,45,	20,45,	20,45,	20,45,	
20,45,	20,45,	20,45,	20,45,	
20,45,	20,45,	20,45,	20,45,	
20,45,	20,45,	20,45,	20,45,	
20,45,	20,45,	20,45,	20,45,	
20,45,	20,45,	20,45,	20,45,	
24,47,	24,47,	24,47,	24,47,	
24,47,	24,47,	24,47,	24,47,	
24,47,	24,47,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	24,47,	24,47,	24,47,	
24,47,	24,47,	24,47,	24,47,	
24,47,	24,47,	24,47,	24,47,	
24,47,	24,47,	24,47,	24,47,	
24,47,	24,47,	24,47,	24,47,	
24,47,	24,47,	24,47,	24,47,	
24,47,	24,47,	24,47,	0,0,	
0,0,	0,0,	0,0,	24,47,	
0,0,	24,47,	24,47,	24,47,	
24,47,	24,47,	24,47,	24,47,	
24,47,	24,47,	24,47,	24,47,	
24,47,	24,47,	24,47,	24,47,	
24,47,	24,47,	24,48,	24,47,	
24,47,	24,47,	24,47,	24,47,	
24,47,	24,47,	24,47,	0,0,	
0,0};
struct yysvf yysvec[] = {
0,	0,	0,
yycrank+-1,	0,		0,	
yycrank+-95,	yysvec+1,	0,	
yycrank+0,	0,		yyvstop+1,
yycrank+5,	0,		yyvstop+3,
yycrank+0,	0,		yyvstop+6,
yycrank+4,	0,		yyvstop+8,
yycrank+-221,	0,		yyvstop+10,
yycrank+-224,	0,		yyvstop+12,
yycrank+2,	0,		yyvstop+14,
yycrank+0,	0,		yyvstop+16,
yycrank+0,	0,		yyvstop+19,
yycrank+0,	0,		yyvstop+22,
yycrank+2,	0,		yyvstop+25,
yycrank+2,	0,		yyvstop+27,
yycrank+0,	0,		yyvstop+31,
yycrank+3,	0,		yyvstop+34,
yycrank+0,	0,		yyvstop+37,
yycrank+0,	0,		yyvstop+40,
yycrank+22,	0,		yyvstop+43,
yycrank+225,	0,		yyvstop+45,
yycrank+1,	yysvec+20,	yyvstop+48,
yycrank+0,	0,		yyvstop+51,
yycrank+0,	0,		yyvstop+54,
yycrank+300,	0,		yyvstop+57,
yycrank+15,	yysvec+24,	yyvstop+60,
yycrank+34,	yysvec+24,	yyvstop+63,
yycrank+20,	yysvec+24,	yyvstop+66,
yycrank+24,	yysvec+24,	yyvstop+69,
yycrank+23,	yysvec+24,	yyvstop+72,
yycrank+0,	0,		yyvstop+75,
yycrank+23,	0,		yyvstop+78,
yycrank+0,	0,		yyvstop+80,
yycrank+0,	0,		yyvstop+83,
yycrank+0,	yysvec+4,	yyvstop+86,
yycrank+0,	0,		yyvstop+88,
yycrank+0,	yysvec+7,	0,	
yycrank+0,	0,		yyvstop+90,
yycrank+0,	yysvec+8,	0,	
yycrank+0,	0,		yyvstop+92,
yycrank+0,	0,		yyvstop+94,
yycrank+7,	0,		0,	
yycrank+0,	yysvec+14,	yyvstop+96,
yycrank+25,	0,		0,	
yycrank+0,	0,		yyvstop+99,
yycrank+0,	yysvec+20,	yyvstop+101,
yycrank+2,	yysvec+20,	yyvstop+103,
yycrank+28,	yysvec+24,	yyvstop+105,
yycrank+46,	yysvec+24,	yyvstop+107,
yycrank+36,	yysvec+24,	yyvstop+109,
yycrank+29,	yysvec+24,	yyvstop+111,
yycrank+37,	yysvec+24,	yyvstop+113,
yycrank+47,	yysvec+24,	yyvstop+115,
yycrank+45,	yysvec+24,	yyvstop+117,
yycrank+0,	0,		yyvstop+119,
yycrank+0,	0,		yyvstop+121,
yycrank+0,	0,		yyvstop+123,
yycrank+9,	0,		0,	
yycrank+0,	yysvec+20,	yyvstop+125,
yycrank+49,	yysvec+24,	yyvstop+128,
yycrank+50,	yysvec+24,	yyvstop+130,
yycrank+70,	yysvec+24,	yyvstop+132,
yycrank+63,	yysvec+24,	yyvstop+134,
yycrank+55,	yysvec+24,	yyvstop+136,
yycrank+69,	yysvec+24,	yyvstop+138,
yycrank+0,	0,		yyvstop+140,
yycrank+65,	yysvec+24,	yyvstop+142,
yycrank+75,	yysvec+24,	yyvstop+144,
yycrank+67,	yysvec+24,	yyvstop+146,
yycrank+76,	yysvec+24,	yyvstop+148,
yycrank+112,	yysvec+24,	yyvstop+150,
yycrank+32,	yysvec+24,	yyvstop+152,
yycrank+58,	yysvec+24,	yyvstop+155,
yycrank+123,	yysvec+24,	yyvstop+158,
yycrank+59,	yysvec+24,	yyvstop+160,
yycrank+121,	yysvec+24,	yyvstop+163,
yycrank+118,	yysvec+24,	yyvstop+165,
yycrank+126,	yysvec+24,	yyvstop+167,
yycrank+128,	yysvec+24,	yyvstop+169,
yycrank+66,	yysvec+24,	yyvstop+171,
yycrank+127,	yysvec+24,	yyvstop+174,
yycrank+77,	yysvec+24,	yyvstop+176,
yycrank+129,	yysvec+24,	yyvstop+178,
yycrank+68,	yysvec+24,	yyvstop+180,
yycrank+125,	yysvec+24,	yyvstop+183,
yycrank+130,	yysvec+24,	yyvstop+186,
0,	0,	0};
struct yywork *yytop = yycrank+422;
struct yysvf *yybgin = yysvec+1;
char yymatch[] = {
  0,   1,   1,   1,   1,   1,   1,   1, 
  1,   9,  10,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  9,   1,  34,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
 48,  48,  48,  48,  48,  48,  48,  48, 
 48,  48,   1,   1,   1,   1,   1,   1, 
  1,  65,  65,  65,  65,  65,  65,  65, 
 65,  65,  65,  65,  65,  65,  65,  65, 
 65,  65,  65,  65,  65,  65,  65,  65, 
 65,  65,  65,   1,   1,   1,   1,  95, 
  1,  97,  97,  97,  97,  97,  97,  97, 
 97,  97,  97,  97,  97,  97,  97,  97, 
 97,  97,  97,  97,  97,  97,  97,  97, 
 97,  97,  97,   1,   1,   1,   1,   1, 
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

#pragma ident	"@(#)ncform	6.12	97/12/08 SMI"

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
#ifdef YYISARRAY
			if(yylastch > &yytext[YYLMAX]) {
				fprintf(yyout,"Input string too long, limit %d\n",YYLMAX);
				exit(1);
			}
#else
			if (yylastch >= &yytext[ yytextsz ]) {
				int	x = yylastch - yytext;

				yytextsz += YYTEXTSZINC;
				if (yytext == yy_tbuf) {
				    yytext = (char *) malloc(yytextsz);
				    memcpy(yytext, yy_tbuf, sizeof (yy_tbuf));
				}
				else
				    yytext = (char *) realloc(yytext, yytextsz);
				if (!yytext) {
				    fprintf(yyout,
					"Cannot realloc yytext\n");
				    exit(1);
				}
				yylastch = yytext + x;
			}
#endif
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
			if ( (uintptr_t)yyt > (uintptr_t)yycrank){
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
			else if((uintptr_t)yyt < (uintptr_t)yycrank) {	/* r < yycrank */
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
