#ifndef __VARNAMES_H
#define __VARNAMES_H
// track variable names thru a resolution proof

// headers
#include <stdlib.h>
#include <iostream>

// local headers
#include <Returns.h>
#include <Debug.h>
#include <adt/String.h>
#include <adt/Tuple.h>
#include <adt/BinaryTree_AVL.h>
#include <adt/Map.h>

using namespace std;
using namespace ombt;

// define structure for tracking variable names
extern BinaryTree_AVL<Tuple<String, String> > variableNames;

// update function for variable names
extern int updateVariableNames(const String &, const String &);
extern int updateVariableNames(const Map<String, String> &);

#endif
