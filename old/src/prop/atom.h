#ifndef __ATOM_H
#define __ATOM_H
// class definition for atoms in clauses

// headers
#include <stdlib.h>
#include <iostream>

// local headers
#include <Returns.h>
#include <Debug.h>
#include <adt/String.h>

// clause class definitions
class Atom {
public:
	// constructors and destructor
	Atom();
	Atom(char);
	Atom(const char *);
	Atom(const Atom &);
	Atom(const ombt::String &);
	~Atom();

	// assignment
	Atom &operator=(const Atom &);
	Atom &operator=(const ombt::String &);

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
	friend std::ostream &operator<<(std::ostream &, const Atom &);
	friend std::istream &operator>>(std::istream &, Atom &);

protected:
	// internal data
	ombt::String atom;
};

#endif


