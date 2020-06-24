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

# line 3 "stab.l"
/* standard unix headers */
#include <stdio.h>
#include <string.h>


# line 7 "stab.l"
/* other headers */
#include "stabyacc.h"

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

# line 35 "stab.l"
MYECHO;
break;
case 2:

# line 36 "stab.l"
MYECHO;
break;
case 3:

# line 37 "stab.l"
	{
		MYECHO;
		YYDUMP();
		strcpy(yylval.string, yytext);
		if (strlen(yytext) == 1)
			return(*yytext);
		else
			return(NAME);
		}
break;
case 4:

# line 46 "stab.l"
GETTOKEN(STRING);
break;
case 5:

# line 47 "stab.l"
	GETTOKEN(REAL);
break;
case 6:

# line 48 "stab.l"
GETTOKEN(INTEGER);
break;
case 7:

# line 49 "stab.l"
GETTOKEN(HEXADECIMAL);
break;
case 8:

# line 50 "stab.l"
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

# line 58 "stab.l"

int yywrap(void) { return(1); }

#if 0
main()
{
	while (yylex() != 0);
}
#endif
int yyvstop[] = {
0,

8,
0, 

1,
8,
0, 

2,
0, 

8,
0, 

8,
0, 

8,
0, 

3,
6,
7,
8,
0, 

3,
7,
8,
0, 

3,
8,
0, 

1,
0, 

4,
0, 

4,
0, 

6,
0, 

3,
6,
7,
0, 

3,
7,
0, 

3,
0, 

5,
0, 

5,
0, 
0};
# define YYTYPE unsigned char
struct yywork { YYTYPE verify, advance; } yycrank[] = {
0,0,	0,0,	1,3,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	6,13,	1,4,	1,5,	
0,0,	0,0,	4,12,	0,0,	
0,0,	6,13,	6,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
13,0,	14,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	1,6,	
2,6,	4,12,	0,0,	0,0,	
1,7,	2,7,	6,14,	7,15,	
1,8,	0,0,	0,0,	0,0,	
0,0,	1,9,	7,16,	6,13,	
13,14,	14,14,	0,0,	0,0,	
6,13,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,10,	0,0,	
0,0,	0,0,	1,10,	0,0,	
1,11,	6,13,	0,0,	0,0,	
0,0,	6,13,	0,0,	6,13,	
8,17,	8,17,	8,17,	8,17,	
8,17,	8,17,	8,17,	8,17,	
8,17,	8,17,	18,22,	18,22,	
18,22,	18,22,	18,22,	18,22,	
18,22,	18,22,	18,22,	18,22,	
0,0,	0,0,	1,11,	0,0,	
0,0,	0,0,	0,0,	0,0,	
9,18,	6,13,	9,19,	9,19,	
9,19,	9,19,	9,19,	9,19,	
9,19,	9,19,	9,19,	9,19,	
0,0,	22,23,	0,0,	0,0,	
0,0,	0,0,	0,0,	9,20,	
9,20,	9,20,	9,20,	9,20,	
9,20,	9,21,	9,21,	9,21,	
9,21,	9,21,	9,21,	9,21,	
9,21,	9,21,	9,21,	9,21,	
9,21,	9,21,	9,21,	9,21,	
9,21,	9,21,	9,21,	9,21,	
9,21,	22,23,	0,0,	0,0,	
0,0,	9,21,	0,0,	9,21,	
9,21,	9,21,	9,21,	9,21,	
9,21,	9,21,	9,21,	9,21,	
9,21,	9,21,	9,21,	9,21,	
9,21,	9,21,	9,21,	9,21,	
9,21,	9,21,	9,21,	9,21,	
9,21,	9,21,	9,21,	9,21,	
9,21,	10,20,	10,20,	10,20,	
10,20,	10,20,	10,20,	10,20,	
10,20,	10,20,	10,20,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	10,20,	10,20,	
10,20,	10,20,	10,20,	10,20,	
10,21,	10,21,	10,21,	10,21,	
10,21,	10,21,	10,21,	10,21,	
10,21,	10,21,	10,21,	10,21,	
10,21,	10,21,	10,21,	10,21,	
10,21,	10,21,	10,21,	10,21,	
0,0,	0,0,	0,0,	0,0,	
10,21,	0,0,	10,21,	10,21,	
10,21,	10,21,	10,21,	10,21,	
10,21,	10,21,	10,21,	10,21,	
10,21,	10,21,	10,21,	10,21,	
10,21,	10,21,	10,21,	10,21,	
10,21,	10,21,	10,21,	10,21,	
10,21,	10,21,	10,21,	10,21,	
11,21,	11,21,	11,21,	11,21,	
11,21,	11,21,	11,21,	11,21,	
11,21,	11,21,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	11,21,	11,21,	11,21,	
11,21,	11,21,	11,21,	21,21,	
21,21,	21,21,	21,21,	21,21,	
21,21,	21,21,	21,21,	21,21,	
21,21,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
21,21,	21,21,	21,21,	21,21,	
21,21,	21,21,	23,24,	0,0,	
23,24,	0,0,	0,0,	23,25,	
23,25,	23,25,	23,25,	23,25,	
23,25,	23,25,	23,25,	23,25,	
23,25,	24,25,	24,25,	24,25,	
24,25,	24,25,	24,25,	24,25,	
24,25,	24,25,	24,25,	0,0,	
0,0};
struct yysvf yysvec[] = {
0,	0,	0,
yycrank+-1,	0,		0,	
yycrank+-2,	yysvec+1,	0,	
yycrank+0,	0,		yyvstop+1,
yycrank+5,	0,		yyvstop+3,
yycrank+0,	0,		yyvstop+6,
yycrank+-8,	0,		yyvstop+8,
yycrank+4,	0,		yyvstop+10,
yycrank+32,	0,		yyvstop+12,
yycrank+62,	0,		yyvstop+14,
yycrank+137,	0,		yyvstop+19,
yycrank+212,	yysvec+10,	yyvstop+23,
yycrank+0,	yysvec+4,	yyvstop+26,
yycrank+-18,	yysvec+6,	0,	
yycrank+-19,	yysvec+6,	yyvstop+28,
yycrank+0,	0,		yyvstop+30,
yycrank+0,	yysvec+7,	0,	
yycrank+0,	yysvec+8,	yyvstop+32,
yycrank+42,	0,		0,	
yycrank+0,	yysvec+9,	yyvstop+34,
yycrank+0,	yysvec+10,	yyvstop+38,
yycrank+235,	yysvec+10,	yyvstop+41,
yycrank+52,	yysvec+18,	yyvstop+43,
yycrank+263,	0,		0,	
yycrank+273,	0,		0,	
yycrank+0,	yysvec+24,	yyvstop+45,
0,	0,	0};
struct yywork *yytop = yycrank+330;
struct yysvf *yybgin = yysvec+1;
char yymatch[] = {
  0,   1,   1,   1,   1,   1,   1,   1, 
  1,   9,  10,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  9,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,  43,   1,  43,   1,   1, 
 48,  48,  48,  48,  48,  48,  48,  48, 
 48,  48,   1,   1,   1,   1,   1,   1, 
  1,  65,  65,  65,  65,  69,  65,  71, 
 71,  71,  71,  71,  71,  71,  71,  71, 
 71,  71,  71,  71,  71,  71,  71,  71, 
 71,  71,  71,   1,   1,   1,   1,  71, 
  1,  71,  71,  71,  71, 101,  71,  71, 
 71,  71,  71,  71,  71,  71,  71,  71, 
 71,  71,  71,  71,  71,  71,  71,  71, 
 71,  71,  71,   1,   1,   1,   1,   1, 
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