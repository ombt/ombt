#ifndef __OMBT_GAUSSLUP_H
#define __OMBT_GAUSSLUP_H

// gaussian LUP decomposition definitions

// headers
#include <stdlib.h>
#include <math.h>
#include <iostream>

// local headers
#include <Returns.h>
#include <Debug.h>
#include <matrix/Vector.h>
#include <matrix/Matrix.h>
#include <matrix/Epsilon.h>

namespace ombt {

// calculates gaussian LUP decomposition for a matrix.
template <class T>
int
GaussianLUP_Pivot(Matrix<T> &, Vector<int> &, T, T &);

// solves set of linear equations using results of 
// gaussian LUP decomposition
template <class T>
int
SolveUsingGaussianLUP_Pivot(Matrix<T> &, 
	Vector<T> &, Vector<T> &, Vector<int> &, T);

// calculate the inverse using gaussian LUP results
template <class T>
int
GetInverseUsingGaussianLUP_Pivot(Matrix<T> &, 
	Matrix<T> &, Vector<int> &, T);

// calculate the determinant using gaussian LUP results
template <class T>
int
GetDeterminantUsingGaussianLUP_Pivot(Matrix<T> &, T &);

}

#include <matrix/GaussLUP.cpp>

#endif
