//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//

// headers
#include <iostream>
#include <iomanip>
#include "conversions/Generic.h"

using namespace ombt;
using namespace std;

int
main(int argc, char **argv)
{
	string s;

	int i = 45;
	s = to_string<int>(i);
	cerr << "from integer = " << s << endl;

	long l = 123456789;
	s = to_string<long>(l);
	cerr << "from long = " << s << endl;

	float f = 47.9877;
	s = to_string<float>(f);
	cerr << "from float = " << s << endl;

	double d = 12345.0005;
	s = to_string<double>(d);
	cerr << "from double = " << s << endl;

	return 0;
}
