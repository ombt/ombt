#ifndef _SPLIT_H
#define _SPLIT_H
// atp problem splitter header file

// required headers
#include "returns.h"
#include "debug.h"
#include "array.h"
#include "mylist.h"
#include "binaryTree_AVL.h"
#include "orderedset.h"
#include "mystring.h"
#include "clause.h"
#include "semantic.h"
#include "symbol.h"
#include "uniquename.h"
#include "literal.h"
#include "substitution.h"
#include "deletion.h"

// symbol table
extern BinaryTree_AVL<Symbol> symbols;

// program data
extern List<Symbol> scope;
extern List<Semantic * > ptrees;
extern List<Semantic * > program;
extern List<List<String> > clauses;

// other globals
extern char errormsg[BUFSIZ];
extern int syntaxErrors;
extern int echoflag;
extern int verbose;
extern int paramodulation;
extern int equality;
extern unsigned int maxdepth;
extern char filename[BUFSIZ];

// externs
extern int convert2cnf();
extern void dumpTree();
extern void dumpClauses();
extern void dumpProgram();
extern int runprover();
extern int addextraaxioms();
extern int addreflexiveaxioms();
extern int addequalityaxioms();
extern int resolveClauses(Array<OrderedSet<Clause> > &, 
	Clause &, Clause &, int &, unsigned int);

// local definitions
#define ATPTEMP "atp_temp"

#endif

