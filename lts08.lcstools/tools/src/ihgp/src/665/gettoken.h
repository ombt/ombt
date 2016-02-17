#ifndef __GETTOKEN_H
#define __GETTOKEN_H
/*
 * define all enumerations, types and constants for tokenizer.
 */

/* required headers */
#include <stdio.h>

/* enumeration for tokens */
enum TOKENTYPE {
	T_AND, T_BEGIN, T_BY, T_CALL, T_CYCLE, T_DECLARE, T_DIV, T_DO,
	T_ELSE, T_ELSEIF, T_END, T_EXIT, T_FALSE, T_FOR, T_FUNCTION, T_IF,
	T_INPUT, T_LOOP, T_MOD, T_NOT, T_OR, T_OUTPUT, T_PROCEDURE, T_RETURN,
	T_THEN, T_TO, T_TRUE, T_WHEN, T_ID, T_NUM, T_STR, T_COMMA,
	T_COLON, T_SEMICOLON, T_ASGN, T_LP, T_RP, T_LB, T_RB, T_PTR, T_GE,
	T_GT, T_LE, T_LT, T_EQ, T_NE, T_PER, T_MUL, T_SLASH,
	T_PLUS, T_MINUS, T_EOF, T_ERR, T_NULL
};
typedef enum TOKENTYPE TOKEN;

/* structure for storing reserved words */
struct RESERVEDWORDTYPE {
	char *name;
	TOKEN type;
};
typedef struct RESERVEDWORDTYPE RESERVEDWORD;

/* define constants for tokenizer */
#define MAXIDENTSIZE 31
#define INBUFSIZE 1024

/* define globals used by tokenizer */
extern char tokenString[];
extern TOKEN tokenType;
extern long tokenValue;

/* define tokenizer functions */
extern TOKEN getToken();
extern TOKEN GetToken();
extern int getNextChar();
extern void ungetNextChar();
extern void printSpaces();

#endif
