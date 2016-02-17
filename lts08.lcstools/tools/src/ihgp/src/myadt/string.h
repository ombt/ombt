#ifndef __STRING_H
#define __STRING_H
// string class definitions

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>

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
	char &operator[](int) const;

	// logical operators
	int operator==(const String &) const;
	int operator!=(const String &) const;
	int operator<(const String &) const;
	int operator<=(const String &) const;
	int operator>(const String &) const;
	int operator>=(const String &) const;

	// substring operator
	String operator()(int, int) const;

	// concatenation operators
	String &operator+=(const String &);
#if 0
	String operator+(const String &) const;
#else
	friend String operator+(const String &, const String &);
#endif

	// string length
	int strlen() const;

	// casting operation
	operator const char *() const;

	// input and output
	friend ostream &operator<<(ostream &, const String &);
	friend istream &operator>>(istream &, String &);

protected:
	// internal data
	int bufferLength;
	char *buffer;
};

#endif

