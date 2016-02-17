
%{
/* parser for oty files */

/* headers */
#include "otyscan.h"

/* externs */
extern int optind;
extern char *optarg;

// globals
static int verbose = 0;
static int syntaxflag = 0;

// local definitions
#define VERBOSE if (verbose) fprintf

%}

%start begin

%union
{
	char cvalue[128];
	long lvalue;
	unsigned long ulvalue;
}

%token <cvalue> ABBREV
%token <cvalue> ACT
%token <cvalue> ACT1
%token <cvalue> ACT2
%token <cvalue> BLANK
%token <cvalue> CRIT
%token <cvalue> DCF
%token <cvalue> FULL
%token <cvalue> RSP
%token <cvalue> INFO
%token <cvalue> DEATH
%token <cvalue> MAJ
%token <cvalue> MAN
%token <cvalue> MIN
%token <cvalue> REGULAR
%token <cvalue> SPURT
%token <cvalue> ZERO
%token <cvalue> ACTION
%token <cvalue> ARY_HEX
%token <cvalue> ARY_NUM
%token <cvalue> ARY_OCT
%token <cvalue> CLASS
%token <cvalue> COMPLR
%token <cvalue> OTYDEBUG
%token <cvalue> VAR
%token <cvalue> DUMP_CNT
%token <cvalue> ENUM
%token <cvalue> FIELD
%token <cvalue> FIELDUP
%token <cvalue> FREQ
%token <cvalue> FWARY_HEX
%token <cvalue> FWARY_NUM
%token <cvalue> FWARY_OCT
%token <cvalue> HEX_NUM
%token <cvalue> HNDLPRIO
%token <cvalue> IM_NUM
%token <cvalue> LRNG
%token <cvalue> MSGCLS
%token <cvalue> MSGID
%token <cvalue> NLOGU
%token <cvalue> NORM
%token <cvalue> NP_HNUM
%token <cvalue> NP_IMNUM
%token <cvalue> NP_NUM
%token <cvalue> NP_ONUM
%token <cvalue> NP_RNG
%token <cvalue> NPVAL
%token <cvalue> NUM
%token <cvalue> OCT_NUM
%token <cvalue> OUTPRIOR
%token <cvalue> PAD0_NUM
%token <cvalue> PAD0_UNUM
%token <cvalue> PAD_NUM
%token <cvalue> PAD_UNUM
%token <cvalue> PHONE_NO
%token <cvalue> PHONE_32
%token <cvalue> PID
%token <cvalue> PROTOTYPE
%token <cvalue> RNG
%token <cvalue> RNGLOGU
%token <cvalue> SENUM
%token <cvalue> SM_ID
%token <cvalue> STR
%token <cvalue> STR4BIT
%token <cvalue> TAB_ENUM
%token <cvalue> TIME
%token <cvalue> UNUM
%token <cvalue> VERB
%token <cvalue> WIDTH
%token <cvalue> OUTPUT
%token <cvalue> LOWERCASE
%token <cvalue> UPPERCASE
%token <cvalue> IDENTIFIER
%token <lvalue> NUMBER
%token <ulvalue> HEXNUMBER
%token <cvalue> NEWLINE
%token <cvalue> EQUAL
%token <cvalue> PERCENT
%token <cvalue> COMMA
%token <cvalue> COLON
%token <cvalue> SEMICOLON
%token <cvalue> MINUS
%token <cvalue> UNDERSCORE
%token <cvalue> PERIOD
%token <cvalue> TILDE
%token <cvalue> LEFTBRACE
%token <cvalue> RIGHTBRACE
%token <cvalue> LEFTPAREN
%token <cvalue> RIGHTPAREN
%token <cvalue> LEFTBRACKET
%token <cvalue> RIGHTBRACKET
%token <cvalue> SLASH
%token <cvalue> BACKSLASH
%token <cvalue> PLUS
%token <cvalue> SHARP
%token <cvalue> SINGLEQUOTE
%token <cvalue> STAR
%token <cvalue> GREATERTHAN
%token <cvalue> LESSTHAN
%token <cvalue> BARTHAN
%token <cvalue> BACKQUOTE
%token <cvalue> DOUBLEQUOTE
%token <cvalue> BAR
%token <cvalue> AND
%token <cvalue> DOLLAR

%type <cvalue> tstamp
%type <cvalue> prioract
%type <cvalue> hndlprio
%type <cvalue> frequency
%type <cvalue> msgname
%type <cvalue> classname
%type <cvalue> punctuation
%type <cvalue> otranstyp
%type <cvalue> varfld
%type <ulvalue> padwidth
%type <cvalue> padtype
%type <cvalue> identifier
%type <cvalue> verbs

%%

begin:		/* empty */
		| begin msg
		| begin NEWLINE
		| begin error RIGHTBRACE NEWLINE
		{
			yyerrok;
		}
		;

msg:		msgstart msgbody msgend
		;


msgstart:	OUTPUT LEFTBRACE NEWLINE
		;

msgend:		RIGHTBRACE NEWLINE
		| RIGHTBRACE SEMICOLON NEWLINE
		;

msgbody:	error NEWLINE
		{
			yyerrok;
		}
		| msgbody error NEWLINE
		{
			yyerrok;
		}
		| NEWLINE
		| msgbody NEWLINE
		| requiredbody NEWLINE
		| msgbody requiredbody NEWLINE
		| optionalbody NEWLINE
		| msgbody optionalbody NEWLINE
		;

requiredbody:	MSGID msgname
		| TIME tstamp
		| CLASS classname
		| ACTION prioract
		| OUTPRIOR hndlprio
		| FREQ frequency
		| PROTOTYPE msgline
		;

msgname:	IDENTIFIER
		{
			strcpy($$, $1);
		}
		| UPPERCASE
		{
			strcpy($$, $1);
		}
		| LOWERCASE
		{
			strcpy($$, $1);
		}
		;

classname:	IDENTIFIER
		{
			strcpy($$, $1);
		}
		| UPPERCASE
		{
			strcpy($$, $1);
		}
		| LOWERCASE
		{
			strcpy($$, $1);
		}
		| VAR
		{
			strcpy($$, $1);
		}
		;

optionalbody:	FIELD itm_name otranstyp varfld padwidth padtype
		| FIELDUP itm_name LOWERCASE LOWERCASE
		| NPVAL npvalue
		| WIDTH widthv
		| ENUM
		;

widthv:		primaryexpr
		;

npvalue:	primaryexpr
		;

itm_name:	postfixexpr
		;

postfixexpr:	primaryexpr
		| postfixexpr LEFTBRACKET postfixexpr RIGHTBRACKET
		| postfixexpr PERIOD identifier
		;

primaryexpr:	identifier
		| NUMBER
		| MINUS NUMBER
		| HEXNUMBER
		| LEFTPAREN postfixexpr RIGHTPAREN
		;

identifier:	LOWERCASE
		{
			strcpy($$, $1);
		}
		| UPPERCASE
		{
			strcpy($$, $1);
		}
		| IDENTIFIER
		{
			strcpy($$, $1);
		}
		| verbs
		{
			strcpy($$, $1);
		}
		;

verbs:		ABBREV
		{
			strcpy($$, $1);
		}
		| ACT
		{
			strcpy($$, $1);
		}
		| ACT1
		{
			strcpy($$, $1);
		}
		| ACT2
		{
			strcpy($$, $1);
		}
		| BLANK
		{
			strcpy($$, $1);
		}
		| CRIT
		{
			strcpy($$, $1);
		}
		| DEATH
		{
			strcpy($$, $1);
		}
		| OTYDEBUG
		{
			strcpy($$, $1);
		}
		| FULL
		{
			strcpy($$, $1);
		}
		| INFO
		{
			strcpy($$, $1);
		}
		| MAJ
		{
			strcpy($$, $1);
		}
		| MAN
		{
			strcpy($$, $1);
		}
		| MIN
		{
			strcpy($$, $1);
		}
		| NORM
		{
			strcpy($$, $1);
		}
		| REGULAR
		{
			strcpy($$, $1);
		}
		| RSP
		{
			strcpy($$, $1);
		}
		| VAR
		{
			strcpy($$, $1);
		}
		| ZERO
		{
			strcpy($$, $1);
		}
		| ACTION
		{
			strcpy($$, $1);
		}
		| ARY_NUM
		{
			strcpy($$, $1);
		}
		| ARY_OCT
		{
			strcpy($$, $1);
		}
		| CLASS
		{
			strcpy($$, $1);
		}
		| COMPLR
		{
			strcpy($$, $1);
		}
		| DUMP_CNT
		{
			strcpy($$, $1);
		}
		| FIELD
		{
			strcpy($$, $1);
		}
		| FIELDUP
		{
			strcpy($$, $1);
		}
		| FREQ
		{
			strcpy($$, $1);
		}
		| FWARY_HEX
		{
			strcpy($$, $1);
		}
		| FWARY_NUM
		{
			strcpy($$, $1);
		}
		| FWARY_OCT
		{
			strcpy($$, $1);
		}
		| HEX_NUM
		{
			strcpy($$, $1);
		}
		| HNDLPRIO
		{
			strcpy($$, $1);
		}
		| IM_NUM
		{
			strcpy($$, $1);
		}
		| LRNG
		{
			strcpy($$, $1);
		}
		| MSGCLS
		{
			strcpy($$, $1);
		}
		| MSGID
		{
			strcpy($$, $1);
		}
		| NLOGU
		{
			strcpy($$, $1);
		}
		| NP_HNUM
		{
			strcpy($$, $1);
		}
		| NP_IMNUM
		{
			strcpy($$, $1);
		}
		| NP_NUM
		{
			strcpy($$, $1);
		}
		| NP_ONUM
		{
			strcpy($$, $1);
		}
		| NP_RNG
		{
			strcpy($$, $1);
		}
		| NPVAL
		{
			strcpy($$, $1);
		}
		| NUM
		{
			strcpy($$, $1);
		}
		| OCT_NUM
		{
			strcpy($$, $1);
		}
		| OUTPRIOR
		{
			strcpy($$, $1);
		}
		| OUTPUT
		{
			strcpy($$, $1);
		}
		| PAD0_NUM
		{
			strcpy($$, $1);
		}
		| PAD0_UNUM
		{
			strcpy($$, $1);
		}
		| PAD_NUM
		{
			strcpy($$, $1);
		}
		| PHONE_NO
		{
			strcpy($$, $1);
		}
		| PID
		{
			strcpy($$, $1);
		}
		| PROTOTYPE
		{
			strcpy($$, $1);
		}
		| RNG
		{
			strcpy($$, $1);
		}
		| RNGLOGU
		{
			strcpy($$, $1);
		}
		| SENUM
		{
			strcpy($$, $1);
		}
		| SM_ID
		{
			strcpy($$, $1);
		}
		| STR
		{
			strcpy($$, $1);
		}
		| STR4BIT
		{
			strcpy($$, $1);
		}
		| TAB_ENUM
		{
			strcpy($$, $1);
		}
		| TIME
		{
			strcpy($$, $1);
		}
		| UNUM
		{
			strcpy($$, $1);
		}
		| VERB
		{
			strcpy($$, $1);
		}
		| WIDTH
		{
			strcpy($$, $1);
		}
		;

otranstyp:	ARY_HEX
		{
			strcpy($$, $1);
		}
		| ARY_NUM
		{
			strcpy($$, $1);
		}
		| ARY_OCT
		{
			strcpy($$, $1);
		}
		| COMPLR
		{
			strcpy($$, $1);
		}
		| DUMP_CNT
		{
			strcpy($$, $1);
		}
		| FWARY_HEX
		{
			strcpy($$, $1);
		}
		| FWARY_NUM
		{
			strcpy($$, $1);
		}
		| FWARY_OCT
		{
			strcpy($$, $1);
		}
		| HEX_NUM
		{
			strcpy($$, $1);
		}
		| HNDLPRIO
		{
			strcpy($$, $1);
		}
		| IM_NUM
		{
			strcpy($$, $1);
		}
		| LRNG
		{
			strcpy($$, $1);
		}
		| MSGCLS
		{
			strcpy($$, $1);
		}
		| NLOGU
		{
			strcpy($$, $1);
		}
		| NP_HNUM
		{
			strcpy($$, $1);
		}
		| NP_IMNUM
		{
			strcpy($$, $1);
		}
		| NP_NUM
		{
			strcpy($$, $1);
		}
		| NP_ONUM
		{
			strcpy($$, $1);
		}
		| NP_RNG
		{
			strcpy($$, $1);
		}
		| NUM
		{
			strcpy($$, $1);
		}
		| OCT_NUM
		{
			strcpy($$, $1);
		}
		| PAD0_NUM
		{
			strcpy($$, $1);
		}
		| PAD0_UNUM
		{
			strcpy($$, $1);
		}
		| PAD_NUM
		{
			strcpy($$, $1);
		}
		| PAD_UNUM
		{
			strcpy($$, $1);
		}
		| PHONE_NO
		{
			strcpy($$, $1);
		}
		| PHONE_32
		{
			strcpy($$, $1);
		}
		| PID
		{
			strcpy($$, $1);
		}
		| RNG
		{
			strcpy($$, $1);
		}
		| RNGLOGU
		{
			strcpy($$, $1);
		}
		| RSP
		{
			strcpy($$, $1);
		}
		| SENUM
		{
			strcpy($$, $1);
		}
		| SM_ID
		{
			strcpy($$, $1);
		}
		| STR
		{
			strcpy($$, $1);
		}
		| STR4BIT
		{
			strcpy($$, $1);
		}
		| TAB_ENUM
		{
			strcpy($$, $1);
		}
		| UNUM
		{
			strcpy($$, $1);
		}
		| VERB
		{
			strcpy($$, $1);
		}
		| WIDTH
		{
			strcpy($$, $1);
		}
		;

varfld:		/* empty */
		{
			strcpy($$, "");
		}
		| LOWERCASE
		{
			strcpy($$, $1);
		}
		| VERB
		{
			strcpy($$, $1);
		}
		| FREQ
		{
			strcpy($$, $1);
		}
		| ACTION
		{
			strcpy($$, $1);
		}
		;

padwidth:	/* empty */
		{
			$$ = 0;
		}
		| NUMBER
		{
			$$ = $1;
		}
		| HEXNUMBER
		{
			$$ = $1;
		}
		;

padtype:	/* empty */
		{
			strcpy($$, "");
		}
		| BLANK
		{
			strcpy($$, $1);
		}
		| ZERO
		{
			strcpy($$, $1);
		}
		;

tstamp:		ABBREV
		{
			strcpy($$, $1);
		}
		| FULL
		{
			strcpy($$, $1);
		}
		;

prioract:	CRIT
		| MAJ
		| MIN
		| MAN
		| ACT
		| INFO
		{
			strcpy($$, $1);
		}
		;

hndlprio:	OTYDEBUG
		{
			strcpy($$, $1);
		}
		| NORM
		{
			strcpy($$, $1);
		}
		| ACT2
		{
			strcpy($$, $1);
		}
		| DCF
		{
			strcpy($$, $1);
		}
		| RSP
		{
			strcpy($$, $1);
		}
		| DEATH
		{
			strcpy($$, $1);
		}
		| INFO
		{
			strcpy($$, $1);
		}
		| ACT1
		{
			strcpy($$, $1);
		}
		| VAR
		{
			strcpy($$, $1);
		}
		;

frequency:	REGULAR
		{
			strcpy($$, $1);
		}
		| SPURT
		{
			strcpy($$, $1);
		}
		;

msgline:	/* empty */
		| msgline msglinefields
		;

msglinefields:	LOWERCASE
		| UPPERCASE
		| IDENTIFIER
		| NUMBER
		| HEXNUMBER
		| punctuation
		| hndlprio
		| FULL
		| ACTION
		| FREQ
		| VERB
		| ACT
		| MIN
		| MAN
		;

punctuation:	EQUAL
		{
			strcpy($$, $1);
		}
		| PERCENT
		{
			strcpy($$, $1);
		}
		| COMMA
		{
			strcpy($$, $1);
		}
		| COLON
		{
			strcpy($$, $1);
		}
		| MINUS
		{
			strcpy($$, $1);
		}
		| UNDERSCORE
		{
			strcpy($$, $1);
		}
		| PERIOD
		{
			strcpy($$, $1);
		}
		| SEMICOLON
		{
			strcpy($$, $1);
		}
		| TILDE
		{
			strcpy($$, $1);
		}
		| SLASH
		{
			strcpy($$, $1);
		}
		| BACKSLASH
		{
			strcpy($$, $1);
		}
		| PLUS
		{
			strcpy($$, $1);
		}
		| SHARP
		{
			strcpy($$, $1);
		}
		| SINGLEQUOTE
		{
			strcpy($$, $1);
		}
		| STAR
		{
			strcpy($$, $1);
		}
		| GREATERTHAN
		{
			strcpy($$, $1);
		}
		| LESSTHAN
		{
			strcpy($$, $1);
		}
		| BAR
		{
			strcpy($$, $1);
		}
		| BACKQUOTE
		{
			strcpy($$, $1);
		}
		| DOUBLEQUOTE
		{
			strcpy($$, $1);
		}
		| AND
		{
			strcpy($$, $1);
		}
		| DOLLAR
		{
			strcpy($$, $1);
		}
		| LEFTPAREN
		{
			strcpy($$, $1);
		}
		| RIGHTPAREN
		{
			strcpy($$, $1);
		}
		| LEFTBRACKET
		{
			strcpy($$, $1);
		}
		| RIGHTBRACKET
		{
			strcpy($$, $1);
		}
		;

%%

/* report yacc errors */
int
yyerror(char *s)
{
	if (syntaxflag)
	{
		fprintf(stdout, "%s.\n", s);
	}
	return(0);
}

/* usage msg */
void
usage(const char *cmd)
{
	fprintf(stderr, 
		"usage: %s [-?V] \n\n"
		"where\n\n"
		"-? = help flag.\n"
		"-V = verbose flag.\n"
		"%s reads stdin and writes to stdout.\n", 
		cmd, cmd);
	return;
}

/* main entry point */
main(int argc, char **argv)
{
	int c;
#ifdef YYDEBUG
	extern int yydebug;
	yydebug = 1;
	syntaxflag = 1;
	setbuf(stdout, (char *)0);
#endif

	// check options
	while ((c = getopt(argc, argv, "?V")) != EOF)
	{
		switch (c)
		{
		case 'V':
			// turn on  verbose option
			verbose = 1;
			syntaxflag = 1;
			break;

		case '?':
			usage(argv[0]);
			exit(0);

		default:
			errno = EINVAL;
			ERROR("invalid command line option", errno);
			exit(2);
		}
	}

	// scan oty files
	VERBOSE(stderr, "yyparse start ...\n");
	yyparse();
	VERBOSE(stderr, "yyparse finish ...\n");

	// all done
	exit(0);
}

