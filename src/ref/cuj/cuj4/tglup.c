// gaussian LUP header, matrix header, vector header, etc.
#include "gausslup.h"

// constants and globals
const int MaxPoints = 2000;
double eps = 0.0;

// structure to hold test data
struct TestCase {
	int testno;
	char *options;
	int nrows;
	int ncols;
};

// define test cases
TestCase test1 = {
	1,
	"-i -v",
	4, 4
};
double data1[] = {
	1.0, 1.0, 1.0, 1.0,
	1.0, 2.0, 3.0, 4.0,
	1.0, 4.0, 9.0, 16.0,
	1.0, 8.0, 27.0, 64.0
};

TestCase test2 = {
	2,
	"-i -s -v",
	3, 3
};
double data2[] = {
	33.0, 16.0, 72.0,
	-24.0, -10.0, -57.0,
	-8.0, -4.0, -17.0,
	-359.0, 281.0, 85.0
};
 
TestCase test3 = {
	3,
	"-i -s -v",
	4, 4
};
double data3[] = {
	1.0, -2.0, 3.0, 1.0,
	-2.0, 1.0, -2.0, -1.0,
	3.0, -2.0, 1.0, 5.0,
	1.0, -1.0, 5.0, 3.0,
	3.0, -4.0, 7.0, 8.0
};
 
TestCase test4 = {
	4,
	"-i -v",
	6, 6
};
double data4[] = {
	1.0, 0.0, 0.0, 0.0, 0.0, 1.0,
	1.0, 1.0, 0.0, 0.0, 0.0, -1.0,
	-1.0, 1.0, 1.0, 0.0, 0.0, -1.0,
	1.0, -1.0, 1.0, 1.0, 0.0, -1.0,
	-1.0, 1.0, -1.0, 1.0, 1.0, 1.0,
	1.0, -1.0, 1.0, -1.0, 1.0, -1.0
};
 
TestCase test5 = {
	5,
	"-i -s -v",
	4, 4
};
double data5[] = {
	5.0, 7.0, 6.0, 5.0,
	7.0, 10.0, 8.0, 7.0,
	6.0, 8.0, 10.0, 9.0,
	5.0, 7.0, 9.0, 10.0,
	23.0, 32.0, 33.0, 31.0
};
 
TestCase test6 = {
	6,
	"-i -v",
	4, 4
};
double data6[] = {
	4.0, 3.0, 2.0, 1.0,
	3.0, 4.0, 3.0, 2.0,
	2.0, 3.0, 4.0, 3.0,
	1.0, 2.0, 3.0, 4.0
};
 
TestCase test7 = {
	7,
	"-i -v",
	4, 4
};
double data7[] = {
	28.0, -42.0, 28.0, -7.0,
	-42.0, 98.0, -77.0, 21.0,
	28.0, -77.0, 70.0, -21.0,
	-7.0, 21.0, -21.0, 7.0
};
 
TestCase test8 = {
	8,
	"-S -i -v",
	10, 10
};
double data8[] = {
	100.0,
	-4950.0, 326700.0,
	79200.0, -5880600.0, 112907520.0,
	-600600.0, 47567520.0, -951350400.0, 8245036800.0,
	2522520.0, -208107900.0, 4281076800.0, -37875637800.0,
		176752976400.0,
	-6306300.0, 535134600.0, -11237826600.0, 101001700800.0,
		-477233036280.0, 130154464440.0,
	9609600.0, -832431600.0, 17758540800.0, -161602721280.0,
		771285715200.0, -2121035716800.0, 3480673996800.0,
	-8751600.0, 770140800.0, -16635041280.0, 152907955200.0,
		-73586934400.0, 2037792556800.0, -3363975014400.0,
		3267861442560.0,
	4375800.0, -389884780.0, 8506555200.0, -78843164400.0,
		382086104400.0, -1064382719400.0, 1766086882560.0,
		-1723286307600.0, 912328045200.0,
	-923780.0, 83140200.0, -1829084400.0, 17071454400.0,
		-83223340200.0, 233025352560.0, -388375587600.0,
		380449555200.0, -202113826200.0, 44914183600.0
};
 
TestCase test9 = {
	9,
	"-S -i -v",
	6, 6
};
double data9[] = {
	36.0,
	-630.0, 14700.0,
	3360.0, -88200.0, 564480.0,
	-7560.0, 211680.0, -1411200.0, 3628800.0,
	7560.0, -220500.0, 1512000.0, -3969000.0, 4410000.0,
	-2772.0, 83160.0, -582120.0, 1552320.0, -1746360.0, 698544.0
};
 
TestCase test10 = {
	10,
	"-i -v",
	6, 6
};
double data10[] = {
	-6.0, 630.0, -6720.0, 22680.0, -30250.0, 13860.0,
	105.0, -7350.0, 88200.0, -317520.0, 441000.0, -207900.0,
	-560.0, 29400.0, -376320.0, 1411200.0, -2016000.0, 970200.0,
	1260.0, -52920.0, 705600.0, -2721600.0, 3969000.0, -1940400.0,
	-1260.0, 44100.0, -604800.0, 2381400.0, -3528000.0, 1746360.0,
	462.0, 13860.0, 194040.0, -776160.0, 116420.0, -582120.0
};
 
TestCase test11 = {
	11,
	"-i -v",
	4, 4
};
double data11[] = {
	4.0, 3.0, 2.0, 1.0,
	3.0, 6.0, 4.0, 2.0,
	2.0, 4.0, 6.0, 3.0,
	1.0, 2.0, 3.0, 4.0
};
 
TestCase test12 = {
	12,
	"-i -s -v",
	1, 1
};
double data12[] = {
	1.0, 
	2.0
};

TestCase test13 = {
	13,
	"-d -v",
	3, 3
};
double data13[] = {
	-73.0, 78.0, 24.0,
	92.0, 66.0, 25.0,
	-80.0, 37.0, 10.0
};
 
TestCase test14 = {
	14,
	"-d -v",
	3, 3
};
double data14[] = {
	-73.0, 78.0, 24.0,
	92.0, 66.0, 25.0,
	-80.0, 37.0, 10.01
};
 
TestCase test15 = {
	15,
	"-d -v",
	3, 3
};
double data15[] = {
	-73.0, 78.0, 24.0,
	92.01, 66.0, 25.0,
	-80.0, 37.0, 10.0
};
 
TestCase test16 = {
	16,
	"-d -v",
	3, 3
};
double data16[] = {
	-73.0, 78.01, 24.0,
	92.0, 66.0, 25.0,
	-80.0, 37.0, 10.0
};
 
TestCase test17 = {
	17,
	"-d -v",
	3, 3
};
double data17[] = {
	-73.0, 78.01, 24.0,
	0.0, 66.0, 25.0,
	0.0, 0.0, 10.0
};

// initialize identity matrices
void
initident(Matrix<double> &mi)
{
	// get rows and columns
	MustBeTrue(mi.getRows() == mi.getCols() && mi.getRows() > 0);

	// initialize
	for (int r = 0; r < int(mi.getRows()); r++)
	{
		for (int c = 0; c < int(mi.getCols()); c++)
		{
			mi(r, c) = 0.0;
		}
		mi(r, r) = 1.0;
	}
	return;
}

// calculate standard deviation for a residuals matrix.
double
rms(Matrix<double> &m)
{
	double sum = 0.0;
	double howmany = m.getCols()*m.getRows();
	for (int r = 0; r < int(m.getRows()); r++)
	{
		for (int c = 0; c < int(m.getCols()); c++)
		{
			sum += m(r, c)*m(r, c);
		}
	}
	return(sqrt(sum/howmany));
}


// run test case
void
runit(TestCase &test, double *data)
{
	// test case that we are running
	cout << endl << ">>>>> STARTING TEST CASE ... ";
	cout << test.testno << endl << endl;

	// get options for test
	int dflag = 0;
	int sflag = 0;
	int iflag = 0;
	int vflag = 0;
	int Sflag = 0;
	int Aflag = 0;
	for (char *popt = test.options; *popt != 0; )
	{
		// skip if not an option
		if (*popt++ != '-') continue;

		// get option 
		switch (*popt++)
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
		}
	}

	// get number of rows and columns
	int nrows = test.nrows;
	int ncols = test.ncols;

	// output precision
	cout.precision(6);
	cout.setf(ios::showpoint);

	// define matrix and initialize elements
	int idata = 0;
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
				m(ir, ic) = data[idata++];
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
				m(ir, ic) = data[idata++];
			}
		}
	}
	cout << "TEST MATRIX IS ... " << endl;
	cout << m << endl;

	// generate identity matrix
	Matrix<double> midentity(m.getRows(), m.getCols());
	initident(midentity);

	cout << "IDENTITY MATRIX IS ..." << endl;
	cout << midentity << endl;

	// save a copy of matrix for sanity checks
	Matrix<double> savem(m);
	MustBeTrue(m == savem);

	// initialize inhomogeneous part.
	Vector<double> y(nrows);
	if (sflag)
	{
		for (int ir = 0; ir < nrows; ir++)
		{
			y[ir] = data[idata++];
		}
		cout << "TEST Y-VECTOR IS ... " << endl;
		cout << y << endl << endl;
	}
	MustBeTrue(m == savem);

	// get LUP decomposition
	Matrix<double> m2(m);
	Vector<int> pv2(nrows);
	double determinant;
#ifdef DEBUG
	cout << "(Before GaussianLUP_Pivot) TEST MATRIX IS ... " << endl;
	cout << m << endl;
#endif
	if (GaussianLUP_Pivot(m2, pv2, 0.0, determinant) != OK)
	{
		cerr << "GaussianLUP_Pivot failed" << endl;
		return;
	}
#ifdef DEBUG
	cout << "(After GaussianLUP_Pivot) TEST MATRIX IS ... " << endl;
	cout << m << endl;
#endif
	MustBeTrue(m == savem);

	// get solution using LUP results
	if (sflag)
	{
		cout << ">>>>> RUNNING SOLUTION TEST ..." << endl;
		cout << ">>>>> GIVEN M*X = Y, SOLVE FOR X ..." << endl;
		Vector<double> x2(nrows);
		Vector<double> y2(y);
#ifdef DEBUG
		cout << "(Before SolveUsingGaussianLUP_Pivot) TEST MATRIX IS ... " << endl;
		cout << m << endl;
#endif
		if (SolveUsingGaussianLUP_Pivot(m2, x2, y2, pv2, 0.0) != OK)
		{
			cerr << "SolveUsingGaussianLUP_Pivot failed" << endl;
			return;
		}
#ifdef DEBUG
		cout << "(After SolveUsingGaussianLUP_Pivot) TEST MATRIX IS ... " << endl;
		cout << m << endl;
#endif
		cout << "SOLUTION X IS ... " << endl << x2 << endl;
		if (vflag) 
		{
			cout << "VERIFICATION: THE M*X AND Y SHOULD BE THE SAME" << endl;
			cout << "SOLUTION: M*X IS ... " << endl;
			cout << m*x2 << endl;
			cout << "SOLUTION: Y IS ... " << endl;
			cout << y << endl;
			cout << "SOLUTION: RMS FOR Y IS ... ";
			cout << sqrt(dot(y-m*x2, y-m*x2)) << endl;
		}
	}
	MustBeTrue(m == savem);

	// get inverse using LUP results
	if (iflag)
	{
		cout << ">>>>> RUNNING MATRIX INVERSE TEST ..." << endl;
		cout << ">>>>> GIVEN M, SOLVE FOR INVERSE OF M ..." << endl;
		Matrix<double> minv2(nrows, ncols);
#ifdef DEBUG
		cout << "(Before GetInverseUsingGaussianLUP_Pivot) TEST MATRIX IS ... " << endl;
		cout << m << endl;
#endif
		if (GetInverseUsingGaussianLUP_Pivot(m2, minv2, pv2, 0.0) != OK)
		{
			cerr << "GetInverseUsingGaussianLUP_Pivot failed" << endl;
			return;
		}
#ifdef DEBUG
		cout << "(After GetInverseUsingGaussianLUP_Pivot) TEST MATRIX IS ... " << endl;
		cout << m << endl;
#endif
		cout << "INVERSE OF M IS ... " << endl;
		cout << minv2 << endl;
		if (vflag) 
		{
			Matrix<double> mresults(nrows, ncols);
			cout << "VERIFICATION: M*MINV AND MINV*M SHOULD GIVE THE IDENTITY MATRIX" << endl;
			cout << "INVERSE: M*MINV IS ... " << endl;
			cout << m*minv2 << endl;
			mresults = midentity - m*minv2;
			cout << "INVERSE: RESIDUALS FOR M*MINV IS ... " << endl;
			cout << mresults << endl;
			cout << "INVERSE: RMS FOR M*MINV IS ... ";
			cout << rms(mresults) << endl;
			cout << "INVERSE: MINV*M IS ... " << endl;
			cout << minv2*m << endl;
			mresults = midentity - minv2*m;
			cout << "INVERSE: RESIDUALS FOR MINV*M IS ... " << endl;
			cout << mresults << endl;
			cout << "INVERSE: RMS FOR MINV*M IS ... ";
			cout << rms(mresults) << endl;
		}
	}
	MustBeTrue(m == savem);

	// get deteminant using LUP results
	if (dflag)
	{
		cout << ">>>>> RUNNING DETERMINANT TEST ..." << endl;
		cout << ">>>>> GIVEN M, GET DETERMINANT OF M ..." << endl;
#ifdef DEBUG
		cout << "(Before GetDeterminantUsingGaussianLUP_Pivot) TEST MATRIX IS ... " << endl;
		cout << m << endl;
#endif
		if (GetDeterminantUsingGaussianLUP_Pivot(m2, determinant) != OK)
		{
			cerr << "GetDeterminantUsingGaussianLUP_Pivot failed" << endl;
			return;
		}
#ifdef DEBUG
		cout << "(After GetDeterminantUsingGaussianLUP_Pivot) TEST MATRIX IS ... " << endl;
		cout << m << endl;
#endif
		cout << "DETERMINANT IS ... " << determinant << endl;
	}
	MustBeTrue(m == savem);

	cout << ">>>>> ENDING TEST CASE ... " << test.testno << endl;
	return;
}

// test driver
main(int , char **)
{
	// start testing
	cout << "START OF LUP TEST CASES ..." << endl;

	// print out epsilon
	eps = calcEpsilon(double(0));
	cout.precision(6);
	cout.setf(ios::showpoint);
	cout << "EPSILON IS ... " << eps << endl;

	// run test cases
	runit(test1, data1);
	runit(test2, data2);
	runit(test3, data3);
	runit(test4, data4);
	runit(test5, data5);
	runit(test6, data6);
	runit(test7, data7);
	runit(test8, data8);
	runit(test9, data9);
	runit(test10, data10);
	runit(test11, data11);
	runit(test12, data12);
	runit(test13, data13);
	runit(test14, data14);
	runit(test15, data15);
	runit(test16, data16);
	runit(test17, data17);

	// all done
	cout << "END OF LUP TEST CASES ..." << endl << endl;
	return(0);
}

