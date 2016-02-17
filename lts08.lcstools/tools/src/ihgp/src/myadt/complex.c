// complex class functions

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>
#include <math.h>

// local headers
#include "complex.h"

// local constants
static const ComplexNumber plusi(0.0, 1.0);
static const ComplexNumber minusi(0.0, -1.0);
static const ComplexNumber plusone(1.0, 0.0);
static const ComplexNumber minusone(-1.0, 0.0);

// local functions
static double rabs(double r)
{
	return((r >= 0.0) ? r : -1.0*r);
}

// constructors and destructor
ComplexNumber::ComplexNumber(double argx, double argy):
        x(argx), y(argy)
{
        // do nothing
}

ComplexNumber::ComplexNumber(const ComplexNumber &argc): 
        x(argc.x), y(argc.y)
{
        // do nothing
}


ComplexNumber::~ComplexNumber() 
{
        // do nothing
}

// arithmetic operators
ComplexNumber &
ComplexNumber::operator=(const ComplexNumber &c)
{
        x = c.x;
        y = c.y;
        return(*this);
}

ComplexNumber &
ComplexNumber::operator+=(const ComplexNumber &c)
{
        x += c.x;
        y += c.y;
        return(*this);
}

ComplexNumber &
ComplexNumber::operator-=(const ComplexNumber &c)
{
        x -= c.x;
        y -= c.y;
        return(*this);
}

ComplexNumber &
ComplexNumber::operator*=(const ComplexNumber &c)
{
        double tmpx = x*c.x - y*c.y;
        double tmpy = x*c.y + y*c.x;
        x = tmpx;
        y = tmpy;
        return(*this);
}

ComplexNumber &
ComplexNumber::operator/=(const ComplexNumber &c)
{
        assert(c.x != 0.0 || c.y != 0.0);
        double tmpm = c.x*c.x + c.y*c.y;
        double tmpx = (x*c.x + y*c.y)/tmpm;
        double tmpy = (y*c.x - x*c.y)/tmpm;
        x = tmpx;
        y = tmpy;
        return(*this);
}

ComplexNumber
ComplexNumber::operator+(const ComplexNumber &c) const
{
        return(ComplexNumber(*this) += c);
}

ComplexNumber
ComplexNumber::operator-(const ComplexNumber &c) const
{
        return(ComplexNumber(*this) -= c);
}

ComplexNumber
ComplexNumber::operator*(const ComplexNumber &c) const
{
        return(ComplexNumber(*this) *= c);
}

ComplexNumber
ComplexNumber::operator/(const ComplexNumber &c) const
{
        return(ComplexNumber(*this) /= c);
}

// logical operators
int
ComplexNumber::operator==(const ComplexNumber &c) const
{
        return((x == c.x) && (y == c.y));
}

int
ComplexNumber::operator!=(const ComplexNumber &c) const
{
        return((x != c.x) || (y != c.y));
}

int
ComplexNumber::operator<(const ComplexNumber &c) const
{
        return(abs(*this) < abs(c));
}

int
ComplexNumber::operator>(const ComplexNumber &c) const
{
        return(abs(*this) > abs(c));
}

int
ComplexNumber::operator<=(const ComplexNumber &c) const
{
        return(abs(*this) <= abs(c));
}

int
ComplexNumber::operator>=(const ComplexNumber &c) const
{
        return(abs(*this) >= abs(c));
}

// arithmetic operators
ComplexNumber &
ComplexNumber::operator=(double r)
{
        x = r;
        y = 0.0;
        return(*this);
}

ComplexNumber &
ComplexNumber::operator+=(double r)
{
        x += r;
        return(*this);
}

ComplexNumber &
ComplexNumber::operator-=(double r)
{
        x -= r;
        return(*this);
}

ComplexNumber &
ComplexNumber::operator*=(double r)
{
        x *= r;
        y *= r;
        return(*this);
}

ComplexNumber &
ComplexNumber::operator/=(double r)
{
        assert(r != 0.0);
        x /= r;
        y /= r;
        return(*this);
}

ComplexNumber
ComplexNumber::operator+(double r) const
{
        return(ComplexNumber(*this) += r);
}

ComplexNumber
ComplexNumber::operator-(double r) const
{
        return(ComplexNumber(*this) -= r);
}

ComplexNumber
ComplexNumber::operator*(double r) const
{
        return(ComplexNumber(*this) *= r);
}

ComplexNumber
ComplexNumber::operator/(double r) const
{
        return(ComplexNumber(*this) /= r);
}

ComplexNumber
operator+(double r, const ComplexNumber &c)
{
        return(ComplexNumber(r) += c);
}

ComplexNumber
operator-(double r, const ComplexNumber &c)
{
        return(ComplexNumber(r) -= c);
}

ComplexNumber
operator*(double r, const ComplexNumber &c)
{
        return(ComplexNumber(r) *= c);
}

ComplexNumber
operator/(double r, const ComplexNumber &c)
{
        return(ComplexNumber(r) /= c);
}

// logical operators
int
ComplexNumber::operator==(double r) const
{
        return((x == r) && (y == 0.0));
}

int
ComplexNumber::operator!=(double r) const
{
        return((x != r) || (y != 0.0));
}

int
ComplexNumber::operator<(double r) const
{
        return(abs(*this) < rabs(r));
}

int
ComplexNumber::operator>(double r) const
{
        return(abs(*this) > rabs(r));
}

int
ComplexNumber::operator<=(double r) const
{
        return(abs(*this) <= rabs(r));
}

int
ComplexNumber::operator>=(double r) const
{
        return(abs(*this) >= rabs(r));
}

int
operator==(double r, const ComplexNumber &c)
{
        return((c.x == r) && (c.y == 0.0));
}

int
operator!=(double r, const ComplexNumber &c)
{
        return((c.x != r) || (c.y != 0.0));
}

int
operator<(double r, const ComplexNumber &c)
{
        return(rabs(r) < abs(c));
}

int
operator>(double r, const ComplexNumber &c)
{       
        return(rabs(r) > abs(c));
}

int
operator<=(double r, const ComplexNumber &c)
{
        return(rabs(r) <= abs(c));
}

int
operator>=(double r, const ComplexNumber &c)
{
        return(rabs(r) >= abs(c));
}

// mathematical functions
ComplexNumber
exp(const ComplexNumber &p)
{
        return(ComplexNumber(exp(p.x)*cos(p.y), exp(p.x)*sin(p.y)));
}

ComplexNumber
log(const ComplexNumber &c)
{
        assert((c.x != 0.0) || (c.y != 0.0));
        return(ComplexNumber(log(abs(c)), arg(c)));
}

ComplexNumber
log10(const ComplexNumber &c)
{
        return(log(c)/ComplexNumber(log(10.0)));
}

ComplexNumber
pow(const ComplexNumber &b, ComplexNumber &p)
{
        if (b == 0.0)
                return(ComplexNumber(0.0));
        else
                return(exp(p*log(b)));
}

ComplexNumber
sqrt(const ComplexNumber &c)
{
        if (c == 0.0)
                return(ComplexNumber(0.0));
        else
                return(exp(ComplexNumber(0.5)*log(c)));
}


ComplexNumber
sin(const ComplexNumber &c)
{
        return(ComplexNumber(sin(c.x)*cosh(c.x), cos(c.x)*sinh(c.y)));
}

ComplexNumber
cos(const ComplexNumber &c)
{
        return(ComplexNumber(cos(c.x)*cosh(c.x), -1.0*sin(c.x)*sinh(c.y)));
}

ComplexNumber
tan(const ComplexNumber &c)
{
        return(sin(c)/cos(c));
}

ComplexNumber
asin(const ComplexNumber &c)
{
        return(minusi*log(plusi*c+sqrt(plusone-c*c)));
}

ComplexNumber
acos(const ComplexNumber &c)
{
        return(minusi*log(c+plusi*sqrt(plusone-c*c)));
}

ComplexNumber
atan(const ComplexNumber &c)
{
        return(ComplexNumber(0.5)*plusi*log((plusi+c)/(plusi-c)));
}


ComplexNumber
sinh(const ComplexNumber &c)
{
        return(ComplexNumber(sinh(c.x)*cos(c.x), cosh(c.x)*sin(c.y)));
}

ComplexNumber
cosh(const ComplexNumber &c)
{
        return(ComplexNumber(cosh(c.x)*cos(c.x), sinh(c.x)*sin(c.y)));
}

ComplexNumber
tanh(const ComplexNumber &c)
{
        double divider = cosh(2.0*c.x)*cos(2.0*c.y);
        return(ComplexNumber(sinh(2.0*c.x)/divider, cos(2.0*c.y)/divider));
}

ComplexNumber
asinh(const ComplexNumber &c)
{
        return(log(c+sqrt(c*c+plusone)));
}

ComplexNumber
acosh(const ComplexNumber &c)
{
        return(log(c+sqrt(c*c-plusone)));
}

ComplexNumber
atanh(const ComplexNumber &c)
{
        return(ComplexNumber(0.5)*log((plusone+c)/(plusone-c)));
}

ComplexNumber
conj(const ComplexNumber &c)
{
        return(ComplexNumber(c.x, -1.0*c.y));
}

double
abs(const ComplexNumber &c)
{
        return(sqrt(c.x*c.x+c.y*c.y));
}

double
norm(const ComplexNumber &c)
{
        return(sqrt(c.x*c.x+c.y*c.y));
}

double
arg(const ComplexNumber &c)
{
        if (c.x > 0.0)
        {
                if (c.y > 0.0)
                {
                        return(atan(c.y/c.x));
                }
                else if (c.y < 0.0)
                {
                        return(2.0*M_PI-atan(rabs(c.y/c.x)));
                }
                else
                {
                        return(0.0);
                }
        }
        else if (c.x < 0.0)
        {
                if (c.y > 0.0)
                {
                        return(M_PI-atan(rabs(c.y/c.x)));
                }
                else if (c.y < 0.0)
                {
                        return(M_PI+atan(c.y/c.x));
                }
                else
                {
                        return(M_PI);
                }
        }
        else
        {
                if (c.y > 0.0)
                {
                        return(M_PI/2.0);
                }
                else if (c.y < 0.0)
                {
                        return(-3.0*M_PI/2.0);
                }
                else
                {
                        return(0.0);
                }
        }
}

double
real(const ComplexNumber &c)
{
        return(c.x);
}

double
imag(const ComplexNumber &c)
{
        return(c.y);
}

/* print complex number */
ostream &
operator<<(ostream &os, const ComplexNumber &c)
{
        os << "(" << c.x << "," << c.y << ")";
        return(os);
}
