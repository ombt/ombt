c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c function for calculating the exp function.
c
c23456789012345678989012345678901234567890
c
      real function myexp(x, prec, nterms)
c
c declare arguments
c
      implicit none
c
      integer nterms
      real x, prec
c
c local variables
c
      real expterm, term
c
c check if precision is zero.
c
      prec = abs(prec)
      if (prec.eq.0.0) prec = 0.000001
c
c start calculating
c
      myexp = 0.0
      nterms = 1
      term = expterm(x, nterms)
 100  continue
	  myexp = myexp + term
          nterms = nterms + 1
          term = expterm(x, nterms)
      if (abs(term/myexp).gt.prec) goto 100
c
c all done
c
      return
      end
c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c calculate a term in a exp series.
c
      real function expterm(x, nterms)
c
c variables
c
      implicit none
c
      real x
      integer nterms, n
c
c calculate a term
c
      expterm = 1.0
      if (nterms.ge.2) then
          do n=2, nterms, 1
              expterm = expterm*(x/float(n-1))
          end do
      end if
c
c all done
c
      return
      end
