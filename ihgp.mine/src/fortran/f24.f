c
c graph test
c
      program testg
c
      implicit none
c
      external f
      intrinsic sin, cos, exp
c
      logical plotf, status
c
      status = plotf(sin, -3.14159, 3.14159, 'angle(radians)',
     1                    -1.0, 1.0, 'sin(x)', 'linear')
      if (.not.status) then
          stop 'plotf failed.'
      endif
c
      status = plotf(cos, -3.14159, 3.14159, 'angle(radians)',
     1                    -1.0, 1.0, 'cos(x)', 'linear')
      if (.not.status) then
          stop 'plotf failed.'
      endif
c
      status = plotf(exp, -5.0, 5.0, 'x',
     1                    0.1, 1000.0, 'exp(x)', 'log')
      if (.not.status) then
          stop 'plotf failed.'
      endif
c
      status = plotf(f, -5.0, 5.0, 'x',
     1                  0.0, 50.0, 'x**2', 'sqrt')
      if (.not.status) then
          stop 'plotf failed.'
      endif
c
      status = plotf(f, -5.0, 5.0, 'x',
     1                  0.0, 50.0, 'x**2', 'linear')
      if (.not.status) then
          stop 'plotf failed.'
      endif
c
      stop
      end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
      real function f(x)
      real x
      f = x*x
      return
      end
