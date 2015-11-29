#include <matrix/Matrix.h>
#include <complex/Complex.h>

using namespace ombt;
using namespace std;

main(int argc, char **argv)
{
	MustBeTrue(argc == 4);
	srand(atol(argv[1]));
	unsigned int nrows = atoi(argv[2]);
	unsigned int ncols = atoi(argv[3]);

	Matrix<Complex<double> > m(nrows, ncols);
	cerr << "m is ... " << m << endl;

	for (int ir = 0 ; ir < nrows; ir++)
	{
		for (int ic = 0; ic < ncols; ic++)
		{
			m(ir, ic) = Complex<double>(rand(), rand());
		}
	}
	cerr << "m is ... " << m << endl;

	cerr << "m*m is ... " << m*m << endl;

	cerr << "m+m is ... " << m+m << endl;

	cerr << "m-m is ... " << m-m << endl;

	cerr << "m*m/5.0 is ... " << m*m/5.0 << endl;

	//cerr << "m*m*5.0 is ... " << m*m*5.0 << endl;

	return(0);
}
