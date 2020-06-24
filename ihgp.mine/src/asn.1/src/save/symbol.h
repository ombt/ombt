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
#include "symboltypes.h"

// symbol class definitions
class Symbol {
public:
	// constructors and destructor
	Symbol();
	Symbol(const Symbol &);
	Symbol(const String &);
	Symbol(const String &, SymbolType, int = 0);
	Symbol(const String &, const String &);
	Symbol(const String &, const String &, SymbolType, int = 0);
	Symbol(const String &, const String &, const String &,
		SymbolType, int, int);
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
	SymbolType getType() const {
		return(type);
	}
	String getName() const {
		return(name);
	};
	String getUniqueName() const {
		return(uniquename);
	};
	String getOriginalName() const {
		return(originalname);
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
	String originalname;
	SymbolType type;
	int argnum;
	int used;

public:
	static int Ccount;
	static int Dcount;
};

#endif
