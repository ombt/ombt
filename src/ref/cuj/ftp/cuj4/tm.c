// gaussian LUP header, matrix header, vector header, etc.
#include "matrix.h"

// main entry 
main(int argc, char **argv)
{
	Matrix<double> m(3,6), n(6, 5);
	for (int r = 0; r < 3; r++)
	{
		for (int c = 0; c < 6; c++)
		{
			cout << "address of m(" << r << "," << c << ") = ";
			cout << &m(r,c) << endl;
		}
	}
	for (int i = 0; i < 3*6; i++)
	{
		cout << "address of m[" << i << "] = ";
		cout << &m[i] << endl;
	}
	for (r = 0; r < 3; r++)
	{
		for (int c = 0; c < 6; c++)
		{
			m(r,c) = r*6+c;
		}
	}
	for (i = 0; i < 3*6; i++)
	{
		cout << "contents for m[" << i << "] = ";
		cout << m[i] << endl;
	}
	return(0);
}
