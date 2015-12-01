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
#line 1 "pr.y" /* yacc.c:339  */

// required headers
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <iostream.h>
#include <fstream.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

// other headers
#include "pr.h"

// externs
extern int yylineno;
extern int optind;
extern char *optarg;

// symbol table
BinaryTree_AVL<Symbol> symbols;

// program data
List<Symbol> scope;
List<Semantic * > ptrees;
List<Semantic * > program;
List<List<String> > clauses;

// other globals
char errormsg[BUFSIZ];
int syntaxErrors = 0;
int equality = 0;

// what phases to run
Map<String, int> phases;


#line 104 "y.tab.c" /* yacc.c:339  */

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
    ARGUMENT = 259,
    BICONDITIONAL = 260,
    COMMA = 261,
    FALSE = 262,
    IDENTIFIER = 263,
    EQUAL = 264,
    NOTEQUAL = 265,
    IMPLICATION = 266,
    EXISTENTIAL = 267,
    LEFTEXISTENTIAL = 268,
    LEFTUNIVERSAL = 269,
    UNIVERSAL = 270,
    LPAREN = 271,
    NEGATION = 272,
    NUMBER = 273,
    NEGNUMBER = 274,
    OR = 275,
    PIDENTIFIER = 276,
    QUIT = 277,
    RIGHTEXISTENTIAL = 278,
    RIGHTUNIVERSAL = 279,
    RPAREN = 280,
    SEMICOLON = 281,
    SOS = 282,
    STRING = 283,
    THEREFORE = 284,
    QUERY = 285,
    TRUE = 286,
    LBRACE = 287,
    RBRACE = 288,
    OPTION = 289
  };
#endif
/* Tokens.  */
#define AND 258
#define ARGUMENT 259
#define BICONDITIONAL 260
#define COMMA 261
#define FALSE 262
#define IDENTIFIER 263
#define EQUAL 264
#define NOTEQUAL 265
#define IMPLICATION 266
#define EXISTENTIAL 267
#define LEFTEXISTENTIAL 268
#define LEFTUNIVERSAL 269
#define UNIVERSAL 270
#define LPAREN 271
#define NEGATION 272
#define NUMBER 273
#define NEGNUMBER 274
#define OR 275
#define PIDENTIFIER 276
#define QUIT 277
#define RIGHTEXISTENTIAL 278
#define RIGHTUNIVERSAL 279
#define RPAREN 280
#define SEMICOLON 281
#define SOS 282
#define STRING 283
#define THEREFORE 284
#define QUERY 285
#define TRUE 286
#define LBRACE 287
#define RBRACE 288
#define OPTION 289

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 41 "pr.y" /* yacc.c:355  */

	int flag;
	char cstring[32];
	Semantic *psemantic;
	List<Semantic *> *parguments;

#line 219 "y.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 234 "y.tab.c" /* yacc.c:358  */

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
#define YYLAST   113

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  35
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  64
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  119

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   289

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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   102,   102,   104,   105,   106,   107,   116,   125,   126,
     130,   134,   138,   142,   146,   150,   154,   160,   230,   237,
     258,   259,   263,   271,   283,   286,   293,   300,   304,   313,
     317,   326,   330,   339,   343,   352,   356,   362,   382,   405,
     435,   468,   498,   531,   535,   542,   549,   556,   582,   621,
     642,   670,   679,   687,   694,   698,   702,   709,   715,   724,
     787,   829,   833,   839,   848
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "AND", "ARGUMENT", "BICONDITIONAL",
  "COMMA", "FALSE", "IDENTIFIER", "EQUAL", "NOTEQUAL", "IMPLICATION",
  "EXISTENTIAL", "LEFTEXISTENTIAL", "LEFTUNIVERSAL", "UNIVERSAL", "LPAREN",
  "NEGATION", "NUMBER", "NEGNUMBER", "OR", "PIDENTIFIER", "QUIT",
  "RIGHTEXISTENTIAL", "RIGHTUNIVERSAL", "RPAREN", "SEMICOLON", "SOS",
  "STRING", "THEREFORE", "QUERY", "TRUE", "LBRACE", "RBRACE", "OPTION",
  "$accept", "start", "options", "optionslist", "rbrace", "argument",
  "sosconclusionlist", "expressionlist", "separator_expressionlist",
  "sos_option", "expression", "biconditional", "implication", "or", "and",
  "unary", "universal", "existential", "atom", "predicate", "arglist",
  "arg", "term", "constant", "variable", "function", "separator",
  "conclusiontype", "linesynch", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289
};
# endif

#define YYPACT_NINF -70

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-70)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -70,     3,   -70,    18,   -19,    24,    20,    -6,    30,   -70,
     -70,   -70,   -70,   -70,    56,   -70,    -3,   -70,    33,    22,
       5,    46,    10,    59,   -70,   -70,   -70,   -70,    33,     5,
     -70,    30,   -70,   -70,    53,    54,    72,    73,    66,    71,
      71,   -70,    74,   -70,    71,    71,   -70,   -70,    86,    82,
      75,    91,   -70,    71,    71,   -70,   -70,    14,   -70,   -70,
     -70,    24,    24,    24,    24,    17,   -70,   -70,    71,     4,
      88,    77,    79,    89,    76,   -70,     4,   -70,   -70,    71,
      71,    71,    71,   -70,   -70,     4,     4,   -70,   -70,   -70,
     -70,    24,    24,    24,   -70,     8,   -70,   -70,    80,   -70,
     -70,    81,   -70,    15,    82,    75,    91,   -70,   -70,   -70,
     -70,   -70,   -70,   -70,     4,   -70,   -70,   -70,   -70
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,    24,     6,
      18,    64,     7,    10,     0,     8,     0,    25,     0,    24,
       0,     0,     0,     0,     9,    17,     3,     4,     0,     0,
      61,    24,    20,    45,    59,     0,     0,     0,     0,     0,
       0,    58,    47,    57,     0,     0,    44,    22,    26,    27,
      29,    31,    33,     0,     0,    35,    43,     0,    54,    55,
      56,     0,     0,     0,     0,     0,     5,    21,     0,     0,
       0,     0,     0,     0,     0,    36,     0,    62,    63,     0,
       0,     0,     0,    37,    38,     0,     0,    19,    12,    13,
      11,     0,     0,     0,    23,     0,    51,    53,     0,    41,
      39,     0,    46,     0,    28,    30,    32,    34,    49,    50,
      15,    16,    14,    60,     0,    42,    40,    48,    52
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -70,   -70,   -70,   -70,    28,   -70,    85,   -70,    90,    67,
     -29,   -70,    29,    27,    31,   -34,   -70,   -70,   -70,   -70,
      34,    -1,   -68,   -70,   -70,   -70,   -69,   -70,   -20
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     7,    16,    26,     8,    18,    19,    20,    21,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      95,    96,    57,    58,    59,    60,    31,    61,    12
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      32,    97,    14,     2,     3,    23,    75,     4,    97,    67,
      74,    30,    34,    10,    30,    77,    78,   108,   109,    83,
      84,    30,    41,    85,    86,     5,   114,    15,    62,    63,
      24,    11,    43,   113,   114,    91,    92,     6,    64,    94,
     117,    87,    88,    89,    90,    93,    97,    27,   107,    17,
      11,     9,    13,    33,    34,    25,    66,    17,    35,    36,
      37,    38,    39,    40,    41,    22,    25,    42,    65,    69,
      70,   110,   111,   112,    43,    44,    45,    46,    33,    34,
      71,    72,    73,    35,    36,    37,    38,    39,    40,    41,
      76,    79,    42,    80,    82,    81,    98,   101,    68,    43,
      99,   102,    46,   100,    28,   115,   116,   105,   104,    29,
     103,     0,   106,   118
};

static const yytype_int8 yycheck[] =
{
      20,    69,     8,     0,     1,     8,    40,     4,    76,    29,
      39,     6,     8,    32,     6,    44,    45,    85,    86,    53,
      54,     6,    18,     9,    10,    22,    95,    33,    18,    19,
      33,    26,    28,    25,   103,    18,    19,    34,    28,    68,
      25,    61,    62,    63,    64,    28,   114,    19,    82,    27,
      26,    33,    32,     7,     8,    33,    28,    27,    12,    13,
      14,    15,    16,    17,    18,     9,    33,    21,     9,    16,
      16,    91,    92,    93,    28,    29,    30,    31,     7,     8,
       8,     8,    16,    12,    13,    14,    15,    16,    17,    18,
      16,     5,    21,    11,     3,    20,     8,     8,    31,    28,
      23,    25,    31,    24,    19,    25,    25,    80,    79,    19,
      76,    -1,    81,   114
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    36,     0,     1,     4,    22,    34,    37,    40,    33,
      32,    26,    63,    32,     8,    33,    38,    27,    41,    42,
      43,    44,     9,     8,    33,    33,    39,    39,    41,    43,
       6,    61,    63,     7,     8,    12,    13,    14,    15,    16,
      17,    18,    21,    28,    29,    30,    31,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    57,    58,    59,
      60,    62,    18,    19,    28,     9,    39,    63,    44,    16,
      16,     8,     8,    16,    45,    50,    16,    45,    45,     5,
      11,    20,     3,    50,    50,     9,    10,    63,    63,    63,
      63,    18,    19,    28,    45,    55,    56,    57,     8,    23,
      24,     8,    25,    55,    47,    48,    49,    50,    57,    57,
      63,    63,    63,    25,    61,    25,    25,    25,    56
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    35,    36,    36,    36,    36,    36,    36,    36,    36,
      37,    38,    38,    38,    38,    38,    38,    39,    40,    41,
      42,    42,    43,    43,    44,    44,    45,    46,    46,    47,
      47,    48,    48,    49,    49,    50,    50,    50,    50,    51,
      51,    52,    52,    53,    53,    53,    53,    54,    54,    54,
      54,    55,    55,    56,    57,    57,    57,    58,    58,    59,
      60,    61,    62,    62,    63
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     4,     4,     5,     3,     3,     3,     4,
       2,     4,     4,     4,     5,     5,     5,     1,     2,     3,
       2,     3,     2,     4,     0,     1,     1,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     2,     2,     2,     3,
       4,     3,     4,     1,     1,     1,     3,     1,     4,     3,
       3,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       4,     1,     2,     2,     1
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
        case 6:
#line 108 "pr.y" /* yacc.c:1646  */
    {
		cout << endl << "Syntax error detected ..." << endl;
		scope.clear();
		symbols.clear();
		ptrees.clear();
		variableNames.clear();
		yyerrok;
	}
#line 1399 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 117 "pr.y" /* yacc.c:1646  */
    {
		cout << endl << "Quitting ..." << endl;
		scope.clear();
		symbols.clear();
		ptrees.clear();
		variableNames.clear();
		YYACCEPT;
	}
#line 1412 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 135 "pr.y" /* yacc.c:1646  */
    {
		options[String((yyvsp[-3].cstring))] = String((yyvsp[-1].cstring));
	}
#line 1420 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 139 "pr.y" /* yacc.c:1646  */
    {
		options[String((yyvsp[-3].cstring))] = String((yyvsp[-1].cstring));
	}
#line 1428 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 143 "pr.y" /* yacc.c:1646  */
    {
		options[String((yyvsp[-3].cstring))] = String((yyvsp[-1].cstring));
	}
#line 1436 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 147 "pr.y" /* yacc.c:1646  */
    {
		options[String((yyvsp[-3].cstring))] = String((yyvsp[-1].cstring));
	}
#line 1444 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 151 "pr.y" /* yacc.c:1646  */
    {
		options[String((yyvsp[-3].cstring))] = String((yyvsp[-1].cstring));
	}
#line 1452 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 155 "pr.y" /* yacc.c:1646  */
    {
		options[String((yyvsp[-3].cstring))] = String((yyvsp[-1].cstring));
	}
#line 1460 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 161 "pr.y" /* yacc.c:1646  */
    {
		/* syntax has been accepted */
		cout << endl << "Argument syntax accepted ..." << endl;

		// dump parse tree
		if (verbose)
		{
			cout << endl << "Dumping parse tree ..." << endl;
			dumpTree();
		}

		// check if we need to add any extra axioms
		if (addextraaxioms() != OK)
		{
			ERROR("unable to add extra axioms.", EINVAL);
			scope.clear();
			symbols.clear();
			ptrees.clear();
			variableNames.clear();
			YYABORT;
		}

		// convert to conjunctive normal form
		if (convert2cnf() != OK)
		{
			ERROR("conversion to CNF failed.", EINVAL);
			scope.clear();
			symbols.clear();
			ptrees.clear();
			variableNames.clear();
			YYABORT;
		}

		// run prover on the problem
		switch (runprover())
		{
		case OK:
			break;
		case NOTPROVEN:
			cout << "NOT PROVEN PROGRAM." << endl;
			break;
		case VALID:
			cout << "VALID PROGRAM." << endl;
			break;
		case NOTOK:
			ERROR("PROVER FAILED", EINVAL);
			scope.clear();
			symbols.clear();
			ptrees.clear();
			variableNames.clear();
			YYABORT;
		default:
			ERROR("UNEXPECTED RETURN.", EINVAL);
			scope.clear();
			symbols.clear();
			ptrees.clear();
			variableNames.clear();
			YYABORT;
		}

		// clear out semantic structures
		scope.clear();
		symbols.clear();
		ptrees.clear();
		variableNames.clear();
	}
#line 1531 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 231 "pr.y" /* yacc.c:1646  */
    {
		equality = 0;
	}
#line 1539 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 238 "pr.y" /* yacc.c:1646  */
    {
		/* negate conclusion for refutation proof */
		Semantic *pnegs = new Semantic(Expression::Negation, NULL, (yyvsp[-1].psemantic));
		MustBeTrue(pnegs != NULL);
		
		/* set of support, conclusion and query */
		pnegs->setSOS((yyvsp[-2].flag));
		pnegs->setConclusion((yyvsp[-1].psemantic)->getConclusion());
		pnegs->setQuery((yyvsp[-1].psemantic)->getQuery());

		// clear expression flags
		(yyvsp[-1].psemantic)->setConclusion(0);
		(yyvsp[-1].psemantic)->setQuery(0);

		/* save expression */
		MustBeTrue(ptrees.insertAtEnd(pnegs) == OK);
	}
#line 1561 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 264 "pr.y" /* yacc.c:1646  */
    {
		/* is this a set of support */
		(yyvsp[0].psemantic)->setSOS((yyvsp[-1].flag));

		/* save expression */
		MustBeTrue(ptrees.insertAtEnd((yyvsp[0].psemantic)) == OK);
	}
#line 1573 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 272 "pr.y" /* yacc.c:1646  */
    {
		/* is this a set of support */
		(yyvsp[0].psemantic)->setSOS((yyvsp[-1].flag));

		/* save expression */
		MustBeTrue(ptrees.insertAtEnd((yyvsp[0].psemantic)) == OK);
	}
#line 1585 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 283 "pr.y" /* yacc.c:1646  */
    {
		(yyval.flag) = 0;
	}
#line 1593 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 287 "pr.y" /* yacc.c:1646  */
    {
		(yyval.flag) = 1;
	}
#line 1601 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 294 "pr.y" /* yacc.c:1646  */
    {
		(yyval.psemantic) = (yyvsp[0].psemantic);
	}
#line 1609 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 301 "pr.y" /* yacc.c:1646  */
    {
		(yyval.psemantic) = (yyvsp[0].psemantic);
	}
#line 1617 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 305 "pr.y" /* yacc.c:1646  */
    {
		/* create a biconditional record */
		(yyval.psemantic) = new Semantic(Expression::Biconditional, (yyvsp[-2].psemantic), (yyvsp[0].psemantic));
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 1627 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 314 "pr.y" /* yacc.c:1646  */
    {
		(yyval.psemantic) = (yyvsp[0].psemantic);
	}
#line 1635 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 318 "pr.y" /* yacc.c:1646  */
    {
		/* create a conditional record */
		(yyval.psemantic) = new Semantic(Expression::Conditional, (yyvsp[-2].psemantic), (yyvsp[0].psemantic));
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 1645 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 327 "pr.y" /* yacc.c:1646  */
    {
		(yyval.psemantic) = (yyvsp[0].psemantic);
	}
#line 1653 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 331 "pr.y" /* yacc.c:1646  */
    {
		/* create an OR record */
		(yyval.psemantic) = new Semantic(Expression::Or, (yyvsp[-2].psemantic), (yyvsp[0].psemantic));
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 1663 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 340 "pr.y" /* yacc.c:1646  */
    {
		(yyval.psemantic) = (yyvsp[0].psemantic);
	}
#line 1671 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 344 "pr.y" /* yacc.c:1646  */
    {
		/* create an AND record */
		(yyval.psemantic) = new Semantic(Expression::And, (yyvsp[-2].psemantic), (yyvsp[0].psemantic));
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 1681 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 353 "pr.y" /* yacc.c:1646  */
    {
		(yyval.psemantic) = (yyvsp[0].psemantic);
	}
#line 1689 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 357 "pr.y" /* yacc.c:1646  */
    {
		/* create an NEGATION record */
		(yyval.psemantic) = new Semantic(Expression::Negation, NULL, (yyvsp[0].psemantic));
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 1699 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 363 "pr.y" /* yacc.c:1646  */
    {
		/* create a UNIVERSAL record */
		(yyval.psemantic) = new Semantic(Expression::Universal, String((yyvsp[-1].cstring)), (yyvsp[0].psemantic));
		MustBeTrue((yyval.psemantic) != NULL);

		/* end scope */
		Symbol uvarsym;
		MustBeTrue(scope.removeAtFront(uvarsym) == OK);

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
#line 1723 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 383 "pr.y" /* yacc.c:1646  */
    {
		/* create an EXISTENTIAL record */
		(yyval.psemantic) = new Semantic(Expression::Existential, String((yyvsp[-1].cstring)), (yyvsp[0].psemantic));
		MustBeTrue((yyval.psemantic) != NULL);

		/* end scope */
		Symbol uvarsym;
		MustBeTrue(scope.removeAtFront(uvarsym) == OK);

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
#line 1747 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 406 "pr.y" /* yacc.c:1646  */
    {
		/* return string name */
		strcpy((yyval.cstring), (yyvsp[-1].cstring));

		/* check if symbol exists in global symbol table */
		Symbol newqvsym(String((yyvsp[-1].cstring)), Symbol::UniversalVariable);
		if (symbols.retrieve(newqvsym) == OK)
		{
			/* symbol was found */
			if (newqvsym.getType() != Symbol::Variable)
			{
				/* type of symbol changed */
				sprintf(errormsg, 
					"type changed for %s.\n", (yyvsp[-1].cstring));
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into symbol table */
			MustBeTrue(symbols.insert(Symbol(String((yyvsp[-1].cstring)), 
				Symbol::Variable)) == OK);
		}

		/* insert name into scope */
		MustBeTrue(scope.insertAtFront(Symbol(String((yyvsp[-1].cstring)), 
			Symbol::UniversalVariable)) == OK);
	}
#line 1781 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 436 "pr.y" /* yacc.c:1646  */
    {
		/* return string name */
		strcpy((yyval.cstring), (yyvsp[-1].cstring));

		/* check if symbol exists in global symbol table */
		Symbol newqvsym(String((yyvsp[-1].cstring)), Symbol::UniversalVariable);
		if (symbols.retrieve(newqvsym) == OK)
		{
			/* symbol was found */
			if (newqvsym.getType() != Symbol::Variable)
			{
				/* type of symbol changed */
				sprintf(errormsg, 
					"type changed for %s.\n", (yyvsp[-1].cstring));
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into symbol table */
			MustBeTrue(symbols.insert(Symbol(String((yyvsp[-1].cstring)), 
				Symbol::Variable)) == OK);
		}

		/* insert name into scope */
		MustBeTrue(scope.insertAtFront(Symbol(String((yyvsp[-1].cstring)), 
			Symbol::UniversalVariable)) == OK);
	}
#line 1815 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 469 "pr.y" /* yacc.c:1646  */
    {
		/* return string name */
		strcpy((yyval.cstring), (yyvsp[-1].cstring));

		/* check if symbol exists in global symbol table */
		Symbol newqvsym(String((yyvsp[-1].cstring)), Symbol::ExistentialVariable);
		if (symbols.retrieve(newqvsym) == OK)
		{
			/* symbol was found */
			if (newqvsym.getType() != Symbol::Variable)
			{
				/* type of symbol changed */
				sprintf(errormsg, 
					"type changed for %s.\n", (yyvsp[-1].cstring));
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into symbol table */
			MustBeTrue(symbols.insert(Symbol(String((yyvsp[-1].cstring)), 
				Symbol::Variable)) == OK);
		}

		/* insert name into scope */
		MustBeTrue(scope.insertAtFront(Symbol(String((yyvsp[-1].cstring)),
			Symbol::ExistentialVariable)) == OK);
	}
#line 1849 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 499 "pr.y" /* yacc.c:1646  */
    {
		/* return string name */
		strcpy((yyval.cstring), (yyvsp[-1].cstring));

		/* check if symbol exists in global symbol table */
		Symbol newqvsym(String((yyvsp[-1].cstring)), Symbol::ExistentialVariable);
		if (symbols.retrieve(newqvsym) == OK)
		{
			/* symbol was found */
			if (newqvsym.getType() != Symbol::Variable)
			{
				/* type of symbol changed */
				sprintf(errormsg, 
					"type changed for %s.\n", (yyvsp[-1].cstring));
				yyerror(errormsg);
				YYERROR;
			}
		}
		else
		{
			/* new symbol, insert into symbol table */
			MustBeTrue(symbols.insert(Symbol(String((yyvsp[-1].cstring)), 
				Symbol::Variable)) == OK);
		}

		/* insert name into scope */
		MustBeTrue(scope.insertAtFront(Symbol(String((yyvsp[-1].cstring)),
			Symbol::ExistentialVariable)) == OK);
	}
#line 1883 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 532 "pr.y" /* yacc.c:1646  */
    {
		(yyval.psemantic) = (yyvsp[0].psemantic);
	}
#line 1891 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 536 "pr.y" /* yacc.c:1646  */
    {
		/* create a predicate logical record */
		(yyval.psemantic) = new Semantic(Predicate::LogicalConstant, 
				String(""), String("True"));
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 1902 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 543 "pr.y" /* yacc.c:1646  */
    {
		/* create a logical constant record */
		(yyval.psemantic) = new Semantic(Predicate::LogicalConstant, 
				String(""), String("False"));
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 1913 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 550 "pr.y" /* yacc.c:1646  */
    {
		(yyval.psemantic) = (yyvsp[-1].psemantic);
	}
#line 1921 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 557 "pr.y" /* yacc.c:1646  */
    {
		/* check if symbol exists */
		Symbol newsym(String((yyvsp[0].cstring)), Symbol::PredicateConstant);
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
			MustBeTrue(symbols.insert(newsym) == OK);
		}

		/* create a predicate constant record */
		(yyval.psemantic) = new Semantic(Predicate::Constant, String((yyvsp[0].cstring)), String(""));
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 1951 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 583 "pr.y" /* yacc.c:1646  */
    {
		/* count the number of arguments */
		int nargs = (yyvsp[-1].parguments)->getCount();

		/* check if symbol exists */
		Symbol newsym(String((yyvsp[-3].cstring)), Symbol::PredicateFunction, nargs);
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
			MustBeTrue(symbols.insert(newsym) == OK);
		}

		/* create a predicate record */
		(yyval.psemantic) = new Semantic(Predicate::Function, String((yyvsp[-3].cstring)), (yyvsp[-1].parguments), nargs);
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 1994 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 622 "pr.y" /* yacc.c:1646  */
    {
		/* turn on equal flag */
		equality = 1;

		/* create a list */
		List<Semantic *> *pargs = new List<Semantic *>;
		MustBeTrue(pargs != NULL);

		/* insert LHS and RHS into list */
		MustBeTrue(pargs->insertAtEnd((yyvsp[-2].psemantic)) == OK);
		MustBeTrue(pargs->insertAtEnd((yyvsp[0].psemantic)) == OK);

		/* TWO arguments */
		int nargs = pargs->getCount();
		MustBeTrue(nargs == 2);

		/* create a new semantic record */
		(yyval.psemantic) = new Semantic(Predicate::Equal, String((yyvsp[-1].cstring)), pargs, nargs);
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 2019 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 643 "pr.y" /* yacc.c:1646  */
    {
		/* turn on equal flag */
		equality = 1;

		/* create a list */
		List<Semantic *> *pargs = new List<Semantic *>;
		MustBeTrue(pargs != NULL);

		/* insert LHS and RHS into list */
		MustBeTrue(pargs->insertAtEnd((yyvsp[-2].psemantic)) == OK);
		MustBeTrue(pargs->insertAtEnd((yyvsp[0].psemantic)) == OK);

		/* TWO arguments */
		int nargs = pargs->getCount();
		MustBeTrue(nargs == 2);

		/* create a new semantic record */
		(yyval.psemantic) = new Semantic(Predicate::Equal, String("="), pargs, nargs);
		MustBeTrue((yyval.psemantic) != NULL);

		/* create a NEGATION record */
		(yyval.psemantic) = new Semantic(Expression::Negation, NULL, (yyval.psemantic));
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 2048 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 671 "pr.y" /* yacc.c:1646  */
    {
		/* create an argument list */
		(yyval.parguments) = new List<Semantic *>;
		MustBeTrue((yyval.parguments) != NULL);

		/* insert an argument in the list */
		MustBeTrue((yyval.parguments)->insertAtEnd((yyvsp[0].psemantic)) == OK);
	}
#line 2061 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 680 "pr.y" /* yacc.c:1646  */
    {
		/* insert an argument in the list */
		MustBeTrue((yyval.parguments)->insertAtEnd((yyvsp[0].psemantic)) == OK);
	}
#line 2070 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 688 "pr.y" /* yacc.c:1646  */
    {
		(yyval.psemantic) = (yyvsp[0].psemantic);
	}
#line 2078 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 695 "pr.y" /* yacc.c:1646  */
    {
		(yyval.psemantic) = (yyvsp[0].psemantic);
	}
#line 2086 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 699 "pr.y" /* yacc.c:1646  */
    {
		(yyval.psemantic) = (yyvsp[0].psemantic);
	}
#line 2094 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 703 "pr.y" /* yacc.c:1646  */
    {
		(yyval.psemantic) = (yyvsp[0].psemantic);
	}
#line 2102 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 710 "pr.y" /* yacc.c:1646  */
    {
		/* create a string record */
		(yyval.psemantic) = new Semantic(Term::QuotedString, String((yyvsp[0].cstring)));
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 2112 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 716 "pr.y" /* yacc.c:1646  */
    {
		/* create a number record */
		(yyval.psemantic) = new Semantic(Term::Number, String((yyvsp[0].cstring)));
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 2122 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 725 "pr.y" /* yacc.c:1646  */
    {
		/* search if name is in the scope */
		Symbol varsym(String((yyvsp[0].cstring)));
		if (scope.retrieve(varsym) == OK)
		{
			/* increment usage */
			varsym++;
			scope.update(varsym);

			/* we have a variable, verify type did not change */
			Symbol sym(String((yyvsp[0].cstring)));
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
			(yyval.psemantic) = new Semantic(Term::Variable, String((yyvsp[0].cstring)));
			MustBeTrue((yyval.psemantic) != NULL);
		}
		else
		{
			/* we have a constant, verify type did not change */
			Symbol sym(String((yyvsp[0].cstring)));
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
				MustBeTrue(symbols.insert(Symbol(String((yyvsp[0].cstring)), 
					Symbol::Constant)) == OK);
			}

			/* create a constant semantic record */
			(yyval.psemantic) = new Semantic(Term::Constant, String((yyvsp[0].cstring)));
			MustBeTrue((yyval.psemantic) != NULL);
		}
	}
#line 2186 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 788 "pr.y" /* yacc.c:1646  */
    {
		/* get number of arguments */
		int nargs = (yyvsp[-1].parguments)->getCount();

		/* check if symbol exists */
		Symbol newsym(String((yyvsp[-3].cstring)), Symbol::Function, nargs);
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
			MustBeTrue(symbols.insert(newsym) == OK);
		}

		/* create a function record */
		(yyval.psemantic) = new Semantic(Term::Function, String((yyvsp[-3].cstring)), (yyvsp[-1].parguments), nargs);
		MustBeTrue((yyval.psemantic) != NULL);
	}
#line 2229 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 834 "pr.y" /* yacc.c:1646  */
    {
		(yyvsp[0].psemantic)->setConclusion(1);
		(yyvsp[0].psemantic)->setQuery(0);
		(yyval.psemantic) = (yyvsp[0].psemantic);
	}
#line 2239 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 840 "pr.y" /* yacc.c:1646  */
    {
		(yyvsp[0].psemantic)->setConclusion(0);
		(yyvsp[0].psemantic)->setQuery(1);
		(yyval.psemantic) = (yyvsp[0].psemantic);
	}
#line 2249 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2253 "y.tab.c" /* yacc.c:1646  */
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
#line 850 "pr.y" /* yacc.c:1906  */


// report syntax errors
void
yyerror(const char *s)
{
	syntaxErrors++;
	fprintf(stdout, " <<==\nlineno %04d: %s\n", yylineno, s);
}

// clear program structures
void
clearProgram()
{
	if (!program.isEmpty())
	{
		ListIterator<Semantic * > programIter(program);
		for ( ; !programIter.done(); programIter++)
		{
			if (programIter() != NULL)
				delete programIter();
		}
		program.clear();
	}
	return;
}

void
clearClauses()
{
	clauses.clear();
	return;
}

// print program data structures
void
dumpTree()
{
	cout << endl << "START OF DUMP TREE ..." << endl;
	ListIterator<Semantic * > ptreesIter(ptrees);
	for ( ; !ptreesIter.done(); ptreesIter++)
	{
		cout << *ptreesIter() << ";" << endl;
	}
	cout << endl << "END OF DUMP TREE ..." << endl;
	return;
}

void
dumpProgram()
{
	ListIterator<Semantic * > programIter(program);
	for ( ; !programIter.done(); programIter++)
	{
		cout << *programIter() << ";" << endl;
	}
	return;
}

void
dumpClauses()
{
	ListIterator<List<String> > clausesIter(clauses);
	for ( ; !clausesIter.done(); clausesIter++)
	{
		ListIterator<String> clauseIter(clausesIter());
		for (int first=1; !clauseIter.done(); first=0, clauseIter++)
		{
			if (!first) cout << "|| ";
			cout << clauseIter();
		}
		cout << endl;
	}
	return;
}

// convert statements to conjunctive normal form
int
convert2cnf()
{
	if (!phases[EXPANDFILE] || !phases[PARSEFILE] || 
	    !phases[ADDEXTRAAXIOMS] || !phases[CONVERT2CNF])
	{
		cout << endl;
		cout << "Skipping conversion to CNF ..." << endl;
		return(OK);
	}
	// clear program structure
	clearProgram();
	clearClauses();

	// loop over all expressions and convert
	ListIterator<Semantic * > ptreesIter(ptrees);
	for ( ; !ptreesIter.done(); ptreesIter++)
	{
		// convert biconditionals to implications
		if (ptreesIter()->removeBiconditionals() != OK)
		{
			ERROR("removeBiconditionals failed.", EINVAL);
			return(NOTOK);
		}

		// convert implications to ORs and NOTs
		if (ptreesIter()->removeConditionals() != OK)
		{
			ERROR("removeConditionals failed.", EINVAL);
			return(NOTOK);
		}

		// apply demorgan's laws to push negation down
		if (ptreesIter()->demorgans() != OK)
		{
			ERROR("demorgans failed.", EINVAL);
			return(NOTOK);
		}

		// rename all variables to unique names
		if (ptreesIter()->renameVariables() != OK)
		{
			ERROR("renameVariables failed.", EINVAL);
			return(NOTOK);
		}

		// remove existential quantifier with skolem functions
		if (ptreesIter()->skolemize() != OK)
		{
			ERROR("skolemize failed.", EINVAL);
			return(NOTOK);
		}

		// remove universal quantifiers
		if (ptreesIter()->removeUniversals() != OK)
		{
			ERROR("removeUniversals failed.", EINVAL);
			return(NOTOK);
		}

		// distribution law for ORs and ANDs.
		if (ptreesIter()->distribution() != OK)
		{
			ERROR("distribution failed.", EINVAL);
			return(NOTOK);
		}

		// get clauses in programs
		if (ptreesIter()->getClauses(program) != OK)
		{
			ERROR("getClauses failed.", EINVAL);
			return(NOTOK);
		}
	}

	// rename variables in clauses
	ListIterator<Semantic *> programIter(program);
	for ( ; !programIter.done(); programIter++)
	{
		// rename all variables to unique names
		if (programIter()->renameAgain() != OK)
		{
			ERROR("renameAgain failed.", EINVAL);
			return(NOTOK);
		}

		// get clauses as lists
		if (programIter()->getClausesAsLists(clauses) != OK)
		{
			ERROR("getClausesAsList failed.", EINVAL);
			return(NOTOK);
		}
	}
	// dump data
	if (verbose)
	{
		cout << endl;
		cout << "Dumping program ..." << endl;
		dumpProgram();
		cout << endl;
		cout << "Dumping program clauses ..." << endl;
		dumpClauses();
	}

	// all done 
	return(OK);
}

