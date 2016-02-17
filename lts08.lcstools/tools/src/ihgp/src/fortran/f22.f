c
c test of newton method for finding roots.
c
      program bisect
c
c variables
c 
      implicit none
c
      real a, b, r, ep
      real f1, f2, f3, f1p, f2p, f3p
      external f1, f2, f3, f1p, f2p, f3p
      logical newton
c
c enter start and end of interval.
c
      print *, 'enter [a, b] ...'
      read *, a, b
      print *, 'enter epsilon ...'
      read *, ep
c
c find root.
c
      if (.not.newton(f1, f1p, a, b, ep, r)) then
          stop 'newton failed'
      endif
c
c print root.
c
      print *, 'root is ... ', r
c
c enter start and end of interval.
c
      print *, 'enter [a, b] ...'
      read *, a, b
      print *, 'enter epsilon ...'
      read *, ep
c
c find root.
c
      if (.not.newton(f2, f2p, a, b, ep, r)) then
          stop 'newton failed'
      endif
c
c print root.
c
      print *, 'root is ... ', r
c
c enter start and end of interval.
c
      print *, 'enter [a, b] ...'
      read *, a, b
      print *, 'enter epsilon ...'
      read *, ep
c
c find root.
c
      if (.not.newton(f3, f3p, a, b, ep, r)) then
          stop 'newton failed'
      endif
c
c print root.
c
      print *, 'root is ... ', r
c
c all done
c
      stop
      end
c
c functions for newton method.
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
      real function f3(x)
      real x
      f3 = tan(x) + x + 10.0
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
      real function f3p(x)
      real x
      f2p = 1.0/(cos(x)**2) + 1.0
      return
      end


