%{
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

#define IFECHO if (echoflag) ECHO

#define GETTOKEN(tokenId) { \
	IFECHO; \
	YYDUMP(); \
	strcpy(yylval.cstring, yytext); \
	return(tokenId); \
}

// externs 
extern int echoflag;

%}
identifier		[a-z][a-zA-Z0-9_]*
pidentifier		[A-Z][a-zA-Z0-9_]*
number			[0-9]+
string			\"[^\"]*\"
whitespace		[ \t]+
pound			#[^\n]*"\n"
newline			"\n"
%%

{whitespace}	IFECHO;
{pound}		IFECHO;
{newline}	IFECHO;
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
"{"		GETTOKEN(LBRACE);
"}"		GETTOKEN(RBRACE);
"="		GETTOKEN(EQUAL);
"!="		GETTOKEN(NOTEQUAL);
"argument"	GETTOKEN(ARGUMENT);
"quit"		GETTOKEN(QUIT);
"True"		GETTOKEN(TRUE);
"False"		GETTOKEN(FALSE);
"SOS"		GETTOKEN(SOS);
{number}	GETTOKEN(NUMBER);
{string}	{
		IFECHO;
		YYDUMP();
		yytext[strlen(yytext)-1] = 0;
		strcpy(yylval.cstring, yytext+1);
		return(STRING);
		}
{identifier}	GETTOKEN(IDENTIFIER);
{pidentifier}	GETTOKEN(PIDENTIFIER);
.		{
		IFECHO;
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
