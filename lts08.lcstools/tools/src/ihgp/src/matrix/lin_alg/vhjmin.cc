// This may look like C code, but it is really -*- C++ -*-
/*
 ************************************************************************
 *
 *		   Verify the Hook-Jeevse minimization
 *
 ************************************************************************
 */

#include "LinAlg.h"
#include "math_num.h"
#include "builtin.h"
#include <iostream.h>


static int iter_count;

/*
 *-----------------------------------------------------------------------
 * 			Simplified vector print
 */

static void pr_vector(Vector& v)
{
  register int i;
  for(i=v.q_lwb(); i<=v.q_upb(); i++)
     printf("%9.3g  ",v(i));
}


/*
 *------------------------------------------------------------------------
 *			  Rosenbroke function
 */

static double f1(const Vector& v)
{
  register double x1 = v(1);
  register double x2 = v(2);
  iter_count++;
  return 100*sqr(x2 - x1*x1) + sqr(1 - x1);
}
 
static void test1()
{
  const int n = 2;
  Vector b0(1,n,-1.2,1.,"END");			// Initial guess
  Vector b = b0;				// Min location found
  Vector bm(1,n,1.0,1.0,"END");			// Exact min location
  Vector h(1,n,10.,10.,"END");			// Initial step

  iter_count = 0;
  cout << "\n\n\tRosenbroke function\n";
  cout << "\n\n\tf = 100*(x2-x1^2)^2 + (1-x1)^2\n\n";
  cout << "\nInitial guess         b0 = "; pr_vector(b0);
  cout << "\nFunction value at it  f0 = " << f1(b0);
  cout << "\nInitial steps         h0 = "; pr_vector(h);
  cout << "\n";
  cout << "\nMinimum f value found f  = " << hjmin(b,h,f1);
  cout << "\n                  at  b  = "; pr_vector(b);
  cout << "\nFinal steps           h = "; pr_vector(h);
  cout << "\nExact min location    bm = "; pr_vector(bm);
  cout << "\nNo. of iterations     ni = " << iter_count << "\n";

}



/*
 *------------------------------------------------------------------------
 *			     Bocks function
 */

static double f2(const Vector& v)
{
  register double x1 = v(1);
  register double x2 = v(2);
  iter_count++;
  return sqr( exp(-1./10) - exp(-x1/10) + exp(-10./10) - exp(-x2/10) );
}
 
static void test2()
{
  const int n = 2;
  Vector b0(1,n,0.,0.,"END");			// Initial guess
  Vector b =b0;					// Min location found
  Vector bm(1,n,10.0,1.0,"END");		// Exact min location
  double h0=10;					// Initial step

  iter_count = 0;
  cout << "\n\n\tBocks function\n";
  cout << "\n\n\t"
	  "f = [ exp(-1/10) - exp(-x1/10) + exp(-10/10) -exp(-x2/10) ]^2\n\n";
  cout << "\nInitial guess         b0 = "; pr_vector(b0);
  cout << "\nFunction value at it  f0 = " << f2(b0);
  cout << "\nInitial steps         h0 = " << h0;
  cout << "\n";
  cout << "\nMinimum f value found f  = " << hjmin(b,h0,f2);
  cout << "\n                  at  b  = "; pr_vector(b);
  cout << "\nExact min location    bm = "; pr_vector(bm);
  cout << "\nNo. of iterations     ni = " << iter_count << "\n";

}



/*
 *------------------------------------------------------------------------
 *			Mile & Cuntrell function
 */

static double f3(const Vector& v)
{
  register double x1 = v(1);
  register double x2 = v(2);
  register double x3 = v(3);
  register double x4 = v(4);
  iter_count++;
  return pow( exp(x1)-x2, 4L) + 100*pow(x2-x3,6L) + pow(atan(x3-x4),4L) +
	 pow(x1,8L);
}
 
static void test3()
{
  const int n = 4;
  Vector b0(1,n,1.,2.,2.,2.,"END");		// Initial guess
  Vector b = b0;					// Min location found
  Vector bm(1,n,0.,1.,1.,1.,"END");		// Exact min location
  double h0 = 10;				// Initial step

  iter_count = 0;
  cout << "\n\n\tMile & Cuntrell function\n";
  cout << "\n\n\t"
	  "f = [ exp(x1)-x2 ]^4 +100(x2-x3)^6 + atan(x3-x4)^4 + x1^8\n\n";
  cout << "\nInitial guess         b0 = "; pr_vector(b0);
  cout << "\nFunction value at it  f0 = " << f3(b0);
  cout << "\nInitial steps         h0 = " << h0;
  cout << "\n";
  cout << "\nMinimum f value found f  = " << hjmin(b,h0,f3);
  cout << "\n                  at  b  = "; pr_vector(b);
  cout << "\nExact min location    bm = "; pr_vector(bm);
  cout << "\nNo. of iterations     ni = " << iter_count << "\n";

}

/*
 *------------------------------------------------------------------------
 *			  Powell function
 */

static double f4(const Vector& v)
{
  register double x1 = v(1);
  register double x2 = v(2);
  register double x3 = v(3);
  register double x4 = v(4);
  iter_count++;
  return sqr(x1+10*x2) + 5*sqr(x3-x4) + pow(x2-2*x3,4L) + 10*pow(x1-x4,4L);
}
 
static void test4()
{
  const int n = 4;
  Vector b0(1,n,3.,-1.,0.,1.,"END");		// Initial guess
  Vector b = b0;				// Min location found
  Vector bm(1,n,0.,0.,0.,0.,"END");		// Exact min location
  Vector h0(1,n,10.,10.,10.,10.,"END");		// Initial step

  iter_count = 0;
  cout << "\n\n\tPowell function\n";
  cout << "\n\n\t" 
	  "f = (x1+10*x2)^2 + 5(x3-x4)^2 + (x2-2x3)^4 + 10(x1-x4)^4\n\n";
  cout << "\nInitial guess         b0 = "; pr_vector(b0);
  cout << "\nFunction value at it  f0 = " << f4(b0);
  cout << "\nInitial steps         h0 = "; pr_vector(h0);
  cout << "\n";
  cout << "\nMinimum f value found f  = " << hjmin(b,h0,f4);
  cout << "\n                  at  b  = "; pr_vector(b);
  cout << "\nExact min location    bm = "; pr_vector(bm);
  cout << "\nNo. of iterations     ni = " << iter_count << "\n";

}


/*
 *------------------------------------------------------------------------
 *				Root module
 */

main()
{
  cout << "\n\n\n\t\tVerify HJMIN multidimensional minimizer\n";
  test1();
  test2();
  test3();
  test4();
}

