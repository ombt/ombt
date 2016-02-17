c
c solution to project 3
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
	program proj3
c
	real sx, sy, sxy, sxx, det, x, y
	integer i, n
c
	print *, 'enter number of points'
	read *, n
c
	sx = 0
	sy = 0
	sxy = 0
	sxx = 0
c
	do 100 i=1, n, 1
		print *, 'enter x, y'
		read *, x, y
c
		x = log(x)
		y = log(y)
c
		sx = sx + x
		sy = sy + y
		sxy = sxy + x*y
		sxx = sxx + x*x
 100	continue
c
	det = float(n)*sxx - sx*sx
c
	a = (sy*sxx - sx*sxy)/det
	b = (float(n)*sxy - sx*sy)/det
c
	a = exp(a)
c
	print *, 'a = ', a
	print *, 'b = ', b
c
	stop
	end
