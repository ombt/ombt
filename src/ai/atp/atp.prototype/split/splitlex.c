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
// standard unix headers
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

// other headers
#include "split.h"
#include "splityacc.h"

// other macros
#ifdef YYDEBUG
#define YYDUMP() fprintf(stdout, "token is <%s>.\n", yytext)
#else
#define YYDUMP()
#endif

#define IFECHO if (echoflag) ECHO

#define GETTOKEN(tokenId) { \
	IFECHO; \
	YYDUMP(); \
	strcpy(yylval.cstring, yytext); \
	return(tokenId); \
}

// externs
extern int echoflag;

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

# line 42 "split.l"
IFECHO;
break;
case 2:

# line 43 "split.l"
	IFECHO;
break;
case 3:

# line 44 "split.l"
IFECHO;
break;
case 4:

# line 45 "split.l"
	GETTOKEN(THEREFORE);
break;
case 5:

# line 46 "split.l"
	GETTOKEN(BICONDITIONAL);
break;
case 6:

# line 47 "split.l"
	GETTOKEN(IMPLICATION);
break;
case 7:

# line 48 "split.l"
	GETTOKEN(OR);
break;
case 8:

# line 49 "split.l"
	GETTOKEN(AND);
break;
case 9:

# line 50 "split.l"
	GETTOKEN(NEGATION);
break;
case 10:

# line 51 "split.l"
	GETTOKEN(LPAREN);
break;
case 11:

# line 52 "split.l"
	GETTOKEN(RPAREN);
break;
case 12:

# line 53 "split.l"
	GETTOKEN(COMMA);
break;
case 13:

# line 54 "split.l"
	GETTOKEN(SEMICOLON);
break;
case 14:

# line 55 "split.l"
	GETTOKEN(LEFTUNIVERSAL);
break;
case 15:

# line 56 "split.l"
	GETTOKEN(RIGHTUNIVERSAL);
break;
case 16:

# line 57 "split.l"
	GETTOKEN(LEFTEXISTENTIAL);
break;
case 17:

# line 58 "split.l"
	GETTOKEN(RIGHTEXISTENTIAL);
break;
case 18:

# line 59 "split.l"
	GETTOKEN(LBRACE);
break;
case 19:

# line 60 "split.l"
	GETTOKEN(RBRACE);
break;
case 20:

# line 61 "split.l"
	GETTOKEN(EQUAL);
break;
case 21:

# line 62 "split.l"
	GETTOKEN(NOTEQUAL);
break;
case 22:

# line 63 "split.l"
GETTOKEN(ARGUMENT);
break;
case 23:

# line 64 "split.l"
	GETTOKEN(QUIT);
break;
case 24:

# line 65 "split.l"
	GETTOKEN(TRUE);
break;
case 25:

# line 66 "split.l"
	GETTOKEN(FALSE);
break;
case 26:

# line 67 "split.l"
	GETTOKEN(SOS);
break;
case 27:

# line 68 "split.l"
GETTOKEN(NUMBER);
break;
case 28:

# line 69 "split.l"
{
		IFECHO;
		YYDUMP();
		yytext[strlen(yytext)-1] = 0;
		strcpy(yylval.cstring, yytext+1);
		return(STRING);
		}
break;
case 29:

# line 76 "split.l"
GETTOKEN(IDENTIFIER);
break;
case 30:

# line 77 "split.l"
GETTOKEN(PIDENTIFIER);
break;
case 31:

# line 78 "split.l"
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

31,
0,

1,
31,
0,

3,
0,

31,
0,

31,
0,

31,
0,

31,
0,

10,
31,
0,

11,
31,
0,

12,
31,
0,

31,
0,

27,
31,
0,

13,
31,
0,

14,
31,
0,

20,
31,
0,

15,
31,
0,

30,
31,
0,

30,
31,
0,

30,
31,
0,

30,
31,
0,

16,
31,
0,

17,
31,
0,

29,
31,
0,

29,
31,
0,

29,
31,
0,

18,
31,
0,

31,
0,

19,
31,
0,

9,
31,
0,

1,
0,

21,
0,

28,
0,

2,
0,

8,
0,

27,
0,

30,
0,

30,
0,

30,
0,

30,
0,

29,
0,

29,
0,

29,
0,

4,
0,

7,
0,

6,
0,

30,
0,

26,
30,
0,

30,
0,

29,
0,

29,
0,

5,
0,

30,
0,

24,
30,
0,

29,
0,

23,
29,
0,

25,
30,
0,

29,
0,

29,
0,

29,
0,

22,
29,
0,
0};
# define YYTYPE unsigned char
struct yywork { YYTYPE verify, advance; } yycrank[] = {
0,0,	0,0,	1,3,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,4,	1,5,	
0,0,	0,0,	4,32,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,6,	1,7,	
1,8,	4,32,	0,0,	1,9,	
9,38,	1,10,	1,11,	0,0,	
0,0,	1,12,	1,13,	13,39,	
16,41,	1,14,	14,40,	14,40,	
14,40,	14,40,	14,40,	14,40,	
14,40,	14,40,	14,40,	14,40,	
1,15,	1,16,	1,17,	1,18,	
6,33,	29,49,	1,19,	39,51,	
41,52,	52,58,	0,0,	1,20,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
21,44,	0,0,	0,0,	0,0,	
1,21,	1,22,	44,54,	0,0,	
0,0,	0,0,	0,0,	0,0,	
1,23,	0,0,	1,24,	0,0,	
1,3,	0,0,	1,25,	1,26,	
1,26,	1,26,	1,26,	1,26,	
1,26,	1,26,	1,26,	1,26,	
1,26,	1,26,	1,26,	1,26,	
1,26,	1,26,	1,27,	1,26,	
1,26,	1,26,	1,26,	1,26,	
1,26,	1,26,	1,26,	1,26,	
1,28,	1,29,	1,30,	1,31,	
2,6,	20,43,	2,8,	22,45,	
26,46,	2,9,	27,46,	2,10,	
2,11,	27,48,	43,53,	2,12,	
2,13,	45,55,	46,46,	48,57,	
29,50,	53,59,	55,60,	47,56,	
56,46,	59,63,	62,46,	56,61,	
48,46,	64,65,	2,15,	2,16,	
2,17,	2,18,	47,46,	57,46,	
65,66,	57,62,	61,64,	67,46,	
65,46,	2,20,	64,46,	61,46,	
66,46,	0,0,	66,67,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	2,21,	2,22,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	2,23,	0,0,	
2,24,	0,0,	0,0,	0,0,	
0,0,	2,26,	2,26,	2,26,	
2,26,	2,26,	2,26,	2,26,	
2,26,	2,26,	2,26,	2,26,	
2,26,	2,26,	2,26,	2,26,	
2,27,	2,26,	2,26,	2,26,	
2,26,	2,26,	2,26,	2,26,	
2,26,	2,26,	2,28,	2,29,	
2,30,	2,31,	7,34,	0,0,	
0,0,	8,36,	0,0,	0,0,	
0,0,	0,0,	7,34,	7,34,	
0,0,	8,36,	8,37,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	7,35,	
0,0,	0,0,	8,36,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	7,34,	0,0,	0,0,	
8,36,	19,42,	19,42,	19,42,	
19,42,	19,42,	19,42,	19,42,	
19,42,	19,42,	19,42,	0,0,	
0,0,	0,0,	7,34,	0,0,	
0,0,	8,36,	19,42,	19,42,	
19,42,	19,42,	19,42,	19,42,	
19,42,	19,42,	19,42,	19,42,	
19,42,	19,42,	19,42,	19,42,	
19,42,	19,42,	19,42,	19,42,	
19,42,	19,42,	19,42,	19,42,	
19,42,	19,42,	19,42,	19,42,	
7,34,	0,0,	7,34,	8,36,	
19,42,	8,36,	19,42,	19,42,	
19,42,	19,42,	19,42,	19,42,	
19,42,	19,42,	19,42,	19,42,	
19,42,	19,42,	19,42,	19,42,	
19,42,	19,42,	19,42,	19,42,	
19,42,	19,42,	19,42,	19,42,	
19,42,	19,42,	19,42,	19,42,	
25,46,	25,46,	25,46,	25,46,	
25,46,	25,46,	25,46,	25,46,	
25,46,	25,46,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	25,46,	25,46,	25,46,	
25,46,	25,46,	25,46,	25,46,	
25,46,	25,46,	25,46,	25,46,	
25,46,	25,46,	25,46,	25,46,	
25,46,	25,46,	25,46,	25,46,	
25,46,	25,46,	25,46,	25,46,	
25,46,	25,46,	25,46,	0,0,	
0,0,	0,0,	0,0,	25,46,	
0,0,	25,46,	25,46,	25,46,	
25,46,	25,46,	25,46,	25,46,	
25,46,	25,46,	25,46,	25,46,	
25,46,	25,46,	25,46,	25,46,	
25,46,	25,46,	25,47,	25,46,	
25,46,	25,46,	25,46,	25,46,	
25,46,	25,46,	25,46,	0,0,	
0,0};
struct yysvf yysvec[] = {
0,	0,	0,
yycrank+-1,	0,		0,	
yycrank+-95,	yysvec+1,	0,	
yycrank+0,	0,		yyvstop+1,
yycrank+5,	0,		yyvstop+3,
yycrank+0,	0,		yyvstop+6,
yycrank+3,	0,		yyvstop+8,
yycrank+-221,	0,		yyvstop+10,
yycrank+-224,	0,		yyvstop+12,
yycrank+2,	0,		yyvstop+14,
yycrank+0,	0,		yyvstop+16,
yycrank+0,	0,		yyvstop+19,
yycrank+0,	0,		yyvstop+22,
yycrank+2,	0,		yyvstop+25,
yycrank+2,	0,		yyvstop+27,
yycrank+0,	0,		yyvstop+30,
yycrank+3,	0,		yyvstop+33,
yycrank+0,	0,		yyvstop+36,
yycrank+0,	0,		yyvstop+39,
yycrank+225,	0,		yyvstop+42,
yycrank+32,	yysvec+19,	yyvstop+45,
yycrank+1,	yysvec+19,	yyvstop+48,
yycrank+17,	yysvec+19,	yyvstop+51,
yycrank+0,	0,		yyvstop+54,
yycrank+0,	0,		yyvstop+57,
yycrank+300,	0,		yyvstop+60,
yycrank+18,	yysvec+25,	yyvstop+63,
yycrank+20,	yysvec+25,	yyvstop+66,
yycrank+0,	0,		yyvstop+69,
yycrank+20,	0,		yyvstop+72,
yycrank+0,	0,		yyvstop+74,
yycrank+0,	0,		yyvstop+77,
yycrank+0,	yysvec+4,	yyvstop+80,
yycrank+0,	0,		yyvstop+82,
yycrank+0,	yysvec+7,	0,	
yycrank+0,	0,		yyvstop+84,
yycrank+0,	yysvec+8,	0,	
yycrank+0,	0,		yyvstop+86,
yycrank+0,	0,		yyvstop+88,
yycrank+5,	0,		0,	
yycrank+0,	yysvec+14,	yyvstop+90,
yycrank+23,	0,		0,	
yycrank+0,	yysvec+19,	yyvstop+92,
yycrank+30,	yysvec+19,	yyvstop+94,
yycrank+3,	yysvec+19,	yyvstop+96,
yycrank+24,	yysvec+19,	yyvstop+98,
yycrank+28,	yysvec+25,	yyvstop+100,
yycrank+44,	yysvec+25,	yyvstop+102,
yycrank+38,	yysvec+25,	yyvstop+104,
yycrank+0,	0,		yyvstop+106,
yycrank+0,	0,		yyvstop+108,
yycrank+0,	0,		yyvstop+110,
yycrank+7,	0,		0,	
yycrank+30,	yysvec+19,	yyvstop+112,
yycrank+0,	yysvec+19,	yyvstop+114,
yycrank+45,	yysvec+19,	yyvstop+117,
yycrank+34,	yysvec+25,	yyvstop+119,
yycrank+45,	yysvec+25,	yyvstop+121,
yycrank+0,	0,		yyvstop+123,
yycrank+48,	yysvec+19,	yyvstop+125,
yycrank+0,	yysvec+19,	yyvstop+127,
yycrank+53,	yysvec+25,	yyvstop+130,
yycrank+36,	yysvec+25,	yyvstop+132,
yycrank+0,	yysvec+19,	yyvstop+135,
yycrank+52,	yysvec+25,	yyvstop+138,
yycrank+50,	yysvec+25,	yyvstop+140,
yycrank+54,	yysvec+25,	yyvstop+142,
yycrank+49,	yysvec+25,	yyvstop+144,
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
