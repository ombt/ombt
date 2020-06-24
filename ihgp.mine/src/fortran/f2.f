c
c calculate an area for a circle and print results.
c
	program AreaOfCirle
c
c local variables
c
	real radius
	real circumference
	real area
c
c get radius from user.
c
	print *, 'Enter circle radius: '
	read *, radius
c
c calculate circumference and print. test out continuation line.
c
	circumference =  3.14156*
     c			 2.0*radius
	print *, 'Circumference is ... ', circumference
c
c calculate area and print.
c
	area =  3.14156*radius**2
	print *, 'Area is ... ', area
c
c all done
c
	end
