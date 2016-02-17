c
c test of root finding methods.
c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      program roots
c
c variables
c 
      implicit none
c
      integer iters
      real savea, saveb, a, b, r, saveep, ep
      real f1, f2, f1p, f2p
      external f1, f2, f1p, f2p
      logical bisect, newton, secant
c
c enter start and end of interval.
c
      print *, 'f1: enter [a, b] ...'
      read *, savea, saveb
      print *, 'f1: enter epsilon ...'
      read *, saveep
c
c find root using bisect, newton and secant method.
c
      a = savea
      b = saveb
      ep = saveep
c
      if (.not.bisect(f1, a, b, ep, r, iters)) then
          print *, 'bisect failed'
      else
          print *, 'bisect: root is ... ', r
      endif
      print *, 'bisect: number of iterations is ...', iters
c
      a = savea
      b = saveb
      ep = saveep
c
      if (.not.newton(f1, f1p, a, b, ep, r, iters)) then
          print *, 'newton failed'
      else
          print *, 'newton: root is ... ', r
      endif
      print *, 'newton: number of iterations is ...', iters
c
      a = savea
      b = saveb
      ep = saveep
c
      if (.not.secant(f1, a, b, ep, r, iters)) then
          print *, 'secant failed'
      else
          print *, 'secant: root is ... ', r
      endif
      print *, 'secant: number of iterations is ...', iters
c
c enter start and end of interval.
c
      print *, 'f2: enter [a, b] ...'
      read *, savea, saveb
      print *, 'f2: enter epsilon ...'
      read *, saveep
c
c find root using bisect, newton and secant method.
c
      a = savea
      b = saveb
      ep = saveep
c
      if (.not.bisect(f2, a, b, ep, r, iters)) then
          print *, 'bisect failed'
      else
          print *, 'bisect: root is ... ', r
      endif
      print *, 'bisect: number of iterations is ...', iters
c
      a = savea
      b = saveb
      ep = saveep
c
      if (.not.newton(f2, f2p, a, b, ep, r, iters)) then
          print *, 'newton failed'
      else
          print *, 'newton: root is ... ', r
      endif
      print *, 'newton: number of iterations is ...', iters
c
      a = savea
      b = saveb
      ep = saveep
c
      if (.not.secant(f2, a, b, ep, r, iters)) then
          print *, 'secant failed'
      else
          print *, 'secant: root is ... ', r
      endif
      print *, 'secant: number of iterations is ...', iters
c
c all done
c
      stop
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c functions for root finding.
c
      real function f1(x)
      real x
      f1 = (x-3.0)**3 + 3.0
      return
      end
c
      real function f2(x)
      real x
      f2 = (x-5.0)**5 + 5.0
      return
      end
c
      real function f1p(x)
      real x
      f1p = 3.0*(x-3.0)**2
      return
      end
c
      real function f2p(x)
      real x
      f2p = 5.0*(x-5.0)**4
      return
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c find a root using a bisection method.
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c2345678901234567890
c
      logical function bisect(f, a, b, ep, r, iters)
c
c variables
c
      external f
      real f, a, b, ep
      real c, u, v, w
      integer iters
c
c put a limit to the number of iterations
c
      integer maxiter, iter
      parameter(maxiter = 100)
c
c check arguments
c
      iters = 0
      if (a.ge.b.or.ep.le.0.0) then
          print *, 'interval is [b, a] or epsilon <= 0.0.'
          bisect = .false.
          return
      endif
c
c verify there is a root.
c
      u = f(a)
      v = f(b)
      if (u*v.gt.0.0) then
          print *, 'interval [a, b] does not contain a root.'
          bisect = .false.
          return
      endif
c
c start loop for searching for root.
c
      iter = 1
 100  if (abs(a-b).gt.ep.and.iter.le.maxiter) then
c
c new midpoint
c
          c = (a + b)/2.0
          w = f(c)
c
c check where root is.
c 
          if (u*w.le.0.0) then
              b = c
              v = w
          else if (v*w.le.0.0) then
              a = c
              u = w
          else
              print *, 'no roots found.'
              iters = iter
              bisect = .false. 
              return
          endif
          iter = iter + 1
          goto 100
      endif
c
      iters = iter
c
c did we find a root?
c
      if (iter.gt.maxiter) then
          print *, 'no root was found.'
          bisect = .false.
      else
          r = (a+b)/2.0
          bisect = .true.
      endif
c
c all done
c
      return
      end
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c find a root using newton's method.
c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c2345678901234567890
c
      logical function newton(f, fp, a, b, ep, r, iters)
c
      implicit none
c
c variables
c
      external f, fp
      real f, fp, a, b, ep, r
      real x, y, yp
      integer iters
c
c put a limit to the number of iterations
c
      integer maxiter, iter
      parameter(maxiter = 100)
c
c verify there is a root.
c
      iters = 0
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
              iters = iter
              newton = .true.
              return
          endif
          if (yp.eq.0.0) then
              print *, 'division by zero.'
              iters = iter
              newton = .false.
              return
          endif
          x = x - y/yp
 100  continue
c
      iters = iter
c
c no root was found.
c
      print *, 'too many iterations.'
      newton = .false.
      return
      end
c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c find a root using secant method.
c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c2345678901234567890
c
      logical function secant(f, a, b, ep, r, iters)
c
      implicit none
c
c variables
c
      external f
      real f, a, b, ep, r, df
      real x, xa, xb
      integer iters
c
c put a limit to the number of iterations
c
      integer maxiter, iter
      parameter(maxiter = 100)
c
c verify there is a root.
c
      iters = 0
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
          df = (f(xb) - f(xa))
          if (df.ne.0.0) then
              x = xb - (xb - xa)*f(xb)/df
          else
              print *, 'division by zero'
              iters = iter
              secant = .false.
              return
          endif
          if (abs(f(x)).lt.ep) then
              r = x
              iters = iter
              secant = .true.
              return
          endif
          xa = xb
          xb = x
 100  continue
c
      iters = iter
c
c no root was found.
c
      secant = .false.
      return
      end
