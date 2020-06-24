c
c show how to use character strings
c
	program useofstrings
c
c local variables
c
	integer number
c
c read in a number.
c
	print *, 'enter a number:'
	read *, number
c
c switch on number.
c
+++++++++++++++++
NOTE: select not supported by sun f77.
+++++++++++++++++
	select case (number)
	case (1)
		print *, 'I have a one.'
	case (2)
		print *, 'I have a two.'
	case default
		print *, 'I have an Italian.'
	end select
c
c all done
c
	end
