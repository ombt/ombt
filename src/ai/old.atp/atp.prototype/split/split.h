#ifndef _SPLIT_H
#define _SPLIT_H
// atp problem splitter header file

// required headers
#include "returns.h"
#include "debug.h"
#include "array.h"
#include "mylist.h"
#include "binaryTree_AVL.h"
#include "semantic.h"
#include "symbol.h"
#include "mystring.h"

// symbol table
extern BinaryTree_AVL<Symbol> symbols;

// program data
extern List<Symbol> scope;
extern List<Semantic * > ptrees;
extern List<List<Semantic * > > programs;

// other globals
extern char errormsg[BUFSIZ];
extern int syntaxErrors;
extern int echoflag;
extern int fileflag;
extern int verbose;
extern int problem;
extern char filename[BUFSIZ];
extern char tmpdir[BUFSIZ];
extern char problemdir[BUFSIZ];
extern List<Semantic * > ptrees;
extern List<String> strategylist;

// externs
extern int createProblems();
extern int makedir(const char *);
extern int removedir(const char *);
extern int createSubProblems();
extern char *findlast(char *, int);

// local definitions
#define ATPTEMP "atp_temp"
#define RMCMD "/usr/bin/rm -rf"

#endif

