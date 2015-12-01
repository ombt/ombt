#ifndef __VARNAMES_H
#define __VARNAMES_H
// track variable names thru a resolution proof

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "mystring.h"
#include "tuple.h"
#include "binaryTree_AVL.h"
#include "map.h"

// define structure for tracking variable names
extern BinaryTree_AVL<Tuple<String, String> > variableNames;

// update function for variable names
extern int updateVariableNames(const String &, const String &);
extern int updateVariableNames(const Map<String, String> &);

#endif
