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

%token <string> HEXADECIMAL
%token <string> INTEGER
%token <string> NAME
%token <string> REAL
%token <string> STRING

%%

stabstring:
	NAME ':' class 
	| ':' class
	;

class:
	'c' '=' constant ';'
	| namedtype
	| parameter
	| procedure
	| variable
	| label
	;

constant:
	'b' ordvalue
	| 'c' ordvalue
	| 'e' typeid ',' ordvalue
	| 'i' INTEGER
	| 'r' REAL
	| 's' STRING
	| 'C' REAL ',' REAL
	| 'S' typeid ',' numelements ',' numbits ',' bitpattern
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
	't' typeid
	| 'T' typeid
	;

parameter:
	'a' typeid
	| 'p' typeid
	| 'v' typeid
	| 'C' typeid
	| 'D' typeid
	| 'R' typeid
	;

procedure:
	proc
	| proc ',' NAME ':' NAME
	;

variable:
	typeid
	| 'd' typeid
	| 'r' typeid
	| 'G' typeid
	| 'S' typeid
	| 'V' typeid
	| 'Y'
	| 'Z' typeid NAME
	;

label:
	'L'
	;

proc:
	'f' typeid
	| 'g' typeid
	| 'm' typeid
	| 'J' typeid
	| 'F' typeid
	| 'I'
	| 'P'
	| 'Q'
	;

typeid:
	INTEGER
	| INTEGER '=' typedef
	| INTEGER typeattrs typedef
	| '(' INTEGER ',' INTEGER ')'
	| '(' INTEGER ',' INTEGER ')' '=' typedef
	| '(' INTEGER ',' INTEGER ')' typeattrs typedef
	;

typeattrs:
	'@' typeattrlist ';'
	;

typeattrlist:
	typeattrlist ';' '@' typeattr
	| typeattr
	;

typeattr:
	'a' INTEGER
	| 's' INTEGER
	| 'p' INTEGER
	| 'P'
	| error
	;

typedef:
	INTEGER
	| 'b' typeid ';' '#' numbytes
	| 'c' typeid ';' '#' numbits
	| 'd' typeid
	| 'e' enumspec ';'
	| 'g' typeid ';' '#' numbits
	| 'i' NAME ':' NAME ';'
	| 'i' NAME ':' NAME ',' typeid ';'
	| 'k' typeid
	| 'l' ';' '#'
	| 'm' optvbasespec optmultibasespec typeid ':' typeid ':' typeid ';'
	| 'n' typeid ';' '#' numbytes
	| 'o' NAME ';'
	| 'o' NAME ',' typeid
	| 'w' typeid
	| 'z' typeid ';' '#' numbytes
	| 'C' usage
	| 'I' numbytes ';' '#' picsize
	| 'K' cobolfiledesc ';'
	| 'M' typeid ';' '#' bound
	| 'N'
	| 'S' typeid
	| '*' typeid
	| '&' typeid
	| 'V' typeid
	| 'Z'
	| array
	| subrange
	| proceduretype
	| record
	;

enumspec:
	enumlist
	| typeid ':' enumlist
	;

enumlist:
	enum
	| enumlist enum
	;

enum:
	NAME ':' ordvalue ',' '#'
	;

array:
	'a' typeid ';' '#' typeid
	| 'A' typeid
	| 'D' INTEGER ',' typeid
	| 'E' INTEGER ',' typeid
	| 'O' INTEGER ',' typeid
	| 'P' typeid ';' '#' typeid
	;

subrange:
	'r' typeid ';' '#' bound ';' '#' bound
	;

bound:
	INTEGER
	| boundtype INTEGER
	| 'J'
	;

boundtype:
	'A'
	| 'S'
	| 'T'
	| 'a'
	| 't'
	;

proceduretype:
	'f' typeid ';'
	| 'f' typeid ',' numparams ';' tparamlist ';'
	| 'p' numparams ';' tparamlist ';'
	| 'R' numparams ';' namedtparamlist
	| 'F' typeid ',' numparams ';' namedtparamlist ';'
	;

numparams:
	INTEGER
	;

tparamlist:
	tparam
	| tparamlist tparam
	;

tparam:
	typeid ',' passby ';' '#'
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
	NAME ':' typeid ',' passby initbody ';' '#'
	| ':' typeid ',' passby initbody ';' '#'
	;

record:
	's' numbytes '#' fieldlist ';'
	| 'u' numbytes '#' fieldlist ';'
	| 'v' numbytes '#' fieldlist variantpart ';'
	| 'Y' numbytes classkey optpbv optbasespeclist '(' extendedfieldlist optnameresolutionlist ';'
	| 'G' redefinition ',' 'n' numbits '#' fieldlist ';'
	| 'G' redefinition ',' 'c' numbits '#' condfieldlist ';'
	| 'G' 'n' numbits fieldlist ';'
	| 'G' 'c' numbits condfieldlist ';'
	;

optvbasespec:
	/* empty */
	| 'v'
	;

optmultibasespec:
	/* empty */
	| 'm'
	;

optpbv:
	/* empty */
	| 'V'
	;

classkey:
	's'
	| 'u'
	| 'c'
	;

optbasespeclist:
	/* empty */
	| basespeclist
	;

basespeclist:
	basespec 
	| basespeclist ',' basespec
	;

basespec:
	virtualaccessspec baseclassoffset ':' classtypeid
	;

baseclassoffset:
	INTEGER
	;

classtypeid:
	typeid
	;

virtualaccessspec:
	/* empty */
	| 'v' accessspec
	| 'v'
	| accessspec
	;

genspec:
	/* empty */
	| 'c' 
	;

accessspec:
	'i' '#'
	| 'o' '#'
	| 'u' '#'
	;

anonspec:
	/* empty */
	| 'a'
	;

virtualspec:
	/* empty */
	| 'v' 'p'
	| 'v'
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
	memberattrs ':' field ';'
	;

memberattrs:
	isstatic isvtblptr isvbaseptr
	;

isstatic:
	/* empty */
	| 's'
	;

isvtblptr:
	/* empty */
	| 'p' INTEGER NAME
	;

isvbaseptr:
	/* empty */
	| 'b'
	| 'r'
	;

memberfunction:
	'[' functype memberfuncattrs ':' NAME ':' typeid ';' '#'
	;

memberfuncattrs:
	/* empty */
	| 's'
	| 'i'
	| 'k'
	| 'V'
	;

nestedclass:
	'N' typeid ';' '#'
	;

friendclass:
	'(' typeid ';' '#'
	;

friendfunction:
	']' NAME ':' typeid ';' '#'
	;

optvirtualfuncindex:
	/* empty */
	| INTEGER
	;

functype:
	'f' 
	| 'c'
	| 'd'
	;

initbody:
	/* empty */
	| STRING
	;

optnameresolutionlist:
	/* empty */
	| ')' nameresolutionlist
	;

nameresolutionlist:
	nameresolution 
	| nameresolution ',' nameresolutionlist
	;

nameresolution:
	membername ':' classtypeid
	| membername ':' 
	;

membername:
	NAME
	;

fieldlist:
	field
	| fieldlist field
	;

field:
	NAME ':' typeid ',' bitoffset ',' numbits ';' '#'
	;

variantpart:
	'[' vtag vfieldlist ']'
	;

vtag:
	'(' field
	| '(' NAME ':' ';' '#'
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
	'(' vrangelist ':' fieldlist 
	;

vrangelist:
	vrange
	| vrangelist ',' vrange
	;

vrange:
	'b' ordvalue
	| 'c' ordvalue
	| 'e' typeid ',' ordvalue
	| 'i' INTEGER
	| 'r' typeid ';' bound ';' bound
	;

condfieldlist:
	conditions ',' '#' fieldlist
	| fieldlist '#'
	;

conditions:
	/* empty */
	| conditions condition
	;

bitoffset:
	INTEGER
	;

usage:
	picstoragetype numbits ',' editdescription ',' picsize ';'
	| redefinition picstoragetype numbits ',' editdescription ',' picsize ';'
	| picstoragetype numbits ',' editdescription ',' picsize ',' '#' condition ';'
	| redefinition picstoragetype numbits ',' editdescription ',' picsize ',' '#' condition ';'
	;

redefinition:
	'r' NAME
	;

picstoragetype:
	'a'
	| 'b' 
	| 'c'
	| 'd'
	| 'e'
	| 'f'
	| 'g'
	| 'h'
	| 'i'
	| 'j'
	| 'k'
	| 'l'
	| 'm'
	| 'n'
	| 'o'
	| 'p'
	| 'q'
	| 'r'
	| 's'
	| 't'
	;

editdescription:
	STRING
	| INTEGER
	;

picsize:
	INTEGER
	;

condition:
	NAME ':' INTEGER '=' 'q' conditiontype ',' valuelist ';' '#'
	;

conditiontype:
	conditionprimitive ',' kanjichar
	;

conditionprimitive:
	'n' sign decimalsite
	| 'a'
	| 'f'
	;

sign:
	'+'
	| '-'
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
	INTEGER ':' arbitrarycharacters '#'
	;

arbitrarycharacters:
	INTEGER
	;

cobolfiledesc:
	organization accessmethod numbytes
	;

organization:
	'i'
	| 'l'
	| 'r'
	| 's'
	;

accessmethod:
	'd'
	| 'o'
	| 'r'
	| 's'
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
