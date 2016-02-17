
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
# define NEWLINE 257
# define OR 258
# define AND 259
# define BITOR 260
# define BITXOR 261
# define BITAND 262
# define EQUAL 263
# define NOTEQUAL 264
# define LT 265
# define GT 266
# define LE 267
# define GE 268
# define LSHIFT 269
# define RSHIFT 270
# define PLUS 271
# define MINUS 272
# define STAR 273
# define SLASH 274
# define PERCENT 275
# define LPAREN 276
# define RPAREN 277
# define TILDE 278
# define NOT 279
# define STRING 280
# define QUIT 281
# define OUTBASE 282
# define HELP 283
# define EXP 284
# define LOG 285
# define LOG10 286
# define POW 287
# define SQRT 288
# define SIN 289
# define COS 290
# define TAN 291
# define ASIN 292
# define ACOS 293
# define ATAN 294
# define COMMA 295
# define ULCONST 296
# define LCONST 297
# define DCONST 298
# define ULCAST 299
# define LCAST 300
# define DCAST 301
