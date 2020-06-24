// This may look like C code, but it is really -*- C++ -*-
/*
 ************************************************************************
 *
 *			Linear Algebra Package
 *
 *			Find the matrix inverse
 *		for matrices of general and special forms
 *
 * $Id: matrix_inv.cc,v 3.1 1995/01/31 17:17:41 oleg Exp oleg $
 *
 ************************************************************************
 */

#include "LinAlg.h"
#include <math.h>
#include <alloca.h>

/*
 *------------------------------------------------------------------------
 *
 *		The most general (Gauss-Jordan) matrix inverse
 *
 * This method works for any matrix (which of course must be square and
 * non-singular). Use this method only if none of specialized algorithms
 * (for symmetric, tridiagonal, etc) matrices isn't applicable/available.
 * Also, the matrix to invert has to be _well_ conditioned:
 * Gauss-Jordan eliminations (even with pivoting) perform poorly for
 * near-singular matrices (e.g., Hilbert matrices).
 *
 * The method inverts matrix inplace and returns the determinant if
 * determ_ptr was specified as not nil. determinant will be exactly zero
 * if the matrix turns out to be (numerically) singular. If determ_ptr is
 * nil and matrix happens to be singular, throw up.
 *
 * The algorithm perform inplace Gauss-Jordan eliminations with
 * full pivoting. It was adapted from my Algol-68 "translation" (ca 1986)
 * of the FORTRAN code described in
 * Johnson, K. Jeffrey, "Numerical methods in chemistry", New York,
 * N.Y.: Dekker, c1980, 503 pp, p.221
 *
 * Note, since it's much more efficient to perform operations on matrix
 * columns rather than matrix rows (due to the layout of elements in the
 * matrix), the present method implements a "transposed" algorithm.
 *
 */

Matrix& Matrix::invert(double * determ_ptr)
{
  is_valid();
  if( nrows != ncols )
    info(),
    _error("Matrix to invert must be square");

  double determinant = 1;
  const double singularity_tolerance = 1e-35;

				// Locations of pivots (indices start with 0)
  struct Pivot { int row, col; } * const pivots = 
  			(Pivot*)alloca(ncols*sizeof(Pivot));
  bool * const was_pivoted = (bool*)alloca(nrows*sizeof(bool)); 
  memset(was_pivoted,false,nrows*sizeof(bool));
  register Pivot * pivotp;

  for(pivotp = &pivots[0]; pivotp < &pivots[ncols]; pivotp++)
  {
    int prow = 0, pcol = 0;		// Location of a pivot to be
    {					// Look through all non-pivoted cols
      REAL max_value = 0;		// (and rows) for a pivot (max elem)
      for(register int j=0; j<ncols; j++)
	if( !was_pivoted[j] )
	{
	  register REAL * cp;
	  register int k;
	  REAL curr_value = 0;
	  for(k=0,cp=index[j]; k<nrows; k++,cp++)
	    if( !was_pivoted[k] && (curr_value = fabs(*cp)) > max_value )
	      max_value = curr_value, prow = k, pcol = j;
	}
      if( max_value < singularity_tolerance )
	if( determ_ptr )
	{
	  *determ_ptr = 0;
	  return *this;
	}
        else
	  _error("Matrix turns out to be singular: can't invert");
      pivotp->row = prow;
      pivotp->col = pcol;
    }

    if( prow != pcol )			// Swap prow-th and pcol-th columns to
    {					// bring the pivot to the diagonal
      register REAL * cr = index[prow];
      register REAL * cc = index[pcol];
      for(register int k=0; k<nrows; k++)
      {
	REAL temp = *cr; *cr++ = *cc; *cc++ = temp;
      }
    }
    was_pivoted[prow] = true;

    {					// Normalize the pivot column and
      register REAL * pivot_cp = index[prow];
      double pivot_val = pivot_cp[prow];	// pivot is at the diagonal
      determinant *= pivot_val;		// correct the determinant
      pivot_cp[prow] = true;
      for(register int k=0; k<nrows; k++)
	*pivot_cp++ /= pivot_val;
    }

    {					// Perform eliminations
      register REAL * pivot_rp = elements + prow;	// pivot row
      for(register int k=0; k<nrows; k++, pivot_rp += nrows)
	if( k != prow )
	{
	  double temp = *pivot_rp;
	  *pivot_rp = 0;
	  register REAL * pivot_cp = index[prow];	// pivot column
	  register REAL * elim_cp  = index[k];		// elimination column
	  for(register int l=0; l<nrows; l++)
	    *elim_cp++ -= temp * *pivot_cp++;
	}
    }
  }

  int no_swaps = 0;		// Swap exchanged *rows* back in place
  for(pivotp = &pivots[ncols-1]; pivotp >= &pivots[0]; pivotp--)
    if( pivotp->row != pivotp->col )
    {
      no_swaps++;
      register REAL * rp = elements + pivotp->row;
      register REAL * cp = elements + pivotp->col;
      for(register int k=0; k<ncols; k++, rp += nrows, cp += nrows)
      {
	REAL temp = *rp; *rp = *cp; *cp = temp;
      }
    }

  if( determ_ptr )
    *determ_ptr = ( no_swaps & 1 ? -determinant : determinant );

  return *this;
}


				// Allocate new matrix and set it to inv(m)
void Matrix::_invert(const Matrix& m)
{
  m.is_valid();
  allocate(m.nrows,m.ncols,m.row_lwb,m.col_lwb);
  *this = m;
  invert(0);
}
  
