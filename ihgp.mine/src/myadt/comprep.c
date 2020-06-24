// complex number class functions

// headers
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <iostream.h>

// local headers
#include "number.h"

// constructors and destructor
ComplexRep::ComplexRep(): 
        Number((Number *)0)
{
        r = 0.0;
        i = 0.0;
}

ComplexRep::ComplexRep(double argr, double argi): 
        Number((Number *)0)
{
        r = argr;
        i = argi;
}

ComplexRep::ComplexRep(const ComplexRep &src): 
        Number((Number *)0)
{
        r = src.r;
        i = src.i;
}

ComplexRep::~ComplexRep()
{
        // do nothing
}

// copy function
Number *
ComplexRep::copy()
{
        // create a new real
        Number *tmp = new ComplexRep(r, i);
        assert(tmp != (Number *)0);
        return(tmp);
}

// assignment
Number &
ComplexRep::operator=(const Number &n)
{
        r = n.r;
        i = n.i;
        rep = (Number *)0;
        return(*this);
}

ComplexRep &
ComplexRep::operator=(const ComplexRep &n)
{
        r = n.r;
        i = n.i;
        rep = (Number *)0;
        return(*this);
}

// arithetic operations
Number
ComplexRep::operator+(const Number &n) const
{
        // add number to complex
        return(n.rep->addToComplex(*this));
}

Number
ComplexRep::operator-(const Number &n) const
{
        // subtract number to complex
        return(n.rep->subtractFromComplex(*this));
}

Number
ComplexRep::operator*(const Number &n) const
{
        // multiply complex by number
        return(n.rep->multiplyComplex(*this));
}

Number
ComplexRep::operator/(const Number &n) const
{
        // divide number by complex
        return(n.rep->divideComplex(*this));
}

// logical operations
int
ComplexRep::operator==(const Number &n) const
{
        // is number equal to real
        return(n.rep->isEqualToReal(*this));
}

int
ComplexRep::operator!=(const Number &n) const
{
        // is number not equal to complex
        return( ! n.rep->isEqualToComplex(*this));
}

int
ComplexRep::operator<(const Number &n) const
{
        // is number less than complex
        return(n.rep->isLessThanComplex(*this));
}

int
ComplexRep::operator>(const Number &n) const
{
        // is number greater than complex
        return(n.rep->isGreaterThanComplex(*this));
}

int
ComplexRep::operator<=(const Number &n) const
{
        // is number less than or equal to complex
        return( ! n.rep->isGreaterThanComplex(*this));
}

int
ComplexRep::operator>=(const Number &n) const
{
        // is number greater than or equal to complex
        return( ! n.rep->isLessThanComplex(*this));
}

// operations for representations
Number
ComplexRep::addToReal(const Number &n) const
{
        // add complex to real number
        return(Number(n.r+r, i));
}

Number
ComplexRep::subtractFromReal(const Number &n) const
{
        // subtract complex from real number
        return(Number(n.r-r, -i));
}

Number
ComplexRep::multiplyReal(const Number &n) const
{
        // multiply complex by real number
        return(Number(n.r*r, n.r*i));
}

Number
ComplexRep::divideReal(const Number &n) const
{
        // divide real by complex
        assert(r != 0.0 || i != 0.0);
        return(Number((n.r*r)/(r*r+i*i), (-n.r*i)/(r*r+i*i)));
}

int
ComplexRep::isEqualToReal(const Number &n) const
{
        // are both equal
        return(n.r == r && i == 0.0);
}

int
ComplexRep::isLessThanReal(const Number &n) const
{
        // is less than
        return((n.r*n.r) < (r*r+i*i));
}

int
ComplexRep::isGreaterThanReal(const Number &n) const
{
        // is greater than
        return((n.r*n.r) > (r*r+i*i));
}

Number
ComplexRep::raiseRealBaseToPower(const Number &b) const
{
        Number rb(b.r);
        Number cp(r, i);
        return(::exp(cp*::log(rb)));
}

Number
ComplexRep::addToComplex(const Number &n) const
{
        // add complex to complex number
        return(Number(n.r+r, n.i+i));
}

Number
ComplexRep::subtractFromComplex(const Number &n) const
{
        // subtract complex from complex number
        return(Number(n.r-r, n.i-i));
}

Number
ComplexRep::multiplyComplex(const Number &n) const
{
        // multiply complex number by complex
        return(Number((n.r*r-n.i*i), (n.r*i+n.i*r)));
}

Number
ComplexRep::divideComplex(const Number &n) const
{
        // divide complex number by complex
        assert(r != 0.0 || i != 0.0);
        return(Number((n.r*r+n.i*i)/(r*r+i*i), (n.i*r-n.r*i)/(r*r+i*i)));
}

int
ComplexRep::isEqualToComplex(const Number &n) const
{
        // are both complex equal
        return(n.r == r && n.i == i);
}

int
ComplexRep::isLessThanComplex(const Number &n) const
{
        // is less than by comparing magnitudes
        return((n.r*n.r+n.i*n.i) < (r*r+i*i));
}

int
ComplexRep::isGreaterThanComplex(const Number &n) const
{
        // is greater than by comparing magnitudes
        return((n.r*n.r+n.i*n.i) > (r*r+i*i));
}

Number
ComplexRep::raiseComplexBaseToPower(const Number &b) const
{
        Number cp(r, i);
        Number cb(b.r, b.i);
        return(::exp(cp*::log(cb)));
}

// mathematical functions for representations
Number
ComplexRep::exp() const
{
        double rpart = ::exp(r)*::cos(i);
        double ipart = ::exp(r)*::sin(i);
        return(Number(rpart, ipart));
}

Number
ComplexRep::log() const
{
        assert(r != 0.0 || i != 0.0);
        double rpart = ::log(::sqrt(r*r+i*i));
        double ipart = ::arg(r, i);
        return(Number(rpart, ipart));
}

Number
ComplexRep::log10() const
{
        assert(r != 0.0 || i != 0.0);
        double rpart = ::log(::sqrt(r*r+i*i))/::log(10.0);
        double ipart = ::arg(r, i)/::log(10.0);
        return(Number(rpart, ipart));
}

Number
ComplexRep::pow(const Number &p) const
{
        return(p.rep->raiseComplexBaseToPower(*this));
}

Number
ComplexRep::sqrt() const
{
        // check for special case of zero
        if (r == 0.0 && i == 0.0)
                return(Number(0.0));

        //get square root
        double tmpr = ::log(::sqrt(r*r+i*i))*0.5;
        double tmpi = ::arg(r, i)*0.5;
        double rpart = ::exp(tmpr)*::cos(tmpi);
        double ipart = ::exp(tmpr)*::sin(tmpi);
        return(Number(rpart, ipart));
}

Number
ComplexRep::sin() const
{
        double rpart = ::sin(r)*::cosh(i);
        double ipart = ::cos(r)*::sinh(i);
        return(Number(rpart, ipart));
}

Number
ComplexRep::cos() const
{
        double rpart = ::cos(r)*::cosh(i);
        double ipart = ::sin(r)*::sinh(i);
        return(Number(rpart, ipart));
}

Number
ComplexRep::tan() const
{
        double divider = ::cos(r)*::cos(r)*::cosh(i)*::cosh(i) +
                         ::sin(r)*::sin(r)*::sinh(i)*::sinh(i);
        assert(divider != 0.0);
        double rpart = ::sin(r)*::cos(r)/divider;
        double ipart = ::sinh(i)*::cosh(i)/divider;
        return(Number(rpart, ipart));
}

Number
ComplexRep::asin() const
{
        Number z(r, i);
        return(minusi*::log(plusi*z+::sqrt(1.0-z*z)));
}

Number
ComplexRep::acos() const
{
        Number z(r, i);
        return(minusi*::log(z+plusi*::sqrt(1.0-z*z)));
}

Number
ComplexRep::atan() const
{
        Number z(r, i);
        return(plusi*::log((plusi+z)/(plusi-z))/2.0);
}

Number
ComplexRep::sinh() const
{
        double rpart = ::sinh(r)*::cos(i);
        double ipart = ::cosh(r)*::sin(i);
        return(Number(rpart, ipart));
}


Number
ComplexRep::cosh() const
{
        double rpart = ::cosh(r)*::cos(i);
        double ipart = ::sinh(r)*::sin(i);
        return(Number(rpart, ipart));
}

Number
ComplexRep::tanh() const
{
        double divider = ::cosh(2.0*r) + ::sin(2.0*i);
        assert(divider != 0.0);
        double rpart = ::sinh(2.0*r)/divider;
        double ipart = ::sin(2.0*i)/divider;
        return(Number(rpart, ipart));
}

Number
ComplexRep::asinh() const
{
        Number z(r, i);
        return(::log(z+::sqrt(z*z+1.0)));
}

Number
ComplexRep::acosh() const
{
        Number z(r, i);
        return(::log(z+::sqrt(z*z-1.0)));
}

Number
ComplexRep::atanh() const
{
        Number z(r, i);
        return(::log((1.0+z)/(1.0-z))/2.0);
}

Number
ComplexRep::conj() const
{
        return(Number(r, -i));
}

Number
ComplexRep::abs() const
{
        double rpart = ::sqrt(r*r+i*i);
        return(Number(rpart));
}

Number
ComplexRep::arg() const
{
        double rpart = ::arg(r, i);
        return(Number(rpart));
}

Number
ComplexRep::norm() const
{
        double rpart = ::sqrt(r*r+i*i);
        return(Number(rpart));
}

Number
ComplexRep::real() const
{
        return(Number(r));
}

Number
ComplexRep::imag() const
{
        return(Number(i));
}

// print data
void
ComplexRep::dump(ostream &os) const
{
        os << "<" << r << "," << i << ">";
        return;
}
