%{
/* required headers */
#include <stdio.h>
#include <string.h>

/* externs */
void yyerror(const char *);

%}

%start stabstring

%union {
	char string[32];
};

%token <string> AMPERSAND
%token <string> ASTERICK
%token <string> AT
%token <string> COLON
%token <string> COMMA
%token <string> EQUAL
%token <string> HEXADECIMAL
%token <string> INTEGER
%token <string> LBRAC
%token <string> LETT
%token <string> LETT_A
%token <string> LETT_C
%token <string> LETT_D
%token <string> LETT_E
%token <string> LETT_F
%token <string> LETT_G
%token <string> LETT_I
%token <string> LETT_J
%token <string> LETT_K
%token <string> LETT_L
%token <string> LETT_M
%token <string> LETT_N
%token <string> LETT_O
%token <string> LETT_P
%token <string> LETT_Q
%token <string> LETT_R
%token <string> LETT_S
%token <string> LETT_T
%token <string> LETT_V
%token <string> LETT_Y
%token <string> LETT_Z
%token <string> LETT_a
%token <string> LETT_b
%token <string> LETT_c
%token <string> LETT_d
%token <string> LETT_e
%token <string> LETT_f
%token <string> LETT_g
%token <string> LETT_h
%token <string> LETT_i
%token <string> LETT_j
%token <string> LETT_k
%token <string> LETT_l
%token <string> LETT_m
%token <string> LETT_n
%token <string> LETT_o
%token <string> LETT_p
%token <string> LETT_q
%token <string> LETT_r
%token <string> LETT_s
%token <string> LETT_t
%token <string> LETT_u
%token <string> LETT_v
%token <string> LETT_w
%token <string> LETT_z
%token <string> LPAREN
%token <string> MINUS
%token <string> NAME
%token <string> PLUS
%token <string> POUIND
%token <string> POUND
%token <string> RBRAC
%token <string> REAL
%token <string> RPAREN
%token <string> SEMICOLON
%token <string> STRING

%%

stabstring:
	NAME COLON class 
	| class
	;

class:
	LETT_c EQUAL constant SEMICOLON
	| namedtype
	| parameter
	| procedure
	| variable
	| label
	;

constant:
	LETT_b ordvalue
	| LETT_c ordvalue
	| LETT_e typeid COMMA ordvalue
	| LETT_i INTEGER
	| LETT_r REAL
	| LETT_s STRING
	| LETT_C REAL COMMA REAL
	| LETT_S typeid COMMA numelements COMMA numbits COMMA bitpattern
	;

ordvalue:
	INTEGER
	;

numelements:
	INTEGER
	;

numbits:
	INTEGER
	;

numbytes:
	INTEGER
	;

bitpattern:
	HEXADECIMAL
	;

namedtype:
	LETT_t typeid
	| LETT_T typeid
	;

parameter:
	LETT_a typeid
	| LETT_p typeid
	| LETT_v typeid
	| LETT_C typeid
	| LETT_D typeid
	| LETT_R typeid
	;


procedure:
	proc
	| proc COMMA NAME COLON NAME
	;

variable:
	typeid
	| LETT_d typeid
	| LETT_r typeid
	| LETT_G typeid
	| LETT_S typeid
	| LETT_V typeid
	| LETT_Y
	| LETT_Z typeid
	;

label:
	LETT_L
	;

proc:
	LETT_f typeid
	| LETT_g typeid
	| LETT_m typeid
	| LETT_J typeid
	| LETT_F typeid
	| LETT_I
	| LETT_P
	| LETT_Q
	;

typeid:
	INTEGER
	| INTEGER EQUAL typedef
	| INTEGER typeattrs typedef
	;

typeattrs:
	AT typeattrlist SEMICOLON
	;

typeattrlist:
	AT typeattr
	| typeattr
	;

typeattr:
	LETT_a INTEGER
	| LETT_s INTEGER
	| LETT_p INTEGER
	| LETT_P
	| error
	;

typedef:
	INTEGER
	| LETT_b typeid SEMICOLON POUND numbytes
	| LETT_c typeid SEMICOLON POUND numbits
	| LETT_d typeid
	| LETT_e enumspec SEMICOLON
	| LETT_g typeid SEMICOLON POUND numbits
	| LETT_i NAME COLON NAME SEMICOLON
	| LETT_i NAME COLON NAME COMMA typeid SEMICOLON
	| LETT_k typeid
	| LETT_l SEMICOLON POUND
	| LETT_m optvbasespec optmultibasespec typeid COLON typeid COLON typeid SEMICOLON
	| LETT_n typeid SEMICOLON POUND numbytes
	| LETT_o NAME SEMICOLON
	| LETT_o NAME COMMA typeid
	| LETT_w typeid
	| LETT_z typeid SEMICOLON POUIND numbytes
	| LETT_C usage
	| LETT_I numbytes SEMICOLON POUND picsize
	| LETT_K cobolfiledesc SEMICOLON
	| LETT_M typeid SEMICOLON POUND bound
	| LETT_N
	| LETT_S typeid
	| ASTERICK typeid
	| AMPERSAND typeid
	| LETT_V typeid
	| LETT_Z
	| array
	| subrange
	| proceduretype
	| record
	;

enumspec:
	enumlist
	| typeid COLON enumlist
	;

enumlist:
	enum
	| enumlist enum
	;

enum:
	NAME COLON ordvalue COMMA POUND
	;

array:
	LETT_a typeid SEMICOLON POUND typeid
	| LETT_A typeid
	| LETT_D INTEGER COMMA typeid
	| LETT_E INTEGER COMMA typeid
	| LETT_O INTEGER COMMA typeid
	| LETT_P typeid SEMICOLON POUND typeid
	;

subrange:
	LETT_t typeid SEMICOLON POUND bound SEMICOLON POUND bound
	;

bound:
	INTEGER
	| boundtype INTEGER
	| LETT_J
	;

boundtype:
	LETT_A
	| LETT_S
	| LETT_T
	| LETT_a
	| LETT_t
	;

proceduretype:
	LETT_f typeid SEMICOLON
	| LETT_f typeid COMMA numparams SEMICOLON tparamlist SEMICOLON
	| LETT_p numparams SEMICOLON tparamlist SEMICOLON
	| LETT_R numparams SEMICOLON namedtparamlist
	| LETT_F typeid COMMA numparams SEMICOLON namedtparamlist SEMICOLON
	;

numparams:
	INTEGER
	;

tparamlist:
	tparam
	| tparamlist tparam
	;

tparam:
	typeid COMMA passby SEMICOLON POUND
	;

namedtparamlist:
	/* empty */
	| namedtplist
	;

namedtplist:
	namedtparam
	| namedtplist namedtparam
	;

namedtparam:
	NAME COLON typeid COMMA passby initbody SEMICOLON POUND
	| COLON typeid COMMA passby initbody SEMICOLON POUND
	;

record:
	LETT_s numbytes POUND fieldlist SEMICOLON
	| LETT_u numbytes POUND fieldlist SEMICOLON
	| LETT_v numbytes POUND fieldlist variantpart SEMICOLON
	| LETT_Y numbytes classkey optpbv optbasespeclist LPAREN extendedfieldlist optnameresolutionlist SEMICOLON
	| LETT_G redefinition COMMA LETT_n numbits POUND fieldlist SEMICOLON
	| LETT_G LETT_n numbits fieldlist SEMICOLON
	| LETT_G redefinition COMMA LETT_c numbits POUND condfieldlist SEMICOLON
	| LETT_G LETT_c numbits condfieldlist SEMICOLON
	;

optvbasespec:
	/* empty */
	| LETT_v
	;

optmultibasespec:
	/* empty */
	| LETT_m
	;

optpbv:
	/* empty */
	| LETT_V
	;

classkey:
	LETT_s
	| LETT_u
	| LETT_c
	;

optbasespeclist:
	/* empty */
	| basespeclist
	;

basespeclist:
	basespec
	| basespeclist COMMA basespec
	;

basespec:
	virtualaccessspec baseclassoffset COLON classtypeid
	;

baseclassoffset:
	INTEGER
	;

classtypeid:
	typeid
	;

virtualaccessspec:
	/* empty */
	| LETT_v accessspec
	| LETT_v
	| accessspec
	;

genspec:
	/* empty */
	| LETT_c 
	;

accessspec:
	LETT_i POUND
	| LETT_o POUND
	| LETT_u POUND
	;

anonspec:
	/* empty */
	| LETT_a
	;

virtualspec:
	/* empty */
	| LETT_v LETT_p
	| LETT_v
	;

extendedfieldlist:
	/* empty */
	| extendedfieldlist extendedfield
	;

extendedfield:
	genspec accessspec anonspec datamember
	| genspec virtualspec accessspec optvirtualfuncindex memberfunction
	| accessspec anonspec nestedclass
	| anonspec friendclass
	| anonspec friendfunction
	;

datamember:
	memberattrs COLON field SEMICOLON
	;

memberattrs:
	isstatic isvtblptr isvbaseptr
	;

isstatic:
	/* empty */
	| LETT_s
	;

isvtblptr:
	/* empty */
	| LETT_p INTEGER NAME
	;

isvbaseptr:
	/* empty */
	| LETT_b
	| LETT_r
	;

memberfunction:
	LBRAC functype memberfuncattrs COLON NAME COLON typeid SEMICOLON POUND
	;

memberfuncattrs:
	/* empty */
	| LETT_s
	| LETT_i
	| LETT_k
	| LETT_V
	;

nestedclass:
	LETT_N typeid SEMICOLON POUND
	;

friendclass:
	LPAREN typeid SEMICOLON POUND
	;

friendfunction:
	RBRAC NAME COLON typeid SEMICOLON POUND
	;

optvirtualfuncindex:
	/* empty */
	| INTEGER
	;

functype:
	LETT_f 
	| LETT_c
	| LETT_d
	;

initbody:
	/* empty */
	| STRING
	;

optnameresolutionlist:
	/* empty */
	| RPAREN nameresolutionlist
	;

nameresolutionlist:
	nameresolution 
	| nameresolution COMMA nameresolutionlist
	;

nameresolution:
	membername COLON classtypeid
	;

membername:
	NAME
	;

fieldlist:
	field
	| fieldlist field
	;

field:
	NAME COLON typeid COMMA bitoffset COMMA numbits SEMICOLON POUND
	;

variantpart:
	LBRAC vtag vfieldlist RBRAC
	;

vtag:
	LPAREN field
	| LPAREN NAME COLON SEMICOLON POUND
	;

vfieldlist:
	vlist
	| vfieldlist vlist
	;

vlist:
	vfield
	| vfield variantpart
	;

vfield:
	LPAREN vrangelist COLON fieldlist 
	;

vrangelist:
	vrange
	| vrangelist COMMA vrange
	;

vrange:
	LETT_b ordvalue
	| LETT_c ordvalue
	| LETT_e typeid COMMA ordvalue
	| LETT_i INTEGER
	| LETT_r typeid SEMICOLON bound SEMICOLON bound
	;

condfieldlist:
	conditions COMMA POUND fieldlist
	| fieldlist POUND
	;

conditions:
	/* empty */
	| conditions condition
	;

bitoffset:
	INTEGER
	;

usage:
	picstoragetype numbits COMMA editdescription COMMA picsize SEMICOLON
	| redefinition picstoragetype numbits COMMA editdescription COMMA picsize SEMICOLON
	| picstoragetype numbits COMMA editdescription COMMA picsize COMMA POUND condition SEMICOLON
	| redefinition picstoragetype numbits COMMA editdescription COMMA picsize COMMA POUND condition SEMICOLON
	;

redefinition:
	LETT_r NAME
	;

picstoragetype:
	LETT_a
	| LETT_b 
	| LETT_c
	| LETT_d
	| LETT_e
	| LETT_f
	| LETT_g
	| LETT_h
	| LETT_i
	| LETT_j
	| LETT_k
	| LETT_l
	| LETT_m
	| LETT_n
	| LETT_o
	| LETT_p
	| LETT_q
	| LETT_r
	| LETT_s
	| LETT_t
	;

editdescription:
	STRING
	| INTEGER
	;

picsize:
	INTEGER
	;

condition:
	NAME COLON INTEGER EQUAL LETT_q conditiontype COMMA valuelist SEMICOLON POUND
	;

conditiontype:
	conditionprimitive COMMA kanjichar
	;

conditionprimitive:
	LETT_n sign decimalsite
	| LETT_a
	| LETT_f
	;

sign:
	PLUS
	| MINUS
	| /* empty */
	;

decimalsite:
	INTEGER
	;

kanjichar:
	INTEGER
	;

valuelist:
	value
	| valuelist value
	;

value:
	INTEGER COLON arbitrarycharacters POUND
	;

arbitrarycharacters:
	INTEGER
	;

cobolfiledesc:
	organization accessmethod numbytes
	;

organization:
	LETT_i
	| LETT_l
	| LETT_r
	| LETT_s
	;

accessmethod:
	LETT_d
	| LETT_o
	| LETT_r
	| LETT_s
	;

passby:
	INTEGER
	;
	

%%

void
yyerror(const char *s)
{
	fprintf(stderr, "%s\n", s);
}

main(int argc, char **argv)
{
#ifdef YYDEBUG
	extern int yydebug;
	yydebug = 1;
#endif

	/* call parser */
	yyparse();

	exit(0);
}
