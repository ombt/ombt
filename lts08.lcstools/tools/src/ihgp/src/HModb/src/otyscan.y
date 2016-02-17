
%{
// parser for oty files

// headers
#include "otyscan.h"

// externs
extern int optind;
extern char *optarg;

// globals
int verbose = 0;
int syntaxflag = 0;

%}

%start begin

%union
{
	char cvalue[128];
}

%token <cvalue> NEWLINE
%token <cvalue> MSGID
%token <cvalue> TIME
%token <cvalue> CLASS
%token <cvalue> ACTION
%token <cvalue> OUTPRIOR
%token <cvalue> FREQ
%token <cvalue> PROTOTYPE
%token <cvalue> FIELD
%token <cvalue> FIELDUP
%token <cvalue> NPVAL
%token <cvalue> WIDTH
%token <cvalue> ENUM
%token <cvalue> OUTPUT
%token <cvalue> LEFTBRACE
%token <cvalue> RIGHTBRACE

%%

begin:		/* empty */
		| begin msg
		| begin NEWLINE
		| begin error RIGHTBRACE NEWLINE
		{
			yyerrok;
		}
		;

msg:		msgstart
		msgbody
		msgend
		;


msgstart:	OUTPUT LEFTBRACE NEWLINE
		{
			msgstartSemantics();
		}
		;

msgend:		RIGHTBRACE NEWLINE
		{
			msgendSemantics();
		}
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

requiredbody:	MSGID 
		{
			msgidSemantics($1);
		}
		| TIME 
		{
			timeSemantics($1);
		}
		| CLASS 
		{
			classSemantics($1);
		}
		| ACTION 
		{
			actionSemantics($1);
		}
		| OUTPRIOR 
		{
			outpriorSemantics($1);
		}
		| FREQ 
		{
			freqSemantics($1);
		}
		| PROTOTYPE 
		{
			prototypeSemantics($1);
		}
		;


optionalbody:	FIELD 
		{
			fieldSemantics($1);
		}
		| FIELDUP 
		{
			fieldupSemantics($1);
		}
		| NPVAL 
		{
			npvalSemantics($1);
		}
		| WIDTH 
		{
			widthSemantics($1);
		}
		| ENUM
		{
			enumSemantics($1);
		}
		;

%%

// report yacc errors
int
yyerror(char *s)
{
	if (syntaxflag)
	{
		fprintf(stdout, "%s.\n", s);
	}
	return(0);
}

// usage msg
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

// main entry point
main(int argc, char **argv)
{
	int c;
#ifdef YYDEBUG
	extern int yydebug;
	syntaxflag = 1;
	yydebug = 1;
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

