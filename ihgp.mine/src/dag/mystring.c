// functions for string class

// headers
#include "mystring.h"

// local definitions
#define BufSzToStrSz(bs) ((bs)-1)
#define StrSzToBufSz(ss) ((ss)+1)

// string constructors and destructor
String::String():
	maxStringSize(BUFSIZ), 
	buffer(new char [StrSzToBufSz(BUFSIZ)])
{
	// check if buffer was allocated
        MustBeTrue(buffer != NULL);

	// null terminate
	*buffer = 0;
}

String::String(int maxStrSz):
	maxStringSize(maxStrSz), 
	buffer(new char [StrSzToBufSz(maxStrSz)])
{
	// check if buffer was allocated
        MustBeTrue(maxStringSize >= 0 && buffer != NULL);

	// null terminate
	*buffer = 0;
}

String::String(char c):
	maxStringSize(BUFSIZ), 
	buffer(new char [StrSzToBufSz(BUFSIZ)])
{
	// check if buffer was allocated
        MustBeTrue(buffer != NULL);

	// store character and null terminate
	buffer[0] = c;
	buffer[1] = 0;
}

String::String(const char *pc):
	maxStringSize(0), buffer(NULL)
{
	// check for invalid pointer
	MustBeTrue(pc != NULL);

	// allocate a default size buffer
	maxStringSize = ::strlen(pc);
        MustBeTrue(maxStringSize >= 0);
	buffer = new char [StrSzToBufSz(maxStringSize)];
	MustBeTrue(buffer != NULL);

	// copy string
	::strcpy(buffer, pc);
}

String::String(const String &s):
	maxStringSize(s.maxStringSize), 
	buffer(new char [StrSzToBufSz(s.maxStringSize)])
{
	// check if buffer was allocated
        MustBeTrue(maxStringSize >= 0 && buffer != NULL);

	// copy string
	::strcpy(buffer, s.buffer);
}

String::~String()
{
	// clear everything
	delete [] buffer;
	maxStringSize = 0;
}

// assignment operators
String &
String::operator=(const String &rhs)
{
	// check for self-assignment
	if (this == &rhs) return(*this);

	// is buffer long enough
	if (rhs.maxStringSize > maxStringSize)
	{
		// delete old buffer and allocate new one
		delete [] buffer;
		maxStringSize = rhs.maxStringSize;
        	MustBeTrue(maxStringSize >= 0);
		buffer = new char [StrSzToBufSz(maxStringSize)];
        	MustBeTrue(buffer != NULL);
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
	MustBeTrue(rhs != NULL);

	// check for self-assignment
	if (buffer == rhs) return(*this);

	// is buffer long enough
	int stringSize = ::strlen(rhs);
	if (stringSize > maxStringSize)
	{
		// delete old buffer and allocate new one
		delete [] buffer;
		maxStringSize = stringSize;
        	MustBeTrue(maxStringSize >= 0);
		buffer = new char [StrSzToBufSz(maxStringSize)];
        	MustBeTrue(buffer != NULL);
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
	MustBeTrue(0 <= idx && idx <= maxStringSize);
	return(buffer[idx]);
}

const char &
String::operator[](int idx) const
{
	MustBeTrue(0 <= idx && idx <= maxStringSize);
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
String::operator()(int start) const
{
	return((*this)(start, maxStringSize));
}

String
String::operator()(int start, int charsToCopy) const
{
	// check start of substring.
	MustBeTrue(0 <= start && start <= maxStringSize);
	MustBeTrue(0 <= charsToCopy);

	// calculate length of string to copy
	if ((start+charsToCopy) > maxStringSize)
		charsToCopy = maxStringSize - start;
	if (charsToCopy < 0)
		charsToCopy = 0;

	// copy substring
	String substr(charsToCopy);
	::strncpy(substr.buffer, buffer+start, charsToCopy);
	substr.buffer[charsToCopy] = 0;

	// return substring
	return(substr);
}

// concatenation operators
String &
String::operator+=(const String &rhs)
{
	// check of current buffer is long enough for new buffer
	int charsInBuffer = ::strlen(buffer);
	int charsToCopy = ::strlen(rhs.buffer);
	int stringSize = charsInBuffer + charsToCopy;
	if (stringSize > maxStringSize)
	{
		// allocate a new buffer 
		char *newBuffer = new char [StrSzToBufSz(stringSize)];
		MustBeTrue(newBuffer != NULL);

		// copy buffers
		::strcpy(newBuffer, buffer);
		::strcpy(newBuffer+charsInBuffer, rhs.buffer);

		// delete old buffer and save new buffer
		delete [] buffer;
		buffer = newBuffer;
		maxStringSize = stringSize;
	}
	else
	{
		// copy buffer
		::strcpy(buffer+charsInBuffer, rhs.buffer);
	}

	// return new string
	return(*this);
}

String
String::operator+(const String &rhs) const
{
	return(String(*this) += rhs);
}

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

String::operator char *()
{
	return(buffer);
}

// input and output
ostream &
operator<<(ostream &os, const String &s)
{
	MustBeTrue(s.buffer != NULL);
	s.buffer[s.maxStringSize] = 0;
	os << s.buffer;
	return(os);
}
 
istream &
operator>>(istream &is, String &s)
{
	char buffer[BUFSIZ];
	is >> buffer;
	s = buffer;
	return(is);
}

istream &
String::getline(istream &is, char delimiter)
{
	char buf[BUFSIZ+1];
	is.get(buf, BUFSIZ);
	is.ignore(BUFSIZ, delimiter);
	*this = buf;
	return(is);
}

// string matcher constructor and destructor
StringMatcher::StringMatcher(const String &s, const String &p):
	current(0), stringLength(s.strlen()), patternLength(p.strlen()),
	string(s), pattern(p)
{
	MustBeTrue(patternLength <= stringLength);
	for ( ; current <= (stringLength-patternLength); current++)
	{
		if (pattern == string(current, patternLength))
		{
			break;
		}
	}
}

StringMatcher::~StringMatcher()
{
	// do nothing
}

// string matcher operations
void
StringMatcher::reset()
{
	for (current=0 ; current <= (stringLength-patternLength); current++)
	{
		if (pattern == string(current, patternLength))
		{
			break;
		}
	}
	return;
}

int
StringMatcher::done() const
{
	return(current > (stringLength-patternLength));
}

String
StringMatcher::operator()() const
{
	MustBeTrue( ! done());
	return(string(current, stringLength-current));
}

int
StringMatcher::operator++(int)
{
	for (current++ ; current <= (stringLength-patternLength); current++)
	{
		if (pattern == string(current, patternLength))
		{
			return(1);
		}
	}
	return(0);
}

int
StringMatcher::location() const
{
	return(current);
}

//
// pseudo-Rabin-Karp string matcher constructor and destructor
// 
// this implementation of the Rabin-Karp algorithm is a little bit
// brain-dead. rather than using a modulo arithmetic and base equal
// to the number of characters in the alphabet (127 for ascii), I 
// just used a power of 2 and didn't bother with the modulo. the net
// result is that the pattern string length is limited to about
// 24 characters, that is, log2(2**31/2**7) = 24. hell, if some
// pattern matches to 24 character, then it has a good chance that
// it is match. if you really need more than 24 characters, then
// switch to a simple sum of all the characters in the search pattern.
// with that many characters you probably don't have to worry about
// 'a'+'b' = 'b'+'a' type of false matches.
//
StringMatcher_RabinKarp::StringMatcher_RabinKarp(
	const String &s, const String &p):
	StringMatcher(s, p), powerOfTwo(0), patternValue(0), stringValue(0)
{
	reset();
}

StringMatcher_RabinKarp::~StringMatcher_RabinKarp()
{
	// do nothing
}

// string matcher operations
void
StringMatcher_RabinKarp::reset()
{
	// initialize variables
	patternValue = 0;
	stringValue = 0;
	powerOfTwo = 1;

	// initialize test values
	for (int ip = 0; ip < patternLength; ip++)
	{
		patternValue = 2*patternValue + pattern[ip];
		stringValue = 2*stringValue + string[ip];
		powerOfTwo *= 2;
	}
	powerOfTwo /= 2;

	// look for the first match
	for (current=0 ; current <= (stringLength-patternLength); current++)
	{
		// check if we found a possible match
		if (patternValue == stringValue)
		{
			if (pattern == string(current, patternLength))
			{
				// we found a match
				return;
			}
		}

		// calculate next case
		if (current < (stringLength-patternLength))
		{
			stringValue = 
				2*(stringValue - powerOfTwo*string[current]) +
				string[current+patternLength];
		}
	}

	// no matches found
	return;
}

int
StringMatcher_RabinKarp::operator++(int)
{
	// calculate next case
	if (current < (stringLength-patternLength))
	{
		stringValue = 
			2*(stringValue - powerOfTwo*string[current]) +
			string[current+patternLength];
	}

	// look for next match
	for (current++ ; current <= (stringLength-patternLength); current++)
	{
		// check if we found a possible match
		if (patternValue == stringValue)
		{
			if (pattern == string(current, patternLength))
			{
				// we found a match
				return(OK);
			}
		}

		// calculate next case
		if (current < (stringLength-patternLength))
		{
			stringValue = 
				2*(stringValue - powerOfTwo*string[current]) +
				string[current+patternLength];
		}
	}

	// no matches found
	return(NOMATCH);
}

// string tokenizer constructor and destructor
StringTokens::StringTokens(const String &s, const String &d):
	tokenStart(0), tokenEnd(0),
	stringLength(s.strlen()), delimiterLength(d.strlen()),
	string(s), delimiter(d)
{
	// sanity check
	MustBeTrue(delimiterLength > 0);

	// skip leading delimiters
	int is;
	for (is = 0; is < stringLength; is++)
	{
		int match = 0;
		for (int id = 0; id < delimiterLength; id++)
		{
			if (string[is] == delimiter[id])
			{
				match = 1;
				break;
			}
		}
		if (!match) break;
	}
	tokenStart = is;

	// find end of token
	int ie;
	for (ie = tokenStart; ie < stringLength; ie++)
	{
		int match = 0;
		for (int id = 0; id < delimiterLength; id++)
		{
			if (string[ie] == delimiter[id])
			{
				match = 1;
				break;
			}
		}
		if (match) break;
	}
	tokenEnd = ie;
}

StringTokens::StringTokens(const StringTokens &st):
	tokenStart(st.tokenStart), tokenEnd(st.tokenEnd), 
	stringLength(st.stringLength), delimiterLength(st.delimiterLength), 
	string(st.string), delimiter(st.delimiter)
{
	// nothing to do
}

StringTokens::~StringTokens()
{
	// do nothing
}

// string matcher operations
void
StringTokens::reset()
{
	// skip leading delimiters
	int is;
	for (is = 0; is < stringLength; is++)
	{
		int match = 0;
		for (int id = 0; id < delimiterLength; id++)
		{
			if (string[is] == delimiter[id])
			{
				match = 1;
				break;
			}
		}
		if (!match) break;
	}
	tokenStart = is;

	// find end of token
	int ie;
	for (ie = tokenStart; ie < stringLength; ie++)
	{
		int match = 0;
		for (int id = 0; id < delimiterLength; id++)
		{
			if (string[ie] == delimiter[id])
			{
				match = 1;
				break;
			}
		}
		if (match) break;
	}
	tokenEnd = ie;
	return;
}

int
StringTokens::done() const
{
	return(tokenStart > stringLength-1);
}

String
StringTokens::operator()() const
{
	MustBeTrue(!done());
	return(string(tokenStart, tokenEnd-tokenStart));
}

int
StringTokens::operator++(int)
{
	// skip leading delimiters
	int is;
	for (is = tokenEnd+1; is < stringLength; is++)
	{
		int match = 0;
		for (int id = 0; id < delimiterLength; id++)
		{
			if (string[is] == delimiter[id])
			{
				match = 1;
				break;
			}
		}
		if (!match) break;
	}
	tokenStart = is;

	// find end of token
	int ie;
	for (ie = tokenStart; ie < stringLength; ie++)
	{
		int match = 0;
		for (int id = 0; id < delimiterLength; id++)
		{
			if (string[ie] == delimiter[id])
			{
				match = 1;
				break;
			}
		}
		if (match) break;
	}
	tokenEnd = ie;
	return(0);
}

int
StringTokens::location() const
{
	return(tokenStart);
}
