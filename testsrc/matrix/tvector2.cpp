//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include "numerics/Rational.h"
#include "random/Random.h"
#include "matrix/Vector.h"

using namespace ombt;
using namespace std;

main(int argc, char **argv)
{
	if (argc != 3)
	{
		cerr << "usage: " << argv[0] 
		    << " seed vector_size" << endl;
		return 0;
	}

	Random rgn;

	rgn.setKey(atol(argv[1]));

	unsigned int dim = atoi(argv[2]);

	Vector<Rational<long> > a(dim);
	cerr << "a is ... " << a << endl;

	int iv;
	for (iv = 0 ; iv < dim; iv++)
	{
		a[iv] = Rational<long>(long(1000*rgn.random0to1()),
				       long(1000*rgn.random0to1()));
	}
	cerr << "a is ... " << a << endl;

	Vector<Rational<long> > b(dim);
	cerr << "b is ... " << b << endl;

	for (iv = 0 ; iv < dim; iv++)
	{
		b[iv] = Rational<long>(long(1000*rgn.random0to1()),
				       long(1000*rgn.random0to1()));
	}
	cerr << "b is ... " << b << endl;

	cerr << "a + b is " << (a+b) << endl;
	cerr << "a - b is " << (a+b) << endl;

	double scalar = 1000*rgn.random0to1();
	cerr << "scalar is ... " << scalar << endl;

	// the following cases *work* but only because the
	// the double is truncated to an integer. the 
	// rational class requires an explicit conversion
	// from double or float to rational.
	cerr << "a/scalar is ... " << a/scalar << endl;
	cerr << "a*scalar is ... " << a*scalar << endl;
	cerr << "scalar*a is ... " << scalar*a << endl;

	// cerr << "dot(a, b) is ... " << dot(a, b) << endl;
	// cerr << "norm(scalar*a) is ... " << norm(scalar*a) << endl;

	return(0);
}
