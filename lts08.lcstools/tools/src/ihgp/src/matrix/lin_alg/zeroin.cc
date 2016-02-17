// This may look like C code, but it is really -*- C++ -*-
/*
 ************************************************************************
 *
 *			  Numerical Math Package
 *
 *			    Brent's root finder
 *	       obtains a zero of a function of one variable
 *
 * Synopsis
 *	double zeroin(ax,bx,f,tol=EPSILON)
 *	const double ax 		The root is to be sought within
 *	const double bx  		the interval [ax,bx]
 *	double (*f)(const double x)	Ptr to the function under 
 *					consideration    
 *	const double tol		Acceptable tolerance for the root
 *					position. It is an optional parameter
 *					with default value EPSILON
 *
 *	Zeroin returns an approximate location for the root with accuracy
 *	4*EPSILON*abs(x) + tol
 *
 * Algorithm
 *	G.Forsythe, M.Malcolm, C.Moler, Computer methods for mathematical
 *	computations. M., Mir, 1980, p.180 of the Russian edition
 *
 * The function makes use of the bissection procedure combined with
 * the linear or quadratic inverse interpolation.
 * At every step program operates three abscissae - a, b, and c.
 *	b - the last and the best approximation to the root
 *	a - the last but one approximation
 *	c - the last but one or even earlier approximation such that
 *		1) |f(b)| <= |f(c)|
 *		2) f(b) and f(c) have opposite signs, i.e. b and c encompass
 *		   the root
 * At every step Zeroin computes two new approximations, one by the 
 * bissection procedure and the other one from interpolation (if a,b, and c
 * are all different the quadratic interpolation is used, linear otherwise).
 * If the latter (i.e. obtained by the interpolation) point looks
 * reasonable (i.e. falls within the current interval [b,c] not close
 * to the end points of the interval), the point is accepted as a new
 * approximation to the root. Otherwise, the bissection result is used.
 * Therefore, the range of uncertainty is guaranteed to be reduced at 
 * least by the factor of 1.6
 *
 ************************************************************************
 */

#pragma implementation "math_num.h"
#include "math_num.h"


double zeroin(				// An estimate to the root
	const double ax,		// Specify the interval the root
	const double bx,		// to be sought in
	double (*f)(const double x),	// Function under investigation
	const double tol)		// Acceptable tolerance
{
  double a = ax, b = bx, c;		// Abscissae, see above
  double fa;				// f(a)
  double fb;				// f(b)
  double fc;				// f(c)

  assure( tol > 0, "Tolerance must be positive");
  assure( b > a, 
	 "Left end point of the interval should be strictly less than the "
	 "right one" );

  fa = (*f)(a);  fb = (*f)(b);
  c = a;   fc = fa;

  for(;;)		// Main iteration loop
  {
    double prev_step = b-a;		// Step from the previous iteration
   
    if( fabs(fc) < fabs(fb) )
    {                         		// Swap data so that b would be the
      a = b;  b = c;  c = a;          	// best approximation found so far
      fa=fb;  fb=fc;  fc=fa;
    }
					// Estimate the effective tolerance
    const double tol_act = 2*EPSILON*fabs(b) + tol/2;
    double new_step = (c-b)/2;		// Bissection step for this iteration

    if( fabs(new_step) <= tol_act || fb == 0 )
      return b;				// Acceptable approximation is found

    			// Figuring out if the interpolation can be tried
    if( fabs(prev_step) >= tol_act	// If prev_step was large enough
	&& fabs(fa) > fabs(fb) )	// and was in true direction,
    {					// Interpolatiom may be tried

      double p;      			// Interpolation step is calcu-
      double q;      			// lated in the form p/q; divi-
  					// sion operations is delayed
 					// until the last moment
      const double cb = c-b;

      if( a==c )			// If we've got only two distinct
      {					// points linear interpolation
	register double t1 = fb/fa;	// can only be applied
	p = cb*t1;
	q = 1.0 - t1;
      }
      else				// Quadratic inverse interpolation
      {
	register double t1, t2;
	q = fa/fc;  t1 = fb/fc;  t2 = fb/fa;
	p = t2 * ( cb*q*(q-t1) - (b-a)*(t1-1.0) );
	q = (q-1.0) * (t1-1.0) * (t2-1.0);
      }

      if( p > 0 )			// Formulas above computed new_step
	q = -q;				// = p/q with wrong sign (on purpose).
      else				// Correct this, but in such a way so
	p = -p;				// that p would be positive
      
      if( p < (0.75*cb*q-fabs(tol_act*q)/2)	// If b+p/q falls in [b,c]
	 && p < fabs(prev_step*q/2) )	// and isn't too large
	new_step = p/q;			// it is accepted
					// If p/q is too large then the
					// bissection procedure can
					// reduce [b,c] to a larger
					// extent
    }

    if( fabs(new_step) < tol_act )	// Adjust the step to be not less
      if( new_step > 0 )		// than the tolerance
	new_step = tol_act;
      else
	new_step = -tol_act;

    a = b;  fa = fb;			// Save the previous approximation
    b += new_step;  fb = (*f)(b);	// Do step to a new approximation
    if( (fb > 0 && fc > 0) || (fb < 0 && fc < 0) )
    {                 			// Adjust c for it to have the sign
      c = a;  fc = fa;                  // opposite to that of b
    }
  }

}
