/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

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
#line 66 "pred.y" /* yacc.c:1909  */

	char cstring[32];
	Semantic *psemantic;
	List<Semantic *> *parguments;

#line 108 "y.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
