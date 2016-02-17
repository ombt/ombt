%{
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

%}

msgid			msgid[^\n]+
time			time[^\n]+
class			class[^\n]+
action			action[^\n]+
outprior		outprior[^\n]+
freq			freq[^\n]+
prototype		prototype[^\n]+
field			field[^\n]+
fieldup			fieldup[^\n]+
npval			npval[^\n]+
width			width[^\n]+
enum			enum[^\n]+
output			output
processing		"otypp: processing file"[^\n]+\n
leftbrace		"{"
rightbrace		"}"
whitespace		[ \t]+
newline			"\n"

%%

{whitespace}	;
{newline}	GETTOKEN(NEWLINE);
{msgid}		GETTOKEN(MSGID);
{time}		GETTOKEN(TIME);
{class}		GETTOKEN(CLASS);
{action}	GETTOKEN(ACTION);
{outprior}	GETTOKEN(OUTPRIOR);
{freq}		GETTOKEN(FREQ);
{prototype}	GETTOKEN(PROTOTYPE);
{field}		GETTOKEN(FIELD);
{fieldup}	GETTOKEN(FIELDUP);
{npval}		GETTOKEN(NPVAL);
{width}		GETTOKEN(WIDTH);
{enum}		GETTOKEN(ENUM);
{output}	GETTOKEN(OUTPUT);
{processing}	{
		YYDUMP();
		VERBOSE(stdout, "otyscan: %s", yytext+strlen("otypp: "));
		}
{leftbrace}	GETTOKEN(LEFTBRACE);
{rightbrace}	GETTOKEN(RIGHTBRACE);

%%

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
