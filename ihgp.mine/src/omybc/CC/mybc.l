%{
/* standard unix headers */
#include <stdio.h>
#include <string.h>

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

%}
real			[0-9]+"."[0-9]+([eE][-+]?[0-9]+)?
decimal			[0-9]+
hexadecimal		[xXhH][a-fA-F0-9]+
octal			[oO][0-7]+
binary			[bB][01]+
identifier		[a-zA-Z][a-zA-Z0-9]*
whitespace		[ \t]+
newline			"\n"
%%

{whitespace}	;
{decimal}	{
		YYDUMP();
		yylval.number.value.lvalue = strtol(yytext, (char **)0, 10);
		yylval.number.type = LONG;
		return(LCONST);
		}
{hexadecimal}	{
		YYDUMP();
		yylval.number.value.ulvalue = strtol(yytext+1, (char **)0, 16);
		yylval.number.type = ULONG;
		return(ULCONST);
		}
{octal}		{
		YYDUMP();
		yylval.number.value.ulvalue = strtol(yytext+1, (char **)0, 8);
		yylval.number.type = ULONG;
		return(ULCONST);
		}
{binary}	{
		YYDUMP();
		yylval.number.value.ulvalue = strtol(yytext+1, (char **)0, 2);
		yylval.number.type = ULONG;
		return(ULCONST);
		}
{real}		{
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
{identifier}	{
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
{newline}	GETTOKEN(NEWLINE);
.		{
		YYDUMP();
		strcpy(yylval.string, yytext);
		return(*yytext);
		}

%%

/*
main()
{
	while (yylex() != 0);
}
 */
