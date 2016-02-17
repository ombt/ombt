%{
// asn.1 lexical analyzer

// THINGS LEFT TO DO ...
//
// 1) add xml tokens as described in sections 11.11, 11.13, 11.15, 11.21, 
// 11.22, 11.23 11.24, 11.25 in ITU-T X.680.
//
// 2) remove whitespace before and after a newline, when the newline
// occurs in the middle of a multiline string as described in 
// section 11.14.1 in ITU-T X.680.
//

// headers
#include "asn1.h"
#include "asn1yacc.h"

%}

%e 2000
%p 5000
%a 4000
%n 1000
%o 6000

typereference		[A-Z][-a-zA-Z0-9]*[a-zA-Z0-9]
identifier		[a-z][-a-zA-Z0-9]*[a-zA-Z0-9]
number			(0|[1-9][0-9]*)
realnumber		[0-9]+(\.[0-9]*)?([eE]\-?[0-9]+)?
bstring			\'[01 ]*\'B
hstring			\'[A-F0-9 ]*\'H
cstring			(\"[^\"]*\")(\"[^\"]*\")*
whitespace		[ \t]+
newline			[\012\013\014\015]+

%%

{typereference}	ECHO;
{identifier} 	ECHO;
{number}	ECHO;
{realnumber}	ECHO;
{bstring}	ECHO;
{hstring}	ECHO;
{cstring}	ECHO;
{whitespace}	ECHO;
{newline}	ECHO;

"*"		ECHO;
"&"		ECHO;
"::="		ECHO;
".."		ECHO;
"..."		ECHO;
"[["		ECHO;
"]]"		ECHO;
"{"		ECHO;
"}"		ECHO;
"<"		ECHO;
">"		ECHO;
","		ECHO;
"."		ECHO;
"("		ECHO;
")"		ECHO;
"["		ECHO;
"]"		ECHO;
"-"		ECHO;
"\""		ECHO;
"'"		ECHO;
" "		ECHO;
":"		ECHO;
";"		ECHO;
"@"		ECHO;
"|"		ECHO;
"!"		ECHO;
"^"		ECHO;

ABSENT			ECHO;
ABSTRACT-SYNTAX		ECHO;
ALL			ECHO;
APPLICATION		ECHO;
AUTOMATIC		ECHO;
BEGIN			ECHO;
BIT			ECHO;
BMPString		ECHO;
BOOLEAN			ECHO;
BY			ECHO;
CHARACTER		ECHO;
CHOICE			ECHO;
CLASS			ECHO;
COMPONENT		ECHO;
COMPONENTS		ECHO;
CONSTRAINED		ECHO;
CONTAINING		ECHO;
DEFAULT			ECHO;
EMBEDDED		ECHO;
EMBEDDED		ECHO;
ENCODED			ECHO;
END			ECHO;
ENUMERATED		ECHO;
EXCEPT			ECHO;
EXPLICIT		ECHO;
EXPORTS			ECHO;
EXTENSIBILITY		ECHO;
EXTERNAL		ECHO;
FALSE			ECHO;
FROM			ECHO;
GeneralizeTime		ECHO;
GeneralString		ECHO;
GraphicString		ECHO;
IA5String		ECHO;
IDENTIFIER		ECHO;
IMPLICIT		ECHO;
IMPLIED			ECHO;
IMPORTS			ECHO;
INCLUDES		ECHO;
INSTANCE		ECHO;
INTEGER			ECHO;
INTERSECTION		ECHO;
ISO646String		ECHO;
MAX			ECHO;
MIN			ECHO;
MINUS-INFINITY		ECHO;
NULL			ECHO;
NumericString		ECHO;
OBJECT			ECHO;
OF			ECHO;
OPTIONAL		ECHO;
PATTERN			ECHO;
PDV			ECHO;
PLUS-INFINITY		ECHO;
PRESENT			ECHO;
PrintableString		ECHO;
PRIVATE			ECHO;
REAL			ECHO;
RELATIVE-OID		ECHO;
SEQUENCE		ECHO;
SET			ECHO;
SIZE			ECHO;
STRING			ECHO;
SYNTAX			ECHO;
T61String		ECHO;
TAGS			ECHO;
TYPE-IDENTIFIER		ECHO;
UNION			ECHO;
UNIQUE			ECHO;
UNIVERSAL		ECHO;
UniversalString		ECHO;
UTCTime			ECHO;
UTF8String		ECHO;
VideotexString		ECHO;
VisibleString		ECHO;
WITH			ECHO;

.			{
				printf("UNKNOWN CHARACTER ==>> ");
				ECHO;
			}

%%

// useless function
int
yywrap(void)
{
	return(1);
}

main(int argc, char **argv)
{
	extern FILE *yyin;

	// tokenize a list of files
	for (int arg = 1; arg < argc; arg++)
	{
		// tell which file is scanned
		cout << "scanning file ... " << argv[arg] << endl;

		// open file to scan
		if ((yyin = fopen(argv[arg], "r")) == NULL)
		{
			ERRORD("fopen failed.", argv[arg], errno);
			continue;
		}

		// call scanner 
		while (yylex() != 0) ;
	}

	// all done
	return(0);
}




