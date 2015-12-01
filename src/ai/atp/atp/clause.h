#ifndef __CLAUSE_H
#define __CLAUSE_H
// class definition of clauses

// headers
#include <stdlib.h>
#include <iostream>

// local headers
#include <Returns.h>
#include <Debug.h>
#include <adt/List.h>
#include <adt/String.h>
#include <adt/BinaryTree_AVL.h>
#include <adt/Map.h>
#include <adt/Tuple.h>
#include "literal.h"
#include "statistics.h"
#include "memoryleak.h"
#include "varnames.h"

// forward declarations
class ClauseIterator;
class PositiveClauseIterator;
class NegativeClauseIterator;
class ClauseAnswersIterator;

// clause class definitions
class Clause {
public:
	// friend classes
	friend class ClauseIterator;
	friend class PositiveClauseIterator;
	friend class NegativeClauseIterator;
	friend class ClauseAnswersIterator;

	// clause type
	enum Type { Positive, Negative, Mixed, Empty,
		Unknown };

	// constructors and destructor
	Clause();
	Clause(const Clause &);
	Clause(const List<String> &);
	~Clause();

	// assignment 
	Clause &operator=(const Clause &);
	Clause &operator=(const List<String> &);

	// comparisons
	int operator==(const Clause &) const;
	int operator!=(const Clause &) const;
	int operator<(const Clause &) const;
	int operator<=(const Clause &) const;
	int operator>(const Clause &) const;
	int operator>=(const Clause &) const;

	// add or remove a literal from a clause
	int insert(const Literal &);
	int retrieve(Literal &) const;
	int update(const Literal &);
	int remove(Literal &);

	// add an answer literal in a clause
	int insertAnswer(const Literal &);

	// parse string to a literal
	int parse(const List<String> &);

	// clear clause
	void clear();

	// tests to run on a clause
	int isInClause(const Literal &) const;
	int isEmpty() const;

	// return count of classes and total members
	int getTotalMembers() const;

	// access internal state data
	unsigned int getDepth() const {
		return(depth);
	}
	void setDepth(unsigned int n) {
		depth = n;
	}
	unsigned int getNumber() const {
		return(number);
	}
	void setNumber(unsigned int n) {
		number = n;
	}
	int getConclusion() const {
		return(partOfConclusion);
	}
	void setConclusion(int poc) {
		partOfConclusion = poc;
	}
	int getQuery() const {
		return(partOfQuery);
	}
	void setQuery(int poq) {
		partOfQuery = poq;
	}
	int getSOS() const {
		return(setOfSupport);
	}
	void setSOS(int sos) {
		setOfSupport = sos;
	}
	Type getType() const {
		return(type);
	}
	void setType(Type t) {
		type = t;
	}

	// return this pointer
	void *getThis() const {
		return((void *)this);
	}

	// get maximal equivalent set or literal
	int getMaximalLiteral(Literal &) const;
	int getMaximalPositiveLiteral(Literal &) const;
	int getMaximalNegativeLiteral(Literal &) const;

	// rename all variables in clause
	int renameVariables();

	// convert clause to a string
	operator String ();
	operator String () const;
	String convertToString(String &) const;

	// basic statistics of clause
	void basicStats(int &, int &, int &, int &, int &, int &, int &) const;

#ifdef MEMORYLEAK
	// overload new and delete for leak detection
	void *operator new(size_t);
	void operator delete(void *);
#endif

	// combine two clauses
	friend Clause operator+(const Clause &, const Clause &);

	// dump data
	friend ostream &operator<<(ostream &, const Clause &);

protected:
	// utility functions
	int compare(const BinaryTree_AVL<Literal> &, 
		const BinaryTree_AVL<Literal> &) const;
	void updateType();

protected:
	// internal data
	unsigned int depth, number;
	Type type;
	String inputstring;
	int setOfSupport;
	int partOfConclusion;
	int partOfQuery;
	List<Literal> answers;
	BinaryTree_AVL<Literal> positiveClause;
	BinaryTree_AVL<Literal> negativeClause;

public:
	// track variable renaming
	static BinaryTree_AVL<Tuple<String, String> > variableNames;

#ifdef MEMORYLEAK
	// memory leak data
	static MemoryLeak memoryleak;
#endif
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
	Literal operator()();

	// advance to next set member
	int operator++(int);

private:
	// not allowed
	ClauseIterator &operator=(const ClauseIterator &);

protected:
	// internal data
	BinaryTree_AVL_Iterator_InOrder<Literal> pclIter;
	BinaryTree_AVL_Iterator_InOrder<Literal> nclIter;
};

class PositiveClauseIterator {
public:
	// constructors and destructor
	PositiveClauseIterator(const Clause &);
	PositiveClauseIterator(const PositiveClauseIterator &);
	~PositiveClauseIterator();

	// reset iterator to start
	void reset();

	// check if at end of set
	int done() const;

	// return current member in set
	Literal operator()();

	// advance to next set member
	int operator++(int);

private:
	// not allowed
	PositiveClauseIterator &operator=(const PositiveClauseIterator &);

protected:
	// internal data
	BinaryTree_AVL_Iterator_InOrder<Literal> clauseIter;
};

class NegativeClauseIterator {
public:
	// constructors and destructor
	NegativeClauseIterator(const Clause &);
	NegativeClauseIterator(const NegativeClauseIterator &);
	~NegativeClauseIterator();

	// reset iterator to start
	void reset();

	// check if at end of set
	int done() const;

	// return current member in set
	Literal operator()();

	// advance to next set member
	int operator++(int);

private:
	// not allowed
	NegativeClauseIterator &operator=(const NegativeClauseIterator &);

protected:
	// internal data
	BinaryTree_AVL_Iterator_InOrder<Literal> clauseIter;
};

// answer clause iterator definition 
class ClauseAnswersIterator {
public:
	// constructors and destructor
	ClauseAnswersIterator(const Clause &);
	ClauseAnswersIterator(const ClauseAnswersIterator &);
	~ClauseAnswersIterator();

	// reset iterator to start
	void reset();

	// check if at end of set
	int done() const;

	// return current member in set
	Literal operator()();

	// advance to next set member
	int operator++(int);

private:
	// not allowed
	ClauseAnswersIterator &operator=(const ClauseAnswersIterator &);

protected:
	// internal data
	ListIterator<Literal> ansIter;
};

#endif


