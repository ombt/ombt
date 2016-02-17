%{
/* standard unix headers */
#include <stdio.h>
#include <string.h>

/* other headers */
/* #include "stabyacc.h" */

/* externs */
#if 0
extern "C" {
int yywrap(void);
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

%}
name			[^;:\'\"]+
string			(\"[^\n]*\"|\'[^\n]*\')
decimal			[-]?[0-9]+
hexadecimal		[A-F0-9]+
real			([+-][0-9]+(\.)?[0-9]*([eEqQ][+-]?[0-9]+)|[-+]?INF|QNAN|SNAN)
whitespace		[ \t\n]

%%

{whitespace}	{
		MYECHO;
		fprintf(stderr, "WHITE SPACE !!!\n");
		YYDUMP();
		}
{name}		{
		MYECHO;
		fprintf(stderr, "NAME !!!\n");
		YYDUMP();
		}
{string}	{
		MYECHO;
		fprintf(stderr, "STRING !!!\n");
		YYDUMP();
		}
{decimal}	{
		MYECHO;
		fprintf(stderr, "DECIMAL !!!\n");
		YYDUMP();
		}
{hexadecimal}	{
		MYECHO;
		fprintf(stderr, "HEXADECIMAL !!!\n");
		YYDUMP();
		}
{real}		{
		MYECHO;
		fprintf(stderr, "REAL !!!\n");
		YYDUMP();
		}
.		{
		MYECHO;
		fprintf(stderr, "DEFAULT CASE !!!\n");
		YYDUMP();
		}

%%

int yywrap(void) { return(1); }

main()
{
	while (yylex() != 0);
}
