#ifndef __NUMBER_H
#define __NUMBER_H

// number class

// headers
#include <stdlib.h>
#include <iostream.h>
#include <math.h>
#include <errno.h>

// local definitions
#define NumberDefaultName "NONAME"
#define NumberDefaultNameSize 7

// forward declaration
class Number;

// number class definition
class Number {
public:
        // constructors and destructor
        Number();
        Number(double);
        Number(const char *);
        Number(const char *, double);
        Number(const Number &);
        ~Number();

        // arithmetic operations
        Number &operator=(const Number &);
        Number &operator+=(const Number &);
        Number &operator-=(const Number &);
        Number &operator*=(const Number &);
        Number &operator/=(const Number &);
        Number operator+(const Number &) const;
        Number operator-(const Number &) const;
        Number operator*(const Number &) const;
        Number operator/(const Number &) const;

        // logical operators 
        int operator==(const Number&) const;
        int operator!=(const Number&) const;
        int operator<(const Number&) const;
        int operator>(const Number&) const;
        int operator<=(const Number&) const;
        int operator>=(const Number&) const;

        // arithmetic operations
        Number &operator=(double);
        Number &operator+=(double);
        Number &operator-=(double);
        Number &operator*=(double);
        Number &operator/=(double);
        Number operator+(double) const;
        Number operator-(double) const;
        Number operator*(double) const;
        Number operator/(double) const;
        friend Number operator+(double, const Number &);
        friend Number operator-(double, const Number &);
        friend Number operator*(double, const Number &);
        friend Number operator/(double, const Number &);

        // logical operators 
        int operator==(double) const;
        int operator!=(double) const;
        int operator<(double) const;
        int operator>(double) const;
        int operator<=(double) const;
        int operator>=(double) const;
        friend int operator==(double, const Number &);
        friend int operator!=(double, const Number &);
        friend int operator<(double, const Number &);
        friend int operator>(double, const Number &);
        friend int operator<=(double, const Number &);
        friend int operator>=(double, const Number &);

        // mathematical functions
        friend Number exp(const Number &);
        friend Number log(const Number &);
        friend Number log10(const Number &);
        friend Number pow(const Number &, const Number &);
        friend Number sqrt(const Number &);
        friend Number sin(const Number &);
        friend Number cos(const Number &);
        friend Number tan(const Number &);
        friend Number asin(const Number &);
        friend Number acos(const Number &);
        friend Number atan(const Number &);
        friend Number sinh(const Number &);
        friend Number cosh(const Number &);
        friend Number tanh(const Number &);
        friend Number asinh(const Number &);
        friend Number acosh(const Number &);
        friend Number atanh(const Number &);
        friend double abs(const Number &);

        // miscellaneous
        Number &operator()();
        friend ostream &operator<<(ostream &, const Number &);
        friend istream &operator>>(istream &, Number &);

protected:
        // internal data
        double n;
	char *name;
};
                   
#endif
