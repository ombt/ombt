%{
/* standard unix headers */
#include <stdio.h>
#include <string.h>
#include <math.h>

/* other headers */
#include "omybc.h"
#include "omybcyacc.h"

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

%}
real			[0-9]+"."[0-9]+([eE][-+]?[0-9]+)?
decimal			[0-9]+
hexadecimal		0?[xXhH][a-fA-F0-9]+
octal			[oO][0-7]+
binary			[bB][01]+
identifier		[a-zA-Z][a-zA-Z0-9]*
whitespace		[ \t]+
pound			#[^\n]*"\n"
newline			"\n"
semicolon		";"
%%

{whitespace}	MYECHO;
{pound}		MYECHO;
{hexadecimal}	{
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
{octal}		{
		MYECHO;
		YYDUMP();
		yylval.number.value.ulvalue = strtol(yytext+1, (char **)0, 8);
		yylval.number.type = ULONG;
		return(ULCONST);
		}
{binary}	{
		MYECHO;
		YYDUMP();
		yylval.number.value.ulvalue = strtol(yytext+1, (char **)0, 2);
		yylval.number.type = ULONG;
		return(ULCONST);
		}
{decimal}	{
		MYECHO;
		YYDUMP();
		yylval.number.value.lvalue = strtol(yytext, (char **)0, 10);
		yylval.number.type = LONG;
		return(LCONST);
		}
{real}		{
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
"||"		GETTOKEN(OR);
"&&"		GETTOKEN(AND);
"|"		GETTOKEN(BITOR);
"^"		GETTOKEN(BITXOR);
"&"		GETTOKEN(BITAND);
"=="		GETTOKEN(EQUAL);
"!="		GETTOKEN(NOTEQUAL);
"<"		GETTOKEN(LT);
">"		GETTOKEN(GT);
"<="		GETTOKEN(LE);
">="		GETTOKEN(GE);
"<<"		GETTOKEN(LSHIFT);
">>"		GETTOKEN(RSHIFT);
"+"		GETTOKEN(PLUS);
"-"		GETTOKEN(MINUS);
"*"		GETTOKEN(STAR);
"/"		GETTOKEN(SLASH);
"%"		GETTOKEN(PERCENT);
"("		GETTOKEN(LPAREN);
")"		GETTOKEN(RPAREN);
"~"		GETTOKEN(TILDE);
"!"		GETTOKEN(NOT);
","		GETTOKEN(COMMA);
"?"		GETTOKEN(HELP);
"-?"		GETTOKEN(HELP);
{identifier}	{
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
{newline}	GETTOKEN(NEWLINE);
{semicolon}	GETTOKEN(SEMICOLON);
.		{
		MYECHO;
		YYDUMP();
		strcpy(yylval.string, yytext);
		return(*yytext);
		}

%%

int yywrap(void) { return(1); }

#if 0
main()
{
	while (yylex() != 0);
}
#endif
