%{
// lexical analyzer for reading oty files 

// headers 
#include "otyscan.h"
#include "otyscanyacc.h"

// keyword list
struct KEYWORDS {
	char *keyword;
	int tokenid;
} keywords[] = {
	"ABBREV", ABBREV,
	"ACT", ACT,
	"ACT1", ACT1,
	"ACT2", ACT2,
	"BLANK", BLANK,
	"CRIT", CRIT,
	"DEATH", DEATH,
	"DCF", DCF,
	"DEBUG", OTYDEBUG,
	"FULL", FULL,
	"INFO", INFO,
	"MAJ", MAJ,
	"MAN", MAN,
	"MIN", MIN,
	"NORM", NORM,
	"REGULAR", REGULAR,
	"RSP", RSP,
	"SPURT", SPURT,
	"VAR", VAR,
	"ZERO", ZERO,
	"action", ACTION,
	"ary_hex", ARY_HEX,
	"ary_num", ARY_NUM,
	"ary_oct", ARY_OCT,
	"class", CLASS,
	"complr", COMPLR,
	"dump_cnt", DUMP_CNT,
	"field", FIELD,
	"fieldup", FIELDUP,
	"freq", FREQ,
	"fwary_hex", FWARY_HEX,
	"fwary_num", FWARY_NUM,
	"fwary_oct", FWARY_OCT,
	"hex_num", HEX_NUM,
	"hndlprio", HNDLPRIO,
	"im_num", IM_NUM,
	"lrng", LRNG,
	"msgcls", MSGCLS,
	"msgid", MSGID,
	"nlogu", NLOGU,
	"np_hnum", NP_HNUM,
	"np_imnum", NP_IMNUM,
	"np_num", NP_NUM,
	"np_onum", NP_ONUM,
	"np_rng", NP_RNG,
	"npval", NPVAL,
	"num", NUM,
	"oct_num", OCT_NUM,
	"outprior", OUTPRIOR,
	"output", OUTPUT,
	"pad0_num", PAD0_NUM,
	"pad0_unum", PAD0_UNUM,
	"pad_num", PAD_NUM,
	"pad_unum", PAD_UNUM,
	"phone_32", PHONE_32,
	"phone_no", PHONE_NO,
	"pid", PID,
	"prototype", PROTOTYPE,
	"rng", RNG,
	"rnglogu", RNGLOGU,
	"senum", SENUM,
	"sm_id", SM_ID,
	"str", STR,
	"str4bit", STR4BIT,
	"tab_enum", TAB_ENUM,
	"time", TIME,
	"unum", UNUM,
	"verb", VERB,
	"width", WIDTH
};

#define KEYWORDSZ (sizeof(keywords)/sizeof(struct KEYWORDS))

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

#define CHECKTOKEN(tokenId) { \
	int low, mid, high; \
	low = 0; \
	high = KEYWORDSZ-1; \
	while (low <= high) \
	{ \
		int result; \
		mid = low + (high-low)/2; \
		if ((result = strcmp(keywords[mid].keyword, yytext)) == 0) \
		{ \
			GETTOKEN(keywords[mid].tokenid); \
		} \
		else if (result < 0) \
		{ \
			low = mid + 1; \
		} \
		else \
		{ \
			high = mid - 1; \
		} \
	} \
	GETTOKEN(tokenId); \
}

%}

otyenum			enum[^\n]+
lowercase		[a-z]+
uppercase		[A-Z]+
identifier		[a-zA-Z][a-zA-Z0-9_]*
number			[0-9]+
hexnumber		0x[0-9a-fA-F]+
whitespace		[ \t]+
newline			"\n"

%%

{whitespace}	;
{newline}	GETTOKEN(NEWLINE);
{otyenum}	GETTOKEN(ENUM);
{lowercase}	CHECKTOKEN(LOWERCASE);
{uppercase}	CHECKTOKEN(UPPERCASE);
{identifier}	CHECKTOKEN(IDENTIFIER);
{number}	{
		YYDUMP();
		yylval.lvalue = atol(yytext);
		return(NUMBER);
		}
{hexnumber}	{
		YYDUMP();
		yylval.ulvalue = strtol(yytext, NULL, BASE16);
		return(NUMBER);
		}
.		{
		YYDUMP();
		strcpy(yylval.cvalue, yytext);
		switch (yytext[0])
		{
		case '=':
			return(EQUAL);
		case '%':
			return(PERCENT);
		case ',':
			return(COMMA);
		case ':':
			return(COLON);
		case ';':
			return(SEMICOLON);
		case '-':
			return(MINUS);
		case '_':
			return(UNDERSCORE);
		case '.':
			return(PERIOD);
		case '~':
			return(TILDE);
		case '{':
			return(LEFTBRACE);
		case '}':
			return(RIGHTBRACE);
		case '(':
			return(LEFTPAREN);
		case ')':
			return(RIGHTPAREN);
		case '[':
			return(LEFTBRACKET);
		case ']':
			return(RIGHTBRACKET);
		case '\\':
			return(BACKSLASH);
		case '/':
			return(SLASH);
		case '+':
			return(PLUS);
		case '#':
			return(SHARP);
		case '\'':
			return(SINGLEQUOTE);
		case '*':
			return(STAR);
		case '>':
			return(GREATERTHAN);
		case '<':
			return(LESSTHAN);
		case '|':
			return(BAR);
		case '`':
			return(BACKQUOTE);
		case '\"':
			return(DOUBLEQUOTE);
		case '&':
			return(AND);
		case '$':
			return(DOLLAR);
		case '\014':
			// ignore form-feeds (^L, \f)
			break;
		default:
			ERRORS("discarding bad token", yytext, EINVAL);
		}
		}
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
	char *tokenid;
	while ((tokenid = (char *)yylex()))
	{
		fprintf(stderr, "token ... %s, %s\n", yytext, tokenid);
	}
	exit(0);
}

#endif
