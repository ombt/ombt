#ifndef _SYMBOL_H
#define _SYMBOL_H
// symbol table classes

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "mystring.h"

// symbol class definitions
class Symbol {
public:
	// type of symbol
	enum Type {
		PredicateFunction,
		PredicateConstant,
		Function,
		Variable,
		Constant,
		UniversalVariable,
		ExistentialVariable,
		Unknown
	};

	// constructors and destructor
	Symbol();
	Symbol(const Symbol &);
	Symbol(const String &);
	Symbol(const String &, Type, int = 0);
	Symbol(const String &, const String &);
	Symbol(const String &, const String &, Type, int = 0);
	~Symbol();

	// assignment operator
	Symbol &operator=(const Symbol &);

	// comparison operators
	int operator==(const Symbol &) const;
	int operator!=(const Symbol &) const;
	int operator<(const Symbol &) const;
	int operator<=(const Symbol &) const;
	int operator>(const Symbol &) const;
	int operator>=(const Symbol &) const;

	// increment operators
	Symbol &operator++();
	Symbol &operator++(int);

	// access operators
	Type getType() const {
		return(type);
	}
	String getName() const {
		return(name);
	};
	String getUniqueName() const {
		return(uniquename);
	};
	int getArgs() const {
		return(argnum);
	};
	int getUsed() const {
		return(used);
	};

	// print data
	friend ostream &operator<<(ostream &, const Symbol &);

protected:
	// internal data
	String name;
	String uniquename;
	Type type;
	int argnum;
	int used;
};

#endif
