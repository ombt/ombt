// This may look like C code, but it is really -*- C++ -*-
/*
 ************************************************************************
 *
 *			  Numerical Math Package
 *		Hook-Jeevse multidimensional minimization
 *
 * Synopsis
 *	double hjmin(b,h,funct)
 *	Vector& b 			should be specified to contain the
 *					initial guess to the point of minimum.
 *					On return, it contains the location
 *					of the minimum the function has found
 *	Vector& h			Initial values for steps along
 *					each direction
 *					On exit contains the final steps
 *					before the termination
 *	double f(const Vector& x)	Procedure to compute a function
 *					value at the specified point
 *
 * The function returns the value of the minimizing function f() at the
 * point of the found minimum.
 *
 * An alternative double hjmin(b,h0,funct) where h0 is a double const
 * uses the same initial steps along each direction. No final steps are
 * reported, though.
 *
 * Algorithm
 *	Hook-Jeevse method of direct search for a function minimum
 *	The method is of the 0. order (i.e. requiring no gradient computation)
 *	See
 *	B.Bondi. Methods of optimization. An Introduction - M.,
 *	"Radio i sviaz", 1988 - 127 p. (in Russian)
 *
 * $Id$
 *
 ************************************************************************
 */


#include "LinAlg.h"
#include "math_num.h"
#include "std.h"

/*
 *------------------------------------------------------------------------
 *		Class to operate on the points in the space (x,f(x))
 */

class FPoint
{
  Vector& x;				// Point in the function domain
  double fval;				// Function value at the point
  double (*fproc)(const Vector& x);	// Procedure to compute the function
					// value
  const bool free_x_on_destructing;	// The flag is needed to free the 
					// dynamic memory allocated when
					// x rather than reference to x has
					// been given to construct FPoint

public:
  FPoint(Vector& b, double (*f)(const Vector& x));
  FPoint(const FPoint& fp);
  ~FPoint();

  FPoint& operator = (const FPoint& fp);

  double f() const			{ return fval; }

  double fiddle_around(const Vector& h);// Examine the function in the
					// neighborhood of the current point.
					// h defines the radius of the region

					// Proceed in direction the function
					// seems decline
  friend void update_in_direction(FPoint& from, FPoint& to);

					// Decide whether the region embracing
					// the local min is small enough
  bool is_step_relatively_small(const Vector& h, const double tau);
};

				// Constructor FPoint from array b
inline FPoint::FPoint(Vector& b, double (*f)(const Vector& x))
	: x(b), fproc(f), free_x_on_destructing(false)
{
  fval = (*fproc)(x);
}

				// Constructor by example
FPoint::FPoint(const FPoint& fp)
	: x(*(new Vector(fp.x))), free_x_on_destructing(true)
{
  x = fp.x;
  fval = fp.fval;
  fproc = fp.fproc;
}
				// Destructor
FPoint::~FPoint()
{
  if( free_x_on_destructing )
    delete &x;
}

				// Assignment; fproc is assumed the same and
				// is not copied
inline FPoint& FPoint::operator = (const FPoint& fp)
{
  x = fp.x;
  fval = fp.fval;
  return *this;
}

/*
 * Examine the function f in the vicinity of the current point
 * by making tentative steps fro/back along each coordinate.
 * Should function decrease, the point is updated to locate the
 * new local min.
 * Examination() returns the minimal function value found in
 * the region.
 *
 */
double FPoint::fiddle_around(const Vector& h)
{
			// Perform a step along a coordinate
  for(register int i=x.q_lwb(); i<=x.q_upb(); i++)
  {
    const double hi = h(i);
    register double xi_old = x(i);  	// Old value of x[i]
    register double fnew;

    if( x(i) = xi_old + hi, fnew = (*fproc)(x), fnew < fval )
      fval = fnew;			// Step caused f to decrease, OK
    else if( x(i) = xi_old - hi, fnew = (*fproc)(x), fnew < fval )
      fval = fnew;
    else				// No function decline has been
      x(i) = xi_old;                	// found along this coord, back up
  }
  return fval;
}                                                

				// Proceed in the direction the function
				// seems to fall
				// to_new = (to - from) + to
				// from = to (before modification)
void update_in_direction(FPoint& from, FPoint& to)
{
  register int i;
  for(i=(from.x).q_lwb(); i<=(from.x).q_upb(); i++)
  {
    register double t = (to.x)(i);
    (to.x)(i)  += (t - (from.x)(i));
    (from.x)(i) = t;
  }
  from.fval = to.fval;
  to.fval = (*(to.fproc))(to.x);
}

				// Estimate if the point of minimum has
				// been located accurately enough
bool FPoint::is_step_relatively_small(const Vector& h, const double tau)
{
  register bool it_is_small = true;
  register int i;
  for(i=h.q_lwb(); it_is_small && i<=h.q_upb(); i++)
    it_is_small &= ( h(i) /(1 + fabs(x(i))) < tau );
  return it_is_small;
}

/*
 *------------------------------------------------------------------------
 *			    Root module
 */

double hjmin(Vector& b, Vector& h, double (*ff)(const Vector& x))
{
				// Function Parameters
  const double tau = 10*EPSILON;        // Termination criterion
  const double threshold = 1e-8;        // Threshold for the function
                                        // decay to be treated as
                                        // significant
  const double step_reduce_factor = 10;   

  are_compatible(b,h);

  FPoint pmin(b,ff);			// Point of min
  FPoint pbase(pmin);			// Base point

  for(;;)			// Main iteration loop
  {                     	// pmin is the approximation to min so far
    if( pbase.fiddle_around(h) < pmin.f() - threshold )
    {   			      	// Function value dropped significantly
      do                              	// from pmin to the point pbase
	update_in_direction(pmin,pbase);// Keep going in the same direction
      while( pbase.fiddle_around(h) < pmin.f() - threshold ); // while it works
      pbase = pmin;			// Save the best approx found
    }
    else                               	// Function didn't fall significantly
      if(                               // upon wandering around pbas
	 h *= 1/step_reduce_factor,	// Try to reduce the step then
	 pbase.is_step_relatively_small(h,tau) )
	return pmin.f();
  }
}

	    		// The same as above with the only difference
	    		// initial steps are given to be the same
	    		// along every direction. The final steps
	    		// aren't reported back though
double hjmin(Vector& b,	const double h0, double (*f)(const Vector& x))
{
  Vector h(b.q_lwb(),b.q_upb()); h = h0;
  return hjmin(b,h,f);
}
