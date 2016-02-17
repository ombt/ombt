c
c test of cosine function
c
      program mycosine
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
      real mycosine, value
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
      value = mycosine(radians, prec, nterms)
c
c print out results
c
      print *, 'cosine(', angle, ') = ', value
      print *, 'number of terms used = ', nterms
c
c all done
c
      stop
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c function for calculating the cosine function.
c
c23456789012345678989012345678901234567890
c
      real function mycosine(angle, prec, nterms)
c
c declare arguments
c
      implicit none
c
      integer nterms
      real angle, angle2, prec, pi
      parameter(pi=3.14159265)
c
c local variables
c
      real cosineterm, term
c
c check if precision is zero.
c
      prec = abs(prec)
      if (prec.eq.0.0) prec = 0.000001
c
c map angle into the range from -2*pi to 2*pi.
c
      angle2 = angle
      if (abs(angle2).gt.2*pi) then
          nterms = int(angle2/(2.0*pi))
          angle2 = angle2 - 2.0*nterms*pi
      endif
c
c start calculating
c
      nterms = 1
      term = cosineterm(angle2, nterms)
      mycosine = term
 100  if (abs(term).gt.prec) then
          nterms = nterms + 1
          term = cosineterm(angle2, nterms)
	  mycosine = mycosine + term
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
c calculate a term in a cosine series.
c
      real function cosineterm(angle, nterms)
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
      cosineterm = (angle**(2*(nterms-1)))/factorial(2*(nterms-1))
      if (mod(nterms, 2).eq.0) cosineterm = -1.0*cosineterm
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
