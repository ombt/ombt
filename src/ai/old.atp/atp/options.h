#ifndef __OPTIONS_H
#define __OPTIONS_H
// options header

// headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "mystring.h"
#include "map.h"
#include "globals.h"

// externs
int getOption(const String &, const String &, String &);
int getOption(const String &, int , int &);

#endif
