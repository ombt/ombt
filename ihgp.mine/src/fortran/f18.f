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
	integer maxsize, size, idx
	parameter(maxsize=100)
	logical status
	real x(maxsize), y(maxsize)
	real a, b, x2
c
c declare function
c
	logical linearfit
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
	status = linearfit(y, x, size, a, b, x2)
	if (.not.status) then
		stop 'linearfit failed.'
	end if
c
c show data.
c
	print *, 'a = ', a, ' b = ', b, ' x2 = ', x2
	do idx=1, size, 1
		print *, ' x = ', x(idx), 
     1                   ' y = ', y(idx), 
     2			 ' f(x) = ', (a + b*x(idx)), 
     3			 ' delta = ', abs(y(idx)-a-b*x(idx))
	end do
c
c all done
c
	stop 'all done with test linearfit'
	end
