//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include <stdlib.h>
#include <assert.h>
#include <iostream>

#include "numerics/SignedInteger.h"

using namespace ombt;
using namespace std;

const char *
bstring(bool flag)
{
	if (flag)
		return("true");
	else
		return("false");
}

main(int argc, char **argv)
{
	cerr << endl << "TESTS FOR SignedInteger - string operations ..." << endl;

	SignedInteger<50> bint0("-65535");
	cerr << endl << "bint0 = " << bint0 << endl;
	SignedInteger<50> bint1("65536");
	cerr << endl << "bint1 = " << bint1 << endl;
	SignedInteger<50> bint2("-1000000000000000000000000000000000000065536");
	cerr << endl << "bint2 = " << bint2 << endl;

	cerr << endl;
	for (int arg=1; arg<argc; ++arg)
	{
		cerr << endl << "argv[" << arg << "] = " << argv[arg] << endl;
		cerr << "argv[" << arg << "] = " << SignedInteger<50>(argv[arg]) << endl;
	}

	cerr << endl;
	SignedInteger<50> bint3;
	for (int arg=1; arg<argc; ++arg)
	{
		cerr << endl << "argv[" << arg << "] = " << argv[arg] << endl;
		bint3 = argv[arg];
		cerr << "bint3 = argv[" << arg << "] = " << bint3 << endl;
		cerr << "bint3.toString() = " << bint3.toString() << endl;
	}

	return(0);
}
