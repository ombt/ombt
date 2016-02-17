#ifndef __MISCUTILS_H
#define __MISCUTILS_H

// headers
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "mystring.h"

// externs
extern String itoa(int);
extern int makedir(const String &);

#endif
