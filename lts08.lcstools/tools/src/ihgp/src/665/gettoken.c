/*
 * this file contains the tokenizer for the NPM language.
 */

/* operating system headers */
#include <stdio.h>
#include <string.h>

/* additional headers */
#include "gettoken.h"

/* externs from parser */
extern FILE *traceFd;
extern FILE *sourceFd;
extern FILE *listFd;
extern FILE *asmFd;
extern int traceFlag;
extern int semanticErrors;

/* declare structure containing reserved words */
static RESERVEDWORD reservedWords[] = {
	{ "and", T_AND },
	{ "begin", T_BEGIN },
	{ "by", T_BY },
	{ "call", T_CALL },
	{ "cycle", T_CYCLE },
	{ "declare", T_DECLARE },
	{ "div", T_DIV },
	{ "do", T_DO },
	{ "else", T_ELSE },
	{ "elsif", T_ELSEIF },
	{ "end", T_END },
	{ "exit", T_EXIT },
	{ "false", T_FALSE },
	{ "for", T_FOR },
	{ "function", T_FUNCTION },
	{ "if", T_IF },
	{ "input", T_INPUT },
	{ "loop", T_LOOP },
	{ "mod", T_MOD },
	{ "not", T_NOT },
	{ "or", T_OR },
	{ "output", T_OUTPUT },
	{ "procedure", T_PROCEDURE },
	{ "return", T_RETURN },
	{ "then", T_THEN },
	{ "to", T_TO },
	{ "true", T_TRUE },
	{ "when", T_WHEN },
	{ "", T_NULL }
};

/* names for tokens, used in debugging */
RESERVEDWORD tokenName[] = {
	"T_AND", T_AND, 
	"T_BEGIN", T_BEGIN, 
	"T_BY", T_BY, 
	"T_CALL", T_CALL, 
	"T_CYCLE", T_CYCLE, 
	"T_DECLARE", T_DECLARE, 
	"T_DIV", T_DIV, 
	"T_DO", T_DO,
	"T_ELSE", T_ELSE, 
	"T_ELSEIF", T_ELSEIF, 
	"T_END", T_END, 
	"T_EXIT", T_EXIT, 
	"T_FALSE", T_FALSE, 
	"T_FOR", T_FOR, 
	"T_FUNCTION", T_FUNCTION, 
	"T_IF", T_IF,
	"T_INPUT", T_INPUT, 
	"T_LOOP", T_LOOP, 
	"T_MOD", T_MOD, 
	"T_NOT", T_NOT, 
	"T_OR", T_OR, 
	"T_OUTPUT", T_OUTPUT, 
	"T_PROCEDURE", T_PROCEDURE, 
	"T_RETURN", T_RETURN,
	"T_THEN", T_THEN, 
	"T_TO", T_TO, 
	"T_TRUE", T_TRUE, 
	"T_WHEN", T_WHEN, 
	"T_ID", T_ID, 
	"T_NUM", T_NUM, 
	"T_STR", T_STR, 
	"T_COMMA", T_COMMA,
	"T_COLON", T_COLON, 
	"T_SEMICOLON", T_SEMICOLON, 
	"T_ASGN", T_ASGN, 
	"T_LP", T_LP, 
	"T_RP", T_RP, 
	"T_LB", T_LB, 
	"T_RB", T_RB, 
	"T_PTR", T_PTR, 
	"T_GE", T_GE,
	"T_GT", T_GT, 
	"T_LE", T_LE, 
	"T_LT", T_LT, 
	"T_EQ", T_EQ, 
	"T_NE", T_NE, 
	"T_PER", T_PER, 
	"T_MUL", T_MUL, 
	"T_SLASH", T_SLASH,
	"T_PLUS", T_PLUS, 
	"T_MINUS", T_MINUS, 
	"T_EOF", T_EOF, 
	"T_ERR", T_ERR, 
	"T_NULL", T_NULL
};

/* declare globals used by tokenizer */
char tokenString[MAXIDENTSIZE+1];
TOKEN tokenType;
long tokenValue;
static char inbuf[INBUFSIZE];
static int inbufLoc = INBUFSIZE;
static int inbufLen = 0;
static int inbufLineNo = 0;

/* local definitions */
#define IS_NUMERIC(ch) ('0' <= (ch) && (ch) <= '9')
#define IS_ALPHABETIC(ch) \
	(('a' <= (ch) && (ch) <= 'z') || ('A' <= (ch) && (ch) <= 'Z'))

/* handle errors */
void
error(fileNm, lineNo, format, s1, s2, s3, s4, s5)
char *fileNm;
int lineNo;
char *format;
unsigned long s1, s2, s3, s4, s5;
{

	/* print error msg */
	fprintf(listFd, "%s'%d: ", fileNm, lineNo);
	fprintf(listFd, "line Number %d\n", inbufLineNo);
	fprintf(listFd, "entire input line:\n<<<%s>>>\n", inbuf);
	fprintf(listFd, "remaining input line:\n<<<%s>>>\n", inbuf+inbufLoc-1);
	fprintf(listFd, format, s1, s2, s3, s4, s5);
	fprintf(listFd, "\n");
	return;
}

/* initialize tokenizer for new file */
void
initGetToken()
{
	/* set line number */
	inbufLineNo = 0;

	/* null terminate the input buffer */
	*inbuf = 0;

	/* zero out constants for reading input buffer */
	inbufLoc = 0;
	inbufLen = 0;

	/* initialize token data */
	tokenValue = 0;
	tokenType = T_NULL;
	tokenString[0] = 0;
	
	/* all done */
	return;
}

/* print leading spaces */
void
printSpaces(off)
int off;
{
	int space;
	for (space = 0; space < off + inbufLoc; space++)
	{
		fprintf(listFd, " ");
		if (traceFlag) fprintf(traceFd, " ");
	}
	fprintf(listFd, "^\n");
	if (traceFlag) fprintf(traceFd, "^\n");
	for (space = 0; space < off + inbufLoc; space++)
	{
		fprintf(listFd, " ");
		if (traceFlag) fprintf(traceFd, " ");
	}
	fprintf(listFd, "| ");
	if (traceFlag) fprintf(traceFd, "| ");
	return;
}

/* get next character in the input stream */
int
getNextChar()
{
	char *ptr;
	
	/* should a new line be read */
	if (inbufLoc >= inbufLen)
	{
		/* read next line of input */
		if ((ptr = fgets(inbuf, INBUFSIZE, sourceFd)) == (char *)0)
		{
			/* we have an error or eof */
			return(EOF);
		}

		/* set length of string and pointer to string */
		inbufLen = strlen(inbuf);
		inbufLoc = 0;
		inbufLineNo++;

		/* write to listing file */
		fprintf(listFd, "%05d: %s", inbufLineNo, inbuf);
		if (semanticErrors == 0 && asmFd != (FILE *) 0)
		{
			fprintf(asmFd, "; %05d: %s", inbufLineNo, inbuf);
		}
		if (traceFlag) fprintf(traceFd, "%05d: %s", inbufLineNo, inbuf);
	}

	/* return next string in line */
	return(inbuf[inbufLoc++]);
}

/* return a character to the stream */
void
ungetNextChar(c)
int c;
{
	/* not used for now */
	c = c;

	/* back up string pointer */
	inbufLoc--;

	/* check if we are before the start of the string */
	if (inbufLoc < 0)
	{
		/* this should never happen */
		error(__FILE__, __LINE__,
		     "ungetNextChar() - inbufLoc (%d) is less than 0.\n", 
		      inbufLoc);
		exit(1);
	}

	/* all done */
	return;
}

/* remove any leading white space */
static TOKEN
removeWhiteSpace()
{
	int c;

	/* read until eof or non-WS character */
	while (1)
	{
		/* get next character in stream */
		c = getNextChar();

		/* check for white space and skip it */         switch (c)
		{
		case ' ':
		case '\t':
		case '\n':
			/* skip spaces, tabs and new lines */
			break;

		case EOF:
			/* null terminate string and return end of file */
			tokenString[0] = c;
			tokenString[1] = 0;
			tokenType = T_EOF;
			tokenValue = 0;
			return(T_EOF);

		default:
			/* put back look-ahead character and just return */
			ungetNextChar(c);
			return(T_NULL);
		}
	}
}

/* store operator in tokenizer global variables */
TOKEN
getOperator(firstChar, secondChar, operatorType)
int firstChar, secondChar;
TOKEN operatorType;
{
	/* store operato token in globals */
	tokenString[0] = firstChar;
	tokenString[1] = secondChar;
	tokenString[2] = 0;
	tokenType = operatorType;
	tokenValue = 0;
	return(operatorType);
}

/* get identifier or reserved word from stream and return it */
TOKEN
getIdentifier()
{
	int c, tindx;

	/* verify 1st character is alphabetic */
	tindx = 0;
	c = getNextChar();
	if ( ! IS_ALPHABETIC(c))
	{
		/* unexpected character */
		error(__FILE__, __LINE__, 
		"getIdentifier: first character is not alphanumeric (%c).\n", c);
		tokenString[0] = c;
		tokenString[1] = 0;
		tokenType = T_ERR;
		tokenValue = 0;
		return(T_ERR);
	}
	tokenString[tindx++] = c;
	
	/* read remaining part of identifier */
	while (((c = getNextChar()) != EOF) && 
	       (IS_ALPHABETIC(c) || IS_NUMERIC(c)))
	{
		/* store character in token string */
		if (tindx < (MAXIDENTSIZE - 1))
		{
			tokenString[tindx++] = c;
		}
	}

	/* null terminate the string and return the look-ahead character */
	tokenString[tindx] = 0;
	ungetNextChar(c);

	/* check if identifier is a reserved word */
	for (tindx = 0; reservedWords[tindx].type != T_NULL; tindx++)
	{
		/* look for a match */
		if (strcmp(tokenString, reservedWords[tindx].name) == 0)
		{
			/* we found a reserved word */
			tokenType = reservedWords[tindx].type;
			tokenValue = 0;
			return(tokenType);
		}
	}

	/* we have a plain old identifier */
	tokenType = T_ID;
	tokenValue = 0;
	return(tokenType);
}

/* get number from stream and return it */
TOKEN
getNumber()
{
	int c, tindx;

	/* verify 1st character is numeric */
	tindx = 0;
	c = getNextChar();
	if ( ! IS_NUMERIC(c))
	{
		/* unexpected character */
		error(__FILE__, __LINE__, 
		"getNumeric: first character is not numeric (%c).\n", c);
		tokenString[0] = c;
		tokenString[1] = 0;
		tokenType = T_ERR;
		tokenValue = 0;
		return(T_ERR);
	}
	tokenString[tindx++] = c;
	
	/* read remaining part of identifier */
	while (((c = getNextChar()) != EOF) && IS_NUMERIC(c))
	{
		/* store character in token string */
		if (tindx < (MAXIDENTSIZE - 1))
		{
			tokenString[tindx++] = c;
		}
	}

	/* null terminate the string and return the look-ahead character */
	tokenString[tindx] = 0;
	ungetNextChar(c);
	tokenType = T_NUM;
	tokenValue = atoi(tokenString);
	return(tokenType);
}

/* get string from stream and return it*/
TOKEN
getString(startChar)
int startChar;
{
	int c, endc, tindx;

	/* verify 1st character is single or double quote */
	switch (startChar)
	{
	case '\'':
	case '"':
		/* we have a string of some type */
		break;
	default:
		/* unexpected character */
		error(__FILE__, __LINE__, 
		"getNumeric: first character is not quote (%c).\n", c);
		tokenString[0] = c;
		tokenString[1] = 0;
		tokenType = T_ERR;
		tokenValue = 0;
		return(T_ERR);
	}
	
	/* read remaining part of string */
	tindx = 0;
	while (((c = getNextChar()) != EOF) && (c != startChar))
	{
		/* store haracter in token string */
		if (tindx < (MAXIDENTSIZE - 1))
		{
			tokenString[tindx++] = c;
		}
		else
		{
			/* string is too long */
			error(__FILE__, __LINE__,
				"getString: string is too long.\n");
			tokenString[tindx] = 0;
			tokenType = T_ERR;
			tokenValue = 0;
			return(T_ERR);
		}
	}

	/* check for end-of-files */
	if (c == EOF)
	{
		/* not a string */
		error(__FILE__, __LINE__, 
			"unexpected character, '%c' (0x%x).\n", (char)c, c);
		tokenString[0] = c;
		tokenString[1] = 0;
		tokenType = T_EOF;
		tokenValue = 0;
		return(T_EOF);
	}


	/* null terminate the string */
	tokenString[tindx] = 0;
	tokenType = T_STR;
	tokenValue = 0;
	return(tokenType);
}
/* remove comments from stream */
TOKEN
removeComment(startChar)
char startChar;
{
	int c;

	/* remove comment depending on how comment started */
	switch (startChar)
	{
	case '{':
		/* look for closing brace */
		while (((c = getNextChar()) != EOF) && (c != '}'));
		if (c == EOF)
		{
			/* not a comment */
			error(__FILE__, __LINE__, 
				"unexpected character, '%c' (0x%x).\n", 
				(char)c, c);
			tokenString[0] = c;
			tokenString[1] = 0;
			tokenType = T_EOF;
			tokenValue = 0;
			return(T_EOF);
		}

		/* comment was removed */
		return(T_NULL);

	case '%':
		/* look for new line */
		while (((c = getNextChar()) != EOF) && (c != '\n'));
		if (c == EOF)
		{
			/* not a comment */
			error(__FILE__, __LINE__, 
				"unexpected character, '%c' (0x%x).\n", 
				(char)c, c);
			tokenString[0] = c;
			tokenString[1] = 0;
			tokenType = T_EOF;
			tokenValue = 0;
			return(T_EOF);
		}

		/* comment was removed */
		return(T_NULL);

	case '(':
		/* look for end of comment */
		while (1)
		{
			/* search for asterick */
			while (((c = getNextChar()) != EOF) && (c != '*'));
			if (c == EOF)
			{
				/* not a comment */
				error(__FILE__, __LINE__, 
					"unexpected character, '%c' (0x%x).\n", 
					(char)c, c);
				tokenString[0] = c;
				tokenString[1] = 0;
				tokenType = T_EOF;
				tokenValue = 0;
				return(T_EOF);
			}
			/* we found an asterick, look for right paren */
			if ((c = getNextChar()) == EOF)
			{
				/* not a comment */
				error(__FILE__, __LINE__, 
					"unexpected character, '%c' (0xx).\n", 
					(char)c, c);
				tokenString[0] = c;
				tokenString[1] = 0;
				tokenType = T_EOF;
				tokenValue = 0;
				return(T_EOF);
			}

			/* if a right paren, then break out of loop */
			if (c == ')') break;

			/* return character */
			ungetNextChar(c);
		}

		/* comment was removed */
		return(T_NULL);

	default:
		/* not a comment */
		error(__FILE__, __LINE__, 
		     "unexpected character, '%c' (0x%x).\n", 
		      (char)startChar, startChar);
		tokenString[0] = startChar;
		tokenString[1] = 0;
		tokenType = T_ERR;
		tokenValue = 0;
		return(T_ERR);
	}

}

/* get next token in input stream */
TOKEN 
getToken()
{
	TOKEN ret;
	ret = GetToken();
	if (traceFlag)
	{
		switch (ret)
		{
		case T_EOF:
			fprintf(traceFd, "\t\tT_EOF <EOF>\n");
			break;
		case T_ERR:
			fprintf(traceFd, "\t\tT_ERR <unknown>\n");
			break;
		case T_NULL:
			fprintf(traceFd, "\t\tT_NULL <unknown>\n");
			break;
		default:
			fprintf(traceFd, "\t\t%s <%s>\n", 
				tokenName[(int)ret].name, tokenString);
			break;
		}
	}
	return(ret);
}

TOKEN 
GetToken()
{
	int c, cnext;
	TOKEN token;

	/* check if a token already exists */
	if (tokenType != T_NULL)
	{
		/* reuse same token */
		return(tokenType);
	}

	/* start a loop to skip comments and white space */
	while  ( 1 )
	{
		/* remove any leading white space */
		switch (removeWhiteSpace())
		{
		case T_ERR:
			/* some type of error */
			error(__FILE__, __LINE__, 
			     "removeWhiteSpace() failed.\n");
			return(T_ERR);
		case T_EOF:
			/* end of file */
			return(T_EOF);
		default:
			/* white saace was removed, continue parsing */
			break;
		}

		/* get next character in stream */
		if ((c = getNextChar()) == EOF)
		{
			/* we got an end of file */
			tokenString[0] = c;
			tokenString[1] = 0;
			tokenType = T_EOF;
			tokenValue = 0;
			return(T_EOF);
		}

		/* check next character for beginning of an identifier */
		if (IS_ALPHABETIC(c))
		{
			/* we have an alphabetic character */
			ungetNextChar(c);
			return(getIdentifier());
		}
		else if (IS_NUMERIC(c))
		{
			/* we have an numeric character */
			ungetNextChar(c);
			return(getNumber());
		}
		/* check if the next token is an operator or a comment */
		switch (c)
		{
		case '{':
		case '%':
			/* remove comment */
			if (removeComment(c) == T_ERR) return(T_ERR);
			break;

		case ',':
			/* we have a comma */
			return(getOperator(c, 0, T_COMMA));

		case ':':
			/* check next character */
			if ((cnext = getNextChar()) == '=')
			{
				/* we have an assignment */
				return(getOperator(c, cnext, T_ASGN));
			}
			else
			{
				/* we have a colon */
				ungetNextChar(cnext);
				return(getOperator(c, 0, T_COLON));
			}

		case ';':
			/* we have a semicolon */
		return(getOperator(c, 0, T_SEMICOLON));

		case '(':
			/* check next character */
			if ((cnext = getNextChar()) == '*')
			{
				/* we have a comment */
				if (removeComment(c) == T_ERR) return(T_ERR);
				continue;
			}
			else
			{
				/* we have a left parenthesis */
				ungetNextChar(cnext);
				return(getOperator(c, 0, T_LP));
			}

		case ')':
			/* we have a right parenthesis */
			return(getOperator(c, 0, T_RP));

		case '[':
			/* we have a left bracket */
			return(getOperator(c, 0, T_LB));

		case ']':
			/* we have a right bracket */
			return(getOperator(c, 0, T_RB));

		case '^':
			/* we have a caret */
			return(getOperator(c, 0, T_PTR));

		case '>':
			/* check next character */
			if ((cnext = getNextChar()) == '=')
			{
				/* we have a greater than or equal */
				return(getOperator(c, cnext, T_GE));
			}
			else
			{
				/* we have a greater than */
				ungetNextChar(cnext);
				return(getOperator(c, 0, T_GT));
			}

		case '<':
			/* check next character */
			if ((cnext = getNextChar()) == '=')
			{
				/* we have a less than or equal */
				return(getOperator(c, cnext, T_LE));
			}
			else if (cnext == '>')
			{
				/* we have a not equal */
				return(getOperator(c, cnext, T_NE));
			}
			else
			{
				/* we have a less than */
				ungetNextChar(cnext);
				return(getOperator(c, 0, T_LT));
			}

		case '=':
			/* we have an equal */
			return(getOperator(c, 0, T_EQ));

		case '.':
			/* we have a period */
			return(getOperator(c, 0, T_PER));

		case '*':
			/* we have a multiplier */
			return(getOperator(c, 0, T_MUL));

		case '/':
			/* we have a slash */
			return(getOperator(c, 0, T_SLASH));

		case '+':
			/* we have a plus */
			return(getOperator(c, 0, T_PLUS));

		case '-':
			/* we have a minus */
			return(getOperator(c, 0, T_MINUS));

		case '"':
		case '\'':
			/* we have a string */
			return(getString(c));

		default:
			/* unknown or unexpected character */
			error(__FILE__, __LINE__, 
			     "unexpected character, '%c' (0x%x).\n", 
			      (char)c, c);
			tokenString[0] = c;
			tokenString[1] = 0;
			tokenType = T_ERR;
			tokenValue = 0;
			return(T_ERR);
		}
	}
}
