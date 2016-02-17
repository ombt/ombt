c
c test of exp function
c
      program expfn
c
c variables
c 
      implicit none
c
      integer nterms
      real x, prec, value, myexp
c
c get x and precision
c
      print *, 'enter x '
      read *, x
      print *, 'enter precision:'
      read *, prec
c
c do exp calculation
c
      value = myexp(x, prec, nterms)
      print *, 'exp(', x, ') = ', value
      print *, 'precision used = ', prec
      print *, 'number of terms used = ', nterms
c
c all done
c
      stop
      end
