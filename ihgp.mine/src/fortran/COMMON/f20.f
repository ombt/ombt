c
c test of bisection method for finding roots.
c
      program mybisect
c
c variables
c 
      implicit none
c
      real a, b, ep, r
      common /BIGTEST/ a, b, ep, r
      real f1, f2, f3
      external f1, f2, f3
      logical bisect
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
      if (.not.bisect(f1, a, b, ep, r)) then
          stop 'bisect failed'
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
      if (.not.bisect(f2, a, b, ep, r)) then
          stop 'bisect failed'
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
      if (.not.bisect(f3, a, b, ep, r)) then
          stop 'bisect failed'
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
c functions for bisection method.
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
