
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif
 {
	char cstring[32];
	Semantic *psemantic;
	List<Semantic * > *parguments;
} YYSTYPE;
extern YYSTYPE yylval;
# define AND 257
# define ARGUMENT 258
# define BICONDITIONAL 259
# define COLON 260
# define COMMA 261
# define EQUAL 262
# define EXISTENTIAL 263
# define FALSE 264
# define IDENTIFIER 265
# define IMPLICATION 266
# define LBRACE 267
# define LEFTEXISTENTIAL 268
# define LEFTUNIVERSAL 269
# define LPAREN 270
# define MINUS 271
# define NEGATION 272
# define NOTEQUAL 273
# define NUMBER 274
# define OPTION 275
# define OR 276
# define PIDENTIFIER 277
# define QUERY 278
# define QUIT 279
# define RBRACE 280
# define RIGHTEXISTENTIAL 281
# define RIGHTUNIVERSAL 282
# define RPAREN 283
# define SEMICOLON 284
# define SOS 285
# define STRING 286
# define THEREFORE 287
# define TRUE 288
# define UNIVERSAL 289
