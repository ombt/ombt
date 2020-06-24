// This may look like C code, but it is really -*- C++ -*-
/*
 ************************************************************************
 *
 *		Verify Advanced Operations on Matrices
 *	    (multiplication, inverse, determinant evaluation)
 *
 * $Id: vmatrix1.cc,v 3.1 1995/02/03 15:32:08 oleg Exp oleg $
 *
 ************************************************************************
 */

#include "LinAlg.h"
#include <math.h>
#include "builtin.h"
#include <iostream.h>
#include <float.h>

/*
 *------------------------------------------------------------------------
 *			Verify the determinant evaluation
 */

static void test_determinant(const int msize)
{
  cout << "\n---> Verify determinant evaluation\n"
          "for a square matrix of size " << msize << "\n";

  Matrix m(msize,msize);

  cout << "\nCheck to see that the determinant of the unit matrix is one";
  m.unit_matrix();
  cout << "\n	determinant is " << m.determinant();
  assert( m.determinant() == 1 );

  const double pattern = 2.5;
  cout << "\nCheck the determinant for the matrix with " << pattern <<
          "\n	at the diagonal";
  register int i,j;
  for(i=m.q_row_lwb(); i<=m.q_row_upb(); i++)
    for(j=m.q_col_lwb(); j<=m.q_col_upb(); j++)
      m(i,j) = ( i==j ? pattern : 0 );
  cout << "\n	determinant is " << m.determinant();
  assert( m.determinant() == pow(pattern,(long)m.q_nrows()) );

  cout << "\nCheck the determinant of the transposed matrix";
  m.unit_matrix();
  m(1,2) = pattern;
  Matrix m_tran(Matrix::Transposed,m);
  assert( !(m == m_tran) );
  assert( m.determinant() == m_tran.determinant() );

  cout << "\nCheck the determinant for the matrix with " << pattern <<
          "\n	at the anti-diagonal";
  for(i=m.q_row_lwb(); i<=m.q_row_upb(); i++)
    for(j=m.q_col_lwb(); j<=m.q_col_upb(); j++)
      m(i,j) = ( i==(m.q_col_upb()+m.q_col_lwb()-j) ? pattern : 0 );
  assert( m.determinant() == pow(pattern,(long)m.q_nrows()) * 
	 ( m.q_nrows()*(m.q_nrows()-1)/2 & 1 ? -1 : 1 ) );

  cout << "\nCheck the determinant for the singular matrix"
          "\n	defined as above with zero first row";
  m.clear();
  for(i=m.q_row_lwb()+1; i<=m.q_row_upb(); i++)
    for(j=m.q_col_lwb(); j<=m.q_col_upb(); j++)
      m(i,j) = ( i==(m.q_col_upb()+m.q_col_lwb()-j) ? pattern : 0 );
  cout << "\n	determinant is " << m.determinant();
  assert( m.determinant() == 0 );

  cout << "\nCheck out the determinant of the Hilbert matrix";
  Matrix H(3,3);
  H.hilbert_matrix();
  cout << "\n    3x3 Hilbert matrix: exact determinant 1/2160 ";
  cout << "\n                              computed    1/"<< 1/H.determinant();

  H.resize_to(4,4);
  H.hilbert_matrix();
  cout << "\n    4x4 Hilbert matrix: exact determinant 1/6048000 ";
  cout << "\n                              computed    1/"<< 1/H.determinant();

  H.resize_to(5,5);
  H.hilbert_matrix();
  cout << "\n    5x5 Hilbert matrix: exact determinant 3.749295e-12";
  cout << "\n                              computed    "<< H.determinant();

  H.resize_to(7,7);
  H.hilbert_matrix();
  cout << "\n    7x7 Hilbert matrix: exact determinant 4.8358e-25";
  cout << "\n                              computed    "<< H.determinant();

  H.resize_to(9,9);
  H.hilbert_matrix();
  cout << "\n    9x9 Hilbert matrix: exact determinant 9.72023e-43";
  cout << "\n                              computed    "<< H.determinant();

  H.resize_to(10,10);
  H.hilbert_matrix();
  cout << "\n    10x10 Hilbert matrix: exact determinant 2.16418e-53";
  cout << "\n                              computed    "<< H.determinant();

  cout << "\nDone\n";
}

/*
 *------------------------------------------------------------------------
 *			Verify matrix multiplications
 */

static void test_mm_multiplications(const int msize)
{
  cout << "\n---> Verify matrix multiplications\n"
          "for matrices of the characteristic size " << msize << "\n\n";

  {
    cout << "Test inline multiplications of the UnitMatrix" << endl;
    Matrix m(-1,msize,-1,msize);
    Matrix u(Matrix::Unit,m);
    m.hilbert_matrix(); m(3,1) = M_PI;
    u *= m;
    verify_matrix_identity(u,m);
  }

  {
    cout << "Test inline multiplications by a DiagMatrix" << endl;
    Matrix m(msize+3,msize);
    m.hilbert_matrix(); m(1,3) = M_PI;
    Vector v(msize);
    register int i;
    for(i=v.q_lwb(); i<=v.q_upb(); i++)
      v(i) = 1+i;
    Matrix diag(msize,msize);
    (MatrixDiag)diag = v;
    Matrix eth = m;
    for(i=eth.q_row_lwb(); i<=eth.q_row_upb(); i++)
      for(register int j=eth.q_col_lwb(); j<=eth.q_col_upb(); j++)
	eth(i,j) *= v(j);
    m *= diag;
    verify_matrix_identity(m,eth);
  }

  {
    cout << "Test XPP = X where P is a permutation matrix\n";
    Matrix m(msize-1,msize);
    m.hilbert_matrix(); m(2,3) = M_PI;
    Matrix eth = m;
    Matrix p(msize,msize);
    for(register int i=p.q_row_lwb(); i<=p.q_row_upb(); i++)
      p(p.q_row_upb()+p.q_row_lwb()-i,i) = 1;
    m *= p;
    m *= p;
    verify_matrix_identity(m,eth);
  }

  {
    cout << "Test general matrix multiplication through inline mult" << endl;
    Matrix m(msize-2,msize);
    m.hilbert_matrix(); m(3,3) = M_PI;
    Matrix mt(Matrix::Transposed,m);
    Matrix p(msize,msize);
    p.hilbert_matrix();
    MatrixDiag(p) += 1;
    Matrix mp(m,Matrix::Mult,p);
    Matrix m1 = m;
    m *= p;
    verify_matrix_identity(m,mp);
    Matrix mp1(mt,Matrix::TransposeMult,p);
    verify_matrix_identity(m,mp1);
    assert( !(m1 == m) );
    Matrix mp2(Matrix::Zero,m1);
    assert( mp2 == 0 );
    mp2.mult(m1,p);
    verify_matrix_identity(m,mp2);
  }

  {
    cout << "Check to see UU' = U'U = E when U is the Haar matrix" << endl;
    const int order = 5;
    const int no_sub_cols = (1<<order) - 5;
    Matrix haar_sub = haar_matrix(5,no_sub_cols);
    Matrix haar_sub_t(Matrix::Transposed,haar_sub);
    Matrix hsths(haar_sub_t,Matrix::Mult,haar_sub);
    Matrix hsths1(Matrix::Zero,hsths); hsths1.mult(haar_sub_t,haar_sub);
    Matrix hsths_eth(Matrix::Unit,hsths);
    assert( hsths.q_nrows() == no_sub_cols && hsths.q_ncols() == no_sub_cols );
    verify_matrix_identity(hsths,hsths_eth);
    verify_matrix_identity(hsths1,hsths_eth);
    
    Matrix haar = haar_matrix(5);
    Matrix unit(Matrix::Unit,haar);
    Matrix haar_t(Matrix::Transposed,haar);
    Matrix hth(haar,Matrix::TransposeMult,haar);
    Matrix hht(haar,Matrix::Mult,haar_t);
    Matrix hht1 = haar; hht1 *= haar_t;
    Matrix hht2(Matrix::Zero,haar); hht2.mult(haar,haar_t);
    verify_matrix_identity(unit,hth);
    verify_matrix_identity(unit,hht);
    verify_matrix_identity(unit,hht1);
    verify_matrix_identity(unit,hht2);
  }
  cout << "\nDone\n";
}

static void test_vm_multiplications(const int msize)
{
  cout << "\n---> Verify vector-matrix multiplications\n"
          "for matrices of the characteristic size " << msize << "\n\n";
  {
    cout << "Check shrinking a vector by multiplying by a non-sq unit matrix"
         << endl;
    Vector vb(-2,msize);
    for(register int i=vb.q_lwb(); i<=vb.q_upb(); i++)
      vb(i) = M_PI - i;
    assert( vb != 0 );
    Matrix mc(1,msize-2,-2,msize);	// contracting matrix
    mc.unit_matrix();
    Vector v1 = vb;
    Vector v2 = vb;
    v1 *= mc;
    v2.resize_to(1,msize-2);
    verify_matrix_identity(v1,v2);
  }

  {
    cout << "Check expanding a vector by multiplying by a non-sq unit matrix"
         << endl;
    Vector vb(msize);
    for(register int i=vb.q_lwb(); i<=vb.q_upb(); i++)
      vb(i) = M_PI + i;
    assert( vb != 0 );
    Matrix me(2,msize+5,1,msize);	// expanding matrix
    me.unit_matrix();
    Vector v1 = vb;
    Vector v2 = vb;
    v1 *= me;
    v2.resize_to(v1);
    verify_matrix_identity(v1,v2);
  }

  {
    cout << "Check general matrix-vector multiplication" << endl;
    Vector vb(msize);
    for(register int i=vb.q_lwb(); i<=vb.q_upb(); i++)
      vb(i) = M_PI + i;
    Matrix vm(msize,1);
    MatrixColumn(vm,1) = vb;
    Matrix m(0,msize,1,msize);
    m.hilbert_matrix();
    vb *= m;
    assert( vb.q_lwb() == 0 );
    Matrix mvm(m,Matrix::Mult,vm);
    verify_matrix_identity(vb,mvm);
  }

  cout << "\nDone\n";
}

static void test_inversion(const int msize)
{
  cout << "\n---> Verify matrix inversion for square matrices\n"
          "of size " << msize << "\n\n";
  {
    cout << "Test invesion of a diagonal matrix" << endl;
    Matrix m(-1,msize,-1,msize);
    Matrix mi(Matrix::Zero,m);
    for(register int i=m.q_row_lwb(); i<=m.q_row_upb(); i++)
      mi(i,i) = 1/(m(i,i) = i-m.q_row_lwb()+1);
    Matrix mi1(Matrix::Inverted,m);
    m.invert();
    verify_matrix_identity(m,mi);
    verify_matrix_identity(mi1,mi);
  }

  {
    cout << "Test invesion of an orthonormal (Haar) matrix" << endl;
    Matrix m = haar_matrix(3);
    Matrix morig = m;
    Matrix mt(Matrix::Transposed,m);
    double det = -1;		// init to a wrong val to see if it's changed
    m.invert(&det);
    assert( (det-1) <= FLT_EPSILON );
    verify_matrix_identity(m,mt);
    Matrix mti(Matrix::Inverted,mt);
    verify_matrix_identity(mti,morig);
  }

  {
    cout << "Test invesion of a good matrix with diagonal dominance" << endl;
    Matrix m(msize,msize);
    m.hilbert_matrix();
    MatrixDiag(m) += 1;
    Matrix morig = m;
    double det_inv = 0;
    const double det_comp = m.determinant();
    m.invert(&det_inv);
    cout << "\tcomputed determinant             " << det_comp << endl;
    cout << "\tdeterminant returned by invert() " << det_inv << endl;

    cout << "\tcheck to see M^(-1) * M is E" << endl;
    Matrix mim(m,Matrix::Mult,morig);
    Matrix unit(Matrix::Unit,m);
    verify_matrix_identity(mim,unit);

    cout << "\tcheck to see M * M^(-1) is E" << endl;
    Matrix mmi = morig; mmi *= m;
    verify_matrix_identity(mmi,unit);
  }
    
  cout << "\nDone\n";
}


/*
 *------------------------------------------------------------------------
 *				Root module
 */

main()
{
  cout << "\n\n" << _Minuses << 
          "\n\t\tVerify Advanced Operations on Matrices\n";

  test_determinant(20);
  test_mm_multiplications(20);
  test_vm_multiplications(20);

  test_inversion(20);
}

