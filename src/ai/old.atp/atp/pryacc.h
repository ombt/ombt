
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
# define EXISTENTIAL 266
# define LEFTEXISTENTIAL 267
# define LEFTUNIVERSAL 268
# define UNIVERSAL 269
# define LPAREN 270
# define NEGATION 271
# define NUMBER 272
# define NEGNUMBER 273
# define OR 274
# define PIDENTIFIER 275
# define QUIT 276
# define RIGHTEXISTENTIAL 277
# define RIGHTUNIVERSAL 278
# define RPAREN 279
# define SEMICOLON 280
# define SOS 281
# define STRING 282
# define THEREFORE 283
# define QUERY 284
# define TRUE 285
# define LBRACE 286
# define RBRACE 287
# define OPTION 288
