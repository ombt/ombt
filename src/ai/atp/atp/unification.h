#ifndef __UNIFICATION_H
#define __UNIFICATION_H
// definitions for unification

// headers
#include <stdlib.h>
#include <iostream>

// local headers
#include <Returns.h>
#include <Debug.h>
#include <adt/String.h>
#include "literal.h"
#include "substitution.h"

// unification functions
int unify(Terms &, Terms &, Substitutions &);
int unify(Literal &, Literal &, Substitutions &);

// unify lists of literals
int unify(List<Literal> &, Substitutions &);

#endif
