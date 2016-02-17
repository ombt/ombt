#ifndef __COMPLEX_H
#define __COMPLEX_H

// complex number class

// headers
#include <stdlib.h>
#include <iostream.h>
#include <math.h>
#include <errno.h>

// forward declaration
class ComplexNumber;

// complex class definition
class ComplexNumber {
public:
        // constructors and destructor
        ComplexNumber(double = 0.0, double = 0.0);
        ComplexNumber(const ComplexNumber &);
        virtual ~ComplexNumber();

        // arithmetic operations
        ComplexNumber &operator=(const ComplexNumber &);
        ComplexNumber &operator+=(const ComplexNumber &);
        ComplexNumber &operator-=(const ComplexNumber &);
        ComplexNumber &operator*=(const ComplexNumber &);
        ComplexNumber &operator/=(const ComplexNumber &);
        ComplexNumber operator+(const ComplexNumber &) const;
        ComplexNumber operator-(const ComplexNumber &) const;
        ComplexNumber operator*(const ComplexNumber &) const;
        ComplexNumber operator/(const ComplexNumber &) const;

        // logical operators 
        int operator==(const ComplexNumber&) const;
        int operator!=(const ComplexNumber&) const;
        int operator<(const ComplexNumber&) const;
        int operator>(const ComplexNumber&) const;
        int operator<=(const ComplexNumber&) const;
        int operator>=(const ComplexNumber&) const;

        // arithmetic operations
        ComplexNumber &operator=(double);
        ComplexNumber &operator+=(double);
        ComplexNumber &operator-=(double);
        ComplexNumber &operator*=(double);
        ComplexNumber &operator/=(double);
        ComplexNumber operator+(double) const;
        ComplexNumber operator-(double) const;
        ComplexNumber operator*(double) const;
        ComplexNumber operator/(double) const;
        friend ComplexNumber operator+(double, const ComplexNumber &);
        friend ComplexNumber operator-(double, const ComplexNumber &);
        friend ComplexNumber operator*(double, const ComplexNumber &);
        friend ComplexNumber operator/(double, const ComplexNumber &);

        // logical operators 
        int operator==(double) const;
        int operator!=(double) const;
        int operator<(double) const;
        int operator>(double) const;
        int operator<=(double) const;
        int operator>=(double) const;
        friend int operator==(double, const ComplexNumber &);
        friend int operator!=(double, const ComplexNumber &);
        friend int operator<(double, const ComplexNumber &);
        friend int operator>(double, const ComplexNumber &);
        friend int operator<=(double, const ComplexNumber &);
        friend int operator>=(double, const ComplexNumber &);

        // mathematical functions
        friend ComplexNumber exp(const ComplexNumber &);
        friend ComplexNumber log(const ComplexNumber &);
        friend ComplexNumber log10(const ComplexNumber &);
        friend ComplexNumber pow(const ComplexNumber &,
                                 const ComplexNumber &);
        friend ComplexNumber sqrt(const ComplexNumber &);
        friend ComplexNumber sin(const ComplexNumber &);
        friend ComplexNumber cos(const ComplexNumber &);
        friend ComplexNumber tan(const ComplexNumber &);
        friend ComplexNumber asin(const ComplexNumber &);
        friend ComplexNumber acos(const ComplexNumber &);
        friend ComplexNumber atan(const ComplexNumber &);
        friend ComplexNumber sinh(const ComplexNumber &);
        friend ComplexNumber cosh(const ComplexNumber &);
        friend ComplexNumber tanh(const ComplexNumber &);
        friend ComplexNumber asinh(const ComplexNumber &);
        friend ComplexNumber acosh(const ComplexNumber &);
        friend ComplexNumber atanh(const ComplexNumber &);
        friend ComplexNumber conj(const ComplexNumber &);
        friend double abs(const ComplexNumber &);
        friend double arg(const ComplexNumber &);
        friend double norm(const ComplexNumber &);
        friend double real(const ComplexNumber &);
        friend double imag(const ComplexNumber &);

        // miscellaneous
        friend ostream &operator<<(ostream &, const ComplexNumber &);

protected:
        // internal data
        double x, y;
};
                   
#endif
