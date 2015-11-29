#ifndef __IR_CLAUSE_H
#define __IR_CLAUSE_H
// class definition for intermediate representation of clauses 

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "list.h"
#include "mystring.h"

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
	IRClause(const List<String> &, int = 0);
	~IRClause();

	// assignment 
	IRClause &operator=(const IRClause &);

	// comparisons
	int operator==(const IRClause &) const;
	int operator!=(const IRClause &) const;

	// add or remove term from clause
	void insert(const String &);
	void remove(String &);
	void clear();

	// tests to run on a clause
	int isInClause(const String &) const;
	int isEmpty() const;

	// access conclusion bit
	int getPartOfConclusion() const;
	void setPartOfConclusion(int);

	// dump data
	friend ostream &operator<<(ostream &, const IRClause &);

protected:
	// internal data
	int partOfConclusion;
	List<String> terms;
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
	String operator()();

	// advance to next set member
	int operator++(int);

private:
	// not allowed
	IRClauseIterator &operator=(const IRClauseIterator &);

protected:
	// internal data
	ListIterator<String> iterator;
};

#endif


