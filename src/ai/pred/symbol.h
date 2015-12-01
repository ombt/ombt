#ifndef _SYMBOL_H
#define _SYMBOL_H
// symbol table classes

// headers
#include <stdlib.h>
#include <iostream>

// local headers
#include "system/Returns.h"
#include "system/Debug.h"
#include "adt/String.h"

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
	Symbol(const ombt::String &);
	Symbol(const ombt::String &, Type, int = 0);
	Symbol(const ombt::String &, const ombt::String &);
	Symbol(const ombt::String &, const ombt::String &, Type, int = 0);
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
	ombt::String getName() const {
		return(name);
	};
	ombt::String getUniqueName() const {
		return(uniquename);
	};
	int getArgs() const {
		return(argnum);
	};
	int getUsed() const {
		return(used);
	};

	// print data
	friend std::ostream &operator<<(std::ostream &, const Symbol &);

protected:
	// internal data
	ombt::String name;
	ombt::String uniquename;
	Type type;
	int argnum;
	int used;
};

#endif
