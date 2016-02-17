c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c least-squares fit to a straight line.
c
c y = a + b*x
c
c calculate and return coefficients a and b. also return chi-squared.
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
	logical function linearfit(y, x, size, a, b, x2)
c
c turn off implicit typing.
c
	implicit none
c
c arguments 
c
	integer size
	real y(size), x(size)
	real a, b, x2
c
c local variables.
c
	real sy, sx, sx2, sxy, determinant
	integer idx
c
c check arguments for sanity.
c
	if (size.le.0) then
		print *, 'linearfit: size <= 0'
		linearfit = .false.
		return
	end if
c
c initialize variables
c
	sx = 0.0
	sy = 0.0
	sx2 = 0.0
	sxy = 0.0
c
c calculate sums.
c
	do idx=1, size, 1
		sy = sy + y(idx)
		sx = sx + x(idx)
		sxy = sxy + x(idx)*y(idx)
		sx2 = sx2 + x(idx)*x(idx)
	end do
c
c calculate determinant
c
	determinant = float(size)*sx2 - sx*sx
	if (determinant.eq.0.0) then
		print *, 'determinant is zero.'
		linearfit = .false.
		return
	end if
c
c calculate coefficients
c
	a = (sy*sx2 - sx*sxy)/determinant
	b = (float(size)*sxy - sx*sy)/determinant
c
c calculate the chi-squared.
c
	x2 = 0.0
	do idx=1, size, 1
		x2 = x2 + abs(y(idx) - a - b*x(idx))**2
	end do
c
c all done.
c
	linearfit = .true.
	return
	end
c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c least-squares fit to the form:
c
c	y = a*b**x
c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
	logical function arbexpfit(y, x, size, a, b, x2)
c
c turn off implicit typing.
c
	implicit none
c
c arguments 
c
	integer size
	real y(size), x(size)
	real a, b, x2
c
c local variables.
c
	integer idx
	logical status
c
c declare functions
c
	logical linearfit
c
c check arguments for sanity.
c
	if (size.le.0) then
		print *, 'arbexpfit: size <= 0'
		arbexpfit = .false.
		return
	end if
c
c linearize the equation and data. remember that
c
c y = a*b**x <====> lny = lna + lnb*x
c
	do idx=1, size, 1
		y(idx) = log(y(idx))
	end do
c
c call linear fit.
c
	status = linearfit(y, x, size, a, b, x2)
	if (.not.status) then
		print *, 'arbexpfit: linearfit failed.'
		arbexpfit = .false.
		return
	end if
c
c take antilogs to get a and b coefficients.
c
	a = exp(a)
	b = exp(b)
	do idx=1, size, 1
		y(idx) = exp(y(idx))
	end do
c
c recalculate chi-squared.
c
	x2 = 0.0
	do idx=1, size, 1
		x2 = x2 + abs(y(idx)-a*b**x(idx))**2
	end do
c
c all done.
c
	arbexpfit = .true.
	return
	end
c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c least-squares fit to the form:
c
c	y = a*exp(b*x)
c
	logical function expfit(y, x, size, a, b, x2)
c
c turn off implicit typing.
c
	implicit none
c
c arguments 
c
	integer size
	real y(size), x(size)
	real a, b, x2
c
c local variables.
c
	integer idx
	logical status
c
c declare functions
c
	logical linearfit
c
c check arguments for sanity.
c
	if (size.le.0) then
		print *, 'expfit: size <= 0'
		expfit = .false.
		return
	end if
c
c linearize the equation and data. remember that
c
c y = a*exp(b*x) <====> lny = lna + b*x
c
	do idx=1, size, 1
		y(idx) = log(y(idx))
	end do
c
c call linear fit.
c
	status = linearfit(y, x, size, a, b, x2)
	if (.not.status) then
		print *, 'expfit: linearfit failed.'
		expfit = .false.
		return
	end if
c
c take antilogs to get a coefficient.
c
	a = exp(a)
	do idx=1, size, 1
		y(idx) = exp(y(idx))
	end do
c
c recalculate chi-squared.
c
	x2 = 0.0
	do idx=1, size, 1
		x2 = x2 + abs(y(idx)-a*exp(b*x(idx)))**2
	end do
c
c all done.
c
	expfit = .true.
	return
	end
c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c least-squares fit to the form:
c
c	y = a*x**b
c
	logical function powerfit(y, x, size, a, b, x2)
c
c turn off implicit typing.
c
	implicit none
c
c arguments 
c
	integer size
	real y(size), x(size)
	real a, b, x2
c
c local variables.
c
	integer idx
	logical status
c
c declare functions
c
	logical linearfit
c
c check arguments for sanity.
c
	if (size.le.0) then
		print *, 'powerfit: size <= 0'
		powerfit = .false.
		return
	end if
c
c linearize the equation and data. remember that
c
c y = a*x**b <====> lny = lna + b*lnx
c
	do idx=1, size, 1
		x(idx) = log(x(idx))
		y(idx) = log(y(idx))
	end do
c
c call linear fit.
c
	status = linearfit(y, x, size, a, b, x2)
	if (.not.status) then
		print *, 'powerfit: linearfit failed.'
		powerfit = .false.
		return
	end if
c
c take antilogs to get a coefficient.
c
	a = exp(a)
	do idx=1, size, 1
		x(idx) = exp(x(idx))
		y(idx) = exp(y(idx))
	end do
c
c recalculate chi-squared.
c
	x2 = 0.0
	do idx=1, size, 1
		x2 = x2 + abs(y(idx)-a*x(idx)**b)**2
	end do
c
c all done.
c
	powerfit = .true.
	return
	end
