// This may look like C code, but it is really -*- C++ -*-
/*
 ************************************************************************
 *
 *			  Numerical Math Package
 *
 *		    Brent's one-dimensional minimizer 
 *
 *	     finds a local minimum of a single argument function
 *			  over the given range
 *
 * Input
 *	double fminbr(ax,bx,f,tol)
 *	const double ax		        a and b, a < b, specify the interval
 *	const double bx  		the minimum is to be sought in
 *	double (*f)(const double x)	Ptr to the function under investigation
 *	const double tol		Acceptable tolerance for the minimum
 *					location. It is an optional parameter
 *					with default value EPSILON
 *
 * Output
 *	Fminbr returns an estimate to the minimum location with accuracy
 *	3*SQRT_EPSILON*abs(x) + tol.
 *	The procedure always determines a local minimum, which coincides with
 *	the global one if and only if the function under investigation is
 *	unimodular.
 *	If a function being examined possesses no local minimum within
 *	the given range, Fminbr returns either the left or the right end
 *	point of the interval, wherever the function value is smaller.
 *
 * Algorithm
 *	G.Forsythe, M.Malcolm, C.Moler, Computer methods for mathematical
 *	computations. M., Mir, 1980, p.202 of the Russian edition
 *
 * The function makes use of the "gold section" procedure combined with
 * the parabolic interpolation.
 * At every step program operates three abscissae - x,v, and w.
 * 	x - the last and the best approximation to the minimum location,
 *		i.e. f(x) <= f(a) or/and f(x) <= f(b)
 * 	    (if the function f has a local minimum in (a,b), then both
 *  	     conditions are met after one or two steps).
 *	v,w are previous approximations to the minimum location. They may
 *	coincide with a, b, or x (although the algorithm tries to make all
 *	u, v, and w distinct). 
 * Points x, v, and w are used to construct an interpolating parabola,
 * whose minimum is to be regarded as a new approximation to the minimum
 * location if the former falls within [a,b] and reduces the minimum 
 * encompassing interval to a larger extent than the gold section procedure.
 * When f(x) has a second derivative positive at the point of minimum
 * (not coinciding with a or b) the procedure converges superlinearly
 * at a rate of about 1.324
 *
 ************************************************************************
 */

#include "math_num.h"


double fminbr(				// An estimate to the min location
	const double ax,		// Specify the interval the minimum
	const double bx,		// to be sought in
	double (*f)(const double x),	// Function under investigation
	const double tol)		// Acceptable tolerance
{
  double a = ax, b = bx;		// Current interval
  double x,v,w;				// Abscissae, descr. see above
  double fx;				// f(x)
  double fv;				// f(v)
  double fw;				// f(w)
  const double r = (3-sqrt(5.0))/2;	// Golden section ratio

  assure( tol > 0, "Tolerance must be positive");
  assure( b > a, 
	 "Left end point of the interval should be strictly less than the "
	 "right one" );

  v = a + r*(b-a);  fv = (*f)(v);       // First step - always gold section
  x = v;  w = v;
  fx=fv;  fw=fv;

  for(;;)		// Main iteration loop
  {
    const double range = b-a;		// Interval over which the minimum
					// is sought for
    const double midpoint = (a+b)/2;
    const double tol_act =		// Actual tolerance
		SQRT_EPSILON*fabs(x) + tol/3;

       

    if( fabs(x-midpoint) + range/2 <= 2*tol_act )
      return x;				// Acceptable approximation is found

					// Compute a new step with the gold
					// section
    double new_step = r * ( x < midpoint ? b-x : a-x );


    			// Decide on the interpolation  
    if( fabs(x-w) >= tol_act  )		// If x and w are distinct
    {					// interpolatiom may be tried
      register double p; 		// Interpolation step is calcula-
      register double q;              	// ted as p/q; division operation
                                        // is delayed until last moment
      register double t;

      t = (x-w) * (fx-fv);
      q = (x-v) * (fx-fw);
      p = (x-v)*q - (x-w)*t;
      q = 2*(q-t);

      if( q > 0 )			// Formulas above computed new_step
	p = -p;				// = p/q with wrong sign (on purpose).
      else				// Correct this, but in such a way so
	q = -q;				// that q would be positive

      if( fabs(p) < fabs(new_step*q) &&	// If x+p/q falls in [a,b] and is not
	 p > q*(a-x+2*tol_act) &&	// too close to a and b, and isn't
	 p < q*(b-x-2*tol_act)  )       // too large, it is accepted
	   new_step = p/q;
					// If p/q is too large then the
					// gold section procedure would
					// reduce [a,b] to larger extent
    }

    if( fabs(new_step) < tol_act )	// Adjust the step to be not less
      if( new_step > 0 )		// than tolerance
	new_step = tol_act;
      else
	new_step = -tol_act;

				// Obtain the next approximation to min
    				// and reduce the encompassing interval
    register double t = x + new_step;	// Tentative point for the min
    register double ft = (*f)(t);
    if( ft <= fx )
    {                                 	// t is a better approximation
      if( t < x )			// Reduce the interval so that
	b = x;                        	// t would fall within it
      else
	a = x;
      
      v = w;  w = x;  x = t;		// Assign the best approx to x
      fv=fw;  fw=fx;  fx=ft;
    }
    else                              	// x remains the better approx
    {
      if( t < x )			// Reduce the interval encompassing x
	a = t;                   
      else
	b = t;
      
      if( ft <= fw || w==x )
      {
	v = w;  w = t;
	fv=fw;  fw=ft;
      }
      else if( ft<=fv || v==x || v==w )
	v = t, fv = ft;
    }
  }		// ===== End of loop =====

}
