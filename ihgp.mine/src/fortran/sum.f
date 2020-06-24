c
	program sumit
c
	real x, sum, fact
	integer n, in, if
c
	print *, 'enter n and x'
	read *, n, x
c
	do in=1, n, 1
		fact = 1.0
		do if=1, 2*in, 1
			fact = fact*if
		enddo
c
		sum = sum + (-1**in)*(x**(2*in+1))/fact
	end do
c
	print *, 'sum(',x,') = ', sum
c
	end
