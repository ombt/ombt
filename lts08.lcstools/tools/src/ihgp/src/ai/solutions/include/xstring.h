/*
 * Copyright (C) 1995, 1996 Peter Bouthoorn.
 *
 * This software may be freely distributed and modified provided
 * this copyright message is left intact. The copyright message must be
 * included both with this (the original) software and with any modified
 * copies of this software or with any new software based on this software.
 * Furthermore any modified copies of this software must carry prominent
 * notices stating the software was changed and the date of any change.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. In no event
 * will the copyright holder be liable for any damage arising out of
 * the use of this software.
 *
 * As a matter of courtesy, the author requests to be informed about
 * any bugs found in this software and about improvements that may be of
 * general interest.
 *
 * Peter Bouthoorn
 * peter@obelix.icce.rug.nl
 */

#ifndef _XSTRING_H_
#define _XSTRING_H_

#include <stdio.h>
#include <iostream.h>
#include <string.h>

class String
{
    public:
	String();
	String(const char *);
	String(const String &);
	~String();
	String &operator=(const String &);
	String &operator=(const char *);
	int operator==(const String &) const;
	int operator!=(const String &) const;
	int operator==(const char *) const;
	int operator!=(const char *) const;
	void display() const;
	String *clone() const;
	int length() const;
	void setstring(const char *);
	const char *getstring() const;
    private:
	char *rep;
};

inline String::String()
{
    rep = new char[1];
    rep[0] = '\0';
}

inline String::~String()
{
    delete [] rep;
}

inline int String::operator==(const String &other) const
{
    return(strcmp(rep, other.rep) == 0);
}

inline int String::operator!=(const String &other) const
{
    return(strcmp(rep, other.rep) != 0);
}

inline int String::operator==(const char *s) const
{
    return(strcmp(rep, s) == 0);
}

inline int String::operator!=(const char *s) const
{
    return(strcmp(rep, s) != 0);
}

inline String &String::operator=(const char *str)
{
    setstring(str);
    return(*this);
}

inline String *String::clone() const
{
    return(new String(*this));
}

inline int String::length() const
{
    return(strlen(rep));
}

inline const char *String::getstring() const
{
    return(rep);
}

inline void String::display() const
{
    printf("%s", rep);
}

inline ostream &operator<<(ostream &stream, const String &s)
{
    return(stream << s.getstring());
}

inline int operator==(const char *s, const String &str)
{
    return(str == s);
}

inline int operator!=(const char *s, const String &str)
{
    return(str != s);
}

#endif
