
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif
 {
	TYPE type;
	VALUE number; 
	char string[32];
} YYSTYPE;
extern YYSTYPE yylval;
# define DCONST 257
# define LCONST 258
# define ULCONST 259
# define ACOS 260
# define AND 261
# define ASIN 262
# define ATAN 263
# define BITAND 264
# define BITOR 265
# define BITXOR 266
# define COMMA 267
# define COS 268
# define EQUAL 269
# define EXP 270
# define GE 271
# define GRAPH 272
# define GT 273
# define HELP 274
# define LE 275
# define LOG 276
# define LOG10 277
# define LPAREN 278
# define LSHIFT 279
# define LT 280
# define MINUS 281
# define NEWLINE 282
# define NOT 283
# define NOTEQUAL 284
# define OR 285
# define OUTBASE 286
# define PERCENT 287
# define PLUS 288
# define POW 289
# define QUIT 290
# define RPAREN 291
# define RSHIFT 292
# define SEMICOLON 293
# define SIN 294
# define SLASH 295
# define SQRT 296
# define STAR 297
# define STRING 298
# define TAN 299
# define TILDE 300
# define DCAST 301
# define LCAST 302
# define ULCAST 303
