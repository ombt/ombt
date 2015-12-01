
# line 2 "stab.y"
/* required headers */
#include <stdio.h>
#include <string.h>

/* externs */
void yyerror(const char *);


# line 13 "stab.y"
typedef union
#ifdef __cplusplus
	YYSTYPE
#endif
 {
	char string[32];
} YYSTYPE;
# define HEXADECIMAL 257
# define INTEGER 258
# define NAME 259
# define REAL 260
# define STRING 261

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

# line 596 "stab.y"


void
yyerror(const char *s)
{
	fprintf(stderr, "%s\n", s);
}

main(int argc, char **argv)
{
#ifdef YYDEBUG
	extern int yydebug;
	yydebug = 1;
#endif

	/* call parser */
	yyparse();

	exit(0);
}
static YYCONST yytabelem yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
-1, 316,
	40, 145,
	-2, 152,
-1, 436,
	59, 198,
	118, 156,
	-2, 161,
	};
# define YYNPROD 280
# define YYLAST 732
static YYCONST yytabelem yyact[]={

   304,   312,   567,   496,    21,    21,   343,   475,   383,   374,
   132,   472,   323,    42,    43,    44,    45,    46,    47,    48,
    49,   460,   452,    51,    52,    53,    54,    55,   367,    56,
   240,    57,    58,    59,    60,    61,   397,   431,   321,   341,
   371,   303,   305,   302,   337,    77,   307,   148,    63,   453,
    39,   339,   124,   347,   338,   138,   385,    39,   477,   309,
   368,   380,   346,   366,   363,   277,   382,   139,   204,   137,
   135,   348,   349,     3,    39,   140,   494,    39,   601,   134,
   142,   143,   144,   147,   150,   350,   152,   589,   183,   156,
   585,   158,   159,   424,   128,   313,   191,   499,   192,   193,
   194,   195,    39,   384,   351,   527,    99,   196,   197,   122,
    98,   125,   201,   202,   203,   435,   149,   207,   285,   244,
   224,   356,   157,   151,    76,    75,   127,   598,   578,   126,
   572,   570,   568,   107,   546,    92,   108,   109,   116,   121,
   133,    93,   241,    94,   492,    95,    96,   110,   111,   455,
   115,    97,   398,   342,   100,   442,   409,   120,   101,   394,
   184,   205,   279,   220,   219,   106,    79,    80,    81,    82,
   113,    83,   218,    84,   217,    85,    86,    87,    88,    89,
   114,   200,   112,   117,   206,   118,   119,    90,   199,   198,
    91,   136,    65,   444,   228,   146,   161,   162,   375,   155,
   576,   594,    73,   544,   376,   508,   208,   209,   210,   211,
   377,   502,   560,   375,   187,   375,   577,   188,    74,   376,
   592,   376,   593,   189,   190,   377,   373,   377,   313,   523,
   591,   319,   276,    67,    68,   286,    69,   248,   235,   288,
    70,   283,   318,   474,   272,   273,   344,   562,   249,    71,
    72,   250,   251,   368,   531,   296,   297,   298,   313,   532,
   308,   313,   317,   313,   313,   506,   525,   530,    38,   149,
   129,   214,   270,   291,     2,    38,   274,   311,   314,   315,
   313,   384,   213,   479,   469,   320,   215,    41,   268,   568,
   269,   325,    38,   433,    64,    38,   352,   604,   461,   462,
   353,   463,   602,   581,   582,   464,   580,   354,    62,   588,
   361,    64,   329,   364,   465,   331,   364,   364,   507,   212,
    38,   584,   364,   483,   326,   364,    78,   324,   447,   301,
   228,   563,   310,   557,   390,   391,   293,   333,   482,   555,
   474,   260,   473,   446,   357,   548,   456,   238,   375,   378,
   379,   536,   332,   359,   376,   533,   259,   517,   388,   242,
   377,   399,   237,   511,   495,   488,   402,   485,   480,   328,
   478,   386,   387,   428,   425,   419,   335,   403,   336,   487,
   433,   395,   381,   410,   362,   393,   355,   262,   261,   258,
   257,   253,   252,   246,   245,   239,   236,   231,   227,   396,
   226,   225,   216,   153,   131,   401,   599,   434,   587,   558,
   549,   520,   466,   438,   420,   417,   365,   360,   334,   232,
   230,   445,   229,   141,     4,   282,   553,   439,   554,   441,
   449,   551,   421,   550,   519,   516,   486,   427,   357,   476,
   429,   364,   426,   364,   422,   418,   471,   407,   437,   400,
   392,   358,   340,   330,   440,   327,   290,   271,   263,   256,
   255,   458,   448,   451,   491,   254,   493,   402,   457,   223,
   222,   221,   489,   490,   484,   130,    50,   275,   406,   405,
   500,   605,   600,   503,   596,   595,   583,   564,   556,   535,
   509,   518,   512,   510,   481,   450,   423,   416,   415,   414,
   514,   413,   412,   411,   389,   300,   299,   526,   247,   295,
   515,   522,   521,    39,   294,   292,   364,   513,   289,   534,
   287,   476,   284,   539,   538,   281,   547,   537,   540,   280,
   266,   265,   264,   233,     5,   186,   597,   571,   552,    40,
    17,    18,   569,    34,    24,   529,    35,    33,   566,    29,
   565,   528,   322,    36,    37,    19,    25,    13,   459,    26,
   574,   432,    27,    28,   573,   430,   404,   454,   498,   586,
    14,   497,     6,    22,   590,    30,    31,   579,   575,   559,
   543,   542,    32,   505,   504,    15,   524,    23,   561,    12,
   545,    16,   541,   468,   501,   470,   408,   372,   370,   467,
   436,   369,   316,   603,   163,   164,   165,   166,   167,   168,
   169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
   179,   243,   181,   182,   163,   164,   165,   166,   167,   168,
   169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
   179,   180,   181,   182,   267,   306,   345,   105,   104,   103,
   102,   185,   160,   234,   154,   145,   123,    20,   443,   278,
    11,    10,     9,     8,     7,    66,     1,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    38 };
static YYCONST yytabelem yypact[]={

    15,-10000000,   366,   473,   473,-10000000,   226,-10000000,-10000000,-10000000,
-10000000,-10000000,    37,    37,    37,    37,    37,    37,    37,    37,
   432,-10000000,    37,    37,    37,    37,    37,-10000000,    37,-10000000,
    37,    37,    37,    37,    37,-10000000,-10000000,-10000000,   247,   -66,
-10000000,   135,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
  -134,-10000000,-10000000,-10000000,-10000000,-10000000,  -135,-10000000,-10000000,-10000000,
-10000000,-10000000,    68,    68,    14,   431,   345,  -118,  -118,    37,
   -67,  -191,  -206,  -193,    37,   365,-10000000,-10000000,-10000000,    37,
    37,    37,    10,    37,  -136,    37,   344,    81,    37,  -137,
    37,    37,   527,   -98,   109,    37,-10000000,    37,    37,    37,
    37,-10000000,-10000000,-10000000,-10000000,-10000000,    37,    37,   -69,   -70,
   -77,    37,    37,    37,   -97,   -97,    37,   -98,   -98,   -98,
   -98,   172,-10000000,   343,-10000000,   -84,   -86,   -94,-10000000,-10000000,
   -95,-10000000,-10000000,-10000000,-10000000,   427,-10000000,-10000000,-10000000,   426,
   425,  -139,   342,   341,-10000000,   339,  -143,   364,-10000000,   362,
   338,   361,-10000000,   498,   129,-10000000,   337,   303,-10000000,   336,
-10000000,  -116,   507,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
  -140,-10000000,-10000000,   335,-10000000,   334,   137,-10000000,-10000000,-10000000,
-10000000,   333,-10000000,-10000000,-10000000,-10000000,   332,-10000000,   421,   416,
   415,   331,   330,   297,   329,-10000000,   328,   414,   497,   496,
   495,   173,   413,  -116,  -116,  -140,   212,-10000000,-10000000,-10000000,
   436,  -118,  -195,   -96,-10000000,   494,   490,-10000000,-10000000,  -143,
  -118,   487,  -141,-10000000,    37,-10000000,   485,-10000000,    37,   483,
   412,-10000000,  -116,-10000000,-10000000,   480,-10000000,   -98,-10000000,-10000000,
-10000000,-10000000,   479,   474,    37,    37,    37,   471,   470,-10000000,
   -97,    37,     1,   -97,  -164,  -164,  -164,   176,-10000000,-10000000,
-10000000,   132,  -164,  -164,    14,   230,-10000000,-10000000,   411,-10000000,
   -98,  -116,  -143,   409,  -116,   293,   360,   -98,-10000000,   -98,
  -207,   408,  -105,-10000000,   -12,    37,-10000000,-10000000,-10000000,    37,
   -12,   327,    62,-10000000,   407,-10000000,     1,-10000000,   359,    37,
   325,     5,-10000000,   358,     4,   -31,   108,-10000000,  -116,  -116,
     2,   323,    22,    21,-10000000,    68,    68,  -116,-10000000,-10000000,
   469,-10000000,-10000000,    37,    37,-10000000,-10000000,   406,-10000000,-10000000,
  -207,-10000000,-10000000,-10000000,-10000000,   -99,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,   322,    37,-10000000,-10000000,  -106,-10000000,
    37,   405,     1,-10000000,-10000000,    37,-10000000,   318,   439,   438,
   403,-10000000,  -102,   110,-10000000,   468,   467,   466,   464,   463,
-10000000,-10000000,   462,-10000000,   357,-10000000,-10000000,-10000000,   401,-10000000,
   316,   356,  -105,   400,-10000000,   461,    34,   315,-10000000,   398,
  -106,   314,   396,-10000000,   340,  -144,-10000000,   108,   355,-10000000,
-10000000,-10000000,-10000000,-10000000,  -164,  -164,  -164,  -103,   -64,-10000000,
    37,   284,  -105,   -12,-10000000,   460,  -106,  -212,-10000000,  -109,
   253,-10000000,   162,   200,-10000000,   354,   243,-10000000,    37,    -1,
   311,  -164,   222,-10000000,-10000000,   309,-10000000,   459,   279,-10000000,
-10000000,  -212,   308,-10000000,   392,-10000000,-10000000,-10000000,-10000000,   321,
-10000000,  -118,  -118,    37,  -114,    37,    17,   305,-10000000,  -162,
    93,   146,   225,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,    92,
-10000000,  -156,-10000000,   458,   304,   457,  -116,  -164,   200,-10000000,
-10000000,   391,-10000000,   298,   456,-10000000,-10000000,   390,   353,-10000000,
   146,   110,   117,   188,-10000000,-10000000,    37,  -154,   157,   296,
  -156,   454,-10000000,   292,  -164,-10000000,  -118,   -12,-10000000,  -162,
    37,    88,  -124,-10000000,-10000000,    37,   286,   352,   389,   387,
   383,-10000000,-10000000,-10000000,   280,-10000000,   453,-10000000,   274,-10000000,
-10000000,-10000000,   351,   100,-10000000,   156,-10000000,   272,   452,    37,
  -126,  -127,  -128,-10000000,-10000000,-10000000,-10000000,   -12,  -164,   102,
  -130,-10000000,   204,   451,-10000000,   262,    31,-10000000,   350,-10000000,
-10000000,-10000000,-10000000,-10000000,   250,-10000000,-10000000,-10000000,  -172,   115,
-10000000,-10000000,-10000000,-10000000,   450,   449,-10000000,  -131,-10000000,-10000000,
   348,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,   447,-10000000,  -181,
-10000000,   244,    37,   238,   446,-10000000 };
static YYCONST yytabelem yypgo[]={

     0,   666,   534,   665,   664,   663,   662,   661,   660,    10,
     0,   659,    30,   658,    88,   657,    45,    48,   656,    52,
   655,   654,   653,   652,    39,   651,     6,   650,   649,   648,
   647,   195,    47,   646,    68,    43,    42,    41,    36,   645,
    46,    22,    12,    28,   644,   602,   601,   600,   599,   197,
    38,   598,    40,   597,   596,     7,     9,   595,    11,   594,
   593,   592,   590,   588,   586,   584,   583,   581,     1,   580,
   579,   578,   577,   574,     3,   571,   568,   567,   566,   565,
    37,   561,   558,    21,   552,     8,   196,    44,   551,   548,
   545,   542,   538,   537,     2,   536,   535,   508 };
static YYCONST yytabelem yyr1[]={

     0,     1,     1,     2,     2,     2,     2,     2,     2,     3,
     3,     3,     3,     3,     3,     3,     3,     9,    11,    12,
    14,    13,     4,     4,     5,     5,     5,     5,     5,     5,
     6,     6,     7,     7,     7,     7,     7,     7,     7,     7,
     8,    15,    15,    15,    15,    15,    15,    15,    15,    10,
    10,    10,    10,    10,    10,    17,    18,    18,    19,    19,
    19,    19,    19,    16,    16,    16,    16,    16,    16,    16,
    16,    16,    16,    16,    16,    16,    16,    16,    16,    16,
    16,    16,    16,    16,    16,    16,    16,    16,    16,    16,
    16,    16,    16,    20,    20,    31,    31,    32,    27,    27,
    27,    27,    27,    27,    28,    26,    26,    26,    33,    33,
    33,    33,    33,    29,    29,    29,    29,    29,    34,    35,
    35,    37,    36,    36,    39,    39,    40,    40,    30,    30,
    30,    30,    30,    30,    30,    30,    21,    21,    22,    22,
    45,    45,    44,    44,    44,    46,    46,    51,    51,    52,
    54,    55,    53,    53,    53,    53,    57,    57,    56,    56,
    56,    58,    58,    59,    59,    59,    47,    47,    60,    60,
    60,    60,    60,    61,    67,    69,    69,    70,    70,    71,
    71,    71,    63,    73,    73,    73,    73,    73,    64,    65,
    66,    62,    62,    72,    72,    72,    41,    41,    48,    48,
    74,    74,    75,    75,    76,    42,    42,    68,    43,    78,
    78,    79,    79,    80,    80,    81,    82,    82,    83,    83,
    83,    83,    83,    50,    50,    84,    84,    77,    23,    23,
    23,    23,    49,    86,    86,    86,    86,    86,    86,    86,
    86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
    86,    86,    86,    87,    87,    24,    85,    88,    90,    90,
    90,    92,    92,    92,    93,    91,    89,    89,    94,    95,
    25,    96,    96,    96,    96,    97,    97,    97,    97,    38 };
static YYCONST yytabelem yyr2[]={

     0,     6,     4,     8,     2,     2,     2,     2,     2,     4,
     4,     8,     4,     4,     4,     8,    16,     2,     2,     2,
     2,     2,     4,     4,     4,     4,     4,     4,     4,     4,
     2,    10,     2,     4,     4,     4,     4,     4,     2,     6,
     2,     4,     4,     4,     4,     4,     2,     2,     2,     2,
     6,     6,    10,    14,    14,     6,     8,     2,     4,     4,
     4,     2,     2,     2,    10,    10,     4,     6,    10,    10,
    14,     4,     6,    18,    10,     6,     8,     4,    10,     4,
    10,     6,    10,     2,     4,     4,     4,     4,     2,     2,
     2,     2,     2,     2,     6,     2,     4,    10,    10,     4,
     8,     8,     8,    10,    16,     2,     4,     2,     2,     2,
     2,     2,     2,     6,    14,    10,     8,    14,     2,     2,
     4,    10,     0,     2,     2,     4,    16,    14,    10,    10,
    12,    18,    16,    16,    10,    10,     0,     2,     0,     2,
     0,     2,     2,     2,     2,     0,     2,     2,     6,     8,
     2,     2,     0,     4,     2,     2,     0,     2,     4,     4,
     4,     0,     2,     0,     4,     2,     0,     4,     8,    10,
     6,     4,     4,     8,     6,     0,     2,     0,     6,     0,
     2,     2,    18,     0,     2,     2,     2,     2,     8,     8,
    12,     0,     2,     2,     2,     2,     0,     2,     0,     4,
     2,     6,     6,     4,     2,     2,     4,    18,     8,     4,
    10,     2,     4,     2,     4,     8,     2,     6,     4,     4,
     8,     4,    12,     8,     4,     0,     4,     2,    14,    16,
    20,    22,     4,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,    20,     6,     6,     2,
     2,     2,     2,     0,     2,     2,     2,     4,     8,     2,
     6,     2,     2,     2,     2,     2,     2,     2,     2,     2 };
static YYCONST yytabelem yychk[]={

-10000000,    -1,   259,    58,    58,    -2,    99,    -4,    -5,    -6,
    -7,    -8,   116,    84,    97,   112,   118,    67,    68,    82,
   -15,   -10,   100,   114,    71,    83,    86,    89,    90,    76,
   102,   103,   109,    74,    70,    73,    80,    81,   258,    40,
    -2,    61,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,
    44,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,
   -10,   -10,    61,   -17,    64,   258,    -3,    98,    99,   101,
   105,   114,   115,    67,    83,   259,   259,   -16,   258,    98,
    99,   100,   101,   103,   105,   107,   108,   109,   110,   111,
   119,   122,    67,    73,    75,    77,    78,    83,    42,    38,
    86,    90,   -27,   -28,   -29,   -30,    97,    65,    68,    69,
    79,    80,   114,   102,   112,    82,    70,   115,   117,   118,
    89,    71,   -16,   -18,   -19,    97,   115,   112,    80,   256,
    44,    59,    -9,   258,    -9,   -10,   258,   260,   261,   260,
   -10,    58,   -10,   -10,   -10,   -20,   -31,   -10,   -32,   259,
   -10,   259,   -10,    59,   -21,   118,   -10,   259,   -10,   -10,
   -23,   -86,   -49,    97,    98,    99,   100,   101,   102,   103,
   104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
   114,   115,   116,   -14,   258,   -25,   -96,   105,   108,   114,
   115,   -10,   -10,   -10,   -10,   -10,   -10,   -10,   258,   258,
   258,   -10,   -10,   -10,   -34,   258,   -34,   -10,   -14,   -14,
   -14,   -14,   -49,   110,    99,   114,    59,   258,   258,   258,
   258,    44,    44,    44,   259,    59,    59,    59,   -32,    58,
    58,    59,    58,    35,   -22,   109,    59,    59,    44,    59,
   -12,   258,   -86,   114,   259,    59,    59,   -97,   100,   111,
   114,   115,    59,    59,    44,    44,    44,    59,    59,    59,
    44,    59,    59,    44,    35,    35,    35,   -44,   115,   117,
    99,    44,   -12,   -12,    64,    41,    -9,   260,   -11,   258,
    35,    35,   -31,    -9,    35,   259,   -10,    35,   -10,    35,
    44,   -12,    35,   -14,    35,    35,   -10,   -10,   -10,    35,
    35,   -34,   -35,   -37,   -10,   -36,   -39,   -40,   259,    58,
   -34,   -42,   -68,   259,   -42,   -42,   -45,    86,   110,    99,
   -42,   -50,   -84,   -42,   -19,    61,   -17,    44,   -14,   -12,
    44,   -12,    59,    44,    58,   -14,   -14,   -87,   261,   258,
    44,   -24,   258,   -26,   258,   -33,    74,    65,    83,    84,
    97,   116,   -10,   -10,   -26,    59,    59,   -37,    44,   -40,
    58,   -10,    59,    59,   -68,    58,    59,   -43,    91,   -46,
   -51,   -52,   -53,   118,   -56,   105,   111,   117,   -12,   -12,
    59,    59,    44,   -85,   259,    35,   -16,   -16,   -12,    35,
   -10,   -10,    44,   -87,   258,    59,   -35,   -38,   258,   -10,
    44,   -36,   -10,    59,   -78,    40,    40,    44,   -54,   258,
   -56,    35,    35,    35,    35,    35,    35,    58,    44,    59,
    58,   -24,    44,    35,    59,    59,    44,   -38,    59,    44,
   -79,   -80,   -81,    40,   -68,   259,   -47,   -52,    58,   -42,
   -50,   -42,   258,   -13,   257,   -10,    59,    44,   -24,   -26,
    35,   -38,   -41,   261,   -77,   258,    93,   -80,   -43,   -82,
   -83,    98,    99,   101,   105,   114,    58,   -48,   -60,    41,
   -57,   -56,   -58,    99,    97,   -55,   -10,    59,    59,    61,
    59,    35,    59,    44,   -41,    59,    44,    58,    44,    -9,
    -9,   -10,   258,   -10,    59,    59,   -74,   -75,   -76,   259,
   -56,   -59,   118,   -58,   -65,   -66,    40,    93,   113,   -85,
    35,    59,    35,   -12,   -42,   -83,    44,    59,    35,    44,
    58,   -58,   -56,   112,   -64,    78,   -10,   259,   -88,   -90,
   110,    97,   102,    59,   -85,    35,    59,    -9,   -26,   -74,
   -55,   -61,   -67,   -69,   115,   -62,   258,   -10,    59,    58,
    44,    44,   -92,    43,    45,    59,    35,    59,    58,   -70,
   112,   -63,    91,    59,    35,   -10,   -89,   -94,   258,   -91,
   258,   -93,   258,   -26,   -68,   -71,    98,   114,   258,   -72,
   102,    99,   100,    35,    59,    59,   -94,    58,    59,   259,
   -73,   115,   105,   107,    86,    35,    35,   -95,   258,    58,
    35,   259,    58,   -10,    59,    35 };
static YYCONST yytabelem yydef[]={

     0,    -2,     0,     0,     0,     2,     0,     4,     5,     6,
     7,     8,     0,     0,     0,     0,     0,     0,     0,     0,
    30,    32,     0,     0,     0,     0,     0,    38,     0,    40,
     0,     0,     0,     0,     0,    46,    47,    48,    49,     0,
     1,     0,    22,    23,    24,    25,    26,    27,    28,    29,
     0,    33,    34,    35,    36,    37,     0,    41,    42,    43,
    44,    45,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    39,    50,    63,     0,
     0,     0,     0,     0,     0,     0,     0,   136,     0,     0,
     0,     0,     0,     0,     0,     0,    83,     0,     0,     0,
     0,    88,    89,    90,    91,    92,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    51,     0,    57,     0,     0,     0,    61,    62,
     0,     3,     9,    17,    10,     0,    12,    13,    14,     0,
     0,     0,     0,     0,    66,     0,    93,     0,    95,     0,
     0,     0,    71,     0,   138,   137,     0,     0,    77,     0,
    79,     0,     0,   233,   234,   235,   236,   237,   238,   239,
   240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
   250,   251,   252,     0,    20,     0,     0,   271,   272,   273,
   274,     0,    84,    85,    86,    87,     0,    99,     0,     0,
     0,     0,     0,     0,     0,   118,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    55,    58,    59,    60,
     0,     0,     0,     0,    31,     0,     0,    67,    96,     0,
     0,     0,     0,    72,     0,   139,     0,    75,     0,     0,
     0,    19,     0,   250,   232,     0,    81,     0,   275,   276,
   277,   278,     0,     0,     0,     0,     0,     0,     0,   113,
     0,     0,   122,     0,     0,     0,     0,   140,   142,   143,
   144,     0,     0,   225,     0,    52,    11,    15,     0,    18,
     0,     0,    94,     0,     0,     0,     0,     0,    76,     0,
     0,     0,     0,   270,     0,     0,   100,   101,   102,     0,
     0,     0,     0,   119,     0,   116,   123,   124,     0,     0,
     0,     0,   205,     0,     0,     0,    -2,   141,     0,     0,
     0,     0,     0,     0,    56,     0,     0,     0,    64,    65,
     0,    68,    69,     0,     0,    74,    78,     0,   253,   254,
     0,    80,   255,    82,   105,     0,   107,   108,   109,   110,
   111,   112,    98,   103,     0,     0,   115,   120,     0,   125,
     0,     0,   122,   128,   206,     0,   129,     0,     0,     0,
   146,   147,     0,   154,   155,     0,     0,     0,     0,     0,
   134,   135,     0,   226,     0,   224,    53,    54,     0,    97,
     0,     0,     0,     0,   106,     0,     0,     0,   279,     0,
     0,     0,     0,   130,     0,     0,   166,   152,     0,   150,
   153,   158,   159,   160,     0,   225,     0,     0,     0,    70,
     0,     0,     0,     0,   114,     0,     0,   196,   117,     0,
     0,   211,   213,     0,   209,     0,    -2,   148,     0,     0,
     0,   223,     0,    16,    21,     0,   228,     0,     0,   104,
   121,   196,     0,   197,     0,   227,   208,   212,   214,     0,
   216,     0,     0,     0,     0,     0,     0,     0,   167,     0,
     0,   161,     0,   157,   162,   149,   151,   132,   133,     0,
    73,     0,   229,     0,     0,     0,     0,     0,     0,   218,
   219,     0,   221,     0,     0,   131,   199,   200,     0,   204,
   161,     0,   165,     0,   171,   172,     0,     0,     0,     0,
     0,     0,   127,     0,   215,   217,     0,     0,   210,     0,
   203,   175,   191,   164,   170,     0,     0,     0,     0,     0,
   263,   259,   260,   230,     0,   126,     0,   220,     0,   201,
   202,   168,     0,   177,   176,     0,   192,     0,     0,     0,
     0,     0,     0,   261,   262,   231,   207,     0,     0,   179,
     0,   169,     0,     0,   189,     0,     0,   266,     0,   257,
   265,   258,   264,   222,     0,   174,   180,   181,     0,   183,
   193,   194,   195,   188,     0,     0,   267,     0,   173,   178,
     0,   184,   185,   186,   187,   190,   256,     0,   269,     0,
   268,     0,     0,     0,     0,   182 };
typedef struct
#ifdef __cplusplus
	yytoktype
#endif
{ char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] =
{
	"HEXADECIMAL",	257,
	"INTEGER",	258,
	"NAME",	259,
	"REAL",	260,
	"STRING",	261,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"stabstring : NAME ':' class",
	"stabstring : ':' class",
	"class : 'c' '=' constant ';'",
	"class : namedtype",
	"class : parameter",
	"class : procedure",
	"class : variable",
	"class : label",
	"constant : 'b' ordvalue",
	"constant : 'c' ordvalue",
	"constant : 'e' typeid ',' ordvalue",
	"constant : 'i' INTEGER",
	"constant : 'r' REAL",
	"constant : 's' STRING",
	"constant : 'C' REAL ',' REAL",
	"constant : 'S' typeid ',' numelements ',' numbits ',' bitpattern",
	"ordvalue : INTEGER",
	"numelements : INTEGER",
	"numbits : INTEGER",
	"numbytes : INTEGER",
	"bitpattern : HEXADECIMAL",
	"namedtype : 't' typeid",
	"namedtype : 'T' typeid",
	"parameter : 'a' typeid",
	"parameter : 'p' typeid",
	"parameter : 'v' typeid",
	"parameter : 'C' typeid",
	"parameter : 'D' typeid",
	"parameter : 'R' typeid",
	"procedure : proc",
	"procedure : proc ',' NAME ':' NAME",
	"variable : typeid",
	"variable : 'd' typeid",
	"variable : 'r' typeid",
	"variable : 'G' typeid",
	"variable : 'S' typeid",
	"variable : 'V' typeid",
	"variable : 'Y'",
	"variable : 'Z' typeid NAME",
	"label : 'L'",
	"proc : 'f' typeid",
	"proc : 'g' typeid",
	"proc : 'm' typeid",
	"proc : 'J' typeid",
	"proc : 'F' typeid",
	"proc : 'I'",
	"proc : 'P'",
	"proc : 'Q'",
	"typeid : INTEGER",
	"typeid : INTEGER '=' typedef",
	"typeid : INTEGER typeattrs typedef",
	"typeid : '(' INTEGER ',' INTEGER ')'",
	"typeid : '(' INTEGER ',' INTEGER ')' '=' typedef",
	"typeid : '(' INTEGER ',' INTEGER ')' typeattrs typedef",
	"typeattrs : '@' typeattrlist ';'",
	"typeattrlist : typeattrlist ';' '@' typeattr",
	"typeattrlist : typeattr",
	"typeattr : 'a' INTEGER",
	"typeattr : 's' INTEGER",
	"typeattr : 'p' INTEGER",
	"typeattr : 'P'",
	"typeattr : error",
	"typedef : INTEGER",
	"typedef : 'b' typeid ';' '#' numbytes",
	"typedef : 'c' typeid ';' '#' numbits",
	"typedef : 'd' typeid",
	"typedef : 'e' enumspec ';'",
	"typedef : 'g' typeid ';' '#' numbits",
	"typedef : 'i' NAME ':' NAME ';'",
	"typedef : 'i' NAME ':' NAME ',' typeid ';'",
	"typedef : 'k' typeid",
	"typedef : 'l' ';' '#'",
	"typedef : 'm' optvbasespec optmultibasespec typeid ':' typeid ':' typeid ';'",
	"typedef : 'n' typeid ';' '#' numbytes",
	"typedef : 'o' NAME ';'",
	"typedef : 'o' NAME ',' typeid",
	"typedef : 'w' typeid",
	"typedef : 'z' typeid ';' '#' numbytes",
	"typedef : 'C' usage",
	"typedef : 'I' numbytes ';' '#' picsize",
	"typedef : 'K' cobolfiledesc ';'",
	"typedef : 'M' typeid ';' '#' bound",
	"typedef : 'N'",
	"typedef : 'S' typeid",
	"typedef : '*' typeid",
	"typedef : '&' typeid",
	"typedef : 'V' typeid",
	"typedef : 'Z'",
	"typedef : array",
	"typedef : subrange",
	"typedef : proceduretype",
	"typedef : record",
	"enumspec : enumlist",
	"enumspec : typeid ':' enumlist",
	"enumlist : enum",
	"enumlist : enumlist enum",
	"enum : NAME ':' ordvalue ',' '#'",
	"array : 'a' typeid ';' '#' typeid",
	"array : 'A' typeid",
	"array : 'D' INTEGER ',' typeid",
	"array : 'E' INTEGER ',' typeid",
	"array : 'O' INTEGER ',' typeid",
	"array : 'P' typeid ';' '#' typeid",
	"subrange : 'r' typeid ';' '#' bound ';' '#' bound",
	"bound : INTEGER",
	"bound : boundtype INTEGER",
	"bound : 'J'",
	"boundtype : 'A'",
	"boundtype : 'S'",
	"boundtype : 'T'",
	"boundtype : 'a'",
	"boundtype : 't'",
	"proceduretype : 'f' typeid ';'",
	"proceduretype : 'f' typeid ',' numparams ';' tparamlist ';'",
	"proceduretype : 'p' numparams ';' tparamlist ';'",
	"proceduretype : 'R' numparams ';' namedtparamlist",
	"proceduretype : 'F' typeid ',' numparams ';' namedtparamlist ';'",
	"numparams : INTEGER",
	"tparamlist : tparam",
	"tparamlist : tparamlist tparam",
	"tparam : typeid ',' passby ';' '#'",
	"namedtparamlist : /* empty */",
	"namedtparamlist : namedtplist",
	"namedtplist : namedtparam",
	"namedtplist : namedtplist namedtparam",
	"namedtparam : NAME ':' typeid ',' passby initbody ';' '#'",
	"namedtparam : ':' typeid ',' passby initbody ';' '#'",
	"record : 's' numbytes '#' fieldlist ';'",
	"record : 'u' numbytes '#' fieldlist ';'",
	"record : 'v' numbytes '#' fieldlist variantpart ';'",
	"record : 'Y' numbytes classkey optpbv optbasespeclist '(' extendedfieldlist optnameresolutionlist ';'",
	"record : 'G' redefinition ',' 'n' numbits '#' fieldlist ';'",
	"record : 'G' redefinition ',' 'c' numbits '#' condfieldlist ';'",
	"record : 'G' 'n' numbits fieldlist ';'",
	"record : 'G' 'c' numbits condfieldlist ';'",
	"optvbasespec : /* empty */",
	"optvbasespec : 'v'",
	"optmultibasespec : /* empty */",
	"optmultibasespec : 'm'",
	"optpbv : /* empty */",
	"optpbv : 'V'",
	"classkey : 's'",
	"classkey : 'u'",
	"classkey : 'c'",
	"optbasespeclist : /* empty */",
	"optbasespeclist : basespeclist",
	"basespeclist : basespec",
	"basespeclist : basespeclist ',' basespec",
	"basespec : virtualaccessspec baseclassoffset ':' classtypeid",
	"baseclassoffset : INTEGER",
	"classtypeid : typeid",
	"virtualaccessspec : /* empty */",
	"virtualaccessspec : 'v' accessspec",
	"virtualaccessspec : 'v'",
	"virtualaccessspec : accessspec",
	"genspec : /* empty */",
	"genspec : 'c'",
	"accessspec : 'i' '#'",
	"accessspec : 'o' '#'",
	"accessspec : 'u' '#'",
	"anonspec : /* empty */",
	"anonspec : 'a'",
	"virtualspec : /* empty */",
	"virtualspec : 'v' 'p'",
	"virtualspec : 'v'",
	"extendedfieldlist : /* empty */",
	"extendedfieldlist : extendedfieldlist extendedfield",
	"extendedfield : genspec accessspec anonspec datamember",
	"extendedfield : genspec virtualspec accessspec optvirtualfuncindex memberfunction",
	"extendedfield : accessspec anonspec nestedclass",
	"extendedfield : anonspec friendclass",
	"extendedfield : anonspec friendfunction",
	"datamember : memberattrs ':' field ';'",
	"memberattrs : isstatic isvtblptr isvbaseptr",
	"isstatic : /* empty */",
	"isstatic : 's'",
	"isvtblptr : /* empty */",
	"isvtblptr : 'p' INTEGER NAME",
	"isvbaseptr : /* empty */",
	"isvbaseptr : 'b'",
	"isvbaseptr : 'r'",
	"memberfunction : '[' functype memberfuncattrs ':' NAME ':' typeid ';' '#'",
	"memberfuncattrs : /* empty */",
	"memberfuncattrs : 's'",
	"memberfuncattrs : 'i'",
	"memberfuncattrs : 'k'",
	"memberfuncattrs : 'V'",
	"nestedclass : 'N' typeid ';' '#'",
	"friendclass : '(' typeid ';' '#'",
	"friendfunction : ']' NAME ':' typeid ';' '#'",
	"optvirtualfuncindex : /* empty */",
	"optvirtualfuncindex : INTEGER",
	"functype : 'f'",
	"functype : 'c'",
	"functype : 'd'",
	"initbody : /* empty */",
	"initbody : STRING",
	"optnameresolutionlist : /* empty */",
	"optnameresolutionlist : ')' nameresolutionlist",
	"nameresolutionlist : nameresolution",
	"nameresolutionlist : nameresolution ',' nameresolutionlist",
	"nameresolution : membername ':' classtypeid",
	"nameresolution : membername ':'",
	"membername : NAME",
	"fieldlist : field",
	"fieldlist : fieldlist field",
	"field : NAME ':' typeid ',' bitoffset ',' numbits ';' '#'",
	"variantpart : '[' vtag vfieldlist ']'",
	"vtag : '(' field",
	"vtag : '(' NAME ':' ';' '#'",
	"vfieldlist : vlist",
	"vfieldlist : vfieldlist vlist",
	"vlist : vfield",
	"vlist : vfield variantpart",
	"vfield : '(' vrangelist ':' fieldlist",
	"vrangelist : vrange",
	"vrangelist : vrangelist ',' vrange",
	"vrange : 'b' ordvalue",
	"vrange : 'c' ordvalue",
	"vrange : 'e' typeid ',' ordvalue",
	"vrange : 'i' INTEGER",
	"vrange : 'r' typeid ';' bound ';' bound",
	"condfieldlist : conditions ',' '#' fieldlist",
	"condfieldlist : fieldlist '#'",
	"conditions : /* empty */",
	"conditions : conditions condition",
	"bitoffset : INTEGER",
	"usage : picstoragetype numbits ',' editdescription ',' picsize ';'",
	"usage : redefinition picstoragetype numbits ',' editdescription ',' picsize ';'",
	"usage : picstoragetype numbits ',' editdescription ',' picsize ',' '#' condition ';'",
	"usage : redefinition picstoragetype numbits ',' editdescription ',' picsize ',' '#' condition ';'",
	"redefinition : 'r' NAME",
	"picstoragetype : 'a'",
	"picstoragetype : 'b'",
	"picstoragetype : 'c'",
	"picstoragetype : 'd'",
	"picstoragetype : 'e'",
	"picstoragetype : 'f'",
	"picstoragetype : 'g'",
	"picstoragetype : 'h'",
	"picstoragetype : 'i'",
	"picstoragetype : 'j'",
	"picstoragetype : 'k'",
	"picstoragetype : 'l'",
	"picstoragetype : 'm'",
	"picstoragetype : 'n'",
	"picstoragetype : 'o'",
	"picstoragetype : 'p'",
	"picstoragetype : 'q'",
	"picstoragetype : 'r'",
	"picstoragetype : 's'",
	"picstoragetype : 't'",
	"editdescription : STRING",
	"editdescription : INTEGER",
	"picsize : INTEGER",
	"condition : NAME ':' INTEGER '=' 'q' conditiontype ',' valuelist ';' '#'",
	"conditiontype : conditionprimitive ',' kanjichar",
	"conditionprimitive : 'n' sign decimalsite",
	"conditionprimitive : 'a'",
	"conditionprimitive : 'f'",
	"sign : '+'",
	"sign : '-'",
	"sign : /* empty */",
	"decimalsite : INTEGER",
	"kanjichar : INTEGER",
	"valuelist : value",
	"valuelist : valuelist value",
	"value : INTEGER ':' arbitrarycharacters '#'",
	"arbitrarycharacters : INTEGER",
	"cobolfiledesc : organization accessmethod numbytes",
	"organization : 'i'",
	"organization : 'l'",
	"organization : 'r'",
	"organization : 's'",
	"accessmethod : 'd'",
	"accessmethod : 'o'",
	"accessmethod : 'r'",
	"accessmethod : 's'",
	"passby : INTEGER",
};
#endif /* YYDEBUG */
# line	1 "/usr/ccs/bin/yaccpar"
/*
 * Copyright (c) 1993 by Sun Microsystems, Inc.
 */

#pragma ident	"@(#)yaccpar	6.16	99/01/20 SMI"

/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#define YYERROR		goto yyerrlab
#define YYACCEPT	return(0)
#define YYABORT		return(1)
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
	{\
		yyerror( "syntax error - cannot backup" );\
		goto yyerrlab;\
	}\
	yychar = newtoken;\
	yystate = *yyps;\
	yylval = newvalue;\
	goto yynewstate;\
}
#define YYRECOVERING()	(!!yyerrflag)
#define YYNEW(type)	malloc(sizeof(type) * yynewmax)
#define YYCOPY(to, from, type) \
	(type *) memcpy(to, (char *) from, yymaxdepth * sizeof (type))
#define YYENLARGE( from, type) \
	(type *) realloc((char *) from, yynewmax * sizeof(type))
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int yydebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-10000000)

/*
** global variables used by the parser
*/
YYSTYPE *yypv;			/* top of value stack */
int *yyps;			/* top of state stack */

int yystate;			/* current state */
int yytmp;			/* extra var (lasts between blocks) */

int yynerrs;			/* number of errors */
int yyerrflag;			/* error recovery flag */
int yychar;			/* current input token number */



#ifdef YYNMBCHARS
#define YYLEX()		yycvtok(yylex())
/*
** yycvtok - return a token if i is a wchar_t value that exceeds 255.
**	If i<255, i itself is the token.  If i>255 but the neither 
**	of the 30th or 31st bit is on, i is already a token.
*/
#if defined(__STDC__) || defined(__cplusplus)
int yycvtok(int i)
#else
int yycvtok(i) int i;
#endif
{
	int first = 0;
	int last = YYNMBCHARS - 1;
	int mid;
	wchar_t j;

	if(i&0x60000000){/*Must convert to a token. */
		if( yymbchars[last].character < i ){
			return i;/*Giving up*/
		}
		while ((last>=first)&&(first>=0)) {/*Binary search loop*/
			mid = (first+last)/2;
			j = yymbchars[mid].character;
			if( j==i ){/*Found*/ 
				return yymbchars[mid].tvalue;
			}else if( j<i ){
				first = mid + 1;
			}else{
				last = mid -1;
			}
		}
		/*No entry in the table.*/
		return i;/* Giving up.*/
	}else{/* i is already a token. */
		return i;
	}
}
#else/*!YYNMBCHARS*/
#define YYLEX()		yylex()
#endif/*!YYNMBCHARS*/

/*
** yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
#if defined(__STDC__) || defined(__cplusplus)
int yyparse(void)
#else
int yyparse()
#endif
{
	register YYSTYPE *yypvt = 0;	/* top of value stack for $vars */

#if defined(__cplusplus) || defined(lint)
/*
	hacks to please C++ and lint - goto's inside
	switch should never be executed
*/
	static int __yaccpar_lint_hack__ = 0;
	switch (__yaccpar_lint_hack__)
	{
		case 1: goto yyerrlab;
		case 2: goto yynewstate;
	}
#endif

	/*
	** Initialize externals - yyparse may be called more than once
	*/
	yypv = &yyv[-1];
	yyps = &yys[-1];
	yystate = 0;
	yytmp = 0;
	yynerrs = 0;
	yyerrflag = 0;
	yychar = -1;

#if YYMAXDEPTH <= 0
	if (yymaxdepth <= 0)
	{
		if ((yymaxdepth = YYEXPAND(0)) <= 0)
		{
			yyerror("yacc initialization error");
			YYABORT;
		}
	}
#endif

	{
		register YYSTYPE *yy_pv;	/* top of value stack */
		register int *yy_ps;		/* top of state stack */
		register int yy_state;		/* current state */
		register int  yy_n;		/* internal state number info */
	goto yystack;	/* moved from 6 lines above to here to please C++ */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	yynewstate:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;
		goto yy_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	yystack:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	yy_stack:
		/*
		** put a state and value onto the stacks
		*/
#if YYDEBUG
		/*
		** if debugging, look up token value in list of value vs.
		** name pairs.  0 and negative (-1) are special values.
		** Note: linear search is used since time is not a real
		** consideration while debugging.
		*/
		if ( yydebug )
		{
			register int yy_i;

			printf( "State %d, token ", yy_state );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++yy_ps >= &yys[ yymaxdepth ] )	/* room on stack? */
		{
			/*
			** reallocate and recover.  Note that pointers
			** have to be reset, or bad things will happen
			*/
			long yyps_index = (yy_ps - yys);
			long yypv_index = (yy_pv - yyv);
			long yypvt_index = (yypvt - yyv);
			int yynewmax;
#ifdef YYEXPAND
			yynewmax = YYEXPAND(yymaxdepth);
#else
			yynewmax = 2 * yymaxdepth;	/* double table size */
			if (yymaxdepth == YYMAXDEPTH)	/* first time growth */
			{
				char *newyys = (char *)YYNEW(int);
				char *newyyv = (char *)YYNEW(YYSTYPE);
				if (newyys != 0 && newyyv != 0)
				{
					yys = YYCOPY(newyys, yys, int);
					yyv = YYCOPY(newyyv, yyv, YYSTYPE);
				}
				else
					yynewmax = 0;	/* failed */
			}
			else				/* not first time */
			{
				yys = YYENLARGE(yys, int);
				yyv = YYENLARGE(yyv, YYSTYPE);
				if (yys == 0 || yyv == 0)
					yynewmax = 0;	/* failed */
			}
#endif
			if (yynewmax <= yymaxdepth)	/* tables not expanded */
			{
				yyerror( "yacc stack overflow" );
				YYABORT;
			}
			yymaxdepth = yynewmax;

			yy_ps = yys + yyps_index;
			yy_pv = yyv + yypv_index;
			yypvt = yyv + yypvt_index;
		}
		*yy_ps = yy_state;
		*++yy_pv = yyval;

		/*
		** we have a new state - find out what to do
		*/
	yy_newstate:
		if ( ( yy_n = yypact[ yy_state ] ) <= YYFLAG )
			goto yydefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		yytmp = yychar < 0;
#endif
		if ( ( yychar < 0 ) && ( ( yychar = YYLEX() ) < 0 ) )
			yychar = 0;		/* reached EOF */
#if YYDEBUG
		if ( yydebug && yytmp )
		{
			register int yy_i;

			printf( "Received token " );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( yy_n += yychar ) < 0 ) || ( yy_n >= YYLAST ) )
			goto yydefault;
		if ( yychk[ yy_n = yyact[ yy_n ] ] == yychar )	/*valid shift*/
		{
			yychar = -1;
			yyval = yylval;
			yy_state = yy_n;
			if ( yyerrflag > 0 )
				yyerrflag--;
			goto yy_stack;
		}

	yydefault:
		if ( ( yy_n = yydef[ yy_state ] ) == -2 )
		{
#if YYDEBUG
			yytmp = yychar < 0;
#endif
			if ( ( yychar < 0 ) && ( ( yychar = YYLEX() ) < 0 ) )
				yychar = 0;		/* reached EOF */
#if YYDEBUG
			if ( yydebug && yytmp )
			{
				register int yy_i;

				printf( "Received token " );
				if ( yychar == 0 )
					printf( "end-of-file\n" );
				else if ( yychar < 0 )
					printf( "-none-\n" );
				else
				{
					for ( yy_i = 0;
						yytoks[yy_i].t_val >= 0;
						yy_i++ )
					{
						if ( yytoks[yy_i].t_val
							== yychar )
						{
							break;
						}
					}
					printf( "%s\n", yytoks[yy_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register YYCONST int *yyxi = yyexca;

				while ( ( *yyxi != -1 ) ||
					( yyxi[1] != yy_state ) )
				{
					yyxi += 2;
				}
				while ( ( *(yyxi += 2) >= 0 ) &&
					( *yyxi != yychar ) )
					;
				if ( ( yy_n = yyxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( yy_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( yyerrflag )
			{
			case 0:		/* new error */
				yyerror( "syntax error" );
				goto skip_init;
			yyerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				yy_pv = yypv;
				yy_ps = yyps;
				yy_state = yystate;
			skip_init:
				yynerrs++;
				/* FALLTHRU */
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				yyerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( yy_ps >= yys )
				{
					yy_n = yypact[ *yy_ps ] + YYERRCODE;
					if ( yy_n >= 0 && yy_n < YYLAST &&
						yychk[yyact[yy_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						yy_state = yyact[ yy_n ];
						goto yy_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( yydebug )
						printf( _POP_, *yy_ps,
							yy_ps[-1] );
#	undef _POP_
#endif
					yy_ps--;
					yy_pv--;
				}
				/*
				** there is no state on stack with "error" as
				** a valid shift.  give up.
				*/
				YYABORT;
			case 3:		/* no shift yet; eat a token */
#if YYDEBUG
				/*
				** if debugging, look up token in list of
				** pairs.  0 and negative shouldn't occur,
				** but since timing doesn't matter when
				** debugging, it doesn't hurt to leave the
				** tests here.
				*/
				if ( yydebug )
				{
					register int yy_i;

					printf( "Error recovery discards " );
					if ( yychar == 0 )
						printf( "token end-of-file\n" );
					else if ( yychar < 0 )
						printf( "token -none-\n" );
					else
					{
						for ( yy_i = 0;
							yytoks[yy_i].t_val >= 0;
							yy_i++ )
						{
							if ( yytoks[yy_i].t_val
								== yychar )
							{
								break;
							}
						}
						printf( "token %s\n",
							yytoks[yy_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( yychar == 0 )	/* reached EOF. quit */
					YYABORT;
				yychar = -1;
				goto yy_newstate;
			}
		}/* end if ( yy_n == 0 ) */
		/*
		** reduction by production yy_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( yydebug )
			printf( "Reduce by (%d) \"%s\"\n",
				yy_n, yyreds[ yy_n ] );
#endif
		yytmp = yy_n;			/* value to switch over */
		yypvt = yy_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using yy_state here as temporary
		** register variable, but why not, if it works...
		** If yyr2[ yy_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto yy_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int yy_len = yyr2[ yy_n ];

			if ( !( yy_len & 01 ) )
			{
				yy_len >>= 1;
				yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
				yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
					*( yy_ps -= yy_len ) + 1;
				if ( yy_state >= YYLAST ||
					yychk[ yy_state =
					yyact[ yy_state ] ] != -yy_n )
				{
					yy_state = yyact[ yypgo[ yy_n ] ];
				}
				goto yy_stack;
			}
			yy_len >>= 1;
			yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
			yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
				*( yy_ps -= yy_len ) + 1;
			if ( yy_state >= YYLAST ||
				yychk[ yy_state = yyact[ yy_state ] ] != -yy_n )
			{
				yy_state = yyact[ yypgo[ yy_n ] ];
			}
		}
					/* save until reenter driver code */
		yystate = yy_state;
		yyps = yy_ps;
		yypv = yy_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( yytmp )
	{
		
# line	531 "/usr/ccs/bin/yaccpar"
	}
	goto yystack;		/* reset registers in driver code */
}

