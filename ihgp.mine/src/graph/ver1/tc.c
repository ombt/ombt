/* test complex */
#include <sysent.h>
#include <stdio.h>
#include <stdlib.h>
#include "returns.h"
#include "debug.h"
#include "complex.h"

main(int argc, char **argv)
{
	if (argc != 5)
	{
		fprintf(stderr, "usage: %s xreal ximag yreal yimag\n", argv[0]);
		exit(2);
	}

	ComplexNumber x(atof(argv[1]), atof(argv[2]));
	DUMPS("x is ...");
	x.dump();
	ComplexNumber y(atof(argv[3]), atof(argv[4]));
	DUMPS("y is ...");
	y.dump();
	ComplexNumber r(0.0, 0.0);

	DUMPS("r = x");
	r = x;
	r.dump();

	DUMPS("r = x + y");
	r = x + y;
	r.dump();

	DUMPS("r = x * y");
	r = x * y;
	r.dump();

	DUMPS("r = x / y");
	r = x / y;
	r.dump();

	DUMPS("r = x - y");
	r = x - y;
	r.dump();

	DUMPS("x == y");
	if (x == y)
		DUMPS("x equals y");
	else
		DUMPS("x NOT equals y");

	DUMPS("x != y");
	if (x != y)
		DUMPS("x NOT equals y");
	else
		DUMPS("x equals y");

	DUMPS("r == y");
	r = y;
	if (r == y)
		DUMPS("r equals y");
	else
		DUMPS("r NOT equals y");

	DUMPS("r != y");
	r = y;
	if (r != y)
		DUMPS("r NOT equals y");
	else
		DUMPS("r equals y");

	DUMPS("x < y");
	if (x < y)
		DUMPS("x lt y");
	else
		DUMPS("x NOT lt y");

	DUMPS("x > y");
	if (x > y)
		DUMPS("x gt y");
	else
		DUMPS("x NOT gt y");

	DUMPS("exp(x)");
	r = exp(x);
	r.dump();

	DUMPS("log(x)");
	r = log(x);
	r.dump();

	DUMPS("log10(x)");
	r = log10(x);
	r.dump();

	DUMPS("pow(x, y)");
	r = pow(x, y);
	r.dump();

	DUMPS("sqrt(x)");
	r = sqrt(x);
	r.dump();

	DUMPS("sin(x)");
	r = sin(x);
	r.dump();

	DUMPS("cos(x)");
	r = cos(x);
	r.dump();

	DUMPS("tan(x)");
	r = tan(x);
	r.dump();

	DUMPS("conj(x)");
	r = conj(x);
	r.dump();

	DUMPS("abs(x)");
	r = abs(x);
	r.dump();

	DUMPS("arg(x)");
	r = arg(x);
	r.dump();

	DUMPS("norm(x)");
	r = norm(x);
	r.dump();

	DUMPS("real(x)");
	r = real(x);
	r.dump();

	DUMPS("imag(x)");
	r = imag(x);
	r.dump();

	exit(0);
}
