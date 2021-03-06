#ifndef _TERM_TREE_H
#define _TERM_TREE_H
// term class for resolution, based on a tree structure

// headers
#include <stdlib.h>
#include <iostream>

// local headers
#include "system/Returns.h"
#include "system/Debug.h"
#include "adt/List.h"
#include "adt/String.h"
#include "adt/StringTokens.h"

using namespace std;
using namespace ombt;

// forward declaration
class Substitution;
class Substitutions;

// symbol class definitions
class Term {
public:
	// friends
	friend class Substitution;
	friend class Substitutions;
	friend int unify(Term &, Term &, Substitutions &);
	friend int compare(const Term &, const Term &);
	friend int CompareAndUnify(Term &, Term &, Substitutions &);

	// type of symbol
	enum Type {
		PredicateConstant,
		PredicateFunction,
		Constant,
		Function,
		Variable,
		QuotedString,
		Number,
		LogicalConstant,
		Unknown
	};

	// constructors and destructor
	Term();
	Term(const Term &);
	Term(const ombt::String &);
	~Term();

	// copy data
	Term *copy() const;

	// assignment operator
	Term &operator=(const Term &);
	Term &operator=(const ombt::String &);

	// comparison operators
	int operator==(const Term &) const;
	int operator!=(const Term &) const;
	int operator<(const Term &) const;
	int operator<=(const Term &) const;
	int operator>(const Term &) const;
	int operator>=(const Term &) const;

	// parse a list representation
	int parse(const ombt::String &);
	int parse(ombt::StringTokens &);

	// negation functions
	int isNegated() const;
	Term operator~() const;

	// check if two terms are equal
	int equals(const Term &) const;

	// does a term appear in another, only for variables
	int occurs(const Term &) const;

	// access operators
	Type getType() const {
		return(type);
	}
	ombt::String getValue() const {
		return(value);
	};

	// print data
	friend std::ostream &operator<<(std::ostream &, const Term &);

protected:
	// internal data
	ombt::String value;
	Type type;
	int argnum;
	ombt::List<Term *> *pargs;
};

// substitution class
class Substitution {
public:
	// friends
	friend class Substitutions;
	friend int unify(Term &, Term &, Substitutions &);
	friend int CompareAndUnify(Term &, Term &, Substitutions &);

	// constructors and destructor
	Substitution();
	Substitution(const Substitution &);
	Substitution(const Term &, const ombt::String &);
	~Substitution();

	// assignment operator
	Substitution &operator=(const Substitution &);

	// relational operators
	int operator==(const Substitution &) const;
	int operator!=(const Substitution &) const;
	int operator<(const Substitution &) const;
	int operator<=(const Substitution &) const;
	int operator>(const Substitution &) const;
	int operator>=(const Substitution &) const;

	// composition
	Substitution operator*(const Substitution &) const;

	// apply the substitution to a term
	int applyTo(Term &) const;

	// access data
	ombt::String getVariable() const {
		return(variable);
	}
	void setVariable(const ombt::String &v) {
		variable = v;
	}
	Term getTerm() const {
		return(term);
	}
	void setTerm(const Term &t) {
		term = t;
	}

	// check of variable is equal to term
	int isDegenerate() const;

	// output function
	friend std::ostream &operator<<(std::ostream &, const Substitution &);

protected:
	// internal data
	ombt::String variable;
	Term term;
};

// substitutions class
class Substitutions {
public:
	// constructors and destructor
	Substitutions();
	Substitutions(const Substitutions &);
	~Substitutions();

	// assignment operator
	Substitutions &operator=(const Substitutions &);

	// relational operators
	int operator==(const Substitutions &) const;
	int operator!=(const Substitutions &) const;
	int operator<(const Substitutions &) const;
	int operator<=(const Substitutions &) const;
	int operator>(const Substitutions &) const;
	int operator>=(const Substitutions &) const;

	// accress substitutions
	int insert(const Substitution &);
	void clear();
	int isEmpty() const;

	// composition
	Substitutions operator*(const Substitution &) const;
	Substitutions operator*(const Substitutions &) const;

	// apply the substitution to a term
	int applyTo(Term &) const;

	// output function
	friend std::ostream &operator<<(std::ostream &, const Substitutions &);

protected:
	// internal data
	ombt::List<Substitution *> *psubs;
};

// unification function
int unify(Term &, Term &, Substitutions &);

// ordering function, imposes an order on terms
int compare(const Term &, const Term &);

// comparison function, return a unification when possible
int ucompare(const Term &, const Term &);
int CompareAndUnify(Term &, Term &, Substitutions &);

#endif
