// symbol class definitions

// headers
#include "symbol.h"

// static variables
int Symbol::Ccount = 0;
int Symbol::Dcount = 0;

// constructors and destructor
Symbol::Symbol():
	name(), uniquename(), originalname(), 
	type(Unknown), argnum(0), used(0)
{
	Ccount++;
}

Symbol::Symbol(const Symbol &src):
	name(src.name), uniquename(src.uniquename),
	originalname(src.originalname),
	type(src.type), argnum(src.argnum), used(src.used)
{
	Ccount++;
}

Symbol::Symbol(const String &nm):
	name(nm), uniquename(), originalname(), 
	type(Unknown), argnum(0), used(0)
{
	Ccount++;
}

Symbol::Symbol(const String &nm, SymbolType t, int an):
	name(nm), uniquename(), originalname(),
	type(t), argnum(an), used(0)
{
	Ccount++;
}

Symbol::Symbol(const String &nm, const String &unm):
	name(nm), uniquename(unm), originalname(), 
	type(Unknown), argnum(0), used(0)
{
	Ccount++;
}

Symbol::Symbol(const String &nm, const String &unm, SymbolType t, int an):
	name(nm), uniquename(unm), originalname(),
	type(t), argnum(an), used(0)
{
	Ccount++;
}

Symbol::Symbol(const String &nm, const String &unm, const String &onm,
	SymbolType t, int an, int u):
	name(nm), uniquename(unm), originalname(onm),
	type(t), argnum(an), used(u)
{
	Ccount++;
}

Symbol::~Symbol()
{
	Dcount++;
}

// assignment operator
Symbol &
Symbol::operator=(const Symbol &rhs)
{
	if (this != &rhs)
	{
		name = rhs.name;
		uniquename = rhs.uniquename;
		originalname = rhs.originalname;
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
	os << "symbol oname : " << sym.originalname << endl;
	os << "symbol type  : ";
	switch (sym.type)
	{
	case PredicateFunction:
		os << "Predicate Function";
		break;
	case PredicateConstant:
		os << "Predicate Constant";
		break;
	case Constant:
		os << "Constant";
		break;
	case Function:
		os << "Function";
		break;
	case Variable:
		os << "Variable";
		break;
	case UniversalVariable:
		os << "Universal Variable";
		break;
	case ExistentialVariable:
		os << "Existential Variable";
		break;
	case Unknown:
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

