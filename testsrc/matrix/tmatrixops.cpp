//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#include "complex/Complex.h"
#include "matrix/Matrix.h"
#include "matrix/MatrixOps.h"

using namespace ombt;
using namespace std;

main(int argc, char **argv)
{
	MustBeTrue(argc == 3);
	unsigned int nrows = atoi(argv[1]);
	unsigned int ncols = atoi(argv[2]);

	Matrix<Complex<double> > m(nrows, ncols);
	cerr << "m is ... " << m << endl;

	int ir;
	for (ir = 0 ; ir < nrows; ir++)
	{
		for (int ic = 0; ic < ncols; ic++)
		{
			m(ir, ic) = Complex<double>(ir+1,ic+1);
		}
	}
	cerr << "m is ... " << m << endl;

	cerr << "m*m is ... " << m*m << endl;

	cerr << "m+m is ... " << m+m << endl;

	cerr << "m-m is ... " << m-m << endl;

	cerr << "m*m/5.0 is ... " << m*m/5.0 << endl;

	cerr << "(m+m)*5.0 is ... " << (m+m)*Complex<double>(5.0) << endl;

	Matrix<Complex<double> > m2(m);
	cerr << "m2 is ... " << m2 << endl;

	MustBeTrue(conjugate(m) == OK);
	cerr << "conjugate(m) is ... " << m << endl;

	MustBeTrue(transpose(m) == OK);
	cerr << "transpose(m) is ... " << m << endl;

	MustBeTrue(adjoint(m2) == OK);
	cerr << "adjoint(m2) is ... " << m2 << endl;

	Matrix<double> rm(nrows, ncols);
	cerr << "rm is ... " << rm << endl;

	for (ir = 0 ; ir < nrows; ir++)
	{
		for (int ic = 0; ic < ncols; ic++)
		{
			rm(ir, ic) = ir+1*ic+1;
		}
	}
	cerr << "rm is ... " << rm << endl;

	cerr << "rm*rm is ... " << rm*rm << endl;

	cerr << "rm+rm is ... " << rm+rm << endl;

	cerr << "rm-rm is ... " << rm-rm << endl;

	cerr << "rm*rm/5.0 is ... " << rm*rm/5.0 << endl;

	cerr << "(rm+rm)*5.0 is ... " << (rm+rm)*5.0 << endl;

	MustBeTrue(conjugate(rm) == OK);
	cerr << "conjugate(rm) is ... " << rm << endl;

	MustBeTrue(transpose(rm) == OK);
	cerr << "transpose(rm) is ... " << rm << endl;

	MustBeTrue(adjoint(rm) == OK);
	cerr << "adjoint(rm) is ... " << rm << endl;

	return(0);
}
