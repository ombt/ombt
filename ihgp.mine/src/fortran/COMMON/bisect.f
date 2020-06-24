c
c find a root using a bisection method.
c
cccccccccccccccccccccccccccccccccccccccccccccc`
c
c2345678901234567890
c
      logical function bisect(f)
c
c variables
c
      external f
      real f, a, b, ep, r
      real c, u, v, w
c
      common /BIGTEST/ a, b, ep, r
c
c put a limit to the number of iterations
c
      integer maxiter, iter
      parameter(maxiter = 100)
c
c check arguments
c
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
          print *, '[a, b] = [', a, ',', b, ']'
          print *, 'f(a) = ', u, ' f(b) = ', v
c
c new midpoint
c
          c = (a + b)/2.0
          w = f(c)
          print *, 'c = ', c, ' f(c) = ', w
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
              bisect = .false. 
              return
          endif
          goto 100
      endif
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
