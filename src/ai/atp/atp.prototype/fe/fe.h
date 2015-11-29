#ifndef _FE_H
#define _FE_H
// atp front end header file

// required headers
#include "returns.h"
#include "debug.h"
#include "array.h"
#include "mylist.h"
#include "binaryTree_AVL.h"
#include "semantic.h"
#include "symbol.h"
#include "mystring.h"

// externs
extern int createProblems();
extern int makedir(const char *);
extern int removedir(const char *);

// local definitions
#define RMCMD "/usr/bin/rm -rf"

#endif
