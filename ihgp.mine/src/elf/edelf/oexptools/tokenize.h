#ifndef __TOKENIZE_H
#define __TOKENIZE_H
// tokenizer for elf editor

// headers
#include <stdio.h>
#include <string.h>

// local headers
#include "returns.h"
#include "debug.h"

// externs
void tokenize(const char *, const char *);
char *gettoken(int);

#endif
