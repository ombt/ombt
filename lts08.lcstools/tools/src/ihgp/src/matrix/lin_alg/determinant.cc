// This may look like C code, but it is really -*- C++ -*-
/*
 ************************************************************************
 *
 *			Linear Algebra Package
 *
 *	    Compute the determinant of a general square matrix
 *
 * Synopsis
 *	Matrix A;
 *	double A.determinant();
 * The matrix is assumed to be square. It is not altered.
 *
 * Method
 *	Gauss-Jordan transformations of the matrix
 *	Matrix elements are arranged in columns. But it doesn't really
 *	matter since the determinant remains invariant under the
 *	matrix transposition. Therefore, it makes sence to eliminate rows
 *	rather than columnss in the Gauss-Jordan transformations.
 *	The matrix is copied to a special object of type MatrixPivoting,
 *	where all Gauss-Jordan eliminations with full pivoting are to
 *	take place.
 *
 * $Id: determinant.cc,v 2.0 1994/03/25 15:02:29 oleg Exp $
 *
 ************************************************************************
 */

#include "LinAlg.h"
#include <math.h>

/*
 *------------------------------------------------------------------------
 *			Class MatrixPivoting
 *
 * It is a descendant of a Matrix which keeps some information
 * that makes pivoting easy. 
 */

class MatrixPivoting
  : public Matrix
{
  int * row_pivoted;			// row_pivoted[i] = 1 if the i-th
					// row has been pivoted. Note,
					// pivoted columns are marked
					// by putting index[j] to zero.

				// Information about the pivot that was
				// just picked up
  double pivot_value;			// Value of the pivoting element
  int pivot_row;			// Row and column for the pivot
  int pivot_col;			// just found
  int pivot_parity;			// +1/-1, reflects the parity

  void pick_up_pivot(void);		// Pick up a pivot from the
					// not-pivoted rows and cols

public:
  MatrixPivoting(const Matrix& m);	// Construct an object 
					// for a given matrix
  ~MatrixPivoting(void);

  double pivoting_and_elimination(void);// Perform the pivoting, return
					// the pivot value times (-1)^(pi+pj)
					// (pi,pj - pivot el row & col)
};


/*
 *------------------------------------------------------------------------
 *		Constructing and decomissioning MatrixPivoting
 */

MatrixPivoting::MatrixPivoting(const Matrix& m)
  : Matrix(m.q_row_lwb(),m.q_row_upb(),m.q_col_lwb(),m.q_col_upb())
{
  m.is_valid();
  memcpy(elements,m.elements,nelems*sizeof(REAL));
  assert( (row_pivoted = (int *)calloc(nrows,sizeof(row_pivoted[0]))) != 0 );
}

MatrixPivoting::~MatrixPivoting(void)
{
  is_valid();
  assert( row_pivoted != 0 );
  free(row_pivoted);
}

/*
 *------------------------------------------------------------------------
 *				Pivoting itself
 */

			// Pick up a pivot, the element with the largest
			// abs value from the yet not-pivoted rows and cols
void MatrixPivoting::pick_up_pivot(void)
{
  register int i,j;
  register int si,sj;			// No. of already pivoted rows & cols
  register REAL max_elem = -1;		// Abs value of the largest element
  register REAL * mp;

  for(j=0,sj=0; j<ncols; j++)		// Picking up a not-pivoted column
    if(index[j] == 0)
      sj++;				// Skip already pivoted columns
    else
      for(i=0,si=0,mp=index[j]; i<nrows; i++)
	if(row_pivoted[i])
	  mp++, si++;			// Skip elems in already pivoted rows
	else
	{
	  register REAL v = *mp++;
	  if( fabs(v) > max_elem )
	  {
	    max_elem = fabs(v);
	    pivot_value = v;
	    pivot_col = j;
	    pivot_row = i;
	    pivot_parity = i+j-si-sj;	// No. of transpositions for the pivot
	  }
	}

  assure( max_elem >= 0, 
	 "All the rows and columns have been already pivoted and eliminated");
  pivot_parity = (pivot_parity & 1 ? -1 : 1);	// (-1)^pivot_parity
}

			// Perform pivoting and gaussian elemination,
			// return the pivot value times pivot_parity
			// The procedure places zeros to the pivot_row of
			// all not yet pivoted columns
			// A[i,j] -= A[i,pivot_col]/pivot*A[pivot_row,j]
double MatrixPivoting::pivoting_and_elimination(void)
{
  is_valid();

  pick_up_pivot();
  if( pivot_value == 0 )
    return 0;

  REAL * pvc = index[pivot_col];		// Pivoted column ptr
  assert( pvc != 0 );
  register REAL * wp;				// Work pointer
  register int i,j;

  index[pivot_col] = 0;			// Mark pivot_row & pivot_col
  row_pivoted[pivot_row] = 1;		// as being pivoted

  for(i=0,wp=pvc; i<nrows; i++)		// Divide the pivoted column by pivot
    if(row_pivoted[i])
      wp++;				// Skip already pivoted rows
    else
      *wp++ /= pivot_value;

  for(j=0; j<ncols; j++)		// Eliminate all the elements from
    if(index[j] != 0)			// the pivot_row in all not pivoted
    {					// columns
      register REAL * mp = index[j];	// mp = A[i,j]
      wp = pvc;				// wp = A[i,pivot_col]/pivot_value
      register double fac = mp[pivot_row]; // fac = A[pivot_row,j]
      for(i=0; i<nrows; i++)
	if(row_pivoted[i])
	  mp++,wp++;			// Skip pivoted rows
        else
	  *mp++ -= *wp++ * fac;
    }

  return pivot_value * pivot_parity;
}


/*
 *------------------------------------------------------------------------
 *				Root module
 */

double Matrix::determinant(void) const
{
  is_valid();

  if( nrows != ncols )
    info(), _error("Can't obtain the determinant of a non_square matrix");

  if( row_lwb != col_lwb )
    info(), _error("Row and col lower bounds are inconsistent");

  MatrixPivoting mp(*this);

  register double det = 1;
  register int k;

  for(k=0; k<ncols && det != 0; k++)
    det *= mp.pivoting_and_elimination();

  return det;
}
