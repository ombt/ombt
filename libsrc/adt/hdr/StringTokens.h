//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __OMBT_STRING_TOKENS_H
#define __OMBT_STRING_TOKENS_H

// string tokens class definitions

// headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// local headers
#include "system/Returns.h"
#include "system/Debug.h"
#include "adt/String.h"

namespace ombt {

// string tokenizer class
class StringTokens {
public:
        // constructors and destructor
	StringTokens(const String &, const String &);
	StringTokens(const StringTokens &);
        virtual ~StringTokens();

	// assignment operator
	StringTokens &operator=(const StringTokens &);

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
	int getTokenStart() const {
		return(tokenStart);
	}
	int getTokenEnd() const {
		return(tokenEnd);
	}

	// get/set delimiter
	void setDelimiter(const String &d) {
		delimiterLength = d.strlen();
		delimiter = d;
		return;
	}
	String getDelimiter() const {
		return(delimiter);
	}

private:
	// not allowed
	StringTokens();

protected:
	// internal data
	unsigned int tokenStart;
	unsigned int tokenEnd;
	unsigned int stringLength;
	unsigned int delimiterLength;
	String string;
	String delimiter;
};

}

#endif

