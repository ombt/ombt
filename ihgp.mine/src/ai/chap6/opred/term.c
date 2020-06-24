// Term functions for term class

// headers
#include "term.h"

// string constructors and destructor
Term::Term():
	term()
{
	// nothing to do
}

Term::Term(char c):
	term(c)
{
	// nothing to do
}

Term::Term(const char *pc):
	term(pc)
{
	// nothing to do
}

Term::Term(const Term &a):
	term(a.term)
{
	// nothing to do
}

Term::Term(const String &s):
	term(s)
{
	// nothing to do
}

Term::~Term()
{
	// nothing to do
}

// assignment operators
Term &
Term::operator=(const Term &rhs)
{
	// check for self-assignment
	if (this != &rhs)
		term = rhs.term;
	return(*this);
}

Term &
Term::operator=(const String &rhs)
{
	term = rhs;
	return(*this);
}

// access operators
char &
Term::operator[](int idx)
{
	return(term[idx]);
}

const char &
Term::operator[](int idx) const
{
	return(term[idx]);
}

// logical operators
int
Term::operator==(const Term &rhs) const
{
	return(term == rhs.term);
}

int
Term::operator!=(const Term &rhs) const
{
	return(term != rhs.term);
}

int
Term::operator<(const Term &rhs) const
{
	return(term < rhs.term);
}

int
Term::operator<=(const Term &rhs) const
{
	return(term <= rhs.term);
}

int
Term::operator>(const Term &rhs) const
{
	return(term > rhs.term);
}

int
Term::operator>=(const Term &rhs) const
{
	return(term >= rhs.term);
}

// substring operator
Term
Term::operator()(int start) const
{
	return(Term(term(start)));
}

Term
Term::operator()(int start, int length) const
{
	return(Term(term(start, length)));
}

String
Term::predicate() const
{
	for (int ic = 0; ic < term.strlen(); ic++)
	{
		if (term[ic] == '(')
			break;
	}
	if (ic >= term.strlen())
	{
		return(term);
	}
	else
	{
		return(term(0, ic));
	}
}

// negation operator
Term
Term::operator~() const
{
	if (term(0,1) == String("~"))
		return(Term(term(1)));
	else
		return(Term(String("~") + term));
}

int
Term::isNegated() const
{
	if (term(0,1) == String("~"))
		return(1);
	else
		return(0);
}

// concatenation operators
Term &
Term::operator+=(const Term &rhs)
{
	term += rhs.term;
	return(*this);
}

Term
operator+(const Term &lhs, const Term &rhs)
{
	return(Term(lhs) += rhs);
}

// string length
int
Term::strlen() const
{
	return(term.strlen());
}

// input and output
ostream &
operator<<(ostream &os, const Term &s)
{
	os << s.term;
	return(os);
}
 
istream &
operator>>(istream &is, Term &s)
{
	is >> s.term;
	return(is);
}

