#ifndef __VECTOR_H
#define __VECTOR_H
// vector class

#ifdef __GNUC__
#pragma interface
#endif

// headers
#include <stdlib.h>
#include <iostream.h>
#include <math.h>
#include <errno.h>

// local headers
#include "returns.h"
#include "debug.h"

// forward declarations
template <class T> class Vector;
template <class T> Vector<T> conj(const Vector<T> &);
template <class T> T dot(const Vector<T> &, const Vector<T> &);
template <class T> T norm(const Vector<T> &);

// vector class definition
template <class T> class Vector
{
public:
	 // constructors and destructor
	 Vector(unsigned int);
	 Vector(const T *, unsigned int);
	 Vector(const Vector<T> &);
	 ~Vector();

	 // assignment
	 Vector<T> &operator=(const Vector<T> &);
	 T &operator[](unsigned int);
	 T &operator[](unsigned int) const;

	 // vector operations
	 Vector<T> &operator+=(const Vector<T> &);
	 Vector<T> &operator-=(const Vector<T> &);
	 Vector<T> operator+(const Vector<T> &) const;
	 Vector<T> operator-(const Vector<T> &) const;

	 // arithmetic operations
	 Vector<T> &operator*=(const T &);
	 Vector<T> &operator/=(const T &);
	 Vector<T> operator*(const T &) const;
	 Vector<T> operator/(const T &) const;
	 friend Vector<T> operator*(const T &n, const Vector<T> &v) {
		return(Vector<T>(v) *= n);
	}

	 // logical operators
	 int operator==(const Vector<T> &) const;
	 int operator!=(const Vector<T> &) const;

	 // vector products
	 friend Vector<T> conj(const Vector<T> &);
	 friend T dot(const Vector<T> &, const Vector<T> &);
	 friend T norm(const Vector<T> &);

	 // miscellaneous 
	 inline unsigned int getDimension() const { 
		  return(dimension);
	 }
	 void dump(ostream &) const;
	 friend ostream &operator<<(ostream &os, const Vector<T> &v) {
		v.dump(os);
		return(os);
	}

protected:
	 // data
	 unsigned int dimension;
	 T *vector;
};

#endif
