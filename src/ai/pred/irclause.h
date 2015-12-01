#ifndef __IR_CLAUSE_H
#define __IR_CLAUSE_H
// class definition for intermediate representation of clauses 

// headers
#include <stdlib.h>
#include <iostream>

// local headers
#include "system/Returns.h"
#include "system/Debug.h"
#include "adt/List.h"
#include "adt/String.h"

using namespace std;
using namespace ombt;

// forward declarations
class IRClauseIterator;

// clause class definitions
class IRClause {
public:
	// friend classes
	friend class IRClauseIterator;

	// constructors and destructor
	IRClause();
	IRClause(const IRClause &);
	IRClause(const ombt::List<ombt::String> &, int = 0);
	~IRClause();

	// assignment 
	IRClause &operator=(const IRClause &);

	// comparisons
	int operator==(const IRClause &) const;
	int operator!=(const IRClause &) const;

	// add or remove term from clause
	void insert(const ombt::String &);
	void remove(ombt::String &);
	void clear();

	// tests to run on a clause
	int isInClause(const ombt::String &) const;
	int isEmpty() const;

	// access conclusion bit
	int getPartOfConclusion() const;
	void setPartOfConclusion(int);

	// dump data
	friend std::ostream &operator<<(std::ostream &, const IRClause &);

protected:
	// internal data
	int partOfConclusion;
	ombt::List<ombt::String> terms;
};

// clause iterator definition
class IRClauseIterator {
public:
	// constructors and destructor
	IRClauseIterator(const IRClause &);
	IRClauseIterator(const IRClauseIterator &);
	~IRClauseIterator();

	// reset iterator to start
	void reset();

	// check if at end of set
	int done() const;

	// return current member in set
	ombt::String operator()();

	// advance to next set member
	int operator++(int);

private:
	// not allowed
	IRClauseIterator &operator=(const IRClauseIterator &);

protected:
	// internal data
	ombt::ListIterator<ombt::String> iterator;
};

#endif


