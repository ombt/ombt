#ifndef __MYSTRING_H
#define __MYSTRING_H
// string class definitions

// headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"

// string class definition
class String {
public:
	// constructors and destructor
	String();
	String(int);
	String(char);
	String(const char *);
	String(const String &);
	~String();

	// assignment
	String &operator=(const String &);
	String &operator=(const char *);

	// access operator
	char &operator[](int);
	const char &operator[](int) const;

	// logical operators
	int operator==(const String &) const;
	int operator!=(const String &) const;
	int operator<(const String &) const;
	int operator<=(const String &) const;
	int operator>(const String &) const;
	int operator>=(const String &) const;

	// substring operator
	String operator()(int) const;
	String operator()(int, int) const;

	// concatenation operators
	String &operator+=(const String &);
	String operator+(const String &) const;

	// string length
	int strlen() const;
	inline int getMaxSize() const {
		return(maxStringSize);
	}

	// casting operation
	operator const char *() const;
	operator char *();

	// input and output
	friend ostream &operator<<(ostream &, const String &);
	friend istream &operator>>(istream &, String &);
	istream &getline(istream &, char = '\n');

protected:
	int maxStringSize;
	char *buffer;
};

// string matcher class
class StringMatcher {
public:
        // constructors and destructor
	StringMatcher(const String &, const String &);
        virtual ~StringMatcher();

	// reset iterator to start
	virtual void reset();

	// check if at end of list
	virtual int done() const;

        // return data 
        virtual String operator()() const;

	// advance iterator to next match, if any
	virtual int operator++(int);

	// get location of match
	virtual int location() const;

private:
	// not allowed
	StringMatcher();
	StringMatcher(const StringMatcher &);
	StringMatcher &operator=(const StringMatcher &);

protected:
	// internal data
	unsigned int current;
	unsigned int stringLength;
	unsigned int patternLength;
	const String &string;
	const String &pattern;
};

// rabin-karp string matcher class
class StringMatcher_RabinKarp: public StringMatcher {
public:
        // constructors and destructor
	StringMatcher_RabinKarp(const String &, const String &);
        ~StringMatcher_RabinKarp();

	// reset iterator to start
	void reset();

	// advance iterator to next match, if any
	int operator++(int);

private:
	// not allowed
	StringMatcher_RabinKarp();
	StringMatcher_RabinKarp(const StringMatcher_RabinKarp &);
	StringMatcher &operator=(const StringMatcher_RabinKarp &);

protected:
	// additional internal data
	unsigned int powerOfTwo;
	unsigned int patternValue;
	unsigned int stringValue;
};

#endif

