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
	integer maxsize, nmax, n
	parameter(maxsize=100)
	logical status
	integer size, row, col
	real x(maxsize), y(maxsize), array(maxsize, maxsize)
c
c declare function
c
	logical gaussian
c
c how may cases to solve.
c
	print *, 'enter number of problems:'
	read *, nmax
c
	do n=1, nmax, 1
c
c get size of array from user.
c
		print *, '============== matrix ', n, ' ==============='
		print *, 'enter matrix size '
		read *, size
c
c read in matrix and y-values.
c
		print *, 'enter matrix elements:'
		read *, ((array(row, col), col = 1, size), row = 1, size)
		print *, 'enter y-values:'
		read *, (y(row), row = 1, size)
c
		print *, 'matrix is ...'
		do row=1, size
			print *, (array(row, col), col = 1, size)
		end do
		print *, 'y-vector is ...'
		print *, (y(row), row = 1, size)
c
c solve set of equations.
c
		status = gaussian(array, y, x, maxsize, size)
		if (.not.status) then
			print *, 'matrix is ...'
			do row=1, size
				print *, (array(row, col), col = 1, size)
			end do
			print *, 'y-vector is ...'
			print *, (y(row), row = 1, size)
			stop 'gaussian failed.'
		end if
c
c show x-values.
c
		do col=1, size, 1
			print *, 'x(', col, ') = ', x(col)
		end do
	end do
c
c all done
c
	stop 'all done with test gaussian'
	end
