#ifndef __GAUSSLUP_H
#define __GAUSSLUP_H

// gaussian LUP decomposition definitions

#ifdef __GNUC__
#pragma interface
#endif

// headers
#include <stdlib.h>
#include <iostream.h>
#include <math.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "vector.h"
#include "matrix.h"
#include "epsilon.h"

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

#endif
