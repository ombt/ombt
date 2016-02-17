// This may look like C code, but it is really -*- C++ -*-
/*
 ************************************************************************
 *
 *			Verify Operations on Vectors
 *
 * $Id: vvector.cc,v 3.1 1995/02/01 16:37:40 oleg Exp oleg $
 *
 ************************************************************************
 */

#include "LinAlg.h"
#include <math.h>
#include "builtin.h"
#include <iostream.h>

/*
 *------------------------------------------------------------------------
 *			Service validation functions
 */
static void verify_vector_identity(const Vector& v1, const Vector& v2)
{ verify_matrix_identity(v1,v2); }

/*
 *------------------------------------------------------------------------
 *	   Test allocation functions and compatibility check
 */

static void test_allocation(void)
{

  cout << "\n\n---> Test allocation and compatibility check\n";

  Vector v1(20);
  Vector v2(1,20);
  Vector v3(0,19);
  Vector v4(v1);
  cout << "The following vector have been allocated\n";
  v1.info(), v2.info(), v3.info(), v4.info();

  cout << "\nStatus information reported for vector v3:\n";
  cout << "  Lower bound ... " << v3.q_lwb() << "\n";
  cout << "  Upper bound ... " << v3.q_upb() << "\n";
  cout << "  No. of elements " << v3.q_no_elems() << "\n";
  cout << "  Name " << v3.q_name() << "\n";

  cout << "\nCheck vectors 1 & 2 for compatibility\n";
  are_compatible(v1,v2);

  cout << "Check vectors 1 & 4 for compatibility\n";
  are_compatible(v1,v4);

#if 0
  cout << "Check vectors 1 & 3 for compatibility\n";
  are_compatible(v1,v3);
#endif

  cout << "v2 has to be compatible with v3 after resizing to v3\n";
  v2.resize_to(v3);
  are_compatible(v2,v3);

  Vector v5(v1.q_upb()+5); v5.set_name("Vector v5");
  cout << "v1 has to be compatible with v5 after resizing to v5.upb\n";
  v5.info(); cout << endl;
  v1.resize_to(v5.q_no_elems());
  are_compatible(v1,v5);

  {
    cout << "Check that shrinking does not change remaining elements" << endl;
    Vector vb(-1,20);
    register int i;
    for(i=vb.q_lwb(); i<=vb.q_upb(); i++)
      vb(i) = i+M_PI;
    Vector v = vb;
    assert( v == vb );
    assert( v != 0 );
    v.resize_to(0,10);
    for(i=v.q_lwb(); i<=v.q_upb(); i++)
      assert( v(i) == vb(i-v.q_lwb()+vb.q_lwb()) );
    cout << "Check that expansion expands by zeros" << endl;
    const int old_nelems = v.q_upb() - v.q_lwb() + 1;
    v.resize_to(vb);
    assert( !(v == vb) );
    for(i=v.q_lwb(); i<v.q_lwb()+old_nelems; i++)
      assert( v(i) == vb(i) );
    for(; i<=v.q_upb(); i++)
      assert( v(i) == 0 );
  }
  cout << "\nDone\n";
}

/*
 *------------------------------------------------------------------------
 *		     Test uniform element operations
 */

static void test_element_op(const int vsize)
{
  const double pattern = M_PI;
  register int i;

  cout << "\n\n---> Test operations that treat each element uniformly\n";

  Vector v(-1,vsize-2);
  Vector v1(v);

  cout << "Writing zeros to v...\n";
  for(i=v.q_lwb(); i<=v.q_upb(); i++)
    v(i) = 0;
  verify_element_value(v,0);

  cout << "Clearing v1 ...\n";
  v1.clear();
  verify_element_value(v1,0);

  cout << "Comparing v1 with 0 ...\n";
  assure(v1 == 0, "v1 is not zero!");

  cout << "Writing a pattern " << pattern << " by assigning to v(i)...\n";
  for(i=v.q_lwb(); i<=v.q_upb(); i++)
    v(i) = pattern;
  verify_element_value(v,pattern);

  cout << "Writing the pattern by assigning to v1 as a whole ...\n";
  v1 = pattern;
  verify_element_value(v1,pattern);

  cout << "Comparing v and v1 ...\n";
  assure(v == v1, "v and v1 are unexpectedly different!");
  cout << "Comparing (v=0) and v1 ...\n";
  assert(!(v.clear() == v1));

  cout << "\nClear v and add the pattern\n";
  v.clear();
  v += pattern;
  verify_element_value(v,pattern);
  cout << "   add the doubled pattern with the negative sign\n";
  v += -2*pattern;
  verify_element_value(v,-pattern);
  cout << "   subtract the trippled pattern with the negative sign\n";
  v -= -3*pattern;
  verify_element_value(v,2*pattern);

  cout << "\nVerify comparison operations\n";
  v = pattern;
  assert( v == pattern && !(v != pattern) && v >= pattern && v <= pattern );
  assert( v > 0 && v >= 0 );
  assert( v > -pattern && v >= -pattern );
  assert( v < pattern+1 && v <= pattern+1 );
  v(v.q_upb()) += 1;
  assert( !(v==pattern) && !(v != pattern) && v != pattern-1 );
  assert( v >= pattern && !(v > pattern) && !(v >= pattern+1) );
  assert( v <= (REAL)pattern+1 && !(v < pattern+1) && !(v <= pattern) );

  cout << "\nAssign 2*pattern to v by repeating additions\n";
  v = 0; v += pattern; v += pattern;
  cout << "Assign 2*pattern to v1 by multiplying by two \n";
  v1 = pattern; v1 *= 2;
  verify_element_value(v1,2*pattern);
  assert( v == v1 );
  cout << "Multiply v1 by one half returning it to the 1*pattern\n";
  v1 *= 1/2.;
  verify_element_value(v1,pattern);

  cout << "\nAssign -pattern to v and v1\n";
  v.clear(); v -= pattern; v1 = -pattern;
  verify_element_value(v,-pattern);
  assert( v == v1 );
  cout << "v = sqrt(sqr(v)); v1 = abs(v1); Now v and v1 have to be the same\n";
  v.sqr();
  verify_element_value(v,pattern*pattern);
  v.sqrt();
  verify_element_value(v,pattern);
  v1.abs();
  verify_element_value(v1,pattern);
  assert( v == v1 );

  {
    cout << "\nCheck out to see that sin^2(x) + cos^2(x) = 1\n";
    for(i=v.q_lwb(); i<=v.q_upb(); i++)
      v(i) = 2*M_PI/v.q_no_elems() * i;
    class SinAction : public ElementPrimAction
    {
      void operation(REAL& element) { element = sin(element); }
      public: SinAction(void) {}
    };
    v.apply(SinAction());
    Vector v2 = v;
    
    class CosAction : public ElementAction
    {
      double factor;
      void operation(REAL& element) { element = cos(factor*i); }
      public:
      CosAction(const int no_elems): factor(2*M_PI/no_elems) {}
    };
    v1.apply(CosAction(v.q_no_elems()));
    Vector v3 = v1;
    v.sqr();
    v1.sqr();
    v += v1;
    verify_element_value(v,1);
 
    cout << "\n\tdo it again through LazyMatrix promise of a vector" << endl;
    class square_add : public LazyMatrix, public ElementAction
    {
      const Vector &v1; Vector &v2;
      void operation(REAL& element)
      		{ assert(j==1); element = v1(i)*v1(i) + v2(i)*v2(i); }
     void fill_in(Matrix& m) const { m.apply(*this); }
     public: square_add(const Vector& _v1, Vector& _v2) :
   	LazyMatrix(_v1.q_row_lwb(),_v1.q_row_upb(),1,1),
   	v1(_v1), v2(_v2) {}
    };
    Vector vres = square_add(v2,v3);
    Vector vres1 = v2; assert( !(vres1 == vres) );
    verify_element_value(vres,1);
    vres1 = square_add(v2,v3);
    verify_element_value(vres1,1);
  }

  cout << "\nVerify constructor with initialization\n";
  Vector vi(1,5,0.0,1.0,2.0,3.0,4.0,"END");
  Vector vit(5);
  for(i=vit.q_lwb(); i<=vit.q_upb(); i++)
    vit(i) = i-1;
  verify_vector_identity(vi,vit);
  
  cout << "\nDone\n\n";
}

/*
 *------------------------------------------------------------------------
 *			Test binary vector operations
 */

static void test_binary_op(const int vsize)
{
  const double pattern = M_PI;
  register int i;

  cout << "\n---> Test Binary Vector operations\n";

  Vector v(2,vsize+1);
  Vector v1(v);
  Vector vp(v);

  for(i=v.q_lwb(); i<=v.q_upb(); i++)
    vp(i) = (i-v.q_no_elems()/2.)*pattern;
  

  cout << "\nVerify assignment of a vector to the vector\n";
  v = pattern;
  v1.clear();
  v1 = v;
  verify_element_value(v1,pattern);
  assert( v1 == v );

  cout << "\nAdding one vector to itself, uniform pattern " << pattern << "\n";
  v.clear(); v = pattern;
  v1 = v; v1 += v1;
  verify_element_value(v1,2*pattern);
  cout << "  subtracting two vectors ...\n";
  v1 -= v;
  verify_element_value(v1,pattern);
  cout << "  subtracting the vector from itself\n";
  v1 -= v1;
  verify_element_value(v1,0);
  cout << "  adding two vectors together\n";
  v1 += v;
  verify_element_value(v1,pattern);

  cout << "\nArithmetic operations on vectors with not the same elements\n";
  cout << "   adding vp to the zero vector...\n";
  v.clear(); v += vp;
//  verify_vector_identity(v,vp);
  assert( v == vp );
  v1 = v;
  cout << "   making v = 3*vp and v1 = 3*vp, via add() and succesive mult\n";
  add(v,2,vp);
  v1 += v1; v1 += vp;
  verify_vector_identity(v,v1);
  cout << "   clear both v and v1, by subtracting from itself and via add()\n";
  v1 -= v1;
  add(v,-3,vp);
  verify_vector_identity(v,v1);

  cout << "\nTesting element-by-element multiplications and divisions\n";
  cout << "   squaring each element with sqr() and via multiplication\n";
  v = vp; v1 = vp;
  v.sqr();
  element_mult(v1,v1);
  verify_vector_identity(v,v1);
  cout << "   compare (v = pattern^2)/pattern with pattern\n";
  v = pattern; v1 = pattern;
  v.sqr();
  element_div(v,v1);
  verify_element_value(v,pattern);
  compare(v1,v,"Original vector and vector after squaring and dividing");

  cout << "\nDone\n";
}

/*
 *------------------------------------------------------------------------
 *			Verify the norm calculation
 */

static void test_norms(const int vsize)
{
  cout << "\n---> Verify norm calculations\n";

  const double pattern = 10.25;

  if( vsize % 2 == 1 )
    _error("Sorry, size of the vector to test must be even for this test\n");

  Vector v(vsize);
  Vector v1(v);

  cout << "\nAssign " << pattern << " to all the elements and check norms\n";
  v = pattern;
  cout << "  1. norm should be pattern*no_elems\n";
  assert( v.norm_1() == pattern*v.q_no_elems() );
  cout << "  Square of the 2. norm has got to be pattern^2 * no_elems\n";
  assert( v.norm_2_sqr() == sqr(pattern)*v.q_no_elems() );
  cout << "  Inf norm should be pattern itself\n";
  assert( v.norm_inf() == pattern );
  cout << "  Scalar product of vector by itself is the sqr(2. vector norm)\n";
  assert( v.norm_2_sqr() == v * v );

  double ap_step = 1;
  double ap_a0   = -pattern;
  int n = v.q_no_elems();
  cout << "\nAssign the arithm progression with 1. term " << ap_a0 <<
          "\nand the difference " << ap_step << "\n";
  register int i;
  for(i=v.q_lwb(); i<=v.q_upb(); i++)
    v(i) = (i-v.q_lwb())*ap_step + ap_a0;
  int l = min(max((int)ceil(-ap_a0/ap_step),0),n);
  double norm = (2*ap_a0 + (l+n-1)*ap_step)/2*(n-l) +
    (-2*ap_a0-(l-1)*ap_step)/2*l;
  cout << "  1. norm should be " << norm << "\n";
  assert( v.norm_1() == norm );
  norm = n*( sqr(ap_a0) + ap_a0*ap_step*(n-1) + sqr(ap_step)*(n-1)*(2*n-1)/6);
  cout << "  Square of the 2. norm has got to be "
          "n*[ a0^2 + a0*q*(n-1) + q^2/6*(n-1)*(2n-1) ], or " << norm << "\n";
  assert( v.norm_2_sqr() == norm );
  norm = max(abs(v(v.q_lwb())),abs(v(v.q_upb())));
  cout << "  Inf norm should be max(abs(a0),abs(a0+(n-1)*q)), ie " << norm <<
          "\n";
  assert( v.norm_inf() == norm );
  cout << "  Scalar product of vector by itself is the sqr(2. vector norm)\n";
  assert( v.norm_2_sqr() == v * v );

  v1.clear();
  compare(v,v1,"Compare the vector v with a zero vector");

  cout << "\nConstruct v1 to be orthogonal to v as v(n), -v(n-1), v(n-2)...\n";
  for(i=0; i<v1.q_no_elems(); i++)
    v1(i+v1.q_lwb()) = v(v.q_upb()-i) * ( i % 2 == 1 ? -1 : 1 );
  cout << "||v1|| has got to be equal ||v|| regardless of the norm def\n";
  assert( v1.norm_1() == v.norm_1() );
  assert( v1.norm_2_sqr() == v.norm_2_sqr() );
  assert( v1.norm_inf() == v.norm_inf() );
  cout << "But the scalar product has to be zero\n";
  assert( v1 * v == 0 );

  cout << "\nDone\n";
}

/*
 *------------------------------------------------------------------------
 *	    Test operations with vectors and matrix slices
 */

static void test_matrix_slices(const int vsize)
{
  const REAL pattern = 8.625;
  register int i;

  cout << "\n\n---> Test operations with vectors and matrix slices\n";

  Vector vc(0,vsize);
  Vector vr(0,vsize+1);
  Matrix m(0,vsize,0,vsize+1);

  cout << "\nCheck modifying the matrix column-by-column\n";
  m = pattern;
  assert( m == pattern );
  for(i=m.q_col_lwb(); i <= m.q_col_upb(); i++)
  {
    MatrixColumn(m,i) = pattern-1;
    assert( !( m == pattern ) && !( m != pattern ) );
    MatrixColumn(m,i) *= 2;
    vc = MatrixColumn(m,i);
    verify_element_value(vc,2*(pattern-1));
    vc = MatrixColumn(m, i+1 > m.q_col_upb() ? m.q_col_lwb() : i+1);
    verify_element_value(vc,pattern);
    MatrixColumn(m,i) *= 0.5;
    MatrixColumn(m,i) += 1;
    assert( m == pattern );
  }

  assert( m == pattern );
  for(i=m.q_col_lwb(); i <= m.q_col_upb(); i++)
  {
    vc = pattern+1;
    MatrixColumn(m,i) = vc;
    assert( !( m == pattern ) && !( m != pattern ) );
    {
      MatrixColumn mc(m,i);
      for(register int j=m.q_row_lwb(); j<=m.q_row_upb(); j++)
        mc(j) *= 4;
    }
    vc = MatrixColumn(m,i);
    verify_element_value(vc,4*(pattern+1));
    MatrixColumn(m,i) *= 0.25;
    MatrixColumn(m,i) += -1;
    vc = MatrixColumn(m,i);
    verify_element_value(vc,pattern);
    assert( m == pattern );
  }

  cout << "\nCheck modifying the matrix row-by-row\n";
  m = pattern;
  assert( m == pattern );
  for(i=m.q_row_lwb(); i <= m.q_row_upb(); i++)
  {
    MatrixRow(m,i) = pattern+2;
    assert( !( m == pattern ) && !( m != pattern ) );
    vr = MatrixRow(m,i);
    verify_element_value(vr,pattern+2);
    vr = MatrixRow(m,i+1 > m.q_row_upb() ? m.q_row_lwb() : i+1);
    verify_element_value(vr,pattern);
    MatrixRow(m,i) += -2;
    assert( m == pattern );
  }

  assert( m == pattern );
  for(i=m.q_row_lwb(); i <= m.q_row_upb(); i++)
  {
    vr = pattern-2;
    MatrixRow(m,i) = vr;
    assert( !( m == pattern ) && !( m != pattern ) );
    {
      MatrixRow mr(m,i);
      for(register int j=m.q_col_lwb(); j<=m.q_col_upb(); j++)
        mr(j) *= 8;
    }
    vr = MatrixRow(m,i);
    verify_element_value(vr,8*(pattern-2));
    MatrixRow(m,i) *= 1./8;
    MatrixRow(m,i) += 2;
    vr = MatrixRow(m,i);
    verify_element_value(vr,pattern);
    assert( m == pattern );
  }

  cout << "\nCheck modifying the matrix diagonal\n";
  m = pattern;
  (MatrixDiag)m = pattern-3;
  assert( !( m == pattern ) && !( m != pattern ) );
  vc = MatrixDiag(m);
  verify_element_value(vc,pattern-3);
  MatrixDiag(m) += 3;
  assert( m == pattern );
  vc = pattern+3;
  (MatrixDiag)m = vc;
  assert( !( m == pattern ) && !( m != pattern ) );
  {
    MatrixDiag md(m);
    for(register int j=1; j<=md.q_ndiags(); j++)
      md(j) /= 1.5;
  }
  vc = MatrixDiag(m);
  verify_element_value(vc,(pattern+3)/1.5);
  MatrixDiag(m) *= 1.5;
  MatrixDiag(m) += -3;
  vc = MatrixDiag(m);
  verify_element_value(vc,pattern);
  assert( m == pattern );

  cout << "\nCheck out to see that multiplying by diagonal is column-wise"
          "\nmatrix multiplication\n";
  Matrix mm(m);
  Matrix m1(m.q_row_lwb(),::max(m.q_row_upb(),m.q_col_upb()),
	    m.q_col_lwb(),::max(m.q_row_upb(),m.q_col_upb()));
  Vector vc1(vc), vc2(vc);
  for(i=m.q_row_lwb(); i<m.q_row_upb(); i++)
    MatrixRow(m,i) = pattern+i;		// Make a multiplicand
  mm = m;				// Save it

  m1 = pattern+10;
  for(i=vr.q_lwb(); i<=vr.q_upb(); i++)
    vr(i) = i+2;
  (MatrixDiag)m1 = vr;			// Make the other multiplicand
  assert( !(m1 == pattern+10) );

  m *= MatrixDiag(m1);
  for(i=m.q_col_lwb(); i<=m.q_col_upb(); i++)
  {
    vc1 = MatrixColumn(mm,i);
    vc1 *= vr(i);			// Do a column-wise multiplication
    vc2 = MatrixColumn(m,i);
    verify_vector_identity(vc1, vc2);
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
          "\n\t\tVerify Operations on Vectors\n";

  test_allocation();
  test_element_op(20);
  test_binary_op(20);
  test_norms(20);
  test_matrix_slices(20);
}

