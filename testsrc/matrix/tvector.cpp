//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include "complex/Complex.h"
#include "matrix/Vector.h"

using namespace ombt;
using namespace std;

main(int argc, char **argv)
{
	MustBeTrue(argc == 3);
	srand(atol(argv[1]));

	unsigned int dim = atoi(argv[2]);

	Vector<Complex<double> > a(dim);
	cerr << "a is ... " << a << endl;

	int iv;
	for (iv = 0 ; iv < dim; iv++)
	{
		a[iv] = Complex<double>(rand(), rand());
	}
	cerr << "a is ... " << a << endl;

	Vector<Complex<double> > b(dim);
	cerr << "b is ... " << b << endl;

	for (iv = 0 ; iv < dim; iv++)
	{
		b[iv] = Complex<double>(rand(), rand());
	}
	cerr << "b is ... " << b << endl;

	cerr << "a + b is " << (a+b) << endl;
	cerr << "a - b is " << (a+b) << endl;

	double scalar = rand();
	cerr << "scalar is ... " << scalar << endl;

	cerr << "a/scalar is ... " << a/scalar << endl;
	cerr << "a*scalar is ... " << a*scalar << endl;
	cerr << "scalar*a is ... " << scalar*a << endl;

	cerr << "dot(a, b) is ... " << dot(a, b) << endl;
	cerr << "norm(scalar*a) is ... " << norm(scalar*a) << endl;

	return(0);
}
