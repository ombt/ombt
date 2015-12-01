#ifndef __LITERAL_H
#define __LITERAL_H
// literal and term classes for resolution

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "mylist.h"
#include "mystring.h"
#include "map.h"
#include "uniquename.h"

// forward declarations
class Terms;
class Literal;
class Clause;
class Substitution;
class Substitutions;

// term class definitions
class Terms {
public:
	// friends
	friend class Clause;
	friend class Substitution;
	friend class Substitutions;
	friend int unify(Terms &, Terms &, Substitutions &);
	friend int unify(Literal &, Literal &, Substitutions &);
	friend int equal(const Terms &, const Terms &);
	friend int equal(const Literal &, const Literal &);

	// type of symbol
	enum Type {
		Constant,
		Function,
		Variable,
		QuotedString,
		Number,
		Unknown
	};

	// constructors and destructor
	Terms();
	Terms(const Terms &);
	Terms(const String &);
	~Terms();

	// copy data
	Terms *copy() const;

	// assignment operator
	Terms &operator=(const Terms &);
	Terms &operator=(const String &);

	// comparison operators
	//
	// WARNING: the comparison operators separates 
	// literals into equivalence classes, there may
	// be more than one element in the equivalence
	// class !!! for absolute equality, you must
	// check if the terms unify or not.
	//
	int operator==(const Terms &) const;
	int operator!=(const Terms &) const;
	int operator<(const Terms &) const;
	int operator<=(const Terms &) const;
	int operator>(const Terms &) const;
	int operator>=(const Terms &) const;

	// utility functions
	int parse(const String &);
	int parse(StringTokens &);

	// occurs check
	int occurs(const Terms &) const;

	// access operators
	Type getType() const {
		return(type);
	}
	String getValue() const {
		return(value);
	};
	String getString() const {
		return(inputstring);
	}

	// rename variables
	int renameVariables();
	int renameVariables(Map<String, String> &);

	// convert term to a string
	operator String ();
	operator String () const;
	String convertToString(String &) const;

	// print data
	friend ostream &operator<<(ostream &, const Terms &);

protected:
	// internal data
	String value;
	String inputstring;
	Type type;
	int argnum;
	List<Terms * > *pargs;
};

// literal class definitions
class Literal {
public:
	// friends
	friend class Clause;
	friend class Substitution;
	friend class Substitutions;
	friend int unify(Terms &, Terms &, Substitutions &);
	friend int unify(Literal &, Literal &, Substitutions &);
	friend int equal(const Terms &, const Terms &);
	friend int equal(const Literal &, const Literal &);

	// type of symbol
	enum Type {
		Equal,
		Constant,
		Function,
		LogicalConstant,
		Unknown
	};

	// constructors and destructor
	Literal();
	Literal(const Literal &);
	Literal(const String &);
	~Literal();

	// copy data
	Literal *copy() const;

	// assignment operator
	Literal &operator=(const Literal &);
	Literal &operator=(const String &);

	// comparison operators
	//
	// WARNING: the comparison operators separates 
	// literals into equivalence classes, there may
	// be more than one element in the equivalence
	// class !!! for absolute equality, you must
	// check if the terms unify or not.
	//
	int operator==(const Literal &) const;
	int operator!=(const Literal &) const;
	int operator<(const Literal &) const;
	int operator<=(const Literal &) const;
	int operator>(const Literal &) const;
	int operator>=(const Literal &) const;

	// negation functions
	int isNegated() const;
	Literal operator~() const;

	// utility functions
	int parse(const String &);
	int parse(StringTokens &);

	// access operators
	Type getType() const {
		return(type);
	}
	String getValue() const {
		return(value);
	};
	String getString() const {
		return(inputstring);
	}

	// rename variables
	int renameVariables();
	int renameVariables(Map<String, String> &);

	// convert literal to a string
	operator String ();
	operator String () const;
	String convertToString(String &) const;

	// print data
	friend ostream &operator<<(ostream &, const Literal &);

protected:
	// internal data
	String value;
	String inputstring;
	Type type;
	int argnum;
	int negated;
	List<Terms * > *pargs;
};

#endif
