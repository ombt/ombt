#ifndef _TERM_H
#define _TERM_H
// term class for resolution

// headers
#include <stdlib.h>
#include <iostream>

// local headers
#include "system/Returns.h"
#include "system/Debug.h"
#include "adt/List.h"
#include "adt/String.h"
#include "adt/StringTokens.h"

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

	// unify two terms, if possible
	int unify(const Term &) const;

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

#endif
