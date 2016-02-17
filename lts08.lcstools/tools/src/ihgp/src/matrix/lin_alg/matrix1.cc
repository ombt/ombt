// This may look like C code, but it is really -*- C++ -*-
/*
 ************************************************************************
 *
 *			Linear Algebra Package
 *
 *		Basic linear algebra operations, level 1
 *		      Element-wise operations
 *
 * $Id: matrix1.cc,v 3.0 1995/01/07 19:06:12 oleg Exp $
 *
 ************************************************************************
 */

#pragma implementation "LinAlg.h"

#include "LinAlg.h"
#include <math.h>


/*
 *------------------------------------------------------------------------
 *			Constructors and destructors
 */

void Matrix::allocate(
	const int no_rows,		// No. of rows
	const int no_cols,		// No. of cols
	const int row_lwb,		// Row index lower bound
	const int col_lwb		// Col index lower bound
)
{
  valid_code = MATRIX_val_code;

  assure((nrows=no_rows) > 0, "No. of matrix cols has got to be positive");
  assure((ncols=no_cols) > 0, "No. of matrix rows has got to be positive");

  Matrix::row_lwb = row_lwb;
  Matrix::col_lwb = col_lwb;

  name = "";

  nelems = nrows * ncols;

  assert( (elements = (REAL *)calloc(nelems,sizeof(REAL))) != 0 );

  if( ncols == 1 )		// Only one col - index is dummy actually
  {
    index = &elements;
    return;
  }
  
  assert( (index  = (REAL **)calloc(ncols,sizeof(REAL *))) != 0 );
  register int i;
  register REAL * col_p;
  for(i=0,col_p=&elements[0]; i<ncols; i++,col_p += nrows)
    index[i] = col_p;
}


Matrix::~Matrix(void)		// Dispose the Matrix struct
{
  is_valid();
  if( ncols != 1 )
    free(index);
  free(elements);
  if( name[0] != '\0' )
    delete name;
  valid_code = 0;
}

				// Set a new Matrix name
void Matrix::set_name(const char * new_name)
{
  if( name != 0 && name[0] != '\0' )	// Dispose of the previous matrix name
    delete name;

  if( new_name == 0 || new_name[0] == '\0' )
    name = "";				// Matrix is anonymous now
  else
    name = new char[strlen(new_name)+1], strcpy(name,new_name);
}

				// Erase the old matrix and create a
				// new one according to new boundaries
				// with indexation starting at 1
void Matrix::resize_to(const int nrows, const int ncols)
{
  is_valid();
  if( nrows == Matrix::nrows && ncols == Matrix::ncols )
    return;

  if( ncols != 1 )
    free(index);
  free(elements);

  char * old_name = name;
  allocate(nrows,ncols);
  name = old_name;
}
				// Erase the old matrix and create a
				// new one according to new boundaries
void Matrix::resize_to(const int row_lwb, const int row_upb,
		       const int col_lwb, const int col_upb)
{
  is_valid();
  const int new_nrows = row_upb-row_lwb+1;
  const int new_ncols = col_upb-col_lwb+1;
  Matrix::row_lwb = row_lwb;
  Matrix::col_lwb = col_lwb;
  if( new_nrows == Matrix::nrows && new_ncols == Matrix::ncols )
    return;

  if( ncols != 1 )
    free(index);
  free(elements);

  char * old_name = name;
  allocate(new_nrows,new_ncols,row_lwb,col_lwb);
  name = old_name;
}

					// Routing constructor module
Matrix::Matrix(const MATRIX_CREATORS_1op op, const Matrix& prototype)
{
  prototype.is_valid();
  switch(op)
  {
    case Zero:
         allocate(prototype.nrows,prototype.ncols,
		  prototype.row_lwb,prototype.col_lwb);
	 break;

    case Unit:
         allocate(prototype.nrows,prototype.ncols,
		  prototype.row_lwb,prototype.col_lwb);
	 unit_matrix();
	 break;

    case Transposed:
	 _transpose(prototype);
	 break;

    case Inverted:
	 _invert(prototype);
	 break;

    default:
	 _error("Operation %d is not yet implemented",op);
  }
}

					// Routing constructor module
Matrix::Matrix(const Matrix& A, const MATRIX_CREATORS_2op op, const Matrix& B)
{
  A.is_valid();
  B.is_valid();
  switch(op)
  {
    case Mult:
         _AmultB(A,B);
	 break;

    case TransposeMult:
	 _AtmultB(A,B);
	 break;

    default:
	 _error("Operation %d is not yet implemented",op);
  }
}

/*
 *------------------------------------------------------------------------
 * 		    Making a matrix of a special kind	
 */

				// Make a unit matrix
				// (Matrix needn't be a square one)
				// The matrix is traversed in the
				// natural (that is, col by col) order
Matrix& Matrix::unit_matrix(void)
{
  is_valid();
  register REAL *ep = elements;
  register int i,j;

  for(j=0; j < ncols; j++)
    for(i=0; i < nrows; i++)
        *ep++ = ( i==j ? 1.0 : 0.0 );

  return *this;
}

				// Make a Hilbert matrix
				// Hilb[i,j] = 1/(i+j-1), i,j=1...max, OR
				// Hilb[i,j] = 1/(i+j+1), i,j=0...max-1
				// (Matrix needn't be a square one)
				// The matrix is traversed in the
				// natural (that is, col by col) order
Matrix& Matrix::hilbert_matrix(void)
{
  is_valid();
  register REAL *ep = elements;
  register int i,j;

  for(j=0; j < ncols; j++)
    for(i=0; i < nrows; i++)
        *ep++ = 1./(i+j+1);

  return *this;
}

			// Create an orthonormal (2^n)*(no_cols) Haar
			// (sub)matrix, whose columns are Haar functions
			// If no_cols is 0, create the complete matrix
			// with 2^n columns
			// E.g., the complete Haar matrix of the second order
			// is
			// column 1: [ 1  1  1  1]/2
			// column 2: [ 1  1 -1 -1]/2
			// column 3: [ 1 -1  0  0]/sqrt(2)
			// column 4: [ 0  0  1 -1]/sqrt(2)
			// Matrix m is assumed to be zero originally
void _make_haar_matrix(Matrix& m)
{
  m.is_valid();
  assert( m.ncols <= m.nrows && m.ncols > 0 );
  register REAL * cp = m.elements;
     const REAL * m_end = m.elements + m.nelems;
  register int i;

  double norm_factor = 1/sqrt((double)m.nrows);

  for(i=0; i<m.nrows; i++)	// First column is always 1 (up to normali- 
    *cp++ = norm_factor;	// zation)

				// The other functions are kind of steps:
				// stretch of 1 followed by the equally
				// long stretch of -1
				// The functions can be grouped in families
				// according to their order (step size),
				// differing only in the location of the step
  int step_length = m.nrows/2;
  while( cp < m_end && step_length > 0 )
  {
    for(register int step_position=0; cp < m_end && step_position < m.nrows;
	step_position += 2*step_length, cp += m.nrows)
    {
      register REAL * ccp = cp + step_position;
      for(i=0; i<step_length; i++)
	*ccp++ = norm_factor;
      for(i=0; i<step_length; i++)
	*ccp++ = -norm_factor;
    }
    step_length /= 2;
    norm_factor *= sqrt(2.0);
  }
  assert( step_length != 0 || cp == m_end );
  assert( m.nrows != m.ncols || step_length == 0 );
}

haar_matrix::haar_matrix(const int order, const int no_cols)
    : LazyMatrix(1<<order,no_cols == 0 ? 1<<order : no_cols)
{
  assert(order > 0 && no_cols >= 0);
}

/*
 *------------------------------------------------------------------------
 * 			Matrix-scalar arithmetics
 * 	     Modify every element according to the operation
 */

				// For every element, do `elem OP value`
#define COMPUTED_VAL_ASSIGNMENT(OP,VALTYPE)				\
									\
Matrix& Matrix::operator OP (const VALTYPE val)				\
{									\
  is_valid();								\
  register REAL * ep = elements;					\
  while( ep < elements+nelems )						\
    *ep++ OP val;							\
									\
  return *this;								\
}									\

COMPUTED_VAL_ASSIGNMENT(=,REAL)
COMPUTED_VAL_ASSIGNMENT(+=,double)
COMPUTED_VAL_ASSIGNMENT(-=,double)
COMPUTED_VAL_ASSIGNMENT(*=,double)

#undef COMPUTED_VAL_ASSIGNMENT


				// is "element OP val" true for all
				// matrix elements?

#define COMPARISON_WITH_SCALAR(OP)					\
									\
bool Matrix::operator OP (const REAL val) const				\
{									\
  is_valid();								\
  register REAL * ep = elements;					\
  while( ep < elements + nelems )					\
    if( !(*ep++ OP val) )						\
      return false;							\
									\
  return true;								\
}									\


COMPARISON_WITH_SCALAR(==)
COMPARISON_WITH_SCALAR(!=)
COMPARISON_WITH_SCALAR(<)
COMPARISON_WITH_SCALAR(<=)
COMPARISON_WITH_SCALAR(>)
COMPARISON_WITH_SCALAR(>=)

#undef COMPARISON_WITH_SCALAR

/*
 *------------------------------------------------------------------------
 *		Apply algebraic functions to all the matrix elements
 */

				// Take an absolute value of a matrix
Matrix& Matrix::abs(void)
{
  is_valid();
  register REAL * ep;
  for(ep=elements; ep < elements+nelems; ep++)
    *ep = ::abs(*ep);

  return *this;
}

				// Square each element
Matrix& Matrix::sqr(void)
{
  is_valid();
  register REAL * ep;
  for(ep=elements; ep < elements+nelems; ep++)
    *ep = *ep * *ep;

  return *this;
}

				// Take a square root of all the elements
Matrix& Matrix::sqrt(void)
{
  is_valid();
  register REAL * ep;
  for(ep=elements; ep < elements+nelems; ep++)
    if( *ep >= 0 )
      *ep = ::sqrt(*ep);
    else
      info(),
      _error("(%d,%d)-th element, %g, is negative. Can't take the square root",
	     (ep-elements) % nrows + row_lwb,
	     (ep-elements) / nrows + col_lwb, *ep );

  return *this;
}

				// Apply a user-defined action to each matrix
				// element. The matrix is traversed in the
				// natural (that is, col by col) order
Matrix& Matrix::apply(ElementAction& action)
{
  is_valid();
  register REAL * ep = elements;
  for(action.j=col_lwb; action.j<col_lwb+ncols; action.j++)
    for(action.i=row_lwb; action.i<row_lwb+nrows; action.i++)
      action.operation(*ep++);
  assert( ep == elements+nelems );

  return *this;
}

/*
 *------------------------------------------------------------------------
 * 		   Matrix-Matrix element-wise operations
 */

				// Check to see if two matrices are identical
bool operator == (const Matrix& im1, const Matrix& im2)
{
  are_compatible(im1,im2);
  return (memcmp(im1.elements,im2.elements,im1.nelems*sizeof(REAL)) == 0);
}

				// Add the source to the target
Matrix& operator += (Matrix& target, const Matrix& source)
{
  are_compatible(target,source);

  register REAL * sp = source.elements;
  register REAL * tp = target.elements;
  for(; tp < target.elements+target.nelems;)
    *tp++ += *sp++;
  
  return target;
}
  
				// Subtract the source from the target
Matrix& operator -= (Matrix& target, const Matrix& source)
{
  are_compatible(target,source);
  register REAL * sp = source.elements;
  register REAL * tp = target.elements;
  for(; tp < target.elements+target.nelems;)
    *tp++ -= *sp++;
  
  return target;
}

				// Modified addition
				//	Target += scalar*Source
Matrix& add(Matrix& target, const double scalar,const Matrix& source)
{
  are_compatible(target,source);

  register REAL * sp = source.elements;
  register REAL * tp = target.elements;
  for(; tp < target.elements+target.nelems;)
    *tp++ += scalar * *sp++;
  
  return target;
}

				// Multiply target by the source
				// element-by-element
Matrix& element_mult(Matrix& target, const Matrix& source)
{
  are_compatible(target,source);
  register REAL * sp = source.elements;
  register REAL * tp = target.elements;
  for(; tp < target.elements+target.nelems;)
    *tp++ *= *sp++;
  
  return target;
}

				// Divide target by the source
				// element-by-element
Matrix& element_div(Matrix& target, const Matrix& source)
{
  are_compatible(target,source);
  register REAL * sp = source.elements;
  register REAL * tp = target.elements;
  for(; tp < target.elements+target.nelems;)
    *tp++ /= *sp++;
  
  return target;
}

/*
 *------------------------------------------------------------------------
 *			Compute matrix norms
 */

				// Row matrix norm
				// MAX{ SUM{ |M(i,j)|, over j}, over i}
				// The norm is induced by the infinity
				// vector norm
double Matrix::row_norm(void) const
{
  is_valid();
  register REAL * ep = elements;
  register double norm = 0;

  while(ep < elements+nrows)		// Scan the matrix row-after-row
  {
    register int j;
    register double sum = 0;
    for(j=0; j<ncols; j++,ep+=nrows)	// Scan a row to compute the sum
      sum += ::abs(*ep);
    ep -= nelems - 1;			// Point ep to the beg of the next row
    norm = ::max(norm,sum);
  }
  assert( ep == elements + nrows );

  return norm;
}

				// Column matrix norm
				// MAX{ SUM{ |M(i,j)|, over i}, over j}
				// The norm is induced by the 1.
				// vector norm
double Matrix::col_norm(void) const
{
  is_valid();
  register REAL * ep = elements;
  register double norm = 0;

  while(ep < elements+nelems)		// Scan the matrix col-after-col
  {					// (i.e. in the natural order of elems)
    register int i;
    register double sum = 0;
    for(i=0; i<nrows; i++)		// Scan a col to compute the sum
      sum += ::abs(*ep++);
    norm = ::max(norm,sum);
  }
  assert( ep == elements + nelems );

  return norm;
}


				// Square of the Euclidian norm
				// SUM{ m(i,j)^2 }
double Matrix::e2_norm(void) const
{
  is_valid();
  register REAL * ep;
  register double sum = 0;
  for(ep=elements; ep < elements+nelems;)
    sum += ::sqr(*ep++);

  return sum;
}

				// Square of the Euclidian norm of the
				// difference between two matrices
double e2_norm(const Matrix& m1, const Matrix& m2)
{
  are_compatible(m1,m2);
  register REAL * mp1 = m1.elements;
  register REAL * mp2 = m2.elements;
  register double sum = 0;
  for(; mp1 < m1.elements+m1.nelems;)
    sum += sqr(*mp1++ - *mp2++);

  return sum;
}

/*
 *------------------------------------------------------------------------
 * 			Some service operations
 */

void Matrix::info(void) const	// Print some information about the matrix
{
  is_valid();
  message("\nMatrix %d:%dx%d:%d '%s'",row_lwb,nrows+row_lwb-1,
	  col_lwb,ncols+col_lwb-1,name);
}

				// Print the Matrix as a table of elements
				// (zeros are printed as dots)
void Matrix::print(const char * title) const
{
  is_valid();
  message("\nMatrix %dx%d '%s' is as follows",nrows,ncols,title);

  const int cols_per_sheet = 6;
  register int sheet_counter;
  
  for(sheet_counter=1; sheet_counter<=ncols; sheet_counter +=cols_per_sheet)
  {
    message("\n\n     |");
    register int i,j;
    for(j=sheet_counter; j<sheet_counter+cols_per_sheet && j<=ncols; j++)
      message("   %6d  |",j+col_lwb-1);
    message("\n%s\n",_Minuses);
    for(i=1; i<=nrows; i++)
    {
      message("%4d |",i+row_lwb-1);
      for(j=sheet_counter; j<sheet_counter+cols_per_sheet && j<=ncols; j++)
	message("%11.4g  ",(*this)(i+row_lwb-1,j+col_lwb-1));
      message("\n");
    }
  }
  message("Done\n");
}

//#include <builtin.h>
void compare(			// Compare the two Matrixs
	const Matrix& matrix1,	// and print out the result of comparison
	const Matrix& matrix2,
	const char * title )
{
  register int i,j;

  are_compatible(matrix1,matrix2);

  message("\n\nComparison of two Matrices:\n\t%s",title);
  matrix1.info();
  matrix2.info();

  double norm1 = 0, norm2 = 0;		// Norm of the Matrixs
  double ndiff = 0;			// Norm of the difference
  int imax=0,jmax=0;			// For the elements that differ most
  REAL difmax = -1;
  register REAL *mp1 = matrix1.elements; // Matrix element pointers
  register REAL *mp2 = matrix2.elements;

  for(j=0; j < matrix1.ncols; j++)	// Due to the column-wise arrangement,
    for(i=0; i < matrix1.nrows; i++)	// the row index changes first
    {
      REAL mv1 = *mp1++;
      REAL mv2 = *mp2++;
      REAL diff = abs(mv1-mv2);

      if( diff > difmax )
      {
	difmax = diff;
	imax = i;
	jmax = j;
      }
      norm1 += abs(mv1);
      norm2 += abs(mv2);
      ndiff += abs(diff);
    }

  imax += matrix1.row_lwb, jmax += matrix1.col_lwb;
  message("\nMaximal discrepancy    \t\t%g",difmax);
  message("\n   occured at the point\t\t(%d,%d)",imax,jmax);
  const REAL mv1 = matrix1(imax,jmax);
  const REAL mv2 = matrix2(imax,jmax);
  message("\n Matrix 1 element is    \t\t%g",mv1);
  message("\n Matrix 2 element is    \t\t%g",mv2);
  message("\n Absolute error v2[i]-v1[i]\t\t%g",mv2-mv1);
  message("\n Relative error\t\t\t\t%g\n",
	 (mv2-mv1)/max(abs(mv2+mv1)/2,1e-7) );

  message("\n||Matrix 1||   \t\t\t%g",norm1);
  message("\n||Matrix 2||   \t\t\t%g",norm2);
  message("\n||Matrix1-Matrix2||\t\t\t\t%g",ndiff);
  message("\n||Matrix1-Matrix2||/sqrt(||Matrix1|| ||Matrix2||)\t%g\n\n",
	  ndiff/max( sqrt(norm1*norm2), 1e-7 )         );

}

/*
 *------------------------------------------------------------------------
 *			Service validation functions
 */

void verify_element_value(const Matrix& m,const REAL val)
{
  register imax = 0, jmax = 0;
  register double max_dev = 0;
  register int i,j;
  for(i=m.q_row_lwb(); i<=m.q_row_upb(); i++)
    for(j=m.q_col_lwb(); j<=m.q_col_upb(); j++)
    {
      register double dev = abs(m(i,j)-val);
      if( dev >= max_dev )
	imax = i, jmax = j, max_dev = dev;
    }

  if( max_dev == 0 )
    return;
  else if( max_dev < 1e-5 )
    message("Element (%d,%d) with value %g differs the most from what\n"
	    "was expected, %g, though the deviation %g is small\n",
	    imax,jmax,m(imax,jmax),val,max_dev);
  else
    _error("A significant difference from the expected value %g\n"
	   "encountered for element (%d,%d) with value %g",
	   val,imax,jmax,m(imax,jmax));
}

void verify_matrix_identity(const Matrix& m1, const Matrix& m2)
{
  register imax = 0, jmax = 0;
  register double max_dev = 0;
  register int i,j;
  are_compatible(m1,m2);
  for(i=m1.q_row_lwb(); i<=m1.q_row_upb(); i++)
    for(j=m1.q_col_lwb(); j<=m1.q_col_upb(); j++)
    {
      register double dev = abs(m1(i,j)-m2(i,j));
      if( dev >= max_dev )
	imax = i, jmax = j, max_dev = dev;
    }

  if( max_dev == 0 )
    return;
  if( max_dev < 1e-5 )
    message("Two (%d,%d) elements of matrices with values %g and %g\n"
	    "differ the most, though the deviation %g is small\n",
	    imax,jmax,m1(imax,jmax),m2(imax,jmax),max_dev);
  else
    _error("A significant difference between the matrices encountered\n"
	   "at (%d,%d) element, with values %g and %g",
	   imax,jmax,m1(imax,jmax),m2(imax,jmax));
}
