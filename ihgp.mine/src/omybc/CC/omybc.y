%{
/* required headers */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

/* other headers */
#include "omybc.h"

/* globals */
long outbase = 10;
double xy[NUMBEROFPOINTS][2];
int xyno;

/* externs */
void yyerror(const char *);

/* local definitions */
#undef YES
#define YES 1
#undef NO
#define NO 0

%}

%start start

%union {
	TYPE type;
	VALUE number; 
	char string[32];
};

%token <number> DCONST
%token <number> LCONST
%token <number> ULCONST
%token <string> ACOS
%token <string> AND
%token <string> ASIN
%token <string> ATAN
%token <string> BITAND
%token <string> BITOR
%token <string> BITXOR
%token <string> COMMA
%token <string> COS
%token <string> EQUAL
%token <string> EXP
%token <string> GE
%token <string> GRAPH
%token <string> GT
%token <string> HELP
%token <string> LE
%token <string> LOG
%token <string> LOG10
%token <string> LPAREN
%token <string> LSHIFT
%token <string> LT
%token <string> MINUS
%token <string> NEWLINE
%token <string> NOT
%token <string> NOTEQUAL
%token <string> OR
%token <string> OUTBASE
%token <string> PERCENT
%token <string> PLUS
%token <string> POW
%token <string> QUIT
%token <string> RPAREN
%token <string> RSHIFT
%token <string> SEMICOLON
%token <string> SIN
%token <string> SLASH
%token <string> SQRT
%token <string> STAR
%token <string> STRING
%token <string> TAN
%token <string> TILDE
%token <type> DCAST
%token <type> LCAST
%token <type> ULCAST

%type <number> add
%type <number> and
%type <number> bitand
%type <number> bitor
%type <number> bitxor
%type <number> cast
%type <number> const
%type <number> equality
%type <number> expr
%type <number> mult
%type <number> or
%type <number> primary
%type <number> rel
%type <number> shift
%type <number> unary
%type <string> cmd
%type <type> type

%%

start:
	/* empty */
	| start expr newline
	{
		switch ($2.type)
		{
		case ULONG:
#if 0
			switch (outbase)
			{
			case 8:
				printf("%lo\n", $2.value.ulvalue);
				break;
			case 10:
				printf("%lu\n", $2.value.ulvalue);
				break;
			case 16:
				printf("%lx\n", $2.value.ulvalue);
				break;
			default:
				printf("%d: default case.\n", __LINE__);
				YYERROR;
			}
#else
			printf("%s\n", num2base(outbase, NO, $2.value.ulvalue));
#endif
			break;
		case LONG:
#if 0
			switch (outbase)
			{
			case 8:
				printf("%lo\n", $2.value.lvalue);
				break;
			case 10:
				printf("%ld\n", $2.value.lvalue);
				break;
			case 16:
				printf("%lx\n", $2.value.lvalue);
				break;
			default:
				printf("%d: default case.\n", __LINE__);
				YYERROR;
			}
#else
			printf("%s\n", num2base(outbase, YES, $2.value.lvalue));
#endif
			break;
		case DOUBLE:
			switch (outbase)
			{
			case 2:
			case 8:
			case 16:
				printf("base = 10 for double.\n");
			case 10:
				printf("%e\n", $2.value.dvalue);
				break;
			default:
				printf("%d: default case.\n", __LINE__);
				YYERROR;
			}
			break;
		default:
			printf("%d: default case.\n", __LINE__);
			YYERROR;
		}
	}
	| start cmd newline
	{
		/* do nothing */
	}
	| start error newline
	{
		yyerrok;
	}
	| start newline
	{
		yyerrok;
	}
	;

newline:
	NEWLINE
	| SEMICOLON
	;

cmd:
	QUIT
	{
		YYACCEPT;
	}
	| OUTBASE LCONST
	{
		switch ($2.value.lvalue)
		{
		case 2:
		case 8:
		case 10:
		case 16:
			outbase = $2.value.lvalue;
			break;
		default:
			yyerror((const char *)"allowed bases are 2, 8, 10, 16.");
			YYERROR;
		}
		strcpy($$, $1);
	}
	| HELP
	{
		printf("mybc supports the following operators and math\n");
		printf("functions.\n");
		printf("\n");
		printf("operators:\n");
		printf("|| && | ^ & == != < > <= >= << >> - * /\n");
		printf("%% ( ) ~ !\n");
		printf("\n");
		printf("math functions:\n");
		printf("exp log log10 pow sqrt sin cos tan\n");
		printf("asin acos atan\n");
		printf("\n");
		printf("math constants:\n");
		printf("pi, e\n");
		printf("\n");
		printf("miscellaneous:\n");
		printf("(l) - long typecast\n");
		printf("(ul) - unsigned long typecast\n");
		printf("(d) - double typecast\n");
		printf("[xh][0-9a-z]* - hex numbers\n");
		printf("o[0-7]* - octal numbers\n");
		printf("b[01]* - binary numbers\n");
		printf("[+|-]?[0-9]+.[0-9]+([eE][+-]?[0-9]+)? - real numbers\n");
		printf("ob [2|8|10|16] - set output base\n");
		printf("? - help\n");
		strcpy($$, $1);
	}
	| HELP EXP
	{
		printf("usage: exp(expr)\n");
	}
	| HELP LOG
	{
		printf("usage: log(expr)\n");
	}
	| HELP LOG10
	{
		printf("usage: log10(expr)\n");
	}
	| HELP POW
	{
		printf("usage: pow(base_expr, power_expr)\n");
	}
	| HELP SQRT
	{
		printf("usage: sqrt(expr)\n");
	}
	| HELP SIN
	{
		printf("usage: sin(expr)\n");
	}
	| HELP COS
	{
		printf("usage: cos(expr)\n");
	}
	| HELP TAN
	{
		printf("usage: tan(expr)\n");
	}
	| HELP ASIN
	{
		printf("usage: asin(expr)\n");
	}
	| HELP ACOS
	{
		printf("usage: acos(expr)\n");
	}
	| HELP ATAN
	{
		printf("usage: atan(expr)\n");
	}
	| HELP OUTBASE
	{
		printf("usage: ob [2|8|10|16]\n");
	}
	| GRAPH 
	{
		initgraph();
	}
	LPAREN exprlist RPAREN
	{
		graph();
	}
	;

exprlist:
	expr COMMA expr
	{
		storexy(number2double($1), number2double($3));
	}
	| exprlist COMMA expr COMMA expr
	{
		storexy(number2double($3), number2double($5));
	}
	;

expr:
	or
	{
		$$ = $1;
	}
	;

or:
	and
	{
		$$ = $1;
	}
	| or OR and
	{
		$$.type = LONG;
		switch ($1.type)
		{
		case ULONG:
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.ulvalue || $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.ulvalue || $3.value.lvalue;
				break;
			case DOUBLE:
				$$.value.lvalue =
					$1.value.ulvalue || $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.lvalue || $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.lvalue || $3.value.lvalue;
				break;
			case DOUBLE:
				$$.value.lvalue =
					$1.value.lvalue || $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.dvalue || $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.dvalue || $3.value.lvalue;
				break;
			case DOUBLE:
				$$.value.lvalue =
					$1.value.dvalue || $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	}
	;

and:
	bitor
	{
		$$ = $1;
	}
	| and AND bitor
	{
		$$.type = LONG;
		switch ($1.type)
		{
		case ULONG:
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.ulvalue && $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.ulvalue && $3.value.lvalue;
				break;
			case DOUBLE:
				$$.value.lvalue =
					$1.value.ulvalue && $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.lvalue && $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.lvalue && $3.value.lvalue;
				break;
			case DOUBLE:
				$$.value.lvalue =
					$1.value.lvalue && $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.dvalue && $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.dvalue && $3.value.lvalue;
				break;
			case DOUBLE:
				$$.value.lvalue =
					$1.value.dvalue && $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	}
	;

bitor:
	bitxor
	{
		$$ = $1;
	}
	| bitor BITOR bitxor
	{
		switch ($1.type)
		{
		case ULONG:
			switch ($3.type)
			{
			case ULONG:
				$$.type = ULONG;
				$$.value.ulvalue =
					$1.value.ulvalue | $3.value.ulvalue;
				break;
			case LONG:
				$$.type = ULONG;
				$$.value.ulvalue =
					$1.value.ulvalue | $3.value.lvalue;
				break;
			case DOUBLE:
				yyerror((const char *)"| cannot be applied to double.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch ($3.type)
			{
			case ULONG:
				$$.type = ULONG;
				$$.value.ulvalue =
					$1.value.lvalue | $3.value.ulvalue;
				break;
			case LONG:
				$$.type = LONG;
				$$.value.lvalue =
					$1.value.lvalue | $3.value.lvalue;
				break;
			case DOUBLE:
				yyerror((const char *)"| cannot be applied to double.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror((const char *)"| cannot be applied to double.");
			YYERROR;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	}
	;

bitxor:
	bitand
	{
		$$ = $1;
	}
	| bitxor BITXOR bitand
	{
		switch ($1.type)
		{
		case ULONG:
			switch ($3.type)
			{
			case ULONG:
				$$.type = ULONG;
				$$.value.ulvalue =
					$1.value.ulvalue ^ $3.value.ulvalue;
				break;
			case LONG:
				$$.type = ULONG;
				$$.value.ulvalue =
					$1.value.ulvalue ^ $3.value.lvalue;
				break;
			case DOUBLE:
				yyerror((const char *)"^ cannot be applied to double.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch ($3.type)
			{
			case ULONG:
				$$.type = ULONG;
				$$.value.ulvalue =
					$1.value.lvalue ^ $3.value.ulvalue;
				break;
			case LONG:
				$$.type = LONG;
				$$.value.lvalue =
					$1.value.lvalue ^ $3.value.lvalue;
				break;
			case DOUBLE:
				yyerror((const char *)"^ cannot be applied to double.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror((const char *)"^ cannot be applied to double.");
			YYERROR;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	}
	;

bitand:
	equality
	{
		$$ = $1;
	}
	| bitand BITAND equality
	{
		switch ($1.type)
		{
		case ULONG:
			switch ($3.type)
			{
			case ULONG:
				$$.type = ULONG;
				$$.value.ulvalue =
					$1.value.ulvalue & $3.value.ulvalue;
				break;
			case LONG:
				$$.type = ULONG;
				$$.value.ulvalue =
					$1.value.ulvalue & $3.value.lvalue;
				break;
			case DOUBLE:
				yyerror((const char *)"& cannot be applied to double.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch ($3.type)
			{
			case ULONG:
				$$.type = ULONG;
				$$.value.ulvalue =
					$1.value.lvalue & $3.value.ulvalue;
				break;
			case LONG:
				$$.type = LONG;
				$$.value.lvalue =
					$1.value.lvalue & $3.value.lvalue;
				break;
			case DOUBLE:
				yyerror((const char *)"& cannot be applied to double.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror((const char *)"& cannot be applied to double.");
			YYERROR;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	}
	;

equality:
	rel
	{
		$$ = $1;
	}
	| equality EQUAL rel
	{
		$$.type = LONG;
		switch ($1.type)
		{
		case ULONG:
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.ulvalue == $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.ulvalue == $3.value.lvalue;
				break;
			case DOUBLE:
				$$.value.lvalue =
					$1.value.ulvalue == $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.lvalue == $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.lvalue == $3.value.lvalue;
				break;
			case DOUBLE:
				$$.value.lvalue =
					$1.value.lvalue == $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.dvalue == $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.dvalue == $3.value.lvalue;
				break;
			case DOUBLE:
				$$.value.lvalue =
					$1.value.dvalue == $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	}
	| equality NOTEQUAL rel
	{
		$$.type = LONG;
		switch ($1.type)
		{
		case ULONG:
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.ulvalue != $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.ulvalue != $3.value.lvalue;
				break;
			case DOUBLE:
				$$.value.lvalue =
					$1.value.ulvalue != $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.lvalue != $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.lvalue != $3.value.lvalue;
				break;
			case DOUBLE:
				$$.value.lvalue =
					$1.value.lvalue != $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.dvalue != $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.dvalue != $3.value.lvalue;
				break;
			case DOUBLE:
				$$.value.lvalue =
					$1.value.dvalue != $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	}
	;

rel:
	shift
	{
		$$ = $1;
	}
	| rel LT shift
	{
		$$.type = LONG;
		switch ($1.type)
		{
		case ULONG:
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.ulvalue < $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.ulvalue < $3.value.lvalue;
				break;
			case DOUBLE:
				$$.value.lvalue =
					$1.value.ulvalue < $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.lvalue < $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.lvalue < $3.value.lvalue;
				break;
			case DOUBLE:
				$$.value.lvalue =
					$1.value.lvalue < $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.dvalue < $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.dvalue < $3.value.lvalue;
				break;
			case DOUBLE:
				$$.value.lvalue =
					$1.value.dvalue < $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	}
	| rel GT shift
	{
		$$.type = LONG;
		switch ($1.type)
		{
		case ULONG:
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.ulvalue > $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.ulvalue > $3.value.lvalue;
				break;
			case DOUBLE:
				$$.value.lvalue =
					$1.value.ulvalue > $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.lvalue > $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.lvalue > $3.value.lvalue;
				break;
			case DOUBLE:
				$$.value.lvalue =
					$1.value.lvalue > $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.dvalue > $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.dvalue > $3.value.lvalue;
				break;
			case DOUBLE:
				$$.value.lvalue =
					$1.value.dvalue > $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	}
	| rel LE shift
	{
		$$.type = LONG;
		switch ($1.type)
		{
		case ULONG:
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.ulvalue <= $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.ulvalue <= $3.value.lvalue;
				break;
			case DOUBLE:
				$$.value.lvalue =
					$1.value.ulvalue <= $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.lvalue <= $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.lvalue <= $3.value.lvalue;
				break;
			case DOUBLE:
				$$.value.lvalue =
					$1.value.lvalue <= $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.dvalue <= $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.dvalue <= $3.value.lvalue;
				break;
			case DOUBLE:
				$$.value.lvalue =
					$1.value.dvalue <= $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	}
	| rel GE shift
	{
		$$.type = LONG;
		switch ($1.type)
		{
		case ULONG:
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.ulvalue >= $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.ulvalue >= $3.value.lvalue;
				break;
			case DOUBLE:
				$$.value.lvalue =
					$1.value.ulvalue >= $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.lvalue >= $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.lvalue >= $3.value.lvalue;
				break;
			case DOUBLE:
				$$.value.lvalue =
					$1.value.lvalue >= $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.dvalue >= $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.dvalue >= $3.value.lvalue;
				break;
			case DOUBLE:
				$$.value.lvalue =
					$1.value.dvalue >= $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	}
	;

shift:
	add
	{
		$$ = $1;
	}
	| shift LSHIFT add
	{
		switch ($1.type)
		{
		case ULONG:
			$$.type = ULONG;
			switch ($3.type)
			{
			case ULONG:
				$$.value.ulvalue =
					$1.value.ulvalue << $3.value.ulvalue;
				break;
			case LONG:
				$$.value.ulvalue =
					$1.value.ulvalue << $3.value.lvalue;
				break;
			case DOUBLE:
				yyerror((const char *)"<< cannot be applied to double.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			$$.type = LONG;
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.lvalue << $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.lvalue << $3.value.lvalue;
				break;
			case DOUBLE:
				yyerror((const char *)"<< cannot be applied to double.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror((const char *)"<< cannot be applied to double.");
			YYERROR;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	}
	| shift RSHIFT add
	{
		switch ($1.type)
		{
		case ULONG:
			$$.type = ULONG;
			switch ($3.type)
			{
			case ULONG:
				$$.value.ulvalue =
					$1.value.ulvalue >> $3.value.ulvalue;
				break;
			case LONG:
				$$.value.ulvalue =
					$1.value.ulvalue >> $3.value.lvalue;
				break;
			case DOUBLE:
				yyerror((const char *)">> cannot be applied to double.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			$$.type = LONG;
			switch ($3.type)
			{
			case ULONG:
				$$.value.lvalue =
					$1.value.lvalue >> $3.value.ulvalue;
				break;
			case LONG:
				$$.value.lvalue =
					$1.value.lvalue >> $3.value.lvalue;
				break;
			case DOUBLE:
				yyerror((const char *)">> cannot be applied to double.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror((const char *)">> cannot be applied to double.");
			YYERROR;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	}
	;

add:
	mult
	{
		$$ = $1;
	}
	| add PLUS mult
	{
		switch ($1.type)
		{
		case ULONG:
			switch ($3.type)
			{
			case ULONG:
				$$.type = ULONG;
				$$.value.ulvalue =
					$1.value.ulvalue + $3.value.ulvalue;
				break;
			case LONG:
				$$.type = ULONG;
				$$.value.ulvalue =
					$1.value.ulvalue + $3.value.lvalue;
				break;
			case DOUBLE:
				$$.type = DOUBLE;
				$$.value.dvalue =
					$1.value.ulvalue + $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch ($3.type)
			{
			case ULONG:
				$$.type = ULONG;
				$$.value.ulvalue =
					$1.value.lvalue + $3.value.ulvalue;
				break;
			case LONG:
				$$.type = LONG;
				$$.value.lvalue =
					$1.value.lvalue + $3.value.lvalue;
				break;
			case DOUBLE:
				$$.type = DOUBLE;
				$$.value.dvalue =
					$1.value.lvalue + $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch ($3.type)
			{
			case ULONG:
				$$.type = DOUBLE;
				$$.value.dvalue =
					$1.value.dvalue + $3.value.ulvalue;
				break;
			case LONG:
				$$.type = DOUBLE;
				$$.value.dvalue =
					$1.value.dvalue + $3.value.lvalue;
				break;
			case DOUBLE:
				$$.type = DOUBLE;
				$$.value.dvalue =
					$1.value.dvalue + $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	}
	| add MINUS mult
	{
		switch ($1.type)
		{
		case ULONG:
			switch ($3.type)
			{
			case ULONG:
				$$.type = ULONG;
				$$.value.ulvalue =
					$1.value.ulvalue - $3.value.ulvalue;
				break;
			case LONG:
				$$.type = ULONG;
				$$.value.ulvalue =
					$1.value.ulvalue - $3.value.lvalue;
				break;
			case DOUBLE:
				$$.type = DOUBLE;
				$$.value.dvalue =
					$1.value.ulvalue - $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch ($3.type)
			{
			case ULONG:
				$$.type = ULONG;
				$$.value.ulvalue =
					$1.value.lvalue - $3.value.ulvalue;
				break;
			case LONG:
				$$.type = LONG;
				$$.value.lvalue =
					$1.value.lvalue - $3.value.lvalue;
				break;
			case DOUBLE:
				$$.type = DOUBLE;
				$$.value.dvalue =
					$1.value.lvalue - $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch ($3.type)
			{
			case ULONG:
				$$.type = DOUBLE;
				$$.value.dvalue =
					$1.value.dvalue - $3.value.ulvalue;
				break;
			case LONG:
				$$.type = DOUBLE;
				$$.value.dvalue =
					$1.value.dvalue - $3.value.lvalue;
				break;
			case DOUBLE:
				$$.type = DOUBLE;
				$$.value.dvalue =
					$1.value.dvalue - $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	}
	;

mult:
	cast
	{
		$$ = $1;
	}
	| mult STAR cast
	{
		switch ($1.type)
		{
		case ULONG:
			switch ($3.type)
			{
			case ULONG:
				$$.type = ULONG;
				$$.value.ulvalue =
					$1.value.ulvalue * $3.value.ulvalue;
				break;
			case LONG:
				$$.type = ULONG;
				$$.value.ulvalue =
					$1.value.ulvalue * $3.value.lvalue;
				break;
			case DOUBLE:
				$$.type = DOUBLE;
				$$.value.dvalue =
					$1.value.ulvalue * $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch ($3.type)
			{
			case ULONG:
				$$.type = ULONG;
				$$.value.ulvalue =
					$1.value.lvalue * $3.value.ulvalue;
				break;
			case LONG:
				$$.type = LONG;
				$$.value.lvalue =
					$1.value.lvalue * $3.value.lvalue;
				break;
			case DOUBLE:
				$$.type = DOUBLE;
				$$.value.dvalue =
					$1.value.lvalue * $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch ($3.type)
			{
			case ULONG:
				$$.type = DOUBLE;
				$$.value.dvalue =
					$1.value.dvalue * $3.value.ulvalue;
				break;
			case LONG:
				$$.type = DOUBLE;
				$$.value.dvalue =
					$1.value.dvalue * $3.value.lvalue;
				break;
			case DOUBLE:
				$$.type = DOUBLE;
				$$.value.dvalue =
					$1.value.dvalue * $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	}
	| mult SLASH cast
	{
		switch ($1.type)
		{
		case ULONG:
			switch ($3.type)
			{
			case ULONG:
				$$.type = ULONG;
				if ($3.value.ulvalue == 0)
				{
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				$$.value.ulvalue =
					$1.value.ulvalue / $3.value.ulvalue;
				break;
			case LONG:
				$$.type = ULONG;
				if ($3.value.lvalue == 0)
				{
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				$$.value.ulvalue =
					$1.value.ulvalue / $3.value.lvalue;
				break;
			case DOUBLE:
				$$.type = DOUBLE;
				if ($3.value.dvalue == 0)
				{
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				$$.value.dvalue =
					$1.value.ulvalue / $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch ($3.type)
			{
			case ULONG:
				$$.type = ULONG;
				if ($3.value.ulvalue == 0)
				{
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				$$.value.ulvalue =
					$1.value.lvalue / $3.value.ulvalue;
				break;
			case LONG:
				$$.type = LONG;
				if ($3.value.lvalue == 0)
				{
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				$$.value.lvalue =
					$1.value.lvalue / $3.value.lvalue;
				break;
			case DOUBLE:
				$$.type = DOUBLE;
				if ($3.value.dvalue == 0)
				{
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				$$.value.dvalue =
					$1.value.lvalue / $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch ($3.type)
			{
			case ULONG:
				$$.type = DOUBLE;
				if ($3.value.ulvalue == 0)
				{
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				$$.value.dvalue =
					$1.value.dvalue / $3.value.ulvalue;
				break;
			case LONG:
				$$.type = DOUBLE;
				if ($3.value.lvalue == 0)
				{
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				$$.value.dvalue =
					$1.value.dvalue / $3.value.lvalue;
				break;
			case DOUBLE:
				$$.type = DOUBLE;
				if ($3.value.dvalue == 0)
				{
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				$$.value.dvalue =
					$1.value.dvalue / $3.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	}
	| mult PERCENT cast
	{
		switch ($1.type)
		{
		case ULONG:
			switch ($3.type)
			{
			case ULONG:
				$$.type = ULONG;
				if ($3.value.ulvalue == 0)
				{
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				$$.value.ulvalue =
					$1.value.ulvalue % $3.value.ulvalue;
				break;
			case LONG:
				$$.type = ULONG;
				if ($3.value.lvalue == 0)
				{
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				$$.value.ulvalue =
					$1.value.ulvalue % $3.value.lvalue;
				break;
			case DOUBLE:
				yyerror((const char *)"% cannot be applied to double.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch ($3.type)
			{
			case ULONG:
				$$.type = ULONG;
				if ($3.value.ulvalue == 0)
				{
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				$$.value.ulvalue =
					$1.value.lvalue % $3.value.ulvalue;
				break;
			case LONG:
				$$.type = LONG;
				if ($3.value.lvalue == 0)
				{
					mybcerror((const char *)"divide by zero.", __LINE__);
					YYERROR;
				}
				$$.value.lvalue =
					$1.value.lvalue % $3.value.lvalue;
				break;
			case DOUBLE:
				yyerror((const char *)"% cannot be applied to double.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror((const char *)"% cannot be applied to double.");
			YYERROR;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	}
	;

cast:
	unary
	{
		$$ = $1;
	}
	| LPAREN type RPAREN cast
	{
		switch ($2)
		{
		case ULONG:
			switch ($4.type)
			{
			case ULONG:
				$$ = $4;
				break;
			case LONG:
				$$.type = ULONG;
				$$.value.ulvalue = 
					(unsigned long)$4.value.lvalue;
				break;
			case DOUBLE:
				$$.type = ULONG;
				$$.value.ulvalue = 
					(unsigned long)$4.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case LONG:
			switch ($4.type)
			{
			case LONG:
				$$ = $4;
				break;
			case ULONG:
				$$.type = LONG;
				$$.value.lvalue = (long)$4.value.ulvalue;
				break;
			case DOUBLE:
				$$.type = LONG;
				$$.value.lvalue = (long)$4.value.dvalue;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			switch ($4.type)
			{
			case ULONG:
				$$.type = DOUBLE;
				$$.value.dvalue = (double)$4.value.ulvalue;
				break;
			case LONG:
				$$.type = DOUBLE;
				$$.value.dvalue = (double)$4.value.lvalue;
				break;
			case DOUBLE:
				$$ = $4;
				break;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
	}
	;

unary:
	primary
	{
		$$ = $1;
	}
	| PLUS cast
	{
		$$ = $2;
	}
	| MINUS cast
	{
		switch ($2.type)
		{
		case ULONG:
			$2.value.ulvalue *= -1;
			break;
		case DOUBLE:
			$2.value.dvalue *= -1.0;
			break;
		case LONG:
			$2.value.lvalue *= -1;
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		$$ = $2;
	}
	| TILDE cast
	{
		switch ($2.type)
		{
		case ULONG:
			$2.value.ulvalue = ~($2.value.ulvalue);
			break;
		case DOUBLE:
			yyerror((const char *)"~ cannot be applied to double.");
			YYERROR;
		case LONG:
			$2.value.lvalue = ~($2.value.lvalue);
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		$$ = $2;
	}
	| NOT cast
	{
		switch ($2.type)
		{
		case ULONG:
			$2.value.ulvalue = !($2.value.ulvalue);
			break;
		case DOUBLE:
			$2.value.dvalue = !($2.value.dvalue);
			break;
		case LONG:
			$2.value.lvalue = !($2.value.lvalue);
			break;
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		$$ = $2;
	}
	;

primary:
	const
	{
		$$ = $1;
	}
	| EXP LPAREN expr RPAREN
	{
		$3 = number2double($3);
		if ($3.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		$$.type = DOUBLE;
		$$.value.dvalue = exp($3.value.dvalue);
	}
	| LOG LPAREN expr RPAREN
	{
		$3 = number2double($3);
		if ($3.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		if ($3.value.dvalue < 0)
		{
			mybcerror((const char *)"log of negative number.", __LINE__);
			YYERROR;
		}
		$$.type = DOUBLE;
		$$.value.dvalue = log($3.value.dvalue);
	}
	| LOG10 LPAREN expr RPAREN
	{
		$3 = number2double($3);
		if ($3.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		if ($3.value.dvalue < 0)
		{
			mybcerror((const char *)"log10 of negative number.", __LINE__);
			YYERROR;
		}
		$$.type = DOUBLE;
		$$.value.dvalue = log10($3.value.dvalue);
	}
	| POW LPAREN expr COMMA expr RPAREN
	{
		$3 = number2double($3);
		if ($3.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		$5 = number2double($5);
		if ($5.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		$$.type = DOUBLE;
		$$.value.dvalue = pow($3.value.dvalue, $5.value.dvalue);
	}
	| SQRT LPAREN expr RPAREN
	{
		$3 = number2double($3);
		if ($3.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		$$.type = DOUBLE;
		$$.value.dvalue = sqrt($3.value.dvalue);
	}
	| SIN LPAREN expr RPAREN
	{
		$3 = number2double($3);
		if ($3.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		$$.type = DOUBLE;
		$$.value.dvalue = sin($3.value.dvalue);
	}
	| COS LPAREN expr RPAREN
	{
		$3 = number2double($3);
		if ($3.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		$$.type = DOUBLE;
		$$.value.dvalue = cos($3.value.dvalue);
	}
	| TAN LPAREN expr RPAREN
	{
		$3 = number2double($3);
		if ($3.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		$$.type = DOUBLE;
		$$.value.dvalue = tan($3.value.dvalue);
	}
	| ASIN LPAREN expr RPAREN
	{
		$3 = number2double($3);
		if ($3.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		$$.type = DOUBLE;
		$$.value.dvalue = asin($3.value.dvalue);
	}
	| ACOS LPAREN expr RPAREN
	{
		$3 = number2double($3);
		if ($3.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		$$.type = DOUBLE;
		$$.value.dvalue = acos($3.value.dvalue);
	}
	| ATAN LPAREN expr RPAREN
	{
		$3 = number2double($3);
		if ($3.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		$$.type = DOUBLE;
		$$.value.dvalue = atan($3.value.dvalue);
	}
	| LPAREN expr RPAREN
	{
		$$ = $2;
	}
	;

const:
	ULCONST 
	{
		$$ = $1;
	}
	| LCONST 
	{
		$$ = $1;
	}
	| DCONST
	{
		$$ = $1;
	}
	;

type:
	ULCAST
	{
		$$ = ULONG;
	}
	| LCAST
	{
		$$ = LONG;
	}
	| DCAST
	{
		$$ = DOUBLE;
	}
	;

%%

void
initgraph()
{
	xyno = 0;
}

void
storexy(VALUE x, VALUE y)
{
	if (xyno < NUMBEROFPOINTS)
	{
		xy[xyno][0] = x.value.dvalue;
		xy[xyno][1] = y.value.dvalue;
		xyno++;
	}
	else
	{
		fprintf(stderr, "ERROR: no room for storing (x,y) point.\n");
	}
	return;
}

void
graph()
{
	int ixy, ix, iy, px, py;
	char xymap[XPIXNUM][YPIXNUM+1];
	double dx, dy, xmin, ymin, xmax, ymax;
	int ixmin, iymin, ixmax, iymax;

	/* dump data for now */
	for (ixy = 0; ixy < xyno; ixy++)
	{
		printf("%03d: (%e, %e)\n", ixy, xy[ixy][0], xy[ixy][1]);
	}

	/* initialize graph */	
	for (ix = 0; ix < XPIXNUM; ix++)
	{
		for (iy = 0; iy < YPIXNUM; iy++)
		{
			xymap[ix][iy] = ' ';
		}
		xymap[ix][iy] = 0;
	}

	/* get min and max */
	xmin = xy[0][0];
	ymin = xy[0][1];
	xmax = xy[0][0];
	ymax = xy[0][1];
	for (ix = 1; ix < xyno; ix++)
	{
		if (xmin > xy[ix][0])
			xmin = xy[ix][0];
		if (ymin > xy[ix][1])
			ymin = xy[ix][1];
		if (xmax < xy[ix][0])
			xmax = xy[ix][0];
		if (ymax < xy[ix][1])
			ymax = xy[ix][1];
	}

	/* calculate the deltas */
	dx = (xmax - xmin)/((double)XPIXNUM);
	dy = (ymax - ymin)/((double)YPIXNUM);

	/* plot points in map */
	for (ix = 0; ix < xyno; ix++)
	{
		px = (xy[ix][0] - xmin)/dx;
		py = (xy[ix][1] - ymin)/dy;
		if (px < 0) px = 0;
		if (px >= XPIXNUM) px = XPIXNUM-1;
		if (py < 0) py = 0;
		if (py >= YPIXNUM) py = YPIXNUM-1;
		xymap[px][py] = '*';
	}

	/* print graph */
	fprintf(stdout, "\n");
	for (iy = 0; iy < (YPIXNUM+5); iy++)
	{
		fprintf(stdout, "-");
	}
	fprintf(stdout, "\n");
	for (ix = 0; ix < XPIXNUM; ix++)
	{
		fprintf(stdout, "%03d| %s\n", ix, &xymap[ix][0]);
	}
	for (iy = 0; iy < (YPIXNUM+5); iy++)
	{
		fprintf(stdout, "-");
	}
	fprintf(stdout, "\n");

	/* all done */
	return;
}

VALUE
number2double(VALUE number)
{
	switch (number.type)
	{
	case LONG:
		number.type = DOUBLE;
		number.value.dvalue = (double)number.value.lvalue;
		break;
	case ULONG:
		number.type = DOUBLE;
		number.value.dvalue = (double)number.value.ulvalue;
		break;
	case DOUBLE:
		break;
	default:
		number.type = UNKNOWN;
		break;
	}
	return(number);
}

char *
num2base(unsigned long base, int issigned, unsigned long number)
{
	long ic, sign, quotient, remainder;
	static char buf[64];

	if (issigned == YES)
	{
		if ((long)number < 0)
		{
			sign = -1;
			number = -1*(long)number;
		}
		else
		{
			sign = 1;
		}
	}
	ic = sizeof(buf);
	buf[--ic] = 0;
	do {
		quotient = number/base;
		remainder = number % base;
		if (remainder < 10)
		{
			buf[--ic] = '0' + remainder;
		}
		else
		{
			buf[--ic] = 'A' + remainder - 10;
		}
	} while ((number = quotient) != 0);
	if (issigned == YES && sign == -1)
	{
		buf[--ic] = '-';
	}
	return(buf+ic);
}

#ifdef SUN
void
yyerror(const char *s)
{
	fprintf(stderr, "%s\n", s);
}
#endif

void
mybcerror(const char *msg, int lnno)
{
	char buf[BUFSIZ];
	sprintf(buf, "%d: %s", lnno, msg);
	yyerror((const char *)buf);
	return;
}

main(int argc, char **argv)
{
#ifdef YYDEBUG
	extern int yydebug;
#endif
	extern FILE *yyin;
	char inbuf[10000];	/* it works, so don't complain !!! */
	FILE fakefd;

	/* check if used in command line mode */
#ifdef YYDEBUG
	yydebug = 1;
#endif
	if (argc > 1)
	{
		/* concatenate the arguments into one string */
		int arg = 1;
		char *pib = inbuf;
		for (*pib = 0; arg < argc; arg++)
		{
			char *parg = argv[arg];
			for ( ; *parg != 0; parg++, pib++)
			{
				*pib = *parg;
			}
			*pib++ = ' ';
		}

		/* add quit to command line */
		*pib++ = ';';
		*pib++ = 'q';
		*pib++ = ';';
		*pib = 0;

		/* set up a fake FILE structure */
		fakefd._flag = _IOREAD ;
		fakefd._ptr = fakefd._base = (unsigned char *)inbuf;
		fakefd._cnt = pib - inbuf;
		fakefd._file = 0;

		/* set yyin to pointer to fake FILE structure */
		yyin = &fakefd;
	}

	/* call parser */
	yyparse();

	exit(0);
}
