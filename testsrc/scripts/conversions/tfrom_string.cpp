//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//

// headers
#include <iostream>
#include "conversions/Generic.h"

using namespace ombt;
using namespace std;

int
main(int argc, char **argv)
{
	int i = from_string<int>("45");
	cerr << "integer = " << i << endl;
	long l = from_string<long>("45");
	cerr << "long = " << l << endl;
	double d = from_string<double>("13.45");
	cerr << "double = " << d << endl;
	float f = from_string<float>("13.45");
	cerr << "float = " << f << endl;
	return 0;
}

