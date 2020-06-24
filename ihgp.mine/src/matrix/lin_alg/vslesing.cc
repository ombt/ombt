// This may look like C code, but it is really -*- C++ -*-
/*
 ************************************************************************
 *
 *	Verify application of SVD to solving sets of simultaneous
 *				linear equations
 *
 * $Id$
 *
 ************************************************************************
 */

#include "svd.h"
#include <iostream.h>

					// Solve Ax=b and test the result
static void test_sle(const Matrix& A, const Vector& b, const Vector& x_true)
{
  SVD svd(A);
  cout << "\ncondition number " << svd.q_cond_number() << endl;
  Vector x = SVD_inv_mult(svd,b);
  Matrix solutions(2,x_true.q_upb());
  MatrixRow(solutions,1) = x_true;
  MatrixRow(solutions,2) = x;
  solutions.print("true vs. computed solution");
  Vector b_comp = x;
  b_comp *= A;
  cout << "\tchecking to see that Ax is indeed b\n";
  verify_matrix_identity(b_comp,b);
}

static void test1(const int neqs, const int nvars)
{
  cout << "\n\nChecking solution of a set of linear equations\n"
          "with a Hilb+E of order " << neqs << "x" << nvars << endl;
  Vector x(nvars);
  Matrix A(neqs,nvars);
  A.hilbert_matrix(); (MatrixDiag)A += 1;
  struct fill : public ElementAction
  {
    void operation(REAL& element) { element = i; }
  };
  x.apply(fill());
  Vector b = x;
  b *= A;
  test_sle(A,b,x);
}

static void test2(const int dim)
{
  cout << "\n\nChecking solution of a set of linear equations\n"
          "with a Hilbert matrix of order " << dim << endl;
  Vector x(dim);
  Matrix A(dim,dim);
  A.hilbert_matrix();
  struct fill : public ElementAction
  {
    void operation(REAL& element) { element = i; }
  };
  x.apply(fill());
  Vector b = x;
  b *= A;
  test_sle(A,b,x);
}


main(void)
{
 cout << "\n\nVerify application of SVD to solving sets of simultaneous "
         "equations" << endl;
 test1(10,10);
 test1(20,10);
 test2(10);
}
