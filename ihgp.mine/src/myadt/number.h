#ifndef __NUMBER_H
#define __NUMBER_H

// number class

// headers
#include <stdlib.h>
#include <string.h>
#include <iostream.h>
#include <math.h>
#include <assert.h>

// local headers
#include "returns.h"
#include "debug.h"

// forward declarations
class Number;
class RealRep;
class ComplexRep;

// constants
extern const Number plusi;
extern const Number minusi;

// number class definition
class Number {
        // friend classes
        // friend class RealRep;
        // friend class ComplexRep;

public:
        // constructors and destructor
        Number();
        Number(double);
        Number(double, double);
        Number(const Number &);
        virtual ~Number();

        // copy function
        virtual Number *copy();

        // initialization for different representations
        virtual Number &makeReal(double);
        virtual Number &makeComplex(double, double);

        // arithmetic operations
        virtual Number &operator=(const Number &);
        virtual Number &operator+=(const Number &);
        virtual Number &operator-=(const Number &);
        virtual Number &operator*=(const Number &);
        virtual Number &operator/=(const Number &);
        virtual Number operator+(const Number &) const;
        virtual Number operator-(const Number &) const;
        virtual Number operator*(const Number &) const;
        virtual Number operator/(const Number &) const;

        // logical operators
        virtual int operator==(const Number &) const;
        virtual int operator!=(const Number &) const;
        virtual int operator<(const Number &) const;
        virtual int operator>(const Number &) const;
        virtual int operator<=(const Number &) const;
        virtual int operator>=(const Number &) const;

        // arithmetic operations with a double
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

        // logical operators with a double
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
        friend Number conj(const Number &);
        friend Number abs(const Number &);
        friend Number arg(const Number &);
        friend Number norm(const Number &);
        friend Number real(const Number &);
        friend Number imag(const Number &);
        friend double arg(double, double);

        // miscellaneous 
        virtual void dump(ostream & = cout) const;
        friend ostream &operator<<(ostream &, const Number &);

protected:
        // constructor for derived classes
        Number(Number *);

        // operations for representations
        virtual Number addToReal(const Number &) const;
        virtual Number subtractFromReal(const Number &) const;
        virtual Number multiplyReal(const Number &) const;
        virtual Number divideReal(const Number &) const;
        virtual int isEqualToReal(const Number &) const;
        virtual int isLessThanReal(const Number &) const;
        virtual int isGreaterThanReal(const Number &) const;
        virtual Number raiseRealBaseToPower(const Number &) const;
        virtual Number addToComplex(const Number &) const;
        virtual Number subtractFromComplex(const Number &) const;
        virtual Number multiplyComplex(const Number &) const;
        virtual Number divideComplex(const Number &) const;
        virtual int isEqualToComplex(const Number &) const;
        virtual int isLessThanComplex(const Number &) const;
        virtual int isGreaterThanComplex(const Number &) const;
        virtual Number raiseComplexBaseToPower(const Number &) const;
        
        // mathematical functions for representations
        virtual Number exp() const;
        virtual Number log() const;
        virtual Number log10() const;
        virtual Number pow(const Number &) const;
        virtual Number sqrt() const;
        virtual Number sin() const;
        virtual Number cos() const;
        virtual Number tan() const;
        virtual Number asin() const;
        virtual Number acos() const;
        virtual Number atan() const;
        virtual Number sinh() const;
        virtual Number cosh() const;
        virtual Number tanh() const;
        virtual Number asinh() const;
        virtual Number acosh() const;
        virtual Number atanh() const;
        virtual Number conj() const;
        virtual Number abs() const;
        virtual Number arg() const;
        virtual Number norm() const;
        virtual Number real() const;
        virtual Number imag() const;

protected:
        // data
        double r, i;
        Number *rep;

};

// real number representation class definition
class RealRep: public Number {
public:
        // constructors and destructor
        RealRep();
        RealRep(double);
        RealRep(const RealRep &);
        ~RealRep();

        // copy function
        Number *copy();

        // assignment
        RealRep &operator=(const RealRep &);
        Number &operator=(const Number &);

        // arithmetic operations
        Number operator+(const Number &) const;
        Number operator-(const Number &) const;
        Number operator*(const Number &) const;
        Number operator/(const Number &) const;

        // logical operators
        int operator==(const Number &) const;
        int operator!=(const Number &) const;
        int operator<(const Number &) const;
        int operator>(const Number &) const;
        int operator<=(const Number &) const;
        int operator>=(const Number &) const;

        // operations for representations
        Number addToReal(const Number &) const;
        Number subtractFromReal(const Number &) const;
        Number multiplyReal(const Number &) const;
        Number divideReal(const Number &) const;
        int isEqualToReal(const Number &) const;
        int isLessThanReal(const Number &) const;
        int isGreaterThanReal(const Number &) const;
        Number raiseRealBaseToPower(const Number &) const;
        Number addToComplex(const Number &) const;
        Number subtractFromComplex(const Number &) const;
        Number multiplyComplex(const Number &) const;
        Number divideComplex(const Number &) const;
        int isEqualToComplex(const Number &) const;
        int isLessThanComplex(const Number &) const;
        int isGreaterThanComplex(const Number &) const;
        Number raiseComplexBaseToPower(const Number &) const;

        // mathematical functions for representations
        Number exp() const;
        Number log() const;
        Number log10() const;
        Number pow(const Number &) const;
        Number sqrt() const;
        Number sin() const;
        Number cos() const;
        Number tan() const;
        Number asin() const;
        Number acos() const;
        Number atan() const;
        Number sinh() const;
        Number cosh() const;
        Number tanh() const;
        Number asinh() const;
        Number acosh() const;
        Number atanh() const;
        Number conj() const;
        Number abs() const;
        Number arg() const;
        Number norm() const;
        Number real() const;
        Number imag() const;

        // miscellaneous
        void dump(ostream & = cout) const;
};

// complex number representation class definition
class ComplexRep: public Number {
public:
        // constructors and destructor
        ComplexRep();
        ComplexRep(double, double);
        ComplexRep(const ComplexRep &);
        ~ComplexRep();

        // copy function
        Number *copy();

        // assignment
        ComplexRep &operator=(const ComplexRep &);
        Number &operator=(const Number &);

        // arithmetic operations
        Number operator+(const Number &) const;
        Number operator-(const Number &) const;
        Number operator*(const Number &) const;
        Number operator/(const Number &) const;

        // logical operators
        int operator==(const Number &) const;
        int operator!=(const Number &) const;
        int operator<(const Number &) const;
        int operator>(const Number &) const;
        int operator<=(const Number &) const;
        int operator>=(const Number &) const;

        // operations for representations
        Number addToReal(const Number &) const;
        Number subtractFromReal(const Number &) const;
        Number multiplyReal(const Number &) const;
        Number divideReal(const Number &) const;
        int isEqualToReal(const Number &) const;
        int isLessThanReal(const Number &) const;
        int isGreaterThanReal(const Number &) const;
        Number raiseRealBaseToPower(const Number &) const;
        Number addToComplex(const Number &) const;
        Number subtractFromComplex(const Number &) const;
        Number multiplyComplex(const Number &) const;
        Number divideComplex(const Number &) const;
        int isEqualToComplex(const Number &) const;
        int isLessThanComplex(const Number &) const;
        int isGreaterThanComplex(const Number &) const;
        Number raiseComplexBaseToPower(const Number &) const;

        // mathematical functions for representations
        Number exp() const;
        Number log() const;
        Number log10() const;
        Number pow(const Number &) const;
        Number sqrt() const;
        Number sin() const;
        Number cos() const;
        Number tan() const;
        Number asin() const;
        Number acos() const;
        Number atan() const;
        Number sinh() const;
        Number cosh() const;
        Number tanh() const;
        Number asinh() const;
        Number acosh() const;
        Number atanh() const;
        Number conj() const;
        Number abs() const;
        Number arg() const;
        Number norm() const;
        Number real() const;
        Number imag() const;

        // miscellaneous
        void dump(ostream & = cout) const;
};

#endif
