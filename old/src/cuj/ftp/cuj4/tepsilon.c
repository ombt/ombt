#include <stdlib.h>
#include <iostream.h>

#include "epsilon.h"
#include "complex.h"

main()
{
	cerr << "long epsilon is ... " << 
		calcEpsilon(long(0)) << endl;

	cerr << "float epsilon is ... " << 
		calcEpsilon(float(0)) << endl;

	cerr << "double epsilon is ... " << 
		calcEpsilon(double(0)) << endl;

	cerr << "long double epsilon is ... " << 
		calcEpsilon((long double)(0)) << endl;

	cerr << "Complex<double> epsilon is ... " << 
		calcEpsilon(Complex<double>(0.0)) << endl;

	cerr << "Complex<long double> epsilon is ... " << 
		calcEpsilon(Complex<long double>(0.0)) << endl;

	cerr << "Complex<float> epsilon is ... " << 
		calcEpsilon(Complex<float>(0.0)) << endl;

	return(0);
}
