c
c test passing arrays to subroutines.
c
	program arrays
c
c no implicit
c
	implicit none
c
c local variables
c
	integer irow, icol
	integer nrows, ncols
	real array(100, 100)
c
c get size of array from user.
c
	print *, 'enter matrix size, rows and columns'
	read *, nrows, ncols
c
c get data from user
c
	read *, ((array(irow, icol), icol = 1, ncols), irow = 1, nrows)
c
c dump data out
c
	call dumpdata(array, 100, 100, nrows, ncols)
c
c all done
c
	end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
	subroutine dumpdata(array, maxrows, maxcols, nrows, ncols)
	real array(maxrows, maxcols)
	integer irow, icol
c
c dumpdata 
c
	print 10, ((array(irow, icol), icol = 1, ncols), irow = 1, nrows)
10	format(' ', 5f7.3)
c
c all done
c
	return
	end
