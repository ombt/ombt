#ifndef __CLAUSE_H
#define __CLAUSE_H
// class definition of clauses

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "mylist.h"
#include "mystring.h"
#include "binaryTree.h"
#include "tuple.h"
#include "map.h"
#include "literal.h"

// forward declarations
class ClauseIterator;
class PositiveClauseIterator;
class NegativeClauseIterator;

// clause class definitions
class Clause {
public:
	// friend classes
	friend class ClauseIterator;
	friend class PositiveClauseIterator;
	friend class NegativeClauseIterator;

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
	int retrieve(Literal &, int) const;
	int update(const Literal &, int);
	int remove(Literal &, int);

	// update an equivalence set in a clause
	int insertEqSet(const Literal &, const List<Literal> &);
	int retrieveEqSet(const Literal &, List<Literal> &) const;
	int updateEqSet(const Literal &, const List<Literal> &);
	int removeEqSet(Literal &, List<Literal> &);

	// parse string to a literal
	int parse(const List<String> &);

	// clear clause
	void clear();

	// tests to run on a clause
	int isInClause(const Literal &) const;
	int isEmpty() const;

	// return count of classes and total members
	int getTotalMembers() const;
	int getTotalSets() const;
	int getMembersInSet(const Literal &) const;

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

	// get maximal equivalent set or literal
	int getMaximalSet(Literal &, List<Literal> &) const;
	int getMaximalLiteral(Literal &) const;
	int getMaximalPositiveSet(Literal &, List<Literal> &) const;
	int getMaximalPositiveLiteral(Literal &) const;
	int getMaximalNegativeSet(Literal &, List<Literal> &) const;
	int getMaximalNegativeLiteral(Literal &) const;

	// rename all variables in clause
	int renameVariables();

	// convert clause to a string
	operator String ();
	operator String () const;
	String convertToString(String &) const;

	// combine two clauses
	friend Clause operator+(const Clause &, const Clause &);

	// dump data
	friend ostream &operator<<(ostream &, const Clause &);

protected:
	// utility functions
	int compare(const BinaryTree<Tuple<Literal, List<Literal> > > &,
		const BinaryTree<Tuple<Literal, List<Literal> > > &) const;
	int insert(const Literal &, 
		BinaryTree<Tuple<Literal, List<Literal> > > &);
	int retrieve(Literal &, int,
		const BinaryTree<Tuple<Literal, List<Literal> > > &) const;
	int update(const Literal &, int,
		BinaryTree<Tuple<Literal, List<Literal> > > &);
	int remove(Literal &, int,
		BinaryTree<Tuple<Literal, List<Literal> > > &);
	void updateType();

protected:
	// internal data
	unsigned int depth, number;
	Type type;
	String inputstring;
	int setOfSupport;
	int partOfConclusion;
	BinaryTree<Tuple<Literal, List<Literal> > > positiveClause;
	BinaryTree<Tuple<Literal, List<Literal> > > negativeClause;
};

// clause iterator definition - returns equivalence classes
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
	Literal key();
	List<Literal> data();
	Tuple<Literal, List<Literal> > operator()();

	// advance to next set member
	int operator++(int);

private:
	// not allowed
	ClauseIterator &operator=(const ClauseIterator &);

protected:
	// internal data
	BinaryTreeIterator_InOrder<Tuple<Literal, List<Literal> > > pclIter;
	BinaryTreeIterator_InOrder<Tuple<Literal, List<Literal> > > nclIter;
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
	Literal key();
	List<Literal> data();
	Tuple<Literal, List<Literal> > operator()();

	// advance to next set member
	int operator++(int);

private:
	// not allowed
	PositiveClauseIterator &operator=(const PositiveClauseIterator &);

protected:
	// internal data
	BinaryTreeIterator_InOrder<Tuple<Literal, List<Literal> > > clauseIter;
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
	Literal key();
	List<Literal> data();
	Tuple<Literal, List<Literal> > operator()();

	// advance to next set member
	int operator++(int);

private:
	// not allowed
	NegativeClauseIterator &operator=(const NegativeClauseIterator &);

protected:
	// internal data
	BinaryTreeIterator_InOrder<Tuple<Literal, List<Literal> > > clauseIter;
};

#endif


