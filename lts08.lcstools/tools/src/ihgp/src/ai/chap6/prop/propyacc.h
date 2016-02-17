
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif
 {
	char string[32];
	Semantic *precord;
} YYSTYPE;
extern YYSTYPE yylval;
# define NEWLINE 257
# define COMMA 258
# define SEMICOLON 259
# define THEREFORE 260
# define BICONDITIONAL 261
# define IMPLICATION 262
# define OR 263
# define AND 264
# define NEGATION 265
# define LPAREN 266
# define RPAREN 267
# define IDENTIFIER 268
# define QUIT 269
# define TRUE 270
# define FALSE 271
