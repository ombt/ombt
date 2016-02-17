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

#include "xstring.h"

String::String(const char *str)
{
    rep = new char[strlen(str) + 1];
    strcpy(rep, str);
}

String::String(const String &other)
{
    rep = new char[strlen(other.rep) + 1];
    strcpy(rep, other.rep);
}

String &String::operator=(const String &other)
{
    if (this != &other)
    {
	delete [] rep;
        rep = new char[strlen(other.rep) + 1];
	strcpy(rep, other.rep);
    }
    return(*this);
}

void String::setstring(const char *str)
{
    delete [] rep;
    rep = new char[strlen(str) + 1 ];
    strcpy(rep, str);
}
