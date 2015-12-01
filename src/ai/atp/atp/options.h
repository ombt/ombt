#ifndef __OPTIONS_H
#define __OPTIONS_H
// options header

// headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// local headers
#include <Returns.h>
#include <Debug.h>
#include <adt/String.h>
#include <adt/Map.h>
#include "globals.h"

// externs
int getOption(const String &, const String &, String &);
int getOption(const String &, int , int &);

#endif
