//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include <gstring/gstring.h>
#include <random/Random.h>

#define ABS(x) ((x) < 0 ? (-(x)) : (x))

using namespace ombt;
using namespace std;

int
main(int argc, char **argv)
{
	// check number of arguments
	MustBeTrue(argc == 2);

	// set key for random number generator
	setKey(atol(argv[1]));

	// initialize some arrays for creating strings
	int csz = 0;
	while (csz <= 0)
	{
		csz = random()%100;
	}
	char *pc = new char [csz];
	MustBeTrue(pc != NULL);
	for (int ic = 0; ic < csz; ic++)
	{
		int newc = (char)random()%52;
		newc = ABS(newc);
		if (newc < 26)
		{
			*(pc+ic) = 'a' + newc;
		}
		else
		{
			*(pc+ic) = 'A' + (newc%26);
		}
	}

	// create some strings
	GString<char> cstr(csz, pc);

	// second string
	csz = 0;
	while (csz <= 0)
	{
		csz = random()%100;
	}
	pc = new char [csz];
	MustBeTrue(pc != NULL);
	for (int ic = 0; ic < csz; ic++)
	{
		int newc = (char)random()%52;
		newc = ABS(newc);
		if (newc < 26)
		{
			*(pc+ic) = 'a' + newc;
		}
		else
		{
			*(pc+ic) = 'A' + (newc%26);
		}
	}

	// create some strings
	GString<char> cstr2(csz, pc);

	// print string
	cout << "string 1 is ... " << cstr << endl;
	cout << "string 2 is ... " << cstr2 << endl;

	// concatenate
	cout << "string1 + string2 are ... " << (cstr+cstr2) << endl;
	cout << "(string1 + string2)(2,5) are ... " << (cstr+cstr2)(2,5) << endl;

	return(0);
}
