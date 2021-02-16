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
    INTEGER = 258,
    _FLOAT = 259,
    PUSH = 260,
    IADD = 261,
    IMINUS = 262,
    IMULTI = 263,
    IDIVIDE = 264,
    FADD = 265,
    FMINUS = 266,
    FMULTI = 267,
    FDIVIDE = 268,
    BIT_OR = 269,
    BIT_AND = 270,
    LOAD = 271,
    JUMP = 272,
    JUMPF = 273,
    HALT = 274,
    POP = 275,
    LESS = 276,
    LOOP = 277,
    ARGS = 278,
    CALL = 279,
    RET = 280
  };
#endif
/* Tokens.  */
#define INTEGER 258
#define _FLOAT 259
#define PUSH 260
#define IADD 261
#define IMINUS 262
#define IMULTI 263
#define IDIVIDE 264
#define FADD 265
#define FMINUS 266
#define FMULTI 267
#define FDIVIDE 268
#define BIT_OR 269
#define BIT_AND 270
#define LOAD 271
#define JUMP 272
#define JUMPF 273
#define HALT 274
#define POP 275
#define LESS 276
#define LOOP 277
#define ARGS 278
#define CALL 279
#define RET 280

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 7 "easm.y" /* yacc.c:1909  */

    int ival;
    float fval;

#line 109 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
