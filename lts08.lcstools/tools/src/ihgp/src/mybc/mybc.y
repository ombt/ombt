%{
/* required headers */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <values.h>

/* other headers */
#include "mybc.h"

/* globals */
long outbase = 10;
double xy[NUMBEROFPOINTS][2];
int xyno;
Complex<double> result(0.0,0.0);
double deps = 0.0;

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

%token <number> CCONST
%token <number> DCONST
%token <number> LCONST
%token <number> ULCONST
%token <string> ABS
%token <string> ACOS
%token <string> ACOSH
%token <string> AND
%token <string> ARG
%token <string> ASIN
%token <string> ASINH
%token <string> ATAN
%token <string> ATANH
%token <string> BITAND
%token <string> BITOR
%token <string> BITXOR
%token <string> COMB
%token <string> COMMA
%token <string> COS
%token <string> COSH
%token <string> COT
%token <string> CSC
%token <string> EQUAL
%token <string> EXP
%token <string> FACT
%token <string> GE
%token <string> GRAPH
%token <string> GT
%token <string> HELP
%token <string> IMAG
%token <string> LE
%token <string> LOG
%token <string> LOG10
%token <string> LPAREN
%token <string> LBRACKET
%token <string> LSHIFT
%token <string> LT
%token <string> MINUS
%token <string> NEWLINE
%token <string> NORM
%token <string> NOT
%token <string> NOTEQUAL
%token <string> OR
%token <string> OUTBASE
%token <string> PERCENT
%token <string> PERM
%token <string> PLUS
%token <string> POW
%token <string> QUIT
%token <string> REAL
%token <string> RPAREN
%token <string> RBRACKET
%token <string> RSHIFT
%token <string> SEMICOLON
%token <string> SIN
%token <string> SINH
%token <string> SLASH
%token <string> SQRT
%token <string> SSEC
%token <string> STAR
%token <string> STRING
%token <string> TAN
%token <string> TANH
%token <string> TILDE
%token <string> USURY
%token <type> CCAST
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
			printf("%s\n", num2base(outbase, NO, $2.value.ulvalue));
			break;
		case LONG:
			printf("%s\n", num2base(outbase, YES, $2.value.lvalue));
			break;
		case DOUBLE:
			switch (outbase)
			{
			case 2:
			case 8:
			case 16:
				printf("base = 10 for double.\n");
			case 10:
				printf("%le\n", $2.value.dvalue);
				break;
			default:
				printf("%d: default case.\n", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch (outbase)
			{
			case 2:
			case 8:
			case 16:
				printf("base = 10 for complex.\n");
			case 10:
				printf("[%le,%le]\n", 
					$2.value.cvalue.r, $2.value.cvalue.i);
				break;
			default:
				printf("%d: default case.\n", __LINE__);
				YYERROR;
			}
			break;
		case VOID:
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
		printf("mybc supports the following operators and math ");
		printf("functions.\n");
		printf("\n");
		printf("operators:\n");
		printf("|| && | ^ & == != < > <= >= << >> - * / ");
		printf("%% ( ) ~ !\n");
		printf("\n");
		printf("math functions:\n");
		printf("exp log log10 pow sqrt\n");
		printf("sin cos tan asin acos atan\n");
		printf("sinh cosh tanh asinh acosh atanh\n");
		printf("abs arg norm real imag\n");
		printf("\n");
		printf("counting functions:\n");
		printf("C(ombinations), P(ermutations), fact(orial)\n");
		printf("\n");
		printf("math constants:\n");
		printf("pi, e, i\n");
		printf("\n");
		printf("finance functions:\n");
		printf("usury(principal, %% interest, loan period in years)\n");
		printf("\n");
		printf("miscellaneous:\n");
		printf("(l) - long typecast\n");
		printf("(ul) - unsigned long typecast\n");
		printf("(d) - double typecast\n");
		printf("(c) - complex typecast\n");
		printf("[xh][0-9a-z]* - hex numbers\n");
		printf("o[0-7]* - octal numbers\n");
		printf("b[01]* - binary numbers\n");
		printf("'c' - ascii character\n");
		printf("[+|-]?[0-9]+.[0-9]+([eE][+-]?[0-9]+)? - real numbers\n");
		printf("[real,real] - complex numbers\n");
		printf("ob [2|8|10|16] - set output base\n");
		printf("? - help\n");
		strcpy($$, $1);
	}
	| HELP USURY
	{
		printf("usage: usury(p, i, lp)\n");
		printf("where p = principal, i = %% interest, and "); 
		printf("lp = loan period in years.\n");
	}
	| HELP ABS
	{
		printf("usage: abs(expr)\n");
	}
	| HELP ARG
	{
		printf("usage: arg(expr)\n");
	}
	| HELP NORM
	{
		printf("usage: norm(expr)\n");
	}
	| HELP REAL
	{
		printf("usage: real(expr)\n");
	}
	| HELP IMAG
	{
		printf("usage: imag(expr)\n");
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
	| HELP CSC
	{
		printf("usage: csc(expr)\n");
	}
	| HELP SSEC
	{
		printf("usage: sec(expr)\n");
	}
	| HELP COT
	{
		printf("usage: cot(expr)\n");
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
	| HELP SINH
	{
		printf("usage: sinh(expr)\n");
	}
	| HELP COSH
	{
		printf("usage: cosh(expr)\n");
	}
	| HELP TANH
	{
		printf("usage: tanh(expr)\n");
	}
	| HELP ASINH
	{
		printf("usage: asinh(expr)\n");
	}
	| HELP ACOSH
	{
		printf("usage: acosh(expr)\n");
	}
	| HELP ATANH
	{
		printf("usage: atanh(expr)\n");
	}
	| HELP OUTBASE
	{
		printf("usage: ob [2|8|10|16]\n");
	}
	| HELP COMB
	{
		printf("usage: C(n, r)\n");
	}
	| HELP PERM
	{
		printf("usage: P(n, r)\n");
	}
	| HELP FACT
	{
		printf("usage: fact(n)\n");
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)$1.value.ulvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 || c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex || failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)$1.value.lvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 || c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex || failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1($1.value.dvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 || c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex || failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch ($3.type)
			{
			case ULONG:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.ulvalue);
				try {
					$$.value.lvalue = (c1 || c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex || failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case LONG:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.lvalue);
				try {
					$$.value.lvalue = (c1 || c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex || failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case DOUBLE:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3($3.value.dvalue);
				try {
					$$.value.lvalue = (c1 || c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex || failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case COMPLEX:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 || c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex || failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)$1.value.ulvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 && c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex && failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)$1.value.lvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 && c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex && failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1($1.value.dvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 && c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex && failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch ($3.type)
			{
			case ULONG:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.ulvalue);
				try {
					$$.value.lvalue = (c1 && c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex && failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case LONG:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.lvalue);
				try {
					$$.value.lvalue = (c1 && c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex && failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case DOUBLE:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3($3.value.dvalue);
				try {
					$$.value.lvalue = (c1 && c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex && failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case COMPLEX:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 && c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex && failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
				yyerror((const char *)"| cannot be applied to complex.");
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
			case COMPLEX:
				yyerror((const char *)"| cannot be applied to complex.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror((const char *)"| cannot be applied to double.");
			YYERROR;
		case COMPLEX:
			yyerror((const char *)"| cannot be applied to complex.");
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
			case COMPLEX:
				yyerror((const char *)"^ cannot be applied to complex.");
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
			case COMPLEX:
				yyerror((const char *)"^ cannot be applied to complex.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror((const char *)"^ cannot be applied to double.");
			YYERROR;
		case COMPLEX:
			yyerror((const char *)"^ cannot be applied to complex.");
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
			case COMPLEX:
				yyerror((const char *)"& cannot be applied to complex.");
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
			case COMPLEX:
				yyerror((const char *)"& cannot be applied to complex.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror((const char *)"& cannot be applied to double.");
			YYERROR;
		case COMPLEX:
			yyerror((const char *)"& cannot be applied to complex.");
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)$1.value.ulvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 == c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex == failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)$1.value.lvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 == c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex == failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1($1.value.dvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 == c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex == failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch ($3.type)
			{
			case ULONG:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.ulvalue);
				try {
					$$.value.lvalue = (c1 == c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex == failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case LONG:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.lvalue);
				try {
					$$.value.lvalue = (c1 == c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex == failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case DOUBLE:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3($3.value.dvalue);
				try {
					$$.value.lvalue = (c1 == c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex == failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case COMPLEX:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 == c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex == failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)$1.value.ulvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 != c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex != failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)$1.value.lvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 != c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex != failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1($1.value.dvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 != c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex != failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch ($3.type)
			{
			case ULONG:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.ulvalue);
				try {
					$$.value.lvalue = (c1 != c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex != failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case LONG:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.lvalue);
				try {
					$$.value.lvalue = (c1 != c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex != failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case DOUBLE:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3($3.value.dvalue);
				try {
					$$.value.lvalue = (c1 != c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex != failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case COMPLEX:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 != c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex != failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)$1.value.ulvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 < c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex < failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)$1.value.lvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 < c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex < failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1($1.value.dvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 < c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex < failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch ($3.type)
			{
			case ULONG:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.ulvalue);
				try {
					$$.value.lvalue = (c1 < c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex < failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case LONG:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.lvalue);
				try {
					$$.value.lvalue = (c1 < c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex < failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case DOUBLE:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3($3.value.dvalue);
				try {
					$$.value.lvalue = (c1 < c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex < failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case COMPLEX:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 < c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex < failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)$1.value.ulvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 > c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex > failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)$1.value.lvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 > c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex > failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1($1.value.dvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 > c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex > failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch ($3.type)
			{
			case ULONG:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.ulvalue);
				try {
					$$.value.lvalue = (c1 > c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex > failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case LONG:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.lvalue);
				try {
					$$.value.lvalue = (c1 > c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex > failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case DOUBLE:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3($3.value.dvalue);
				try {
					$$.value.lvalue = (c1 > c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex > failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case COMPLEX:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 > c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex > failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)$1.value.ulvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 <= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex <= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)$1.value.lvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 <= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex <= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)$1.value.dvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 <= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex <= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch ($3.type)
			{
			case ULONG:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.ulvalue);
				try {
					$$.value.lvalue = (c1 <= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex <= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case LONG:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.lvalue);
				try {
					$$.value.lvalue = (c1 <= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex <= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case DOUBLE:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.dvalue);
				try {
					$$.value.lvalue = (c1 <= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex <= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case COMPLEX:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 <= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex <= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)$1.value.ulvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 >= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex >= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)$1.value.lvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 >= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex >= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
			{
				Complex<double> 
					c1((double)$1.value.dvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 >= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex >= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch ($3.type)
			{
			case ULONG:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.ulvalue);
				try {
					$$.value.lvalue = (c1 >= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex >= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case LONG:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.lvalue);
				try {
					$$.value.lvalue = (c1 >= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex >= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case DOUBLE:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.dvalue);
				try {
					$$.value.lvalue = (c1 >= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex >= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
			case COMPLEX:
			{
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					$$.value.lvalue = (c1 >= c3);
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex >= failed.", __LINE__);
					YYERROR;
				}
				break;
			}
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
			case COMPLEX:
				yyerror((const char *)"<< cannot be applied to complex.");
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
			case COMPLEX:
				yyerror((const char *)"<< cannot be applied to complex.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror((const char *)"<< cannot be applied to double.");
			YYERROR;
		case COMPLEX:
			yyerror((const char *)"<< cannot be applied to complex.");
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
			case COMPLEX:
				yyerror((const char *)">> cannot be applied to complex.");
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
			case COMPLEX:
				yyerror((const char *)">> cannot be applied to complex.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror((const char *)">> cannot be applied to double.");
			YYERROR;
		case COMPLEX:
			yyerror((const char *)">> cannot be applied to complex.");
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
			case COMPLEX:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1((double)$1.value.ulvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					result = c1+c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex addition failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
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
			case COMPLEX:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1((double)$1.value.lvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					result = c1+c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex addition failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
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
			case COMPLEX:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1($1.value.dvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					result = c1+c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex addition failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch ($3.type)
			{
			case ULONG:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.ulvalue);
				try {
					result = c1+c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex addition failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
			case LONG:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.lvalue);
				try {
					result = c1+c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex addition failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
			case DOUBLE:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3($3.value.dvalue);
				try {
					result = c1+c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex addition failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
			case COMPLEX:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					result = c1+c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex addition failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
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
			case COMPLEX:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1((double)$1.value.ulvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					result = c1-c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex subtraction failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
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
			case COMPLEX:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1((double)$1.value.lvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					result = c1-c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex subtraction failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
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
			case COMPLEX:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1($1.value.dvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					result = c1-c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex subtraction failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch ($3.type)
			{
			case ULONG:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.ulvalue);
				try {
					result = c1-c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex subtraction failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
			case LONG:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.lvalue);
				try {
					result = c1-c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex subtraction failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
			case DOUBLE:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3($3.value.dvalue);
				try {
					result = c1-c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex subtraction failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
			case COMPLEX:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					result = c1-c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex subtraction failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
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
			case COMPLEX:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1((double)$1.value.ulvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					result = c1*c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex multiply failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
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
			case COMPLEX:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1((double)$1.value.lvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					result = c1*c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex multiply failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
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
			case COMPLEX:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1($1.value.dvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					result = c1*c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex multiply failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch ($3.type)
			{
			case ULONG:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.ulvalue);
				try {
					result = c1*c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex multiply failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
			case LONG:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.lvalue);
				try {
					result = c1*c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex multiply failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
			case DOUBLE:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3($3.value.dvalue);
				try {
					result = c1*c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex multiply failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
			case COMPLEX:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					result = c1*c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex multiply failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
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
			case COMPLEX:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1((double)$1.value.ulvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					result = c1/c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex divide failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
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
			case COMPLEX:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1((double)$1.value.lvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					result = c1/c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex divide failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
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
			case COMPLEX:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1($1.value.dvalue);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					result = c1/c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex divide failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch ($3.type)
			{
			case ULONG:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.ulvalue);
				try {
					result = c1/c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex divide failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
			case LONG:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3((double)$3.value.lvalue);
				try {
					result = c1/c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex divide failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
			case DOUBLE:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3($3.value.dvalue);
				try {
					result = c1/c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex divide failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
			case COMPLEX:
			{
				$$.type = COMPLEX;
				Complex<double> 
					c1($1.value.cvalue.r,$1.value.cvalue.i);
				Complex<double> 
					c3($3.value.cvalue.r,$3.value.cvalue.i);
				try {
					result = c1/c3;
				}
				catch (const char *pe)
				{
					mybcerror((const char *)"complex divide failed.", __LINE__);
					YYERROR;
				}
				$$.value.cvalue.r = real(result);
				$$.value.cvalue.i = imag(result);
				break;
			}
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
			case COMPLEX:
				yyerror((const char *)"% cannot be applied to complex.");
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
			case COMPLEX:
				yyerror((const char *)"% cannot be applied to complex.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case DOUBLE:
			yyerror((const char *)"% cannot be applied to double.");
			YYERROR;
		case COMPLEX:
			yyerror((const char *)"% cannot be applied to complex.");
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
			case COMPLEX:
				yyerror((const char *)"cannot typecast complex.");
				YYERROR;
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
			case COMPLEX:
				yyerror((const char *)"cannot typecast complex.");
				YYERROR;
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
			case COMPLEX:
				yyerror((const char *)"cannot typecast complex.");
				YYERROR;
			default:
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			break;
		case COMPLEX:
			switch ($4.type)
			{
			case ULONG:
				$$.type = COMPLEX;
				$$.value.cvalue.r = (double)$4.value.ulvalue;
				$$.value.cvalue.i = 0;
				break;
			case LONG:
				$$.type = COMPLEX;
				$$.value.cvalue.r = (double)$4.value.lvalue;
				$$.value.cvalue.i = 0;
				break;
			case DOUBLE:
				$$.type = COMPLEX;
				$$.value.cvalue.r = $4.value.dvalue;
				$$.value.cvalue.i = 0;
				break;
			case COMPLEX:
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
		case COMPLEX:
			$2.value.cvalue.r *= -1.0;
			$2.value.cvalue.i *= -1.0;
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
		case COMPLEX:
			yyerror((const char *)"~ cannot be applied to complex.");
			YYERROR;
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
		case COMPLEX:
			if ($2.value.cvalue.r != 0 || $2.value.cvalue.i != 0)
			{
				$2.value.cvalue.r = 0;
				$2.value.cvalue.i = 0;
			}
			else
			{
				$2.value.cvalue.r = 1;
				$2.value.cvalue.i = 0;
			}
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
		if ($3.type != COMPLEX)
		{
			$3 = number2double($3);
			if ($3.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			$$.type = DOUBLE;
			$$.value.dvalue = exp($3.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex($3);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = exp(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex exp failed.");
				YYERROR;
			}
			$$.type = COMPLEX;
			$$.value.cvalue.r = real(c2);
			$$.value.cvalue.i = imag(c2);
		}
	}
	| LOG LPAREN expr RPAREN
	{
		if ($3.type != COMPLEX)
		{
			$3 = number2double($3);
			if ($3.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			if ($3.value.dvalue < 0)
			{
				mybcerror((const char *)"log of negative number.", 
						__LINE__);
				YYERROR;
			}
			$$.type = DOUBLE;
			$$.value.dvalue = log($3.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex($3);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = log(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex log failed.");
				YYERROR;
			}
			$$.type = COMPLEX;
			$$.value.cvalue.r = real(c2);
			$$.value.cvalue.i = imag(c2);
		}
	}
	| LOG10 LPAREN expr RPAREN
	{
		if ($3.type != COMPLEX)
		{
			$3 = number2double($3);
			if ($3.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			if ($3.value.dvalue < 0)
			{
				mybcerror((const char *)"log10 of negative number.", 
					__LINE__);
				YYERROR;
			}
			$$.type = DOUBLE;
			$$.value.dvalue = log10($3.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex($3);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = log10(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex log10 failed.");
				YYERROR;
			}
			$$.type = COMPLEX;
			$$.value.cvalue.r = real(c2);
			$$.value.cvalue.i = imag(c2);
		}
	}
	| POW LPAREN expr COMMA expr RPAREN
	{
		if ($3.type != COMPLEX)
		{
			$3 = number2double($3);
			if ($3.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			if ($3.value.dvalue < 0)
			{
				mybcerror((const char *)"log of negative number.", 
						__LINE__);
				YYERROR;
			}
			$$.type = DOUBLE;
			$$.value.dvalue = log($3.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex($3);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = log(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex log failed.");
				YYERROR;
			}
			$$.type = COMPLEX;
			$$.value.cvalue.r = real(c2);
			$$.value.cvalue.i = imag(c2);
		}
		if ($3.type != COMPLEX && $5.type != COMPLEX)
		{
			$3 = number2double($3);
			if ($3.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			$5 = number2double($5);
			if ($5.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			$$.type = DOUBLE;
			$$.value.dvalue = pow($3.value.dvalue, $5.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex($3);
			Complex<double> c2 = number2complex($5);
			Complex<double> c3(0.0, 0.0);
			try {
				c3 = pow(c1, c2);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex pow failed.");
				YYERROR;
			}
			$$.type = COMPLEX;
			$$.value.cvalue.r = real(c3);
			$$.value.cvalue.i = imag(c3);
		}
	}
	| COMB LPAREN expr COMMA expr RPAREN
	{
		if ($3.type == COMPLEX || $5.type == COMPLEX)
		{
			mybcerror((const char *)"C(n,r) not supported for complex.", __LINE__);
			YYERROR;
		}
		if (($3.type == ULONG || $3.type == LONG) &&
		    ($5.type == ULONG || $5.type == LONG))
		{
			if ($3.type == LONG)
			{
				if ($3.value.lvalue < 0)
				{
					mybcerror((const char *)"n < 0 is not allowed.", __LINE__);
					YYERROR;
				}
				$3.type = ULONG;
				$3.value.ulvalue = $3.value.lvalue;
			}
			if ($5.type == LONG)
			{
				if ($5.value.lvalue < 0)
				{
					mybcerror((const char *)"r < 0 is not allowed.", __LINE__);
					YYERROR;
				}
				$5.type = ULONG;
				$5.value.ulvalue = $5.value.lvalue;
			}
			if ($3.value.ulvalue < $5.value.ulvalue)
			{
				mybcerror((const char *)"n < r not allowed.", __LINE__);
				YYERROR;
			}
			$$.type = ULONG;
			$$.value.ulvalue = comb($3.value.ulvalue, $5.value.ulvalue);
		}
		else
		{
			$3 = number2double($3);
			if ($3.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			$5 = number2double($5);
			if ($5.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			if ($3.value.dvalue < $5.value.dvalue)
			{
				mybcerror((const char *)"n < r not allowed.", __LINE__);
				YYERROR;
			}
			$$.type = DOUBLE;
			$$.value.dvalue = comb($3.value.dvalue, $5.value.dvalue);
		}
	}
	| PERM LPAREN expr COMMA expr RPAREN
	{
		if ($3.type == COMPLEX || $5.type == COMPLEX)
		{
			mybcerror((const char *)"P(n,r) not supported for complex.", __LINE__);
			YYERROR;
		}
		if (($3.type == ULONG || $3.type == LONG) &&
		    ($5.type == ULONG || $5.type == LONG))
		{
			if ($3.type == LONG)
			{
				if ($3.value.lvalue < 0)
				{
					mybcerror((const char *)"n < 0 is not allowed.", __LINE__);
					YYERROR;
				}
				$3.type = ULONG;
				$3.value.ulvalue = $3.value.lvalue;
			}
			if ($5.type == LONG)
			{
				if ($5.value.lvalue < 0)
				{
					mybcerror((const char *)"r < 0 is not allowed.", __LINE__);
					YYERROR;
				}
				$5.type = ULONG;
				$5.value.ulvalue = $5.value.lvalue;
			}
			if ($3.value.ulvalue < $5.value.ulvalue)
			{
				mybcerror((const char *)"n < r not allowed.", __LINE__);
				YYERROR;
			}
			$$.type = ULONG;
			$$.value.ulvalue = perm($3.value.ulvalue, $5.value.ulvalue);
		}
		else
		{
			$3 = number2double($3);
			if ($3.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			$5 = number2double($5);
			if ($5.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			if ($3.value.dvalue < $5.value.dvalue)
			{
				mybcerror((const char *)"n < r not allowed.", __LINE__);
				YYERROR;
			}
			$$.type = DOUBLE;
			$$.value.dvalue = perm($3.value.dvalue, $5.value.dvalue);
		}
	}
	| USURY LPAREN expr COMMA expr COMMA expr RPAREN
	{
		if ($3.type == COMPLEX || $5.type == COMPLEX || $7.type == COMPLEX)
		{
			mybcerror((const char *)"usury(p,i,lp) not supported for complex.", __LINE__);
			YYERROR;
		}
		$3 = number2double($3);
		if ($3.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", 
					__LINE__);
			YYERROR;
		}
		$5 = number2double($5);
		if ($5.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", 
					__LINE__);
			YYERROR;
		}
		$7 = number2double($7);
		if ($7.type != DOUBLE)
		{
			mybcerror((const char *)"default case.", 
					__LINE__);
			YYERROR;
		}
		try {
			$$.type = VOID;
			MustBeTrue($3.value.dvalue > 0.0);
			MustBeTrue($5.value.dvalue > 0.0);
			MustBeTrue($7.value.dvalue > 0.0);
			usury($3.value.dvalue, $5.value.dvalue, 
				$7.value.dvalue);
		}
		catch (const char *pe)
		{
			yyerror((const char *)"usury failed.");
			YYERROR;
		}
	}
	| FACT LPAREN expr RPAREN
	{
		if ($3.type == COMPLEX)
		{
			mybcerror((const char *)"fact(n) not supported for complex.", __LINE__);
			YYERROR;
		}
		if ($3.type == ULONG || $3.type == LONG)
		{
			if ($3.type == LONG)
			{
				if ($3.value.lvalue < 0)
				{
					mybcerror((const char *)"n < 0 is not allowed.", __LINE__);
					YYERROR;
				}
				$3.type = ULONG;
				$3.value.ulvalue = $3.value.lvalue;
			}
			if ($3.value.ulvalue <= 12)
			{
				/* ulong overflows with n > 12 */
				$$.type = ULONG;
				$$.value.ulvalue = fact($3.value.ulvalue);
			}
			else
			{
				/* ulong overflows with n > 12 */
				$3 = number2double($3);
				if ($3.type != DOUBLE)
				{
					mybcerror((const char *)"default case.", 
							__LINE__);
					YYERROR;
				}
				$$.type = DOUBLE;
				$$.value.dvalue = fact($3.value.dvalue);
			}
		}
		else
		{
			$3 = number2double($3);
			if ($3.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			$$.type = DOUBLE;
			$$.value.dvalue = fact($3.value.dvalue);
		}
	}
	| SQRT LPAREN expr RPAREN
	{
		if ($3.type != COMPLEX)
		{
			$3 = number2double($3);
			if ($3.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			$$.type = DOUBLE;
			$$.value.dvalue = sqrt($3.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex($3);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = sqrt(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex sqrt failed.");
				YYERROR;
			}
			$$.type = COMPLEX;
			$$.value.cvalue.r = real(c2);
			$$.value.cvalue.i = imag(c2);
		}
	}
	| SIN LPAREN expr RPAREN
	{
		if ($3.type != COMPLEX)
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
		else
		{
			Complex<double> c1 = number2complex($3);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = sin(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex sin failed.");
				YYERROR;
			}
			$$.type = COMPLEX;
			$$.value.cvalue.r = real(c2);
			$$.value.cvalue.i = imag(c2);
		}
	}
	| COS LPAREN expr RPAREN
	{
		if ($3.type != COMPLEX)
		{
			$3 = number2double($3);
			if ($3.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			$$.type = DOUBLE;
			$$.value.dvalue = cos($3.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex($3);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = cos(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex cos failed.");
				YYERROR;
			}
			$$.type = COMPLEX;
			$$.value.cvalue.r = real(c2);
			$$.value.cvalue.i = imag(c2);
		}
	}
	| TAN LPAREN expr RPAREN
	{
		if ($3.type != COMPLEX)
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
		else
		{
			Complex<double> c1 = number2complex($3);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = tan(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex tan failed.");
				YYERROR;
			}
			$$.type = COMPLEX;
			$$.value.cvalue.r = real(c2);
			$$.value.cvalue.i = imag(c2);
		}
	}
	| CSC LPAREN expr RPAREN
	{
		if ($3.type != COMPLEX)
		{
			$3 = number2double($3);
			if ($3.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			$$.type = DOUBLE;
			$$.value.dvalue = sin($3.value.dvalue);
			if ($$.value.dvalue != 0.0)
			{
				$$.value.dvalue /= 1.0;
			}
			else
			{
				yyerror((const char *)"csc division by zero.");
				YYERROR;
			}
		}
		else
		{
			Complex<double> c1 = number2complex($3);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = sin(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex sin failed.");
				YYERROR;
			}
			try {
				c2 /= 1.0;
			}
			catch (const char *pe)
			{
				yyerror((const char *)"csc complex division by zero.");
				YYERROR;
			}
			$$.type = COMPLEX;
			$$.value.cvalue.r = real(c2);
			$$.value.cvalue.i = imag(c2);
		}
	}
	| SSEC LPAREN expr RPAREN
	{
		if ($3.type != COMPLEX)
		{
			$3 = number2double($3);
			if ($3.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			$$.type = DOUBLE;
			$$.value.dvalue = cos($3.value.dvalue);
			if ($$.value.dvalue != 0.0)
			{
				$$.value.dvalue /= 1.0;
			}
			else
			{
				yyerror((const char *)"sec division by zero.");
				YYERROR;
			}
		}
		else
		{
			Complex<double> c1 = number2complex($3);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = cos(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex cos failed.");
				YYERROR;
			}
			try {
				c2 /= 1.0;
			}
			catch (const char *pe)
			{
				yyerror((const char *)"sec complex division by zero.");
				YYERROR;
			}
			$$.type = COMPLEX;
			$$.value.cvalue.r = real(c2);
			$$.value.cvalue.i = imag(c2);
		}
	}
	| COT LPAREN expr RPAREN
	{
		if ($3.type != COMPLEX)
		{
			$3 = number2double($3);
			if ($3.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			$$.type = DOUBLE;
			$$.value.dvalue = tan($3.value.dvalue);
			if ($$.value.dvalue != 0.0)
			{
				$$.value.dvalue /= 1.0;
			}
			else
			{
				yyerror((const char *)"cot division by zero.");
				YYERROR;
			}
		}
		else
		{
			Complex<double> c1 = number2complex($3);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = tan(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex tan failed.");
				YYERROR;
			}
			try {
				c2 /= 1.0;
			}
			catch (const char *pe)
			{
				yyerror((const char *)"cot complex division by zero.");
				YYERROR;
			}
			$$.type = COMPLEX;
			$$.value.cvalue.r = real(c2);
			$$.value.cvalue.i = imag(c2);
		}
	}
	| ASIN LPAREN expr RPAREN
	{
		if ($3.type != COMPLEX)
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
		else
		{
			Complex<double> c1 = number2complex($3);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = asin(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex asin failed.");
				YYERROR;
			}
			$$.type = COMPLEX;
			$$.value.cvalue.r = real(c2);
			$$.value.cvalue.i = imag(c2);
		}
	}
	| ACOS LPAREN expr RPAREN
	{
		if ($3.type != COMPLEX)
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
		else
		{
			Complex<double> c1 = number2complex($3);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = acos(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex acos failed.");
				YYERROR;
			}
			$$.type = COMPLEX;
			$$.value.cvalue.r = real(c2);
			$$.value.cvalue.i = imag(c2);
		}
	}
	| ATAN LPAREN expr RPAREN
	{
		if ($3.type != COMPLEX)
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
		else
		{
			Complex<double> c1 = number2complex($3);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = atan(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex atan failed.");
				YYERROR;
			}
			$$.type = COMPLEX;
			$$.value.cvalue.r = real(c2);
			$$.value.cvalue.i = imag(c2);
		}
	}
	| SINH LPAREN expr RPAREN
	{
		if ($3.type != COMPLEX)
		{
			$3 = number2double($3);
			if ($3.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			$$.type = DOUBLE;
			$$.value.dvalue = sinh($3.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex($3);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = sinh(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex sinh failed.");
				YYERROR;
			}
			$$.type = COMPLEX;
			$$.value.cvalue.r = real(c2);
			$$.value.cvalue.i = imag(c2);
		}
	}
	| COSH LPAREN expr RPAREN
	{
		if ($3.type != COMPLEX)
		{
			$3 = number2double($3);
			if ($3.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", 
						__LINE__);
				YYERROR;
			}
			$$.type = DOUBLE;
			$$.value.dvalue = cosh($3.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex($3);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = cosh(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex cosh failed.");
				YYERROR;
			}
			$$.type = COMPLEX;
			$$.value.cvalue.r = real(c2);
			$$.value.cvalue.i = imag(c2);
		}
	}
	| TANH LPAREN expr RPAREN
	{
		if ($3.type != COMPLEX)
		{
			$3 = number2double($3);
			if ($3.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			$$.type = DOUBLE;
			$$.value.dvalue = tanh($3.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex($3);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = tanh(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex tanh failed.");
				YYERROR;
			}
			$$.type = COMPLEX;
			$$.value.cvalue.r = real(c2);
			$$.value.cvalue.i = imag(c2);
		}
	}
	| ASINH LPAREN expr RPAREN
	{
		if ($3.type != COMPLEX)
		{
			$3 = number2double($3);
			if ($3.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			$$.type = DOUBLE;
			$$.value.dvalue = asinh($3.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex($3);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = asinh(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex asinh failed.");
				YYERROR;
			}
			$$.type = COMPLEX;
			$$.value.cvalue.r = real(c2);
			$$.value.cvalue.i = imag(c2);
		}
	}
	| ACOSH LPAREN expr RPAREN
	{
		if ($3.type != COMPLEX)
		{
			$3 = number2double($3);
			if ($3.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			$$.type = DOUBLE;
			$$.value.dvalue = acosh($3.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex($3);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = acosh(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex acosh failed.");
				YYERROR;
			}
			$$.type = COMPLEX;
			$$.value.cvalue.r = real(c2);
			$$.value.cvalue.i = imag(c2);
		}
	}
	| ATANH LPAREN expr RPAREN
	{
		if ($3.type != COMPLEX)
		{
			$3 = number2double($3);
			if ($3.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			$$.type = DOUBLE;
			$$.value.dvalue = atanh($3.value.dvalue);
		}
		else
		{
			Complex<double> c1 = number2complex($3);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = atanh(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex atanh failed.");
				YYERROR;
			}
			$$.type = COMPLEX;
			$$.value.cvalue.r = real(c2);
			$$.value.cvalue.i = imag(c2);
		}
	}
	| ABS LPAREN expr RPAREN
	{
		switch ($3.type)
		{
		case ULONG:
			break;
		case DOUBLE:
			if ($3.value.dvalue < 0.0)
				$3.value.dvalue *= -1.0;
			break;
		case LONG:
			if ($3.value.lvalue < 0)
				$3.value.lvalue *= -1;
			break;
		case COMPLEX:
		{
			Complex<double> c1 = number2complex($3);
			Complex<double> c2(0.0, 0.0);
			try {
				c2 = abs(c1);
			}
			catch (const char *pe)
			{
				yyerror((const char *)"complex abs failed.");
				YYERROR;
			}
			$3.type = COMPLEX;
			$3.value.cvalue.r = real(c2);
			$3.value.cvalue.i = imag(c2);
			break;
		}
		default:
			mybcerror((const char *)"default case.", __LINE__);
			YYERROR;
		}
		$$ = $3;
	}
	| ARG LPAREN expr RPAREN
	{
		Complex<double> c1 = number2complex($3);
		Complex<double> c2(0.0, 0.0);
		try {
			c2 = arg(c1);
		}
		catch (const char *pe)
		{
			yyerror((const char *)"complex arg failed.");
			YYERROR;
		}
		$$.type = DOUBLE;
		$$.value.dvalue = real(c2);
	}
	| NORM LPAREN expr RPAREN
	{
		Complex<double> c1 = number2complex($3);
		Complex<double> c2(0.0, 0.0);
		try {
			c2 = norm(c1);
		}
		catch (const char *pe)
		{
			yyerror((const char *)"complex norm failed.");
			YYERROR;
		}
		$$.type = DOUBLE;
		$$.value.dvalue = real(c2);
	}
	| REAL LPAREN expr RPAREN
	{
		Complex<double> c1 = number2complex($3);
		Complex<double> c2(0.0, 0.0);
		try {
			c2 = real(c1);
		}
		catch (const char *pe)
		{
			yyerror((const char *)"complex real failed.");
			YYERROR;
		}
		$$.type = DOUBLE;
		$$.value.dvalue = real(c2);
	}
	| IMAG LPAREN expr RPAREN
	{
		Complex<double> c1 = number2complex($3);
		Complex<double> c2(0.0, 0.0);
		try {
			c2 = imag(c1);
		}
		catch (const char *pe)
		{
			yyerror((const char *)"complex imag failed.");
			YYERROR;
		}
		$$.type = DOUBLE;
		$$.value.dvalue = real(c2);
	}
	| LBRACKET cast COMMA cast RBRACKET
	{
		if ($2.type != COMPLEX && $4.type != COMPLEX)
		{
			$2 = number2double($2);
			if ($2.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			$4 = number2double($4);
			if ($4.type != DOUBLE)
			{
				mybcerror((const char *)"default case.", __LINE__);
				YYERROR;
			}
			$$.value.cvalue.r = $2.value.dvalue;
			$$.value.cvalue.i = $4.value.dvalue;
			$$.type = COMPLEX;
		}
		else
		{
			yyerror((const char *)"cannot include complex in a complex.");
			YYERROR;
		}
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
	| CCONST
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
	| CCAST
	{
		$$ = COMPLEX;
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
		printf("%03d: (%le, %le)\n", ixy, xy[ixy][0], xy[ixy][1]);
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

Complex<double>
number2complex(VALUE number)
{
	switch (number.type)
	{
	case LONG:
		return(Complex<double>((double)number.value.lvalue, 0.0));
	case ULONG:
		return(Complex<double>((double)number.value.ulvalue, 0.0));
	case DOUBLE:
		return(Complex<double>((double)number.value.dvalue, 0.0));
	case COMPLEX:
		break;
	default:
		MustBeTrue(0);
		break;
	}
	return(Complex<double>( 
			number.value.cvalue.r,
			number.value.cvalue.i));
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

void
yyerror(const char *s)
{
	fprintf(stderr, "%s\n", s);
}

void
mybcerror(const char *msg, int lnno)
{
	char buf[BUFSIZ];
	sprintf(buf, "%d: %s", lnno, msg);
	yyerror((const char *)buf);
	return;
}

unsigned long 
perm(unsigned long n, unsigned long r)
{
	unsigned long total = 1;
	for (unsigned long i=(n-r+1); i<=n; i++)
	{
		total = total*i;
	}
	return(total);
}

unsigned long 
comb(unsigned long n, unsigned long r)
{
	return(perm(n,r)/fact(r));
}

unsigned long 
fact(unsigned long n)
{
	if (n == 0 || n == 1)
		return(1L);
	else if (n == 2)
		return(2L);
	else
	{
		unsigned long total = 1;
		for ( ; n > 1; n--)
		{
			total *= n;
		}
		return(total);
	}
}

double
perm(double n, double r)
{
	return(fact(n)/fact(n-r));
}

double
comb(double n, double r)
{
	return(fact(n)/(fact(r)*fact(n-r)));
}

double
dabs(double d)
{
	if (d < 0.0)
		return(-d);
	else
		return(d);
}

double 
fact(double n)
{
	/* use stirling's approximation */
#if 0
	return(pow(n, n)*exp(-n)*sqrt(2.0*M_PI*n)*(1.0+1.0/(12.0*n)));
#else
	return(pow(n, n)*sqrt(2.0*M_PI*n)*exp(-n+1.0/(12.0*n)));
#endif
}

void
usury(double principal, double interest, double length)
{
	length *= 12.0; 		// convert to months
	interest /= 100.0*12.0;		// convert to fraction/month
	double payment = 
		principal*interest/(1.0-pow((1.0+interest), -length));
	double totalpayment = length*payment;
	printf("\n");
	printf("monthly payment: %-10.2lf\n", payment);
	printf("total payment  : %-10.2lf\n", totalpayment);
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

	/* error tolerance for doubles */
	deps = calcEpsilon(double(0));

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
