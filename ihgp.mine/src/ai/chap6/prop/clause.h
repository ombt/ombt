#ifndef __CLAUSE_H
#define __CLAUSE_H
// class definition for clauses 

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "atom.h"
#include "set_List.h"

// forward declarations
class ClauseIterator;

// clause class definitions
class Clause {
public:
	// friend classes
	friend class ClauseIterator;

	// constructors and destructor
	Clause();
	Clause(const Clause &);
	Clause(const Set_List<Atom> &, int = -1, int = 0);
	virtual ~Clause();

	// assignment 
	Clause &operator=(const Clause &);

	// comparisons
	virtual int operator==(const Clause &) const;
	virtual int operator!=(const Clause &) const;
	virtual int operator<=(const Clause &) const;
	virtual int operator>=(const Clause &) const;
	virtual int operator<(const Clause &) const;
	virtual int operator>(const Clause &) const;

	// add or remove atoms from clause
	void insert(const Atom &);
	void remove(Atom &);
	void clear();

	// tests to run on a clause
	int isInClause(const Atom &) const;
	int isEmpty() const;
	int isATautology() const;
	int isPartOfConclusion() const;
	int subsumes(const Clause &) const;

	// access conclusion bit
	int getPartOfConclusion() const;
	void setPartOfConclusion(int);

	// access depth and number fields
	int getDepth() const;
	void setDepth(int);
	int getNumber() const;
	void setNumber(int);

        // union set operations
        Clause &operator|=(const Clause &);
        Clause operator|(const Clause &) const;

        // intersection set operations
        Clause &operator&=(const Clause &);
        Clause operator&(const Clause &) const;

        // difference set operations
        Clause &operator-=(const Clause &);
        Clause operator-(const Clause &) const;

	// dump data
	friend ostream &operator<<(ostream &, const Clause &);

protected:
	// internal data
	int depth;
	int number;
	int partOfConclusion;
	Set_List<Atom> atoms;
};

// clause class sorted by length definitions
class LClause: public Clause {
public:
	// friend classes
	friend class ClauseIterator;

	// constructors and destructor
	LClause();
	LClause(const Clause &);
	LClause(const LClause &);
	virtual ~LClause();

	// assignment 
	LClause &operator=(const LClause &);

	// comparisons
	virtual int operator==(const Clause &) const;
	virtual int operator!=(const Clause &) const;
	virtual int operator<=(const Clause &) const;
	virtual int operator>=(const Clause &) const;
	virtual int operator<(const Clause &) const;
	virtual int operator>(const Clause &) const;

	// dump data
	friend ostream &operator<<(ostream &, const LClause &);
};

// clause iterator definition
class ClauseIterator {
public:
	// constructors and destructor
	ClauseIterator(const Clause &);
	ClauseIterator(const ClauseIterator &);
	~ClauseIterator();

	// reset iterator to start
	void reset();

	// check if at end of set
	int done() const;

	// return current member in set
	Atom operator()();

	// advance to next set member
	int operator++(int);

private:
	// not allowed
	ClauseIterator &operator=(const ClauseIterator &);

protected:
	// internal data
	Set_List_Iterator<Atom> iterator;
};

#endif


