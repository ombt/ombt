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
#include "index.h"
#include "memoryleak.h"
#include "varnames.h"

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

	// comparison operators using index
	int operator==(const Terms &) const;
	int operator!=(const Terms &) const;
	int operator<(const Terms &) const;
	int operator<=(const Terms &) const;
	int operator>(const Terms &) const;
	int operator>=(const Terms &) const;

	// comparison operators, not using index
	int eq(const Terms &) const;
	int unify_eq(const Terms &) const;
	int ne(const Terms &) const;
	int unify_ne(const Terms &) const;
	int lt(const Terms &) const;
	int le(const Terms &) const;
	int gt(const Terms &) const;
	int ge(const Terms &) const;

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
	}
	String getString() const {
		return(inputstring);
	}

	// accessing index
	long getIndex() const {
		return(index);
	}
	void setIndex(long idx) {
		index = idx;
	}

	// rename variables
	int renameVariables();
	int renameVariables(Map<String, String> &);

	// convert term to a string
	operator String ();
	operator String () const;
	String convertToString(String &) const;

	// basic statistics for a term
	void basicStats(int &, int &, int &, int &) const;

#ifdef MEMORYLEAK
	// overload new and delete for memory-leak detection
	void *operator new(size_t);
	void operator delete(void *);
#endif

	// print data
	friend ostream &operator<<(ostream &, const Terms &);

protected:
	// internal data
	String value;
	String inputstring;
	Type type;
	int argnum;
	List<Terms * > *pargs;
	long index;

public:
#ifdef MEMORYLEAK
	// memory leak data
	static MemoryLeak memoryleak;
#endif
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

	// comparison operators using index
	int operator==(const Literal &) const;
	int operator!=(const Literal &) const;
	int operator<(const Literal &) const;
	int operator<=(const Literal &) const;
	int operator>(const Literal &) const;
	int operator>=(const Literal &) const;

	// comparison operators not using index
	int eq(const Literal &) const;
	int unify_eq(const Literal &) const;
	int ne(const Literal &) const;
	int unify_ne(const Literal &) const;
	int lt(const Literal &) const;
	int le(const Literal &) const;
	int gt(const Literal &) const;
	int ge(const Literal &) const;

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

	// accessing index
	long getIndex() const {
		return(index);
	}
	void setIndex(long idx) {
		index = idx;
	}

	// rename variables
	int renameVariables();
	int renameVariables(Map<String, String> &);

	// convert literal to a string
	operator String ();
	operator String () const;
	String convertToString(String &) const;

	// basic statistics for a literal
	void basicStats(int &, int &, int &, 
			int &, int &, int &, int &) const;

#ifdef MEMORYLEAK
	// overload new and delete for memory-leak detection
	void *operator new(size_t);
	void operator delete(void *);
#endif

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
	long index;

public:
#ifdef MEMORYLEAK
	// memory leak data
	static MemoryLeak memoryleak;
#endif
};

#endif
