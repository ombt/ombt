#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <interpolation/CubicSpline.h>

using namespace ombt;
using namespace std;

main(int argc, char **argv)
{
	cerr << endl << "TESTS FOR CubicSpline ..." << endl;

	CubicSpline<double> cs;
	CubicSpline<double>::Coordinates xs;
	CubicSpline<double>::Coordinates ys;

	xs.resize(100);
	ys.resize(100);

	cerr << endl << "TESTS FOR CubicSpline USING sin() ..." << endl;
	for (int i=0; i<100; ++i)
	{
		xs[i] = i*M_PI/50.0;
		ys[i] = ::sin(xs[i]);
	}

	cs.calculate(xs, ys, 0.0, 0.0);
	cerr << endl << cs << endl << endl;

	for (int i=0; i<120; ++i)
	{
		double x = i*M_PI/60;
		double y = ::sin(x);
		double yspline = cs(x);
		cerr << "(i,x,y,ysp,y-ysp) = ("
                     << i << ","
                     << x << ","
                     << y << ","
                     << yspline << ","
                     << (y-yspline) << ")" << endl;
	}

	cerr << endl << "TESTS FOR CubicSpline USING cos() ..." << endl;
	for (int i=0; i<100; ++i)
	{
		xs[i] = i*M_PI/50.0;
		ys[i] = ::cos(xs[i]);
	}

	cs.calculate(xs, ys, 0.0, 0.0);
	cerr << endl << cs << endl << endl;

	for (int i=0; i<120; ++i)
	{
		double x = i*M_PI/60;
		double y = ::cos(x);
		double yspline = cs(x);
		cerr << "(i,x,y,ysp,y-ysp) = ("
                     << i << ","
                     << x << ","
                     << y << ","
                     << yspline << ","
                     << (y-yspline) << ")" << endl;
	}

	cerr << endl << "TESTS FOR CubicSpline USING exp() ..." << endl;
	for (int i=0; i<100; ++i)
	{
		xs[i] = i*M_PI/50.0;
		ys[i] = ::exp(xs[i]);
	}

	cs.calculate(xs, ys, 0.0, 0.0);
	cerr << endl << cs << endl << endl;

	for (int i=0; i<120; ++i)
	{
		double x = i*M_PI/60;
		double y = ::exp(x);
		double yspline = cs(x);
		cerr << "(i,x,y,ysp,y-ysp) = ("
                     << i << ","
                     << x << ","
                     << y << ","
                     << yspline << ","
                     << (y-yspline) << ")" << endl;
	}

	return(0);
}
