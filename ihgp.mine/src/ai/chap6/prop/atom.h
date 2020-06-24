#ifndef __ATOM_H
#define __ATOM_H
// class definition for atoms in clauses

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "mystring.h"

// clause class definitions
class Atom {
public:
	// constructors and destructor
	Atom();
	Atom(char);
	Atom(const char *);
	Atom(const Atom &);
	Atom(const String &);
	~Atom();

	// assignment
	Atom &operator=(const Atom &);
	Atom &operator=(const String &);

	// access operator
	char &operator[](int);
	const char &operator[](int) const;

	// logical operators
	int operator==(const Atom &) const;
	int operator!=(const Atom &) const;
	int operator<(const Atom &) const;
	int operator<=(const Atom &) const;
	int operator>(const Atom &) const;
	int operator>=(const Atom &) const;

	// substring operators
	Atom operator()(int) const;
	Atom operator()(int, int) const;

	// negation operator
	Atom operator~() const;
	int isNegated() const;

	// concatenation operators
	Atom &operator+=(const Atom &);
	friend Atom operator+(const Atom &, const Atom &);

	// string length
	int strlen() const;

	// input and output
	friend ostream &operator<<(ostream &, const Atom &);
	friend istream &operator>>(istream &, Atom &);

protected:
	// internal data
	String atom;
};

#endif


