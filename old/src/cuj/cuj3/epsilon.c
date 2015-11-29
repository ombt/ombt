#include "epsilon.h"

#ifdef SHORTVERSION

template <class T>
T
calcEpsilon(T)
{
	T f1 = 1.0;
	T f2 = 1.0;
	T oldf2 = 0.0;

	for (f2 /= 2.0; ((f1-f2) != f1); oldf2=f2, f2 /= 2.0) ;
	return(2.0*oldf2);
}

#else

template <class T>
T
calcEpsilon(T)
{
	T f1 = 1.0;
	T f2 = 1.0;
	T oldf2 = 1.0;

	for (int i = 1; i <= sizeof(T)*8; i++)
	{
		f2 /= 2.0;
		if ((f1-f2) == f1)
			return(2.0*oldf2);
		oldf2 = f2;
	}
	return(0);
}

#endif
