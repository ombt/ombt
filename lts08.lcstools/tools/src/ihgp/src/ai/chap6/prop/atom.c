// functions for atom class

// headers
#include "atom.h"

// string constructors and destructor
Atom::Atom():
	atom()
{
	// nothing to do
}

Atom::Atom(char c):
	atom(c)
{
	// nothing to do
}

Atom::Atom(const char *pc):
	atom(pc)
{
	// nothing to do
}

Atom::Atom(const Atom &a):
	atom(a.atom)
{
	// nothing to do
}

Atom::Atom(const String &s):
	atom(s)
{
	// nothing to do
}

Atom::~Atom()
{
	// nothing to do
}

// assignment operators
Atom &
Atom::operator=(const Atom &rhs)
{
	// check for self-assignment
	if (this != &rhs)
		atom = rhs.atom;
	return(*this);
}

Atom &
Atom::operator=(const String &rhs)
{
	atom = rhs;
	return(*this);
}

// access operators
char &
Atom::operator[](int idx)
{
	return(atom[idx]);
}

const char &
Atom::operator[](int idx) const
{
	return(atom[idx]);
}

// logical operators
int
Atom::operator==(const Atom &rhs) const
{
	return(atom == rhs.atom);
}

int
Atom::operator!=(const Atom &rhs) const
{
	return(atom != rhs.atom);
}

int
Atom::operator<(const Atom &rhs) const
{
	return(atom < rhs.atom);
}

int
Atom::operator<=(const Atom &rhs) const
{
	return(atom <= rhs.atom);
}

int
Atom::operator>(const Atom &rhs) const
{
	return(atom > rhs.atom);
}

int
Atom::operator>=(const Atom &rhs) const
{
	return(atom >= rhs.atom);
}

// substring operator
Atom
Atom::operator()(int start) const
{
	return(Atom(atom(start)));
}

Atom
Atom::operator()(int start, int length) const
{
	return(Atom(atom(start, length)));
}

// negation operator
Atom
Atom::operator~() const
{
	if (atom(0,1) == String("~"))
		return(Atom(atom(1)));
	else
		return(Atom(String("~") + atom));
}

int
Atom::isNegated() const
{
	if (atom(0,1) == String("~"))
		return(1);
	else
		return(0);
}

// concatenation operators
Atom &
Atom::operator+=(const Atom &rhs)
{
	atom += rhs.atom;
	return(*this);
}

Atom
operator+(const Atom &lhs, const Atom &rhs)
{
	return(Atom(lhs) += rhs);
}

// string length
int
Atom::strlen() const
{
	return(atom.strlen());
}

// input and output
ostream &
operator<<(ostream &os, const Atom &s)
{
	os << s.atom;
	return(os);
}
 
istream &
operator>>(istream &is, Atom &s)
{
	is >> s.atom;
	return(is);
}

