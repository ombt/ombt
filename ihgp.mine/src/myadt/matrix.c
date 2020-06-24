// matrix class functions

// headers
#include <stdlib.h>
#include <assert.h>
#include <math.h>

// local headers
#include "matrix.h"

// constructors and destructor
Matrix::Matrix(int rows, int cols)
{
        // store dimensions
        nrows = rows;
        ncols = cols;
        assert(nrows > 0 && ncols > 0);

        // allocate a matrix
        matrix = new Number *[nrows];
        assert(matrix != (Number **)0);
        for (int ir = 0; ir < nrows; ir++)
        {
                matrix[ir] = new Number [ncols];
                assert(matrix[ir] != (Number *)0);
        }
}

Matrix::Matrix(const Matrix &m)
{
        // store dimensions
        nrows = m.nrows;
        ncols = m.ncols;
        assert(nrows > 0 && ncols > 0);

        // allocate a matrix
        matrix = new Number *[nrows];
        assert(matrix != (Number **)0);
        for (int ir = 0; ir < nrows; ir++)
        {
                matrix[ir] = new Number [ncols];
                assert(matrix[ir] != (Number *)0);
                for (int ic = 0; ic < ncols; ic++)
                {
                        matrix[ir][ic] = m.matrix[ir][ic];
                }
        }
}

Matrix::~Matrix()
{
        for (int ir = 0; ir < nrows; ir++)
        {
                delete [] matrix[ir];
        }
        delete [] matrix;
        matrix = (Number **)0;
}

// assignment operations
Matrix &
Matrix::operator=(const Matrix &m)
{
        // check if assigning to itself
        if (this == &m) return(*this);

        // delete matrix
        for (int ir = 0; ir < nrows; ir++)
        {
                delete [] matrix[ir];
        }
        delete [] matrix;
        matrix = (Number **)0;

        // store new matrix dimension
        nrows = m.nrows;
        ncols = m.ncols;
        assert(nrows > 0 && ncols > 0);

        // allocate a matrix
        matrix = new Number *[nrows];
        assert(matrix != (Number **)0);
        for (ir = 0; ir < nrows; ir++)
        {
                matrix[ir] = new Number [ncols];
                assert(matrix[ir] != (Number *)0);
                for (int ic = 0; ic < ncols; ic++)
                {
                        matrix[ir][ic] = m.matrix[ir][ic];
                }
        }

        // all done
        return(*this);
}

Number &
Matrix::operator()(int row, int col)
{
        assert((0 <= row && row < nrows) && ( 0 <= col && col < ncols));
        return(matrix[row][col]);
}

Number &
Matrix::operator()(int row, int col) const
{
        assert((0 <= row && row < nrows) && ( 0 <= col && col < ncols));
        return(matrix[row][col]);
}

// matrix operations
Matrix &
Matrix::operator+=(const Matrix &m)
{
        // check that rows and columns match
        assert(nrows == m.nrows && ncols == m.ncols);

        // add element by element
        for (int ir = 0; ir < nrows; ir++)
        {
                for (int ic = 0; ic < ncols; ic++)
                {
                        matrix[ir][ic] += m.matrix[ir][ic];
                }
        }

        // all done
        return(*this);
}

Matrix &
Matrix::operator-=(const Matrix &m)
{
        // check that rows and columns match
        assert(nrows == m.nrows && ncols == m.ncols);

        // subtract element by element
        for (int ir = 0; ir < nrows; ir++)
        {
                for (int ic = 0; ic < ncols; ic++)
                {
                        matrix[ir][ic] -= m.matrix[ir][ic];
                }
        }

        // all done
        return(*this);
}

Matrix &
Matrix::operator*=(const Matrix &m)
{
        // check that rows and columns match
        assert(ncols == m.nrows && ncols > 0);

        // get size of new matrix
        int tmpnrows = nrows;
        int tmpncols = m.ncols;
        assert(tmpnrows > 0 && tmpncols > 0);
        int nsum = ncols;

        // allocate a new matrix
        Number **tmp = new Number *[tmpnrows];
        assert(tmp != (Number **)0);
        for (int ir = 0; ir < tmpnrows; ir++)
        {
                matrix[ir] = new Number [tmpncols];
                assert(matrix[ir] != (Number *)0);
        }

        // multiply element by element
        for (ir = 0; ir < tmpnrows; ir++)
        {
                for (int ic = 0; ic < tmpncols; ic++)
                {
                        for (int is = 0; is < nsum; is++)
                        {
                                tmp[ir][ic] = 
                                        matrix[ir][is]*m.matrix[is][ic];
                        }
                }
        }

        // delete old matrix and save new one
        for (ir = 0; ir < nrows; ir++)
        {
                delete [] matrix[ir];
        }
        delete [] matrix;
        matrix = tmp;
        nrows = tmpnrows;
        ncols = tmpncols;

        // all done
        return(*this);
}

Matrix &
Matrix::operator/=(const Matrix &m)
{
        // get inverse and multiply
        Matrix invm(m);
        inverse(invm);
        return(*this /= invm);
}


Matrix
Matrix::operator+(const Matrix &m) const
{
        return(Matrix(*this) += m);
}

Matrix
Matrix::operator-(const Matrix &m) const
{
        return(Matrix(*this) -= m);
}

Matrix
Matrix::operator*(const Matrix &m) const
{
        return(Matrix(*this) *= m);
}

Matrix
Matrix::operator/(const Matrix &m) const
{
        return(Matrix(*this) /= m);
}

// matrix and vector operations
Vector
Matrix::operator*(const Vector &v) const
{
        // check that rows and columns match
        assert(ncols == v.getDimension() && ncols > 0);

        // new vector to hold results
        Vector tmpv(nrows);

        // multiply element by element
        for (int ir = 0; ir < nrows; ir++)
        {
                for (int is = 0;is < ncols; is++)
                {
                        tmpv[ir] += matrix[ir][is]*v[is];
                }
        }

        // all done
        return(tmpv);
}

Vector
operator*(const Vector &v, const Matrix &m)
{
        // check that rows and columns match
        assert(v.getDimension() == m.nrows && m.nrows > 0);

        // new vector to hold results
        Vector tmpv(m.ncols);

        // multiply element by element
        for (int ic = 0; ic < m.ncols; ic++)
        {
                for (int is = 0; is < m.nrows; is++)
                {
                        tmpv[ic] += v[is]*m.matrix[is][ic];
                }
        }

        // all done
        return(tmpv);
}

// matrix and scalar operations
Matrix &
Matrix::operator*=(const Number &n)
{
        // check dimensions
        assert(nrows > 0 && ncols > 0);

        // multiply matrix by scalar
        for (int ir = 0; ir < nrows; ir++)
        {
                for (int ic = 0; ic < ncols; ic++)
                {
                        matrix[ir][ic] *= n;
                }
        }

        // all done
        return(*this);
}

Matrix &
Matrix::operator/=(const Number &n)
{
        // check dimensions
        assert(nrows > 0 && ncols > 0);
        assert(n != 0.0);

        // multiply matrix by scalar
        for (int ir = 0; ir < nrows; ir++)
        {
                for (int ic = 0; ic < ncols; ic++)
                {
                        matrix[ir][ic] /= n;
                }
        }

        // all done 
        return(*this);
}


Matrix
Matrix::operator*(const Number &n) const
{
        return(Matrix(*this) *= n);
}

Matrix
Matrix::operator/(const Number &n) const
{
        return(Matrix(*this) /= n);
}

Matrix
operator*(const Number &n, const Matrix &m)
{
        // switch around and multiply
        return(m*n);
}

Matrix &
Matrix::operator*=(double d)
{
        return(*this *= Number(d));
}

Matrix &
Matrix::operator/=(double d)
{
        return(*this /= Number(d));
}


Matrix
Matrix::operator*(double d) const
{
        return(Matrix(*this) *= Number(d));
}

Matrix
Matrix::operator/(double d) const
{
        return(Matrix(*this) /= Number(d));
}

Matrix
operator*(double d, const Matrix &m)
{
        // switch around and multiply
        return(m*Number(d));
}

// logical operators
int
Matrix::operator==(const Matrix &m) const
{
        // check if the same matrix
        if (this == &m) return(1);

        // check if dimensions are the same
        if (nrows!= m.nrows || ncols != m.ncols) return(0);

        // compare element by element
        for (int ir = 0; ir < nrows; ir++)
        {
                for (int ic = 0; ic < ncols; ic++)
                {
                        if (matrix[ir][ic] != m.matrix[ir][ic])
                        {      
                                // a mismatch
                                return(0);
                        }
                }
        }

        // matrices are the same
        return(1);
}

int
Matrix::operator!=(const Matrix &m) const
{
        return( ! (*this == m));
}

// matrix transformation functions
int
transpose(Matrix &m)
{
        // create transpose matrix
        Matrix tmp(m.ncols, m.nrows);

        // transpose element and copy
        for (int ir = 0; ir < m.nrows; ir++)
        {
                for (int ic = 0; ic < m.ncols; ic++)
                {
                        tmp(ic, ir) = m(ir, ic);
                }
        }
        m = tmp;

        // all done
        return(OK);

}

int
conjugate(Matrix &m)
{
        // conjugate element and copy
        for (int ir = 0; ir < m.nrows; ir++)
        {
                for (int ic = 0; ic < m.ncols; ic++)
                {
                        m(ic, ir) = conj(m(ir, ic));
                }
        }

        // all done
        return(OK);

}

int
inverse(Matrix &m, double epsilon)
{
        // check dimensions
        if ((m.nrows != m.ncols) || (m.nrows <= 0))
        {
                ERROR("invalid dimensions");
                return(NOTOK);
        }
        epsilon = abs(epsilon);

        // set loop limit
        int nd = m.nrows;

        // create augmented matrix and initialize
        Matrix augm(m.nrows, 2*m.ncols);
        for (int ir = 0; ir < m.nrows; ir++)
        {
                for (int ic = 0; ic < m.ncols; ic++)
                {
                        augm(ir, ic) = m(ir, ic);
                }
                augm(ir, m.ncols+ir) = 1.0;
        }

        // implement a gaussian elimination with partial pivoting
        for (int id = 0; id < nd; id++)
        {
                // find maximum element in column
                Number maxv = abs(m(id, id));
                int maxir = id;
                for (int ir = id+1; ir < m.nrows; ir++)
                {
                        if (abs(augm(ir, id)) > maxv)
                        {
                                maxv = abs(augm(ir, id));
                                maxir = ir;
                        }
                }

                // check maximum value in column for singularity
                if (maxv <= epsilon)
                {
                        // matrix is or is almost singular
			ERRORF("matrix is singular", maxv);
                        return(NOTOK);
                }

                // swap rows
                if (maxir != id)
                {
                        // swap matrix rows and vector values
                        for (int ic = id; ic < 2*m.ncols; ic++)
                        {
                                maxv = augm(maxir, ic);
                                augm(maxir, ic) = augm(id, ic);
                                augm(id, ic) = maxv;
                        }

                }

                // do gaussian elimination for other rows and columns
                for (ir = 0; ir < m.nrows; ir++)
                {
                        if (ir != id)
                        {
                                Number scale = augm(ir, id)/augm(id, id);
                                for (int ic = id; ic < 2*m.ncols; ic++)
                                {
                                        augm(ir, ic) -= scale*augm(id, ic);
                                }
                        }
                        else
                        {
                                Number scale = 1.0/augm(id, id);
                                for (int ic = id; ic < 2*m.ncols; ic++)
                                {
                                        augm(ir, ic) *= scale;
                                }
                        }
                }
        }

        // store inverse in original matrix
        for (ir = 0; ir < m.nrows; ir++)
        {
                for (int ic = 0; ic < m.ncols; ic++)
                {
                        m(ir, ic) = augm(ir, m.ncols+ic);
                }
        }

        // all done
        return(OK);
}

int
solve(Matrix &m, Vector &x, Vector &b, double epsilon)
{
        // check dimensions
        if ((m.nrows != m.ncols) || 
            (x.getDimension() != b.getDimension()) ||
            (m.nrows != b.getDimension()) || (m.nrows <= 0))
        {
                ERROR("invalid dimensions");
                return(NOTOK);
        }
        epsilon = abs(epsilon);

        // set loop limit
        int nd = m.nrows;

        // implement a gaussian elimination with partial pivoting
        for (int id = 0; id < nd; id++)
        {
                // find maximum element in column
                Number maxv = abs(m(id, id));
                int maxir = id;
                for (int ir = id+1; ir < m.nrows; ir++)
                {
                        if (abs(m(ir, id)) > maxv)
                        {
                                maxv = abs(m(ir, id));
                                maxir = ir;
                        }
                }

                // check maximum value in column for singularity
                if (maxv <= epsilon)
                {
                        // matrix is or is almost singular
			ERRORF("matrix is singular", maxv);
                        return(NOTOK);
                }

                // swap rows
                if (maxir != id)
                {
                        // swap matrix rows and vector values
                        for (int ic = id; ic < m.ncols; ic++)
                        {
                                maxv = m(maxir, ic);
                                m(maxir, ic) = m(id, ic);
                                m(id, ic) = maxv;
                        }
                        Number tmpb = b[maxir];
                        b[maxir] = b[id];
                        b[id] = tmpb;
                }

                // do gaussian elimination for other rows and columns
                for (ir = id+1; ir < m.nrows; ir++)
                {
                        Number scale = m(ir, id)/m(id, id);
                        for (int ic = id; ic < m.ncols; ic++)
                        {
                                m(ir, ic) -= scale*m(id, ic);
                        }
                        b[ir] -= scale*b[id];
                }
        }

        // store inverse in original matrix
        for (int ir = m.nrows-1; ir >= 0; ir--)
        {
                x[ir] = b[ir];
                for (int ic = ir+1; ic < m.ncols; ic++)
                {
                        x[ir] -= m(ir, ic)*x[ic];
                }
                x[ir] /= m(ir, ic);
        }

        // all done
        return(OK);
}

int
determinant(const Matrix &m, Number &det, double epsilon)
{
        // check dimensions
        if ((m.nrows != m.ncols) || (m.nrows <= 0))
        {
                ERROR("invalid dimensions");
                return(NOTOK);
        }
        epsilon = abs(epsilon);
        Matrix tmp(m);

        // set loop limit
        int nd = m.nrows;

        // implement a gaussian elimination with partial pivoting
        int nswaps = 0;
        for (int id = 0; id < nd; id++)
        {
                // find maximum element in column
                Number maxv = abs(tmp(id, id));
                int maxir = id;
                for (int ir = id+1; ir < tmp.nrows; ir++)
                {
                        if (abs(tmp(ir, id)) > maxv)
                        {
                                maxv = abs(tmp(ir, id));
                                maxir = ir;
                        }
                }

                // check maximum value in column for singularity
                if (maxv <= epsilon)
                {
                        // matrix is or is almost singular
			ERRORF("matrix is singular", maxv);
                        return(NOTOK);
                }

                // swap rows
                if (maxir != id)
                {
                        // swap matrix rows and vector values
                        for (int ic = id; ic < tmp.ncols; ic++)
                        {
                                maxv = tmp(maxir, ic);
                                tmp(maxir, ic) = tmp(id, ic);
                                tmp(id, ic) = maxv;
                        }
                        nswaps++;
                }

                // do gaussian elimination for other rows and columns
                for (ir = id+1; ir < tmp.nrows; ir++)
                {
                        Number scale = tmp(ir, id)/tmp(id, id);
                        for (int ic = id; ic < tmp.ncols; ic++)
                        {
                                tmp(ir, ic) -= scale*tmp(id, ic);
                        }
                }
        }

        // store inverse in original matrix
        for (id = 1, det = tmp(0, 0); id < tmp.nrows; id++)
        {
                det += tmp(id, id);
        }
        if (nswaps%2 != 0) det *= -1.0;

        // all done
        return(OK);
}

int
trace(const Matrix &m, Number &tr)
{
        // matrix should be square
        assert(m.nrows == m.ncols && m.nrows > 0);

        // add up diagonal elements
        tr = 0.0;
        for (int id = 0; id < m.nrows; id++)
        {
                tr += m(id, id);
        }

        // all done
        return(OK);
}

// print matrix
void
Matrix::dump(ostream &os) const
{
        os << "matrix[" << nrows << "," << ncols << "] = {" << endl;
        for (int ir = 0; ir < nrows; ir++)
        {
                for (int ic = 0; ic < ncols; ic++)
                {
                        os << matrix[ir][ic] << " ";
                }
                os << endl;
        }
        os << "}" << endl;
        return;
}

ostream &
operator<<(ostream &os, const Matrix &m)
{
        m.dump(os);
        return(os);
}

