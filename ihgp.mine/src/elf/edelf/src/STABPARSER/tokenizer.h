#ifndef __TOKENIZER_H
#define __TOKENIZER_H
// tokenizer class definition

// headers 
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// local headers
#include "returns.h"
#include "debug.h"

// tokenizer class
class Tokenizer {
public:
	// internal data types
	enum { Unknown, Name, String, Integer, 
		HexInteger, Real, Char, EndOfString } Type;

	// constructors and destructor
	Tokenizer(const char *ps):
		type(Unknown), start(-1), end(-1), len(0) {
		MustBeTrue(ps != NULL && *ps != '\0');
		len = strlen(ps);
		string = new char [len+1];
		MustBeTrue(string != NULL);
		strcpy(string, ps);
	}
	~Tokenizer() {
		if (string != NULL)
			delete [] string;
		string = NULL;
		type = Unknown;
		start = end = -1;
		len = 0;
	}

	// check next token type
	int isString() {
		// is a string already recognized?
		if (type == String)
			return(1);

		// skip any leading white space
		skipws();
		if (iseos())
			return(0);

		// try to recognize a string, if not found,
		// leave tokenizer in unknown state.
		type = Unknown;
		char sc = string[start];
		if (sc != '"' && sc != '\'')
			return(0);
		for (end=start+1; end<(start+len); )
		{
			char ec = string[end];
			if (sc == ec)
			{
				// found a string.
				type = String;
				return(1);
			}
			else if (ec == '\\')
			{
				// skip whatever character is escaped
				end += 2;
			}
			else
				end += 1;
		}

		// did not find a string
		return(0);
	}
	int isName() {
		if (type == Name)
			return(1);
		skipws();
		if (iseos())
			return(0);
		type = Unknown;
	}
	int isReal() {
		if (type == Real)
			return(1);
		skipws();
		if (iseos())
			return(0);
		type = Unknown;
	}
	int isInteger() {
		// is an integer already recognized?
		if (type == Integer)
			return(1);

		// skip any leading white space
		skipws();
		if (iseos())
			return(0);

		// try to find an integer
		type = Unknown;
		end = start;
		char c = string[end];

		// check for a sign
		if (c == '+' || c == '-')
			end++;
		if (iseos())
			return(0);

		// we must have at least one digit
		c = string[end];
		if (!isdigit(c))
			return(0);
		for (c=string[++end]; 
		     isdigit((int)c) && end<len; 
		     c=string[++end]) ;
		end--;
	}
	int isHexInteger() {
		if (type == HexInteger)
			return(1);
		skipws();
		if (iseos())
			return(0);
		type = Unknown;
	}
	int isChar() {
		if (type == Char)
			return(1);
		skipws();
		if (iseos())
			return(0);
		type = Unknown;
	}
	int iseos() {
		if (start >= len)
			type = EndOfString;
		return(type == EndOfString);
	}

	// get token value
	Type getType() const {
		return(type);
	}
	int getValue(Type tokentype, char *&token) {
		if (type != tokentype) return(NOTOK);
		strncpy(token, string, end-start+1);
		token[end-start+1] = '\0';
		return(OK);
	}

	// accept the token and move on
	void accept() {
		switch (type)
		{
		case Unknown:
			break;
		case EndOfString:
			break;
		default:
			type = Unknown;
			start = end+1;
			if (start >= len) 
				type = EndOfString;
			break;
		}
		return;
	}

protected:
	// utility functions
	void skipws() {
		if (iseos())
			return;
		for (char c=string[start]; 
		    ((c==' ' || c=='\t') && (start<len));
		     c=string[++start]) ;
		return;
	}

	// internal data
	char *string;
	Type type;
	int len, start, end;
}
#endif
