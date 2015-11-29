#ifndef __SUBSTITUTION_H
#define __SUBSTITUTION_H
// header for defining substitution class

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "list.h"
#include "mystring.h"
#include "term.h"

// forward declarations
class Term;
class Substitution;
class Substitutions;

// substitution class
class Substitution {
public:
	// friends
	friend class Substitutions;
	friend int unify(const Term &, const Term &);

	// constructors and destructor
	Substitution();
	Substitution(const Substitution &);
	Substitution(const Term &, const String &);
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
	String getVariable() const {
		return(variable);
	}
	void setVariable(const String &v) {
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
	friend ostream &operator<<(ostream &, const Substitution &);

protected:
	// internal data
	String variable;
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
	friend ostream &operator<<(ostream &, const Substitutions &);

protected:
	// internal data
	List<Substitution *> *psubs;
};

#endif
