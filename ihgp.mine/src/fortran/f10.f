c
c bubble sort and binary search.
c
	program bsort
c
c no implicit
c
	implicit none
c
c local variables
c
	logical done, found
	real x(100), temp, value
	integer idx1, idx2, last, start, mid
c
c get how many from user
c
	last = 0
	do while ((last.lt.1).or.(last.gt.100))
		print 100
100		format(' ', 'how many numbers to read: ', $)
		read *, last
	end do
c
c start looping to read in values
c
	do idx1 = 1, last
		print 200
200		format(' ', 'enter number: ', $)
		read *, x(idx1)
	end do
c
c bubble sort
c
	idx1 = 1
	done = .false.
	do while ((idx1.le.last).and.(done.eq..false.))
		print *, 'while loop: iteration ... ', idx1
		done = .true.
		do idx2 = 1, last-1
			if (x(idx2).gt.x(idx2+1)) then
				print *, 'still have work to do ...'
				print *, 'switching x(', idx2, ')',
     1					 ' and x(', idx2+1, ')'
				temp = x(idx2)
				x(idx2) = x(idx2+1)
				x(idx2+1) = temp
				done = .false.
			endif
		end do
		if (done.eq..true.) print *, 'all done !!!'
		idx1 = idx1 + 1
	end do
c
c print out results
c
	do idx1 = 1, last
		print 300, idx1, x(idx1)
300		format(' ', 'x(', i4, ') = ', f9.4)
	end do
c
c ask user for a value to locate
c
	print *, 'which value to find: '
	read *, value
c
c use a binary search to locate value, if it exists.
c
	start = 1
	found = .false.
	do while (start.lt.last.and.found.eq..false.)
		print *, 'start = ', start, x(start)
		print *, 'last = ', last, x(last)
		print *, 'mid = ', mid, x(mid)
		mid = (start + last)/2 
		if (value.lt.x(mid)) then
			last = mid - 1
		else if (value.gt.x(mid)) then
			start = mid + 1
		else
			print *, 'found it'
			found = .true.
		endif
	end do
c
c was it found
c
	if (found.eq..true.) then
		print *, 'x(', mid, ') = ', x(mid)
	else
		print *, 'not found !!!'
	end if
c
c all done
c
	end
