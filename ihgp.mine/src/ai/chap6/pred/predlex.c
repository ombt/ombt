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
#include "pred.h"
#include "predyacc.h"

// other macros
#ifdef YYDEBUG
#define YYDUMP() fprintf(stdout, "token is <%s>.\n", yytext)
#else
#define YYDUMP()
#endif

#define GETTOKEN(tokenId) { \
	ECHO; \
	YYDUMP(); \
	strcpy(yylval.cstring, yytext); \
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

# line 37 "pred.l"
ECHO;
break;
case 2:

# line 38 "pred.l"
	ECHO;
break;
case 3:

# line 39 "pred.l"
GETTOKEN(NEWLINE);
break;
case 4:

# line 40 "pred.l"
	GETTOKEN(THEREFORE);
break;
case 5:

# line 41 "pred.l"
	GETTOKEN(BICONDITIONAL);
break;
case 6:

# line 42 "pred.l"
	GETTOKEN(IMPLICATION);
break;
case 7:

# line 43 "pred.l"
	GETTOKEN(OR);
break;
case 8:

# line 44 "pred.l"
	GETTOKEN(AND);
break;
case 9:

# line 45 "pred.l"
	GETTOKEN(NEGATION);
break;
case 10:

# line 46 "pred.l"
	GETTOKEN(LPAREN);
break;
case 11:

# line 47 "pred.l"
	GETTOKEN(RPAREN);
break;
case 12:

# line 48 "pred.l"
	GETTOKEN(COMMA);
break;
case 13:

# line 49 "pred.l"
	GETTOKEN(SEMICOLON);
break;
case 14:

# line 50 "pred.l"
	GETTOKEN(LEFTUNIVERSAL);
break;
case 15:

# line 51 "pred.l"
	GETTOKEN(RIGHTUNIVERSAL);
break;
case 16:

# line 52 "pred.l"
	GETTOKEN(LEFTEXISTENTIAL);
break;
case 17:

# line 53 "pred.l"
	GETTOKEN(RIGHTEXISTENTIAL);
break;
case 18:

# line 54 "pred.l"
	GETTOKEN(QUIT);
break;
case 19:

# line 55 "pred.l"
	GETTOKEN(TRUE);
break;
case 20:

# line 56 "pred.l"
	GETTOKEN(FALSE);
break;
case 21:

# line 57 "pred.l"
GETTOKEN(NUMBER);
break;
case 22:

# line 58 "pred.l"
{
		ECHO;
		YYDUMP();
		yytext[strlen(yytext)-1] = 0;
		strcpy(yylval.cstring, yytext+1);
		return(STRING);
		}
break;
case 23:

# line 65 "pred.l"
GETTOKEN(IDENTIFIER);
break;
case 24:

# line 66 "pred.l"
GETTOKEN(PIDENTIFIER);
break;
case 25:

# line 67 "pred.l"
	{
		ECHO;
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

25,
0,

1,
25,
0,

3,
0,

25,
0,

25,
0,

25,
0,

10,
25,
0,

11,
25,
0,

12,
25,
0,

25,
0,

21,
25,
0,

13,
25,
0,

14,
25,
0,

15,
25,
0,

24,
25,
0,

24,
25,
0,

24,
25,
0,

16,
25,
0,

17,
25,
0,

23,
25,
0,

23,
25,
0,

25,
0,

9,
25,
0,

1,
0,

22,
0,

2,
0,

8,
0,

21,
0,

24,
0,

24,
0,

24,
0,

23,
0,

23,
0,

4,
0,

7,
0,

6,
0,

24,
0,

24,
0,

23,
0,

5,
0,

24,
0,

19,
24,
0,

18,
23,
0,

20,
24,
0,
0};
# define YYTYPE unsigned char
struct yywork { YYTYPE verify, advance; } yycrank[] = {
0,0,	0,0,	1,3,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,4,	1,5,	
0,0,	0,0,	4,26,	0,0,	
0,0,	6,27,	0,0,	0,0,	
0,0,	0,0,	7,29,	0,0,	
0,0,	6,27,	6,27,	0,0,	
0,0,	0,0,	7,29,	7,30,	
0,0,	0,0,	0,0,	1,6,	
1,7,	4,26,	0,0,	1,8,	
8,31,	1,9,	1,10,	0,0,	
0,0,	1,11,	1,12,	12,32,	
2,7,	1,13,	6,28,	2,8,	
15,34,	2,9,	2,10,	7,29,	
0,0,	2,11,	2,12,	24,40,	
1,14,	1,15,	34,43,	1,16,	
6,27,	0,0,	1,17,	0,0,	
0,0,	7,29,	0,0,	1,18,	
2,14,	2,15,	0,0,	2,16,	
32,42,	43,47,	0,0,	0,0,	
0,0,	6,27,	0,0,	2,18,	
0,0,	1,19,	7,29,	0,0,	
0,0,	0,0,	0,0,	0,0,	
1,20,	0,0,	1,21,	0,0,	
1,3,	2,19,	1,22,	18,36,	
0,0,	0,0,	0,0,	0,0,	
2,20,	0,0,	2,21,	0,0,	
0,0,	0,0,	0,0,	6,27,	
0,0,	6,27,	1,23,	19,37,	
7,29,	45,49,	7,29,	39,46,	
48,51,	36,44,	0,0,	0,0,	
0,0,	1,24,	2,23,	1,25,	
44,48,	46,50,	23,39,	37,45,	
0,0,	0,0,	0,0,	0,0,	
0,0,	2,24,	24,41,	2,25,	
13,33,	13,33,	13,33,	13,33,	
13,33,	13,33,	13,33,	13,33,	
13,33,	13,33,	17,35,	17,35,	
17,35,	17,35,	17,35,	17,35,	
17,35,	17,35,	17,35,	17,35,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	17,35,	
17,35,	17,35,	17,35,	17,35,	
17,35,	17,35,	17,35,	17,35,	
17,35,	17,35,	17,35,	17,35,	
17,35,	17,35,	17,35,	17,35,	
17,35,	17,35,	17,35,	17,35,	
17,35,	17,35,	17,35,	17,35,	
17,35,	0,0,	0,0,	0,0,	
0,0,	17,35,	0,0,	17,35,	
17,35,	17,35,	17,35,	17,35,	
17,35,	17,35,	17,35,	17,35,	
17,35,	17,35,	17,35,	17,35,	
17,35,	17,35,	17,35,	17,35,	
17,35,	17,35,	17,35,	17,35,	
17,35,	17,35,	17,35,	17,35,	
17,35,	22,38,	22,38,	22,38,	
22,38,	22,38,	22,38,	22,38,	
22,38,	22,38,	22,38,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	22,38,	22,38,	
22,38,	22,38,	22,38,	22,38,	
22,38,	22,38,	22,38,	22,38,	
22,38,	22,38,	22,38,	22,38,	
22,38,	22,38,	22,38,	22,38,	
22,38,	22,38,	22,38,	22,38,	
22,38,	22,38,	22,38,	22,38,	
0,0,	0,0,	0,0,	0,0,	
22,38,	0,0,	22,38,	22,38,	
22,38,	22,38,	22,38,	22,38,	
22,38,	22,38,	22,38,	22,38,	
22,38,	22,38,	22,38,	22,38,	
22,38,	22,38,	22,38,	22,38,	
22,38,	22,38,	22,38,	22,38,	
22,38,	22,38,	22,38,	22,38,	
0,0};
struct yysvf yysvec[] = {
0,	0,	0,
yycrank+-1,	0,		0,	
yycrank+-13,	yysvec+1,	0,	
yycrank+0,	0,		yyvstop+1,
yycrank+5,	0,		yyvstop+3,
yycrank+0,	0,		yyvstop+6,
yycrank+-16,	0,		yyvstop+8,
yycrank+-21,	0,		yyvstop+10,
yycrank+2,	0,		yyvstop+12,
yycrank+0,	0,		yyvstop+14,
yycrank+0,	0,		yyvstop+17,
yycrank+0,	0,		yyvstop+20,
yycrank+2,	0,		yyvstop+23,
yycrank+92,	0,		yyvstop+25,
yycrank+0,	0,		yyvstop+28,
yycrank+7,	0,		yyvstop+31,
yycrank+0,	0,		yyvstop+34,
yycrank+102,	0,		yyvstop+37,
yycrank+2,	yysvec+17,	yyvstop+40,
yycrank+1,	yysvec+17,	yyvstop+43,
yycrank+0,	0,		yyvstop+46,
yycrank+0,	0,		yyvstop+49,
yycrank+177,	0,		yyvstop+52,
yycrank+13,	yysvec+22,	yyvstop+55,
yycrank+14,	0,		yyvstop+58,
yycrank+0,	0,		yyvstop+60,
yycrank+0,	yysvec+4,	yyvstop+63,
yycrank+0,	yysvec+6,	0,	
yycrank+0,	0,		yyvstop+65,
yycrank+0,	yysvec+7,	0,	
yycrank+0,	0,		yyvstop+67,
yycrank+0,	0,		yyvstop+69,
yycrank+14,	0,		0,	
yycrank+0,	yysvec+13,	yyvstop+71,
yycrank+17,	0,		0,	
yycrank+0,	yysvec+17,	yyvstop+73,
yycrank+13,	yysvec+17,	yyvstop+75,
yycrank+14,	yysvec+17,	yyvstop+77,
yycrank+0,	yysvec+22,	yyvstop+79,
yycrank+14,	yysvec+22,	yyvstop+81,
yycrank+0,	0,		yyvstop+83,
yycrank+0,	0,		yyvstop+85,
yycrank+0,	0,		yyvstop+87,
yycrank+15,	0,		0,	
yycrank+13,	yysvec+17,	yyvstop+89,
yycrank+16,	yysvec+17,	yyvstop+91,
yycrank+13,	yysvec+22,	yyvstop+93,
yycrank+0,	0,		yyvstop+95,
yycrank+19,	yysvec+17,	yyvstop+97,
yycrank+0,	yysvec+17,	yyvstop+99,
yycrank+0,	yysvec+22,	yyvstop+102,
yycrank+0,	yysvec+17,	yyvstop+105,
0,	0,	0};
struct yywork *yytop = yycrank+299;
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