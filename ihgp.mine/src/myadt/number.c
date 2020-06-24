// number class functions

// headers
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <iostream.h>

// local headers
#include "number.h"

// useful constants
const Number plusi(0.0, 1.0);
const Number minusi(0.0, -1.0);

// constructors and destructor
Number::Number()
{
        // initialize representation
        rep = new RealRep(0.0);
        assert(rep != (Number *)0);
}

Number::Number(Number *prep)
{
        // initialize representation for derived classes
        rep = prep;
}

Number::Number(double argr)
{
        // initialize representation
        rep = new RealRep(argr);
        assert(rep != (Number *)0);
}

Number::Number(double argr, double argi)
{
        // initialize representation
        rep = new ComplexRep(argr, argi);
        assert(rep != (Number *)0);
}

Number::Number(const Number &src)
{
        // copy representation
        rep = src.rep->copy();
        assert(rep != (Number *)0);
}

Number::~Number()
{
        // delete representation
        if (rep != (Number *)0) delete rep;
        rep = (Number *)0;
}

// copy function
Number *
Number::copy()
{
        // create a new number
        Number *tmp = new Number(*this);
        assert(tmp != (Number *)0);
        return(tmp);
}

// initialization for different types of numbers
Number &
Number::makeReal(double argr)
{
        // delete representation
        delete rep;

        // allocate a new real representation
        rep = new RealRep(argr);
        assert(rep != (RealRep *)0);
        return(*this);
}

Number &
Number::makeComplex(double argr, double argi)
{
        // delete representation
        delete rep;

        // allocate a new complex representation
        rep = new ComplexRep(argr, argi);
        assert(rep != (ComplexRep *)0);
        return(*this);
}

// arithetic operations
Number &
Number::operator=(const Number &n)
{
        // check if assigning to itself
        if (this == &n) return(*this);

        // delete representation 
        delete rep;

        // copy representation
        rep = n.rep->copy();
        assert(rep != (Number *)0);
        return(*this);
}

Number &
Number::operator+=(const Number &n)
{
        // add number
        *this = *this + n;
        return(*this);
}

Number &
Number::operator-=(const Number &n)
{
        // subtract number
        *this = *this - n;
        return(*this);
}

Number &
Number::operator*=(const Number &n)
{
        // multiply by number
        *this = *this*n;
        return(*this);
}

Number &
Number::operator/=(const Number &n)
{
        // divide by number
        *this = *this/n;
        return(*this);
}

Number
Number::operator+(const Number &n) const
{
        // forward operation to representation
        return(rep->operator+(n));
}

Number
Number::operator-(const Number &n) const
{
        // forward operation to representation
        return(rep->operator-(n));
}

Number
Number::operator*(const Number &n) const
{
        // forward operation to representation
        return(rep->operator*(n));
}

Number
Number::operator/(const Number &n) const
{
        // forward operation to representation
        return(rep->operator/(n));
}

// logical operations
int
Number::operator==(const Number &n) const
{
        // forward operation to representation
        return(rep->operator==(n));
}

int
Number::operator!=(const Number &n) const
{
        // forward operation to representation
        return(rep->operator!=(n));
}

int
Number::operator<(const Number &n) const
{
        // forward operation to representation
        return(rep->operator<(n));
}

int
Number::operator>(const Number &n) const
{
        // forward operation to representation
        return(rep->operator>(n));
}

int
Number::operator<=(const Number &n) const
{
        // forward operation to representation
        return(rep->operator<=(n));
}

int
Number::operator>=(const Number &n) const
{
        // forward operation to representation
        return(rep->operator>=(n));
}

// arithmetic operations with a double
Number &
Number::operator=(double d)
{
        return(*this = Number(d));
}

Number &
Number::operator+=(double d)
{
        return(*this = Number(d));
}

Number &
Number::operator-=(double d)
{
        return(*this -= Number(d));
}

Number &
Number::operator*=(double d)
{
        return(*this *= Number(d));
}

Number &
Number::operator/=(double d)
{
        return(*this /= Number(d));
}

Number
Number::operator+(double d) const
{
        return(Number(*this) += Number(d));
}

Number
Number::operator-(double d) const
{
        return(Number(*this) -= Number(d));
}

Number
Number::operator*(double d) const
{
        return(Number(*this) *= Number(d));
}

Number
Number::operator/(double d) const
{
        return(Number(*this) /= Number(d));
}

Number
operator+(double d, const Number &n)
{
        return(Number(d) += n);
}

Number
operator-(double d, const Number &n)
{
        return(Number(d) -= n);
}

Number
operator*(double d, const Number &n)
{
        return(Number(d) *= n);
}

Number
operator/(double d, const Number &n)
{
        return(Number(d) /= n);
}

// logical operators with a double
int
Number::operator==(double d) const
{
        return(*this == Number(d));
}

int
Number::operator!=(double d) const
{
        return(*this != Number(d));
}

int
Number::operator<(double d) const
{
        return(*this < Number(d));
}

int
Number::operator>(double d) const
{
        return(*this > Number(d));
}

int
Number::operator<=(double d) const
{
        return(*this <= Number(d));
}

int
Number::operator>=(double d) const
{
        return(*this >= Number(d));
}

int
operator==(double d, const Number &n)
{
        return(Number(d) == n);
}

int
operator!=(double d, const Number &n)
{
        return(Number(d) != n);
}

int
operator<(double d, const Number &n)
{
        return(Number(d) < n);
}

int
operator>(double d, const Number &n)
{
        return(Number(d) > n);
}

int
operator<=(double d, const Number &n)
{
        return(Number(d) <= n);
}

int
operator>=(double d, const Number &n)
{
        return(Number(d) >= n);
}

// mathematical functions
Number
exp(const Number &n)
{
        return(n.rep->exp());
}

Number
log(const Number &n)
{
        return(n.rep->log());
}

Number
log10(const Number &n)
{
        return(n.rep->log10());
}

Number
pow(const Number &b, const Number &p)
{
        return(b.rep->pow(p));
}

Number
sqrt(const Number &n)
{
        return(n.rep->sqrt());
}

Number
sin(const Number &n)
{
        return(n.rep->sin());
}

Number
cos(const Number &n)
{
        return(n.rep->cos());
}

Number
tan(const Number &n)
{
        return(n.rep->tan());
}

Number
asin(const Number &n)
{
        return(n.rep->asin());
}

Number
acos(const Number &n)
{
        return(n.rep->acos());
}

Number
atan(const Number &n)
{
        return(n.rep->atan());
}

Number
sinh(const Number &n)
{
        return(n.rep->sinh());
}

Number
cosh(const Number &n)
{
        return(n.rep->cosh());
}

Number
tanh(const Number &n)
{
        return(n.rep->tanh());
}

Number
asinh(const Number &n)
{
        return(n.rep->asinh());
}

Number
acosh(const Number &n)
{
        return(n.rep->acosh());
}

Number
atanh(const Number &n)
{
        return(n.rep->atanh());
}

Number
conj(const Number &n)
{
        return(n.rep->conj());
}

Number
abs(const Number &n)
{
        return(n.rep->abs());
}

Number
arg(const Number &n)
{
        return(n.rep->arg());
}

Number
norm(const Number &n)
{
        return(n.rep->norm());
}

Number
real(const Number &n)
{
        return(n.rep->real());
}

Number
imag(const Number &n)
{
        return(n.rep->imag());
}

double
arg(double r, double i)
{
        double theta = 0.0;

        // get polar angle for a complex number
        if (r > 0.0)
        {
                if (i > 0.0)
                        theta = ::atan(i/r);
                else if (i < 0.0)
                        theta = 2.0*M_PI-::atan(::abs(i/r));
                else
                        theta = 0.0;
        }
        else if (r < 0.0)
        {
                if (i > 0.0)
                        theta = M_PI-::atan(abs(i/r));
                else if (i < 0.0)
                        theta = M_PI+::atan(i/r);
                else
                        theta = M_PI;
        }
        else
        {
                if (i > 0.0)
                        theta = M_PI/2.0;
                else if (i < 0.0)
                        theta = -3.0*M_PI/2.0;
                else
                        theta = 0.0;
        }
        return(theta);
}

// miscellaneous
void
Number::dump(ostream &os) const
{
        rep->dump(os);
        return;
}

ostream &
operator<<(ostream &os, const Number &n)
{
        n.dump(os);
        return(os);
}

// operations for representations
Number
Number::addToReal(const Number &n) const
{
        return(rep->addToReal(n));
}

Number
Number::subtractFromReal(const Number &n) const
{
        return(rep->subtractFromReal(n));
}

Number
Number::multiplyReal(const Number &n) const
{
        return(rep->multiplyReal(n));
}

Number
Number::divideReal(const Number &n) const
{
        return(rep->divideReal(n));
}

int
Number::isEqualToReal(const Number &n) const
{
        return(rep->isEqualToReal(n));
}

int
Number::isLessThanReal(const Number &n) const
{
        return(rep->isLessThanReal(n));
}

int
Number::isGreaterThanReal(const Number &n) const
{
        return(rep->isGreaterThanReal(n));
}

Number
Number::raiseRealBaseToPower(const Number &b) const
{
        return(rep->raiseRealBaseToPower(b));
}

Number
Number::addToComplex(const Number &n) const
{
        return(rep->addToComplex(n));
}

Number
Number::subtractFromComplex(const Number &n) const
{
        return(rep->subtractFromComplex(n));
}

Number
Number::multiplyComplex(const Number &n) const
{
        return(rep->multiplyComplex(n));
}

Number
Number::divideComplex(const Number &n) const
{
        return(rep->divideComplex(n));
}

int
Number::isEqualToComplex(const Number &n) const
{
        return(rep->isEqualToComplex(n));
}

int
Number::isLessThanComplex(const Number &n) const
{
        return(rep->isLessThanComplex(n));
}

int
Number::isGreaterThanComplex(const Number &n) const
{
        return(rep->isGreaterThanComplex(n));
}

Number
Number::raiseComplexBaseToPower(const Number &b) const
{
        return(rep->raiseComplexBaseToPower(b));
}

// mathematical functions for representations
Number
Number::exp() const
{
        return(rep->exp());
}

Number
Number::log() const
{
        return(rep->log());
}

Number
Number::log10() const
{
        return(rep->log10());
}

Number
Number::pow(const Number &p) const
{
        return(rep->pow(p));
}

Number
Number::sqrt() const
{
        return(rep->sqrt());
}

Number
Number::sin() const
{
        return(rep->sin());
}

Number
Number::cos() const
{
        return(rep->cos());
}

Number
Number::tan() const
{
        return(rep->tan());
}

Number
Number::asin() const
{
        return(rep->asin());
}

Number
Number::acos() const
{
        return(rep->acos());
}

Number
Number::atan() const
{
        return(rep->atan());
}

Number
Number::sinh() const
{
        return(rep->sinh());
}

Number
Number::cosh() const
{
        return(rep->cosh());
}

Number
Number::tanh() const
{
        return(rep->tanh());
}

Number
Number::asinh() const
{
        return(rep->asinh());
}

Number
Number::acosh() const
{
        return(rep->acosh());
}

Number
Number::atanh() const
{
        return(rep->atanh());
}

Number
Number::conj() const
{
        return(rep->conj());
}

Number
Number::abs() const
{
        return(rep->abs());
}

Number
Number::arg() const
{
        return(rep->arg());
}

Number
Number::norm() const
{
        return(rep->norm());
}

Number
Number::real() const
{
        return(rep->real());
}

Number
Number::imag() const
{
        return(rep->imag());
}

