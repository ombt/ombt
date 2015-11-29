
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif
 {
	int flag;
	char cstring[32];
	Semantic *psemantic;
	List<Semantic *> *parguments;
} YYSTYPE;
extern YYSTYPE yylval;
# define AND 257
# define ARGUMENT 258
# define BICONDITIONAL 259
# define COMMA 260
# define FALSE 261
# define IDENTIFIER 262
# define EQUAL 263
# define NOTEQUAL 264
# define IMPLICATION 265
# define LEFTEXISTENTIAL 266
# define LEFTUNIVERSAL 267
# define LPAREN 268
# define NEGATION 269
# define NUMBER 270
# define OR 271
# define PIDENTIFIER 272
# define QUIT 273
# define RIGHTEXISTENTIAL 274
# define RIGHTUNIVERSAL 275
# define RPAREN 276
# define SEMICOLON 277
# define SOS 278
# define STRING 279
# define THEREFORE 280
# define TRUE 281
# define LBRACE 282
# define RBRACE 283
