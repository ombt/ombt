// This may look like C code, but it is really -*- C++ -*-
/*
 ************************************************************************
 *
 *			  Linear Algebra Package
 *
 * The present package implements all the basic algorithms dealing
 * with vectors, matrices, matrix columns, etc.
 * Matrix is a basic object in the package; vectors, symmetric matrices,
 * etc. are considered matrices of a special type.
 *
 * Matrix elements are arranged in memory in a COLUMN-wise
 * fashion (in FORTRAN's spirit). In fact, it makes it very easy to
 * feed the matrices to FORTRAN procedures, which implement more
 * elaborate algorithms.
 *
 * Unless otherwise specified, matrix and vector indices always start
 * with 1, spanning up to the specified limit.
 *
 * The present package provides all facilities to completely AVOID returning
 * matrices. Use "Matrix A(Matrix::Transposed,B);" and other fancy constructors
 * as much as possible. If one really needs to return a matrix, return
 * a LazyMatrix object instead. The conversion is completely transparent
 * to the end user, e.g. "Matrix m = haar_matrix(5);" and _is_ efficient.
 *
 * $Id: LinAlg.h,v 3.1 1995/02/03 15:26:10 oleg Exp oleg $
 *
 ************************************************************************
 */

#ifndef __GNUC__
#pragma once
#endif
#ifndef _LinAlg_h
#define _LinAlg_h 1

#ifdef __GNUC__
#pragma interface
#endif

#include "myenv.h"
#include "std.h"
#include <math.h>
#include "builtin.h"
#include <minmax.h>

typedef float REAL;			// Scalar field of the Linear Vector
					// space

class Vector;				// Vector over the real domain

class MatrixRow;			// A row of the matrix
class MatrixColumn;			// A column of the matrix
class MatrixDiag;			// The diagonal of the matrix
class MatrixPivoting;			// For the determinant evaluation

				// A class to do a specific operation on 
				// every matrix element regardless of its
				// position
class ElementPrimAction
{
  friend class Matrix;
  friend class Vector;
  virtual void operation(REAL& element) = 0;
				// Those are'n implemented; but since they're
				// private, it forbids the assignement
//  ElementPrimAction(const PixelPrimAction&);
  ElementPrimAction& operator= (const ElementPrimAction&);
};

				// A class to do a specific operation on 
				// every matrix element as the matrix
				// is efficiently traversed
class ElementAction
{
  friend class Matrix;
  friend class Vector;
  virtual void operation(REAL& element) = 0;
protected:
  int i, j;			// position of the element being passed to
				// 'operator()'

private:			// Those are'n implemented; but since they're
				// private, it forbids the assignement
//  ElementAction(const PixelAction&);
  ElementAction& operator= (const ElementAction&);
};

				// Lazy matrix constructor
				// That is, a promise of a matrix rather than
				// a matrix itself. The promise is forced
				// by a Matrix constructor or assignment
				// operator, see below.
				// It's highly recommended a function never
				// return Matrix itself. Use LazyMatrix
				// instead
class LazyMatrix
{
  friend class Matrix;
  virtual void fill_in(Matrix& m) const = 0;

  LazyMatrix(const LazyMatrix&);	// Don't implement to forbid assignment
  LazyMatrix& operator = (const LazyMatrix&);
protected:
  int row_upb, row_lwb, col_upb, col_lwb;
public:
  LazyMatrix(const int nrows, const int ncols)	// Indices start with 1
    : row_upb(nrows), row_lwb(1), col_upb(ncols), col_lwb(1) {}
  LazyMatrix(const int _row_lwb, const int _row_upb,// Or with low:upper
	     const int _col_lwb, const int _col_upb)// boundary specif.
    : row_upb(_row_upb), row_lwb(_row_lwb),
      col_upb(_col_upb), col_lwb(_col_lwb) {}
};

class MinMax
{
  REAL min_val, max_val;
public:
  MinMax(REAL _min_val, REAL _max_val)
    : min_val(_min_val), max_val(_max_val) {}
  REAL min(void) const		{ return min_val; }
  REAL max(void) const		{ return max_val; }
  double ratio(void) const	{ return max_val/min_val; }
};

/*
 *------------------------------------------------------------------------
 *			Basic class of matrix
 */

class Matrix			// General type matrix
{
  friend class Vector;
  friend class MatrixRow;
  friend class MatrixColumn;
  friend class MatrixDiag;
  friend class MatrixPivoting;

private:			// Private part
  int valid_code;			// Validation code
  enum { MATRIX_val_code = 575767 };	// Matrix validation code value

protected:			// May be used in derived classes
  int nrows;				// No. of rows
  int ncols;				// No. of columns
  int row_lwb;				// Lower bound of the row index
  int col_lwb;				// Lower bound of the col index
  char * name;				// Name for the matrix
  int nelems;				// Total no of elements, nrows*ncols
  REAL ** index;			// index[i] = &matrix(0,i) (col index)
  REAL * elements;			// Elements themselves

  void allocate(const int nrows, const int ncols,
			const int row_lwb = 1, const int col_lwb = 1);

				// Elementary constructors
  void _transpose(const Matrix& m);	// Allocate new matrix and set it to m'
  void _invert(const Matrix& m);	// Allocate new matrix and set it to minv
  void _AmultB(const Matrix& A, const Matrix& B);	// Matrix multipli-
  void _AtmultB(const Matrix& A, const Matrix& B);	// cators

  friend void _make_haar_matrix(Matrix& m);

public:			// Public interface

				// Constructors and destructors
					// Make a blank matrix
  Matrix(const int nrows, const int ncols);	// Indices start with 1
  Matrix(const int row_lwb, const int row_upb,	// Or with low:upper
	 const int col_lwb, const int col_upb);	// boundary specif.

  Matrix(const Matrix&  another);	// A real copy constructor, expensive

					// Construct a matrix applying a spec
					// operation to the prototype
					// Example: Matrix A(10,12);
					// Matrix B(Matrix::Transposed,A);
  enum MATRIX_CREATORS_1op { Zero, Unit, Transposed, Inverted };
  Matrix(const MATRIX_CREATORS_1op op, const Matrix& prototype);

					// Construct a matrix applying a spec
					// operation to two prototypes
					// Example: Matrix A(10,12), B(12,5);
					// Matrix C(A,Matrix::Mult,B);
  enum MATRIX_CREATORS_2op { Mult, 		// A*B
			     TransposeMult, 	// A'*B
			     InvMult, 		// A^(-1)*B
			     AtBA }; 		// A'*B*A
  Matrix(const Matrix& A, const MATRIX_CREATORS_2op op, const Matrix& B);
  Matrix(const Vector& x, const Vector& y);	// x'*y (diad) matrix
					// Construct a matrix applying an
					// arbitrary action to the prototype
//  Matrix(ElementAction& action, const Matrix& prototype);
  Matrix(const LazyMatrix& lazy_constructor);//Make a matrix using given recipe
  Matrix(const char * file_name);	// Read the matrix from the file
					// (not yet implemented!)

  virtual ~Matrix();			// Destructor

  void set_name(const char * name);	// Set a new matrix name

					// Erase the old matrix and create a
					// new one according to new boundaries
  void resize_to(const int nrows, const int ncols);	// Indexation @ 1
  void resize_to(const int row_lwb, const int row_upb,	// Or with low:upper
		 const int col_lwb, const int col_upb);	// boundary specif.
  void resize_to(const Matrix& m);			// Like other matrix m


  void is_valid() const
  { assure(valid_code == MATRIX_val_code,"Invalid matrix"); }

				// Status inquires
  int q_row_lwb() const			{ return row_lwb; }
  int q_row_upb() const			{ return nrows+row_lwb-1; }
  int q_nrows()	const			{ return nrows; }
  int q_col_lwb() const			{ return col_lwb; }
  int q_col_upb() const			{ return ncols+col_lwb-1; }
  int q_ncols()	const			{ return ncols; }

  int q_no_elems() const		{ return nelems; }

  const char * q_name() const		{ return name; }

				// Individual element manipulations
  inline const REAL& operator () (const int rown, const int coln) const;
  REAL& operator () (const int rown, const int coln)
	{ return (REAL&)((*(const Matrix *)this)(rown,coln)); }


			// Element-wise matrix operations

				// Matrix-scalar arithmetics
				// Modify every element of the
				// Matrix according to the operation
  Matrix& operator =   (const REAL val);	// Assignment to all the elems
  Matrix& operator -=  (const double val);	// Add to elements
  Matrix& operator +=  (const double val);	// Take from elements
  Matrix& operator *=  (const double val);	// Multiply elements by a val

				// Comparisons
				// Find out if the predicate
				// "element op val" is true for ALL matrix
				// elements
  bool	  operator ==  (const REAL val) const;	// ? all elems == val
  bool	  operator !=  (const REAL val) const;	// ? all elems != val
  bool	  operator <   (const REAL val) const;	// ? all elems <  val
  bool	  operator <=  (const REAL val) const;	// ? all elems <= val
  bool	  operator >   (const REAL val) const;	// ? all elems >  val
  bool	  operator >=  (const REAL val) const;	// ? all elems >= val

				// Other element-wise matrix operations
  Matrix& clear(void);			// Clear the matrix (fill out with 0)
  Matrix& abs(void);			// Take an absolute value of a matrix
  Matrix& sqr(void);			// Square each element
  Matrix& sqrt(void);			// Take a square root

  Matrix& apply(ElementPrimAction& action);	// Apply a user-defined action
  Matrix& apply(ElementAction& action);		// to each matrix element

					// Invert the matrix returning the
					// determinant if desired
					// determinant = 0 if the matrix is
					// singular
					// If determ_ptr=0 and the matrix *is*
					// singular, throw up
  Matrix& invert(double * determ_ptr = 0);

				// Element-wise operations on two matrices
  inline Matrix& operator = (const Matrix& source);	// Assignment
  Matrix& operator = (const LazyMatrix& source);// Force the promise of a matrix
 
					// Arithmetics
  friend Matrix& operator += (Matrix& target, const Matrix& source);
  friend Matrix& operator -= (Matrix& target, const Matrix& source);
  friend Matrix& add(Matrix& target, const double scalar,const Matrix& source);
  friend Matrix& element_mult(Matrix& target, const Matrix& source);
  friend Matrix& element_div(Matrix& target, const Matrix& source);

					// Comparisons
  friend bool  operator == (const Matrix& im1, const Matrix& im2);
  friend void compare(const Matrix& im1, const Matrix& im2, 
		      const char * title);
  friend inline void are_compatible(const Matrix& im1, const Matrix& im2);


				// True matrix operations
				// (on a matrix as a whole)  
  Matrix& operator *= (const Matrix& source);	// Inplace multiplication
						// possible only for square src

  Matrix& operator *= (const MatrixDiag& diag);	// Multiply by the diagonal of
						// another matrix
  
  void mult(const Matrix& A, const Matrix& B);  // Compute A*B

				// Compute matrix norms
  double row_norm(void) const;		// MAX{ SUM{ |M(i,j)|, over j}, over i}
  double norm_inf(void) const		// Alias, shows the norm is induced
         { return row_norm(); }		// 	by the vector inf-norm
  double col_norm(void) const;		// MAX{ SUM{ |M(i,j)|, over i}, over j}
  double norm_1(void) const		// Alias, shows the norm is induced
         { return col_norm(); }		// 	by the vector 1-norm
  double e2_norm(void) const;		// SUM{ m(i,j)^2 }, Note it's square
					// of the Frobenius rather than 2. norm

  friend double e2_norm(const Matrix& m1, const Matrix& m2);
					// e2_norm(m1-m2)

  double determinant(void) const;	// Matrix must be a square one

  double asymmetry_index(void) const;	// How far is the matrix from being
					// symmetrical (0 means complete symm)
					// (not yet implemented)

				// Make matrices of a special kind
  Matrix& unit_matrix(void);		// Matrix needn't be a square
  Matrix& hilbert_matrix(void);		// Hilb[i,j] = 1/(i+j-1); i,j=1..max

				// I/O: write, read, print 
  					// Write to a file
					// "| command name" is OK as a file
					// name
  void write(const char * file_name,const char * title = "") const;
  void info(void) const;		// Print the info about the Matrix
  void print(const char * title) const;	// Print the Matrix as a table

};

				// Create an orthogonal (2^n)*(no_cols) Haar
				// (sub)matrix, whose columns are Haar
				// functions
				// If no_cols is 0, create the complete matrix
				// with 2^n columns
class haar_matrix : public LazyMatrix
{
  void fill_in(Matrix& m) const		{ _make_haar_matrix(m); }
public:
  haar_matrix(const int n, const int no_cols=0);
};


/*
 *------------------------------------------------------------------------
 *			Inline Matrix operations
 */

inline Matrix::Matrix(const int no_rows, const int no_cols)
{
  allocate(no_rows,no_cols);
}

inline Matrix::Matrix(const int row_lwb, const int row_upb,
		      const int col_lwb, const int col_upb)
{
  allocate(row_upb-row_lwb+1,col_upb-col_lwb+1,row_lwb,col_lwb);
}

inline Matrix::Matrix(const LazyMatrix& lazy_constructor)
{
  allocate(lazy_constructor.row_upb-lazy_constructor.row_lwb+1,
	   lazy_constructor.col_upb-lazy_constructor.col_lwb+1,
	   lazy_constructor.row_lwb,lazy_constructor.col_lwb);
  lazy_constructor.fill_in(*this);
}


				// Force the promise of a matrix
				// That is, apply a lazy_constructor
				// to the current matrix
inline Matrix& Matrix::operator = (const LazyMatrix& lazy_constructor)
{
  is_valid();
  if( lazy_constructor.row_upb != q_row_upb() ||
      lazy_constructor.col_upb != q_col_upb() ||
      lazy_constructor.row_lwb != q_row_lwb() ||
      lazy_constructor.col_lwb != q_col_lwb() )
    info(),
    _error("The matrix above is incompatible with the assigned "
    	   "Lazy matrix");
      
  lazy_constructor.fill_in(*this);
  return *this;
}

					// Copy constructor, expensive: use
					// sparingly
inline Matrix::Matrix(const Matrix& another)
{
  another.is_valid();
  allocate(another.nrows,another.ncols,another.row_lwb,another.col_lwb);
  *this = another;
}

				// Resize the matrix to accomodate to a pattern
inline void Matrix::resize_to(const Matrix& m)
{
  resize_to(m.q_row_lwb(),m.q_row_upb(),m.q_col_lwb(),m.q_col_upb());
}

inline const REAL& Matrix::operator () (const int rown, const int coln) const
{
  is_valid();
  register int arown = rown-row_lwb;		// Effective indices
  register int acoln = coln-col_lwb;

  if( arown >= nrows || arown < 0 )
    _error("Row index %d is out of Matrix boundaries [%d,%d]",
	   rown,row_lwb,nrows+row_lwb-1);
  if( acoln >= ncols || acoln < 0 )
    _error("Col index %d is out of Matrix boundaries [%d,%d]",
	   coln,col_lwb,ncols+col_lwb-1);
  
  return (index[acoln])[arown];
}

inline Matrix& Matrix::clear(void)	// Clean the Matrix
{
  is_valid();
  memset(elements,0,nelems*sizeof(REAL));
  return *this;
}

inline void are_compatible(const Matrix& im1, const Matrix& im2)
{
  im1.is_valid();
  im2.is_valid();
  
  if( im1.nrows != im2.nrows || im1.ncols != im2.ncols ||
      im1.row_lwb != im2.row_lwb || im1.col_lwb != im2.col_lwb )
    im1.info(), im2.info(), _error("The matrices above are incompatible");
}

				// Assignment
inline Matrix& Matrix::operator = (const Matrix& source)
{
  are_compatible(*this,source);
  memcpy(elements,source.elements,nelems*sizeof(REAL));
  return *this;
}

				// Apply a user-defined action to each matrix
				// element
inline Matrix& Matrix::apply(ElementPrimAction& action)
{
  is_valid();
  for(register REAL * ep=elements; ep < elements+nelems; ep++)
    action.operation(*ep);

  return *this;
}

/*
 *------------------------------------------------------------------------
 *		Friend classes - MatrixRow, MatrixCol, MatrixDiag
 */

class MatrixColumn		// Special representation of a Col of the
{				// matrix
  friend class Matrix;
  friend class Vector;

  const Matrix& matrix;			// The matrix i am a column of
  REAL * ptr;				// Pointer to the a[0,i]

public:					// Take a col of the matrix
  MatrixColumn(const Matrix& matrix, const int col);

					// Assign a value to all the elements
					// of the Matrix Col
  void operator = (const REAL val);
  					// Modify the elements in the col
  void operator +=  (const double val);
  void operator *=  (const double val);

  void operator = (const Vector& vec);	// Assign a vector to a matrix col

  					// Individual element manipulations
  inline const REAL& operator () (const int i) const;
  inline REAL& operator () (const int i)
	{ return (REAL&)((*(const MatrixColumn *)this)(i)); }
};

				// Construct the MatrixColumn
inline MatrixColumn::MatrixColumn(const Matrix& _matrix, const int col)
	: matrix(_matrix)
{
  matrix.is_valid();

  register int colind = col - matrix.col_lwb;

  if( colind >= matrix.ncols || colind < 0 )
    matrix.info(),
    _error("Column #%d is not within the above matrix",col);

  MatrixColumn::ptr = &(matrix.index[colind][0]);
}

				// Access the i-th element of the column
inline const REAL& MatrixColumn::operator () (const int i) const
{
  matrix.is_valid();
  register int arown = i-matrix.row_lwb;		// Effective indices

  if( arown >= matrix.nrows || arown < 0 )
    _error("MatrixColumn index %d is out of column boundaries [%d,%d]",
	   i,matrix.row_lwb,matrix.nrows+matrix.row_lwb-1);
  return ptr[arown];
}

class MatrixRow			// Special representation of a Row of the
{				// matrix
  friend class Matrix;
  friend class Vector;

  const Matrix& matrix;			// The matrix i am a row of
  const int inc;			// if ptr=@a[row,i], then
					//    ptr+inc = @a[row,i+1]
					// Since elements of a[] are stored
					// col after col, inc = nrows
  REAL * ptr;				// Pointer to the a[row,0]

public:				// Take a row of the matrix
  MatrixRow(const Matrix& matrix, const int row);

					// Assign a value to all the elements
					// of the Matrix Row   
  void operator = (const REAL val);
  					// Modify the elements in the row
  void operator +=  (const double val);
  void operator *=  (const double val);

  void operator = (const Vector& vec);	// Assign a vector to a matrix row

  					// Individual element manipulations
  inline const REAL& operator () (const int i) const;
  inline REAL& operator () (const int i)
	{ return (REAL&)((*(const MatrixRow *)this)(i)); }
};

				// Construct the MatrixRow
inline MatrixRow::MatrixRow(const Matrix& _matrix, const int row)
  : matrix(_matrix), inc(_matrix.nrows)
{
  matrix.is_valid();

  register int rowind = row - matrix.row_lwb;

  if( rowind >= matrix.nrows || rowind < 0 )
    matrix.info(),
    _error("Row #%d is not within the above matrix",row);

  MatrixRow::ptr = &(matrix.index[0][rowind]);
}

				// Get hold of the i-th row's element
inline const REAL& MatrixRow::operator () (const int i) const
{
  matrix.is_valid();
  register int acoln = i-matrix.col_lwb;		// Effective index

  if( acoln >= matrix.ncols || acoln < 0 )
    _error("MatrixRow index %d is out of row boundaries [%d,%d]",
	   i,matrix.col_lwb,matrix.ncols+matrix.col_lwb-1);
  return matrix.index[acoln][ptr-matrix.elements];
}



class MatrixDiag		// Special representation of the diagonal of a
{				// matrix (for easy manipulation)
  friend class Matrix;
  friend class Vector;

  const Matrix& matrix;			// The matrix i am the diagonal of
  const int inc;			// if ptr=@a[i,i], then
					//    ptr+inc = @a[i+1,i+1]
					// Since elements of a[] are stored
					// col after col, inc = nrows+1
  const int ndiag;			// No of diag elems, min(nrows,ncols)
  REAL * ptr;				// Pointer to the a[0,0]

public:					// Take a diag of the matrix
  MatrixDiag(const Matrix& matrix);
					// Assign a value to all the elements
					// of the Matrix Diag
  void operator = (const REAL val);
  					// Modify the elements in the diag
  void operator +=  (const double val);
  void operator *=  (const double val);

  void operator = (const Vector & vec);	// Assign a vector to a matrix diag

  					// Individual element manipulations
  inline const REAL& operator () (const int i) const;
  inline REAL& operator () (const int i)
	{ return (REAL&)((*(const MatrixDiag *)this)(i)); }
  int q_ndiags(void) const		{ return ndiag; }
};

				// Construct the MatrixDiag
inline MatrixDiag::MatrixDiag(const Matrix& _matrix)
: matrix(_matrix), inc(_matrix.nrows+1), 
  ndiag(::min(_matrix.nrows,_matrix.ncols))
{
  matrix.is_valid();
  MatrixDiag::ptr = &(matrix.elements[0]);
}

				// Get hold of the i-th diag element
				// (indexing always starts at 1, regardless
				// of matrix' col_lwb and row_lwb)
inline const REAL& MatrixDiag::operator () (const int i) const
{
  matrix.is_valid();
  if( i > ndiag || i < 1 )
    _error("MatrixDiag index %d is out of diag boundaries [1,%d]",
	   i,ndiag);
  return matrix.index[i-1][i-1];
}

/*
 *------------------------------------------------------------------------
 *		   Vector as a n*1 matrix (that is, a col-matrix)
 */

class Vector : public Matrix
{
  friend class MatrixRow;
  friend class MatrixColumn;
  friend class MatrixDiag;

public:
  Vector(const int n);		// Specify a blank vector for a given
					// dimension, with indexation at 1
  Vector(const int lwb, const int upb); // Specify a general lwb:upb vector
//  Vector(const Vector& another);	// a copy constructor (to be inferred)

					// Make a vector and assign init vals
  Vector(const int lwb, const int upb,  // lower and upper bounds
	 double iv1, ...    		// DOUBLE numbers. The last arg of
	 ); 				// the list must be string "END"
					// E.g: Vector f(1,2,0.0,1.5,"END");

  Vector(const LazyMatrix& lazy_constructor);//Make a vector using given recipe

				// Resize the vector (keeping as many old
				// elements as possible), expand by zeros
  void resize_to(const int n);			// Indexation @ 1
  void resize_to(const int lwb, const int upb); 	// lwb:upb specif
  void resize_to(const Vector& v);			// like other vector

  inline REAL & operator () (const int index) const;
  inline REAL& operator () (const int index)
	{ return (REAL&)((*(const Vector *)this)(index)); }

				// Listed below are specific vector operations
				// (unlike n*1 matrices)

					// Status inquires
  int q_lwb(void) const		{ return row_lwb; }
  int q_upb(void) const		{ return nrows + row_lwb - 1; }

					// Compute the scalar product
  friend double operator * (const Vector& v1, const Vector& v2);

					// "Inplace" multiplication
					// target = A*target
					// A needn't be a square one (the
					// target will be resized to fit)
  Vector& operator *= (const Matrix& A);

  Vector& operator *=  (const double val)	// Multiply elements by a val
	{ Matrix::operator *=(val); return *this; }

					// Vector norms
  double norm_1(void) const;   			// SUM{ |v[i]| }
  double norm_2_sqr(void) const;	       	// SUM{ v[i]^2 }
  double norm_inf(void) const;			// MAX{ |v[i]| }

  Vector& operator = (const Vector& v)
  	{ Matrix::operator =(v); return *this; }
  Vector& operator = (const REAL val)	// Assign to all elems of a vector
  	{ Matrix::operator =(val); return *this; }
  Vector& operator = (const LazyMatrix& source)// Force the promise of a vector
  	{ Matrix::operator =(source); return *this; }

  Vector& operator = (const MatrixRow& mr);
  Vector& operator = (const MatrixColumn& mc);
  Vector& operator = (const MatrixDiag& md);
};
  
				// Create a blank vector of a given size
inline Vector::Vector(const int n) : Matrix(n,1)
{}

				// Create a general blank vector
inline Vector::Vector(const int lwb, const int upb) : Matrix(lwb,upb,1,1)
{}

				// Resize the vector for a specified number
				// of elements, trying to keep intact as many
				// elements of the old vector as possible.
				// If the vector is expanded, the new elements
				// will be zeroes
inline void Vector::resize_to(const int n)	{ Vector::resize_to(1,n); }

inline void Vector::resize_to(const Vector& v)
{
  Vector::resize_to(v.q_lwb(),v.q_upb());
}

				// Get access to a vector element
inline REAL& Vector::operator () (const int ind) const
{
  is_valid();
  register int aind = ind - row_lwb;
  if( aind >= nelems || aind < 0 )
    _error("Requested element %d is out of Vector boundaries [%d,%d]",
	   ind,row_lwb,nrows+row_lwb-1);
  
  return elements[aind];
}

				// Make a vector follow a recipe
inline Vector::Vector(const LazyMatrix& lazy_constructor)
  : Matrix(lazy_constructor)
{
  assure(ncols == 1 && col_lwb == 1,
         "cannot make a vector from a promise of a full matrix");
}

				// Service functions (useful in the
				// verification code). They print some detail
				// info if the validation condition fails
void verify_element_value(const Matrix& m,const REAL val);
void verify_matrix_identity(const Matrix& m1, const Matrix& m2);

#endif
