// number class functions

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>
#include <math.h>
#include <string.h>

// local headers
#include "number.h"
#include "debug.h"

// constructors and destructor
Number::Number():
        n(0.0),
	name(new char [NumberDefaultNameSize])
{
	assert(name != 0);
	strcpy(name, NumberDefaultName);
}

Number::Number(double argn):
        n(argn),
	name(new char [NumberDefaultNameSize])
{
	assert(name != 0);
	strcpy(name, NumberDefaultName);
}

Number::Number(const char *argnm):
        n(0.0),
	name(0)
{
	assert(argnm != 0);
	name = new char [strlen(argnm)+1];
	assert(name != 0);
	strcpy(name, argnm);
#ifdef REGISTER
	cerr << "Number:" << name << endl;
#endif
}

Number::Number(const char *argnm, double argn):
        n(argn),
	name(0)
{
	assert(argnm != 0);
	name = new char [strlen(argnm)+1];
	assert(name != 0);
	strcpy(name, argnm);
#ifdef REGISTER
	cerr << "Number:" << name << endl;
#endif
}

Number::Number(const Number &argn): 
        n(argn.n),
	name(0)
{
	assert(argn.name != 0);
	name = new char [strlen(argn.name)+1];
	assert(name != 0);
	strcpy(name, argn.name);
}

Number::~Number() 
{
	delete [] name;
	name = 0;
	n = 0.0;
}

// arithmetic operators
Number &
Number::operator=(const Number &argn)
{
        n = argn.n;
        return(*this);
}

Number &
Number::operator+=(const Number &argn)
{
        n += argn.n;
        return(*this);
}

Number &
Number::operator-=(const Number &argn)
{
        n -= argn.n;
        return(*this);
}

Number &
Number::operator*=(const Number &argn)
{
        n *= argn.n;
        return(*this);
}

Number &
Number::operator/=(const Number &argn)
{
	assert(argn.n != 0.0);
        n /= argn.n;
        return(*this);
}

Number
Number::operator+(const Number &argn) const
{
        return(Number(*this) += argn);
}

Number
Number::operator-(const Number &argn) const
{
        return(Number(*this) -= argn);
}

Number
Number::operator*(const Number &argn) const
{
        return(Number(*this) *= argn);
}

Number
Number::operator/(const Number &argn) const
{
        return(Number(*this) /= argn);
}

// logical operators
int
Number::operator==(const Number &argn) const
{
        return(n == argn.n);
}

int
Number::operator!=(const Number &argn) const
{
        return(n != argn.n);
}

int
Number::operator<(const Number &argn) const
{
        return(n < argn.n);
}

int
Number::operator>(const Number &argn) const
{
        return(n > argn.n);
}

int
Number::operator<=(const Number &argn) const
{
        return(n <= argn.n);
}

int
Number::operator>=(const Number &argn) const
{
        return(n >= argn.n);
}

// arithmetic operators
Number &
Number::operator=(double d)
{
        n = d;
        return(*this);
}

Number &
Number::operator+=(double d)
{
        n += d;
        return(*this);
}

Number &
Number::operator-=(double d)
{
        n -= d;
        return(*this);
}

Number &
Number::operator*=(double d)
{
        n *= d;
        return(*this);
}

Number &
Number::operator/=(double d)
{
        assert(d != 0.0);
        n /= d;
        return(*this);
}

Number
Number::operator+(double d) const
{
        return(Number(*this) += d);
}

Number
Number::operator-(double d) const
{
        return(Number(*this) -= d);
}

Number
Number::operator*(double d) const
{
        return(Number(*this) *= d);
}

Number
Number::operator/(double d) const
{
        return(Number(*this) /= d);
}

Number
operator+(double d, const Number &argn)
{
        return(Number(d) += argn);
}

Number
operator-(double d, const Number &argn)
{
        return(Number(d) -= argn);
}

Number
operator*(double d, const Number &argn)
{
        return(Number(d) *= argn);
}

Number
operator/(double d, const Number &argn)
{
        return(Number(d) /= argn);
}

// logical operators
int
Number::operator==(double d) const
{
        return(n == d);
}

int
Number::operator!=(double d) const
{
        return(n != d);
}

int
Number::operator<(double d) const
{
        return(n < d);
}

int
Number::operator>(double d) const
{
        return(n > d);
}

int
Number::operator<=(double d) const
{
        return(n <= d);
}

int
Number::operator>=(double d) const
{
        return(n >= d);
}

int
operator==(double d, const Number &argn)
{
        return(d == argn.n);
}

int
operator!=(double d, const Number &argn)
{
        return(d != argn.n);
}

int
operator<(double d, const Number &argn)
{
        return(d < argn.n);
}

int
operator>(double d, const Number &argn)
{       
        return(d > argn.n);
}

int
operator<=(double d, const Number &argn)
{
        return(d <= argn.n);
}

int
operator>=(double d, const Number &argn)
{
        return(d >= argn.n);
}

// mathematical functions
Number
exp(const Number &p)
{
        return(Number(exp(p.n)));
}

Number
log(const Number &n)
{
        assert(n.n != 0.0);
        return(Number(log(n.n)));
}

Number
log10(const Number &n)
{
        return(Number(log10(n.n)));
}

Number
pow(const Number &b, const Number &p)
{
        if (b.n == 0.0)
                return(Number(0.0));
        else
                return(Number(pow(b.n, p.n)));
}

Number
sqrt(const Number &n)
{
        if (n.n == 0.0)
                return(Number(0.0));
        else
                return(Number(sqrt(n.n)));
}


Number
sin(const Number &n)
{
        return(Number(sin(n.n)));
}

Number
cos(const Number &n)
{
        return(Number(cos(n.n)));
}

Number
tan(const Number &n)
{
        return(Number(tan(n.n)));
}

Number
asin(const Number &n)
{
        return(Number(asin(n.n)));
}

Number
acos(const Number &n)
{
        return(Number(acos(n.n)));
}

Number
atan(const Number &n)
{
        return(Number(atan(n.n)));
}


Number
sinh(const Number &n)
{
        return(Number(sinh(n.n)));
}

Number
cosh(const Number &n)
{
        return(Number(cosh(n.n)));
}

Number
tanh(const Number &n)
{
        return(Number(tanh(n.n)));
}

Number
asinh(const Number &n)
{
        return(Number(asinh(n.n)));
}

Number
acosh(const Number &n)
{
        return(Number(acosh(n.n)));
}

Number
atanh(const Number &n)
{
        return(Number(atanh(n.n)));
}

double
abs(const Number &n)
{
        return(n.n < 0.0 ? -n.n : n.n);
}

// read and write a number
Number &
Number::operator()()
{
	return(*this);
}

ostream &
operator<<(ostream &os, const Number &n)
{
	if (strcmp(n.name, NumberDefaultName) != 0)
		os << n.name << ": ";
        os << n.n;
        return(os);
}

istream &
operator>>(istream &is, Number &n)
{
	if (strcmp(n.name, NumberDefaultName) != 0)
		cout << "Enter " << n.name << ": ";
	is >> n.n;
	return(is);
}
