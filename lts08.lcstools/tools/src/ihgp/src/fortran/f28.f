c
c test of least-squares linear fit.
c
	program linearfit
c
c no implicit
c
	implicit none
c
c local variables
c
	character*10 ans
	integer maxsize, size, idx
	parameter(maxsize=100)
	logical status
	real x(maxsize), y(maxsize)
	real a, b, x2
c
c declare functions
c
	logical linearfit, arbexpfit, expfit, powerfit
c
c get type of fit from user.
c
 10	print *, 'y = a + b*x    : (l)inear'
	print *, 'y = a*b**x     : (p)ower'
	print *, 'y = a*exp(b*x) : (e)xponential'
	print *, 'y = a*x**b     : pol(y)nomial'
	print *, 'enter type of fit: '
	read *, ans
	if (ans(1:1).ne.'l'.and.ans(1:1).ne.'p'.and.
     c      ans(1:1).ne.'e'.and.ans(1:1).ne.'y') then
		print *, 'invalid choice. try again.'
		goto 10
	end if
c
c get size of array from user.
c
	print *, 'enter number of points '
	read *, size
c
c read in matrix and y-values.
c
	print *, 'enter (x, y) point:'
	read *, (x(idx), y(idx), idx = 1, size)
c
c solve set of equations.
c
	if (ans(1:1).eq.'l') then
		status = linearfit(y, x, size, a, b, x2)
		if (.not.status) then
			stop 'linearfit failed.'
		end if
		print *, 'a = ', a, ' b = ', b, ' x2 = ', x2
		do idx=1, size, 1
			print *, ' x = ', x(idx), 
     1                   	 ' y = ', y(idx), 
     2			 	 ' f(x) = ', (a + b*x(idx)), 
     3			 	 ' delta = ', abs(y(idx)-a-b*x(idx))
		end do
	else if (ans(1:1).eq.'p') then
		status = arbexpfit(y, x, size, a, b, x2)
		if (.not.status) then
			stop 'arbexpfit failed.'
		end if
		print *, 'a = ', a, ' b = ', b, ' x2 = ', x2
		do idx=1, size, 1
			print *, ' x = ', x(idx), 
     1                   	 ' y = ', y(idx), 
     2			 	 ' f(x) = ', (a*b**x(idx)), 
     3			 	 ' delta = ', abs(y(idx)-a*b**x(idx))
		end do
	else if (ans(1:1).eq.'e') then
		status = expfit(y, x, size, a, b, x2)
		if (.not.status) then
			stop 'expfit failed.'
		end if
		print *, 'a = ', a, ' b = ', b, ' x2 = ', x2
		do idx=1, size, 1
			print *, ' x = ', x(idx), 
     1                   	 ' y = ', y(idx), 
     2			 	 ' f(x) = ', (a*exp(b*x(idx))), 
     3			 	 ' delta = ', abs(y(idx)-a*exp(b*x(idx)))
		end do
	else
		status = powerfit(y, x, size, a, b, x2)
		if (.not.status) then
			stop 'powerfit failed.'
		end if
		print *, 'a = ', a, ' b = ', b, ' x2 = ', x2
		do idx=1, size, 1
			print *, ' x = ', x(idx), 
     1                   	 ' y = ', y(idx), 
     2			 	 ' f(x) = ', (a*x(idx)**b), 
     3			 	 ' delta = ', abs(y(idx)-a*x(idx)**b)
		end do
	endif
c
c all done
c
	stop 'all done with test linearfit'
	end
