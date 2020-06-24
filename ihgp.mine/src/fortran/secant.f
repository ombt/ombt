c
c find a root using secant method.
c
cccccccccccccccccccccccccccccccccccccccccccccc`
c
c2345678901234567890
c
      logical function secant(f, a, b, ep, r)
c
      implicit none
c
c variables
c
      external f
      real f, a, b, ep, r, df
      real x, xa, xb
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
          secant = .false.
          return
      endif
c
c start loop for searching for root.
c
      xa = a
      xb = b
      do 100 iter=1, maxiter, 1
c
c         print *, '======================'
c         print *, 'iter = ', iter
c         print *, 'xa, f(xa) = ', xa, f(xa)
c         print *, 'xb, f(xb) = ', xb, f(xb)
c
          df = (f(xb) - f(xa))
          if (df.ne.0.0) then
              x = xb - (xb - xa)*f(xb)/df
          else
              print *, 'division by zero'
              secant = .false.
              return
          endif
          if (abs(f(x)).lt.ep) then
              r = x
              secant = .true.
              return
          endif
          xa = xb
          xb = x
 100  continue
c
c no root was found.
c
      secant = .false.
      return
      end
