#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <matrix/Matrix.h>
#include <matrix/GaussLUP.h>
#include <matrix/TriDiagonal.h>

using namespace ombt;
using namespace std;

main(int argc, char **argv)
{
	int arg = 0;

	if (argc == 2)
	{
		srand(time(NULL));
	}
	else if (argc == 3)
	{
		srand(atol(argv[++arg]));
	}
	else
	{
		cerr << "usage: " << argv[0] << " [seed] nrows" << endl;
		return(2);
	}

	unsigned int nrows = atoi(argv[++arg]);
	unsigned int ncols = nrows;

	Vector<double> d(nrows);
	Vector<double> d2(nrows);
	Vector<double> dsave(nrows);
	Matrix<double> m(nrows, ncols);
	Matrix<double> m2(nrows, ncols);
	Matrix<double> msave(nrows, ncols);
	for (int ir = 0 ; ir < nrows; ++ir)
	{
		d[ir] = rand()%99+1;
		for (int ic = -1; ic <= 1; ++ic)
		{
			if ((0<=(ir+ic)) && ((ir+ic)<ncols))
				m(ir, ir+ic) = rand()%99+1;
		}
	}
	msave = m2 = m;
	dsave = d2 = d;

	cerr << "m = " << m << endl;
	cerr << "d = " << d << endl;

	Vector<double> x(nrows);

	double ep(0);

	SolveTriDiagonal(m, x, d, ep);

	cerr << "x = " << x << endl;
	cerr << "m*x=" << (msave*x) << endl;
	cerr << "d=" << dsave << endl;
	cerr << "m*x-d=" << (msave*x-dsave) << endl << endl;

	Vector<int> p(nrows);
	double sign;

	MustBeTrue(GaussianLUP_Pivot(m2, p, ep, sign) == 0);
	Vector<double> x2(nrows);
	MustBeTrue(SolveUsingGaussianLUP_Pivot(m2, x2, d2, p, ep) == 0);

	cerr << "x2 = " << x2 << endl;
	cerr << "m2*x2=" << (msave*x2) << endl;
	cerr << "d2=" << dsave << endl;
	cerr << "m2*x2-d2=" << (msave*x2-dsave) << endl << endl;

	return(0);
}
