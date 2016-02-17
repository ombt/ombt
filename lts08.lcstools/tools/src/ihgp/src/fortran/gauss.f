c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c gaussian elimination with row pivoting and back-substitution.
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
	integer diagonal, rowmax
	logical status
c
c declare functions.
c
	logical findmax
	logical switchrows
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
c find row with maximum leading element.
c
		status = findmax(array, diagonal, rowmax, maxsize, size)
		if (.not.status) then
			print *, 'findmax failed.'
			gaussian = .false.
			return
		end if
c
c switch rows if different that the diagonal row.
c
		if (diagonal.ne.rowmax) then
			status = switchrows(array, y, diagonal, 
     1					    rowmax, maxsize, size)
			if (.not.status) then
				print *, 'switchrows failed.'
				gaussian = .false.
				return
			end if
		end if
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
c find maximum element in a matrix.
c
	logical function findmax(array, diagonal, rowmax, maxsize, size)
c
c turn off implicit typing.
c
	implicit none
c
c arguments 
c
	integer maxsize, size, rowmax, diagonal
	real array(maxsize, maxsize)
c
c local variables.
c
	real arraymax
	integer row
c
c check arguments for sanity.
c
	if (maxsize.le.0.or.size.gt.maxsize) then
		print *, 'findmax: maxsize <= 0 or size > maxsize'
		findmax = .false.
		return
	end if
c
c find maximum element in a column.
c
	arraymax = abs(array(diagonal, diagonal))
	rowmax = diagonal
	do 100 row=diagonal+1, size, 1
		if (abs(array(row, diagonal)).gt.arraymax) then
			rowmax = row
			arraymax = abs(array(row, diagonal))
		end if
 100	continue
c
c all done.
c
	findmax = .true.
	return
	end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c switch rows in a matrix.
c
	logical function switchrows(array, y, diagonal, 
     1				    rowmax, maxsize, size)
c
c turn off implicit typing.
c
	implicit none
c
c arguments 
c
	integer diagonal, rowmax, maxsize, size
	real array(maxsize, maxsize), y(maxsize)
c
c local variables.
c
	real tmpy, tmparray
	integer column
c
c check arguments for sanity.
c
	if (maxsize.le.0.or.size.gt.maxsize) then
		print *, 'switchrows: maxsize <= 0 or size > maxsize'
		switchrows = .false.
		return
	end if
c
c switch y values.
c
	tmpy = y(diagonal)
	y(diagonal) = y(rowmax)
	y(rowmax) = tmpy
c
c switch entire row.
c
	do 100 column=diagonal, size, 1
		tmparray = array(diagonal, column)
		array(diagonal, column) = array(rowmax, column)
		array(rowmax, column) = tmparray
 100	continue
c
c all done.
c
	switchrows = .true.
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
