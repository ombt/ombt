c
c play with do-loop.
c
	program sdev
c
c no implicit
c
	implicit none
c
c local variables
c
	real x(100), avg, sdev
	integer idx, last
c
c get how many from user
c
	last = 0
	do while ((last.lt.1).or.(last.gt.100))
		print 100
100		format(' ', 'how many numbers to read: ')
		read *, last
	end do
c
c start looping to read in values
c
	avg = 0
	do idx = 1, last
		print 200
200		format(' ', 'enter number: ')
		read *, x(idx)
		avg = avg + x(idx)
	end do
c
c	get average
c
	avg = avg/float(last)
c
c get standard deviation
c
	sdev = 0
	do idx = 1, last
		sdev = sdev + abs(x(idx)-avg)**2
	end do
	sdev = sqrt(sdev/float(last))
c
c print average
c
	print 300, avg
300	format(' ', 'average is ... ', e12.4)
	print 400, sdev
400	format(' ', 'standard deviation is ... ', e12.4)
c
c all done
c
	end
