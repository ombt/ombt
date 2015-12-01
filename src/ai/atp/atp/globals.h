#ifndef __GLOBALS_H
#define __GLOBALS_H
// global header

// headers
#include <stdlib.h>
#include <iostream>
#include <limits.h>

// local headers
#include <Returns.h>
#include <Debug.h>
#include <adt/String.h>
#include <adt/List.h>
#include <adt/Tuple.h>
#include "statistics.h"

using namespace std;
using namespace ombt;

// definitions
#define DEFAULTTEMPDIR "atp_temp"
#define DEFAULTTEMPBASEDIR "."

// types of reports
enum ReportType { 
	ReportNone, 
	ReportBoth, 
	ReportParent, 
	ReportStack
};

// types of searches
enum SearchType { 
	Saturation, 
	BreadthFirst, 
	DepthFirst, 
	DepthFirstHillClimb, 
	BestFirst, 
	IterativeDeepening
};

// externs
extern int verbose;
extern int reportmemoryusage;
extern int echo;
extern int maxdepth;
extern int maxclause;
extern int maxliterals;
extern int paramodulation;
extern int subsumptiontest;
extern int tautologytest;
extern SearchType searchtype;
extern ReportType reporttype;
extern int bfswithchecks;
extern int unitliteral;
extern int usedepth;
extern int solutionsrequired;
extern int solutionsfound;
extern int weight;
extern String tempbase;
extern List<String> includedirs;
extern String tempdir;
extern Statistics statistics;
extern Statistics totalstatistics;
extern Statistics programstatistics;
extern int nextClause;
extern unsigned int currentBFSDepth;
extern Map<String, String> options;
extern int complexity;

#endif
