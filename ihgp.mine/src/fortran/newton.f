c
c find a root using newton's method.
c
cccccccccccccccccccccccccccccccccccccccccccccc`
c
c2345678901234567890
c
      logical function newton(f, fp, a, b, ep, r)
c
      implicit none
c
c variables
c
      external f, fp
      real f, fp, a, b, ep, r
      real x, y, yp
c
c put a limit to the number of iterations
c
      integer maxiter, iter
      parameter(maxiter = 100)
c
c verify there is a root.
c
      if (f(a)*f(b).gt.0.0) then
          print *, 'interval [a, b] does not contain a root.'
          newton = .false.
          return
      endif
c
c initial guess for root
c
      x = (a + b)/2.0
c
c start loop for searching for root.
c
      do 100 iter=1, maxiter, 1
          y = f(x)
          yp = fp(x)
          if (abs(y).lt.ep) then
              r = x
              newton = .true.
              return
          endif
          if (yp.eq.0.0) then
              print *, 'division by zero.'
              newton = .false.
              return
          endif
          x = x - y/yp
 100  continue
c
c no root was found.
c
      print *, 'too many iterations.'
      newton = .false.
      return
      end
