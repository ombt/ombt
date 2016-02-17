// This may look like C code, but it is really -*- C++ -*-
//
//			Verify FMINBR routine


#include "math_num.h"
#include <iostream.h>

static int counter;			// Iteration counter

			// Run a test
static void test
  (const double a, const double b, double (*f)(const double x), const char *msg)
{
  double minloc;
  counter = 0;
  printf("\nFor function %s\nthe minimum over [%g,%g] is found"
	       " to be at\t%.9e\n",msg,a,b,
	       (minloc=fminbr(a,b,f)) );
  printf("Min function value found\t%.4e\nNo. of iterations\t\t%d\n",
	       (*f)(minloc), counter);
}

static double f1(const double x)		// Test from the Forsythe book
{
  counter++;
  return (sqr(x)-2)*x - 5;
}

static double f2(const double x)		// Modified test 1
{
  counter++;
  return sqr( (sqr(x)-2)*x - 5 );
}

static double f3(const double x)
{
  counter++;
  return sqr( cos(x) - x ) - 2;
}

static double f4(const double x)
{
  counter++;
  return sqr( sin(x) - x ) + 1;
}


main(void)
{
  cout << "\n\n" << _Minuses <<
          "\n\t\tTesting the Brent's one-dimensional minimizer\n\n";

  test(0.0,1.0,f1,"x^3 - 2*x - 5");
  cout << "Exact min is at\t\t0.81650\n";

  test(2.0,3.0,f2,"(x^3 - 2*x - 5)^2");
  cout << "Exact root is \t\t2.0945514815\n";

  test(2.0,3.0,f3,"(cos(x)-x)^2 - 2");
  test(-1.0,3.0,f3,"(cos(x)-x)^2 - 2");
  test(-1.0,3.0,f4,"(sin(x)-x)^2 + 1");
}

