#ifndef __STATISTICS_H
#define __STATISTICS_H
// map used to store statistics

// headers
#include "returns.h"
#include "debug.h"
#include "mystring.h"
#include "map.h"

// type definitions
typedef Map<String, unsigned long> Statistics;
typedef MapIterator<String, unsigned long> StatisticsIterator;

// extern for statistics structure
extern Statistics statistics;
extern Statistics totalstatistics;
extern Statistics programstatistics;

// statistics names
#define AttemptedSubsumptionTests "Attempted Subsumption Tests"
#define AttemptedStillmanSubsumptionTests "Attempted Stillman Subsumption Tests"
#define SubsumedClausesRemoved "Subsumed Clauses Removed"
#define AttemptedTautologyTests "Attempted Tautology Tests"
#define TautologiesRemoved "Tautologies Removed"
#define ClausesGenerated "Clauses Generated"
#define ResolutionsAttempted "Resolutions Attempted"
#define RedundantClauseTestsAttempted "Redundant Clause Tests Attempted"
#define RedundantClausesRejected "Redundant Clauses Rejected"
#define SolutionsFound "Solutions Found"
#define MaximumLiteralsClausesRejected "Maximum Literals Clauses Rejected"

#define TotalAttemptedSubsumptionTests "Total Attempted Subsumption Tests"
#define TotalAttemptedStillmanSubsumptionTests "Total Attempted Stillman Subsumption Tests"
#define TotalSubsumedClausesRemoved "Total Subsumed Clauses Removed"
#define TotalAttemptedTautologyTests "Total Attempted Tautology Tests"
#define TotalTautologiesRemoved "Total Tautologies Removed"
#define TotalClausesGenerated "Total Clauses Generated"
#define TotalResolutionsAttempted "Total Resolutions Attempted"
#define TotalRedundantClauseTestsAttempted "Total Redundant Clause Tests Attempted"
#define TotalRedundantClausesRejected "Total Redundant Clauses Rejected "
#define TotalSolutionsFound "Total Solutions Found"
#define TotalMaximumLiteralsClausesRejected "Total Maximum Literals Clauses Rejected"

#define TotalFilesRead "Total Files Read"
#define TotalProgramsExecuted "Total Programs Executed"
#define TotalValidPrograms "Total Valid Programs"
#define TotalNotProvenPrograms "Total Not Proven Programs"
#define TotalMaximumClauseExceededPrograms "Total Maximum Clause Exceeded Programs"
#define TotalMaximumDepthExceededPrograms "Total Maximum Depth Exceeded Programs"

#endif
