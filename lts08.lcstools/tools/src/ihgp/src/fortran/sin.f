c
c test of sine function
c
      program mysine
c
c variables
c 
      implicit none
c
      real pi
      parameter (pi=3.141593)
c
      integer nterms
      real angle, prec, radians
      real mysine, value
c
c get angle and precision
c
      print *, 'enter angle in degrees:'
      read *, angle
      print *, 'enter precision:'
      read *, prec
c
c do calculation
c
      radians = angle*2.0*pi/360.0
      value = mysine(radians, prec, nterms)
c
c print out results
c
      print *, 'sine(', angle, ') = ', value
      print *, 'number of terms used = ', nterms
c
c all done
c
      stop
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c function for calculating the sine function.
c
c23456789012345678989012345678901234567890
c
      real function mysine(angle, prec, nterms)
c
c declare arguments
c
      implicit none
c
      integer nterms
      real angle, prec
c
c local variables
c
      real sineterm, term
c
c check if precision is zero.
c
      prec = abs(prec)
      if (prec.eq.0.0) prec = 0.000001
c
c start calculating
c
      nterms = 1
      term = sineterm(angle, nterms)
      mysine = term
 100  if (abs(term).gt.prec) then
          nterms = nterms + 1
          term = sineterm(angle, nterms)
	  mysine = mysine + term
          goto 100
      endif
c
c all done
c
      return
      end
c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c calculate a term in a sine series.
c
      real function sineterm(angle, nterms)
c
c variables
c
      implicit none
c
      real angle, factorial
      integer nterms
c
c calculate a a term
c
      sineterm = (angle**(2*nterms-1))/factorial(2*nterms-1)
      if (mod(nterms, 2).eq.0) sineterm = -1.0*sineterm
c
c all done
c
      return
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c calculate a factorial
c
      real function factorial(n)
c
c variables
c
      implicit none
c
      integer i, n
c
c initialize
c
      factorial = 1.0
c
c check for special cases
c
      if (n.eq.0.or.n.eq.1) then
          factorial = 1.0
          return
      endif
c
c general case
c 
      do 100 i=2, n, 1
          factorial = factorial*float(i)
 100  continue
c
c all done
c
      return
      end
