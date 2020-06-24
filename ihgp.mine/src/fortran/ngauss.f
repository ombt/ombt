c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c gaussian elimination with back-substitution.
c
	logical function gaussian(array, y, x, maxsize, size)
c
c turn off implicit typing.
c
	implicit none
c
c arguments 
c
	integer maxsize, size
	real array(maxsize, maxsize)
	real y(maxsize), x(maxsize)
c
c local variables.
c
	integer diagonal
	logical status
c
c declare functions.
c
	logical eliminaterows
	logical backsubstitution
c
c check arguments for sanity.
c
	if (maxsize.le.0.or.size.gt.maxsize) then
		print *, 'gaussian: maxsize <= 0 or size > maxsize'
		gaussian = .false.
		return
	end if
c
c start gaussian elimination.
c
	do 100 diagonal=1, size-1, 1
c
c eliminate all leading coefficients in the present column.
c
		status = eliminaterows(array, y, diagonal, 
     1				       maxsize, size)
		if (.not.status) then
			print *, 'eliminaterows failed.'
			gaussian = .false.
			return
		end if
 100	continue
c
c do back substitution to get solution.
c
	status = backsubstitution(array, x, y, maxsize, size)
	if (.not.status) then
		print *, 'backsubstitution failed.'
		gaussian = .false.
		return
	end if
c
c all done.
c
	gaussian = .true.
	return
	end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c do gaussian elimination.
c
	logical function eliminaterows(array, y, diagonal, maxsize, size)
c
c turn off implicit typing.
c
	implicit none
c
c arguments 
c
	integer maxsize, size, diagonal
	real array(maxsize, maxsize)
	real y(maxsize)
c
c local variables.
c
	integer column, row
	real factor
c
c check arguments for sanity.
c
	if (maxsize.le.0.or.size.gt.maxsize) then
		print *, 'eliminaterows: maxsize <= 0 or size > maxsize'
		eliminaterows = .false.
		return
	end if
c
c check for division by zero.
c
	if (array(diagonal, diagonal).eq.0.0) then
		print *, 'eliminaterows: array(', 
     1		diagonal, ',', diagonal, ') is zero.'
		eliminaterows = .false.
		return
	end if
c
c calculate new rows.
c
	do 100 row=diagonal+1, size, 1
c
c start eliminating rows.
c
		factor = array(row, diagonal)/array(diagonal, diagonal)
		do column=diagonal, size, 1
			array(row, column) = 
     1			array(row, column) - factor*array(diagonal, column)
		end do
c
c update y-vector.
c
		y(row) = y(row) - factor*y(diagonal)
 100	continue
c
c all done.
c
	eliminaterows = .true.
	return
	end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c solve upper-triangular matrix.
c
	logical function backsubstitution(array, x, y, maxsize, size)
c
c turn off implicit typing.
c
	implicit none
c
c arguments 
c
	integer maxsize, size
	real array(maxsize, maxsize)
	real y(maxsize), x(maxsize)
c
c local variables.
c
	integer row, column
c
c check arguments for sanity.
c
	if (maxsize.le.0.or.size.gt.maxsize) then
		print *, 'backsubstitution: maxsize <= 0 or size > maxsize'
		backsubstitution = .false.
		return
	end if
c
c solve upper-triangular matrix.
c
	do 100 row=size, 1, -1
		x(row) = y(row)
		do column=size, row+1, -1
			x(row) = x(row) - array(row, column)*x(column)
		end do
		if (array(row, row).eq.0.0) then
			print *, 'backsubstitution: array(', 
     1			row, ',', row, ') is zero.'
			backsubstitution = .false.
			return
		else
			x(row) = x(row)/array(row, row)
		end if
 100	continue
c
c all done.
c
	backsubstitution = .true.
	return
	end
