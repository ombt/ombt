// functions for string class

// headers
#include <stdlib.h>
#include <iostream.h>
#include <string.h>
#include <assert.h>

// local headers
#include "string.h"

// string constructors and destructor
String::String():
	bufferLength(1), buffer(new char [bufferLength])
{
	// check if buffer was allocated
        assert(buffer != 0);

	// null terminate
	*buffer = 0;
}

String::String(int buflen):
	bufferLength(buflen), buffer(new char [bufferLength])
{
	// check if buffer was allocated
        assert(bufferLength > 0 && buffer != 0);

	// null terminate
	*buffer = 0;
}

String::String(char c):
	bufferLength(2), buffer(new char [bufferLength])
{
	// check if buffer was allocated
        assert(buffer != 0);

	// store character and null terminate
	buffer[0] = c;
	buffer[1] = 0;
}

String::String(const char *pc):
	bufferLength(::strlen(pc)+1), buffer(new char [bufferLength])
{
	// check if buffer was allocated
        assert(bufferLength > 0 && buffer != 0);

	// copy string
	::strcpy(buffer, pc);
}

String::String(const String &s):
	bufferLength(s.bufferLength), buffer(new char [bufferLength])
{
	// check if buffer was allocated
        assert(bufferLength > 0 && buffer != 0);

	// copy string
	::strcpy(buffer, s.buffer);
}

String::~String()
{
	// clear everything
	delete [] buffer;
	bufferLength = 0;
}

// assignment operators
String &
String::operator=(const String &rhs)
{
	// check for self-assignment
	if (this == &rhs) return(*this);

	// is buffer long enough
	int newBufferLength = ::strlen(rhs.buffer)+1;
	if (newBufferLength > bufferLength)
	{
		// delete old buffer and allocate new one
		delete [] buffer;
		bufferLength = newBufferLength;
		buffer = new char [bufferLength];
        	assert(bufferLength > 0 && buffer != 0);
	}

	// copy string
	::strcpy(buffer, rhs.buffer);

	// return modified string
	return(*this);
}

String &
String::operator=(const char *rhs)
{
	// check for null pointers
	assert(rhs != 0);

	// check for self-assignment
	if (buffer == rhs) return(*this);

	// is buffer long enough
	int newBufferLength = ::strlen(rhs)+1;
	if (newBufferLength > bufferLength)
	{
		// delete old buffer and allocate new one
		delete [] buffer;
		bufferLength = newBufferLength;
		buffer = new char [bufferLength];
        	assert(bufferLength > 0 && buffer != 0);
	}

	// copy string
	::strcpy(buffer, rhs);

	// return modified string
	return(*this);
}

// access operators
char &
String::operator[](int idx)
{
	assert(0 <= idx && idx < bufferLength);
	return(buffer[idx]);
}

char &
String::operator[](int idx) const
{
	assert(0 <= idx && idx < bufferLength);
	return(buffer[idx]);
}

// logical operators
int
String::operator==(const String &rhs) const
{
	return(::strcmp(buffer, rhs.buffer) == 0);
}

int
String::operator!=(const String &rhs) const
{
	return(::strcmp(buffer, rhs.buffer) != 0);
}

int
String::operator<(const String &rhs) const
{
	return(::strcmp(buffer, rhs.buffer) < 0);
}

int
String::operator<=(const String &rhs) const
{
	return(::strcmp(buffer, rhs.buffer) <= 0);
}

int
String::operator>(const String &rhs) const
{
	return(::strcmp(buffer, rhs.buffer) > 0);
}

int
String::operator>=(const String &rhs) const
{
	return(::strcmp(buffer, rhs.buffer) >= 0);
}

// substring operator
String
String::operator()(int start, int length) const
{
	// check start of substring
	assert(0 <= start && start < bufferLength);

	// calculate length of string to copy
	assert(0 <= length);
	if ((start+length) > bufferLength) 
		length = bufferLength - start;

	// copy substring
	String substr(length+1);
	::strncpy(substr.buffer, buffer+start, length);

	// return substring
	return(substr);
}

// concatenation operators
String &
String::operator+=(const String &rhs)
{
	// check of current buffer is long enough for new buffer
	int blen = ::strlen(buffer);
	int rhsblen = ::strlen(rhs.buffer);
	int newBufferLength = blen + rhsblen + 1;
	if (newBufferLength > bufferLength)
	{
		// allocate a new buffer 
		char *newBuffer = new char [newBufferLength];
		assert(newBuffer != 0);

		// copy buffers
		strcpy(newBuffer, buffer);
		strcpy(newBuffer+blen, rhs.buffer);

		// delete old buffer and save new buffer
		delete [] buffer;
		buffer = newBuffer;
		bufferLength = newBufferLength;
	}
	else
	{
		// copy buffer
		strcpy(buffer+blen, rhs.buffer);
	}

	// return new string
	return(*this);
}

#if 0
String
String::operator+(const String &rhs) const
{
	return(String(*this) += rhs);
}
#else
String
operator+(const String &lhs, const String &rhs)
{
	return(String(lhs) += rhs);
}
#endif

// string length
int
String::strlen() const
{
	return(::strlen(buffer));
}

// casting string to character array
String::operator const char *() const
{
	return(buffer);
}

// input and output
ostream &
operator<<(ostream &os, const String &s)
{
	os << s.buffer;
	return(os);
}
 
istream &
operator>>(istream &is, String &s)
{
	is >> s.buffer;
	return(is);
}
