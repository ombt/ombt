#ifndef __SUBSTITUTION_H
#define __SUBSTITUTION_H
// header for defining substitution class

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "mylist.h"
#include "mystring.h"
#include "literal.h"
#include "clause.h"

// forward declarations
class Substitution;
class Substitutions;

// substitution class
class Substitution {
public:
	// friends
	friend class Substitutions;
	friend int unify(const Literal &, const Literal &);
	friend int unify(const Terms &, const Terms &);

	// constructors and destructor
	Substitution();
	Substitution(const Substitution &);
	Substitution(const Terms &, const String &);
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
	int applyTo(Terms &) const;
	int applyTo(Literal &) const;
	int applyTo(Clause &) const;

	// access data
	String getVariable() const {
		return(variable);
	}
	void setVariable(const String &v) {
		variable = v;
	}
	Terms getTerm() const {
		return(term);
	}
	void setTerm(const Terms &t) {
		term = t;
	}

	// check of variable is equal to term
	int isDegenerate() const;

	// output function
	friend ostream &operator<<(ostream &, const Substitution &);

protected:
	// internal data
	String variable;
	Terms term;
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
	int applyTo(Terms &) const;
	int applyTo(Literal &) const;
	int applyTo(Clause &) const;

	// output function
	friend ostream &operator<<(ostream &, const Substitutions &);

protected:
	// internal data
	List<Substitution> subs;
};

#endif
