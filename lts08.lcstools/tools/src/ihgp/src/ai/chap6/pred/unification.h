#ifndef __UNIFICATION_H
#define __UNIFICATION_H
// definitions for unification

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "mystring.h"
#include "term.h"
#include "substitution.h"

// unification function
int unify(Term &, Term &, Substitutions &);

#endif
