//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include <stdlib.h>
#include <iostream>

#include "numerics/Rational.h"

using namespace ombt;
using namespace std;

main(int argc, char **argv)
{
	int imax = 20;

	if (argc >= 2)
	{
		imax = atoi(argv[1]);
	}

	Rational<long> r(1,1);

	for (int i=1; i<=imax; ++i)
	{
		cout << "r is " << r << endl;
		r = Rational<long>(1) + Rational<long>(1)/r;
	}

	return(0);
}
