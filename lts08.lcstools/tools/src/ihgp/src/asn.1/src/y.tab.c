
# line 2 "asn1.y"
/* headers */
#include "asn1.h"

/* externs */
void yyerror(const char *);


# line 14 "asn1.y"
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif
 {
	char string[128];
} YYSTYPE;
# define typereference 257
# define identifier 258
# define valuereference 259
# define modulereference 260
# define comment 261
# define number 262
# define realnumber 263
# define bstring 264
# define hstring 265
# define cstring 266
# define xmlbstring 267
# define xmlhstring 268
# define xmlcstring 269
# define xmlasn1typename 270
# define true 271
# define false 272
# define ASTERICK 273
# define AMPERSAND 274
# define COLONCOLONEQUAL 275
# define DOUBLELEFTBRACKET 276
# define DOUBLERIGHTBRACKET 277
# define TWOPERIODS 278
# define ELLIPSIS 279
# define LESSTHANSLASH 280
# define SLASHGREATERTHAN 281
# define LEFTBRACE 282
# define RIGHTBRACE 283
# define LESSTHAN 284
# define GREATERTHAN 285
# define COMMA 286
# define PERIOD 287
# define LEFTPAREN 288
# define RIGHTPAREN 289
# define LEFTBRACKET 290
# define RIGHTBRACKET 291
# define DASH 292
# define COLON 293
# define EQUAL 294
# define QUOTATION 295
# define APOSTROPHE 296
# define SPACE 297
# define SEMICOLON 298
# define AT 299
# define PIPE 300
# define EXCLAMATION 301
# define CARET 302
# define ABSENT 303
# define ABSTRACTSYNTAX 304
# define ALL 305
# define APPLICATION 306
# define AUTOMATIC 307
# define BEGIN 308
# define BIT 309
# define BMPString 310
# define BOOLEAN 311
# define BY 312
# define CHARACTER 313
# define CHOICE 314
# define CLASS 315
# define COMPONENT 316
# define COMPONENTS 317
# define CONSTRAINED 318
# define CONTAINING 319
# define DEFAULT 320
# define DEFINITIONS 321
# define EMBEDDED 322
# define ENCODED 323
# define END 324
# define ENUMERATED 325
# define EXCEPT 326
# define EXPLICIT 327
# define EXPORTS 328
# define EXTENSIBILITY 329
# define EXTERNAL 330
# define FALSE 331
# define FROM 332
# define GeneralizedTime 333
# define GeneralString 334
# define GraphicString 335
# define IA5String 336
# define IDENTIFIER 337
# define IMPLICIT 338
# define IMPLIED 339
# define IMPORTS 340
# define INCLUDES 341
# define INSTANCE 342
# define INTEGER 343
# define INTERSECTION 344
# define ISO646String 345
# define MAX 346
# define MIN 347
# define MINUSINFINITY 348
# define NULL 349
# define NumericString 350
# define OBJECT 351
# define ObjectDescriptor 352
# define OCTET 353
# define OF 354
# define OPTIONAL 355
# define PATTERN 356
# define PDV 357
# define PLUSINFINITY 358
# define PRESENT 359
# define PrintableString 360
# define PRIVATE 361
# define REAL 362
# define RELATIVEOID 363
# define SEQUENCE 364
# define SET 365
# define SIZE 366
# define STRING 367
# define SYNTAX 368
# define T61String 369
# define TAGS 370
# define TeletexString 371
# define TRUE 372
# define TYPEIDENTIFIER 373
# define UNION 374
# define UNIQUE 375
# define UNIVERSAL 376
# define UniversalString 377
# define UTCTime 378
# define UTF8String 379
# define VideotexString 380
# define VisibleString 381
# define WITH 382

#include <inttypes.h>

#ifdef __STDC__
#include <stdlib.h>
#include <string.h>
#define	YYCONST	const
#else
#include <malloc.h>
#include <memory.h>
#define	YYCONST
#endif

#include <values.h>

#if defined(__cplusplus) || defined(__STDC__)

#if defined(__cplusplus) && defined(__EXTERN_C__)
extern "C" {
#endif
#ifndef yyerror
#if defined(__cplusplus)
	void yyerror(YYCONST char *);
#endif
#endif
#ifndef yylex
	int yylex(void);
#endif
	int yyparse(void);
#if defined(__cplusplus) && defined(__EXTERN_C__)
}
#endif

#endif

#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
YYSTYPE yylval;
YYSTYPE yyval;
typedef int yytabelem;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#if YYMAXDEPTH > 0
int yy_yys[YYMAXDEPTH], *yys = yy_yys;
YYSTYPE yy_yyv[YYMAXDEPTH], *yyv = yy_yyv;
#else	/* user does initial allocation */
int *yys;
YYSTYPE *yyv;
#endif
static int yymaxdepth = YYMAXDEPTH;
# define YYERRCODE 256

# line 1331 "asn1.y"


main(int argc, char **argv)
{
	yyparse();
}
