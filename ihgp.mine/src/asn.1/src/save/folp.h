#ifndef __PR_H
#define __PR_H
// theorem prover header file

// headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
 
// local headers
#include "returns.h"
#include "debug.h"
#include "mystring.h"
#include "array.h"
#include "mylist.h"
#include "binaryTree.h"
#include "map.h"
#include "symbol.h"
#include "semantic.h"

// globals
extern int echo;
extern int equality;
extern int paramodulation;
extern int postorder;
extern String reductionType;
extern List<Semantic * > premises;
extern List<Semantic * > extraaxioms;
extern List<Semantic * > conclusions;
extern List<Semantic * > clpremises;
extern List<Semantic * > clextraaxioms;
extern List<Semantic * > clconclusions;
extern BinaryTree<Symbol> symbols;
extern Map<String, String> options;
extern List<String> opts;
extern List<Symbol> scope;
extern String tmpdir;
extern String suffix;
extern String fname;
extern String ppfname;
extern String dname;

// externs
extern void clearConclusions();
extern void clearExtraAxioms();
extern void clearPremises();
extern void clearClauseConclusions();
extern void clearClauseExtraAxioms();
extern void clearClausePremises();
extern void clearAll();
extern void dumpcnf();
extern void dumpParseTrees();
extern void dumpOptions();
extern void dumpAll();
extern int convert2cnf();
extern int convert2cnf(Semantic *);
extern int convert2cnf(ClauseType, List<Semantic * > &, List<Semantic * > &);
extern int addextraaxioms();
extern int addreflexiveaxioms(int, int, const String &,
	const Array<String> &, Semantic **);
extern int addreflexiveaxioms();
extern int addeqreflexiveaxiom();
extern int addeqsymmetryaxiom();
extern int addeqtransitiveaxiom();
extern int addpredicateaxioms(int, int, int, const String &,
	const Array<String> &, Semantic **);
extern int addpredicateaxioms();
extern int addfunctionaxioms(int, int, int, const String &,
	const Array<String> &, Semantic **);
extern int addfunctionaxioms();
extern int addequalityaxioms();
extern int execute();

#endif

