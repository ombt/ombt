#ifndef __DELETION_H
#define __DELETION_H
// definitions for deletion strategies

// headers
#include <stdlib.h>
#include <iostream>

// local headers
#include <Returns.h>
#include <Debug.h>
#include <adt/String.h>
#include <adt/Array.h>
#include <adt/Map.h>
#include "literal.h"
#include "clause.h"
#include "substitution.h"
#include "unification.h"
#include "uniquename.h"
#include "statistics.h"
#include "globals.h"

using namespace std;
using namespace ombt;

// deletion-related functions
int tautology(const Clause &);
int subsumes(const Clause &, const Clause &, Substitutions &);
int equal(const Terms &, const Terms &);
int equal(const Literal &, const Literal &);
int subset(const Clause &, const Clause &);
int groundSubstitutions(const Clause &, Substitutions &);
int factor(Literal &, Clause &, Substitutions &);

#endif
