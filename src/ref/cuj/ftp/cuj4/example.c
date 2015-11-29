// gaussian LUP header, matrix header, vector header, etc.
#include "gausslup.h"

double data[] = {
	33.0, 16.0, 72.0,
	-24.0, -10.0, -57.0,
	-8.0, -4.0, -17.0,
	-359.0, 281.0, 85.0
};
 
// test driver
main(int , char **)
{
	// output precision
	cout.precision(6);
	cout.setf(ios::showpoint);

	// define matrix and initialize elements
	int nrows = 3;
	int ncols = 3;
	int idata = 0;
	int ir = 0;
	Matrix<double> m(nrows, ncols);
	for ( ; ir < nrows; ir++)
	{
		for (int ic = 0; ic < ncols; ic++)
		{
			m(ir, ic) = data[idata++];
		}
	}
	cout << "TEST MATRIX IS ... " << endl;
	cout << m << endl;
	Matrix<double> msave(m);

	// initialize inhomogeneous part.
	Vector<double> y(nrows);
	for (ir = 0; ir < nrows; ir++)
	{
		y[ir] = data[idata++];
	}
	cout << "TEST Y-VECTOR IS ... " << endl;
	cout << y << endl << endl;

	// get LUP decomposition
	Vector<int> pv(nrows);
	double determinant;
	MustBeTrue(GaussianLUP_Pivot(m, pv, 0.0, determinant) == OK);

	// get solution using LUP results
	Vector<double> x(nrows);
	MustBeTrue(SolveUsingGaussianLUP_Pivot(m, x, y, pv, 0.0) == OK);
	cout << "SOLUTION X IS ... " << x << endl << endl;

	// get inverse using LUP results
	Matrix<double> minv(nrows, ncols);
	MustBeTrue(GetInverseUsingGaussianLUP_Pivot(m, minv, pv, 0.0) == OK)
	cout << "INVERSE OF M IS ... " << endl << endl;
	cout << minv << endl;
	cout << "M*MINV IS ... " << endl;
	cout << msave*minv << endl << endl;

	return(0);
}

