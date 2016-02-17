%{
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

%}
identifier		[a-z][a-zA-Z0-9_]*
pidentifier		[A-Z][a-zA-Z0-9_]*
number			[0-9]+
string			\"[^\"]*\"
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
"<"		GETTOKEN(LEFTUNIVERSAL);
">"		GETTOKEN(RIGHTUNIVERSAL);
"["		GETTOKEN(LEFTEXISTENTIAL);
"]"		GETTOKEN(RIGHTEXISTENTIAL);
"quit"		GETTOKEN(QUIT);
"True"		GETTOKEN(TRUE);
"False"		GETTOKEN(FALSE);
{number}	GETTOKEN(NUMBER);
{string}	GETTOKEN(STRING);
{identifier}	GETTOKEN(IDENTIFIER);
{pidentifier}	GETTOKEN(PIDENTIFIER);
.		{
		ECHO;
		YYDUMP();
		strcpy(yylval.cstring, yytext);
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
