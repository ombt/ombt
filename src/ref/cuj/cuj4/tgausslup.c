#include "gausslup.h"

void
usage(char *cmd)
{
	cerr << "usage: " << cmd << " [-?vdsiSA] ";
	cerr << "nrows ncols matrix-values ... [y-values ...]" << endl;
	cerr << endl;
	cerr << "\t-v - verify output is turned on, needs y-values." << endl;
	cerr << "\t-d - determinant is calculated." << endl;
	cerr << "\t-s - solution is calculated, needs y-values." << endl;
	cerr << "\t-i - inverse is calculated." << endl;
	cerr << "\t-S - symmetric matrix, half the matrix values are given." << endl;
	cerr << "\t-A - anti-symmetric matrix, half the matrix values are given." << endl;
	cerr << endl;
	cerr << "for symmetric or anti-symmetric matrices, (n*(n+1)/2)" << endl;
	cerr << "matrix-values are needed; n*n values for all other matrices." << endl;
	return;
}

main(int argc, char **argv)
{
	// command line options
	int dflag = 0;
	int sflag = 0;
	int iflag = 0;
	int vflag = 0;
	int Sflag = 0;
	int Aflag = 0;
	for (int c = 0; (c = getopt(argc, argv, "?ASvdsi")) != EOF; )
	{
		switch (c)
		{
		case 'A':
			Aflag = 1;
			Sflag = 0;
			break;
		case 'S':
			Sflag = 1;
			Aflag = 0;
			break;
		case 'v':
			vflag = 1;
			break;
		case 'd':
			dflag = 1;
			break;
		case 's':
			sflag = 1;
			break;
		case 'i':
			iflag = 1;
			break;
		case '?':
		default:
			usage(argv[0]);
			return(2);
		}
	}

	// get number of rows and columns
	unsigned int nrows = atoi(argv[optind++]);
	unsigned int ncols = atoi(argv[optind++]);

	// output precision
	cout.precision(6);

	// define matrix and initialize elements
	Matrix<double> m(nrows, ncols);
	if (Sflag || Aflag)
	{
		double sign = 1;
		if (Aflag) 
			sign = -sign;
		for (int ir = 0 ; ir < nrows; ir++)
		{
			for (int ic = 0; ic <= ir; ic++)
			{
				m(ir, ic) = strtod(argv[optind++], NULL);
				m(ic, ir) = sign*m(ir, ic);
			}
		}
	}
	else
	{
		for (int ir = 0 ; ir < nrows; ir++)
		{
			for (int ic = 0; ic < ncols; ic++)
			{
				m(ir, ic) = strtod(argv[optind++], NULL);
			}
		}
	}
	cout << "m is ... " << m << endl;

	// initialize inhomogeneous part.
	Vector<double> y(nrows);
	if (sflag || vflag)
	{
		for (int ir = 0; ir < nrows; ir++)
		{
			y[ir] = strtod(argv[optind++], NULL);
		}
		cout << "y is ... " << y << endl << endl;
	}

	// get LUP decomposition
	Matrix<double> m2(m);
	Vector<int> pv2(nrows);
	double determinant;
	if (GaussianLUP_Pivot(m2, pv2, 0.0, determinant) != OK)
	{
		cerr << "GaussianLUP_Pivot failed" << endl;
		return(2);
	}

	// get solution using LUP results
	if (sflag)
	{
		Vector<double> x2(nrows);
		Vector<double> y2(y);
		if (SolveUsingGaussianLUP_Pivot(m2, x2, y2, pv2, 0.0) != OK)
		{
			cerr << "SolveUsingGaussianLUP_Pivot failed" << endl;
			return(2);
		}
		cout << "SOLUTION: x is ... " << x2 << endl;
		if (vflag) cout << "SOLUTION: m*x is ... " << m*x2 << endl;
		if (vflag) cout << "SOLUTION: y is ... " << y << endl;
	}

	// get inverse using LUP results
	if (iflag)
	{
		Matrix<double> minv2(nrows, ncols);
		Vector<double> x2(nrows);
		if (GetInverseUsingGaussianLUP_Pivot(m2, minv2, pv2, 0.0) != OK)
		{
			cerr << "GetInverseUsingGaussianLUP_Pivot failed" << endl;
			return(2);
		}
		cout << "INVERSE: minv is ... " << minv2 << endl;
		if (vflag) cout << "INVERSE: minv*y is ... " << minv2*y << endl;
		if (vflag) cout << "INVERSE: m*minv is ... " << m*minv2 << endl;
		if (vflag) cout << "INVERSE: minv*m is ... " << minv2*m << endl;
	}

	// get deteminant using LUP results
	if (dflag)
	{
		if (GetDeterminantUsingGaussianLUP_Pivot(m2, determinant) != OK)
		{
			cerr << "GetDeterminantUsingGaussianLUP_Pivot failed" << endl;
			return(2);
		}
		cout << "DETERMINANT: determinant is ... " << determinant << endl;
	}

	return(0);
}
