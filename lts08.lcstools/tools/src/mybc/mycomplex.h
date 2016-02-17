#ifndef __COMPLEX_H
#define __COMPLEX_H
// complex number class

// headers
#include <stdlib.h>
#include <iostream.h>
#include <math.h>
#include <errno.h>

// local headers
#include "returns.h"
#include "debug.h"

// forward declarations (required by SUN C++ 4.X compiler)
template <class T> class Complex;
template <class T> Complex<T> operator+(T, const Complex<T> &);
template <class T> Complex<T> operator-(T, const Complex<T> &);
template <class T> Complex<T> operator*(T, const Complex<T> &);
template <class T> Complex<T> operator/(T, const Complex<T> &);
template <class T> int operator==(T, const Complex<T> &);
template <class T> int operator!=(T, const Complex<T> &);
template <class T> int operator<(T, const Complex<T> &);
template <class T> int operator>(T, const Complex<T> &);
template <class T> int operator<=(T, const Complex<T> &);
template <class T> int operator>=(T, const Complex<T> &);
template <class T> ostream &operator<<(ostream &, const Complex<T> &);
template <class T> Complex<T> exp(const Complex<T> &);
template <class T> Complex<T> log(const Complex<T> &);
template <class T> Complex<T> log10(const Complex<T> &);
template <class T> Complex<T> pow(const Complex<T> &, const Complex<T> &);
template <class T> Complex<T> sqrt(const Complex<T> &);
template <class T> Complex<T> sin(const Complex<T> &);
template <class T> Complex<T> cos(const Complex<T> &);
template <class T> Complex<T> tan(const Complex<T> &);
template <class T> Complex<T> asin(const Complex<T> &);
template <class T> Complex<T> acos(const Complex<T> &);
template <class T> Complex<T> atan(const Complex<T> &);
template <class T> Complex<T> sinh(const Complex<T> &);
template <class T> Complex<T> cosh(const Complex<T> &);
template <class T> Complex<T> tanh(const Complex<T> &);
template <class T> Complex<T> asinh(const Complex<T> &);
template <class T> Complex<T> acosh(const Complex<T> &);
template <class T> Complex<T> atanh(const Complex<T> &);
template <class T> Complex<T> conj(const Complex<T> &);
template <class T> T abs(const Complex<T> &);
template <class T> T arg(const Complex<T> &);
template <class T> T norm(const Complex<T> &);
template <class T> T real(const Complex<T> &);
template <class T> T imag(const Complex<T> &);
template <class T> T rabs(T);

// complex class definition
template <class T> class Complex
{
public:
	// constructors and destructor
	Complex();
	Complex(T);
	Complex(T, T);
	Complex(const Complex &);
	~Complex();

	// arithmetic operations
	Complex &operator=(const Complex &);
	Complex &operator+=(const Complex &);
	Complex &operator-=(const Complex &);
	Complex &operator*=(const Complex &);
	Complex &operator/=(const Complex &);
	Complex operator+(const Complex &) const;
	Complex operator-(const Complex &) const;
	Complex operator*(const Complex &) const;
	Complex operator/(const Complex &) const;

	// logical operators 
	int operator==(const Complex &) const;
	int operator!=(const Complex &) const;
	int operator<(const Complex &) const;
	int operator>(const Complex &) const;
	int operator<=(const Complex &) const;
	int operator>=(const Complex &) const;
	int operator||(const Complex &) const;
	int operator&&(const Complex &) const;

	// arithmetic operations
	Complex &operator=(T);
	Complex &operator+=(T);
	Complex &operator-=(T);
	Complex &operator*=(T);
	Complex &operator/=(T);
	Complex operator+(T) const;
	Complex operator-(T) const;
	Complex operator*(T) const;
	Complex operator/(T) const;
	friend Complex<T> operator+(T, const Complex<T> &);
	friend Complex<T> operator-(T, const Complex<T> &);
	friend Complex<T> operator*(T, const Complex<T> &);
	friend Complex<T> operator/(T, const Complex<T> &);

	// logical operators 
	int operator==(T) const;
	int operator!=(T) const;
	int operator<(T) const;
	int operator>(T) const;
	int operator<=(T) const;
	int operator>=(T) const;
	friend int operator==(T, const Complex<T> &);
	friend int operator!=(T, const Complex<T> &);
	friend int operator< (T, const Complex<T> &);
	friend int operator>(T, const Complex<T> &);
	friend int operator<=(T, const Complex<T> &);
	friend int operator>=(T, const Complex<T> &);

	// mathematical functions
	friend Complex<T> exp(const Complex<T> &);
	friend Complex<T> log(const Complex<T> &);
	friend Complex<T> log10(const Complex<T> &);
	friend Complex<T> pow(const Complex<T> &, const Complex<T> &);
	friend Complex<T> sqrt(const Complex<T> &);
	friend Complex<T> sin(const Complex<T> &);
	friend Complex<T> cos(const Complex<T> &);
	friend Complex<T> tan(const Complex<T> &);
	friend Complex<T> asin(const Complex<T> &);
	friend Complex<T> acos(const Complex<T> &);
	friend Complex<T> atan(const Complex<T> &);
	friend Complex<T> sinh(const Complex<T> &);
	friend Complex<T> cosh(const Complex<T> &);
	friend Complex<T> tanh(const Complex<T> &);
	friend Complex<T> asinh(const Complex<T> &);
	friend Complex<T> acosh(const Complex<T> &);
	friend Complex<T> atanh(const Complex<T> &);
	friend Complex<T> conj(const Complex<T> &);
	friend T abs(const Complex<T> &);
	friend T arg(const Complex<T> &);
	friend T norm(const Complex<T> &);
	friend T real(const Complex<T> &);
	friend T imag(const Complex<T> &);
	friend T rabs(T);

	// miscellaneous
	friend ostream &operator<< (ostream &, const Complex<T> &);

protected:
	// internal data
	T x, y;
};

#endif
