// vector class functions

// headers
#include <stdlib.h>
#include <assert.h>
#include <math.h>

// local headers
#include "vector.h"

// constructors and destructor
Vector::Vector(int argd)
{
        // store dimension
        dimension = argd;
        assert(dimension > 0);

        // allocate a vector
        vector = new Number[dimension];
        assert(vector != (Number *)0);
}


Vector::Vector(const Number *argn, int argd)
{
        // store dimension
        dimension = argd;
        assert(dimension > 0);

        // allocate a vector
        vector = new Number[dimension];
        assert(vector != (Number *)0);

        // copy coordinates
        assert(argn != (Number *)0);
        for (int id = 0; id < dimension; id++)
        {
                vector[id] = argn[id];
        }
}

Vector::Vector(const Vector &argv)
{
        // store dimension
        dimension = argv.dimension;
        assert(dimension > 0);

        // allocate a vector
        vector = new Number[dimension];
        assert(vector != (Number *)0);

        // copy coordinates
        for (int id = 0; id < dimension; id++)
        {
                vector[id] = argv[id];
        }
}


Vector::~Vector()
{
        // delete vector
        if (vector != (Number *)0) delete [] vector;
        vector = (Number *)0;
        dimension = 0;
}

// assignment 
Vector &
Vector::operator=(const Vector &v)
{
        // check if assigning to itself
        if (this == &v) return(*this);

        // clear vector
        delete [] vector;

        // store vctor dimension
        dimension = v.dimension;
        assert(dimension > 0);

        // allocate a vector
        vector = new Number[dimension];
        assert(vector != (Number*)0);

        // copy vector to new vector
        for (int ic = 0; ic < dimension; ic++)
        {
                vector[ic] = v[ic];
        }

        // all done
        return(*this);
}

Number &
Vector::operator[](int ic)
{
        // check index
        assert(0 <= ic && ic < dimension);

        // return coordinate
        return(vector[ic]);
}

Number &
Vector::operator[](int ic) const
{
        // check index
        assert(0 <= ic && ic < dimension);

        // return coordinate
        return(vector[ic]);
}

// vector operations
Vector &
Vector::operator+=(const Vector &v)
{
        // check dimension
        assert(dimension > 0);

        // allocate a new vector
        int newdim = ((dimension > v.dimension) ? dimension : v.dimension);
        Number *tmp = new Number[newdim];
        assert(tmp != (Number *)0);

        // add vector
        if (v.dimension < dimension)
        {
                for (int id = 0; id < v.dimension; id++)
                {
                        tmp[id] = v[id] + vector[id];
                }
                for ( ; id < dimension; id++)
                {
                        tmp[id] = vector[id];
                }
        }
        else
        {
                for (int id = 0; id < dimension; id++)
                {
                        tmp[id] = v[id] + vector[id];
                }
                for ( ; id < v.dimension; id++)
                {
                        tmp[id] = v[id];
                }
        }

        // save new vector
        delete [] vector;
        vector = tmp;
        dimension = newdim;

        // all done
        return(*this);
}

Vector &
Vector::operator-=(const Vector &v)
{
        // check dimension
        assert(dimension > 0);

        // allocate a new vector
        int newdim = ((dimension > v.dimension) ? dimension : v.dimension);
        Number *tmp = new Number[newdim];
        assert(tmp != (Number *)0);

        // subtract vector
        if (v.dimension < dimension)
        {
                for (int id = 0; id < v.dimension; id++)
                {
                        tmp[id] = vector[id] - v[id];
                }
        }
        else
        {
                for (int id = 0; id < dimension; id++)
                {
                        tmp[id] = vector[id] - v[id];
                }
                for ( ; id < v.dimension; id++)
                {
                        tmp[id] = -1.0*v[id];
                }
        }

        // save new vector
        delete [] vector;
        vector = tmp;
        dimension = newdim;

        // all done
        return(*this);
}

Vector
Vector::operator+(const Vector &v) const
{
        // add vectors
        return(Vector(*this) += v);
}

Vector
Vector::operator-(const Vector &v) const
{
        // subtract vectors
        return(Vector(*this) -= v);
}

// vector and scalar operations
Vector &
Vector::operator*=(const Number &n)
{
        // check dimension
        assert(dimension > 0);

        // multiply vector by a scalar
        for (int ic = 0; ic < dimension; ic++)
        {
                vector[ic] *= n;
        }

        // all done
        return(*this);
}

Vector &
Vector::operator/=(const Number &n)
{
        // check dimension
        assert(dimension > 0);

        // divide vector by a scalar
        for (int ic = 0; ic < dimension; ic++)
        {
                vector[ic] /= n;
        }

        // all done
        return(*this);
}

Vector
Vector::operator*(const Number &n) const
{
        // multiply vector by a scalar
        return(Vector(*this) *= n);
}

Vector
Vector::operator/(const Number &n) const
{
        // divide vector by a scalar
        return(Vector(*this) /= n);
}

Vector
operator*(const Number &n, const Vector &v)
{
        // multiply vector by a scalar
        return(Vector(v) *= n);
}

Vector &
Vector::operator*=(double d)
{
        // multiply vector by a scalar
        return(*this *= Number(d));
}

Vector &
Vector::operator/=(double d)
{
        // divide vector by a scalar
        return(*this /= Number(d));
}

Vector
Vector::operator*(double d) const
{
        // multiply vector by a scalar
        return(Vector(*this) *= Number(d));
}

Vector
Vector::operator/(double d) const
{
        // divide vector by a scalar
        return(Vector(*this) /= Number(d));
}

Vector
operator*(double d, const Vector &v)
{
        // multiply vector by a scalar
        return(Vector(v) *= Number(d));
}

// logical operators
int
Vector::operator==(const Vector &v) const
{
        // check dimension
        assert(dimension > 0);

        // check if vectors are equal
        if (v.dimension < dimension)
        {
                for (int id = 0; id < v.dimension; id++)
                {
                        if (v[id] != vector[id]) return(0);
                }
                for ( ; id < dimension; id++)
                {
                        if (vector[id] != 0.0) return(0);
                }
        }
        else
        {
                for (int id = 0; id < dimension; id++)
                {
                        if (v[id] != vector[id]) return(0);
                }
                for ( ; id < v.dimension; id++)
                {
                        if (v[id] != 0.0) return(0);
                }
        }

        // vectors are equal
        return(1);
}


int
Vector::operator!=(const Vector &v) const
{
        return( ! (*this== v));
}

// vector products
Vector
conj(const Vector &v)
{
        // check dimension
        assert(v.dimension > 0);

        // copy vector and conjugate
        Vector tmp(v.dimension);
        for (int id = 0; id < v.dimension; id++)
        {
                tmp[id] = conj(v[id]);
        }

        // all done
        return(tmp);
}

Number
dot(const Vector &v1, const Vector &v2)
{
        // check dimensions
        assert(v1.dimension > 0 && v2.dimension > 0);

        // calculate dot product
        Number sum = 0.0;
        int maxd = 
                (v1.dimension < v2.dimension) ? v1.dimension : v2.dimension;
        for (int id = 0; id < maxd; id++)
        {
                sum += v1[id]*conj(v2[id]);
        }

        // return the sum
        return(sum);
}

Number
norm(const Vector &v)
{
        // return vector magnitudes
        return(sqrt(dot(v, v)));
}

// print vector
void
Vector::dump(ostream &os) const
{
        os << "{";
        for (int ic = 0; ic < dimension; )
        {
                vector[ic].dump(os);
                if (++ic != dimension) os << ",";
        }
        os << "}";
        return;
}

ostream &
operator<<(ostream &os, const Vector &v)
{
        v.dump(os);
        return(os);
}
