c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c program to read in an array and do some basic statistics on the data
c
	program statistics
c
c no implicit
c
	implicit none
c
c parameters
c
	integer arraysize
	parameter (arraysize = 100)
c
c local variables
c
	integer narray
	real array(arraysize)
	real avrg, sdev
c
c declare functions
c
	logical getdata
	logical getaverage
	logical getsdev
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c get data from user
c
	if (.not.getdata(array, arraysize, narray)) then
		print *, 'getdata failed !!!'
		stop
	end if
c
c do basic statistics
c
	if (.not.getaverage(array, arraysize, narray, avrg)) then
		print *, 'getaverage failed !!!'
		stop
	end if
	print 100, avrg
100	format(' ', 'average is ... ', f10.4)
c
c get standard deviation
c
	if (.not.getsdev(array, arraysize, narray, avrg, sdev)) then
		print *, 'getsdev failed !!!'
		stop
	end if
	print 200, sdev
200	format(' ', 'standard deviation is ... ', f10.4)
c
c all done
c
	end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
	logical function getdata(array, arraysize, narray)
c
c no implicit
c
	implicit none
c
c arguments and variables
c
	integer arraysize, narray
	real array(arraysize)
	integer iarray
c
c read in data
c
	print *, 'enter number of data elements: '
	read *, narray
	if (narray.le.0) then
		print *, 'narray <= 0 !!!'
		getdata = .false.
		return
	end if
	print *, 'enter ', narray, ' data points ...'
	read *, (array(iarray), iarray = 1, narray)
c
c all done
c
	getdata = .true.
	return
	end
c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
	logical function getaverage(array, arraysize, narray, avrg)
c
c no implicit
c
	implicit none
c
c arguments and variables
c
	integer arraysize, narray
	real array(arraysize)
	real avrg
	integer iarray
c
c figure out the average
c
	avrg = 0.0
	do iarray = 1, narray
		avrg = avrg + array(iarray)
	end do
	avrg = avrg/float(narray)
c
c all done
c
	getaverage = .true.
	return
	end
c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
	logical function getsdev(array, arraysize, narray, avrg, sdev)
c
c no implicit
c
	implicit none
c
c arguments and variables
c
	integer arraysize, narray
	real array(arraysize)
	real avrg, sdev
	integer iarray
c
c figure out the average
c
	sdev = 0.0
	do iarray = 1, narray
		sdev = sdev + (avrg-array(iarray))**2
	end do
	sdev = sqrt(sdev/float(narray))
c
c all done
c
	getsdev = .true.
	return
	end
c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
