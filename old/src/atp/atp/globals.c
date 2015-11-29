// global variables

// headers
#include "globals.h"

// globals
int verbose = 0;
int reportmemoryusage = 0;
int echo = 0;
int maxdepth = 12;
int maxclause = 200;
int maxliterals = INT_MAX;
int paramodulation = 0;
int subsumptiontest = 1;
int tautologytest = 1;
SearchType searchtype = DepthFirst;
ReportType reporttype = ReportNone;
int bfswithchecks = 0;
int unitliteral = 1;
int usedepth = 1;
int complexity = 1;
int solutionsrequired = 1;
int solutionsfound = 0;
int weight = 5;
String tempbase(DEFAULTTEMPBASEDIR);
List<String> includedirs;
String tempdir("");
Statistics statistics;
Statistics totalstatistics;
Statistics programstatistics;
int nextClause = 1;
unsigned int currentBFSDepth = 1;
Map<String, String> options;

