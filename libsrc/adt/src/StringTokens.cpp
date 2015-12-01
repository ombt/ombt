//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
// functions for string tokens class

// header
#include "adt/StringTokens.h"

namespace ombt {

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

}

