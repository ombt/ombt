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
      mysine = 0.0
      nterms = 1
      term = sineterm(angle, nterms)
 100  continue
          mysine = mysine + term
          nterms = nterms + 1
          term = sineterm(angle, nterms)
      if (abs(term).gt.prec) goto 100
      nterms = nterms - 1
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
c calculate a term
c
      sineterm = (angle**(2*nterms-1))/factorial(2*nterms-1)
      if (mod(nterms, 2).eq.0) sineterm = -1.0*sineterm
c
c all done
c
      return
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
      real angle, prec
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
c start calculating
c
      mycosine = 0.0
      nterms = 1
      term = cosineterm(angle, nterms)
 100  continue
          mycosine = mycosine + term
          nterms = nterms + 1
          term = cosineterm(angle, nterms)
      if (abs(term).gt.prec) goto 100
      nterms = nterms - 1
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
c calculate a term
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
