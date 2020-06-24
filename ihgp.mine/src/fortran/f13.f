c
c play with strings
c
	program strings
c
	implicit none
c
c local variables
c
	character*20 a,b,c
	character*200 d
c
	a = 'aaaaaaaaaaaaaaaaaaaa'
	print *, 'a is ... ', a
	b = 'bbbbbbbbbbbbbbbbbbbb'
	print *, 'b is ... ', b
	c = a
	print *, 'c is ... ', c
	c(10:20) = b
	print *, 'c is ... ', c
	d = a // b // c
	print *, 'd is ... ', d
c
c all done
c
	end
