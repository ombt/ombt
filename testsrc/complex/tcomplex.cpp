//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include <stdlib.h>
#include <iostream>
#include "complex/Complex.h"

using namespace ombt;

main(int argc, char **argv)
{
	MustBeTrue(argc == 2);
	srand(atol(argv[1]));

	Complex<double> a(rand(), rand());
	std::cerr << "a is ... " << a << std::endl;

	Complex<double> b(rand(), rand());
	std::cerr << "b is ... " << b << std::endl;

	Complex<double> c(a);
	std::cerr << "copy a to c, c(a) is ... " << c << std::endl;

	a += b;
	std::cerr << "a += b is ... " << a << std::endl;

	a -= c;
	std::cerr << "a -= c is ... " << a << std::endl;

	a *= c;
	std::cerr << "a *= c is ... " << a << std::endl;

	a /= c;
	std::cerr << "a /= c is ... " << a << std::endl;

	std::cerr << "a is ... " << a << std::endl;
	std::cerr << "b is ... " << b << std::endl;

	std::cerr << "a + b is ... " << (a+b) << std::endl;
	std::cerr << "a - b is ... " << (a-b) << std::endl;
	std::cerr << "a * b is ... " << (a*b) << std::endl;
	std::cerr << "a / b is ... " << (a/b) << std::endl;

	if (a == b)
		std::cerr << "a == b is true" << std::endl;
	else
		std::cerr << "a == b is false" << std::endl;

	if (a != b)
		std::cerr << "a != b is true" << std::endl;
	else
		std::cerr << "a != b is false" << std::endl;

	if (a < b)
		std::cerr << "a < b is true" << std::endl;
	else
		std::cerr << "a < b is false" << std::endl;

	if (a > b)
		std::cerr << "a > b is true" << std::endl;
	else
		std::cerr << "a > b is false" << std::endl;

	if (a <= b)
		std::cerr << "a <= b is true" << std::endl;
	else
		std::cerr << "a <= b is false" << std::endl;

	if (a >= b)
		std::cerr << "a >= b is true" << std::endl;
	else
		std::cerr << "a >= b is false" << std::endl;

	Complex<double> d(1,1);
	std::cerr << "d is ... " << d << std::endl;
	std::cerr << "log(exp(d)) is ... " << log(exp(d)) << std::endl;
	std::cerr << "asin(sin(d)) is ... " << asin(sin(d)) << std::endl;
	std::cerr << "asinh(sinh(d)) is ... " << asinh(sinh(d)) << std::endl;

	Complex<double> e(1,-1);
	std::cerr << "e is ... " << e << std::endl;
	std::cerr << "log(exp(e)) is ... " << log(exp(e)) << std::endl;
	std::cerr << "asin(sin(e)) is ... " << asin(sin(e)) << std::endl;
	std::cerr << "asinh(sinh(e)) is ... " << asinh(sinh(e)) << std::endl;

	return(0);
}
