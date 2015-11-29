#ifndef __MATRIX_H
#define __MATRIX_H

// matrix class definitions

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
#include "epsilon.h"

// forward declarations
template <class T> class Matrix;
template <class T> Vector<T> operator*(const Vector<T> &, const Matrix<T> &);
template <class T> Matrix<T> operator*(const T &, const Matrix<T> &);
template <class T> Matrix<T> operator*(T, const Matrix<T> &);
template <class T> ostream &operator<<(ostream &, const Matrix<T> &);

// matrix class definition
template <class T> class Matrix
{
public:
	// constructors and destructor
	Matrix(unsigned int, unsigned int);
	Matrix(const Matrix<T> &);
	~Matrix();

	// assignment operators and accessors
	Matrix<T> &operator=(const Matrix<T> &);
	T &operator[](unsigned int);
	T &operator[](unsigned int) const;
	T &operator()(unsigned int, unsigned int);
	T &operator()(unsigned int, unsigned int) const;

	// matrix operations
	Matrix<T> &operator+=(const Matrix<T> &);
	Matrix<T> &operator-=(const Matrix<T> &);
	Matrix<T> &operator*=(const Matrix<T> &);
	Matrix<T> operator+(const Matrix<T> &) const;
	Matrix<T> operator-(const Matrix<T> &) const;
	Matrix<T> operator*(const Matrix<T> &) const;

	// matrix and vector operations
	Vector<T> operator*(const Vector<T> &) const;
	friend Vector<T> operator*(const Vector<T> &, const Matrix<T> &);

	// matrix and scalar operations
	Matrix<T> &operator*=(const T &);
	Matrix<T> &operator/=(const T &);
	Matrix<T> operator*(const T &) const;
	Matrix<T> operator/(const T &) const;
	friend Matrix<T> operator*(const T &, const Matrix<T> &);

	// logical operators
	int operator==(const Matrix<T> &) const;
	int operator!=(const Matrix<T> &) const;

	// other functions
	inline unsigned int getRows() { return(nrows); }
	inline unsigned int getCols() { return(ncols); }
	void dump(ostream &) const;
	friend ostream &operator<<(ostream &, const Matrix<T> &);

	// get epsilon 
	T getEpsilon() {
		return(epsilon);
	}

protected:
#ifdef SORTANDADD
	// specialized routines for adding two arrays.
	T sortAndAdd(const Vector<T> &, const Vector<T> &, unsigned int) const;
#endif

protected:
	// internal data
	T *matrix;
	unsigned int nrows, ncols;
	T epsilon;
};

#endif
