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

// unification function
int unify(Term &, Term &, Substitutions &);

#endif
