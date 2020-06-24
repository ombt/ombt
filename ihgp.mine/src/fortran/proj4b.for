c
c another solution to project 4.
c
	program proj4
c
	implicit none
c
c local variables
c
	integer iset, ix, iy, npts, nsets
	parameter(npts=15, nsets=3)
c
	real x(npts), y(nsets, npts), a(nsets), b(nsets)
	real logx, logy
	real sx, sy, sxy, sxx, det, chi
	real amean, asdev, bmean, bsdev
c
c read in x and y values
c
	do ix=1, npts
		read *, x(ix), (y(iy, ix), iy=1, nsets)
	end do
c
c calculate a and b for 3 sets of (x, y) points
c
	do iset = 1, nsets
c
c clear sums
c
		sx = 0
		sy = 0
		sxy = 0
		sxx = 0
c
c read in y values and calculate sums
c
		do iy=1, npts
			logx = log(x(iy))
			logy = log(y(iset, iy))
c
			sx = sx + logx
			sy = sy + logy
			sxx = sxx + logx*logx
			sxy = sxy + logx*logy
		end do
c
c calculate a and b
c
		det = float(npts)*sxx - sx*sx
		a(iset) = exp((sy*sxx - sx*sxy)/det)
		b(iset) = (float(npts)*sxy - sx*sy)/det
c
c calculate the the chi-squared
c
		chi = 0
		do ix = 1, npts
			chi = chi + (y(iset, ix) - a(iset)*x(ix)**b(iset))**2
		end do
c
c print results
c
		print *, '-------------------------------'
		print *, 'set ', iset, ' results ...'
		print *, 'a   = ', a(iset)
		print *, 'b   = ', b(iset)
		print *, 'chi = ', chi
	end do
c
c calculate the mean and standard deviation for a and b
c
	amean = 0
	bmean = 0
	do iset = 1, nsets
		amean = amean + a(iset)
		bmean = bmean + b(iset)
	end do
	amean = amean/float(nsets)
	bmean = bmean/float(nsets)
c
	asdev = 0
	bsdev = 0
	do iset = 1, nsets
		asdev = asdev + (a(iset) - amean)**2
		bsdev = bsdev + (b(iset) - bmean)**2
	end do
	asdev = sqrt(asdev/float(nsets-1))
	bsdev = sqrt(bsdev/float(nsets-1))
c
c print results
c
	print *, '-------------------------------'
	print *, 'a = ', amean, ' +/- ', asdev
	print *, 'b = ', bmean, ' +/- ', bsdev
c
c all done
c	stop
	end
