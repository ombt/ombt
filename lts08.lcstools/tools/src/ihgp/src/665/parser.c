/* operating system headers */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <malloc.h>

/* parser headers */
#include "gettoken.h"
#include "parser.h"
#include "symtab.h"

/* globals for parser */
FILE *traceFd;              
FILE *sourceFd;              
FILE *listFd;
FILE *asmFd;
char traceFile[BUFSIZ];
char sourceFile[BUFSIZ];
char listFile[BUFSIZ];
char asmFile[BUFSIZ];
char fname[100][128];
int fnidx = 0;
int traceFlag;
int semanticErrors;
int numberOfReturns;
SEMANTICINFO returnInfo;
int loopCounter = 0;
int verboseFlag = 0;

/* stack for tracking block nesting */
BLOCK blockNesting[BLOCKTABLESIZE];

/* array to hold space for strings */
int nextString = 0;
char *stringTable[STRINGTABLESIZE];

/* index for generating generic labels */
int labelIndex = 0;

/* macros for tracing */
#define ENTRY(traceMsg) traceEntry(__LINE__, traceMsg)

#define RETURN \
{ \
	traceReturn(__LINE__); \
	return; \
}

#define VRETURN(retval) \
{ \
	traceReturn(__LINE__); \
	return(retval); \
}

#define listPrint(listMsg) ListPrint(__LINE__, listMsg)

#define acceptToken(expectedToken) AcceptToken(__LINE__, expectedToken)

/* macro to copy symbol info to a semantic structure */
#define SYMBOL2SEMANTIC(sym, sem) \
{ \
	sem.type = sym.type; \
	sem.kind = sym.kind; \
	sem.symbolTableIndex = sym.entry; \
}

/* trace entry/return functions */
void
traceEntry(lnno, traceMsg)
int lnno;
char *traceMsg;
{
	if (traceFlag)
	{
		fnidx++;
		if (fnidx < 100)
		{
			strcpy(fname[fnidx], traceMsg);
			fprintf(traceFd, "\tentry into %s (%d)\n", traceMsg, lnno);
		}
		else
		{
			fprintf(traceFd, "\tENTRY OVERFLOW FOR %s (%d)!!!\n", traceMsg, lnno);
		}
	}
	return;
}

void 
traceReturn(lnno)
int lnno;
{
	if (traceFlag)
	{
		if (fnidx < 100)
		{
			fprintf(traceFd, "\treturn from %s (%d)\n", fname[fnidx], lnno);
			fname[fnidx][0] = 0;
		}
		fnidx--;
	}
	return;
}

/* function to initialize tracing */
int
initTrace(fname, trace)
char *fname;
int trace;
{
	int len;

	/* turn off tracing by default */
	traceFlag = 0;

	/* check if trace is enabled */
	if (trace == 0)
	{
		/* no tracing */
		traceFd = (FILE *)0;
		return(OK);
	}

	/* trace is on, check file name */
	if (fname == 0 || *fname == 0)
	{
		fprintf(stderr, "initTrace(): file name is null.\n");
		errno = EINVAL;
		return(NOTOK);
	}

	/* construct name of trace file */
	strcpy(traceFile, fname);
	len = strcspn(traceFile, ".");
	traceFile[len] = 0;
	strcat(traceFile, ".dbg");

	/* open trace file */
	if ((traceFd = fopen(traceFile, "w")) == (FILE *)0)
	{
		fprintf(stderr, "initTrace(): fopen(%s) failed, errno = %d.\n",
			traceFile, errno);
		return(NOTOK);
	}

	/* set trace flag */
	traceFlag = trace;

	/* all done */
	return(OK);
}

/* function to open source and tracing files */
int
initParser(fname, reopen, verbose)
char *fname;
int reopen;
int verbose;
{
	int len;

	/* set verbose flag for comments */
	verboseFlag = verbose;

	/* check flag to determine how files are open */
	switch (reopen)
	{
	case FIRSTOPEN:
		/* first time to open files, check file name */
		if (fname == 0 || *fname == 0)
		{
			fprintf(stderr, "initParser(): file name is null.\n");
			errno = EINVAL;
			return(NOTOK);
		}
		sourceFd = (FILE *)0;
		listFd = (FILE *)0;
		asmFd = (FILE *)0;

		/* no semantic error as of now */
		semanticErrors = 0;

		/* construct name of listing file */
		strcpy(sourceFile, fname);
		strcpy(listFile, fname);
		len = strcspn(listFile, ".");
		listFile[len] = 0;    
		strcat(listFile, ".lst");

		/* open listing file */
		if ((listFd = fopen(listFile, "w")) == (FILE *)0)
		{
			fprintf(stderr, "initParser(): fopen(%s) failed, errno = %d.\n",
				listFile, errno);
			return(NOTOK);
		}
		break;

	case REOPEN:
		/* reopen listing and source files again */
		fclose(listFd);
		listFd = (FILE *)0;
		fclose(sourceFd);
		sourceFd = (FILE *)0;

		/* open listing file */
		if ((listFd = fopen(listFile, "w")) == (FILE *)0)
		{
			fprintf(stderr, "initParser(): fopen(%s) failed, errno = %d.\n",
				listFile, errno);
			return(NOTOK);
		}

		/* open assembler file */
		strcpy(asmFile, fname);
		len = strcspn(asmFile, ".");
		asmFile[len] = 0;    
		strcat(asmFile, ".asm");
		if ((asmFd = fopen(asmFile, "w")) == (FILE *)0)
		{
			fprintf(stderr, "initParser(): fopen(%s) failed, errno = %d.\n",
				asmFile, errno);
			return(NOTOK);
		}
		break;

	case NOREOPEN:
		/* keep listing file open, but reopen source file */
		fclose(sourceFd);
		sourceFd = (FILE *)0;
		break;

	default:
		fprintf(stderr, "initParser(): invalid reopen flag.\n");
		errno = EINVAL;
		return(NOTOK);
	}

	/* open source file */
	if ((sourceFd = fopen(sourceFile, "r")) == (FILE *)0)
	{
		fprintf(stderr, "initParser(): fopen(%s) failed, errno = %d.\n",
			sourceFile, errno);
		return(NOTOK);
	}

	/* initialize tokenizer */
	initGetToken();

	/* all done */
	return(OK);
}

/* close up trace files */
void
closeTrace()
{
	if (traceFd != (FILE *)0) fclose(traceFd);
	traceFd = (FILE *)0;
	return;
}

/* close source and listing files */
void
closeParser()
{
	if (sourceFd != (FILE *)0) fclose(sourceFd);
	if (listFd != (FILE *)0) fclose(listFd);
	if (asmFd != (FILE *)0) fclose(asmFd);
	sourceFd = (FILE *)0;
	listFd = (FILE *)0;
	asmFd = (FILE *)0;
	return;
}

/* function to print a line to the listing file */
void
ListPrint(lineNo, msg)
int lineNo;
char *msg;
{
	fprintf(listFd, "(%d) present token <%s>\n", lineNo, tokenString);
	fprintf(listFd, msg);
	if (traceFlag != 0)
	{
		fprintf(traceFd, "(%d) present token <%s>\n", lineNo, tokenString);
		fprintf(traceFd, msg);
	}
	return;
}

/* function to print a line to the listing file */
void
errorPrint(lineNo, format, l1, l2, l3, l4, l5)
int lineNo;
char *format;
unsigned long l1, l2, l3, l4, l5;
{
	printSpaces(7);
	fprintf(listFd, "(%d) ", lineNo);
	fprintf(listFd, format, l1, l2, l3, l4, l5);
	if (traceFlag != 0)
	{
		fprintf(traceFd, "(%d) ", lineNo);
		fprintf(traceFd, format, l1, l2, l3, l4, l5);
	}
	return;
}

/* write to assembler output file */
void
asmWrite(fmt, l1, l2, l3, l4, l5)
char *fmt;
unsigned long l1, l2, l3, l4, l5;
{
	/* write to output file if no errors */
	if (semanticErrors == 0 && asmFd != (FILE *)0)
	{
		fprintf(asmFd, fmt, l1, l2, l3, l4, l5);
	}

	/* all done */
	return;
}

/* write comment to assembler output file */
void
asmComment(comment)
char *comment;
{
	/* write to output file if no errors */
	if (semanticErrors == 0 && asmFd != (FILE *)0)
	{
		if (verboseFlag) fprintf(asmFd, "; %s\n", comment);
	}

	/* all done */
	return;
}

/* function to exit */
void
suicide()
{
	closeTrace();
	closeParser();
	exit(2);
}

/* accept a token */
void
AcceptToken(acceptLineNo, acceptType)
int acceptLineNo;
TOKEN acceptType;
{
	char buf[256];
	if (acceptType == tokenType)
	{
		if (traceFlag != 0)
		{
			switch (tokenType)
			{
			case T_EOF:
				fprintf(traceFd, "\t\taccepting EOF (%d)\n", acceptLineNo);
				break;
			case T_ERR:
				fprintf(traceFd, "\t\taccepting ERR (%d)\n", acceptLineNo);
				break;
			case T_NULL:
				fprintf(traceFd, "\t\taccepting NULL (%d)\n", acceptLineNo);
				break;
			default:
				fprintf(traceFd, "\t\taccepting <%s> (%d)\n", tokenString, acceptLineNo);
				break;
			}
		}
		tokenType = T_NULL;
		tokenString[0] = 0;
		tokenValue = 0;
	}
	else
	{
		sprintf(buf, "acceptToken: expected (%d) and actual (%d, %s) tokens do not match.\n", 
			(int)acceptType, (int)tokenType, tokenString);
		listPrint(buf);
		suicide();
	}
	return;
}

/* store a string in string table */
void
storeString(string, label)
char *string, *label;
{
	int istr;

	/* search if string already exists */
	for (istr = 0; istr < nextString; istr++)
	{
		/* compare new string with existing strings */
		if (strcmp(stringTable[istr], string) == 0)
		{
			/* found string, generate label */
			sprintf(label, "@STRING%d", istr);
			return;
		}
	}

	/* store string in table */
	if (nextString >= STRINGTABLESIZE)
	{
		listPrint("ERROR: storeString() - string table is exhausted.\n");
		suicide();
	}

	/* allocate string and store */
	stringTable[nextString] = (char *) malloc(strlen(string)+1);
	if (stringTable[nextString] == (char *)0)
	{
		listPrint("ERROR: storeString() - unable to malloc a string.\n");
		suicide();
	}
	strcpy(stringTable[nextString], string);

	/* generate label */
	sprintf(label, "@STRING%d", nextString);

	/* next available string */
	nextString++;

	/* all done */
	return;
}

/* write strings to a data section */
void
writeStrings()
{
	int istr;

	/* any strings to write */
	if (nextString < 1) return;

	/* create a data section for strings */
	asmWrite("\n\t.DATA\n");
	asmComment("data section containing strings.");

	/* write out strings */
	for (istr = 0; istr < nextString; istr++)
	{
		asmWrite("@STRING%ld\tdb\t\"%s\", 0\n", (long)istr, stringTable[istr]);
		asmWrite("@STRING%ldLEN\tEQU\t$-@STRING%ld\n", (long)istr, (long)istr);
	}

	/* all done */
	return;
}

/* generate a unique label */
void
getLabel(label)
char *label;
{
	sprintf(label, "@LABEL%d", labelIndex++);
	return;
}

/* main entry in parser */
int
parser(pass)
int pass;
{
	ENTRY("parser");

	/* get first token ready */
	getToken();

	/* check for empty files */
	if (tokenType == T_EOF)
	{
		acceptToken(T_EOF);
		listPrint("WARNING: empty file detected.\n");
		VRETURN(OK);
	}

	/* which pass is it */
	switch (pass)
	{
	case FIRSTPASS:
		/* create and initialize the symbol table */
		if (createSymbolTable(SYMBOLTABLESIZE, BLOCKTABLESIZE) != OK)
		{
			errorPrint(__LINE__, "createSymbolTable(%d, %d) failed.\n",
				(long) SYMBOLTABLESIZE, (long)BLOCKTABLESIZE);
			suicide();
		}

		/* create symbol table for global symbols */
		if (createBlock() != OK)
		{
			errorPrint(__LINE__, "createBlock() failed.\n");
			suicide();
		}

		/* first pass thru source file */
		fp_program();
		if (semanticErrors > 0)
		{
			fprintf(listFd, "\n%d SEMANTIC ERRORS WERE FOUND IN FIRST PASS.\n",
				semanticErrors);
			if (traceFlag != 0)
			{
				fprintf(traceFd, "\n%d SEMANTIC ERRORS WERE FOUND IN FIRST PASS.\n",
					semanticErrors);
			}

			/* dump symbols */
			dumpBlock(1);
			VRETURN(NOTOK);
		}

		/* dump symbols */
		dumpBlock(1);
		break;

	case SECONDPASS:
		/* program non-terminal */
		program();
		if (semanticErrors > 0)
		{
			fprintf(listFd, "TOTAL OF %d SEMANTIC ERRORS WERE FOUND.\n", 
				semanticErrors);
			if (traceFlag != 0)
			{
				fprintf(traceFd, "TOTAL OF %d SEMANTIC ERRORS WERE FOUND.\n", 
					semanticErrors);
			}

			/* dump symbols */
			dumpBlock(1);
			VRETURN(NOTOK);
		}

		/* dump symbols */
		dumpBlock(1);
		break;

	default:
		/* wrong flag */
		fprintf(listFd, "wrong pass requested from parser.\n");
		suicide();
	}

	/* print msg that no errors were found */
	fprintf(listFd, "No errors found in %s.\n", sourceFile);
	if (traceFlag != 0)
	{
		fprintf(traceFd, "No errors found in %s.\n", sourceFile);
	}

	/* alldone */
	VRETURN(OK);
}

/* first pass program non-terminal */
void
fp_program()
{
	ENTRY("fp_program");
	/* look for function and procedure declarations */
	getToken();
	while (tokenType != T_ERR && tokenType != T_EOF)
	{
		/* check for functions or procedures */
		switch (tokenType)
		{
		case T_FUNCTION:
			fp_funcdecl();
			break;

		case T_PROCEDURE:
			fp_procdecl();
			break;

		default:
			/* skip other tokens */
			acceptToken(tokenType);
			break;
		}

		/* get next token */
		getToken();
	}

	/* all done */
	RETURN;
}

/* first pass function declaration non-terminal */
void
fp_funcdecl()
{
	SYMBOL funcSymbol;

	ENTRY("fp_funcdecl");
	/* initialize fields in symbol table entry for function */
	funcSymbol.name[0] = 0;         /* name is stored later */
	funcSymbol.offset = 0;          /* for now */
	funcSymbol.argOffset = 0;       /* for now */
	funcSymbol.level = -1;          /* out of range value for now */
	funcSymbol.type = S_ERROR;      /* unknown for now */
	funcSymbol.kind = K_FUNCTION;   /* function type */
	funcSymbol.isReference = NO;    /* only for parameters */
	funcSymbol.isParameter = NO;    /* only for parameters */
	funcSymbol.dimension = 0;       /* unknown for now */
	funcSymbol.nextParameter = -1;  /* unknown for now */

	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_FUNCTION:
		/* accept function */
		acceptToken(T_FUNCTION);
		break;

	default:
		/* bad syntax */
		listPrint("ERROR: funcdecl -> <function> ident ( params ) : typeid ; block ident ;.\n");
		listPrint("expecting function.\n");
		suicide();
	}

	/* check for function name */
	getToken();
	if (tokenType != T_ID)
	{
		listPrint("ERROR: funcdecl -> function <ident> ( params ) : typeid ; block ident ;.\n");
		listPrint("expecting ident.\n");
		suicide();
	}
	strcpy(funcSymbol.name, tokenString);   /* store function name */
	acceptToken(T_ID);

	/* looking for a parameter list */
	getToken();
	if (tokenType != T_LP)
	{
		listPrint("ERROR: funcdecl -> function ident <(> params ) : typeid ; block ident ;.\n");
		listPrint("expecting left parenthesis.\n");
		suicide();
	}
	acceptToken(T_LP);
	fp_params(&funcSymbol);
	getToken();
	if (tokenType != T_RP)
	{
		listPrint("ERROR: funcdecl -> function ident ( params <)> : typeid ; block ident ;.\n");
		listPrint("expecting right parenthesis.\n");
		suicide();
	}
	acceptToken(T_RP);
	getToken();
	if (tokenType != T_COLON)
	{
		listPrint("ERROR: funcdecl -> function ident ( params ) <:> typeid ; block ident ;.\n");
		listPrint("expecting colon.\n");
		suicide();
	}
	acceptToken(T_COLON);
	funcSymbol.type = fp_typeid();
	getToken();
	if (tokenType != T_SEMICOLON)
	{
		listPrint("ERROR: funcdecl -> function ident ( params ) : typeid <;> block ident ;.\n");
		listPrint("expecting semicolon.\n");
		suicide();
	}
	acceptToken(T_SEMICOLON);

	/* update offsets in function parameters */
	if (updateParamOffsets(&funcSymbol) != OK)
	{
		semanticErrors++;
		errorPrint(__LINE__, "SEMANTIC ERROR: unable to update offsets for function (%s).\n",
			   funcSymbol.name);
	}

	/* insert function symbol table entry */
	switch (insertEntry(&funcSymbol, NOPARAMETERS))
	{
	case OK:
		/* success */
		break;
	case DUPLICATE:
		/* duplicate entry in symbol table */
		semanticErrors++;
		errorPrint(__LINE__, "SEMANTIC ERROR: function name (%s) redefined.\n", funcSymbol.name);
		break;
	default:
		/* unknown error */
		errorPrint(__LINE__, "insertEntry failed.\n");
		suicide();
	}

	/* all done */
	RETURN;
}

/* first pass procedure declaration non-terminal */
void
fp_procdecl()
{
	SYMBOL funcSymbol;

	ENTRY("fp_procdecl");
	/* initialize fields in symbol table entry for procedure */
	funcSymbol.name[0] = 0;         /* name is stored later */
	funcSymbol.offset = 0;          /* for now */
	funcSymbol.argOffset = 0;       /* for now */
	funcSymbol.level = -1;          /* out of range value for now */
	funcSymbol.type = S_VOID;       /* just a procedure */
	funcSymbol.kind = K_PROCEDURE;  /* procedure type */
	funcSymbol.isReference = NO;    /* only for parameters */
	funcSymbol.isParameter = NO;    /* only for parameters */
	funcSymbol.dimension = 0;       /* unknown for now */
	funcSymbol.nextParameter = -1;  /* unknown for now */

	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_PROCEDURE:
		/* accept procedure */
		acceptToken(T_PROCEDURE);
		break;

	default:
		/* bad syntax */
		listPrint("ERROR: procdecl -> <procedure> ident ( params ) ; block ident ;.\n");
		listPrint("expecting procedure.\n");
		suicide();
	}

	/* check for procedure name */
	getToken();
	if (tokenType != T_ID)
	{
		listPrint("ERROR: procdecl -> procedure <ident> ( params ) ; block ident ;.\n");
		listPrint("expecting ident.\n");
		suicide();
	}
	strcpy(funcSymbol.name, tokenString);   /* store function name */
	acceptToken(T_ID);

	/* looking for a parameter list */
	getToken();
	if (tokenType != T_LP)
	{
		listPrint("ERROR: procdecl -> procedure ident <(> params ) ; block ident ;.\n");
		listPrint("expecting left parenthesis.\n");
		suicide();
	}
	acceptToken(T_LP);
	fp_params(&funcSymbol);
	getToken();
	if (tokenType != T_RP)
	{
		listPrint("ERROR: procdecl -> procedure ident ( params <)> ; block ident ;.\n");
		listPrint("expecting right parenthesis.\n");
		suicide();
	}
	acceptToken(T_RP);
	getToken();
	if (tokenType != T_SEMICOLON)
	{
		listPrint("ERROR: procdecl -> procedure ident ( params ) <;> block ident ;.\n");
		listPrint("expecting right parenthesis.\n");
		suicide();
	}
	acceptToken(T_SEMICOLON);

	/* update offsets in function parameters */
	if (updateParamOffsets(&funcSymbol) != OK)
	{
		semanticErrors++;
		errorPrint(__LINE__, "SEMANTIC ERROR: unable to update offsets for function (%s).\n",
			   funcSymbol.name);
	}

	/* insert function symbol table entry */
	switch (insertEntry(&funcSymbol, NOPARAMETERS))
	{
	case OK:
		/* success */
		break;
	case DUPLICATE:
		/* duplicate entry in symbol table */
		semanticErrors++;
		errorPrint(__LINE__, "SEMANTIC ERROR: procedure name (%s) redefined.\n", funcSymbol.name);
		break;
	default:
		/* unknown error */
		errorPrint(__LINE__, "insertEntry failed.\n");
		suicide();
	}

	/* all done */
	RETURN;
}

/* first pass parameters nonterminal */
void
fp_params(funcSymbol)
SYMBOL *funcSymbol;
{
	int minParam, maxParam, absMin, absMax;

	ENTRY("fp_params");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_ID:
	case T_PTR:
		/* parameter list nonterminal */
		fp_paramlist(funcSymbol, &minParam, &maxParam);
		absMin = minParam;
		absMax = maxParam;

		/* get token */
		getToken();

		/* check for more parameter lists */
		while (tokenType == T_SEMICOLON)
		{
			/* accept semicolon and call parameter list nonterminal */
			acceptToken(T_SEMICOLON);
			fp_paramlist(funcSymbol, &minParam, &maxParam);
			if (absMin == -1) absMin = minParam;
			if (absMax == -1) absMax = maxParam;
			if (maxParam > absMax) absMax = maxParam;

			/* next token */
			getToken();
		}

		/* setup link list for parameter list */
		funcSymbol->nextParameter = absMin;
		updateNextParameter(absMin, absMax);
		break;

	default:
		/* empty production is allowed */
		break;
	}

	/* all done */
	RETURN;
}

/* first pass parameter list nonterminal */
void
fp_paramlist(funcSymbol, minParam, maxParam)
SYMBOL *funcSymbol;
int *minParam, *maxParam;
{
	int thisParam;
	TYPE paramType;

	ENTRY("fp_paramlist");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_ID:
	case T_PTR:
		/* parameter item nonterminal */
		*minParam = *maxParam = fp_paramitem(funcSymbol);

		/* get token */
		getToken();

		/* check for more variable declaration items */
		while (tokenType == T_COMMA)
		{
			/* accept comma, call parameter item nonterminal */
			acceptToken(T_COMMA);
			thisParam = fp_paramitem(funcSymbol);

			/* check for bad parameters */
			if (*minParam == -1) *minParam = thisParam;
			if (*maxParam == -1) *maxParam = thisParam;
			if (thisParam > *maxParam) *maxParam = thisParam;

			/* get next token */
			getToken();
		}

		/* look for a colon */
		getToken();
		if (tokenType != T_COLON)
		{
			listPrint("ERROR: paramlist -> paramitem { , paramitem } <:> typeid.\n");
			listPrint("expecting colon.\n");
			suicide();
		}
		acceptToken(T_COLON);

		/* typeid nonterminal */
		paramType = fp_typeid();

		/* update symbol table type for parameters */
		updateSymbolType(*minParam, *maxParam, paramType);
		break;

	default:
		/* bad syntax */
		listPrint("ERROR: paramlist -> <paramitem> { , paramitem } : typeid.\n");
		listPrint("expecting ident for paramitem.\n");
		suicide();
	}

	/* all done */
	RETURN;
}

/* first pass parameter item nonterminal */
int
fp_paramitem(funcSymbol)
SYMBOL *funcSymbol;
{
	SYMBOL paramSymbol;

	ENTRY("fp_paramitem");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_PTR:
		/* accept pointer */
		acceptToken(T_PTR);

		/* next token */
		getToken();

		/* check for an identifier */
		if (tokenType != T_ID)
		{
			listPrint("ERROR: paramitem -> ^ <ident> | ident | ident[].\n");
			listPrint("expecting ident.\n");
			suicide();
		}

		/* store symbol information */
		strcpy(paramSymbol.name, tokenString);      
		paramSymbol.offset = 0;       
		paramSymbol.argOffset = 0;
		paramSymbol.level = -1;
		paramSymbol.type = S_ERROR;   
		paramSymbol.kind = K_SIMPLE;
		paramSymbol.isReference = YES;
		paramSymbol.isParameter = YES;
		paramSymbol.dimension = 0;
		paramSymbol.nextParameter = -1;

		/* insert parameter symbol table entry */
		switch (insertEntry(&paramSymbol, ONLYPARAMETERS))
		{          
		case OK:
			/* success */
			funcSymbol->dimension++;
			break;
		case DUPLICATE:
			/* duplicate entry in symbol table */
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: parameter name (%s) redefined.\n", paramSymbol.name);
			break;
		default:
			/* unknown error */
			errorPrint(__LINE__, "insertEntry failed.\n");
			suicide();
		}

		/* accept identifier */
		acceptToken(T_ID);
		break;

	case T_ID:
		/* store symbol information */
		strcpy(paramSymbol.name, tokenString);      
		paramSymbol.offset = 0;       
		paramSymbol.argOffset = 0;
		paramSymbol.level = -1;
		paramSymbol.type = S_ERROR;   
		paramSymbol.kind = K_SIMPLE;
		paramSymbol.isReference = NO;
		paramSymbol.isParameter = YES;
		paramSymbol.dimension = 0;
		paramSymbol.nextParameter = -1;

		/* accept identifier */
		acceptToken(T_ID);

		/* look for a left bracket, if any */
		getToken();
		if (tokenType != T_LB)
		{
			/* not an array, all done */
			switch (insertEntry(&paramSymbol, ONLYPARAMETERS))
			{          
			case OK:
				/* success */
				funcSymbol->dimension++;
				break;
			case DUPLICATE:
				/* duplicate entry in symbol table */
				semanticErrors++;
				errorPrint(__LINE__, "SEMANTIC ERROR: parameter name (%s) redefined.\n", paramSymbol.name);
				break;
			default:
				/* unknown error */
				errorPrint(__LINE__, "insertEntry failed.\n");
				suicide();
			}
			VRETURN(paramSymbol.entry);
		}
		acceptToken(T_LB);

		/* get remaining part of array */
		getToken();
		if (tokenType != T_RB)
		{
			listPrint("ERROR: paramitem -> ^ ident | ident | ident[<]>.\n");
			listPrint("expecting right bracket.\n");
			suicide();
		}
		acceptToken(T_RB);

		/* we have an array */
		paramSymbol.kind = K_ARRAY;
		paramSymbol.isReference = YES;

		/* insert parameter symbol table entry */
		switch (insertEntry(&paramSymbol, ONLYPARAMETERS))
		{          
		case OK:
			/* success */
			funcSymbol->dimension++;
			break;
		case DUPLICATE:
			/* duplicate entry in symbol table */
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: parameter name (%s) redefined.\n", paramSymbol.name);
			break;
		default:
			/* unknown error */
			errorPrint(__LINE__, "insertEntry failed.\n");
			suicide();
		}
		break;

	default:
		/* bad syntax */
		listPrint("ERROR: paramitem -> <^> ident | <ident> | <ident>[].\n");
		listPrint("expecting ident or reference.\n");
		suicide();
	}

	/* all done */
	VRETURN(paramSymbol.entry);
}

/* first pass type id nonterminal */
TYPE
fp_typeid()
{
	TYPE returnType;

	ENTRY("fp_typeid");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_ID:
		/* accept ident */
		if (strcmp(tokenString, "integer") == 0)
		{
			returnType = S_INTEGER;
		}
		else if (strcmp(tokenString, "boolean") == 0)
		{
			returnType = S_BOOLEAN;
		}
		else
		{
			returnType = S_ERROR;
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: invalid type id (%s).\n", tokenString);
		}
		acceptToken(T_ID);
		break;

	default:
		/* bad syntax */
		listPrint("ERROR: typeid -> <ident>.\n");
		listPrint("expecting ident.\n");
		suicide();
	}

	/* all done */
	VRETURN(returnType);
}

/* function for program non-terminal */
void
program()
{
	int offset;

	ENTRY("program");
	/* get token */
	getToken();

	/* start of program code */
	asmComment("starting an NPM program, declare stack size and model.");
	asmWrite("\t.DOSSEG\n");
	asmWrite("\n\t.MODEL\tSMALL\n");
	asmWrite("\n@STACKSIZE\tEQU\t0200H\n");
	asmWrite("\t.STACK\t@STACKSIZE\n");

	/* declare any externs for functions */
	asmComment("declare externs for system routines.");
	asmWrite("\tEXTRN\t_getnum:FAR\n");
	asmWrite("\tEXTRN\t_writestr:FAR\n");
	asmWrite("\tEXTRN\t_newline:FAR\n");
	asmWrite("\tEXTRN\t_writeint:FAR\n");
	asmWrite("\tEXTRN\t_writebool:FAR\n");

	/* declare display */
	asmWrite("\n\t.DATA\n");
	asmComment("data section, declare display, and temp area.");
	asmWrite("@DISPLAYSIZE\tEQU\t010H\n");
	asmWrite("@DISPLAY\tdw\t@DISPLAYSIZE dup(0)\n");
	asmWrite("@SAVERETURN\tdw\t0\n");

	/* assume no global variables */
	offset = 0;

	/* check starter symbols */
	switch (tokenType)
	{
	case T_DECLARE:
		/* global variable declarations nonterminal */
		offset = vardecl();

		/* check for function or procedure declarations */
		getToken();
		while (tokenType == T_FUNCTION || tokenType == T_PROCEDURE)
		{
			/* function or procedure nonterminal */
			switch (tokenType)
			{
			case T_PROCEDURE:
				procdecl();
				break;

			case T_FUNCTION:
				funcdecl();
				break;
			}

			/* next token */
			getToken();
		}

		/* start main program */
		asmWrite("\n\t.CODE\n");
		asmComment("start main entry point.");
		asmWrite("_MAIN\tPROC\tFAR\n\n");
		asmComment("push sp, si, di; save bp in display.");
		asmWrite("\tpush\tsp\n");
		asmWrite("\tmov\tbp, sp\n");
		asmWrite("\tmov\tax, @DATA\n");
		asmWrite("\tmov\tds, ax\n");
		asmWrite("\tpush\tsi\n");
		asmWrite("\tpush\tdi\n");
		asmWrite("\tmov\tdi, offset @DISPLAY\n");
		asmWrite("\tmov\t[di], bp\n");

		/* set up global variables on stack */
		asmComment("declare global parameters in stack.");
		asmWrite("\tsub\tsp, %ld\n", (long)offset);

		/* set up block info for main program */
		strcpy(blockNesting[level].name, "MAIN");
		blockNesting[level].isFunction = YES;
		blockNesting[level].size = offset;

		/* program body nonterminal */
		progbody();
		break;

	case T_PROCEDURE:
	case T_FUNCTION:
		/* check for function or procedure declarations */
		getToken();
		while (tokenType == T_FUNCTION || tokenType == T_PROCEDURE)
		{
			/* function or procedure nonterminal */
			switch (tokenType)
			{
			case T_PROCEDURE:
				procdecl();
				break;

			case T_FUNCTION:
				funcdecl();
				break;
		       }

			/* next token */
			getToken();
		}

		/* start main program */
		asmWrite("\t.CODE\n");
		asmComment("start main entry point.");
		asmWrite("_MAIN\tPROC\tFAR\n\n");
		asmComment("push sp, si, di; save bp in display.");
		asmWrite("\tpush\tsp\n");
		asmWrite("\tmov\tbp, sp\n");
		asmWrite("\tmov\tax, @DATA\n");
		asmWrite("\tmov\tds, ax\n");
		asmWrite("\tpush\tsi\n");
		asmWrite("\tpush\tdi\n");
		asmWrite("\tmov\tdi, offset @DISPLAY\n");
		asmWrite("\tmov\t[di], bp\n");

		/* set up block info for main program */
		strcpy(blockNesting[level].name, "MAIN");
		blockNesting[level].isFunction = YES;
		blockNesting[level].size = offset;

		/* program body nonterminal */
		progbody();
		break;

	case T_BEGIN:
		/* start main program */
		asmWrite("\t.CODE\n");
		asmComment("start main entry point.");
		asmWrite("_MAIN\tPROC\tFAR\n\n");
		asmComment("push sp, si, di; save bp in display.");
		asmWrite("\tpush\tsp\n");
		asmWrite("\tmov\tbp, sp\n");
		asmWrite("\tmov\tax, @DATA\n");
		asmWrite("\tmov\tds, ax\n");
		asmWrite("\tpush\tsi\n");
		asmWrite("\tpush\tdi\n");
		asmWrite("\tmov\tdi, offset @DISPLAY\n");
		asmWrite("\tmov\t[di], bp\n");

		/* set up block info for main program */
		strcpy(blockNesting[level].name, "MAIN");
		blockNesting[level].isFunction = YES;
		blockNesting[level].size = offset;

		/* program body nonterminal */
		progbody();
		break;

	default:
		listPrint("ERROR: program -> <vardecl> { <funcdecl> | <funcdecl> } <programbody>.\n");
		listPrint("expecting declare, procedure, function, or begin.\n");
		suicide();
	}

	/* end of program code */
	asmComment("exit from main.");
	asmWrite("\n@_MAIN:\n");
	if (offset > 0)
	{
		asmComment("deallocate global variables.");
		asmWrite("\tadd\tsp, %ld\n", (long)offset);
	}
	asmComment("pop di, si, sp; call DOS exit.");
	asmWrite("\tpop\tdi\n");
	asmWrite("\tpop\tsi\n");
	asmWrite("\tpop\tsp\n");
	asmWrite("\tmov\tal, 0H\n");
	asmWrite("\tmov\tah, 04CH\n");
	asmWrite("\tint\t021H\n");
	asmWrite("\n_MAIN\tENDP\n");

	/* write string data */
	writeStrings();

	/* end of file */
	asmWrite("\tEND\t_MAIN\n");

	/* all done */
	RETURN;
}

/* variable declarations nonterminal */
int
vardecl()
{
	int offset, minVar, maxVar;

	ENTRY("vardecl");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_DECLARE:
		/* accept token and variable declarations list nonterminal */
		acceptToken(T_DECLARE);
		maxVar = INT_MIN;
		minVar = INT_MAX;
		vardecllist(&minVar, &maxVar);

		/* update variable offsets in block */
		if ((offset = updateVarOffsets(minVar, maxVar)) <= 0)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: updateVarOffsets failed.\n");
		}
		break;

	default:
		/* empty production is allowed */
		offset = 0;
		break;
	}

	/* all done */
	VRETURN(offset);
}

/* program body nonterminal */
void
progbody()
{
	ENTRY("progbody");
	/* get token */
	getToken();

	/* set up info for optional return statements */
	returnInfo.type = S_VOID;
	returnInfo.kind = K_PROCEDURE;
	numberOfReturns = 0;

	/* check starter symbol */
	switch (tokenType)
	{
	case T_BEGIN:
		/* accept token and call statemest list nonterminal */
		acceptToken(T_BEGIN);
		stmtlist();
		getToken();
		if (tokenType != T_END)
		{
			listPrint("ERROR: progbody -> begin stmtlist <end> ..\n");
			listPrint("expecting end.\n");
			suicide();
		}
		acceptToken(T_END);
		getToken();
		if (tokenType != T_PER)
		{
			listPrint("ERROR: progbody -> begin stmtlist end <.>.\n");
			listPrint("expecting period.\n");
			suicide();
		}
		acceptToken(T_PER);
		break;

	default:
		/* bad syntax */
		listPrint("ERROR: progbody -> <begin> stmtlist end ..\n");
		listPrint("expecting begin.\n");
		suicide();
	}

	/* all done */
	RETURN;
}

/* variable declarations list nonterminal */
void
vardecllist(minVar, maxVar)
int *minVar, *maxVar;
{
	int min, max, this;
	TYPE type;

	ENTRY("vardecllist");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_ID:
		/* variable declarations item nonterminal */
		min = max = vardeclitem();

		/* get token */
		getToken();

		/* check for more variable declaration items */
		while (tokenType == T_COMMA)
		{
			/* accept comma and variable declaration item nonterminal */
			acceptToken(T_COMMA);
			this = vardeclitem();
			if (min == -1) min = this;
			if (max == -1) max = this;
			if (this > max) max = this;

			/* get next token */
			getToken();
		}

		/* look for a colon */
		getToken();
		if (tokenType != T_COLON)
		{
			listPrint("ERROR: vardecllist -> vadeclitem { , vardeclitem } <:> typeid ; morevars.\n");
			listPrint("expecting colon.\n");
			suicide();
		}
		acceptToken(T_COLON);

		/* typeid nonterminal */
		type = typeid();

		/* look for a semicolon */
		getToken();
		if (tokenType != T_SEMICOLON)
		{
			listPrint("ERROR: vardecllist -> vardeclitem { , vardeclitem } : typeid <;> morevars.\n");
			listPrint("expecting semicolon.\n");
			suicide();
		}
		acceptToken(T_SEMICOLON);

		/* update the symbol type for identifiers */
		updateSymbolType(min, max, type);

		/* reset start and end of variable declarations */
		if (min < *minVar) *minVar = min;
		if (max > *maxVar) *maxVar = max;

		/* get token and check for more variables */
		getToken();
		if (tokenType == T_ID)
		{
			/* more variables nonterminal */
			morevars(minVar, maxVar);
		}
		break;

	default:
		/* bad syntax */
		listPrint("ERROR: vardecllist -> <vardeclitem> { , vardeclitem } : typeid ; morevars.\n");
		listPrint("expecting ident forvardeclitem.\n");
		suicide();
	}

	/* all done */
	RETURN;
}

/* more variables nonterminal */
void
morevars(minVar, maxVar)
int *minVar, *maxVar;
{
	ENTRY("morevars");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_ID:
		/* call variable declaration list nonterminal */
		vardecllist(minVar, maxVar);
		break;

	default:
		/* empty production is allowed */
		break;
	}

	/* all done */
	RETURN;
}

/* variable declaration item nonterminal */
int
vardeclitem()
{
	SYMBOL varSymbol;

	ENTRY("vardeclitem");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_ID:
		/* store symbol information */
		strcpy(varSymbol.name, tokenString);      
		varSymbol.offset = 0;       
		varSymbol.argOffset = 0;
		varSymbol.level = -1;
		varSymbol.type = S_ERROR;   
		varSymbol.kind = K_SIMPLE;
		varSymbol.isReference = NO;
		varSymbol.isParameter = NO;
		varSymbol.dimension = 0;
		varSymbol.nextParameter = -1;

		/* accept identifier */
		acceptToken(T_ID);

		/* check if we have an array */
		getToken();
		if (tokenType != T_LB)
		{
			/* not an array, all done */
			switch (insertEntry(&varSymbol, NOPARAMETERS))
			{          
			case OK:
				/* success */
				break;
			case DUPLICATE:
				/* duplicate entry in symbol table */
				semanticErrors++;
				errorPrint(__LINE__, "SEMANTIC ERROR: variable name (%s) redefined.\n", varSymbol.name);
				break;
			default:
				/* unknown error */
				errorPrint(__LINE__, "insertEntry failed.\n");
				suicide();
			}
			VRETURN(varSymbol.entry);
		}
		acceptToken(T_LB);

		/* get array size */
		getToken();
		if (tokenType != T_NUM)
		{
			listPrint("ERROR: vardeclitem -> ident | ident [ <unsignednum> ].\n");
			listPrint("expecting unsigned number.\n");
			suicide();
		}
		if (tokenValue <= 0)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: array size (%d) must be greater than 0.\n",
				tokenValue);
		}
		varSymbol.kind = K_ARRAY;
		varSymbol.dimension = tokenValue;
		acceptToken(T_NUM);

		/* get closing bracket */
		getToken();
		if (tokenType != T_RB)
		{
			listPrint("ERROR: vardeclitem -> ident | ident [ unsignednum <]>.\n");
			listPrint("expecting right bracke.\n");
			suicide();
		}
		acceptToken(T_RB);

		/* insert variable symbol table entry */
		switch (insertEntry(&varSymbol, NOPARAMETERS))
		{          
		case OK:
			/* success */
			break;
		case DUPLICATE:
			/* duplicate entry in symbol table */
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: variable name (%s) redefined.\n", varSymbol.name);
			break;
		default:
			/* unknown error */
			errorPrint(__LINE__, "insertEntry failed.\n");
			suicide();
		}
		break;

	default:
		/* bad syntax */
		listPrint("ERROR: vardeclitem -> <ident> | <ident> [ unsignednum ].\n");
		listPrint("expecting ident.\n");
		suicide();
	}

	/* all done */
	VRETURN(varSymbol.entry);
}


/* type id nonterminal */
TYPE
typeid()
{
	TYPE returnType;

	ENTRY("typeid");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_ID:
		/* accept ident */
		if (strcmp(tokenString, "integer") == 0)
		{
			returnType = S_INTEGER;
		}
		else if (strcmp(tokenString, "boolean") == 0)
		{
			returnType = S_BOOLEAN;
		}
		else
		{
			returnType = S_ERROR;
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: invalid type id (%s).\n", tokenString);
		}
		acceptToken(T_ID);
		break;

	default:
		/* bad syntax */
		listPrint("ERROR: typeid -> <ident>.\n");
		listPrint("expecting ident.\n");
		suicide();
	}

	/* all done */
	VRETURN(returnType);
}

/* procedure declaration nonterminal */
void
procdecl()
{
	SYMBOL funcSym;
	char funcName[MAXIDENTSIZE+1];

	ENTRY("procdecl");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_PROCEDURE:
		/* accept procedure */
		acceptToken(T_PROCEDURE);
		break;

	default:
		/* bad syntax */
		listPrint("ERROR: procdecl -> <procedure> ident ( params ) ; block ident ;.\n");
		listPrint("expecting procedure.\n");
		suicide();
	}

	/* check for procedure name */
	getToken();
	if (tokenType != T_ID)
	{
		listPrint("ERROR: procdecl -> procedure <ident> ( params ) ; block ident ;.\n");
		listPrint("expecting ident.\n");
		suicide();
	}
	strcpy(funcName, tokenString);
	acceptToken(T_ID);

	/* start a new function */
	asmWrite("\n\n");
	asmWrite("\t.CODE\n");
	asmComment("start a procedure.");
	asmWrite("_%s\tPROC\tNEAR\n\n", funcName);
	asmComment("save si, di, setup display.");
	asmWrite("\tmov\tbp,sp\n");
	asmWrite("\tpush\t@DISPLAY+2\n");
	asmWrite("\tpush\tsi\n");
	asmWrite("\tpush\tdi\n"),
	asmWrite("\tmov\tdi, offset @DISPLAY\n");
	asmWrite("\tmov\t[di]+2, bp\n");

	/* create a new block in symbol table */
	createBlock();

	/* looking for a parameter list */
	getToken();
	if (tokenType != T_LP)
	{
		listPrint("ERROR: procdecl -> procedure ident <(> params ) ; block ident ;.\n");
		listPrint("expecting left parenthesis.\n");
		suicide();
	}
	acceptToken(T_LP);
	params(funcName);
	getToken();
	if (tokenType != T_RP)
	{
		listPrint("ERROR: procdecl -> procedure ident ( params <)> ; block ident ;.\n");
		listPrint("expecting right parenthesis.\n");
		suicide();
	}
	acceptToken(T_RP);
	getToken();
	if (tokenType != T_SEMICOLON)
	{
		listPrint("ERROR: procdecl -> procedure ident ( params ) <;> block ident ;.\n");
		listPrint("expecting right parenthesis.\n");
		suicide();
	}
	acceptToken(T_SEMICOLON);

	/* set up info for return statement checks */
	returnInfo.type = S_VOID;
	returnInfo.kind = K_PROCEDURE;
	numberOfReturns = 0;

	/* looking for a function block */
	block(funcName);
	getToken();
	if (tokenType != T_ID)
	{
		listPrint("ERROR: prodecl -> procedure ident ( params ) ; block <ident> ;.\n");
		listPrint("expecting ident.\n");
		suicide();
	}
	if (strcmp(funcName, tokenString) != 0)
	{
		semanticErrors++;
		errorPrint(__LINE__, 
			"SEMANTIC ERROR: procedure name (%s) different from end name (%s).\n", 
			funcName, tokenString);
	}
	acceptToken(T_ID);
	getToken();
	if (tokenType != T_SEMICOLON)
	{
		listPrint("ERROR: procdecl -> procedure ident ( params ) ; block ident <;>.\n");
		listPrint("expecting right parenthesis.\n");
		suicide();
	}
	acceptToken(T_SEMICOLON);

	/* check number of returns found */
	/* remove this for now
	if (numberOfReturns <= 0)
	{
		semanticErrors++;
		errorPrint(__LINE__, "SEMANTIC ERROR: no returns found for procedure (%s).\n", funcName);
	}
	*/

	/* add end of function processing */
	if (findEntry(funcName, &funcSym, GLOBALBLOCK, NOPARAMETERS) != OK)
	{
		semanticErrors++;
		errorPrint(__LINE__, "SEMANTIC ERROR: procedure (%s) not found in symbol table.\n", funcName);
	}
	else
	{
		asmComment("end of procedure.");
		asmWrite("\n@_%s:\n", funcName);
		asmComment("deallocate auto variables.");
		asmWrite("\tadd\tsp,%ld\n", (long) funcSym.offset);
		asmComment("pop di, si; reset display.");
		asmWrite("\tpop\tdi\n");
		asmWrite("\tpop\tsi\n");
		asmWrite("\tpop\tbp\n");
		asmWrite("\tmov\tbx, offset @DISPLAY+2\n");
		asmWrite("\tmov\tds:[bx], bp\n");
		asmComment("return from procedure and pop parameters.");
		asmWrite("\tret\t%ld\n", (long) funcSym.argOffset);
		asmWrite("\n");
		asmWrite("_%s\tENDP\n", funcName);
	}

	/* dump symbol table block */
	dumpBlock(level);

	/* destroy block */
	destroyBlock();

	/* all done */
	RETURN;
}

/* function declaration nonterminal */
void
funcdecl()
{
	SYMBOL funcSym;
	char funcName[MAXIDENTSIZE+1];

	ENTRY("funcdecl");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_FUNCTION:
		/* accept function */
		acceptToken(T_FUNCTION);
		break;

	default:
		/* bad syntax */
		listPrint("ERROR: funcdecl -> <function> ident ( params ) : typeid ; block ident ;.\n");
		listPrint("expecting function.\n");
		suicide();
	}

	/* check for function name */
	getToken();
	if (tokenType != T_ID)
	{
		listPrint("ERROR: funcdecl -> function <ident> ( params ) : typeid ; block ident ;.\n");
		listPrint("expecting ident.\n");
		suicide();
	}
	strcpy(funcName, tokenString);
	acceptToken(T_ID);

	/* start a new function */
	asmWrite("\n\n");
	asmWrite("\t.CODE\n");
	asmComment("start a function.");
	asmWrite("_%s\tPROC\tNEAR\n\n", funcName);
	asmComment("push si, di; setup display.");
	asmWrite("\tmov\tbp,sp\n");
	asmWrite("\tpush\t@DISPLAY+2\n");
	asmWrite("\tpush\tsi\n");
	asmWrite("\tpush\tdi\n"),
	asmWrite("\tmov\tdi, offset @DISPLAY\n");
	asmWrite("\tmov\t[di]+2, bp\n");

	/* create a new symbol table block */
	createBlock();

	/* looking for a parameter list */
	getToken();
	if (tokenType != T_LP)
	{
		listPrint("ERROR: funcdecl -> function ident <(> params ) : typeid ; block ident ;.\n");
		listPrint("expecting left parenthesis.\n");
		suicide();
	}
	acceptToken(T_LP);
	params(funcName);
	getToken();
	if (tokenType != T_RP)
	{
		listPrint("ERROR: funcdecl -> function ident ( params <)> : typeid ; block ident ;.\n");
		listPrint("expecting right parenthesis.\n");
		suicide();
	}
	acceptToken(T_RP);
	getToken();
	if (tokenType != T_COLON)
	{
		listPrint("ERROR: funcdecl -> function ident ( params ) <:> typeid ; block ident ;.\n");
		listPrint("expecting colon.\n");
		suicide();
	}
	acceptToken(T_COLON);
	returnInfo.type = typeid();
	getToken();
	if (tokenType != T_SEMICOLON)
	{
		listPrint("ERROR: funcdecl -> function ident ( params ) : typeid <;> block ident ;.\n");
		listPrint("expecting semicolon.\n");
		suicide();
	}
	acceptToken(T_SEMICOLON);

	/* set up info for return statement checks */
	returnInfo.kind = K_FUNCTION;
	numberOfReturns = 0;

	/* looking for a function block */
	block(funcName);
	getToken();
	if (tokenType != T_ID)
	{
		listPrint("ERROR: funcdecl -> function ident ( params ) : typeid ; block <ident> ;.\n");
		listPrint("expecting ident.\n");
		suicide();
	}
	if (strcmp(funcName, tokenString) != 0)
	{
		semanticErrors++;
		errorPrint(__LINE__, 
			"SEMANTIC ERROR: function name (%s) different from end name (%s).\n", 
			funcName, tokenString);
	}
	acceptToken(T_ID);
	getToken();
	if (tokenType != T_SEMICOLON)
	{
		listPrint("ERROR: funcdecl -> function ident ( params ) : typeid ; block ident <;>.\n");
		listPrint("expecting semicolon.\n");
		suicide();
	}
	acceptToken(T_SEMICOLON);

	/* check number of returns found */
	if (numberOfReturns <= 0)
	{
		semanticErrors++;
		errorPrint(__LINE__, "SEMANTIC ERROR: no returns found for function (%s).\n", funcName);
	}

	/* add end of function processing */
	if (findEntry(funcName, &funcSym, GLOBALBLOCK, NOPARAMETERS) != OK)
	{
		semanticErrors++;
		errorPrint(__LINE__, "SEMANTIC ERROR: function (%s) not found in symbol table.\n", funcName);
	}
	else
	{
		asmComment("end of function.");
		asmWrite("\n@_%s:\n", funcName);
		asmComment("move return to temp place.");
		asmWrite("\tadd\tsp,%ld\n", (long)funcSym.offset);
		asmWrite("\tmov\tbx, offset @SAVERETURN\n");
		asmWrite("\tmov\tds:[bx], ax\n");
		asmComment("pop di, si, bp; reset display.");
		asmWrite("\tpop\tdi\n");
		asmWrite("\tpop\tsi\n");
		asmWrite("\tpop\tbp\n");
		asmWrite("\tmov\tbx, offset @DISPLAY+2\n");
		asmWrite("\tmov\tds:[bx], bp\n");
		asmComment("move return back to AX register.");
		asmWrite("\tmov\tbx, offset @SAVERETURN\n");
		asmWrite("\tmov\tax, ds:[bx]\n");
		asmComment("return from function and pop parameters.");
		asmWrite("\tret\t%ld\n", (long)funcSym.argOffset);
		asmWrite("\n");
		asmWrite("_%s\tENDP\n", funcName);
	}

	/* dump symbol table block */
	dumpBlock(level);

	/* destroy symbol table block */
	destroyBlock();

	/* all done */
	RETURN;
}

/* parameters nonterminal */
void
params(fname)
char *fname;
{
	int minPar, maxPar;

	ENTRY("params");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_ID:
	case T_PTR:
		/* parameter list nonterminal */
		minPar = INT_MAX;
		maxPar = INT_MIN;
		paramlist(&minPar, &maxPar);

		/* get token */
		getToken();

		/* check for more parameter lists */
		while (tokenType == T_SEMICOLON)
		{
			/* accept semicolon and call parameter list nonterminal */
			acceptToken(T_SEMICOLON);
			paramlist(&minPar, &maxPar);

			/* next token */
			getToken();
		}

		/* update parameter offsets */
		setParamOffsets(minPar, maxPar);
		break;

	default:
		/* empty production is allowed */
		break;
	}

	/* all done */
	RETURN;
}

/* parameter list nonterminal */
void
paramlist(minPar, maxPar)
int *minPar, *maxPar;
{
	int min, max, this;
	TYPE type;

	ENTRY("paramlist");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_ID:
	case T_PTR:
		/* parameter item nonterminal */
		min = max = paramitem();

		/* get token */
		getToken();

		/* check for more variable declaration items */
		while (tokenType == T_COMMA)
		{
			/* accept comma, call parameter item nonterminal */
			acceptToken(T_COMMA);
			this = paramitem();
			if (min == -1) min = this;
			if (max == -1) max = this;
			if (this > max) max = this;

			/* get next token */
			getToken();
		}

		/* look for a colon */
		getToken();
		if (tokenType != T_COLON)
		{
			listPrint("ERROR: paramlist -> paramitem { , paramitem } <:> typeid.\n");
			listPrint("expecting colon.\n");
			suicide();
		}
		acceptToken(T_COLON);

		/* typeid nonterminal */
		type = typeid();

		/* update symbol type */
		updateSymbolType(min, max, type);

		/* set minimum and maximum parameters */
		if (min < *minPar) *minPar = min;
		if (max > *maxPar) *maxPar = max;
		break;

	default:
		/* bad syntax */
		listPrint("ERROR: paramlist -> <paramitem> { , paramitem } : typeid.\n");
		listPrint("expecting ident for paramitem.\n");
		suicide();
	}

	/* all done */
	RETURN;
}

/* parameter item nonterminal */
int
paramitem()
{
	SYMBOL paramSymbol;

	ENTRY("paramitem");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_PTR:
		/* accept pointer */
		acceptToken(T_PTR);

		/* next token */
		getToken();

		/* check for an identifier */
		if (tokenType != T_ID)
		{
			listPrint("ERROR: paramitem -> ^ <ident> | ident | ident[].\n");
			listPrint("expecting ident.\n");
			suicide();
		}

		/* store symbol information */
		strcpy(paramSymbol.name, tokenString);      
		paramSymbol.offset = 0;       
		paramSymbol.argOffset = 0;
		paramSymbol.level = -1;
		paramSymbol.type = S_ERROR;   
		paramSymbol.kind = K_SIMPLE;
		paramSymbol.isReference = YES;

		/* parameters are treated as "regular variables" in second pass */
		paramSymbol.isParameter = NO;
		paramSymbol.dimension = 0;
		paramSymbol.nextParameter = -1;

		/* insert parameter symbol table entry */
		switch (insertEntry(&paramSymbol, NOPARAMETERS))
		{          
		case OK:
			/* success */
			break;
		case DUPLICATE:
			/* duplicate entry in symbol table */
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: parameter name (%s) redefined.\n", paramSymbol.name);
			break;
		default:
			/* unknown error */
			errorPrint(__LINE__, "insertEntry failed.\n");
			suicide();
		}
		acceptToken(T_ID);
		break;

	case T_ID:
		/* store symbol information */
		strcpy(paramSymbol.name, tokenString);      
		paramSymbol.offset = 0;       
		paramSymbol.argOffset = 0;
		paramSymbol.level = -1;
		paramSymbol.type = S_ERROR;   
		paramSymbol.kind = K_SIMPLE;
		paramSymbol.isReference = NO;

		/* parameters are treated as "regular variables" in second pass */
		paramSymbol.isParameter = NO;
		paramSymbol.dimension = 0;
		paramSymbol.nextParameter = -1;

		/* accept identifier */
		acceptToken(T_ID);

		/* look for a left bracket, if any */
		getToken();
		if (tokenType != T_LB)
		{
			/* not an array, all done */
			switch (insertEntry(&paramSymbol, NOPARAMETERS))
			{          
			case OK:
				/* success */
				break;
			case DUPLICATE:
				/* duplicate entry in symbol table */
				semanticErrors++;
				errorPrint(__LINE__, "SEMANTIC ERROR: parameter name (%s) redefined.\n", paramSymbol.name);
				break;
			default:
				/* unknown error */
				errorPrint(__LINE__, "insertEntry failed.\n");
				suicide();
			}
			VRETURN(paramSymbol.entry);
		}
		acceptToken(T_LB);

		/* get remaining part of array */
		getToken();
		if (tokenType != T_RB)
		{
			listPrint("ERROR: paramitem -> ^ ident | ident | ident[<]>.\n");
			listPrint("expecting right bracket.\n");
			suicide();
		}
		acceptToken(T_RB);

		/* we have an array */
		paramSymbol.kind = K_ARRAY;
		paramSymbol.isReference = YES;

		/* insert parameter symbol table entry */
		switch (insertEntry(&paramSymbol, NOPARAMETERS))
		{          
		case OK:
			/* success */
			break;
		case DUPLICATE:
			/* duplicate entry in symbol table */
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: parameter name (%s) redefined.\n", paramSymbol.name);
			break;
		default:
			/* unknown error */
			errorPrint(__LINE__, "insertEntry failed.\n");
			suicide();
		}
		break;

	default:
		/* bad syntax */
		listPrint("ERROR: paramitem -> <^> ident | <ident> | <ident>[].\n");
		listPrint("expecting ident or reference.\n");
		suicide();
	}

	/* all done */
	VRETURN(paramSymbol.entry);
}

/* block nonterminal */
void
block(funcName)
char *funcName;
{
	SYMBOL funcSym;
	int offset;

	ENTRY("block");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_DECLARE:
		/* variable declarations */
		offset = vardecl();

		/* update stack pointer */
		asmComment("allocate auto variables.");
		asmWrite("\tsub\tsp, %ld\n", (long)offset);

		/* if a function name was given, update offset */
		if (funcName != (char *)0)
		{
			/* find symbol table entry */
			if (findEntry(funcName, &funcSym, GLOBALBLOCK, NOPARAMETERS) != OK)
			{
				semanticErrors++;
				errorPrint(__LINE__, "SEMANTIC ERROR: function name (%s) not found\n", 
					   funcName);
			}
			else
			{
				/* update offset in function entry */
				funcSym.offset = offset;
				updateEntry(&funcSym);
			}

			/* update block nesting */
			strcpy(blockNesting[level].name, funcName);
			blockNesting[level].isFunction = YES;
			blockNesting[level].size = offset;
		}
		else
		{
			/* update block nesting */
			blockNesting[level].name[0] = (char)0;
			blockNesting[level].isFunction = NO;
			blockNesting[level].size = offset;
		}

		/* next token */
		getToken();

		/* verify it is a begin */
		if (tokenType != T_BEGIN)
		{
			listPrint("ERROR: block -> vardecl <begin> stmtlist end.\n");
			listPrint("expecting begin.\n");
			suicide();
		}
		acceptToken(T_BEGIN);

		/* call statement list nonterminal */
		stmtlist();
		getToken();
		if (tokenType != T_END)
		{
			listPrint("ERROR: block -> vardecl begin stmtlist <end>.\n");
			listPrint("expecting end.\n");
			suicide();
		}
		acceptToken(T_END);
		break;

	case T_BEGIN:
		/* accept token */
		acceptToken(T_BEGIN);

		/* if a function name was given, update offset */
		offset = 0;
		if (funcName != (char *)0)
		{
			/* find symbol table entry */
			if (findEntry(funcName, &funcSym, GLOBALBLOCK, NOPARAMETERS) != OK)
			{
				semanticErrors++;
				errorPrint(__LINE__, "SEMANTIC ERROR: function name (%s) not found\n", 
					   funcName);
			}
			else
			{
				/* update offset in function entry */
				funcSym.offset = offset;
				updateEntry(&funcSym);
			}

			/* update block nesting */
			strcpy(blockNesting[level].name, funcName);
			blockNesting[level].isFunction = YES;
			blockNesting[level].size = offset;
		}
		else
		{
			/* update block nesting */
			blockNesting[level].name[0] = (char)0;
			blockNesting[level].isFunction = NO;
			blockNesting[level].size = offset;
		}

		/* get statements in block */
		stmtlist();
		getToken();
		if (tokenType != T_END)
		{
			listPrint("ERROR: block -> vardecl begin stmtlist <end>.\n");
			listPrint("expecting end.\n");
			suicide();
		}
		acceptToken(T_END);
		break;

	default:
		/* bad syntax */
		listPrint("ERROR: block -> <vardecl> begin stmtlist end.\n");
		listPrint("expecting vardecl.\n");
		suicide();
	}

	/* al done */
	RETURN;
}

/* statement list nonterminal */
void
stmtlist()
{
	ENTRY("stmtlist");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_ID:
	case T_LOOP:
	case T_WHEN:
	case T_INPUT:
	case T_OUTPUT:
	case T_IF:
	case T_RETURN:
	case T_DECLARE:
	case T_BEGIN:
	case T_CALL:
	case T_FOR:
		/* statement nonterminal */
		stmt();

		/* get token */
		getToken();

		/* check for more parameter lists */
		while (tokenType == T_SEMICOLON)
		{
			/* accept semicolon and statement nonterminal */
			acceptToken(T_SEMICOLON);
			stmt();

			/* next token */
			getToken();
		}
		break;

	default:
		/* empty production is allowed */
		break;
	}

	/* all done */
	RETURN;
}

/* statement nonterminal */
void
stmt()
{
	ENTRY("stmt");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_ID:
		assignment();
		break;
	case T_LOOP:
		loopstmt();
		break;
	case T_WHEN:
		whenclause();
		break;
	case T_INPUT:
		inputstmt();
		break;
	case T_OUTPUT:
		outputstmt();
		break;
	case T_IF:
		ifstmt();
		break;
	case T_RETURN:
		returnstmt();
		break;
	case T_DECLARE:
	case T_BEGIN:
		/* create a new level for block */
		createBlock();

		/* set up new stack frame */
		asmComment("setup a new frame for block. save regs, setup display.");
		asmWrite("\tmov\tbp,sp\n");
		asmWrite("\tpush\t@DISPLAY+%ld\n", (long)INTEGERPOINTERSIZE*level);
		asmWrite("\tpush\tsi\n");
		asmWrite("\tpush\tdi\n"),
		asmWrite("\tmov\tdi, offset @DISPLAY\n");
		asmWrite("\tmov\t[di]+%ld, bp\n", (long)INTEGERPOINTERSIZE*level);

		/* block non-terminal */
		block((char *)0);

		/* pop stack frame */
		asmComment("delete frame for block. pop regs, reset display.");
		asmWrite("\tadd\tsp, %d\n", blockNesting[level].size);
		asmWrite("\tpop\tdi\n");
		asmWrite("\tpop\tsi\n");
		asmWrite("\tpop\tbp\n");
		asmWrite("\tmov\tbx, offset @DISPLAY\n");
		asmWrite("\tmov\tds:[bx]+%ld, bp\n", (long)INTEGERPOINTERSIZE*level);

		/* dump block symbols */
		dumpBlock(level);

		/* delete block */
		destroyBlock();
		break;
	case T_CALL:
		proccall();
		break;
	case T_FOR:
		forstmt();
		break;
	default:
		/* empty production is allowed */
		break;
	}

	/* all done */
	RETURN;
}

/* assignment non-terminal */
void
assignment()
{
	SYMBOL sym;
	SEMANTICINFO varInfo, exprInfo;

	ENTRY("assignment");
	/* get token */
	getToken();

	/* check starter symbols */
	switch (tokenType)
	{
	case T_ID:
		/* variable item nonterminal */
		varInfo = variableitem();

		/* check type and kind */
		if (varInfo.type != S_INTEGER && varInfo.type != S_BOOLEAN)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: variable must be an integer or boolean.\n");
		}
		if (varInfo.kind != K_SIMPLE)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: variable must be simple kind.\n");
		}

		/* check if identifier is an array */
		sym = *symbolTable[varInfo.symbolTableIndex];
		if (sym.kind == K_ARRAY)
		{
			/* save array index */
			asmWrite("\tpush\tax\n");
		}

		/* check for assignment operator */
		getToken();
		if (tokenType != T_ASGN)
		{
			listPrint("ERROR: assignment -> variableitem <:=> expr.\n");
			listPrint("expecting asignment operator.\n");
			suicide();
		}
		acceptToken(T_ASGN);

		/* get expression */
		exprInfo = expr();

		/* check consistency between expr and variable */
		if (exprInfo.type != S_INTEGER && exprInfo.type != S_BOOLEAN)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: expr must be an integer or boolean.\n");
		}
		if (exprInfo.kind != K_SIMPLE)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: expr must be simple kind.\n");
		}
		if (exprInfo.type != varInfo.type)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: variable type != expr type.\n");
		}
		if (exprInfo.kind != varInfo.kind)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: variable kind != expr kind.\n");
		}

		/* check if expr is a variable */
		if (semanticErrors == 0 && exprInfo.isVariable == YES)
		{
			readVariable(exprInfo);
			exprInfo.isVariable = NO;
		}

		/* check if we have an array */
		if (sym.kind == K_ARRAY)
		{
			/* pop index and save expr value in stack for now */
			asmComment("save array index in ax and expr value in stack.");
			asmWrite("\tpop\tbx\n");
			asmWrite("\tpush\tax\n");
			asmWrite("\tmov\tax, bx\n");
		}

		/* store value in memory */
		writeVariable(varInfo);

		break;

	default:
		listPrint("ERROR: assignment -> <variableitem> := expr.\n");
		listPrint("expecting ident for variableitem.\n");
		suicide();
	}

	/* all done */
	RETURN;
}

/* variable item nonterminal */
SEMANTICINFO
variableitem()
{
	int missingId;
	SYMBOL varSym;
	SEMANTICINFO varInfo;
	SEMANTICINFO exprInfo;

	ENTRY("variableitem");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_ID:
		/* get info for identifier */
		if (findEntry(tokenString, &varSym, ALLBLOCKS, NOPARAMETERS) != OK)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: identifier (%s) not defined.\n", tokenString);
			errorPrint(__LINE__, "SEMANTIC ERROR: defaulting to an integer variable.\n");

			/* construct a dummy symbol table entry */
			strcpy(varSym.name, tokenString);
			varSym.offset = 0;
			varSym.argOffset = 0;
			varSym.level = -1;
			varSym.type = S_INTEGER;
			varSym.isReference = NO;
			varSym.isParameter = NO;
			varSym.dimension = 0;
			varSym.nextParameter = -1;

			/* identifier was not found */
			missingId = YES;
		}
		else           
		{
			/* identifier was found */
			missingId = NO;
		}

		/* copy data to semantic structure */
		SYMBOL2SEMANTIC(varSym, varInfo);

		/* accept identifier */
		acceptToken(T_ID);

		/* check if we have an array */
		getToken();
		if (tokenType != T_LB)
		{
			/* not an array, all done */
			if (missingId == YES)
			{
				varSym.kind = K_SIMPLE;
				(void)insertEntry(&varSym);
				SYMBOL2SEMANTIC(varSym, varInfo);
				errorPrint(__LINE__, 
					"WARNING: inserting dummy variable identifier (%s).\n", varSym.name);
			}

			/* we do have a variable */
			varInfo.isVariable = YES;
			VRETURN(varInfo);
		}

		/* accept token */
		acceptToken(T_LB);

		/* insert dummy entry, ignore any errors */
		if (missingId == YES)
		{
			varSym.kind = K_ARRAY;
			(void)insertEntry(&varSym);
			SYMBOL2SEMANTIC(varSym, varInfo);
			errorPrint(__LINE__, "WARNING: inserting dummy array identifier (%s).\n", varSym.name);
		}

		/* check identifier is for an array */
		if (varInfo.kind != K_ARRAY)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: variable (%s) is not an array.\n", varSym.name);
		}

		/* call expression nonterminal*/
		exprInfo = expr();

		/* get expression for index, check type and kind */
		if (exprInfo.type != S_INTEGER)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: expression is not an integer.\n");
		}
		if (exprInfo.kind != K_SIMPLE)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: expression is not simple.\n");
		}

		/* check if expr is a variable */
		if (semanticErrors == 0 && exprInfo.isVariable == YES)
		{
			readVariable(exprInfo);
			exprInfo.isVariable = NO;
		}

		/* get closing bracket */
		getToken();
		if (tokenType != T_RB)
		{
			listPrint("ERROR: variable -> ident | ident [ expr <]>.\n");
			listPrint("expecting right bracket.\n");
			suicide();
		}
		acceptToken(T_RB);

		/* we have a variable */
		varInfo.kind = K_SIMPLE;
		varInfo.isVariable = YES;
		break;

	default:
		/* bad syntax */
		listPrint("ERROR: variableitem -> <ident> | <ident> [ expr ].\n");
		listPrint("expecting ident.\n");
		suicide();
	}

	/* all done */
	VRETURN(varInfo);
}

/* loop statement nonterminal */
void
loopstmt()
{
	ENTRY("loopstmt");
	/* get token */
	getToken();

	/* check starte symbol */
	switch (tokenType)
	{
	case T_LOOP:
		/* accept loop */
		acceptToken(T_LOOP);

		/* entering a loop */
		loopCounter++;

		/* get statement list */
		getToken();
		stmtlist();

		/* end of loop */
		getToken();
		if (tokenType != T_END)
		{
			listPrint("ERROR: loopstmt -> loop stmtlist <end> loop.\n");
			listPrint("expecting end.\n");
			suicide();
		}
		acceptToken(T_END);
		getToken();
		if (tokenType != T_LOOP)
		{
			listPrint("ERROR: loopstmt -> loop stmtlist end <loop>.\n");
			listPrint("expecting loop.\n");
			suicide();
		}
		acceptToken(T_LOOP);

		/* exiting a loop */
		loopCounter--;
		break;

	default:
		/* bad syntax */
		listPrint("ERROR: loopstmt -> <loop> stmtlist end loop.\n");
		listPrint("expecting loop.\n");
		suicide();
	}

	/* all done */
	RETURN;
}

/* when clause nonterminal */
void
whenclause()
{
	SEMANTICINFO exprInfo;

	ENTRY("whenclause");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_WHEN:
		/* accept when */
		acceptToken(T_WHEN);

		/* we better be in a loop */
		if (loopCounter <= 0)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: when-clause outside of a loop construct.\n");
		}

		/* get when expression */
		exprInfo = expr();
		if (exprInfo.type != S_BOOLEAN)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: when-clause expr must be boolean.\n");
		}
		if (exprInfo.kind != K_SIMPLE)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: when-clause expr must be simple.\n");
		}

		/* check if expr is a variable */
		if (semanticErrors == 0 && exprInfo.isVariable == YES)
		{
			readVariable(exprInfo);
			exprInfo.isVariable = NO;
		}

		/* get "do" token */
		getToken();
		if (tokenType != T_DO)
		{
			listPrint("ERROR: when expr <do> stmtlist exit.\n");
			listPrint("ERROR: when expr <do> stmtlist cycle.\n");
			listPrint("expecting do.\n");
			suicide();
		}
		acceptToken(T_DO);

		/* get statement block */
		stmtlist();

		/* check loop termination */
		getToken();
		switch (tokenType)
		{
		case T_EXIT:
			acceptToken(T_EXIT);
			break;
		case T_CYCLE:
			acceptToken(T_CYCLE);
			break;
		default:
			listPrint("ERROR: when expr do stmtlist <exit>.\n");
			listPrint("ERROR: when expr do stmtlist <cycle>.\n");
			listPrint("expecting exit or cycle.\n");
			suicide();
		}
		break;

	default:
		/* bad syntax */
		listPrint("ERROR: <when> expr do stmtlist exit.\n");
		listPrint("ERROR: <when> expr do stmtlist cycle.\n");
		listPrint("expecting when.\n");
		suicide();
	}

	/* all done */
	RETURN;
}

/* input statement nonterminal */
void
inputstmt()
{
	SEMANTICINFO varInfo;

	ENTRY("inputstmt");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_INPUT:
		/* accept input */
		acceptToken(T_INPUT);

		/* get left parenthesis */
		getToken();
		if (tokenType != T_LP)
		{
			listPrint("ERROR: input <(> variableitem { , variableitem } ).\n");
			listPrint("expecting left parenthesis.\n");
			suicide();
		}
		acceptToken(T_LP);

		/* get variables for input */
		getToken();
		if (tokenType != T_ID)
		{
			listPrint("ERROR: input ( <variableitem> { , variableitem } ).\n");
			listPrint("expecting ident for variableitem.\n");
			suicide();
		}
		varInfo = variableitem();
		if (varInfo.type != S_INTEGER)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: variable (%s) not an integer.\n", 
				   symbolTable[varInfo.symbolTableIndex]->name);
		}
		if (varInfo.kind != K_SIMPLE)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: variable (%s) is not simple.\n", 
				   symbolTable[varInfo.symbolTableIndex]->name);
		}

		/* check for more variables */
		getToken();
		while (tokenType == T_COMMA)
		{
			/* accept comma and get next variable */
			acceptToken(T_COMMA);
			varInfo = variableitem();
			if (varInfo.type != S_INTEGER)
			{
				semanticErrors++;
				errorPrint(__LINE__, "SEMANTIC ERROR: variable (%s) not an integer.\n", 
					symbolTable[varInfo.symbolTableIndex]->name);
			}
			if (varInfo.kind != K_SIMPLE)
			{
				semanticErrors++;
				errorPrint(__LINE__, "SEMANTIC ERROR: variable (%s) is not simple.\n", 
					symbolTable[varInfo.symbolTableIndex]->name);
			}
			getToken();
		}

		/* check for right parenthesis */
		getToken();
		if (tokenType != T_RP)
	       {
			listPrint("ERROR: input ( variableitem { , variableitem } <)>.\n");
			listPrint("expecting right parenthesis.\n");
			suicide();
		}
		acceptToken(T_RP);
		break;

	default:
		/* bad syntax */
		listPrint("ERROR: <input> ( variableitem { , variableitem } ).\n");
		listPrint("expecting input.\n");
		suicide();
	}

	/* all done */
	RETURN;
}

/* output statement nonterminal */
void
outputstmt()
{
	SEMANTICINFO exprInfo;

	ENTRY("outputstmt");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_OUTPUT:
		/* accept output */
		acceptToken(T_OUTPUT);

		/* get left parenthesis */
		getToken();
		if (tokenType != T_LP)
		{
			listPrint("ERROR: outputstmt -> output <(> expr { , expr } ).\n");
			listPrint("expecting left parenthesis.\n");
			suicide();
		}
		acceptToken(T_LP);

		/* get expressions for output */
		exprInfo = expr();
		
		/* check type of expression */
		switch (exprInfo.type)
		{
		case S_INTEGER:
		case S_BOOLEAN:
			if (exprInfo.kind != K_SIMPLE)
			{
				semanticErrors++;
				errorPrint(__LINE__, 
					"SEMANTIC ERROR: expr must be simple for integers and booleans.\n");
			}

			/* check if expr is a variable */
			if (semanticErrors == 0 && exprInfo.isVariable == YES)
			{
				readVariable(exprInfo);
				exprInfo.isVariable = NO;
			}

			/* print integer or boolean */
			asmComment("write out boolean or integer.");
			if (exprInfo.type == S_INTEGER)
			{
				asmWrite("\tcall\t_writeint\n");
			}
			else
			{
				asmWrite("\tcall\t_writebool\n");
			}
			break;
		case S_STRING:
			/* print string  */
			asmComment("write out string.");
			asmWrite("\tcall\t_writestr\n");
			break;
		default:
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: expr must be integer, boolean, or string.\n");
			break;
		}

		/* get remaining expressions */
		getToken();
		while (tokenType == T_COMMA)
		{
			/* accept comma and get next expression */
			acceptToken(T_COMMA);
			exprInfo = expr();

			/* check type of expression */
			switch (exprInfo.type)
			{
			case S_INTEGER:
			case S_BOOLEAN:
				if (exprInfo.kind != K_SIMPLE)
				{
					semanticErrors++;
					errorPrint(__LINE__, 
						"SEMANTIC ERROR: expr must be simple for integers and booleans.\n");
				}
				if (semanticErrors == 0 && exprInfo.isVariable == YES)
				{
					readVariable(exprInfo);
					exprInfo.isVariable = NO;
				}

				/* print integer or boolean */
				asmComment("write out boolean or integer.");
				if (exprInfo.type == S_INTEGER)
				{
					asmWrite("\tcall\t_writeint\n");
				}
				else
				{
					asmWrite("\tcall\t_writebool\n");
				}
				break;
			case S_STRING:
				/* print string  */
				asmComment("write out string.");
				asmWrite("\tcall\t_writestr\n");
				break;
			default:
				semanticErrors++;
				errorPrint(__LINE__, "SEMANTIC ERROR: expr must be integer, boolean, or string.\n");
				break;
			}

			/* get next token */
			getToken();
		}

		/* write out a new line */
		asmWrite("\tcall\t_newline\n");

		/* get closing parenthesis */
		if (tokenType != T_RP)
		{
			listPrint("ERROR: outputstmt -> output ( expr { , expr } <)>.\n");
			listPrint("expecting right parenthesis.\n");
			suicide();
		}
		acceptToken(T_RP);
		break;

	default:
		/* bd syntax */
		listPrint("ERROR: outputstmt -> output ( expr { , expr } ).\n");
		listPrint("expecting output.\n");
		suicide();
	}

	/* all done */
	RETURN;
}

/* for statement nonterminal */
void
forstmt()
{
	SEMANTICINFO exprInfo, incrInfo;
	SYMBOL indexSymbol;

	ENTRY("forstmt");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_FOR:
		/* accept for */
		acceptToken(T_FOR);

		/* entering a loop */
		loopCounter++;

		/* start a new block for for-stmt index */
		createBlock();

		/* get identifier */
		getToken();
		if (tokenType != T_ID)
		{
			listPrint("ERROR: forstmt -> for <ident> := expr to expr incr do stmtlist end for.\n");
			listPrint("expecting identifier.\n");
			suicide();
		}
		strcpy(indexSymbol.name, tokenString);      
		indexSymbol.offset = AUTOOFFSET;
		indexSymbol.offset -= INTEGERSIZE;
		indexSymbol.argOffset = 0;
		indexSymbol.level = -1;
		indexSymbol.type = S_INTEGER;   
		indexSymbol.kind = K_SIMPLE;
		indexSymbol.isReference = NO;
		indexSymbol.isParameter = NO;
		indexSymbol.dimension = 0;
		indexSymbol.nextParameter = -1;
		acceptToken(T_ID);

		/* insert variable symbol table entry */
		switch (insertEntry(&indexSymbol, NOPARAMETERS))
		{          
		case OK:
			/* success */
			break;
		case DUPLICATE:
			/* duplicate entry in symbol table */
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: index variable name (%s) redefined.\n", indexSymbol.name);
			break;
		default:
			/* unknown error */
			errorPrint(__LINE__, "insertEntry failed.\n");
			suicide();
		}

		/* update block nesting */
		blockNesting[level].name[0] = (char)0;
		blockNesting[level].isFunction = NO;
		blockNesting[level].size = IABS(indexSymbol.offset);

		/* set up new stack frame */
		asmComment("set up frame for for-stmt.");
		asmWrite("\tmov\tbp,sp\n");
		asmWrite("\tpush\t@DISPLAY+%ld\n", (long)INTEGERPOINTERSIZE*level);
		asmWrite("\tpush\tsi\n");
		asmWrite("\tpush\tdi\n"),
		asmWrite("\tmov\tdi, offset @DISPLAY\n");
		asmWrite("\tmov\t[di]+%ld, bp\n", (long)INTEGERPOINTERSIZE*level);

		/* get assignment */
		getToken();
		if (tokenType != T_ASGN)
		{
			listPrint("ERROR: forstmt -> for ident <:=> expr to expr incr do stmtlist end for.\n");
			listPrint("expecting assignment.\n");
			suicide();
		}
		acceptToken(T_ASGN);

		/* get an expression */
		exprInfo = expr();

		/* check expression */
		if (exprInfo.type != S_INTEGER)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: expr must be integer.\n");
		}
		if (exprInfo.kind != K_SIMPLE)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: expr must be simple.\n");
		}

		/* check if expr is a variable */
		if (semanticErrors == 0 && exprInfo.isVariable == YES)
		{
			readVariable(exprInfo);
			exprInfo.isVariable = NO;
		}

		/* get "to" */
		getToken();
		if (tokenType != T_TO)
		{
			listPrint("ERROR: forstmt -> for ident := expr <to> expr incr do stmtlist end for.\n");
			listPrint("expecting to.\n");
			suicide();
		}
		acceptToken(T_TO);

		/* get an expression */
		exprInfo = expr();

		/* check expression */
		if (exprInfo.type != S_INTEGER)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: expr must be integer.\n");
		}
		if (exprInfo.kind != K_SIMPLE)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: expr must be simple.\n");
		}

		/* check if expr is a variable */
		if (semanticErrors == 0 && exprInfo.isVariable == YES)
		{
			readVariable(exprInfo);
			exprInfo.isVariable = NO;
		}

		/* get the increment */
		getToken();
		if (tokenType == T_BY)
		{
			/* get increment */
			incrInfo = incr();
		}

		/* get "do" */
		getToken();
		if (tokenType != T_DO)
		{
			listPrint("ERROR: forstmt -> for ident := expr to expr incr <do> stmtlist end for.\n");
			listPrint("expecting do.\n");
			suicide();
		}
		acceptToken(T_DO);

		/* get statement list */
		stmtlist();
		
		/* get "end for" */
		getToken();
		if (tokenType != T_END)
		{
			listPrint("ERROR: forstmt -> for ident := expr to expr incr do stmtlist <end> for.\n");
			listPrint("expecting end.\n");
			suicide();
		}
		acceptToken(T_END);
		getToken();
		if (tokenType != T_FOR)
		{
			listPrint("ERROR: forstmt -> for ident := expr to expr incr do stmtlist end <for>.\n");
			listPrint("expecting for.\n");
			suicide();
		}
		acceptToken(T_FOR);

		/* exiting a loop */
		loopCounter--;

		/* pop stack frame */
		asmComment("delete frame for for-stmt.");
		asmWrite("\tadd\tsp, %ld\n", (long)blockNesting[level].size);
		asmWrite("\tpop\tdi\n");
		asmWrite("\tpop\tsi\n");
		asmWrite("\tpop\tbp\n");
		asmWrite("\tmov\tbx, offset @DISPLAY\n");
		asmWrite("\tmov\tds:[bx]+%ld, bp\n", (long)INTEGERPOINTERSIZE*level);

		/* destroy block for for-stmt */
		dumpBlock(level);
		destroyBlock();
		break;

	default:
		/* bad syntax */
		listPrint("ERROR: forstmt -> <for>ident := expr to expr incr do stmtlist end for.\n");
		listPrint("expecting for.\n");
		suicide();
	}

	/* all done */
	RETURN;
}

/* increment nonterminal */
SEMANTICINFO
incr()
{
	TOKEN sign;
	SEMANTICINFO incrInfo;

	ENTRY("incr");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_BY:
		/* accept by */
		acceptToken(T_BY);

		/* check for a sign */
		getToken();
		if (tokenType == T_PLUS || tokenType == T_MINUS)
		{
			sign = optionalsign();
		}

		/* get unsigned constant */
		incrInfo = unsignedconst();

		/* check expression */
		if (incrInfo.type != S_INTEGER)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: incr expr must be integer.\n");
		}
		if (incrInfo.kind != K_SIMPLE)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: incr expr must be simple.\n");
		}

		/* apply sign */
		if (semanticErrors == 0 && sign == T_MINUS)
		{
			incrInfo.integerValue *= -1;
		}
		break;

	default:
		/* allows empty production */
		break;
	}

	/* all done */
	VRETURN(incrInfo);
}

/*if statement nonterminal */
void
ifstmt()
{
	SEMANTICINFO exprInfo;

	ENTRY("ifstmt");
	/* get token */
	getToken();

	/* check starter symbol */
	if (tokenType != T_IF)
	{
		listPrint("ERROR: ifstmt -> <if> expr then stmtlist elseifpart elsepart end if.\n");
		listPrint("expecting if.\n");
		suicide();
	}
	acceptToken(T_IF);

	/* get expression */
	exprInfo = expr();

	/* check expression */
	if (exprInfo.type != S_BOOLEAN)
	{
		semanticErrors++;
		errorPrint(__LINE__, "SEMANTIC ERROR: if-stmt expr must be boolean.\n");
	}
	if (exprInfo.kind != K_SIMPLE)
	{
		semanticErrors++;
		errorPrint(__LINE__, "SEMANTIC ERROR: if-stmt expr must be simple.\n");
	}

	/* check if expr is a variable */
	if (semanticErrors == 0 && exprInfo.isVariable == YES)
	{
		readVariable(exprInfo);
		exprInfo.isVariable = NO;
	}

	/* get then */
	getToken();
	if (tokenType != T_THEN)
	{
		listPrint("ERROR: ifstmt -> if expr <then> stmtlist elseifpart elsepart end if.\n");
		listPrint("expecting then.\n");
		suicide();
	}
	acceptToken(T_THEN);

	/* get statement list */
	stmtlist();

	/* get else if part */
	elseifpart();

	/* get else part */
	elsepart();

	/* get end if */
	getToken();
	if (tokenType != T_END)
	{
		listPrint("ERROR: ifstmt -> if expr then stmtlist elseifpart elsepart <end> if.\n");
		listPrint("expecting end.\n");
		suicide();
	}
	acceptToken(T_END);
	getToken();
	if (tokenType != T_IF)
	{
		listPrint("ERROR: fstmt -> if expr then stmtlist elseifpart elsepart end <if>.\n");
		listPrint("expecting if.\n");
		suicide();
	}
	acceptToken(T_IF);

	/* all done */
	RETURN;
}

/* else-if statement nonterminal */
void
elseifpart()
{
	SEMANTICINFO exprInfo;

	ENTRY("elseifpart");
	/* get token and remember null production is allowed */
	getToken();

	/* check starter symbol and call statement */
	while (tokenType == T_ELSEIF)
	{
		/* accept else if */
		acceptToken(T_ELSEIF);

		/* get expression */
		exprInfo = expr();

		/* check expression */
		if (exprInfo.type != S_BOOLEAN)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: elseif-stmt expr must be boolean.\n");
		}
		if (exprInfo.kind != K_SIMPLE)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: elseif-stmt expr must be simple.\n");
		}

		/* check if expr is a variable */
		if (semanticErrors == 0 && exprInfo.isVariable == YES)
		{
			readVariable(exprInfo);
			exprInfo.isVariable = NO;
		}

		/* get "then" part */
		getToken();
		if (tokenType != T_THEN)
		{
			listPrint("ERROR: elseifpart -> { elseif expr <then> stmtlist.\n");
			listPrint("expecting then.\n");
			suicide();
		}
		acceptToken(T_THEN);

		/* get statement list */
		stmtlist();

		/* get next token */
		getToken();
	}

	/* all done */
	RETURN;
}

/* else statement nonterminal */
void
elsepart()
{
	ENTRY("elsepart");
	/* get token */
	getToken();

	/* check starter symbol */
	if (tokenType != T_ELSE)
	{
		/* null production is allowed */
		RETURN;
	}

	/* accept else */
	acceptToken(T_ELSE);

	/* get statement list */
	stmtlist();

	/* all done */
	RETURN;
}

/* procedure call nonterminal */
void
proccall()
{
	SYMBOL idSym;

	ENTRY("proccall");
	/* get token */
	getToken();

	/* check starter symbol */
	if (tokenType != T_CALL)
	{
		listPrint("ERROR: proccall -> <call> ident ( arguments ).\n");
		listPrint("expecting call.\n");
		suicide();
	}
	acceptToken(T_CALL);

	/* get identifier */
	getToken();
	if (tokenType != T_ID)
	{
		listPrint("ERROR: proccall -> call <ident> ( arguments ).\n");
		listPrint("expecting identifier.\n");
		suicide();
	}

	/* identifier must be a procedure */
	if (findEntry(tokenString, &idSym, GLOBALBLOCK, NOPARAMETERS) != OK)
	{
		semanticErrors++;
		errorPrint(__LINE__, "SEMANTIC ERROR: identifier (%s) not defined.\n", tokenString);
		errorPrint(__LINE__, "SEMANTIC ERROR: defaulting to a procedure.\n");

		/* construct a dummy symbol table entry */
		strcpy(idSym.name, tokenString);
		idSym.offset = 0;
		idSym.argOffset = 0;
		idSym.level = -1;
		idSym.type = S_VOID;
		idSym.kind = K_PROCEDURE;
		idSym.isReference = NO;
		idSym.isParameter = NO;
		idSym.dimension = 0;
		idSym.nextParameter = -1;

		/* insert entry */
		(void)insertEntry(&idSym);
		errorPrint(__LINE__, "WARNING: inserting dummy procedure identifier (%s).\n", idSym.name);
	}

	/* accept procedure name */
	acceptToken(T_ID);

	/* get arguments */
	getToken();
	if (tokenType != T_LP)
	{
		listPrint("ERROR: proccall -> call ident <(> arguments ).\n");
		listPrint("expecting left parethesis.\n");
		suicide();
	}
	acceptToken(T_LP);
	arguments(&idSym);
	getToken();
	if (tokenType != T_RP)
	{
		listPrint("ERROR: proccall -> call ident ( arguments <)>.\n");
		listPrint("expecting right parethesis.\n");
		suicide();
	}
	acceptToken(T_RP);

	/* call procedure */
	asmComment("calling a procedure");
	asmWrite("\tcall\t_%s\n", idSym.name);

	/* all done */
	RETURN;
}

/* arguments nonterminal */
void
arguments(funcSym)
SYMBOL *funcSym;
{
	int arg, paramIndex;
	SEMANTICINFO exprInfo;
	SYMBOL sym;

	ENTRY("arguments");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_PLUS:
	case T_MINUS:
	case T_NUM:
	case T_ID:
	case T_LP:
	case T_NOT:
		/* get expression */
		arg = 1;
		exprInfo = expr();

		/* check parameters */
		paramIndex = funcSym->nextParameter;
		sym.nextParameter = -1;
		if (paramIndex < 0)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: function (%s) takes no parameters.\n", 
				   funcSym->name);
		}
		else
		{
			/* check type and kind */
			sym = *symbolTable[paramIndex];
			switch (sym.kind)
			{
			case K_ARRAY:
				/* arrays must be passed by reference */
				if (sym.isReference != YES)
				{
					listPrint("ERROR: arrays must be passed by reference to functions.\n");
					suicide();
				}
				if (exprInfo.kind != K_ARRAY)
				{
					semanticErrors++;
					errorPrint(__LINE__, 
						"SEMANTIC ERROR: arg %ld must be an array.\n", (long)arg);
				}
				if (exprInfo.isVariable != YES)
				{
					semanticErrors++;
					errorPrint(__LINE__, 
						"SEMANTIC ERROR: arg %ld must be a variable.\n", (long)arg);
				}

				/* generate code for accessing array */
				if (semanticErrors != 0)
				{
					/* skip generating any code */
					break;
				}
				if (sym.level == level)
				{
					/* symbol is in the present level */
					if (sym.offset < 0)
					{
						/* array in present frame */
						asmComment("pushing array in present frame.");
						asmWrite("\tmov\tax, bp\n");
						asmWrite("\tadd\tax, %ld\n", (long)sym.offset);
						asmWrite("\tpush\tax\n");
					}
					else
					{
						/* argument array */
						asmComment("pushing array in argument list.");
						asmWrite("\tmov\tax, [bp]+%ld\n", (long)sym.offset);
						asmWrite("\tpush\tax\n");
					}
				}
				else
				{
					/* symbol in another frame */
					if (sym.offset < 0)
					{
						/* array in another frame */
						asmComment("pushing array in another frame.");
						asmWrite("\tmov\tax, @DISPLAY+%ld\n", 
							(long)INTEGERPOINTERSIZE*sym.level);
						asmWrite("\tadd\tax, %ld\n", (long)sym.offset);
						asmWrite("\tpush\tax\n");
					}
					else
					{
						/* accessing an argument in another frame */
						asmComment("pushing array in argument list in another frame.");
						asmWrite("\tmov\tbx, @DISPLAY+%ld\n", 
							(long)INTEGERPOINTERSIZE*sym.level);
						asmWrite("\tmov\tax, ss:[bx]%ld\n", (long)sym.offset);
						asmWrite("\tpush\tax\n");
					}
				}
				break;

			case K_SIMPLE:
				/* simple expessions can be passed by value or reference */
				if (exprInfo.kind != K_SIMPLE)
				{
					semanticErrors++;
					errorPrint(__LINE__, 
						"SEMANTIC ERROR: arg %ld must be simple.\n", (long)arg);
				}

				/* check if passed by reference */
				if (sym.isReference == YES && exprInfo.isVariable != YES)
				{
					semanticErrors++;
					errorPrint(__LINE__, 
						"SEMANTIC ERROR: arg %ld must be a variable.\n", (long)arg);
				}

				/* check for errors */
				if (semanticErrors != 0)
				{
					/* skip code generation */
					break;
				}

				/* generate code for a variable */
				if (sym.isReference == NO)
				{
					/* pass by value */
					if (exprInfo.isVariable == YES)
					{
						readVariable(exprInfo);
						exprInfo.isVariable = NO;
					}
					asmComment("push variable.");
					asmWrite("\tpush\tax\n");
					break;
				}

				/* generate code for passing a variable by reference */
				if (sym.level == level)
				{
					/* symbol is in the present level */
					if (sym.offset < 0)
					{
						/* variable in present frame */
						asmComment("pushing variable pointer in present frame.");
						asmWrite("\tmov\tax, bp\n");
						asmWrite("\tadd\tax, %ld\n", (long)sym.offset);
						asmWrite("\tpush\tax\n");
					}
					else
					{
						/* argument variable */
						asmComment("pushing variable pointer in argument list.");
						asmWrite("\tmov\tax, [bp]+%ld\n", (long)sym.offset);
						asmWrite("\tpush\tax\n");
					}
				}
				else
				{
					/* symbol in another frame */
					if (sym.offset < 0)
					{
						/* variable in another frame */
						asmComment("pushing variable pointer in another frame.");
						asmWrite("\tmov\tax, @DISPLAY+%ld\n", 
							(long)INTEGERPOINTERSIZE*sym.level);
						asmWrite("\tadd\tax, %ld\n", (long)sym.offset);
						asmWrite("\tpush\tax\n");
					}
					else
					{
						/* accessing an argument in another frame */
						asmComment("pushing variable pointer in argument list in another frame.");
						asmWrite("\tmov\tbx, @DISPLAY+%ld\n", 
							(long)INTEGERPOINTERSIZE*sym.level);
						asmWrite("\tmov\tax, ss:[bx]%ld\n", (long)sym.offset);
						asmWrite("\tpush\tax\n");
					}
				}
				break;

			default:
				semanticErrors++;
				errorPrint(__LINE__, "SEMANTIC ERROR: arguments can only be arrays or simple.\n");
			}
		}

		/* get any more expressions */
		getToken();
		while (tokenType == T_COMMA)
		{
			/* increment number of arguments found */
			arg++;

			/* accept comma */
			acceptToken(T_COMMA);

			/* get next expression */
			exprInfo = expr();

			/* check parameters */
			paramIndex = sym.nextParameter;
			if (paramIndex < 0)
			{
				semanticErrors++;
				errorPrint(__LINE__, "SEMANTIC ERROR: argument %ld is unexpected for function %s.\n", 
					(long)arg, funcSym->name);
			}
			else
			{
				/* check type and kind */
				sym = *symbolTable[paramIndex];
				switch (sym.kind)
				{
				case K_ARRAY:
					/* arrays must be passed by reference */
					if (sym.isReference != YES)
					{
						semanticErrors++;
						errorPrint(__LINE__, 
							"SEMANTIC ERROR: arrays must be passed by reference to functions.\n");
					}
					if (exprInfo.kind != K_ARRAY)
					{
						semanticErrors++;
						errorPrint(__LINE__, 
							"SEMANTIC ERROR: arg %ld must be an array.\n", (long)arg);
					}
					if (exprInfo.isVariable != YES)
					{
						semanticErrors++;
						errorPrint(__LINE__, 
							"SEMANTIC ERROR: arg %ld must be a variable.\n", (long)arg);
					}

					/* generate code for accessing array */
					if (semanticErrors != 0)
					{
						/* skip generating any code */
						break;
					}
					if (sym.level == level)
					{
						/* symbol is in the present level */
						if (sym.offset < 0)
						{
							/* array in present frame */
							asmComment("pushing array in present frame.");
							asmWrite("\tmov\tax, bp\n");
							asmWrite("\tadd\tax, %ld\n", (long)sym.offset);
							asmWrite("\tpush\tax\n");
						}
						else
						{
							/* argument array */
							asmComment("pushing array in argument list.");
							asmWrite("\tmov\tax, [bp]+%ld\n", (long)sym.offset);
							asmWrite("\tpush\tax\n");
						}
					}
					else
					{
						/* symbol in another frame */
						if (sym.offset < 0)
						{
							/* array in another frame */
							asmComment("pushing array in another frame.");
							asmWrite("\tmov\tax, @DISPLAY+%ld\n", 
								(long)INTEGERPOINTERSIZE*sym.level);
							asmWrite("\tadd\tax, %ld\n", (long)sym.offset);
							asmWrite("\tpush\tax\n");
						}
						else
						{
							/* accessing an argument in another frame */
							asmComment("pushing array in argument list in another frame.");
							asmWrite("\tmov\tbx, @DISPLAY+%ld\n", 
								(long)INTEGERPOINTERSIZE*sym.level);
							asmWrite("\tmov\tax, ss:[bx]%ld\n", (long)sym.offset);
							asmWrite("\tpush\tax\n");
						}
					}
					break;

				case K_SIMPLE:
					/* simple expessions can be passed by value or reference */
					if (exprInfo.kind != K_SIMPLE)
					{
						semanticErrors++;
						errorPrint(__LINE__, 
							"SEMANTIC ERROR: arg %ld must be simple.\n", (long)arg);
					}

					/* check if passed by reference */
					if (sym.isReference == YES && exprInfo.isVariable != YES)
					{
						semanticErrors++;
						errorPrint(__LINE__, 
							"SEMANTIC ERROR: arg %ld must be a variable.\n", (long)arg);
					}

					/* check for errors */
					if (semanticErrors != 0)
					{
						/* skip code generation */
						break;
					}

					/* generate code for a variable */
					if (sym.isReference == NO)
					{
						/* pass by value */
						if (exprInfo.isVariable == YES)
						{
							readVariable(exprInfo);
							exprInfo.isVariable = NO;
						}
						asmComment("push variable.");
						asmWrite("\tpush\tax\n");
						break;
					}

					/* generate code for passing a variable by reference */
					if (sym.level == level)
					{
						/* symbol is in the present level */
						if (sym.offset < 0)
						{
							/* variable in present frame */
							asmComment("pushing variable pointer in present frame.");
							asmWrite("\tmov\tax, bp\n");
							asmWrite("\tadd\tax, %ld\n", (long)sym.offset);
							asmWrite("\tpush\tax\n");
						}
						else
						{
							/* argument variable */
							asmComment("pushing variable pointer in argument list.");
							asmWrite("\tmov\tax, [bp]+%ld\n", (long)sym.offset);
							asmWrite("\tpush\tax\n");
						}
					}
					else
					{
						/* symbol in another frame */
						if (sym.offset < 0)
						{
							/* variable in another frame */
							asmComment("pushing variable pointer in another frame.");
							asmWrite("\tmov\tax, @DISPLAY+%ld\n", 
								(long)INTEGERPOINTERSIZE*sym.level);
							asmWrite("\tadd\tax, %ld\n", (long)sym.offset);
							asmWrite("\tpush\tax\n");
						}
						else
						{
							/* accessing an argument in another frame */
							asmComment("pushing variable pointer in argument list in another frame.");
							asmWrite("\tmov\tbx, @DISPLAY+%ld\n", 
								(long)INTEGERPOINTERSIZE*sym.level);
							asmWrite("\tmov\tax, ss:[bx]%ld\n", (long)sym.offset);
							asmWrite("\tpush\tax\n");
						}
					}
					break;

				default:
					semanticErrors++;
					errorPrint(__LINE__, "SEMANTIC ERROR: arguments can only be arrays or simple.\n");
				}
			}

			/* next token */
			getToken();
		}

		/* check if any arguments are missing */
		if (sym.nextParameter != -1)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: missing argument(s) for function %s.\n", 
				funcSym->name);
		}
		break;

	default:
		/* empty production is allowed */
		break;
	}

	/* all done */
	RETURN;
}

/* return statement nonterminal */
void
returnstmt()
{
	int ilevel;
	SEMANTICINFO exprInfo;

	ENTRY("returnstmt");
	/* get token */
	getToken();

	/* check starter symbol */
	if (tokenType != T_RETURN)
	{
		listPrint("ERROR: returnstmt -> <return> | <return> expr.\n");
		listPrint("expecting return.\n");
		suicide();
	}
	acceptToken(T_RETURN);

	/* increment number of returns found */
	numberOfReturns++;

	/* check for an expression */
	getToken();
	switch (tokenType)
	{
	case T_PLUS:
	case T_MINUS:
	case T_NUM:
	case T_ID:
	case T_LP:
	case T_NOT:
	case T_TRUE:
	case T_FALSE:
		/* check kind of expected return */
		if (returnInfo.kind != K_FUNCTION)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: function return not expected.\n");
		}

		/* get expression */
		exprInfo = expr();

		/* check if expression matches function */
		if (exprInfo.kind != K_SIMPLE)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: function return expr must be simple.\n");
		}
		if (exprInfo.type != returnInfo.type)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: return expr type != function type.\n");
		}

		/* check if expr is a variable */
		if (semanticErrors == 0 && exprInfo.isVariable == YES)
		{
			readVariable(exprInfo);
			exprInfo.isVariable = NO;
		}
		break;

	default:
		/* empty production is allowed */
		if (returnInfo.kind != K_PROCEDURE)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: procedure return not expected.\n");
		}
		break;
	}

	/* pop any stack frames which are in blocks, not including functions */
	for (ilevel = level; blockNesting[ilevel].isFunction != YES ; ilevel--)
	{
		asmComment("return statement. pop all frames until a function is reached.");
		asmWrite("\tadd\tsp, %d\n", blockNesting[ilevel].size);
		asmWrite("\tpop\tdi\n");
		asmWrite("\tpop\tsi\n");
		asmWrite("\tpop\tbp\n");
		asmWrite("\tmov\tbx, offset @DISPLAY\n");
		asmWrite("\tmov\tds:[bx]+%ld, bp\n", (long)INTEGERPOINTERSIZE*ilevel);
	}

	/* jump to end of function */
	asmComment("jump to end of function label.");
	asmWrite("\tjmp\t@_%s\n", blockNesting[ilevel].name);

	/* all done */
	RETURN;
}

/* factor nonterminal */
SEMANTICINFO
factor()
{
	SEMANTICINFO info;

	ENTRY("factor");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_NUM:
	case T_STR:
	case T_TRUE:
	case T_FALSE:
		/* accept unsigned constant */
		info = unsignedconst();
		break;

	case T_ID:
		/* call factor identifier nonterminal */
		info = factorident();
		break;

	case T_LP:
		/* accept left parenthesis */
		acceptToken(T_LP);

		/* get expression */
		info = expr();

		/* get right parenthesis */
		getToken();
		if (tokenType != T_RP)
		{
			listPrint("ERROR: factor -> unsignedconst | <(> expr ) | not factor | factorident.\n");
			listPrint("expecting right parenthesis.\n");
			suicide();
		}
		acceptToken(T_RP);
		break;

	case T_NOT:
		/* accept not */
		acceptToken(T_NOT);

		/* get factor */
		info = factor();

		/* check type of factor */
		if (info.type != S_BOOLEAN)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: factor must be a boolean type.\n");
		}

		/* check if a variable */
		if (info.isVariable == YES)
		{
			readVariable(info);
			info.isVariable = NO;
		}

		/* negate the boolean factor */
		asmWrite("\tnot\tax\n");
		asmWrite("\tand\tax,1\n");
		break;

	default:
		listPrint("ERROR: factor -> <unsignedconst> | <(> expr ) | <not> factor | <factorident>.\n");
		listPrint("expecting number, ident, left parenthesis, or not.\n");
		suicide();
	}

	/* all done */
	VRETURN(info);
}

/* term nonterminal */
SEMANTICINFO
term()
{
	char label1[MAXIDENTSIZE], label2[MAXIDENTSIZE], label3[MAXIDENTSIZE];
	TOKEN operType, firstRelOp;
	SEMANTICINFO factorInfo1, factorInfo2, saveInfo;

	ENTRY("term");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_NUM:
	case T_ID:
	case T_LP:
	case T_NOT:
	case T_STR:
	case T_TRUE:
	case T_FALSE:
		/* get expression */
		saveInfo = factorInfo1 = factor();

		/* get any more factors */
		getToken();
		firstRelOp = T_NULL;
		while ((tokenType == T_MUL) || (tokenType == T_SLASH) ||
		       (tokenType == T_AND) || (tokenType == T_MOD))
		{
			/* acceptToken operation */
			operType = multop();

			/* check relop with type/kind of first term */
			switch (operType)
			{
			case T_MUL:
			case T_MOD:
			case T_SLASH:
				if (factorInfo1.type != S_INTEGER)
				{
					semanticErrors++;
					errorPrint(__LINE__, "SEMANTIC ERROR: mult/div/mod only apply to integer types.\n");
				}
				if (factorInfo1.kind != K_SIMPLE)
				{
					semanticErrors++;
					errorPrint(__LINE__, "SEMANTIC ERROR: mult/div/mod only apply to simple kinds.\n");
				}
				break;

			case T_AND:
				if (firstRelOp == T_NULL)
				{
					firstRelOp = operType;
					saveInfo.type = S_BOOLEAN;
				}
				if (factorInfo1.type != S_BOOLEAN)
				{
					semanticErrors++;
					errorPrint(__LINE__, "SEMANTIC ERROR: AND only applys to boolean type.\n");
				}                       
				if (factorInfo1.kind != K_SIMPLE)
				{
					semanticErrors++;
					errorPrint(__LINE__, "SEMANTIC ERROR: AND only applys to simple kind.\n");
				}
				break;
			}

			/* check if a variable */
			if (factorInfo1.isVariable == YES)
			{
				readVariable(factorInfo1);
				factorInfo1.isVariable = NO;
				saveInfo.isVariable = NO;
			}

			/* save intermediate results */
			asmComment("save intermediate factor result.");
			asmWrite("\tpush\tax\n");

			/* get next factor */
			factorInfo2 = factor();

			/* compare both factors */
			if (factorInfo1.type != factorInfo2.type)
			{
				semanticErrors++;
				errorPrint(__LINE__, "SEMANTIC ERROR: factor1 type != factor2 type.\n");
			}
			if (factorInfo1.kind != factorInfo2.kind)
			{
				semanticErrors++;
				errorPrint(__LINE__, "SEMANTIC ERROR: factor1 kind != factor2 kind.\n");
			}

			/* check if a variable */
			if (factorInfo2.isVariable == YES)
			{
				readVariable(factorInfo2);
				factorInfo2.isVariable = NO;
			}

			/* execute operation */
			asmComment("execute mult/div/mod/and operation.");
			switch (operType)
			{
			case T_MUL:
				asmWrite("\tpop\tbx\n");
				asmWrite("\timul\tbx\n");
				break;
			case T_SLASH:
				asmWrite("\tpop\tbx\n");
				asmWrite("\txchg\tbx, ax\n");
				asmWrite("\tcwd\n");
				asmWrite("\tidiv\tbx\n");
				break;
			case T_AND:
				getLabel(label1);
				getLabel(label2);
				getLabel(label3);
				asmWrite("\tpop\tbx\n");
				asmWrite("\tcmp\tax, 0\n");
				asmWrite("\tje\t%s\n", label1);
				asmWrite("\tcmp\tbx, 0\n");
				asmWrite("\tje\t%s\n", label1);
				asmWrite("\tjmp\t%s\n", label2);
				asmWrite("%s:\n", label1);
				asmWrite("\tmov\tax, 0\n");
				asmWrite("\tjmp\t%s\n", label3);
				asmWrite("%s:\n", label2);
				asmWrite("\tmov\tax, 1\n");
				asmWrite("%s:\n", label3);
				break;
			case T_MOD:
				asmWrite("\tpop\tbx\n");
				asmWrite("\txchg\tbx, ax\n");
				asmWrite("\tcwd\n");
				asmWrite("\tidiv\tbx\n");
				asmWrite("\tmov\tax, dx\n");
				break;
			default:
				listPrint("ERROR: simpleexpr -> optionalsign term <addop> term.\n");
				listPrint("expecting addop.\n");
				suicide();
			}

			/* copy data */
			factorInfo1 = factorInfo2;

			/* next token */
			getToken();
		}
		break;

	default:
		listPrint("ERROR: term -> <factor> { multop factor }.\n");
		listPrint("expecting factor.\n");
		suicide();
	}

	/* all don */
	VRETURN(saveInfo);
}

/* multiplier operation nonterminal */
TOKEN
multop()
{
	TOKEN multType;

	ENTRY("multop");
	/* get token */
	getToken();

	/* check starter symbol */
	multType = tokenType;
	switch (tokenType)
	{
	case T_MUL:
		acceptToken(T_MUL);
		break;
	case T_SLASH:
		acceptToken(T_SLASH);
		break;
	case T_AND:
		acceptToken(T_AND);
		break;
	case T_MOD:
		acceptToken(T_MOD);
		break;
	default:
		listPrint("ERROR: multop -> <*> | </> | <and> | <mod>.\n");
		listPrint("expecting multiplier.\n");
		suicide();
	}

	/* all done */
	VRETURN(multType);
}

/* simple expression nonterminal */
SEMANTICINFO
simpleexpr()
{
	char label1[MAXIDENTSIZE], label2[MAXIDENTSIZE], label3[MAXIDENTSIZE];
	TOKEN sign, relType, firstRelOp;
	SEMANTICINFO termInfo1, termInfo2, saveInfo;

	ENTRY("simpleexpr");
	/* get token */
	getToken();

	/* assume no sign */
	sign = T_NULL;

	/* check starter symbol */
	switch (tokenType)
	{
	case T_PLUS:
	case T_MINUS:
		/* handle optional sign and fall thru to next case */
		sign = optionalsign();

	case T_NUM:
	case T_ID:
	case T_LP:
	case T_NOT:
	case T_STR:
	case T_FALSE:
	case T_TRUE:
		/* get term */
		saveInfo = termInfo1 = term();

		/* check if term is consistent with sign */
		if (sign == T_PLUS || sign == T_MINUS)
		{
			if (termInfo1.type != S_INTEGER)
			{
				semanticErrors++;
				errorPrint(__LINE__, "SEMANTIC ERROR: signs only apply to integer types.\n");
			}
			if (termInfo1.kind != K_SIMPLE)
			{
				semanticErrors++;
				errorPrint(__LINE__, "SEMANTIC ERROR: signs only apply to simple kinds.\n");
			}
			if (termInfo1.isVariable == YES)
			{
				readVariable(termInfo1);
				termInfo1.isVariable = NO;
				saveInfo.isVariable = NO;
			}
			if (sign == T_MINUS)
			{
				asmWrite("\tneg\tax\n");
			}
		}

		/* get next token */
		getToken();

		/* get any additional terms */
		firstRelOp = T_NULL;
		while ((tokenType == T_PLUS) || (tokenType == T_MINUS) ||
		       (tokenType == T_OR))
		{
			/* check if a variable */
			if (termInfo1.isVariable == YES)
			{
				readVariable(termInfo1);
				termInfo1.isVariable = NO;
				saveInfo.isVariable = NO;
			}

			/* accept operation */
			relType = addop();

			/* check relop with type/kind of first term */
			switch (relType)
			{
			case T_PLUS:
			case T_MINUS:
				if (termInfo1.type != S_INTEGER)
				{
					semanticErrors++;
					errorPrint(__LINE__, "SEMANTIC ERROR: plus/minus only apply to integer types.\n");
				}
				if (termInfo1.kind != K_SIMPLE)
				{
					semanticErrors++;
					errorPrint(__LINE__, "SEMANTIC ERROR: plus/minus only apply to simple kinds.\n");
				}
				break;

			case T_OR:
				if (firstRelOp == T_NULL)
				{
					firstRelOp = relType;
					saveInfo.type = S_BOOLEAN;
				}
				if (termInfo1.type != S_BOOLEAN)
				{
					semanticErrors++;
					errorPrint(__LINE__, "SEMANTIC ERROR: OR only applys to boolean type.\n");
				}                       
				if (termInfo1.kind != K_SIMPLE)
				{
					semanticErrors++;
					errorPrint(__LINE__, "SEMANTIC ERROR: OR only applys to simple kind.\n");
				}
				break;
			}

			/* save intermediate results */
			asmComment("save intermediate term result.");
			asmWrite("\tpush\tax\n");

			/* get next term */
			termInfo2 = term();

			/* compare both terms */
			if (termInfo1.type != termInfo2.type)
			{
				semanticErrors++;
				errorPrint(__LINE__, "SEMANTIC ERROR: term1 type != term2 type.\n");
			}
			if (termInfo1.kind != termInfo2.kind)
			{
				semanticErrors++;
				errorPrint(__LINE__, "SEMANTIC ERROR: term1 kind != term2 kind.\n");
			}

			/* check if a variable */
			if (termInfo2.isVariable == YES)
			{
				readVariable(termInfo2);
				termInfo2.isVariable = NO;
			}

			/* execute operation */
			asmComment("execute plus/sub/or operation.");
			asmWrite("\tpop\tbx\n");
			switch (relType)
			{
			case T_PLUS:
				asmWrite("\tadd\tax, bx\n");
				break;
			case T_MINUS:
				asmWrite("\tsub\tbx, ax\n");
				asmWrite("\tmov\tax, bx\n");
				break;
			case T_OR:
				getLabel(label1);
				getLabel(label2);
				getLabel(label3);
				asmWrite("\tcmp\tax, 1\n");
				asmWrite("\tje\t%s\n", label1);
				asmWrite("\tcmp\tbx, 1\n");
				asmWrite("\tje\t%s\n", label1);
				asmWrite("\tjmp\t%s\n", label2);
				asmWrite("%s:\n", label1);
				asmWrite("\tmov\tax, 1\n");
				asmWrite("\tjmp\t%s\n", label3);
				asmWrite("%s:\n", label2);
				asmWrite("\tmov\tax, 0\n");
				asmWrite("%s:\n", label3);
				break;
			default:
				listPrint("ERROR: simpleexpr -> optionalsign term <addop> term.\n");
				listPrint("expecting addop.\n");
				suicide();
			}

			/* copy info */
			termInfo1 = termInfo2;

			/* get next token */
			getToken();
		}
		break;

	default:
		listPrint("ERROR: simpleexpr -> <optionalsign> <term> { addop term }.\n");
		listPrint("expecting optional sign or term.\n");
		suicide();
	}

	/* all done */
	VRETURN(saveInfo);
}

/* addition operation nonterminal */
TOKEN
addop()
{
	TOKEN addType;

	ENTRY("addop");
	/* get token */
	getToken();

	/* check starter symbol */
	addType = tokenType;
	switch (tokenType)
	{
	case T_PLUS:
		acceptToken(T_PLUS);
		break;
	case T_MINUS:
		acceptToken(T_MINUS);
		break;
	case T_OR:
		acceptToken(T_OR);
		break;
	default:
		listPrint("ERROR: addop -> <+> | <-> | <or>.\n");
		listPrint("expecting addition operator.\n");
		suicide();
	}

	/* all done */
	VRETURN(addType);
}

/* option sign nonterminal */
TOKEN
optionalsign()
{
	TOKEN signType;

	ENTRY("optionalsign");
	/* get token */
	getToken();

	/* check starter symbol */
	signType = tokenType;
	switch (tokenType)
	{
	case T_PLUS:
		acceptToken(T_PLUS);
		break;
	case T_MINUS:
		acceptToken(T_MINUS);
		break;
	default:
		/* empty production is allowed */
		signType = T_NULL;
		break;
	}

	/* alldone */
	VRETURN(signType);
}

/* factor identifier nonterminal */
SEMANTICINFO
factorident()
{
	int missingId, size;
	SYMBOL idSym;
	SEMANTICINFO idInfo;
	SEMANTICINFO exprInfo;

	ENTRY("factorident");
	/* get token */
	getToken();

	/* check starter symbol */
	if (tokenType != T_ID)
	{
		listPrint("ERROR: factorident -> <ident> | <ident> [ expr ] | <ident> ( arguments ).\n");
		listPrint("expecting ident.\n");
		suicide();
	}

	/* get info for identifier */
	if (findEntry(tokenString, &idSym, ALLBLOCKS, NOPARAMETERS) != OK)
	{
		semanticErrors++;
		errorPrint(__LINE__, "SEMANTIC ERROR: identifier (%s) not defined.\n", tokenString);
		errorPrint(__LINE__, "SEMANTIC ERROR: defaulting to an integer variable.\n");

		/* construct a dummy symbol table entry */
		strcpy(idSym.name, tokenString);
		idSym.offset = 0;
		idSym.argOffset = 0;
		idSym.level = -1;
		idSym.type = S_INTEGER;
		idSym.isReference = NO;
		idSym.isParameter = NO;
		idSym.dimension = 0;
		idSym.nextParameter = -1;

		/* identifier was not found */
		missingId = YES;
	}
	else
	{
		/* identifier was found */
		missingId = NO;
	}

	/* copy data to semantic structure */
	SYMBOL2SEMANTIC(idSym, idInfo);

	/* accept identifier name */
	acceptToken(T_ID);

	/* get next token */
	getToken();

	/* check if we have an array or a function call */
	switch (tokenType)
	{
	case T_LB:
		/* we have an array */
		acceptToken(T_LB);

		/* insert dummy entry, ignore any errors */
		if (missingId == YES)
		{
			idSym.kind = K_ARRAY;
			(void)insertEntry(&idSym);
			SYMBOL2SEMANTIC(idSym, idInfo);
			errorPrint(__LINE__, "WARNING: inserting dummy array identifier (%s).\n", idSym.name);
		}

		/* check identifier is for an array */
		if (idInfo.kind != K_ARRAY)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: identifier (%s) not an array.\n", idSym.name);
		}

		/* get expression for index, check type and kind */
		exprInfo = expr();
		if (exprInfo.type != S_INTEGER)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: expression is not an integer.\n");
		}
		if (exprInfo.kind != K_SIMPLE)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: expression is not simple.\n");
		}

		/* get right bracket for array */
		getToken();
		if (tokenType != T_RB)
		{
			listPrint("ERROR: factorident -> ident | ident [ expr <]> | ident ( arguments ).\n");
			listPrint("expecting right bracket.\n");
			suicide();
		}
		acceptToken(T_RB);

		/* we have a variable */
		idInfo.kind = K_SIMPLE;
		idInfo.isVariable = YES;
		break;

	case T_LP:
		/* we have a function call */
		acceptToken(T_LP);

		/* insert dummy entry, ignore any errors */
		if (missingId == YES)
		{
			idSym.kind = K_FUNCTION;
			(void)insertEntry(&idSym);
			SYMBOL2SEMANTIC(idSym, idInfo);
			errorPrint(__LINE__, "WARNING: inserting dummy function identifier (%s).\n", idSym.name);
		}

		/* check identifier is for a function */
		if (idInfo.kind != K_FUNCTION)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: identifier (%s) not a function.\n", idSym.name);
		}

		/* get function arguments */
		arguments(&idSym);

		/* get closing parenthesis for function */
		getToken();
		if (tokenType != T_RP)
		{
			listPrint("ERROR: factorident -> ident | ident [ expr ] | ident ( arguments <)>.\n");
			listPrint("expecting right parenthesis.\n");
			suicide();
		}
		acceptToken(T_RP);

		/* call function */
		asmComment("calling a function.");
		asmWrite("\tcall\t_%s\n", idSym.name);

		/* we do not have a variable */
		idInfo.kind = K_SIMPLE;
		idInfo.isVariable = NO;
		break;

	default:
		/* just an identifier */
		if (missingId == YES)
		{
			idSym.kind = K_SIMPLE;
			(void)insertEntry(&idSym);
			SYMBOL2SEMANTIC(idSym, idInfo);
			errorPrint(__LINE__, "WARNING: inserting dummy variable identifier (%s).\n", idSym.name);
		}

		/* we do have a variable */
		idInfo.isVariable = YES;
		break;
	}

	/* all done */
	VRETURN(idInfo);
}

/* unsigned constants nonterminal */
SEMANTICINFO
unsignedconst()
{
	char label[MAXIDENTSIZE+1];
	SEMANTICINFO info;

	ENTRY("unsignedconst");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_NUM:
		/* semantic info for a number */
		info.type = S_INTEGER;
		info.kind = K_SIMPLE;
		info.isVariable = NO;
		info.integerValue = tokenValue;
		info.booleanValue = NO;
		info.stringValue[0] = 0;
		info.symbolTableIndex = -1;
		acceptToken(T_NUM);

		/* store number in AX */
		asmComment("store constant in ax.");
		asmWrite("\tmov\tax, %ld\n", (long)info.integerValue);
		break;

	case T_STR:
		/* semantic info for a string */
		info.type = S_STRING;
		info.kind = K_SIMPLE;
		info.isVariable = NO;
		info.integerValue = 0;
		info.booleanValue = NO;
		strcpy(info.stringValue, tokenString);
		info.symbolTableIndex = -1;
		acceptToken(T_STR);

		/* store string in link list */
		storeString(info.stringValue, label);

		/* store string offset and length */
		asmComment("store string offset and length in dx and cx.");
		asmWrite("\tmov\tdx, offset %s\n", label);
		asmWrite("\tmov\tcx, %sLEN\n", label);
		break;

	case T_TRUE:
		/* semantic info for boolean */
		info.type = S_BOOLEAN;
		info.kind = K_SIMPLE;
		info.isVariable = NO;
		info.integerValue = 0;
		info.booleanValue = YES;
		info.stringValue[0] = 0;
		info.symbolTableIndex = -1;
		acceptToken(T_TRUE);

		/* store number in AX */
		asmComment("store TRUE in ax.");
		asmWrite("\tmov\tax, 1\n");
		break;

	case T_FALSE:
		/* semantic info for boolean */
		info.type = S_BOOLEAN;
		info.kind = K_SIMPLE;
		info.isVariable = NO;
		info.integerValue = 0;
		info.booleanValue = NO;
		info.stringValue[0] = 0;
		info.symbolTableIndex = -1;
		acceptToken(T_FALSE);

		/* store number in AX */
		asmComment("store FALSE in ax.");
		asmWrite("\tmov\tax, 0\n");
		break;

	default:
		listPrint("ERROR: unsignedconst -> <unsignednum> | <characterstring> | <true> | <false>.\n");
		listPrint("expecting unsigned constant.\n");
		suicide();
	}

	/* all done */
	VRETURN(info);
}

/* relation operators nonterminal */
TOKEN
relops()
{
	TOKEN relType;

	ENTRY("relops");
	/* get token */
	getToken();

	/* check starter symbol */
	relType = tokenType;
	switch (tokenType)
	{
	case T_LT:
		acceptToken(T_LT);
		break;
	case T_LE:
		acceptToken(T_LE);
		break;
	case T_NE:
		acceptToken(T_NE);
		break;
	case T_GT:
		acceptToken(T_GT);
		break;
	case T_GE:
		acceptToken(T_GE);
		break;
	case T_EQ:
		acceptToken(T_EQ);
		break;
	default:
		listPrint("ERROR: relop -> < | <= | <> | > | >= | =.\n");
		listPrint("expecting relational operator.\n");
		suicide();
	}

	/* all done */
	VRETURN(relType);
}

/* expression nonterminal */
SEMANTICINFO
expr()
{
	SEMANTICINFO saveInfo, exprInfo1, exprInfo2;
	TOKEN relType;
	char label1[MAXIDENTSIZE], label2[MAXIDENTSIZE], label3[MAXIDENTSIZE];

	ENTRY("expr");
	/* get token */
	getToken();

	/* check starter symbol */
	switch (tokenType)
	{
	case T_PLUS:
	case T_MINUS:
	case T_NUM:
	case T_STR:
	case T_TRUE:
	case T_FALSE:
	case T_ID:
	case T_LP:
	case T_NOT:
		/* get smple expression */
		saveInfo = exprInfo1 = simpleexpr();

		/* get next token, check for relation operation */
		getToken();
		switch (tokenType)
		{
		case T_LT:
		case T_LE:
		case T_NE:
		case T_GT:
		case T_GE:
		case T_EQ:
			/* handle relation operation */
			relType = relops();
			saveInfo.type = S_BOOLEAN;
			break;
		default:
			/* empty is allowed */
			VRETURN(exprInfo1);
		}

		/* check if a variable */
		if (exprInfo1.isVariable == YES)
		{
			readVariable(exprInfo1);
			exprInfo1.isVariable = NO;
		}

		/* save present value in stack for later use */
		asmComment("save simplexpr partial result in stack.");
		asmWrite("\tpush\tax\n");

		/* get simple expression */
		exprInfo2 = simpleexpr();

		/* check that expressions are simple */
		if (exprInfo1.kind != K_SIMPLE || exprInfo2.kind != K_SIMPLE)
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: expression(s) is(are) not simple.\n");
		}

		/*
		 * check consistency between expr1, relop, and expr2
		 *
		 * expr relop expr allowed?
		 * ------------------------
		 * int    <   int  yes
		 * int    <=  int  yes
		 * int    <>  int  yes
		 * int    >   int  yes
		 * int    >=  int  yes
		 * int    =   int  yes
		 * int   any  bool no
		 * bool   <   bool no
		 * bool   <=  bool no
		 * bool   <>  bool yes
		 * bool   >   bool no
		 * bool   >=  bool no
		 * bool   =   bool yes
		 * bool  any  int  no
		 */

		if (exprInfo1.type == S_INTEGER)
		{
			if (exprInfo2.type != S_INTEGER)
			{
				semanticErrors++;
				errorPrint(__LINE__, "SEMANTIC ERROR: expr1 = integer, but expr2 != integer.\n");
			}
		}
		else if (exprInfo1.type == S_BOOLEAN)
		{
			if (exprInfo2.type != S_BOOLEAN)
			{
				semanticErrors++;
				errorPrint(__LINE__, "SEMANTIC ERROR: expr1 = boolean, but expr2 != boolean\n");
			}
			else if (relType != T_NE && relType != T_EQ)
			{
				semanticErrors++;
				errorPrint(__LINE__, "SEMANTIC ERROR: only <> and = allowed for boolean exprs.\n");
			}
		}
		else
		{
			semanticErrors++;
			errorPrint(__LINE__, "SEMANTIC ERROR: only integers and booleans allowed with relops.\n");
		}

		/* check if a variable */
		if (exprInfo2.isVariable == YES)
		{
			readVariable(exprInfo2);
			exprInfo2.isVariable = NO;
		}

		/* get labels for comparisons */
		getLabel(label1);
		getLabel(label2);
		getLabel(label3);

		/* do comparison */
		asmComment("compare contents of bx to ax.");
		asmWrite("\tpop\tbx\n");
		asmWrite("\tcmp\tbx, ax\n");
		switch (relType)
		{
		case T_LT:
			asmWrite("\tjl\t%s\n", label1);
			break;
		case T_LE:
			asmWrite("\tjle\t%s\n", label1);
			break;
		case T_NE:
			asmWrite("\tjne\t%s\n", label1);
			break;
		case T_GT:
			asmWrite("\tjg\t%s\n", label1);
			break;
		case T_GE:
			asmWrite("\tjge\t%s\n", label1);
			break;
		case T_EQ:
			asmWrite("\tje\t%s\n", label1);
			break;
		default:
			listPrint("ERROR: expr -> simpleexpr <relop> simpleexpr.\n");
			listPrint("expecting relop.\n");
			suicide();
		}
		asmWrite("\tjmp\t%s\n", label2);
		asmWrite("%s:\n", label1);
		asmComment("comparison is true.");
		asmWrite("\tmov\tax, 1\n");
		asmWrite("\tjmp\t%s\n", label3);
		asmWrite("%s:\n", label2);
		asmComment("comparison is false.");
		asmWrite("\tmov\tax, 0\n");
		asmWrite("%s:\n", label3);

		/* not a variable */
		saveInfo.isVariable = NO;
		break;

	default:
		listPrint("ERROR: expr -> <simpleexpr> relop simpleexpr | <simpleexpr>.\n");
		listPrint("expecting simpleexpr.\n");
		suicide();
	}

	/* all done */
	VRETURN(saveInfo);
}

/* read a variable and store in ax register */
void
readVariable(idInfo)
SEMANTICINFO idInfo;
{
	int size;
	SYMBOL idSym;

	ENTRY("readVariable");
	/* get info for identifier */
	idSym = *symbolTable[idInfo.symbolTableIndex];

	/* check if we have an array or a variable */
	if (idInfo.kind == K_ARRAY)
	{
		/* generate code for accessing array */
		if (semanticErrors == 0)
		{
			/* generate code to access an array */
			if (idSym.level == level)
			{
				/* symbol is in the present level */
				if (idSym.offset < 0)
				{
					/* array in present frame */
					asmComment("accessing array in present frame.");
					size = (idSym.type == S_INTEGER) ? INTEGERSIZE : BOOLEANSIZE;
					asmWrite("\tmov\tdi, %ld\n", (long)size);
					asmWrite("\timul\tdi\n");
					asmWrite("\tmov\tdi, ax\n");
					asmWrite("\tmov\tax, ss:[bp][di]%ld\n", (long)idSym.offset);
				}
				else
				{
					/* argument array */
					asmComment("accessing array in argument list.");
					size = (idSym.type == S_INTEGER) ? INTEGERSIZE : BOOLEANSIZE;
					asmWrite("\tmov\tdi, %ld\n", (long)size);
					asmWrite("\timul\tdi\n");
					asmWrite("\tmov\tdi, ax\n");
					asmWrite("\tmov\tbx, [bp]+%ld\n", (long)idSym.offset);
					asmWrite("\tmov\tax, ss:[bx][di]\n");
				}
			}
			else
			{
				/* symbol in another frame */
				if (idSym.offset < 0)
				{
					/* array in another frame */
					asmComment("accessing array in another frame.");
					size = (idSym.type == S_INTEGER) ? INTEGERSIZE : BOOLEANSIZE;
					asmWrite("\tmov\tdi, %ld\n", (long)size);
					asmWrite("\timul\tdi\n");
					asmWrite("\tmov\tdi, ax\n");
					asmWrite("\tmov\tbx, @DISPLAY+%ld\n", (long)INTEGERPOINTERSIZE*idSym.level);
					asmWrite("\tmov\tax, ss:[bx][di]%ld\n", (long)idSym.offset);
				}
				else
				{
					/* accessing an argument in another frame */
					asmComment("accessing array in argument list in another frame.");
					size = (idSym.type == S_INTEGER) ? INTEGERSIZE : BOOLEANSIZE;
					asmWrite("\tmov\tdi, %ld\n", (long)size);
					asmWrite("\timul\tdi\n");
					asmWrite("\tmov\tdi, ax\n");
					asmWrite("\tmov\tbx, @DISPLAY+%ld\n", (long)INTEGERPOINTERSIZE*idSym.level);
					asmWrite("\tmov\tax, ss:[bx]%ld\n", (long)idSym.offset);
					asmWrite("\tmov\tbx, ax\n");
					asmWrite("\tmov\tax, ss:[bx][di]\n");
				}
			}
		}
	}
	else
	{
		/* just an identifier */
		if (semanticErrors == 0)
		{
			/* generate code to access a variable */
			if (idSym.level == level)
			{
				/* symbol is in the present level */
				if (idSym.offset < 0)
				{
					/* variable in present frame */
					asmComment("accessing variable in present frame.");
					asmWrite("\tmov\tax, ss:[bp]%ld\n", (long)idSym.offset);
				}
				else if (idSym.isReference == NO)
				{
					/* argument is not a pointer */
					asmComment("accessing variable in argument list.");
					asmWrite("\tmov\tax, ss:[bp]+%ld\n", (long)idSym.offset);
				}
				else
				{
					/* argument is a pointer */
					asmComment("accessing pointer variable in argument list.");
					asmWrite("\tmov\tbx, ss:[bp]+%ld\n", (long)idSym.offset);
					asmWrite("\tmov\tax, ss:[bx]\n");
				}
			}
			else
			{
				/* symbol in another frame */
				if (idSym.offset < 0)
				{
					/* variable in another frame */
					asmComment("accessing variable in another frame.");
					asmWrite("\tmov\tbx, @DISPLAY+%ld\n", (long)INTEGERPOINTERSIZE*idSym.level);
					asmWrite("\tmov\tax, ss:[bx]%ld\n", (long)idSym.offset);
				}
				else if (idSym.isReference == NO)
				{
					/* argument in not a pointer */
					asmComment("accessing variable in argument list in another frame.");
					asmWrite("\tmov\tbx, @DISPLAY+%ld\n", (long)INTEGERPOINTERSIZE*idSym.level);
					asmWrite("\tmov\tax, ss:[bx]+%ld\n", (long)idSym.offset);
				}
				else
				{
					/* accessing an argument in another frame */
					asmComment("accessing pointer variable in argument list in another frame.");
					asmWrite("\tmov\tbx, @DISPLAY+%ld\n", (long)INTEGERPOINTERSIZE*idSym.level);
					asmWrite("\tmov\tax, ss:[bx]+%ld\n", (long)idSym.offset);
					asmWrite("\tmov\tbx, ax\n");
					asmWrite("\tmov\tax, ss:[bx]\n");
				}
			}
		}
	}

	/* all done */
	RETURN;
}

/* write value to a variable */
void
writeVariable(idInfo)
SEMANTICINFO idInfo;
{
	int size;
	SYMBOL idSym;

	ENTRY("writeVariable");
	/* get info for identifier */
	idSym = *symbolTable[idInfo.symbolTableIndex];

	/* check if we have an array or a variable */
	if (idSym.kind == K_ARRAY)
	{
		/* generate code for accessing array */
		if (semanticErrors == 0)
		{
			/* generate code to access an array */
			if (idSym.level == level)
			{
				/* symbol is in the present level */
				if (idSym.offset < 0)
				{
					/* array in present frame */
					asmComment("setting value in array in present frame.");
					size = (idSym.type == S_INTEGER) ? INTEGERSIZE : BOOLEANSIZE;
					asmWrite("\tmov\tdi, %ld\n", (long)size);
					asmWrite("\timul\tdi\n");
					asmWrite("\tmov\tdi, ax\n");
					asmWrite("\tpop\tax\n");
					asmWrite("\tmov\tss:[bp][di]%ld, ax\n", (long)idSym.offset);
				}
				else
				{
					/* argument array */
					asmComment("setting value in array in argument list.");
					size = (idSym.type == S_INTEGER) ? INTEGERSIZE : BOOLEANSIZE;
					asmWrite("\tmov\tdi, %ld\n", (long)size);
					asmWrite("\timul\tdi\n");
					asmWrite("\tmov\tdi, ax\n");
					asmWrite("\tmov\tbx, [bp]+%ld\n", (long)idSym.offset);
					asmWrite("\tpop\tax\n");
					asmWrite("\tmov\tss:[bx][di], ax\n");
				}
			}
			else
			{
				/* symbol in another frame */
				if (idSym.offset < 0)
				{
					/* array in another frame */
					asmComment("setting value in array in another frame.");
					size = (idSym.type == S_INTEGER) ? INTEGERSIZE : BOOLEANSIZE;
					asmWrite("\tmov\tdi, %ld\n", (long)size);
					asmWrite("\timul\tdi\n");
					asmWrite("\tmov\tdi, ax\n");
					asmWrite("\tmov\tbx, @DISPLAY+%ld\n", (long)INTEGERPOINTERSIZE*idSym.level);
					asmWrite("\tpop\tax\n");
					asmWrite("\tmov\tss:[bx][di]%ld, ax\n", (long)idSym.offset);
				}
				else
				{
					/* accessing an argument in another frame */
					asmComment("settting value in array in argument list in another frame.");
					size = (idSym.type == S_INTEGER) ? INTEGERSIZE : BOOLEANSIZE;
					asmWrite("\tmov\tdi, %ld\n", (long)size);
					asmWrite("\timul\tdi\n");
					asmWrite("\tmov\tdi, ax\n");
					asmWrite("\tmov\tbx, @DISPLAY+%ld\n", (long)INTEGERPOINTERSIZE*idSym.level);
					asmWrite("\tmov\tax, ss:[bx]%ld\n", (long)idSym.offset);
					asmWrite("\tmov\tbx, ax\n");
					asmWrite("\tpop\tax\n");
					asmWrite("\tmov\tss:[bx][di], ax\n");
				}
			}
		}
	}
	else
	{
		/* just an identifier */
		if (semanticErrors == 0)
		{
			/* generate code to access a variable */
			if (idSym.level == level)
			{
				/* symbol is in the present level */
				if (idSym.offset < 0)
				{
					/* variable in present frame */
					asmComment("accessing variable in present frame.");
					asmWrite("\tmov\tss:[bp]%ld, ax\n", (long)idSym.offset);
				}
				else if (idSym.isReference == NO)
				{
					/* argument is not a pointer */
					asmComment("accessing variable in argument list.");
					asmWrite("\tmov\tss:[bp]+%ld, ax\n", (long)idSym.offset);
				}
				else
				{
					/* argument is a pointer */
					asmComment("accessing pointer variable in argument list.");
					asmWrite("\tmov\tbx, ss:[bp]+%ld\n", (long)idSym.offset);
					asmWrite("\tmov\tss:[bx], ax\n");
				}
			}
			else
			{
				/* symbol in another frame */
				if (idSym.offset < 0)
				{
					/* variable in another frame */
					asmComment("accessing variable in another frame.");
					asmWrite("\tmov\tbx, @DISPLAY+%ld\n", (long)INTEGERPOINTERSIZE*idSym.level);
					asmWrite("\tmov\tss:[bx]%ld, ax\n", (long)idSym.offset);
				}
				else if (idSym.isReference == NO)
				{
					/* argument in not a pointer */
					asmComment("accessing variable in argument list in another frame.");
					asmWrite("\tmov\tbx, @DISPLAY+%ld\n", (long)INTEGERPOINTERSIZE*idSym.level);
					asmWrite("\tmov\tss:[bx]+%ld, ax\n", (long)idSym.offset);
				}
				else
				{
					/* accessing an argument in another frame */
					asmComment("accessing pointer variable in argument list in another frame.");
					asmWrite("\tpush\tax\n");
					asmWrite("\tmov\tbx, @DISPLAY+%ld\n", (long)INTEGERPOINTERSIZE*idSym.level);
					asmWrite("\tmov\tax, ss:[bx]+%ld\n", (long)idSym.offset);
					asmWrite("\tmov\tbx, ax\n");
					asmWrite("\tpop\tax\n");
					asmWrite("\tmov\tss:[bx], ax\n");
				}
			}
		}
	}

	/* all done */
	RETURN;
}
