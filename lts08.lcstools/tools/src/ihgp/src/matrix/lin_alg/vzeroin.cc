// This may look like C code, but it is really -*- C++ -*-
//
//			Verify ZEROIN routine

#include "math_num.h"
#include <iostream.h>

static int counter;			// Iteration counter

			// Run a test
static void test
  (const double a, const double b, double (*f)(double x), const char *msg)
{
  double root;
  counter = 0;
  printf("\nFor function %s,\nthe root over [%g,%g] is\t%.9e\n",msg,a,b,
	 (root=zeroin(a,b,f)) );
  printf("Function value at the root found\t%.4e\n"
	       "No. of iterations\t\t%d\n",
	       (*f)(root), counter);
}

static double f1(const double x)		// Test from the Forsythe book
{
  counter++;
  return (sqr(x)-2)*x - 5;
}

static double f2(const double x)
{
  counter++;
  return cos(x) - x;
}

static double f3(const double x)
{
  counter++;
  return sin(x) - x;
}


main(void)
{
  cout << "\n\n" << _Minuses <<
          "\n\t\tTesting the Brent's root finder\n\n";

  test(2.0,3.0,f1,"x^3 - 2*x - 5");
  cout << "Exact root is \t\t2.0945514815\n";

  test(2.0,3.0,f2,"cos(x)-x");
  test(-1.0,3.0,f2,"cos(x)-x");
  test(-1.0,3.0,f3,"sin(x)-x");
}
