c
c test of secant method for finding roots.
c
      program secant
c
c variables
c 
      implicit none
c
      real a, b, r, ep
      real f1, f2
      external f1, f2
      logical secant
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
      if (.not.secant(f1, a, b, ep, r)) then
          stop 'secant failed'
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
      if (.not.secant(f2, a, b, ep, r)) then
          stop 'secant failed'
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
c functions for secant method.
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
