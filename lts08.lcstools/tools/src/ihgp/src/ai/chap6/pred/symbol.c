// symbol class definitions

// headers
#include "symbol.h"

// constructors and destructor
Symbol::Symbol():
	name(), type(Unknown), argnum(0), used(0)
{
	// nothing to do
}

Symbol::Symbol(const Symbol &src):
	name(src.name), uniquename(src.uniquename),
	type(src.type), argnum(src.argnum), used(src.used)
{
	// nothing to do
}

Symbol::Symbol(const String &nm):
	name(nm), uniquename(""), type(Unknown), argnum(0), used(0)
{
	// nothing to do
}

Symbol::Symbol(const String &nm, Type t, int an):
	name(nm), uniquename(""), type(t), argnum(an), used(0)
{
	// nothing to do
}

Symbol::Symbol(const String &nm, const String &unm):
	name(nm), uniquename(unm), type(Unknown), argnum(0), used(0)
{
	// nothing to do
}

Symbol::Symbol(const String &nm, const String &unm, Type t, int an):
	name(nm), uniquename(unm), type(t), argnum(an), used(0)
{
	// nothing to do
}

Symbol::~Symbol()
{
	// nothing to do
}

// assignment operator
Symbol &
Symbol::operator=(const Symbol &rhs)
{
	if (this != &rhs)
	{
		name = rhs.name;
		uniquename = rhs.uniquename;
		type = rhs.type;
		argnum = rhs.argnum;
		used = rhs.used;
	}
	return(*this);
}

// comparison operators
int
Symbol::operator==(const Symbol &rhs) const
{
	return(name == rhs.name);
}

int
Symbol::operator!=(const Symbol &rhs) const
{
	return(name != rhs.name);
}

int
Symbol::operator<(const Symbol &rhs) const
{
	return(name < rhs.name);
}

int
Symbol::operator<=(const Symbol &rhs) const
{
	return(name <= rhs.name);
}

int
Symbol::operator>(const Symbol &rhs) const
{
	return(name > rhs.name);
}

int
Symbol::operator>=(const Symbol &rhs) const
{
	return(name >= rhs.name);
}

// increment operators
Symbol &
Symbol::operator++()
{
	used++;
	return(*this);
}

Symbol &
Symbol::operator++(int)
{
	used++;
	return(*this);
}

// output operator
ostream &
operator<<(ostream &os, const Symbol &sym)
{
	os << "Symbol operator<< ..." << endl;
	os << "symbol name  : " << sym.name << endl;
	os << "symbol uname : " << sym.uniquename << endl;
	os << "symbol type  : ";
	switch (sym.type)
	{
	case Symbol::PredicateFunction:
		os << "Predicate Function";
		break;
	case Symbol::PredicateConstant:
		os << "Predicate Constant";
		break;
	case Symbol::Constant:
		os << "Constant";
		break;
	case Symbol::Function:
		os << "Function";
		break;
	case Symbol::Variable:
		os << "Variable";
		break;
	case Symbol::UniversalVariable:
		os << "Universal Variable";
		break;
	case Symbol::ExistentialVariable:
		os << "Existential Variable";
		break;
	case Symbol::Unknown:
		os << "Unknown";
		break;
	default:
		MustBeTrue(0);
	}
	os << endl;
	os << "symbol args  : " << sym.argnum << endl;
	os << "symbol used  : " << sym.used << endl;
	return(os);
}

