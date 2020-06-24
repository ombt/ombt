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
	print *, 'your name is ', first, ' ', last
	print *, 'your age is ', age, ' in years.'
	print *, 'your age is ', months, ' in months.'
c
c all done
c
	end
