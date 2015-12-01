/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "pred.y" /* yacc.c:339  */

// required headers
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <unistd.h>

// other headers
#include "pred.h"

// local definitions
#define IfDebug(DebugLevel, DebugMsg, StuffToDump) \
	if (DebugLevel <= currentDebugLevel) \
	{ \
		std::cout << DebugMsg << std::endl; \
		std::cout << StuffToDump << std::endl; \
	}

#define IfDebugCall(DebugLevel, Called) \
	if (DebugLevel <= currentDebugLevel) \
	{ \
		Called; \
	}

// phases for semantic processing
#define PHASE1 1
#define PHASE2 (PHASE1+1)
#define PHASE3 (PHASE2+1)
#define PHASE4 (PHASE3+1)
#define PHASE5 (PHASE4+1)
#define PHASE6 (PHASE5+1)
#define PHASE7 (PHASE6+1)
#define PHASE8 (PHASE7+1)
#define PHASE9 (PHASE8+1)
#define PHASE10 (PHASE9+1)
#define MAXPHASE PHASE10

// externs
extern int yylineno;
extern int optind;
extern char *optarg;
extern int yylex();
extern void yyerror(const char *s);

// symbol table
ombt::BinaryTree_AVL<Symbol> symbols;

// program data
ombt::List<Symbol> scope;
ombt::List<Semantic * > ptrees;
ombt::List<ombt::List<Semantic * > > programs;
ombt::List<ombt::List<IRClause> > clausesPrograms;

// other globals
int currentDebugLevel = 0;
int maxPhase = MAXPHASE;
char errormsg[BUFSIZ];
int syntaxErrors = 0;


#line 129 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    AND = 258,
    BICONDITIONAL = 259,
    COMMA = 260,
    FALSE = 261,
    IDENTIFIER = 262,
    IMPLICATION = 263,
    LEFTEXISTENTIAL = 264,
    LEFTUNIVERSAL = 265,
    LPAREN = 266,
    NEGATION = 267,
    NEWLINE = 268,
    NUMBER = 269,
    OR = 270,
    PIDENTIFIER = 271,
    QUIT = 272,
    RIGHTEXISTENTIAL = 273,
    RIGHTUNIVERSAL = 274,
    RPAREN = 275,
    SEMICOLON = 276,
    STRING = 277,
    THEREFORE = 278,
    TRUE = 279
  };
#endif
/* Tokens.  */
#define AND 258
#define BICONDITIONAL 259
#define COMMA 260
#define FALSE 261
#define IDENTIFIER 262
#define IMPLICATION 263
#define LEFTEXISTENTIAL 264
#define LEFTUNIVERSAL 265
#define LPAREN 266
#define NEGATION 267
#define NEWLINE 268
#define NUMBER 269
#define OR 270
#define PIDENTIFIER 271
#define QUIT 272
#define RIGHTEXISTENTIAL 273
#define RIGHTUNIVERSAL 274
#define RPAREN 275
#define SEMICOLON 276
#define STRING 277
#define THEREFORE 278
#define TRUE 279

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 66 "pred.y" /* yacc.c:355  */

	char cstring[32];
	Semantic *psemantic;
	ombt::List<Semantic *> *parguments;

#line 223 "y.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 238 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   74

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  25
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  47
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  76

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   279

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   115,   115,   117,   123,   129,   134,   141,   148,   158,
     159,   163,   168,   176,   183,   187,   196,   200,   209,   213,
     222,   226,   235,   239,   245,   266,   290,   324,   358,   362,
     369,   376,   383,   409,   453,   462,   470,   477,   481,   485,
     492,   498,   507,   574,   618,   622,   634,   638
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "AND", "BICONDITIONAL", "COMMA", "FALSE",
  "IDENTIFIER", "IMPLICATION", "LEFTEXISTENTIAL", "LEFTUNIVERSAL",
  "LPAREN", "NEGATION", "NEWLINE", "NUMBER", "OR", "PIDENTIFIER", "QUIT",
  "RIGHTEXISTENTIAL", "RIGHTUNIVERSAL", "RPAREN", "SEMICOLON", "STRING",
  "THEREFORE", "TRUE", "$accept", "start", "expressionlist",
  "separator_expressionlist", "expression", "biconditional", "implication",
  "or", "and", "unary", "universal", "existential", "atom", "predicate",
  "arglist", "arg", "term", "constant", "variable", "function",
  "separator", "conclusion", "linesynch", "newline", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279
};
# endif

#define YYPACT_NINF -52

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-52)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -52,     7,   -52,    -9,   -52,     5,    19,    46,    46,   -52,
      16,   -10,    46,   -52,    30,     4,   -52,    25,    39,    22,
      45,   -52,    46,    46,   -52,   -52,    40,   -52,   -52,    32,
      41,    31,   -52,    52,    -9,   -52,   -52,   -52,     4,    40,
     -52,    46,   -52,    46,    46,    46,    46,   -52,   -52,   -52,
     -52,   -52,   -52,    53,   -52,   -52,    23,   -52,   -52,   -52,
     -52,   -52,   -52,   -52,   -52,   -52,    39,    22,    45,   -52,
      52,   -52,    52,    29,   -52,   -52
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,    30,     0,     0,     0,     0,    47,
      32,     0,     0,    29,     0,     0,    11,    13,    14,    16,
      18,    20,     0,     0,    22,    28,     0,     5,     6,     0,
       0,     0,    23,     0,     0,     7,     8,    45,     0,     0,
      44,     0,     9,     0,     0,     0,     0,    24,    25,     3,
      27,    26,    31,    42,    41,    40,     0,    34,    36,    37,
      38,    39,    46,    10,     4,    12,    15,    17,    19,    21,
       0,    33,     0,     0,    35,    43
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -52,   -52,   -52,    49,    -6,   -52,    24,    21,    26,    -8,
     -52,   -52,   -52,   -52,    -2,    -3,   -52,   -52,   -52,   -52,
     -51,    58,     6,    -1
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    56,    57,    58,    59,    60,    61,
      41,    26,    35,    27
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      32,    31,    28,     9,     9,    72,    37,     2,     3,    40,
      36,    34,    29,     4,    47,    48,     5,     6,     7,     8,
       9,    42,    72,    10,    11,    34,    30,    33,    40,    43,
      12,    13,    49,    62,    40,    65,     4,    45,    69,     5,
       6,     7,     8,    71,    63,    64,    10,    44,    46,    75,
      50,    52,     4,    12,    13,     5,     6,     7,     8,    53,
      51,    34,    10,    38,    70,    67,    54,    66,    73,    74,
      13,    68,    39,     0,    55
};

static const yytype_int8 yycheck[] =
{
       8,     7,     3,    13,    13,    56,    12,     0,     1,     5,
      11,    21,     7,     6,    22,    23,     9,    10,    11,    12,
      13,    15,    73,    16,    17,    21,     7,    11,     5,     4,
      23,    24,    26,    34,     5,    41,     6,    15,    46,     9,
      10,    11,    12,    20,    38,    39,    16,     8,     3,    20,
      18,    20,     6,    23,    24,     9,    10,    11,    12,     7,
      19,    21,    16,    14,    11,    44,    14,    43,    70,    72,
      24,    45,    14,    -1,    22
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    26,     0,     1,     6,     9,    10,    11,    12,    13,
      16,    17,    23,    24,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    46,    48,    48,     7,
       7,    29,    34,    11,    21,    47,    48,    29,    28,    46,
       5,    45,    47,     4,     8,    15,     3,    34,    34,    47,
      18,    19,    20,     7,    14,    22,    39,    40,    41,    42,
      43,    44,    48,    47,    47,    29,    31,    32,    33,    34,
      11,    20,    45,    39,    40,    20
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    25,    26,    26,    26,    26,    26,    26,    26,    27,
      27,    28,    28,    29,    30,    30,    31,    31,    32,    32,
      33,    33,    34,    34,    34,    34,    35,    36,    37,    37,
      37,    37,    38,    38,    39,    39,    40,    41,    41,    41,
      42,    42,    43,    44,    45,    46,    47,    48
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     3,     4,     2,     3,     3,     3,     2,
       3,     1,     3,     1,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     2,     2,     2,     3,     3,     1,     1,
       1,     3,     1,     4,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     4,     1,     2,     2,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 118 "pred.y" /* yacc.c:1646  */
    {
		fprintf(stdout, "argument syntax accepted ...\n");
		scope.clear();
		symbols.clear();
	}
#line 1369 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 124 "pred.y" /* yacc.c:1646  */
    {
		fprintf(stdout, "argument syntax accepted ...\n");
		scope.clear();
		symbols.clear();
	}
#line 1379 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 130 "pred.y" /* yacc.c:1646  */
    {
		scope.clear();
		symbols.clear();
	}
#line 1388 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 135 "pred.y" /* yacc.c:1646  */
    {
		fprintf(stdout, "syntax error detected ...\n");
		scope.clear();
		symbols.clear();
		yyerrok;
	}
#line 1399 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 142 "pred.y" /* yacc.c:1646  */
    {
		fprintf(stdout, "quitting ...\n");
		scope.clear();
		symbols.clear();
		YYACCEPT;
	}
#line 1410 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 149 "pred.y" /* yacc.c:1646  */
    {
		fprintf(stdout, "quitting ...\n");
		scope.clear();
		symbols.clear();
		YYACCEPT;
	}
#line 1421 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 164 "pred.y" /* yacc.c:1646  */
    {
		/* save expression parse tree */
		ptrees.insertAtEnd((yyvsp[0].psemantic));
	}
#line 1430 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 169 "pred.y" /* yacc.c:1646  */
    {
		/* save expression parse tree */
		ptrees.insertAtEnd((yyvsp[0].psemantic));
	}
#line 1439 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 177 "pred.y" /* yacc.c:1646  */
    {
		(yyval.psemantic) = (yyvsp[0].psemantic);
	}
#line 1447 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 184 "pred.y" /* yacc.c:1646  */
    {
		(yyval.psemantic) = (yyvsp[0].psemantic);
	}
#line 1455 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 188 "pred.y" /* yacc.c:1646  */
    {
		/* create a biconditional record */
		(yyval.psemantic) = new Semantic(Expression::Biconditional, (yyvsp[-2].psemantic), (yyvsp[0].psemantic));
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 1465 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 197 "pred.y" /* yacc.c:1646  */
    {
		(yyval.psemantic) = (yyvsp[0].psemantic);
	}
#line 1473 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 201 "pred.y" /* yacc.c:1646  */
    {
		/* create a conditional record */
		(yyval.psemantic) = new Semantic(Expression::Conditional, (yyvsp[-2].psemantic), (yyvsp[0].psemantic));
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 1483 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 210 "pred.y" /* yacc.c:1646  */
    {
		(yyval.psemantic) = (yyvsp[0].psemantic);
	}
#line 1491 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 214 "pred.y" /* yacc.c:1646  */
    {
		/* create an OR record */
		(yyval.psemantic) = new Semantic(Expression::Or, (yyvsp[-2].psemantic), (yyvsp[0].psemantic));
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 1501 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 223 "pred.y" /* yacc.c:1646  */
    {
		(yyval.psemantic) = (yyvsp[0].psemantic);
	}
#line 1509 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 227 "pred.y" /* yacc.c:1646  */
    {
		/* create an AND record */
		(yyval.psemantic) = new Semantic(Expression::And, (yyvsp[-2].psemantic), (yyvsp[0].psemantic));
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 1519 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 236 "pred.y" /* yacc.c:1646  */
    {
		(yyval.psemantic) = (yyvsp[0].psemantic);
	}
#line 1527 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 240 "pred.y" /* yacc.c:1646  */
    {
		/* create an NEGATION record */
		(yyval.psemantic) = new Semantic(Expression::Negation, NULL, (yyvsp[0].psemantic));
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 1537 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 246 "pred.y" /* yacc.c:1646  */
    {
		/* create an NEGATION record */
		(yyval.psemantic) = new Semantic(Expression::Universal, ombt::String((yyvsp[-1].cstring)), (yyvsp[0].psemantic));
		MustBeTrue((yyval.psemantic) != NULL);

		/* end scope */
		Symbol uvarsym;
		MustBeTrue(scope.removeAtFront(uvarsym) == OK);
		IfDebug(5, "ending a universal scope ...", scope);

		/* check if variable was used */
		if (uvarsym.getUsed() <= 0)
		{
			sprintf(errormsg, 
				"variable %s not used.\n", 
				(char *)uvarsym.getName());
			yyerror(errormsg);
			YYERROR;
		}
	}
#line 1562 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 267 "pred.y" /* yacc.c:1646  */
    {
		/* create an NEGATION record */
		(yyval.psemantic) = new Semantic(Expression::Existential, (yyvsp[-1].cstring), (yyvsp[0].psemantic));
		MustBeTrue((yyval.psemantic) != NULL);

		/* end scope */
		Symbol uvarsym;
		MustBeTrue(scope.removeAtFront(uvarsym) == OK);
		IfDebug(5, "ending an existential scope ...", scope);

		/* check if variable was used */
		if (uvarsym.getUsed() <= 0)
		{
			sprintf(errormsg, 
				"variable %s not used.\n", 
				(char *)uvarsym.getName());
			yyerror(errormsg);
			YYERROR;
		}
	}
#line 1587 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 291 "pred.y" /* yacc.c:1646  */
    {
		/* return string name */
		strcpy((yyval.cstring), (yyvsp[-1].cstring));

		/* check if symbol exists in global symbol table */
		Symbol newqvsym(ombt::String((yyvsp[-1].cstring)), Symbol::UniversalVariable);
		if (symbols.retrieve(newqvsym) == OK)
		{
			/* symbol was found */
			if (newqvsym.getType() != Symbol::Variable)
			{
				/* type of symbol changed */
				sprintf(errormsg, 
					"type changed for %s.\n", (yyvsp[-2].cstring));
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into symbol table */
			symbols.insert(Symbol(ombt::String((yyvsp[-1].cstring)), 
				Symbol::Variable));
		}

		/* insert name into scope */
		scope.insertAtFront(Symbol(ombt::String((yyvsp[-1].cstring)), 
			Symbol::UniversalVariable));
		IfDebug(5, "starting a new universal scope ...", scope);
	}
#line 1622 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 325 "pred.y" /* yacc.c:1646  */
    {
		/* return string name */
		strcpy((yyval.cstring), (yyvsp[-1].cstring));

		/* check if symbol exists in global symbol table */
		Symbol newqvsym(ombt::String((yyvsp[-1].cstring)), Symbol::ExistentialVariable);
		if (symbols.retrieve(newqvsym) == OK)
		{
			/* symbol was found */
			if (newqvsym.getType() != Symbol::Variable)
			{
				/* type of symbol changed */
				sprintf(errormsg, 
					"type changed for %s.\n", (yyvsp[-2].cstring));
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into symbol table */
			symbols.insert(Symbol(ombt::String((yyvsp[-1].cstring)), 
				Symbol::Variable));
		}

		/* insert name into scope */
		scope.insertAtFront(Symbol(ombt::String((yyvsp[-1].cstring)),
			Symbol::ExistentialVariable));
		IfDebug(5, "starting a new existential scope ...", scope);
	}
#line 1657 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 359 "pred.y" /* yacc.c:1646  */
    {
		(yyval.psemantic) = (yyvsp[0].psemantic);
	}
#line 1665 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 363 "pred.y" /* yacc.c:1646  */
    {
		/* create a predicate logical record */
		(yyval.psemantic) = new Semantic(Predicate::LogicalConstant, 
				ombt::String(""), ombt::String("True"));
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 1676 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 370 "pred.y" /* yacc.c:1646  */
    {
		/* create a logical constant record */
		(yyval.psemantic) = new Semantic(Predicate::LogicalConstant, 
				ombt::String(""), ombt::String("False"));
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 1687 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 377 "pred.y" /* yacc.c:1646  */
    {
		(yyval.psemantic) = (yyvsp[-1].psemantic);
	}
#line 1695 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 384 "pred.y" /* yacc.c:1646  */
    {
		/* check if symbol exists */
		Symbol newsym(ombt::String((yyvsp[0].cstring)), Symbol::PredicateConstant);
		if (symbols.retrieve(newsym) == OK)
		{
			/* symbol was found */
			if (newsym.getType() != Symbol::PredicateConstant)
			{
				/* type of symbol changed */
				sprintf(errormsg, 
					"type changed for %s.\n", (yyvsp[0].cstring));
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into table */
			symbols.insert(newsym);
		}

		/* create a predicate constant record */
		(yyval.psemantic) = new Semantic(Predicate::Constant, ombt::String((yyvsp[0].cstring)), ombt::String(""));
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 1725 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 410 "pred.y" /* yacc.c:1646  */
    {
		/* count the number of arguments */
		int nargs;
		ombt::ListIterator<Semantic * > argsIter(*(yyvsp[-1].parguments));
		for (nargs = 0; !argsIter.done(); argsIter++, nargs++);

		/* check if symbol exists */
		Symbol newsym(ombt::String((yyvsp[-3].cstring)), Symbol::PredicateFunction, nargs);
		if (symbols.retrieve(newsym) == OK)
		{
			/* symbol was found, type should not change */
			if (newsym.getType() != Symbol::PredicateFunction)
			{
				sprintf(errormsg, "type changed for %s.\n", (yyvsp[-3].cstring));
				yyerror(errormsg);
				YYERROR;
			}
			if (newsym.getArgs() != nargs)
			{
				char buffer[BUFSIZ];
				sprintf(buffer, "number of args changed for %s.\n", (yyvsp[-3].cstring));
				strcpy(errormsg, buffer);
				sprintf(buffer, "old number of args = %d.\n", newsym.getArgs());
				strcat(errormsg, buffer);
				sprintf(buffer, "new number of args = %d.\n", nargs);
				strcat(errormsg, buffer);
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into table */
			symbols.insert(newsym);
		}

		/* create a predicate record */
		(yyval.psemantic) = new Semantic(Predicate::Function, ombt::String((yyvsp[-3].cstring)), (yyvsp[-1].parguments), nargs);
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 1770 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 454 "pred.y" /* yacc.c:1646  */
    {
		/* create an argument list */
		(yyval.parguments) = new ombt::List<Semantic *>;
		MustBeTrue((yyval.parguments) != NULL);

		/* insert an argument in the list */
		(yyval.parguments)->insertAtEnd((yyvsp[0].psemantic));
	}
#line 1783 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 463 "pred.y" /* yacc.c:1646  */
    {
		/* insert an argument in the list */
		(yyval.parguments)->insertAtEnd((yyvsp[0].psemantic));
	}
#line 1792 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 471 "pred.y" /* yacc.c:1646  */
    {
		(yyval.psemantic) = (yyvsp[0].psemantic);
	}
#line 1800 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 478 "pred.y" /* yacc.c:1646  */
    {
		(yyval.psemantic) = (yyvsp[0].psemantic);
	}
#line 1808 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 482 "pred.y" /* yacc.c:1646  */
    {
		(yyval.psemantic) = (yyvsp[0].psemantic);
	}
#line 1816 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 486 "pred.y" /* yacc.c:1646  */
    {
		(yyval.psemantic) = (yyvsp[0].psemantic);
	}
#line 1824 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 493 "pred.y" /* yacc.c:1646  */
    {
		/* create a string record */
		(yyval.psemantic) = new Semantic(Argument::QuotedString, ombt::String((yyvsp[0].cstring)));
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 1834 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 499 "pred.y" /* yacc.c:1646  */
    {
		/* create a number record */
		(yyval.psemantic) = new Semantic(Argument::Number, ombt::String((yyvsp[0].cstring)));
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 1844 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 508 "pred.y" /* yacc.c:1646  */
    {
		/* search if name is in the scope */
		Symbol varsym(ombt::String((yyvsp[0].cstring)));
		if (scope.retrieve(varsym) == OK)
		{
			/* increment usage */
			IfDebug(5, "varsym incremented (before) ...", varsym);
			varsym++;
			IfDebug(5, "varsym incremented (after) ...", varsym);
			IfDebug(5, "scope updated (before) ...", scope);
			scope.update(varsym);
			IfDebug(5, "scope updated (after) ...", scope);

			/* we have a variable, verify type did not change */
			Symbol sym(ombt::String((yyvsp[0].cstring)));
			if (symbols.retrieve(sym) != OK)
			{
				sprintf(errormsg, 
					"variable %s not in symbol table.\n", 
					(yyvsp[0].cstring));
				yyerror(errormsg);
				YYERROR;
			}
			if (sym.getType() != Symbol::Variable)
			{
				sprintf(errormsg, 
					"symbol %s not a variable.\n", 
					(yyvsp[0].cstring));
				yyerror(errormsg);
				YYERROR;
			}

			/* create a variable semantic record */
			(yyval.psemantic) = new Semantic(Argument::Variable, ombt::String((yyvsp[0].cstring)));
			MustBeTrue((yyval.psemantic) != NULL);
		}
		else
		{
			/* we have a constant, verify type did not change */
			Symbol sym(ombt::String((yyvsp[0].cstring)));
			if (symbols.retrieve(sym) == OK)
			{
				if (sym.getType() != Symbol::Constant)
				{
					sprintf(errormsg, 
						"symbol %s not a constant.\n", 
						(yyvsp[0].cstring));
					yyerror(errormsg);
					YYERROR;
				}
			}
			else
			{
				/* insert new constant symbol entry */
				symbols.insert(Symbol(ombt::String((yyvsp[0].cstring)), 
					Symbol::Constant));
			}

			/* create a constant semantic record */
			(yyval.psemantic) = new Semantic(Argument::Constant, ombt::String((yyvsp[0].cstring)));
			MustBeTrue((yyval.psemantic) != NULL);
		}
	}
#line 1912 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 575 "pred.y" /* yacc.c:1646  */
    {
		/* get number of arguments */
		int nargs;
		ombt::ListIterator<Semantic * > argsIter(*(yyvsp[-1].parguments));
		for (nargs = 0; !argsIter.done(); argsIter++, nargs++);

		/* check if symbol exists */
		Symbol newsym(ombt::String((yyvsp[-3].cstring)), Symbol::Function, nargs);
		if (symbols.retrieve(newsym) == OK)
		{
			/* symbol was found, type should not change */
			if (newsym.getType() != Symbol::Function)
			{
				sprintf(errormsg, "type changed for %s.\n", (yyvsp[-3].cstring));
				yyerror(errormsg);
				YYERROR;
			}
			if (newsym.getArgs() != nargs)
			{
				char buffer[BUFSIZ];
				sprintf(buffer, "number of args changed for %s.\n", (yyvsp[-3].cstring));
				strcpy(errormsg, buffer);
				sprintf(buffer, "old number of args = %d.\n", newsym.getArgs());
				strcat(errormsg, buffer);
				sprintf(buffer, "new number of args = %d.\n", nargs);
				strcat(errormsg, buffer);
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into table */
			symbols.insert(newsym);
		}

		/* create a function record */
		(yyval.psemantic) = new Semantic(Argument::Function, ombt::String((yyvsp[-3].cstring)), (yyvsp[-1].parguments), nargs);
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 1957 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 623 "pred.y" /* yacc.c:1646  */
    {
		/* negate conclusion */
		(yyval.psemantic) = new Semantic(Expression::Negation, NULL, (yyvsp[0].psemantic), 1);
		MustBeTrue((yyval.psemantic) != NULL);

		/* save expression */
		ptrees.insertAtEnd((yyval.psemantic));
	}
#line 1970 "y.tab.c" /* yacc.c:1646  */
    break;


#line 1974 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 640 "pred.y" /* yacc.c:1906  */


// report syntax errors
void
yyerror(char *s)
{
	syntaxErrors++;
	fprintf(stdout, " <<==\nlineno %04d: %s\n", yylineno, s);
}

// usage message
void
usage(const char *cmd)
{
	std::cout << "usage: " << cmd << " [-?d:p:] [-o outfile] input_file" << std::endl;
}

// print program
void
dumpPtrees()
{
	// check if any program to print
	if (ptrees.isEmpty())
		return;

	// iterate over all parsetrees, and print them.
	int progno = 1;
	ombt::ListIterator<Semantic * > ptreesIter(ptrees);
	for (int lnno = 1; !ptreesIter.done(); ptreesIter++, lnno++)
	{
		if (lnno == 1)
			std::cout << std::endl << "PARSE TREE: " << progno++ << std::endl;
		std::cout << "Line " << lnno << ": ";
		if (ptreesIter()->getConclusion())
		{
			std::cout << "conclusion: ";
			lnno = 0;
		}
		else
			std::cout << "assumption: ";
		std::cout << *ptreesIter() << std::endl;
	}
	return;
}

void
dumpPrograms()
{
	// check if any program to print
	if (programs.isEmpty())
		return;
 
	// iterate over all programs, and print them.
	ombt::ListIterator<ombt::List<Semantic *> > programsIter(programs);
	for (int progno = 1; !programsIter.done(); programsIter++, progno++)
	{
		// iterate over all clauses in program
		std::cout << std::endl << "PROGRAM: " << progno << std::endl;
		ombt::ListIterator<Semantic *> programIter(programsIter());
		for (int lnno = 1; !programIter.done(); programIter++, lnno++)
		{
			std::cout << "Clause " << lnno << ": ";
			std::cout << *programIter() << std::endl;
		}
	}
	return;
}

void
dumpIRClausesPrograms()
{
	// check if any program to print
	if (clausesPrograms.isEmpty())
		return;

	// iterate over all programs, and print them.
	ombt::ListIterator<ombt::List<IRClause> > cprogsIter(clausesPrograms);
	for (int progno = 1; !cprogsIter.done(); cprogsIter++, progno++)
	{
		// iterate over all clauses in program
		std::cout << std::endl << "CLAUSE PROGRAM: " << progno << std::endl;
		ombt::ListIterator<IRClause> clauseIter(cprogsIter());
		for (int lnno = 1; !clauseIter.done(); clauseIter++, lnno++)
		{
			std::cout << "Clause " << lnno << ": " << clauseIter();
			if (clauseIter().getPartOfConclusion())
				std::cout << "(part of conclusion)";
			std::cout << std::endl;
		}
	}
	return;
}

void
dumpIRClausesPrograms(const char *ofile, std::ofstream &outfile, int &emptyFile)
{
	// check if any program to print
	if (clausesPrograms.isEmpty())
		return;

	// iterate over all programs, and print them.
	emptyFile = 0;
	outfile << "file " << ofile << ":" << std::endl;
	ombt::ListIterator<ombt::List<IRClause> > cprogsIter(clausesPrograms);
	for (int progno = 1; !cprogsIter.done(); cprogsIter++, progno++)
	{
		// iterate over all clauses in program
		outfile << "program " << progno << ":" << std::endl;
		ombt::ListIterator<IRClause> clauseIter(cprogsIter());
		for (int lnno = 1; !clauseIter.done(); clauseIter++, lnno++)
		{
			if (clauseIter().getPartOfConclusion())
				outfile << "conclusion " << lnno << ": " << clauseIter();
			else
				outfile << "assumption " << lnno << ": " << clauseIter();
			outfile << std::endl;
		}
	}
	return;
}

// convert statements to conjunctive normal form
int
convert2cnf()
{
	// list for holding individual programs
	ombt::List<Semantic *> program;
	ombt::List<IRClause> clausesProgram;

	// loop over all expressions and convert
	ombt::ListIterator<Semantic * > ptreesIter(ptrees);
	for ( ; !ptreesIter.done(); ptreesIter++)
	{
		// initial tree
		IfDebug(2, "starting with ...", *ptreesIter());

		// convert biconditionals to implications
		if (maxPhase < PHASE1) continue;
		if (ptreesIter()->removeBiconditionals() != OK)
			return(NOTOK);
		IfDebug(2, "removeBiconditionals (after) ...", *ptreesIter());

		// convert implications to ORs and NOTs
		if (maxPhase < PHASE2) continue;
		if (ptreesIter()->removeConditionals() != OK)
			return(NOTOK);
		IfDebug(2, "removeConditionals (after) ...", *ptreesIter());

		// apply demorgan's laws to push negation down
		if (maxPhase < PHASE3) continue;
		if (ptreesIter()->demorgans() != OK)
			return(NOTOK);
		IfDebug(2, "demorgans (after) ...", *ptreesIter());

		// rename all variables to unique names
		if (maxPhase < PHASE4) continue;
		if (ptreesIter()->renameVariables() != OK)
			return(NOTOK);
		IfDebug(2, "renameVariables (after) ...", *ptreesIter());

		// remove existential quantifier with skolem functions
		if (maxPhase < PHASE5) continue;
		if (ptreesIter()->skolemize() != OK)
			return(NOTOK);
		IfDebug(2, "skolemize (after) ...", *ptreesIter());

		// remove universal quantifiers
		if (maxPhase < PHASE6) continue;
		if (ptreesIter()->removeUniversals() != OK)
			return(NOTOK);
		IfDebug(2, "removeUniversals (after) ...", *ptreesIter());

		// distribution law for ORs and ANDs.
		if (maxPhase < PHASE7) continue;
		if (ptreesIter()->distribution() != OK)
			return(NOTOK);
		IfDebug(2, "distribution (after) ...", *ptreesIter());

		// get clauses in programs
		if (maxPhase < PHASE8) continue;
		if (ptreesIter()->getClauses(program) != OK)
			return(NOTOK);
		IfDebug(2, "getClauses (after) ...", *ptreesIter());
		IfDebug(2, "getClauses (after) ...", program);

		// check for end of program
		if (ptreesIter()->getConclusion())
		{
			// rename variables in clauses
			ombt::ListIterator<Semantic *> programIter(program);
			for (int ic=1 ; !programIter.done(); ic++,programIter++)
			{
				// rename all variables to unique names
				if (maxPhase < PHASE9) continue;
				IfDebug(2, "renameAgain (before) ...", 
					*programIter());
				if (programIter()->renameAgain() != OK)
					return(NOTOK);
				IfDebug(2, "renameAgain (after) ...", 
					*programIter());

				// convert semantic clauses to a 
				// list representation
				if (maxPhase < PHASE10) continue;
				if (programIter()->getClausesAsLists(
					clausesProgram) != OK)
					return(NOTOK);
				IfDebug(2, "getClausesAsLists (after) ...", 
					*programIter());
				IfDebug(2, "getClausesAsLists (after) ...", 
					clausesProgram);
			}

			// save current program
			programs.insertAtEnd(program);
			clausesPrograms.insertAtEnd(clausesProgram);

			// clear old program
			program.clear();
			clausesProgram.clear();
		}
	}

	// all done 
	return(OK);
}

// delete everything
void
clearPtrees()
{
	// check if any program to print
	if (ptrees.isEmpty())
		return;
 
	// iterate over all parsetrees, and delete them
	ombt::ListIterator<Semantic * > ptreesIter(ptrees);
	for ( ; !ptreesIter.done(); ptreesIter++)
	{
		if (ptreesIter() != NULL)
			delete ptreesIter();
	}
	ptrees.clear();
	return;
}

void
clearPrograms()
{
	// check if any program to print
	if (programs.isEmpty())
		return;
 
	// iterate over all programs, and delete them
	ombt::ListIterator<ombt::List<Semantic * > > programsIter(programs);
	for ( ; !programsIter.done(); programsIter++)
	{
		ombt::ListIterator<Semantic * > programIter(programsIter());
		for ( ; !programIter.done(); programIter++)
		{
			if (programIter() != NULL)
				delete programIter();
		}
	}
	programs.clear();
	return;
}

void
clearIRClausesPrograms()
{
	clausesPrograms.clear();
	return;
}


// main entry point
main(int argc, char **argv)
{
#ifdef YYDEBUG
	extern int yydebug;
	yydebug = 1;
#endif
	extern FILE *yyin;
	char *outfilename = NULL;
	int emptyFile = 1;

	// get command line options
	for (int c = 0; (c = getopt(argc, argv, "?d:p:o:")) != EOF; )
	{
		switch (c)
		{
		case 'd':
			// debug level: 0, 1, 2, 5 (as of now).
			currentDebugLevel = atoi(optarg);
			break;

		case 'p':
			// what conversion phases to run
			maxPhase = atoi(optarg);
			break;

		case 'o':
			// output file for intermediate representations
			outfilename = optarg;
			break;

		case '?':
			usage(argv[0]);
			return(0);

		default:
			fprintf(stdout, "invalid option.\n");
			usage(argv[0]);
			return(2);
		}
	}

	// turn off buffering
	setbuf(stdout, NULL);

	// check if an output file was given. also, do not write out
	// a file if a full conversion is NOT done.
	//
	std::ofstream outfile;
	if (outfilename != NULL && maxPhase >= MAXPHASE)
	{
		outfile.open(outfilename);
		if (!outfile)
		{
			fprintf(stdout, "unable to open %s.\n", optarg);
			return(2);
		}
	}

	// scan files
	for (int arg = optind; arg < argc; arg++)
	{
		// tell user which file is scanned
		fprintf(stdout, "\nscanning file %s ...\n", argv[arg]);

		// open file to read
		if ((yyin = fopen(argv[arg], "r")) == NULL)
		{
			fprintf(stdout, "unable to read %s.\n", argv[arg]);
			continue;
		}

		// check syntax of program
		if (yyparse() != OK || syntaxErrors > 0)
		{
			fprintf(stdout, "yyparse failed. [errors=%d]\n",
				syntaxErrors);
		}
		else
		{
			// tell user that program passed
			fprintf(stdout, "yyparse passed.\n");

			// print out parse tree
			IfDebugCall(1, dumpPtrees());

			// convert to conjunctive normal form
			if (convert2cnf() != OK)
			{
				fprintf(stdout, "convert2cnf failed.\n");
			}
			else
			{
				fprintf(stdout, "convert2cnf passed.\n");
				IfDebugCall(1, dumpPtrees());
				IfDebugCall(1, dumpPrograms());
				if (maxPhase >= MAXPHASE)
					dumpIRClausesPrograms();
				if (outfilename != NULL)
					dumpIRClausesPrograms(argv[arg], outfile, emptyFile);
			}
		}

		// clear parser data
		clearPtrees();
		clearPrograms();
		clearIRClausesPrograms();
		scope.clear();
		symbols.clear();
		syntaxErrors = 0;

		// close file.
		fclose(yyin);
		yyin = NULL;
	}

	// close output file, if any.
	if (outfilename != NULL)
		outfile.close();

	// check if anything was written
	if (emptyFile)
		unlink(outfilename);

	// all done
	return(0);
}

