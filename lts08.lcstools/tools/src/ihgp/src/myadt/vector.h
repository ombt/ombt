#ifndef __VECTOR_H
#define __VECTOR_H

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
#include "number.h"

// forward declarations
class Matrix;
class Vector;

// vector class definition
class Vector {
        // friend functions and classes
        friend class Matrix;
        friend Vector operator+(const Vector &, const Matrix &);

public:
        // constructors and destructor
        Vector(int = 3);
        Vector(const Number *, int);
        Vector(const Vector &);
        virtual ~Vector();

        // assignment
        virtual Vector &operator=(const Vector &);
        virtual Number &operator[](int);
        virtual Number &operator[](int) const;

        // vector operations
        virtual Vector &operator+=(const Vector &);
        virtual Vector &operator-=(const Vector &);
        virtual Vector operator+(const Vector &) const;
        virtual Vector operator-(const Vector &) const;

        // arithmetic operations
        virtual Vector &operator*=(const Number &);
        virtual Vector &operator/=(const Number &);
        virtual Vector operator*(const Number &) const;
        virtual Vector operator/(const Number &) const;
        friend Vector operator*(const Number &, const Vector &);
        virtual Vector &operator*=(double);
        virtual Vector &operator/=(double);
        virtual Vector operator*(double) const;
        virtual Vector operator/(double) const;
        friend Vector operator*(double, const Vector &);

        // logical operators
        virtual int operator==(const Vector &) const;
        virtual int operator!=(const Vector &) const;

        // vector products
        friend Vector conj(const Vector &);
        friend Number dot(const Vector &, const Vector &);
        friend Number norm(const Vector &);

        // miscellaneous 
        inline int getDimension() const { 
                return(dimension);
        }
        virtual void dump(ostream & = cout) const;
        friend ostream &operator<<(ostream &, const Vector &);


protected:
        // data
        Number *vector;
        int dimension;
};


#endif
