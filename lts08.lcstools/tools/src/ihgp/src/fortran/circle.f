	program circle
c
	real r1, r2, x, y
	integer i, imax
c
	r2 = 5.0
	r1 = 4.0
c
	print *, 'enter number of points'
	read *, imax
c
	do i=1, imax, 1
		print *, 'enter x, y'
		read *, x, y
c
		if (x**2+y**2.ge.r2**2) then
			print *, 'outside r2'
		else if (x**2*y**2.lt.r1**2) then
			print *, 'inside r1'
		else
			print *, 'between r1 and r2'
		endif
	enddo
c
	end
