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
// lexical analyzer for reading oty files

// headers
#include "otyscan.h"
#include "otyscanyacc.h"

// definitions
#ifdef YYDEBUG
#define YYDUMP() fprintf(stdout, "token is <%s>.\n", yytext)
#else
#define YYDUMP()
#endif
#undef input
#define input() (((yytchar=yysptr>yysbuf?U(*--yysptr):MYgetc(yyin))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)

#define GETTOKEN(tokenId) { \
	YYDUMP(); \
	strcpy(yylval.cvalue, yytext); \
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

# line 46 "otyscan.l"
;
break;
case 2:

# line 47 "otyscan.l"
GETTOKEN(NEWLINE);
break;
case 3:

# line 48 "otyscan.l"
	GETTOKEN(MSGID);
break;
case 4:

# line 49 "otyscan.l"
	GETTOKEN(TIME);
break;
case 5:

# line 50 "otyscan.l"
	GETTOKEN(CLASS);
break;
case 6:

# line 51 "otyscan.l"
GETTOKEN(ACTION);
break;
case 7:

# line 52 "otyscan.l"
GETTOKEN(OUTPRIOR);
break;
case 8:

# line 53 "otyscan.l"
	GETTOKEN(FREQ);
break;
case 9:

# line 54 "otyscan.l"
GETTOKEN(PROTOTYPE);
break;
case 10:

# line 55 "otyscan.l"
	GETTOKEN(FIELD);
break;
case 11:

# line 56 "otyscan.l"
GETTOKEN(FIELDUP);
break;
case 12:

# line 57 "otyscan.l"
	GETTOKEN(NPVAL);
break;
case 13:

# line 58 "otyscan.l"
	GETTOKEN(WIDTH);
break;
case 14:

# line 59 "otyscan.l"
	GETTOKEN(ENUM);
break;
case 15:

# line 60 "otyscan.l"
GETTOKEN(OUTPUT);
break;
case 16:

# line 61 "otyscan.l"
{
		YYDUMP();
		VERBOSE(stdout, "otyscan: %s", yytext+strlen("otypp: "));
		}
break;
case 17:

# line 65 "otyscan.l"
GETTOKEN(LEFTBRACE);
break;
case 18:

# line 66 "otyscan.l"
GETTOKEN(RIGHTBRACE);
break;
case -1:
break;
default:
(void)fprintf(yyout,"bad switch yylook %d",nstr);
} return(0); }
/* end of yylex */

// wrap up function, does nothing.
int
yywrap()
{
	return(1);
}

// remove garbage chars from input
int
MYgetc(FILE *fd)
{
	static int nextbyte = 0;
	static int maxbyte = -1;
	static char buf[10*BUFSIZ];
	register unsigned int c;

	// don`t pass on nulls, this kills lex
	do {
		// check if time to read next buffer
		if (nextbyte >= maxbyte)
		{
			if ((maxbyte = read(fileno(fd), buf, 10*BUFSIZ)) <= 0)
			{
				return(0);
			}
			nextbyte = 0;
		}
		c = buf[nextbyte++];
	} while ((c & 0xff80) != 0 || c == 0);
	return(c);
}

#if 0
main()
{
	int tokenid;
	while ((tokenid = yylex()))
	{
		fprintf(stderr, "token ... %s, %d\n", yytext, tokenid);
	}
	exit(0);
}
#endif
int yyvstop[] = {
0,

1,
0,

2,
0,

17,
0,

18,
0,

14,
0,

8,
0,

4,
0,

5,
0,

10,
0,

10,
0,

3,
0,

12,
0,

15,
0,

13,
0,

6,
0,

10,
0,

10,
11,
0,

7,
0,

9,
0,

16,
0,
0};
# define YYTYPE unsigned char
struct yywork { YYTYPE verify, advance; } yycrank[] = {
0,0,	0,0,	0,0,	0,0,	
0,0,	43,55,	0,0,	45,57,	
0,0,	51,64,	1,3,	1,4,	
3,3,	43,55,	43,0,	45,57,	
45,0,	51,64,	51,0,	54,67,	
55,0,	57,0,	58,70,	64,0,	
67,0,	70,0,	71,0,	54,67,	
54,0,	59,71,	58,70,	58,0,	
76,0,	1,3,	65,76,	3,3,	
66,77,	59,71,	59,0,	68,68,	
72,79,	56,68,	65,76,	65,0,	
66,77,	66,0,	77,0,	68,68,	
68,0,	56,68,	56,0,	82,0,	
87,0,	78,82,	90,0,	84,87,	
97,98,	88,90,	103,104,	60,72,	
69,0,	78,82,	78,0,	84,87,	
84,0,	88,90,	88,0,	102,103,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	102,103,	
102,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,5,	18,30,	
1,6,	5,17,	1,7,	1,8,	
20,32,	21,33,	8,20,	13,27,	
14,28,	6,18,	1,9,	1,10,	
1,11,	1,12,	7,19,	8,21,	
9,22,	1,13,	10,23,	12,26,	
1,14,	11,24,	11,25,	17,29,	
1,15,	19,31,	1,16,	22,34,	
23,35,	24,36,	25,37,	26,38,	
27,39,	28,40,	29,41,	30,42,	
31,43,	32,44,	33,45,	34,46,	
35,47,	36,48,	37,49,	38,50,	
39,51,	40,52,	41,53,	42,54,	
44,56,	46,58,	47,59,	48,60,	
49,61,	50,63,	52,65,	49,62,	
53,66,	56,69,	61,73,	62,74,	
63,75,	73,80,	69,78,	75,81,	
79,83,	80,84,	81,85,	83,86,	
85,88,	86,89,	89,91,	91,92,	
92,93,	93,94,	94,95,	95,96,	
96,97,	98,99,	99,100,	100,101,	
101,102,	0,0,	0,0,	0,0,	
0,0};
struct yysvf yysvec[] = {
0,	0,	0,
yycrank+1,	0,		0,	
yycrank+0,	yysvec+1,	0,	
yycrank+3,	0,		yyvstop+1,
yycrank+0,	0,		yyvstop+3,
yycrank+2,	0,		0,	
yycrank+1,	0,		0,	
yycrank+4,	0,		0,	
yycrank+1,	0,		0,	
yycrank+1,	0,		0,	
yycrank+6,	0,		0,	
yycrank+5,	0,		0,	
yycrank+5,	0,		0,	
yycrank+2,	0,		0,	
yycrank+3,	0,		0,	
yycrank+0,	0,		yyvstop+5,
yycrank+0,	0,		yyvstop+7,
yycrank+7,	0,		0,	
yycrank+2,	0,		0,	
yycrank+8,	0,		0,	
yycrank+3,	0,		0,	
yycrank+4,	0,		0,	
yycrank+24,	0,		0,	
yycrank+10,	0,		0,	
yycrank+8,	0,		0,	
yycrank+14,	0,		0,	
yycrank+20,	0,		0,	
yycrank+23,	0,		0,	
yycrank+33,	0,		0,	
yycrank+29,	0,		0,	
yycrank+20,	0,		0,	
yycrank+27,	0,		0,	
yycrank+29,	0,		0,	
yycrank+25,	0,		0,	
yycrank+34,	0,		0,	
yycrank+43,	0,		0,	
yycrank+29,	0,		0,	
yycrank+30,	0,		0,	
yycrank+27,	0,		0,	
yycrank+43,	0,		0,	
yycrank+29,	0,		0,	
yycrank+35,	0,		0,	
yycrank+32,	0,		0,	
yycrank+-4,	0,		0,	
yycrank+48,	0,		0,	
yycrank+-6,	0,		0,	
yycrank+49,	0,		0,	
yycrank+42,	0,		0,	
yycrank+39,	0,		0,	
yycrank+38,	0,		0,	
yycrank+42,	0,		0,	
yycrank+-8,	0,		0,	
yycrank+50,	0,		0,	
yycrank+46,	0,		0,	
yycrank+-18,	0,		0,	
yycrank+-10,	yysvec+43,	yyvstop+9,
yycrank+-40,	0,		0,	
yycrank+-11,	yysvec+45,	yyvstop+11,
yycrank+-21,	0,		0,	
yycrank+-28,	0,		0,	
yycrank+1,	0,		0,	
yycrank+53,	0,		0,	
yycrank+43,	0,		0,	
yycrank+44,	0,		0,	
yycrank+-13,	yysvec+51,	yyvstop+13,
yycrank+-33,	0,		0,	
yycrank+-35,	0,		0,	
yycrank+-14,	yysvec+54,	yyvstop+15,
yycrank+-38,	0,		yyvstop+17,
yycrank+-50,	yysvec+68,	yyvstop+19,
yycrank+-15,	yysvec+58,	yyvstop+21,
yycrank+-16,	yysvec+59,	yyvstop+23,
yycrank+8,	0,		0,	
yycrank+50,	0,		0,	
yycrank+0,	0,		yyvstop+25,
yycrank+42,	0,		0,	
yycrank+-22,	yysvec+65,	yyvstop+27,
yycrank+-36,	yysvec+66,	yyvstop+29,
yycrank+-52,	0,		yyvstop+31,
yycrank+52,	0,		0,	
yycrank+51,	0,		0,	
yycrank+54,	0,		0,	
yycrank+-41,	yysvec+78,	yyvstop+33,
yycrank+53,	0,		0,	
yycrank+-54,	0,		0,	
yycrank+67,	0,		0,	
yycrank+58,	0,		0,	
yycrank+-42,	yysvec+84,	yyvstop+36,
yycrank+-56,	0,		0,	
yycrank+71,	0,		0,	
yycrank+-44,	yysvec+88,	yyvstop+38,
yycrank+70,	0,		0,	
yycrank+57,	0,		0,	
yycrank+58,	0,		0,	
yycrank+69,	0,		0,	
yycrank+65,	0,		0,	
yycrank+73,	0,		0,	
yycrank+24,	0,		0,	
yycrank+75,	0,		0,	
yycrank+73,	0,		0,	
yycrank+71,	0,		0,	
yycrank+79,	0,		0,	
yycrank+-66,	0,		0,	
yycrank+-48,	yysvec+102,	0,	
yycrank+0,	0,		yyvstop+40,
0,	0,	0};
struct yywork *yytop = yycrank+180;
struct yysvf *yybgin = yysvec+1;
char yymatch[] = {
  0,   1,   1,   1,   1,   1,   1,   1, 
  1,   9,  10,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  9,   1,   1,   1,   1,   1,   1,   1, 
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
