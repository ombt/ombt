#ifndef __MATRIX_H
#define __MATRIX_H

// matrix class definitions

// headers
#include <stdlib.h>
#include <iostream.h>
#include <math.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "number.h"
#include "vector.h"

// forward declarations
class Matrix;

// local definitions
#define DEFAULTROWS 3
#define DEFAULTCOLUMNS 3
#define EPSILON 1.0e-8

// matrix class definition
class Matrix {
public:
        // constructors and destructor
        Matrix(int = DEFAULTROWS, int = DEFAULTCOLUMNS);
        Matrix(const Matrix &);
        virtual ~Matrix();

        // assignment operators
        virtual Matrix &operator=(const Matrix &);
        virtual Number &operator()(int, int);
        virtual Number &operator()(int, int) const;

        // matrix operations
        virtual Matrix &operator+=(const Matrix &);
        virtual Matrix &operator-=(const Matrix &);
        virtual Matrix &operator*=(const Matrix &);
        virtual Matrix &operator/=(const Matrix &);
        virtual Matrix operator+(const Matrix &) const;
        virtual Matrix operator-(const Matrix &) const;
        virtual Matrix operator*(const Matrix &) const;
        virtual Matrix operator/(const Matrix &) const;

        // matrix and vcetor operations
        virtual Vector operator*(const Vector &) const;
        friend Vector operator*(const Vector &, const Matrix &);

        // matrix and scalar operations
        virtual Matrix &operator*=(const Number &);
        virtual Matrix &operator/=(const Number &);
        virtual Matrix operator*(const Number &) const;
        virtual Matrix operator/(const Number &) const;
        friend Matrix operator*(const Number &, const Matrix &);
        virtual Matrix &operator*=(double);
        virtual Matrix &operator/=(double);
        virtual Matrix operator*(double) const;
        virtual Matrix operator/(double) const;
        friend Matrix operator*(double, const Matrix &);

        // logical operators
        virtual int operator==(const Matrix &) const;
        virtual int operator!=(const Matrix &) const;

        // matrix tranformations
        friend int transpose(Matrix &);
        friend int conjugate(Matrix &);
        friend int inverse(Matrix &, double = EPSILON);
        friend int solve(Matrix &, Vector &, Vector &, double = EPSILON);
        friend int determinant(const Matrix &, Number &, double = EPSILON);
        friend int trace(const Matrix &, Number &);

        // other functions
        inline int getRows() { return(nrows); }
        inline int getCols() { return(ncols); }
        virtual void dump(ostream & = cout) const;
        friend ostream &operator<<(ostream &, const Matrix &);

protected:
        // internal data
        Number **matrix;
        int nrows, ncols;
};

#endif
