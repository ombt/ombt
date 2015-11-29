
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "prop.y"

// required headers
#include <stdio.h>
#include <string.h>
#include <limits.h>

// other headers
#include "prop.h"

// externs
extern int yylineno;
extern int optind;
extern char *optarg;

// globals 
int allflag = 1;
int supportsetflag = 1;
int tautologyflag = 1;
int subsumeflag = 1;
int chasteflag = 1;
int dbflag = 1;
int maxdepth = 0;
int maxbreadth = INT_MAX;
int nextclause = 0;
int verbose = 0;

// constants
const int MaxDepth = 100;

// pointer to program tree
List<Semantic * > ptrees;
List<List<Semantic * > > programs;
List<Set_List<Clause> > clausesPrograms;
BinaryTree_AVL<Clause> clausesDB;



/* Line 189 of yacc.c  */
#line 111 "y.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NEWLINE = 258,
     COMMA = 259,
     SEMICOLON = 260,
     THEREFORE = 261,
     BICONDITIONAL = 262,
     IMPLICATION = 263,
     OR = 264,
     AND = 265,
     NEGATION = 266,
     LPAREN = 267,
     RPAREN = 268,
     IDENTIFIER = 269,
     QUIT = 270,
     TRUE = 271,
     FALSE = 272
   };
#endif
/* Tokens.  */
#define NEWLINE 258
#define COMMA 259
#define SEMICOLON 260
#define THEREFORE 261
#define BICONDITIONAL 262
#define IMPLICATION 263
#define OR 264
#define AND 265
#define NEGATION 266
#define LPAREN 267
#define RPAREN 268
#define IDENTIFIER 269
#define QUIT 270
#define TRUE 271
#define FALSE 272




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 40 "prop.y"

	char string[32];
	Semantic *precord;



/* Line 214 of yacc.c  */
#line 188 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 200 "y.tab.c"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   48

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  18
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  15
/* YYNRULES -- Number of rules.  */
#define YYNRULES  31
/* YYNRULES -- Number of states.  */
#define YYNSTATES  49

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   272

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
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
      15,    16,    17
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     4,     8,    13,    16,    20,    24,    28,
      31,    35,    37,    41,    43,    45,    49,    51,    55,    57,
      61,    63,    67,    69,    72,    74,    76,    78,    82,    84,
      87,    90
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      19,     0,    -1,    -1,    19,    30,    31,    -1,    19,    20,
      30,    31,    -1,    19,    32,    -1,    19,     1,    32,    -1,
      19,    15,    31,    -1,    19,    15,    32,    -1,    21,    31,
      -1,    20,    21,    31,    -1,    22,    -1,    21,    29,    22,
      -1,    23,    -1,    24,    -1,    23,     7,    24,    -1,    25,
      -1,    24,     8,    25,    -1,    26,    -1,    25,     9,    26,
      -1,    27,    -1,    26,    10,    27,    -1,    28,    -1,    11,
      27,    -1,    14,    -1,    16,    -1,    17,    -1,    12,    22,
      13,    -1,     4,    -1,     6,    22,    -1,     5,    32,    -1,
       3,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    72,    72,    74,    78,    82,    83,    88,    93,   101,
     102,   106,   110,   117,   124,   128,   136,   140,   147,   151,
     159,   163,   171,   175,   183,   188,   193,   198,   205,   209,
     219,   223
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NEWLINE", "COMMA", "SEMICOLON",
  "THEREFORE", "BICONDITIONAL", "IMPLICATION", "OR", "AND", "NEGATION",
  "LPAREN", "RPAREN", "IDENTIFIER", "QUIT", "TRUE", "FALSE", "$accept",
  "start", "expressionlist", "separator_expressionlist", "expression",
  "biconditional", "implication", "or", "and", "unary", "primary",
  "separator", "conclusion", "linesynch", "newline", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    18,    19,    19,    19,    19,    19,    19,    19,    20,
      20,    21,    21,    22,    23,    23,    24,    24,    25,    25,
      26,    26,    27,    27,    28,    28,    28,    28,    29,    30,
      31,    32
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     3,     4,     2,     3,     3,     3,     2,
       3,     1,     3,     1,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     2,     1,     1,     1,     3,     1,     2,
       2,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,    31,     0,     0,     0,    24,     0,
      25,    26,     0,     0,    11,    13,    14,    16,    18,    20,
      22,     0,     5,     6,    29,    23,     0,     0,     7,     8,
       0,     0,    28,     0,     9,     0,     0,     0,     0,     3,
      27,    30,    10,     4,    12,    15,    17,    19,    21
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    33,    21,    28,    22
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -10
static const yytype_int8 yypact[] =
{
     -10,    24,   -10,    10,   -10,    31,    31,    31,   -10,     2,
     -10,   -10,     3,     6,   -10,     9,    -2,    14,     8,   -10,
     -10,    23,   -10,   -10,   -10,   -10,    18,    10,   -10,   -10,
       6,    23,   -10,    31,   -10,    31,    31,    31,    31,   -10,
     -10,   -10,   -10,   -10,   -10,    -2,    14,     8,   -10
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -10,   -10,   -10,    21,    -4,   -10,    11,     1,    -3,    -6,
     -10,   -10,    32,    -9,    -1
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      25,    24,    23,    26,    34,     4,    36,    27,    29,     5,
      32,    27,    39,     4,     6,     7,    35,     8,    38,    10,
      11,    42,    43,    37,     2,     3,    41,     4,    27,    44,
       5,    40,    48,    30,    47,     6,     7,    46,     8,     9,
      10,    11,     6,     7,    31,     8,    45,    10,    11
};

static const yytype_uint8 yycheck[] =
{
       6,     5,     3,     7,    13,     3,     8,     5,     9,     6,
       4,     5,    21,     3,    11,    12,     7,    14,    10,    16,
      17,    30,    31,     9,     0,     1,    27,     3,     5,    33,
       6,    13,    38,    12,    37,    11,    12,    36,    14,    15,
      16,    17,    11,    12,    12,    14,    35,    16,    17
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    19,     0,     1,     3,     6,    11,    12,    14,    15,
      16,    17,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    30,    32,    32,    22,    27,    22,     5,    31,    32,
      21,    30,     4,    29,    31,     7,     8,     9,    10,    31,
      13,    32,    31,    31,    22,    24,    25,    26,    27
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
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
  int yytoken;
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

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

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
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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
  *++yyvsp = yylval;

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
     `$$ = $1'.

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

/* Line 1455 of yacc.c  */
#line 75 "prop.y"
    {
		fprintf(stdout, "argument syntax accepted ...\n");
	}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 79 "prop.y"
    {
		fprintf(stdout, "argument syntax accepted ...\n");
	}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 84 "prop.y"
    {
		fprintf(stdout, "syntax error detected ...\n");
		yyerrok;
	}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 89 "prop.y"
    {
		fprintf(stdout, "quitting ...\n");
		YYACCEPT;
	}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 94 "prop.y"
    {
		fprintf(stdout, "quitting ...\n");
		YYACCEPT;
	}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 107 "prop.y"
    {
		ptrees.insertAtEnd((yyvsp[(1) - (1)].precord));
	}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 111 "prop.y"
    {
		ptrees.insertAtEnd((yyvsp[(3) - (3)].precord));
	}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 118 "prop.y"
    {
		(yyval.precord) = (yyvsp[(1) - (1)].precord);
	}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 125 "prop.y"
    {
		(yyval.precord) = (yyvsp[(1) - (1)].precord);
	}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 129 "prop.y"
    {
		(yyval.precord) = new Semantic(Expression::Biconditional, (yyvsp[(1) - (3)].precord), (yyvsp[(3) - (3)].precord));
		MustBeTrue((yyval.precord) != NULL);
	}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 137 "prop.y"
    {
		(yyval.precord) = (yyvsp[(1) - (1)].precord);
	}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 141 "prop.y"
    {
		(yyval.precord) = new Semantic(Expression::Conditional, (yyvsp[(1) - (3)].precord), (yyvsp[(3) - (3)].precord));
		MustBeTrue((yyval.precord) != NULL);
	}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 148 "prop.y"
    {
		(yyval.precord) = (yyvsp[(1) - (1)].precord);
	}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 152 "prop.y"
    {
		(yyval.precord) = new Semantic(Expression::Or, (yyvsp[(1) - (3)].precord), (yyvsp[(3) - (3)].precord));
		MustBeTrue((yyval.precord) != NULL);
	}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 160 "prop.y"
    {
		(yyval.precord) = (yyvsp[(1) - (1)].precord);
	}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 164 "prop.y"
    {
		(yyval.precord) = new Semantic(Expression::And, (yyvsp[(1) - (3)].precord), (yyvsp[(3) - (3)].precord));
		MustBeTrue((yyval.precord) != NULL);
	}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 172 "prop.y"
    {
		(yyval.precord) = (yyvsp[(1) - (1)].precord);
	}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 176 "prop.y"
    {
		(yyval.precord) = new Semantic(Expression::Negation, NULL, (yyvsp[(2) - (2)].precord));
		MustBeTrue((yyval.precord) != NULL);
	}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 184 "prop.y"
    {
		(yyval.precord) = new Semantic(Atomic::Variable, (yyvsp[(1) - (1)].string), "");
		MustBeTrue((yyval.precord) != NULL);
	}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 189 "prop.y"
    {
		(yyval.precord) = new Semantic(Atomic::Constant, (yyvsp[(1) - (1)].string), "true");
		MustBeTrue((yyval.precord) != NULL);
	}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 194 "prop.y"
    {
		(yyval.precord) = new Semantic(Atomic::Constant, (yyvsp[(1) - (1)].string), "false");
		MustBeTrue((yyval.precord) != NULL);
	}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 199 "prop.y"
    {
		(yyval.precord) = (yyvsp[(2) - (3)].precord);
	}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 210 "prop.y"
    {
		// negate conclusion for resolution
		(yyval.precord) = new Semantic(Expression::Negation, NULL, (yyvsp[(2) - (2)].precord), 1);
		MustBeTrue((yyval.precord) != NULL);
		ptrees.insertAtEnd((yyval.precord));
	}
    break;



/* Line 1455 of yacc.c  */
#line 1648 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
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

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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

  *++yyvsp = yylval;


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

#if !defined(yyoverflow) || YYERROR_VERBOSE
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
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 225 "prop.y"


// report syntax errors
void
yyerror(char *s)
{
	fprintf(stdout, " <<==\nlineno %04d: %s\n", yylineno, s);
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
	ListIterator<Semantic * > ptreesIter(ptrees);
	for (int lnno = 1; !ptreesIter.done(); ptreesIter++, lnno++)
	{
		if (lnno == 1)
			cout << endl << "PARSE TREE: " << progno++ << endl;
		cout << "Line " << lnno << ": ";
		if (ptreesIter()->getConclusion())
		{
			cout << "conclusion: ";
			lnno = 0;
		}
		else
			cout << "assumption: ";
		cout << *ptreesIter() << endl;
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
	ListIterator<List<Semantic *> > programsIter(programs);
	for (int progno = 1; !programsIter.done(); programsIter++, progno++)
	{
		// iterate over all clauses in program
		cout << endl << "PROGRAM: " << progno << endl;
		ListIterator<Semantic *> programIter(programsIter());
		for (int lnno = 1; !programIter.done(); programIter++, lnno++)
		{
			cout << "Clause " << lnno << ": ";
			cout << *programIter() << endl;
		}
	}
	return;
}

void
dumpClausesPrograms()
{
	// check if any program to print
	if (clausesPrograms.isEmpty())
		return;

	// iterate over all programs, and print them.
	ListIterator<Set_List<Clause> > cprogsIter(clausesPrograms);
	for (int progno = 1; !cprogsIter.done(); cprogsIter++, progno++)
	{
		// iterate over all clauses in program
		cout << endl << "CLAUSE PROGRAM: " << progno << endl;
		Set_List_Iterator<Clause> clauseIter(cprogsIter());
		for (int lnno = 1; !clauseIter.done(); clauseIter++, lnno++)
		{
			cout << "Clause " << lnno << ": ";
			ClauseIterator atomIter(clauseIter());
			for (int first = 1; !atomIter.done(); atomIter++)
			{
				if (first)
					cout << atomIter();
				else
					cout << " || " << atomIter();
				first = 0;
			}
			cout << endl;
		}
	}
	return;
}

// remove tautologies from clauses programs
void
removeTautologies(Set_List<Clause> &clausesProgram)
{
	Clause clause;
	Set_List<Clause> newClausesProgram;
	Set_List_Iterator<Clause> clprogIter(clausesProgram);
	for ( ; !clprogIter.done(); clprogIter++)
	{
		clause = clprogIter();
		if (clause.isPartOfConclusion())
		{
			newClausesProgram.insert(clause);
		}
		else if (!tautologyflag || !clause.isATautology())
		{
			newClausesProgram.insert(clause);
		}
		else if (verbose)
		{
			cout << "TAUTOLOGY, removing " << clause << endl;
		}
	}
	clausesProgram = newClausesProgram;
}

// remove subsumed clauses from clauses programs
void
removeSubsumedClauses(Set_List<Clause> &clausesProgram)
{
	Set_List<Clause> deletedClauses;
	Set_List_Iterator<Clause> clprogIter(clausesProgram);
	Set_List_Iterator<Clause> clprogIter2(clausesProgram);
	for( ; !clprogIter.done(); clprogIter++)
	{
		for (clprogIter2.reset(); !clprogIter2.done(); clprogIter2++)
		{
			if (clprogIter().subsumes(clprogIter2()))
			{
				deletedClauses.insert(clprogIter2());
			}
		}
	}
	Set_List_Iterator<Clause> deletedIter(deletedClauses);
	for( ; !deletedIter.done(); deletedIter++)
	{
		clausesProgram.remove(deletedIter());
		if (verbose)
		{
			cout << "SUBSUMED, removing " << deletedIter() << endl;
		}
	}
}

// remove chaste clauses from clauses programs
void
removeChasteClauses(Set_List<Clause> &clausesProgram)
{
	// generate list of all atoms in program
	Clause allAtoms;
	Set_List_Iterator<Clause> clprogIter(clausesProgram);
	for ( ; !clprogIter.done(); clprogIter++)
	{
		allAtoms |= clprogIter();
	}

	// now make sure every atom in program has a complement
	Clause clause;
	Set_List<Clause> newClausesProgram;
	for (clprogIter.reset() ; !clprogIter.done(); clprogIter++)
	{
		// include ALL clauses from conclusions
		clause = clprogIter();
		if (clause.isPartOfConclusion())
		{
			newClausesProgram.insert(clause);
			continue;
		}

		// check for atoms without a complement 
		ClauseIterator clauseIter(clause);
		for (int includeit = 1 ; !clauseIter.done(); clauseIter++)
		{
			Atom negatedAtom = ~clauseIter();
			if (!allAtoms.isInClause(negatedAtom))
			{
				includeit = 0;
				break;
			}
				
		}
		if (includeit)
		{
			newClausesProgram.insert(clause);
		}
		else if (verbose)
		{
			cout << "CHASTE, removing " << clause << endl;
		}
	}
	clausesProgram = newClausesProgram;
	return;
}

// convert program to conjunctive normal form
int
convert2cnf()
{
	// list for holding clauses
	List<Semantic *> program;
	Set_List<Clause> clausesProgram;
	Clause clause;

	// loop over all expressions and convert
	ListIterator<Semantic * > ptreesIter(ptrees);
	for ( ; !ptreesIter.done(); ptreesIter++)
	{
		// convert biconditionals to implications
		if (ptreesIter()->removeBiconditionals() != OK)
			return(NOTOK);

		// convert implications to ORs and NOTs
		if (ptreesIter()->removeConditionals() != OK)
			return(NOTOK);

		// apply demorgan's laws to push negation down
		if (ptreesIter()->demorgans() != OK)
			return(NOTOK);

		// distribution law for ORs and ANDs.
		if (ptreesIter()->distribution() != OK)
			return(NOTOK);

		// get clauses in programs
		if (ptreesIter()->getClauses(program) != OK)
			return(NOTOK);
		if (ptreesIter()->getClauses(clausesProgram, clause) != OK)
			return(NOTOK);

		// check for end of program
		if (ptreesIter()->getConclusion())
		{
			// save current program
			programs.insertAtEnd(program);
			if (tautologyflag)
				removeTautologies(clausesProgram);
			if (chasteflag)
				removeChasteClauses(clausesProgram);
			if (subsumeflag)
				removeSubsumedClauses(clausesProgram);
			clausesPrograms.insertAtEnd(clausesProgram);

			// clear old program
			program.clear();
			clausesProgram.clear();
		}
	}

	// all done 
	return(OK);
}

// clean up program data
void
clearPrograms()
{
	// check if any program to print
	if (ptrees.isEmpty())
		return;

	// iterate over all parsetrees, and delete them
	ListIterator<Semantic * > ptreesIter(ptrees);
	for ( ; !ptreesIter.done(); ptreesIter++)
	{
		if (ptreesIter() != NULL)
			delete ptreesIter();
	}
	ptrees.clear();
	return;
}

// print atoms in clause
void
dumpClause(char *searchType, int currentDepth, int icl1, int icl2, 
		int nextClause, Clause &clause)
{
	cout << "st(" << searchType << "), ";
	cout << "cl(" << nextClause << "), ";
	cout << "con(" << clause.isPartOfConclusion() << "), ";
	cout << "res(" << icl1 << "," << icl2 << "), ";
	cout << "dep(" << currentDepth << ") = ";
	ClauseIterator atomIter(clause);
	for (int first = 1; !atomIter.done(); atomIter++)
	{
		if (first)
			cout << atomIter();
		else
			cout << " || " << atomIter();
		first = 0;
	}
	cout << endl;
}

// resolve two clauses if possible
int
resolveClauses(Array<BinaryTree_AVL<Clause> > &clausesArray, 
	Clause &clause1, Clause &clause2, int &clausesAdded, int currentDepth)
{
	// scan one clause, atom by atom, and search the other clause
	// for the negation of the atom.
	//
	Atom cl1atom;
	ClauseIterator cl1Iter(clause1);
	for ( ; !cl1Iter.done(); cl1Iter++)
	{
		// negated the current atom
		cl1atom = cl1Iter();
		Atom negatedAtom = ~cl1atom;

		// check if negated atom exists in clause2
		if (clause2.isInClause(negatedAtom))
		{
			// it exists !!! we can resolve these
			// two clauses together. resolve them
			// and check if the null string results.
			//
			// resolve clause1 and clause2
			//
			Clause tmpcl1 = clause1;
			Clause tmpcl2 = clause2;
			tmpcl1.remove(cl1atom);
			tmpcl2.remove(negatedAtom);
			Clause newcl = tmpcl1 | tmpcl2;

			// set conclusion bit if either clause is
			// part of a conclusion
			//
			if (tmpcl1.isPartOfConclusion() ||
			    tmpcl2.isPartOfConclusion())
			{
				newcl.setPartOfConclusion(1);
			}

			// set clause depth and number
			newcl.setDepth(currentDepth+1);
			newcl.setNumber(nextclause);

			// add new clause to clause array,
			// but first check if empty.
			//
			if (newcl.isEmpty())
			{
				// we have generated a contradiction.
				dumpClause("BREADTH", currentDepth+1, 
					clause1.getNumber(),
					clause2.getNumber(),
					nextclause, newcl);
				return(VALID);
			}

			// a new clause was generated. check if it 
			// already exists.
			//
			if ((!dbflag || !clausesDB.isInTree(newcl)) && 
			   ((!tautologyflag) || !newcl.isATautology()))
			{
				// insert into data base
				clausesDB.insert(newcl);

				// update flag to indicate that a new
				// clause was resolved. added it to 
				// clauses array.
				//
				clausesAdded = 1;
				if ((currentDepth+1) >= MaxDepth)
				{
					ERROR("(currentDepth+1) >= MaxDepth", 
						NOTOK);
					return(NOTOK);
				}
				clausesArray[currentDepth+1].insert(newcl);

				// print new clause
				dumpClause("BREADTH", currentDepth+1, 
					clause1.getNumber(),
					clause2.getNumber(),
					nextclause, newcl);
				nextclause++;
				break;
			}
		}
	}

	// all done
	return(OK);
}

// run a depth-first search using fewest-literals strategy at a depth
int 
linearSearch(Array<BinaryTree_AVL<Clause> > &clausesArray, int currentDepth)
{
#if 0
	// first, sort clauses by length
	List<LClause> lset;
	for (int idep = 0; idep <= currentDepth; idep++)
	{
		BinaryTree_AVL_Iterator_InOrder<Clause>
			clIter(clausesArray[idep]);
		for ( ; !clIter.done(); clIter++)
		{
			lset.insertByValue(LClause(clIter()));
		}
	}

	// get conclusions for initial set
	BinaryTree_AVL_Iterator_InOrder<Clause> initIter(clausesArray[0]);
	for( ; !initIter.done(); initIter++)
	{
		// skip if clause is not part of conclusion
		if (!initIter().isPartOfConclusion())
			continue;

		// get clause from conclusion
		Clause clause1(initIter();

		// try to resolve by length
		List_Iterator<LClause> lsetIter(lset);
		for ( ; !lsetIter.done(); lsetIter++)
		{
			// get clause to resolve
			Clause clause2(lsetIter());
		}
	}
#endif

#if 0
	// get start and end of clauses for the current depth
	int startClause = start[currentDepth];
	int endClause = end[currentDepth];

	// use the conclusions as the starting point for each linear search.
	for (int icl1 = startClause; icl1 <= endClause; icl1++)
	{
		// get starting clause for linear resolution
		Clause clause1(clausesArray[icl1]);
		int isaveicl1 = icl1;

		// skip clauses that are not descendant from the conclusion
		if (supportsetflag && (!clause1.isPartOfConclusion()))
			continue;

		// limit the number of cycles in the linear search
		for (int idepth = 1; idepth <= maxdepth; idepth++)
		{
			// loop over existing clauses
			int saveNextClause = nextClause;
			for (int icl2 = startClause; icl2 < saveNextClause; icl2++)
			{
				// current clause to check
				Clause clause2(clausesArray[icl2]);

				// check if clauses are resolvable
				ClauseIterator cl1Iter(clause1);
				for ( ; !cl1Iter.done(); cl1Iter++)
				{
					// negate the current atom
					Atom cl1Atom = cl1Iter();
					Atom negatedCl1Atom = ~cl1Atom;

					// does negated atom exist in clause2
					if (clause2.isInClause(negatedCl1Atom))
					{
						// resolve clause1 and clause2
						Clause tmpcl1 = clause1;
						Clause tmpcl2 = clause2;
						tmpcl1.remove(cl1Atom);
						tmpcl2.remove(negatedCl1Atom);
						Clause newcl = tmpcl1 | tmpcl2;

						// add new clause to clause array,
						// but first check if empty.
						if (newcl.isEmpty())
						{
							// we have generated a 
							// contradiction.
							dumpClause("DEPTH", 
								++currentDepth, 
								isaveicl1, icl2, 
								nextClause, newcl);
							return(VALID);
						}

						// a new clause was generated. 
						if (nextClause >= MaxClauses)
						{
							ERROR("nextClause >= MaxClauses", NOTOK);
							return(NOTOK);
						}
						clausesArray[nextClause] = newcl;

						// print new clause
						dumpClause("DEPTH", 
							++currentDepth, 
							isaveicl1, icl2, 
							nextClause, newcl);

						// point to next clause
						clause1 = newcl;
						isaveicl1 = nextClause++;
						break;
					}
				}
			}
		}
	}
#endif
	return(OK);
}

// resolve all clauses at the current depth
int
resolveDepth(Array<BinaryTree_AVL<Clause> > &clausesArray, 
	int &clausesAdded, int currentDepth)
{
	// scan over clauses and resolve, when possible
	BinaryTree_AVL_Iterator_InOrder<Clause>
		cdIter(clausesArray[currentDepth]);
	for ( ; !cdIter.done(); cdIter++)
	{
		// resolve with clause at lower or current level;
		for (int isd = 0; isd <= currentDepth; isd++)
		{
			BinaryTree_AVL_Iterator_InOrder<Clause>
				sdIter(clausesArray[isd]);
			for ( ; !sdIter.done(); sdIter++)
			{
				// only resolve clauses where at least
				// one of them is part of conclusion.
				// this is the set of support approach.
				//
				if (supportsetflag &&
				   (!cdIter().isPartOfConclusion() &&
				    !sdIter().isPartOfConclusion()))
					continue;

				// resolve the clauses
				int status = resolveClauses(clausesArray,
					cdIter(), sdIter(),
					clausesAdded, currentDepth);

				// check return value
				switch (status)
				{
				case OK:
				case CONTINUE:
					break;
				case NOTOK:
					ERROR("resolveClauses failed.", status);
					return(NOTOK);
				case VALID:
					cout << "Valid program." << endl;
					return(VALID);
				case INVALID:
					cout << "Invalid program." << endl;
					return(INVALID);
				default:
					MustBeTrue(0);
					return(NOTOK);
				}
			}
		}
	}
	return(OK);
}

// remove subsumed clauses at current depth
void
removeSubsumedClauses(
	Array<BinaryTree_AVL<Clause> > &clausesArray, int currentDepth)
{
	Set_List<Clause> deletedClauses;
	for (int idep = 0; idep <= currentDepth; idep++)
	{
		BinaryTree_AVL_Iterator_InOrder<Clause>
			clIter(clausesArray[idep]);
		for ( ; !clIter.done(); clIter++)
		{
			for (int idep2 = 0; idep2 <= currentDepth; idep2++)
			{
				BinaryTree_AVL_Iterator_InOrder<Clause>
					clIter2(clausesArray[idep2]);
				for ( ; !clIter2.done(); clIter2++)
				{
					if (clIter().subsumes(clIter2()))
					{
						deletedClauses.insert(clIter2());
					}
				}
			}
		}
	}
	Set_List_Iterator<Clause> deletedIter(deletedClauses);
	for( ; !deletedIter.done(); deletedIter++)
	{
		for (int idep = 0; idep <= currentDepth; idep++)
		{
			clausesArray[idep].remove(deletedIter());
			if (verbose)
			{
				cout << "SUBSUMED, removing " << deletedIter() << endl;
			}
		}
	}
}

// resolve programs, for now, use a breadth-first approach.
int
resolveProgram(const Set_List<Clause> &clauses)
{
	// create an array of trees to track clauses at each depth
	int currentDepth = 0;
	Array<BinaryTree_AVL<Clause> > clausesArray(MaxDepth);

	// copy over initial set of clauses
	Set_List_Iterator<Clause> clausesIter(clauses);
	for (nextclause = 0; !clausesIter.done(); nextclause++, clausesIter++)
	{
		// copy clause to clauses tree
		Clause clause(clausesIter());
		clause.setDepth(currentDepth);
		clause.setNumber(nextclause);
		clausesArray[currentDepth].insert(clause);

		// save in database for later checks for duplicates
		clausesDB.insert(clause);

		// print clause data
		dumpClause("BREADTH", currentDepth, -1, -1, nextclause, clause);
	}

	// loop until all resolutions are done
	for (int clausesAdded = 1; 
	    (currentDepth < maxbreadth && currentDepth < MaxDepth) && 
	     clausesAdded; currentDepth++)
	{
		// turn off flag. it will be reset if any new clauses
		// were added by the function resolveClauses().
		//
		clausesAdded = 0;

		// check if we should do a linear-search at each depth
		if (maxdepth > 0)
		{
			// perform linear search
			int status = 
				linearSearch(clausesArray, currentDepth);

			// check return value
			switch (status)
			{
			case OK:
			case CONTINUE:
				break;
			case NOTOK:
				ERROR("linearSearch failed.", status);
				return(NOTOK);
			case VALID:
				cout << "Valid program." << endl;
				return(OK);
			case INVALID:
				cout << "Invalid program." << endl;
				return(OK);
			default:
				MustBeTrue(0);
				return(NOTOK);
			}
		}

		// resolve all clauses for the current depth.
		int status = 
			resolveDepth(clausesArray, clausesAdded, currentDepth);

		// check return value
		switch (status)
		{
		case OK:
		case CONTINUE:
			break;
		case NOTOK:
			ERROR("resolveDepth failed.", status);
			return(NOTOK);
		case VALID:
			cout << "Valid program." << endl;
			return(OK);
		case INVALID:
			cout << "Invalid program." << endl;
			return(OK);
		default:
			MustBeTrue(0);
			return(NOTOK);
		}

		// remove subsumed clauses
		if (subsumeflag && clausesAdded)
			removeSubsumedClauses(clausesArray, currentDepth+1);
	}

	// invalid programs terminate without producing the empty clause
	cout << "Invalid program." << endl;

	// all done
	return(OK);
}

void
resolvePrograms()
{
	// check if any programs to resolve
	if (clausesPrograms.isEmpty())
	{
		cout << "No programs to resolve. Returning." << endl;
		return;
	}

	// make sure database is all clear
	clausesDB.clear();

	// iterate over all programs, and print them.
	ListIterator<Set_List<Clause> > cprogsIter(clausesPrograms);
	for (int progno = 1; !cprogsIter.done(); cprogsIter++, progno++)
	{
		cout << endl << "RESOLVING CLAUSE PROGRAM: " << progno << endl;
		if (resolveProgram(cprogsIter()) == OK)
			cout << "PROGRAM " << progno << " SUCCEEDED." << endl;
		else
			cout << "PROGRAM " << progno << " FAILED." << endl;
		clausesDB.clear();
	}
	return;
}


// usage message
void
usage(const char *cmd)
{
	cout << endl;
	cout << "usage: " << cmd << " [-?V] [-A [on|off]]" << endl;
	cout << "	[-C[on|off]] [-D[on|off] " << endl;
	cout << "	[-S[on|off]] [-T[on|off]] " << endl;
	cout << "	[-d maxlevel] [-b maxlevel] " << endl;
	cout << "	input_file ..." << endl;
	cout << endl;
	cout << "-V = turn verbose on," << endl;
	cout << "-A = turn ALL filtering on/off," << endl;
	cout << "-C = turn chaste clause filtering on/off," << endl;
	cout << "-D = turn database filtering on/off," << endl;
	cout << "-S = turn set-of-support filtering on/off," << endl;
	cout << "-T = turn tautology filtering on/off," << endl;
	cout << "-U = turn subsumption filtering on/off," << endl;
	cout << "-d = depth-first levels to search (not implemented)," << endl;
	cout << "-b = breadth-first levels to search," << endl;
	cout << endl;
	cout << "input file contains the clauses to prove." << endl;
	cout << "by default, the flags -A,-C,-D,-S,-T,-U are all on." << endl;
}

// main entry point
main(int argc, char **argv)
{
#ifdef YYDEBUG
	extern int yydebug;
	yydebug = 1;
#endif
	extern FILE *yyin;

	// get command line options
	for (int c = 0; (c = getopt(argc, argv, "?Vb:d:C:D:S:T:U:A:")) != EOF; )
	{
		switch (c)
		{
		case 'V':
			verbose = 1;
			break;

		case 'b':
			maxbreadth = atoi(optarg);
			break;

		case 'd':
			maxdepth = atoi(optarg);
			break;

		case 'A':
			if (String(optarg) == String("on"))
			{
				allflag = 1;
				supportsetflag = 1;
				tautologyflag = 1;
				subsumeflag = 1;
				chasteflag = 1;
				dbflag = 1;
			}
			else if (String(optarg) == String("off"))
			{
				allflag = 0;
				supportsetflag = 0;
				tautologyflag = 0;
				subsumeflag = 0;
				chasteflag = 0;
				dbflag = 0;
			}
			break;

		case 'C':
			if (String(optarg) == String("on"))
				chasteflag = 1;
			else if (String(optarg) == String("off"))
				chasteflag = 0;
			else
				chasteflag = 0;
			break;

		case 'D':
			if (String(optarg) == String("on"))
				dbflag = 1;
			else if (String(optarg) == String("off"))
				dbflag = 0;
			else
				dbflag = 0;
			break;

		case 'S':
			if (String(optarg) == String("on"))
				supportsetflag = 1;
			else if (String(optarg) == String("off"))
				supportsetflag = 0;
			else
				supportsetflag = 0;
			break;

		case 'T':
			if (String(optarg) == String("on"))
				tautologyflag = 1;
			else if (String(optarg) == String("off"))
				tautologyflag = 0;
			else
				tautologyflag = 0;
			break;

		case 'U':
			if (String(optarg) == String("on"))
				subsumeflag = 1;
			else if (String(optarg) == String("off"))
				subsumeflag = 0;
			else
				subsumeflag = 0;
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

	// scan files
	for (int arg = optind; arg < argc; arg++)
	{
		// tell user which file is scanned
		fprintf(stdout, "\nscanning file %s ...\n", 
			argv[arg]);

		// open file to read
		if ((yyin = fopen(argv[arg], "r")) == NULL)
		{
			fprintf(stdout, "unable to read %s.\n", 
				argv[arg]);
			continue;
		}

		// check syntax of program
		if (yyparse() != OK)
		{
			fprintf(stdout, "yyparse failed.\n");
			goto cleanup;
		}
		fprintf(stdout, "yyparse passed.\n");
		dumpPtrees();

		// convert to conjunctive normal form
		if (convert2cnf() != OK)
		{
			fprintf(stdout, "convert2cnf failed.\n");
			goto cleanup;
		}
		fprintf(stdout, "convert2cnf passed.\n");
		dumpPtrees();
		dumpPrograms();
		dumpClausesPrograms();

		// resolve all programs
		resolvePrograms();

		// clean up: delete syntax tree and close file.
cleanup:
		fclose(yyin);
		yyin = NULL;
		clearPrograms();
	}
	return(0);
}


