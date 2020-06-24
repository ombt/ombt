// number class functions

// headers
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <iostream.h>

// local headers
#include "number.h"

// constructors and destructor
RealRep::RealRep(): 
        Number((Number *)0)
{
        r = 0.0;
        i = 0.0;
}

RealRep::RealRep(double argr):
        Number((Number *)0)
{
        r = argr;
        i = 0.0;
}

RealRep::RealRep(const RealRep &src):
        Number((Number *)0)
{
        r = src.r;
        i = 0.0;
}

RealRep::~RealRep()
{
        // do nothing
}

// copy function
Number *
RealRep::copy()
{
        // create a new real
        Number *tmp = new RealRep(r);
        assert(tmp != (Number *)0);
        return(tmp);
}

// assignment
Number &
RealRep::operator=(const Number &n)
{
        r = n.r;
        i = n.i;
        rep = (Number *)0;
        return(*this);
}

RealRep &
RealRep::operator=(const RealRep &n)
{
        r = n.r;
        i = n.i;
        rep = (Number *)0;
        return(*this);
}

// arithetic operations
Number
RealRep::operator+(const Number &n) const
{
        // add number to real
        return(n.rep->addToReal(*this));
}

Number
RealRep::operator-(const Number &n) const
{
        // subtract number to real
        return(n.rep->subtractFromReal(*this));
}

Number
RealRep::operator*(const Number &n) const
{
        // multiply real by number
        return(n.rep->multiplyReal(*this));
}

Number
RealRep::operator/(const Number &n) const
{
        // divide number by real
        return(n.rep->divideReal(*this));
}

// logical operations
int
RealRep::operator==(const Number &n) const
{
        // is number equal to real
        return(n.rep->isEqualToReal(*this));
}

int
RealRep::operator!=(const Number &n) const
{
        // is number not equal to real
        return( ! n.rep->isEqualToReal(*this));
}

int
RealRep::operator<(const Number &n) const
{
        // is number less than real
        return(n.rep->isLessThanReal(*this));
}

int
RealRep::operator>(const Number &n) const
{
        // is number greater than real
        return(n.rep->isGreaterThanReal(*this));
}

int
RealRep::operator<=(const Number &n) const
{
        // is number less than or equal to real
        return( ! n.rep->isGreaterThanReal(*this));
}

int
RealRep::operator>=(const Number &n) const
{
        // is number greater than or equal to real
        return( ! n.rep->isLessThanReal(*this));
}

// operations for representations
Number
RealRep::addToReal(const Number &n) const
{
        // add real to real number
        return(Number(n.r+r));
}

Number
RealRep::subtractFromReal(const Number &n) const
{
        // subtract real from real number
        return(Number(n.r-r));
}

Number
RealRep::multiplyReal(const Number &n) const
{
        // multiply real by real number
        return(Number(n.r*r));
}

Number
RealRep::divideReal(const Number &n) const
{
        // divide real by real number
        assert(r != 0.0);
        return(Number(n.r/r));
}

int
RealRep::isEqualToReal(const Number &n) const
{
        // are both reals equal
        return(n.r == r);
}

int
RealRep::isLessThanReal(const Number &n) const
{
        // is less than
        return(n.r < r);
}

int
RealRep::isGreaterThanReal(const Number &n) const
{
        // is greater than
        return(n.r > r);
}

Number
RealRep::raiseRealBaseToPower(const Number &b) const
{
        if (r >= 0.0)
        {
                double rpart = ::pow(b.r, r);
                return(Number(rpart));
        }
        else
        {
                // convert to imaginary
                return(::pow(Number(b.r, 0.0), Number(r)));
        }
}

Number
RealRep::addToComplex(const Number &n) const
{
        // add real to complex number
        return(Number(n.r+r, n.i));
}

Number
RealRep::subtractFromComplex(const Number &n) const
{
        // subtract real from complex number
        return(Number(n.r-r, n.i));
}

Number
RealRep::multiplyComplex(const Number &n) const
{
        // multiply complex number by real
        return(Number(n.r*r, n.i*r));
}

Number
RealRep::divideComplex(const Number &n) const
{
        // divide complex number by real
        assert(r != 0.0);
        return(Number(n.r/r, n.i/r));
}

int
RealRep::isEqualToComplex(const Number &n) const
{
        // are complex and real equal
        return(n.r == r && n.i == 0.0);
}

int
RealRep::isLessThanComplex(const Number &n) const
{
        // is less than by comparing magnitudes
        return((n.r*n.r+n.i*n.i) < (r*r));
}

int
RealRep::isGreaterThanComplex(const Number &n) const
{
        // is greater than by comparing magnitudes
        return((n.r*n.r+n.i*n.i) > (r*r));
}

Number
RealRep::raiseComplexBaseToPower(const Number &b) const
{
        Number rp(r);
        Number cb(b.r, b.i);
        return(::exp(rp*::log(cb)));
}

// mathematical functions for representations
Number
RealRep::exp() const
{
        double rpart = ::exp(r);
        return(Number(rpart));
}

Number
RealRep::log() const
{
        if (r > 0.0)
        {
                double rpart = ::log(r);
                return(Number(rpart));
        }
        else
        {
                // convert to imaginary
                return(::log(Number(r, 0.0)));
        }
}

Number
RealRep::log10() const
{
        if (r > 0.0)
        {
                double rpart = ::log10(r);
                return(Number(rpart));
        }
        else
        {
                // convert to imaginary
                return(::log10(Number(r, 0.0)));
        }
}

Number
RealRep::pow(const Number &p) const
{
        return(p.rep->raiseRealBaseToPower(*this));
}

Number
RealRep::sqrt() const
{
        if (r > 0.0)
        {
                double rpart = ::sqrt(r);
                return(Number(rpart));
        }
        else
        {
                // convert to imaginary
                return(::sqrt(Number(r, 0.0)));
        }
}

Number
RealRep::sin() const
{
        double rpart = ::sin(r);
        return(Number(rpart));
}

Number
RealRep::cos() const
{
        double rpart = ::cos(r);
        return(Number(rpart));
}

Number
RealRep::tan() const
{
        double rpart = ::tan(r);
        return(Number(rpart));
}

Number
RealRep::asin() const
{
        double rpart = ::asin(r);
        return(Number(rpart));
}

Number
RealRep::acos() const
{
        double rpart = ::acos(r);
        return(Number(rpart));
}

Number
RealRep::atan() const
{
        double rpart = ::atan(r);
        return(Number(rpart));
}

Number
RealRep::sinh() const
{
        double rpart = ::sinh(r);
        return(Number(rpart));
}


Number
RealRep::cosh() const
{
        double rpart = ::cosh(r);
        return(Number(rpart));
}

Number
RealRep::tanh() const
{
        double rpart = ::tanh(r);
        return(Number(rpart));
}

Number
RealRep::asinh() const
{
#ifdef NATIVEHYPERBOLIC
        double rpart = ::asinh(r);
#else
        double rpart = ::log(r+::sqrt(r*r+1.0));
#endif
        return(Number(rpart));
}

Number
RealRep::acosh() const
{
#ifdef NATIVEHYPERBOLIC
        double rpart = ::acosh(r);
#else
        assert(r >= 1.0);
        double rpart = ::log(r+::sqrt(r*r-1.0));
#endif
        return(Number(rpart));
}

Number
RealRep::atanh() const
{
#ifdef NATIVEHYPERBOLIC
        double rpart = ::atanh(r);
#else
        assert(-1.0 < r && r < 1.0);
        double rpart = ::log((1.0+r)/(1.0-r))/2.0;
#endif
        return(Number(rpart));
}

Number
RealRep::conj() const
{
        return(Number(r));
}

Number
RealRep::abs() const
{
        double rpart = (( r < 0.0) ? -r : r);
        return(Number(rpart));
}

Number
RealRep::arg() const
{
        double rpart = ((r < 0.0) ? 0.0 : M_PI);
        return(Number(rpart));
}

Number
RealRep::norm() const
{
        double rpart = ((r < 0.0) ? 0.0 : M_PI);
        return(Number(rpart));
}

Number
RealRep::real() const
{
        return(Number(r));
}

Number
RealRep::imag() const
{
        return(Number(0.0));
}

// print data
void
RealRep::dump(ostream &os) const
{
        os << "<" << r << ">";
        return;
}
