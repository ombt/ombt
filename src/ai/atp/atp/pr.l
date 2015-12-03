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

#define IFECHO if (echo) ECHO

#define GETTOKEN(tokenId) { \
	IFECHO; \
	YYDUMP(); \
	strcpy(yylval.cstring, yytext); \
	return(tokenId); \
}

// externs 
extern int echo;

%}
identifier		[a-z][a-zA-Z0-9_]*
pidentifier		[A-Z][a-zA-Z0-9_]*
number			[0-9]+
negnumber		-[0-9]+
string			\"[^\"]*\"
whitespace		[ \t]+
pound			#[^\n]*"\n"
newline			"\n"
%%

{whitespace}	IFECHO;
{pound}		IFECHO;
{newline}	IFECHO;
"|-"		GETTOKEN(THEREFORE);
"?-"		GETTOKEN(QUERY);
"<-->"		GETTOKEN(BICONDITIONAL);
"-->"		GETTOKEN(IMPLICATION);
"||"		GETTOKEN(OR);
"&&"		GETTOKEN(AND);
"~"		GETTOKEN(NEGATION);
"("		GETTOKEN(LPAREN);
")"		GETTOKEN(RPAREN);
","		GETTOKEN(COMMA);
";"		GETTOKEN(SEMICOLON);
"forall"	GETTOKEN(UNIVERSAL);
"<"		GETTOKEN(LEFTUNIVERSAL);
">"		GETTOKEN(RIGHTUNIVERSAL);
"forsome"	GETTOKEN(EXISTENTIAL);
"["		GETTOKEN(LEFTEXISTENTIAL);
"]"		GETTOKEN(RIGHTEXISTENTIAL);
"{"		GETTOKEN(LBRACE);
"}"		GETTOKEN(RBRACE);
"="		GETTOKEN(EQUAL);
"!="		GETTOKEN(NOTEQUAL);
"argument"	GETTOKEN(ARGUMENT);
"quit"		GETTOKEN(QUIT);
"true"		GETTOKEN(TRUE);
"false"		GETTOKEN(FALSE);
"SOS"		GETTOKEN(SOS);
"options"	GETTOKEN(OPTION);
{number}	GETTOKEN(NUMBER);
{negnumber}	GETTOKEN(NEGNUMBER);
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
