// This may look like C code, but it is really -*- C++ -*-
/*
 ************************************************************************
 *
 * 		         Fast Fourier Transform
 *
 *		Return the transformation results in the form
 *			   the user wants them
 *
 * $Id$
 *
 ************************************************************************
 */

#include "fft.h"
#include "std.h"

/*
 *-----------------------------------------------------------------------
 *	     Give a real/imaginaire part / absolute value
 *			of the complex transform
 */

void FFT::real(Vector& xf_re)		// [0:N-1] vector
{
  if( xf_re.q_lwb() != 0 || xf_re.q_upb() != N-1 )
    _error("Vector [%d:%d] isn't fit for placing the resulting transform.\n",
	   "[%d:%d] vector was expected",
	   xf_re.q_lwb(), xf_re.q_upb(), 0, N-1);

  register int i;
  register const Complex * ap;
  for(i=0,ap=A; i<N; i++)
    xf_re(i) = ::real(*ap++);
  assert( ap == A_end );
}

void FFT::imag(Vector& xf_im)		// [0:N-1] vector
{
  if( xf_im.q_lwb() != 0 || xf_im.q_upb() != N-1 )
    _error("Vector [%d:%d] isn't fit for placing the resulting transform.\n",
	   "[%d:%d] vector was expected",
	   xf_im.q_lwb(), xf_im.q_upb(), 0, N-1);

  register int i;
  register const Complex * ap;
  for(i=0,ap=A; i<N; i++)
    xf_im(i) = ::imag(*ap++);
  assert( ap == A_end );
}

void FFT::abs(Vector& xf_abs)		// [0:N-1] vector
{
  if( xf_abs.q_lwb() != 0 || xf_abs.q_upb() != N-1 )
    _error("Vector [%d:%d] isn't fit for placing the resulting transform.\n",
	   "[%d:%d] vector was expected",
	   xf_abs.q_lwb(), xf_abs.q_upb(), 0, N-1);

  register int i;
  register const Complex * ap;
  for(i=0,ap=A; i<N; i++)
    xf_abs(i) = ::abs(*ap++);
  assert( ap == A_end );
}

/*
 *-----------------------------------------------------------------------
 *	       Give only a half of the resulting transform
 */


void FFT::real_half(Vector& xf_re)	// [0:N/2-1] vector
{
  if( xf_re.q_lwb() != 0 || xf_re.q_upb() != N/2-1 )
    _error("Vector [%d:%d] isn't fit for placing the resulting transform.\n",
	   "[%d:%d] vector was expected",
	   xf_re.q_lwb(), xf_re.q_upb(), 0, N/2-1);

  register int i;
  register const Complex * ap;
  for(i=0,ap=A; i<N/2; i++)
    xf_re(i) = ::real(*ap++);
  assert( ap + N/2 == A_end );
}

void FFT::imag_half(Vector& xf_im)	// [0:N/2-1] vector
{
  if( xf_im.q_lwb() != 0 || xf_im.q_upb() != N/2-1 )
    _error("Vector [%d:%d] isn't fit for placing the resulting transform.\n",
	   "[%d:%d] vector was expected",
	   xf_im.q_lwb(), xf_im.q_upb(), 0, N/2-1);

  register int i;
  register const Complex * ap;
  for(i=0,ap=A; i<N/2; i++)
    xf_im(i) = ::imag(*ap++);
  assert( ap + N/2 == A_end );
}

void FFT::abs_half(Vector& xf_abs)	// [0:N/2-1] vector
{
  if( xf_abs.q_lwb() != 0 || xf_abs.q_upb() != N/2-1 )
    _error("Vector [%d:%d] isn't fit for placing the resulting transform.\n",
	   "[%d:%d] vector was expected",
	   xf_abs.q_lwb(), xf_abs.q_upb(), 0, N/2-1);

  register int i;
  register const Complex * ap;
  for(i=0,ap=A; i<N/2; i++)
    xf_abs(i) = ::abs(*ap++);
  assert( ap + N/2 == A_end );
}

/*
 *-----------------------------------------------------------------------
 *		Perform sin/cos transforms of a real function
 *  			as a postprocessing of FFT
 *
 * Sine-transform:   F(k) = Integrate[ f(x) sin(kx) dx ], x = 0..Infinity
 * Cosine-transform: F(k) = Integrate[ f(x) cos(kx) dx ], x = 0..Infinity
 * Inverse
 *   sin-transform:  f(x) = 2/pi Integrate[ F(k) sin(kx) dk ], k = 0..Infinity
 *   cos-transform:  f(x) = 2/pi Integrate[ F(k) cos(kx) dk ], k = 0..Infinity
 *
 * Function f(x) is tabulated over the uniform grid xj = j*dr, j=0..n-1
 * Function F(k) is tabulated over the uniform grid kj = j*dk, j=0..n-1
 * 							n=N/2
 * Source and destination arguments of the functions below may point to
 * the same vector (in that case, transform is computed inplace)
 */

void FFT::sin_transform(Vector& F, const Vector& f)
{
  are_compatible(F,f);
  input_pad0(f);

  register int j;
  register const Complex * ap;
  for(j=0,ap=A; j<N/2; j++)
    F(j) = - ::imag(*ap++) * dr;
  assert( ap + N/2 == A_end );
}

void FFT::cos_transform(Vector& F, const Vector& f)
{
  are_compatible(F,f);
  input_pad0(f);

  register int j;
  register const Complex * ap;
  for(j=0,ap=A; j<N/2; j++)
    F(j) = ::real(*ap++) * dr;
  assert( ap + N/2 == A_end );
}


void FFT::sin_inv_transform(Vector& f, const Vector& F)
{
  are_compatible(F,f);
  input_pad0(F);

  register int j;
  register const Complex * ap;
  for(j=0,ap=A; j<N/2; j++)
    f(j) = - ::imag(*ap++) * 4/N/dr;	// 2/pi * dk = 2/pi * 2pi/N/dr
  assert( ap + N/2 == A_end );
}

void FFT::cos_inv_transform(Vector& f, const Vector& F)
{
  are_compatible(F,f);
  input_pad0(F);

  register int j;
  register const Complex * ap;
  for(j=0,ap=A; j<N/2; j++)
    f(j) = ::real(*ap++) * 4/N/dr;
  assert( ap + N/2 == A_end );
}

