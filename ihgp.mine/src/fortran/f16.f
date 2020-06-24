	program strings
c
	implicit none
c
	character*10 string
	character*5 string2
	character*10 s1, s2
	character*15 s3
c
	data string / '0123456789' /
	data string2 / 'abcde' /
	data s1, s2 / '0123456789', '0123456789' / 
c
	print *, 'string = ', string
	print *, 'string(2:8) = ', string(2:8)
	print *, 'string2 = ', string2
	print *, 'string2(2:4) = ', string2(2:4)
c
	print *, 's3 = s1 // s2'
	print *, 's3 = ', s1, ' // ', s2
	s3 = s1 // s2
	print *, 's3 = ', s3
c
	stop
	end
