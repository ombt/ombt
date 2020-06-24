c
c test of gaussian elimination routine.
c
	program gauss
c
c no implicit
c
	implicit none
c
c local variables
c
	integer maxsize
	parameter(maxsize=100)
	logical status
	integer size, row, col
	real x(maxsize), y(maxsize), array(maxsize, maxsize)
c
c declare function
c
	logical gaussian
c
c get size of array from user.
c
	print *, 'enter matrix size '
	read *, size
c
c read in matrix and y-values.
c
	print *, 'enter matrix elements:'
	read *, ((array(row, col), col = 1, size), row = 1, size)
	print *, 'enter y-values:'
	read *, (y(col), col = 1, size)
c
c solve set of equations.
c
	status = gaussian(array, y, x, maxsize, size)
	if (.not.status) then
		stop 'gaussian failed.'
	end if
c
c show x-values.
c
	do 100 col=1, size, 1
		print *, 'x(', col, ') = ', x(col)
 100	continue
c
c all done
c
	stop 'all done with test gaussian'
	end
