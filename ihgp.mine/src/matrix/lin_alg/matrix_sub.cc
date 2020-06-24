// This may look like C code, but it is really -*- C++ -*-
/*
 ************************************************************************
 *
 *			Linear Algebra Package
 *
 *		Basic linear algebra operations, levels 1 & 2
 *	    Operations on a single row, column, or the diagonal
 *			   	of a matrix
 *
 * $Id: matrix_sub.cc,v 3.1 1995/01/31 17:27:47 oleg Exp oleg $
 *
 ************************************************************************
 */

#include "LinAlg.h"
#include <math.h>


/*
 *------------------------------------------------------------------------
 *		    Messing with a single column of a matrix
 */

 				// For every element, do `elem OP value`
#define COMPUTED_VAL_ASSIGNMENT(OP,VALTYPE)				\
									\
void MatrixColumn::operator OP (const VALTYPE val)			\
{									\
  matrix.is_valid();							\
									\
  register REAL * cp = ptr;		/* Column ptr */		\
  while( cp < ptr + matrix.nrows )					\
    *cp++ OP val;							\
}									\

COMPUTED_VAL_ASSIGNMENT(=,REAL)
COMPUTED_VAL_ASSIGNMENT(+=,double)
COMPUTED_VAL_ASSIGNMENT(*=,double)

#undef COMPUTED_VAL_ASSIGNMENT


				// Assign a vector to a matrix col
void MatrixColumn::operator = (const Vector & vec)
{
  matrix.is_valid();
  vec.is_valid();

  if( matrix.row_lwb != vec.row_lwb || matrix.nrows != vec.nrows )
    matrix.info(), vec.info(),
   _error("Vector cannot be assigned to a column of the mentioned matrix");

  register REAL * cp = ptr;		// Column ptr
  register REAL * vp = vec.elements;	// Vector ptr
  while( cp < ptr + matrix.nrows )
    *cp++ = *vp++;

  assert( vp == vec.elements + vec.nrows );
}

				// Assign a matrix column to a vector
Vector& Vector::operator = (const MatrixColumn & mc)
{
  is_valid();
  mc.matrix.is_valid();

  if( mc.matrix.q_row_lwb() != row_lwb || mc.matrix.q_nrows() != nrows )
    mc.matrix.info(), info(),
   _error("Can't assign a column of the matrix above to the vector");

  register REAL * cp = mc.ptr;		// Column ptr
  register REAL * vp = elements;	// Vector ptr
  while( vp < elements + nrows )
    *vp++ = *cp++;

  assert( cp == mc.ptr + mc.matrix.q_nrows() );

  return *this;
}


/*
 *------------------------------------------------------------------------
 *		     Messing with a single row of a matrix
 *  Keep in mind the matrix elements are arranged in columns!
 */


 				// For every element, do `elem OP value`
#define COMPUTED_VAL_ASSIGNMENT(OP,VALTYPE)				\
									\
void MatrixRow::operator OP (const VALTYPE val)				\
{									\
  matrix.is_valid();							\
									\
  register REAL * rp = ptr;		/* Row ptr */			\
  for(; rp < ptr + matrix.nelems; rp += inc)				\
    *rp OP val;								\
}									\

COMPUTED_VAL_ASSIGNMENT(=,REAL)
COMPUTED_VAL_ASSIGNMENT(+=,double)
COMPUTED_VAL_ASSIGNMENT(*=,double)

#undef COMPUTED_VAL_ASSIGNMENT


				// Assign a vector to a matrix row
				// The vector is considered row-vector
				// to allow the assignment in the strict
				// sense
void MatrixRow::operator = (const Vector & vec)
{
  matrix.is_valid();
  vec.is_valid();

  if( matrix.col_lwb != vec.row_lwb || matrix.ncols != vec.nrows )
    matrix.info(), vec.info(),
   _error("Transposed vector cannot be assigned to a row of the matrix");

  register REAL * rp = ptr;		// Row ptr
  register REAL * vp = vec.elements;	// Vector ptr
  for(; rp < ptr + matrix.nelems; rp += inc)
    *rp = *vp++;

  assert( vp == vec.elements + vec.nrows );
}

				// Assign a matrix row to a vector
				// The matrix row is implicitly transposed
				// to allow the assignment in the strict
				// sense
Vector& Vector::operator = (const MatrixRow & mr)
{
  is_valid();
  mr.matrix.is_valid();

  if( mr.matrix.col_lwb != row_lwb || mr.matrix.ncols != nrows )
    mr.matrix.info(), info(),
   _error("Can't assign the transposed row of the matrix to the vector");

  register REAL * rp = mr.ptr;		// Row ptr
  register REAL * vp = elements;	// Vector ptr
  for(; vp < elements + nrows; rp += mr.inc)
    *vp++ = *rp;

  assert( rp == mr.ptr + mr.matrix.nelems );

  return *this;
}

/*
 *------------------------------------------------------------------------
 *			Messing with the matrix diagonal
 */

 				// For every element, do `elem OP value`
#define COMPUTED_VAL_ASSIGNMENT(OP,VALTYPE)				\
									\
void MatrixDiag::operator OP (const VALTYPE val)			\
{									\
  matrix.is_valid();							\
									\
  register REAL * dp = ptr;		/* Diag ptr */			\
  register int i;							\
  for(i=0; i < ndiag; i++, dp += inc)					\
    *dp OP val;								\
}									\

COMPUTED_VAL_ASSIGNMENT(=,REAL)
COMPUTED_VAL_ASSIGNMENT(+=,double)
COMPUTED_VAL_ASSIGNMENT(*=,double)

#undef COMPUTED_VAL_ASSIGNMENT


				// Assign a vector to the matrix diagonal
void MatrixDiag::operator = (const Vector & vec)
{
  matrix.is_valid();
  vec.is_valid();

  if( ndiag != vec.nrows )
    matrix.info(), vec.info(),
   _error("Vector cannot be assigned to the matrix diagonal");

  register REAL * dp = ptr;		// Diag ptr
  register REAL * vp = vec.elements;	// Vector ptr
  for(; vp < vec.elements + vec.nrows; dp += inc)
    *dp = *vp++;

  assert( dp < ptr + matrix.nelems + inc );
}


				// Assign the matrix diagonal to a vector
Vector& Vector::operator = (const MatrixDiag & md)
{
  is_valid();
  md.matrix.is_valid();

  if( md.ndiag != nrows )
    md.matrix.info(), info(),
   _error("Can't assign the diagonal of the matrix to the vector");

  register REAL * dp = md.ptr;		// Diag ptr
  register REAL * vp = elements;	// Vector ptr
  for(; vp < elements + nrows; dp += md.inc)
    *vp++ = *dp;

  assert( dp < md.ptr + md.matrix.nelems + md.inc );

  return *this;
}

/*
 *------------------------------------------------------------------------
 *		   Multiplications with the diagonal matrix
 */

				// Multiply a matrix by the diagonal
				// of another matrix
				// matrix(i,j) *= diag(j)
Matrix& Matrix::operator *= (const MatrixDiag& diag)
{
  is_valid();
  diag.matrix.is_valid();

  if( ncols != diag.ndiag )
    info(), diag.matrix.info(),
   _error("Matrix cannot be multiplied by the diagonal of the other matrix");

  register REAL * dp = diag.ptr;	// Diag ptr
  register REAL * mp = elements;	// Matrix ptr
  register int i;
  for(; mp < elements + nelems; dp += diag.inc)
    for(i=0; i<nrows; i++)
      *mp++ *= *dp;

  assert( dp < diag.ptr + diag.matrix.nelems + diag.inc );

  return *this;
}
