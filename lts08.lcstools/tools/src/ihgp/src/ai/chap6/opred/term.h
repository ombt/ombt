#ifndef __TERM_H
#define __TERM_H
// class definition for terms in clauses

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "mystring.h"

// clause class definitions
class Term {
public:
	// constructors and destructor
	Term();
	Term(char);
	Term(const char *);
	Term(const Term &);
	Term(const String &);
	~Term();

	// assignment
	Term &operator=(const Term &);
	Term &operator=(const String &);

	// access operator
	char &operator[](int);
	const char &operator[](int) const;

	// logical operators
	int operator==(const Term &) const;
	int operator!=(const Term &) const;
	int operator<(const Term &) const;
	int operator<=(const Term &) const;
	int operator>(const Term &) const;
	int operator>=(const Term &) const;

	// substring operators
	Term operator()(int) const;
	Term operator()(int, int) const;
	String predicate() const;

	// negation operator
	Term operator~() const;
	int isNegated() const;

	// concatenation operators
	Term &operator+=(const Term &);
	friend Term operator+(const Term &, const Term &);

	// string length
	int strlen() const;

	// input and output
	friend ostream &operator<<(ostream &, const Term &);
	friend istream &operator>>(istream &, Term &);

protected:
	// internal data
	String term;
};

#endif


