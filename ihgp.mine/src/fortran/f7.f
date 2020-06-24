c
c play with do-loop.
c
	program doloop
c
c local variables
c
	integer start, stop, step, s
c
c read in variables
c
	print 100
100	format(' ', 'enter start, stop, step: ')
	read *, start, stop, step
c
c tell user what was entered.
c
	print 200, start, stop, step
200	format(' ', /,
     1	       'start = ', t15, i4,/
     2	       'stop  = ', t15, i4,/
     3	       'step  = ', t15, i4)
c
c run thru do-loop
c
	do s=start, stop, step
		print 250, s
250		format(' ', 'loop variable = ', i4)
	end do
c
c all done
c
	end
