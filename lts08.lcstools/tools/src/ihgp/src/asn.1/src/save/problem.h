#ifndef __PROBLEM_H
#define __PROBLEM_H

// headers
#include <stdlib.h>
#include <iostream.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// local headers
#include "array.h"
#include "mylist.h"
#include "mystring.h"
#include "semantic.h"
#include "map.h"
#include "folp.h"
#include "miscutils.h"

// problem decomposition class
struct Problem {
	// constructor and destructor
	Problem(const List<Semantic * > &, const List<Semantic * > &,
		const Semantic *, const String & );
	~Problem();

	// split problem according to reduction type
	int split();

	// convert to cnf
	int convert2cnf();

	// negate split problems
	int negate();

	// write intermediate representations for prover
	int writeOptions(const String &, const Map<String, String> &) const;
	int writeAndOr(const String &) const;
	int writepfile(const String &, int) const;
	int writeIR(const Map<String, String> &, 
		const String &, const String & ) const;

	// output 
	friend ostream &operator<<(ostream &, const Problem &);

	// internal data
protected:
	const List<Semantic * > &clpremises;
	const List<Semantic * > &clextraaxioms;
	const Semantic *conclusion;
	String andor;
	String reductiontype;
	Array<List<Semantic * > > subconclusions;
	Array<List<Semantic * > > subextraaxioms;
	Array<List<Semantic * > > clsubconclusions;
	Array<List<Semantic * > > clsubextraaxioms;
};

#endif
