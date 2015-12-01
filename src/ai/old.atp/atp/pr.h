#ifndef __PR_H
#define __PR_H
// theorem prover header file

// headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <re_comp.h>
#include <signal.h>
 
// local headers
#include "returns.h"
#include "debug.h"
#include "mystring.h"
#include "mylist.h"
#include "tuple.h"
#include "binaryTree_AVL.h"
#include "clause.h"
#include "semantic.h"
#include "symbol.h"
#include "uniquename.h"
#include "literal.h"
#include "substitution.h"
#include "deletion.h"
#include "statistics.h"
#include "globals.h"
#include "search.h"
#include "bfsnode.h"
#include "varnames.h"

// symbol table
extern BinaryTree_AVL<Symbol> symbols;

// program data
extern List<Symbol> scope;
extern List<Semantic * > ptrees;
extern List<Semantic * > program;
extern List<List<String> > clauses;

// externs
extern int verbose;
extern int echo;
extern int maxdepth;
extern int maxclause;
extern int paramodulation;
extern int equality;
extern String tempbase;
extern List<String> includedirs;
extern String tempdir;
extern int subsumptiontest;
extern int tautologytest;

// phases to run
extern Map<String, int> phases;

// phase names
#define EXPANDFILE "exp"
#define PARSEFILE  "prs"
#define ADDEXTRAAXIOMS  "xax"
#define CONVERT2CNF  "cnv"
#define RUNPROVER  "prv"

// functions
extern void usage(const char *);
extern int isadir(const String &);
extern int expandFiles(const List<String> &, List<Tuple<String, String> > &);
extern int parser(const List<String> &);
extern int runprover();
extern int addextraaxioms();
extern int addreflexiveaxioms();
extern int addequalityaxioms();
extern void clearProgram();
extern void clearClauses();
extern void dumpTree();
extern void dumpProgram();
extern void dumpClauses();
extern int convert2cnf();
#endif

