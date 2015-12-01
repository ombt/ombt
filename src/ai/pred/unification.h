#ifndef __UNIFICATION_H
#define __UNIFICATION_H
// definitions for unification

// headers
#include <stdlib.h>
#include <iostream>

// local headers
#include "system/Returns.h"
#include "system/Debug.h"
#include "adt/String.h"
#include <term.h>
#include <substitution.h>

using namespace std;
using namespace ombt;

// unification function
int unify(Term &, Term &, Substitutions &);

#endif
