%{
// standard unix headers
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

// other headers
#include "returns.h"
#include "debug.h"
#include "prop.h"
#include "propyacc.h"

// other macros
#ifdef YYDEBUG
#define YYDUMP() fprintf(stdout, "token is <%s>.\n", yytext)
#else
#define YYDUMP() 
#endif

#define GETTOKEN(tokenId) { \
	ECHO; \
	YYDUMP(); \
	strcpy(yylval.string, yytext); \
	return(tokenId); \
}

%}
identifier		[a-zA-Z0-9_][a-zA-Z0-9_]*
whitespace		[ \t]+
pound			#[^\n]*"\n"
newline			"\n"
%%

{whitespace}	ECHO;
{pound}		ECHO;
{newline}	GETTOKEN(NEWLINE);
"|-"		GETTOKEN(THEREFORE);
"<-->"		GETTOKEN(BICONDITIONAL);
"-->"		GETTOKEN(IMPLICATION);
"||"		GETTOKEN(OR);
"&&"		GETTOKEN(AND);
"~"		GETTOKEN(NEGATION);
"("		GETTOKEN(LPAREN);
")"		GETTOKEN(RPAREN);
","		GETTOKEN(COMMA);
";"		GETTOKEN(SEMICOLON);
"quit"		GETTOKEN(QUIT);
"true"		GETTOKEN(TRUE);
"false"		GETTOKEN(FALSE);
{identifier}	GETTOKEN(IDENTIFIER);
.		{
		ECHO;
		YYDUMP();
		strcpy(yylval.string, yytext);
		return(*yytext);
		}

%%

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
