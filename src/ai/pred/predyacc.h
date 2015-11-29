
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif
 {
	char cstring[32];
	Semantic *psemantic;
	List<Semantic *> *parguments;
} YYSTYPE;
extern YYSTYPE yylval;
# define AND 257
# define BICONDITIONAL 258
# define COMMA 259
# define FALSE 260
# define IDENTIFIER 261
# define IMPLICATION 262
# define LEFTEXISTENTIAL 263
# define LEFTUNIVERSAL 264
# define LPAREN 265
# define NEGATION 266
# define NEWLINE 267
# define NUMBER 268
# define OR 269
# define PIDENTIFIER 270
# define QUIT 271
# define RIGHTEXISTENTIAL 272
# define RIGHTUNIVERSAL 273
# define RPAREN 274
# define SEMICOLON 275
# define STRING 276
# define THEREFORE 277
# define TRUE 278
