c
c program to read in a series of points and compute the mean and
c standard deviation. shows how to read in values for arrays and
c how to use arrays in calculations.
c
c        1         2         3         4
c234567890123456789012345678901234567890
c
c         n
c        ---
c mean = \   x(i) / n
c        /
c        ---
c        i=1
c
c                       n
c                      ---
c standard deviation = \   (x(i) - mean)**2 / (n - 1)
c                      /
c                      ---
c                      i=1
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
	program avrg
c
c declare variables
c
	integer maxsz
	parameter(maxsz=1000)
c
	real sum, mean, sdev
	real x(maxsz)
	integer npts, i
c
c how many points to read in?
c
 50	print *, 'enter the number of points'
	read *, npts
	if (npts.lt.2.or.npts.gt.maxsz) goto 50
c
c read in points
c
	do 100 i=1, npts, 1
		print *, 'enter point'
		read *, x(i)
 100	continue
c
c calculate the mean
c
	sum = 0.0
	do 200 i=1, npts, 1
		sum = sum + x(i)
 200	continue
	mean = sum/float(npts)
c
c calculate the standard deviation
c
	sum = 0.0
	do 300 i=1, npts, 1
		sum = sum + (x(i) - mean)**2
 300	continue
	sdev = sum/float(npts-1)
c
c print out results
c
	print *, 'mean = ', mean
	print *, 'standard deviation  = ', sdev
c
c all done
c
	stop
	end
