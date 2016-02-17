// This may look like C code, but it is really -*- C++ -*-
/*
 ************************************************************************
 *
 *		  Verify Primitive Operations on Matrices
 *	(creation, special cases and element-by-element operations)
 *
 *
 * $Id: vmatrix.cc,v 3.1 1995/02/03 15:24:27 oleg Exp oleg $
 *
 ************************************************************************
 */

#include "LinAlg.h"
#include "builtin.h"
#include <math.h>
#include <iostream.h>
#include <float.h>

/*
 *------------------------------------------------------------------------
 *	   Test allocation functions and compatibility check
 */

static void test_allocation(void)
{

  cout << "\n\n---> Test allocation and compatibility check\n";

  Matrix m1(4,20);	m1.set_name("Matrix m1");
  Matrix m2(1,4,1,20);	m2.set_name("Matrix m2");
  Matrix m3(1,4,0,19);	m3.set_name("Matrix m3");
  Matrix m4(m1);	m4.set_name("Matrix m4");
  cout << "The following matrices have been allocated\n";
  m1.info(), m2.info(), m3.info(), m4.info();

  cout << "\nStatus information reported for matrix m3:\n";
  cout << "  Row lower bound ... " << m3.q_row_lwb() << "\n";
  cout << "  Row upper bound ... " << m3.q_row_upb() << "\n";
  cout << "  Col lower bound ... " << m3.q_col_lwb() << "\n";
  cout << "  Col upper bound ... " << m3.q_col_upb() << "\n";
  cout << "  No. rows ..........." << m3.q_nrows() << "\n";
  cout << "  No. cols ..........." << m3.q_ncols() << "\n";
  cout << "  No. of elements ...." << m3.q_no_elems() << "\n";
  cout << "  Name " << m3.q_name() << "\n";

  cout << "\nCheck matrices 1 & 2 for compatibility\n";
  are_compatible(m1,m2);

  cout << "Check matrices 1 & 4 for compatibility\n";
  are_compatible(m1,m4);

#if 0
  cout << "Check matrices 1 & 3 for compatibility\n";
  are_compatible(m1,m3);
#endif

  cout << "m2 has to be compatible with m3 after resizing to m3\n";
  m2.resize_to(m3);
  are_compatible(m2,m3);

  Matrix m5(m1.q_nrows()+1,m1.q_ncols()+5); m5.set_name("Matrix m5");
  cout << "m1 has to be compatible with m5 after resizing to m5\n";
  m5.info();
  m1.resize_to(m5.q_nrows(),m5.q_ncols());
  are_compatible(m1,m5);

}

/*
 *------------------------------------------------------------------------
 *		     Test uniform element operations
 */

static void test_element_op(const int rsize, const int csize)
{
  const double pattern = 8.625;
  register int i,j;

  cout << "\n\n---> Test operations that treat each element uniformly\n";

  Matrix m(-1,rsize-2,1,csize);

  cout << "Writing zeros to m...\n";
  for(i=m.q_row_lwb(); i<=m.q_row_upb(); i++)
    for(j=m.q_col_lwb(); j<=m.q_col_upb(); j++)
      m(i,j) = 0;
  verify_element_value(m,0);

  cout << "Creating zero m1 ...\n";
  Matrix m1(Matrix::Zero,m);
  verify_element_value(m1,0);

  cout << "Comparing m1 with 0 ...\n";
  assure(m1 == 0, "m1 is not zero!");
  assure(!(m1 != 0), "m1 is not zero!");

  cout << "Writing a pattern " << pattern << " by assigning to m(i,j)...\n";
  for(i=m.q_row_lwb(); i<=m.q_row_upb(); i++)
    for(j=m.q_col_lwb(); j<=m.q_col_upb(); j++)
      m(i,j) = pattern;
  verify_element_value(m,pattern);

  cout << "Writing the pattern by assigning to m1 as a whole ...\n";
  m1 = pattern;
  verify_element_value(m1,pattern);

  cout << "Comparing m and m1 ...\n";
  assure(m == m1, "m and m1 are unexpectedly different!");
  cout << "Comparing (m=0) and m1 ...\n";
  assert(!(m.clear() == m1));

  cout << "Clearing m1 ...\n";
  m1.clear();
  verify_element_value(m1,0);

  cout << "\nClear m and add the pattern\n";
  m.clear();
  m += pattern;
  verify_element_value(m,pattern);
  cout << "   add the doubled pattern with the negative sign\n";
  m += -2*pattern;
  verify_element_value(m,-pattern);
  cout << "   subtract the trippled pattern with the negative sign\n";
  m -= -3*pattern;
  verify_element_value(m,2*pattern);

  cout << "\nVerify comparison operations when all elems are the same\n";
  m = pattern;
  assert( m == pattern && !(m != pattern) );
  assert( m > 0 && m >= pattern && m <= pattern );
  assert( m > -pattern && m >= -pattern );
  assert( m <= pattern && !(m < pattern) );
  m -= 2*pattern;
  assert( m  < -pattern/2 && m <= -pattern/2 );
  assert( m  >= -pattern && !(m > -pattern) );

  cout << "\nVerify comparison operations when not all elems are the same\n";
  m = pattern; m(m.q_row_upb(),m.q_col_upb()) = pattern-1;
  assert( !(m == pattern) && !(m != pattern) );
  assert( m != 0 ); 		// none of elements are 0
  assert( !(m >= pattern) && m <= pattern && !(m<pattern) );
  assert( !(m <= pattern-1) && m >= pattern-1 && !(m>pattern-1) );

  cout << "\nAssign 2*pattern to m by repeating additions\n";
  m = 0; m += pattern; m += pattern;
  cout << "Assign 2*pattern to m1 by multiplying by two \n";
  m1 = pattern; m1 *= 2;
  verify_element_value(m1,2*pattern);
  assert( m == m1 );
  cout << "Multiply m1 by one half returning it to the 1*pattern\n";
  m1 *= 1/2.;
  verify_element_value(m1,pattern);

  cout << "\nAssign -pattern to m and m1\n";
  m.clear(); m -= pattern; m1 = -pattern;
  verify_element_value(m,-pattern);
  assert( m == m1 );
  cout << "m = sqrt(sqr(m)); m1 = abs(m1); Now m and m1 have to be the same\n";
  m.sqr();
  verify_element_value(m,pattern*pattern);
  m.sqrt();
  verify_element_value(m,pattern);
  m1.abs();
  verify_element_value(m1,pattern);
  assert( m == m1 );

  cout << "\nCheck out to see that sin^2(x) + cos^2(x) = 1\n";
  class FillMatrix : public ElementAction
  {
    int no_elems, no_cols;
    void operation(REAL& element)
		{ element = 4*M_PI/no_elems * (i*no_cols+j); }
    public: FillMatrix(const Matrix& m) :
			no_elems(m.q_no_elems()), no_cols(m.q_ncols()) {}
  };
  m.apply(FillMatrix(m));
  m1 = m;
  class ApplyFunction : public ElementPrimAction
  {
    double (*func)(const double x);
    void operation(REAL& element) { element = func(element); }
    public: ApplyFunction(double (*_func)(const double x)) : func(_func) {}
  };
  m.apply(ApplyFunction(sin));
  m1.apply(ApplyFunction(cos));
  m.sqr();
  m1.sqr();
  m += m1;
  verify_element_value(m,1);

  cout << "\nDone\n\n";
}

/*
 *------------------------------------------------------------------------
 *  		Test binary matrix element-by-element operations
 */

static void test_binary_ebe_op(const int rsize, const int csize)
{
  const double pattern = 4.25;
  register int i,j;

  cout << "\n---> Test Binary Matrix element-by-element operations\n";

  Matrix m(2,rsize+1,0,csize-1);
  Matrix m1(Matrix::Zero,m);
  Matrix mp(Matrix::Zero,m);

  for(i=mp.q_row_lwb(); i<=mp.q_row_upb(); i++)
    for(j=mp.q_col_lwb(); j<=mp.q_col_upb(); j++)
      mp(i,j) = (i-m.q_nrows()/2.)*j*pattern;
  

  cout << "\nVerify assignment of a matrix to the matrix\n";
  m = pattern;
  m1.clear();
  m1 = m;
  verify_element_value(m1,pattern);
  assert( m1 == m );

  cout << "\nAdding the matrix to itself, uniform pattern " << pattern << "\n";
  m.clear(); m = pattern;
  m1 = m; m1 += m1;
  verify_element_value(m1,2*pattern);
  cout << "  subtracting two matrices ...\n";
  m1 -= m;
  verify_element_value(m1,pattern);
  cout << "  subtracting the matrix from itself\n";
  m1 -= m1;
  verify_element_value(m1,0);
  cout << "  adding two matrices together\n";
  m1 += m;
  verify_element_value(m1,pattern);

  cout << "\nArithmetic operations on matrices with not the same elements\n";
  cout << "   adding mp to the zero matrix...\n";
  m.clear(); m += mp;
  verify_matrix_identity(m,mp);
  m1 = m;
  cout << "   making m = 3*mp and m1 = 3*mp, via add() and succesive mult\n";
  add(m,2,mp);
  m1 += m1; m1 += mp;
  verify_matrix_identity(m,m1);
  cout << "   clear both m and m1, by subtracting from itself and via add()\n";
  m1 -= m1;
  add(m,-3,mp);
  verify_matrix_identity(m,m1);

  cout << "\nTesting element-by-element multiplications and divisions\n";
  cout << "   squaring each element with sqr() and via multiplication\n";
  m = mp; m1 = mp;
  m.sqr();
  element_mult(m1,m1);
  verify_matrix_identity(m,m1);
  cout << "   compare (m = pattern^2)/pattern with pattern\n";
  m = pattern; m1 = pattern;
  m.sqr();
  element_div(m,m1);
  verify_element_value(m,pattern);
  compare(m1,m,"Original vector and vector after squaring and dividing");

  cout << "\nDone\n";
}

/*
 *------------------------------------------------------------------------
 *		       Verify matrix transposition
 */

static void test_transposition(const int msize)
{
  cout << "\n---> Verify matrix transpose\n"
          "for matrices of a characteristic size " << msize << endl;

  {
    cout << "\nCheck to see that a square UnitMatrix' stays the same";
    Matrix m(msize,msize);
    m.unit_matrix();
    Matrix mt(Matrix::Transposed,m);
    assert( m == mt );
  }
  
  {
    cout << "\nTest a non-square UnitMatrix";
    Matrix m(msize,msize+1);
    m.unit_matrix();
    Matrix mt(Matrix::Transposed,m);
    assert( m.q_nrows() == mt.q_ncols() && m.q_ncols() == mt.q_nrows() );
    register int i,j;
    for(i=m.q_row_lwb(); i<=min(m.q_row_upb(),m.q_col_upb()); i++)
      for(j=m.q_col_lwb(); j<=min(m.q_row_upb(),m.q_col_upb()); j++)
	assert( m(i,j) == mt(i,j) );
  }

  {
    cout << "\nCheck to see that a symmetric (Hilbert)Matrix' stays the same";
    Matrix m(msize,msize);
    m.hilbert_matrix();
    Matrix mt(Matrix::Transposed,m);
    assert( m == mt );
  }

  {
    cout << "\nCheck transposing a non-symmetric matrix";
    Matrix m(msize+1,msize);
    m.hilbert_matrix();
    m(1,2) = M_PI;
    Matrix mt(Matrix::Transposed,m);
    assert( m.q_nrows() == mt.q_ncols() && m.q_ncols() == mt.q_nrows() );
    assert( mt(2,1) == (REAL)M_PI && mt(1,2) != (REAL)M_PI );

    cout << "\nCheck double transposing a non-symmetric matrix";
    Matrix mtt(Matrix::Transposed,mt);
    assert( m == mtt );
  }

  cout << "\nDone\n";
}

/*
 *------------------------------------------------------------------------
 *			Test special matrix creation
 */

static void test_special_creation(const int dim)
{
  cout << "\n---> Check creating some special matrices of dimension " <<
    dim << "\n\n";

  {
    cout << "test creating Hilbert matrices" << endl;
    Matrix m(dim+1,dim);
    Matrix m1(Matrix::Zero,m);
    m.hilbert_matrix();
    assert( !(m == m1) );
    assert( m != 0 );
    class MakeHilbert : public ElementAction
    {
      void operation(REAL& element) { element = 1./(i+j-1); }
      public: MakeHilbert(void) {}
    };
    m1.apply(MakeHilbert());
    assert( m1 != 0 );
    assert( m == m1 );
  }

  {
    cout << "test creating zero matrix and copy constructor" << endl;
    Matrix m(dim,dim+1);
    m.hilbert_matrix();
    assert( m != 0 );
    Matrix m1(m);			// Applying the copy constructor
    assert( m1 == m );
    Matrix m2(Matrix::Zero,m);
    assert( m2 == 0 );
    assert( m != 0 );
  }

  {
    cout << "test creating unit matrices" << endl;
    class TestUnit : public ElementAction
    {
      int is_unit;
      void operation(REAL& element)
      { if( is_unit ) is_unit = i==j ? element == 1.0 : element == 0; }
      public: TestUnit(void) : is_unit(0==0) {}
      int is_indeed_unit(void) const 		{ return is_unit; }
    };
    Matrix m(dim,dim);
    {
      TestUnit test_unit;
      m.apply(test_unit);
      assert( !test_unit.is_indeed_unit() );
    }
    m.unit_matrix();
    {
      TestUnit test_unit;
      m.apply(test_unit);
      assert( test_unit.is_indeed_unit() );
    }
    m.resize_to(dim-1,dim);
    Matrix m2(Matrix::Unit,m);
    {
      TestUnit test_unit;
      m2.apply(test_unit);
      assert( test_unit.is_indeed_unit() );
    }
    m.resize_to(dim,dim-2);
    m.unit_matrix();
    {
      TestUnit test_unit;
      m.apply(test_unit);
      assert( test_unit.is_indeed_unit() );
    }
  }

  {
    cout << "check to see that Haar matrix has *exactly* orthogonal columns"
					<< endl;
    const int order = 5;
    Matrix haar = haar_matrix(order);
    assert( haar.q_nrows() == (1<<order) && haar.q_nrows() == haar.q_ncols() );
    Vector colj(1<<order), coll(1<<order);
    register int j;
    for(j=haar.q_col_lwb(); j<=haar.q_col_upb(); j++)
    {
      colj = MatrixColumn(haar,j);
      assert( fabs(abs(colj*colj - 1.0)) <= FLT_EPSILON );
      for(register int l=j+1; l<=haar.q_col_upb(); l++)
      {
	coll = MatrixColumn(haar,l);
	assert( colj*coll == 0 );
      }
    }
    cout << "make Haar (sub)matrix and test it *is* a submatrix" << endl;
    const int no_sub_cols = (1<<order) - 3;
    Matrix haar_sub = haar_matrix(order,no_sub_cols);
    assert( haar_sub.q_nrows() == (1<<order) && 
	    haar_sub.q_ncols() == no_sub_cols );
    for(j=haar_sub.q_col_lwb(); j<=haar_sub.q_col_upb(); j++)
    {
      colj = MatrixColumn(haar,j);
      coll = MatrixColumn(haar_sub,j);
      verify_matrix_identity(colj,coll);
    }
  }

  cout << "\nDone\n";
}


static void test_matrix_promises(const int dim)
{
  cout << "\n---> Check making/forcing promises, (lazy)matrices of dimension " <<
    dim << "\n\n";

  class hilbert_matrix_promise : public LazyMatrix
  {
    void fill_in(Matrix& m) const { m.hilbert_matrix(); }
  public:
    hilbert_matrix_promise(const int nrows, const int ncols)
	: LazyMatrix(nrows,ncols) {}
    hilbert_matrix_promise(const int _row_lwb, const int _row_upb,
	     const int _col_lwb, const int _col_upb)
	: LazyMatrix(_row_lwb,_row_upb,_col_lwb,_col_upb) {}
  };
  
  {
    cout << "make a promise and force it by a constructor" << endl;
    Matrix m = hilbert_matrix_promise(dim,dim+1);
    Matrix m1(Matrix::Zero,m);
    assert( !(m1 == m) && m1 == 0 );
    m1.hilbert_matrix();
    verify_matrix_identity(m,m1);
  }
  
  {
    cout << "make a promise and force it by an assignment" << endl;
    Matrix m(-1,dim,0,dim);
    Matrix m1(Matrix::Zero,m);
    m = hilbert_matrix_promise(-1,dim,0,dim);
    assert( !(m1 == m) && m1 == 0 );
    m1.hilbert_matrix();
    verify_matrix_identity(m,m1);
  }

  cout << "\nDone\n";
}

/*
 *------------------------------------------------------------------------
 *			Verify the norm calculation
 */

static void test_norms(const int rsize, const int csize)
{
  cout << "\n---> Verify norm calculations\n";

  const double pattern = 10.25;

  if( rsize % 2 == 1 || csize %2 == 1 )
    _error("Sorry, size of the matrix to test must be even for this test\n");

  Matrix m(rsize,csize);

  cout << "\nAssign " << pattern << " to all the elements and check norms\n";
  m = pattern;
  cout << "  1. (col) norm should be pattern*nrows\n";
  assert( m.norm_1() == pattern*m.q_nrows() );
  assert( m.norm_1() == m.col_norm() );
  cout << "  Inf (row) norm should be pattern*ncols\n";
  assert( m.norm_inf() == pattern*m.q_ncols() );
  assert( m.norm_inf() == m.row_norm() );
  cout << "  Square of the Eucl norm has got to be pattern^2 * no_elems\n";
  assert( m.e2_norm() == sqr(pattern)*m.q_no_elems() );
  Matrix m1(Matrix::Zero,m);
  assert( m.e2_norm() == e2_norm(m,m1) );

  cout << "\nDone\n";
}

/*
 *------------------------------------------------------------------------
 *				Root module
 */

main()
{
  cout << "\n\n" << _Minuses << 
          "\n\t\tVerify Operations on Matrices\n";

  test_allocation();
  test_element_op(20,10);
  test_binary_ebe_op(10,20);
  test_norms(40,20);
  test_special_creation(20);
  test_matrix_promises(20);
  test_transposition(20);
}

