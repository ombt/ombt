/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

union YYSTYPE
{
#line 43 "pr.y" /* yacc.c:1909  */

	int flag;
	char cstring[32];
	Semantic *psemantic;
	List<Semantic *> *parguments;

#line 129 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
