// calculate a minimum precission for a data type
#include "epsilon.h"

template <class T>
T
calcEpsilon(T)
{
	T f1 = 1.0;
	T f2 = 1.0;
	T oldf2 = 0.0;

	for (f2 /= 2.0; ((f1-f2) != f1); oldf2=f2, f2 /= 2.0) ;
	return(T(2.0)*oldf2);
}
