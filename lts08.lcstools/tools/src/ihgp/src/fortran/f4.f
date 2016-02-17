c
c show how to use character strings
c
	program useofstrings
c
c local variables
c
	character first*20, last*20
	integer age
	integer months
c
c read in dolt's name.
c
	print *, 'enter your first and last name:'
	read *, first, last
c
c read in age.
c
	print *, 'enter age:'
	read *, age
	months = 12*age
c
c calculate age in months and tell dolt.
c
	print 100, first, last
100	format(' ', 'your name is ', a20, 1x, a20)
c
	print 200, age
200	format(' ', 'your age is ', i5, ' in years.')
c
	print 300, months
300	format(' ', 'your age is ', i5, ' in months.')
c
c all done
c
	end
