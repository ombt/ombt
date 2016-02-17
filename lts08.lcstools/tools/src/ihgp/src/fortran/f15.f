c
c file io demo
c
	program FileIO
c
	implicit none
c
c local variables
c
	character*256 filename
	character*256 filestatus
c
c get in file name from user
c
	print 10
10	format(' ', 'enter file name: ', $)
	read *, filename
c
c get status of file
c
	filestatus = ' '
	do while (filestatus.ne.'old'.and.filestatus.ne.'new')
		print 20
20		format(' ', 'file status, new or old: ', $)
		read *, filestatus
	end do
c
c open file
c
	open(unit=10, file=filename, status=filestatus, err=1000)
c
c if a new file, write to it. if old, dump data.
c
	if (filestatus.eq.'old') then
		call readfile(10)
	else
		call writefile(10)
	end if
c
c close file
c
	close(unit=10, err=1001)
c
c all done
c
	stop
c
c error statements
c
1000	stop 'error in open'
1001	stop 'error in close'
c
	end
c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
	integer function lastblank(string)
c
	implicit none
c
c arguments and local variables
c
	character*(*) string
	integer is
	character tab, blank
	parameter(tab = char(9), blank = char(32))
c
c find last blank
c
	do is=len(string), 1, -1
		if (string(is:is).ne.tab.and.string(is:is).ne.blank) then
			lastblank = is
			return
		end if
	end do
c
c all done
c
	lastblank = len(string)
	return
	end
c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
	subroutine readfile(unit)
c
	implicit none
c
c arguments and local variables
c
	integer unit
	character*256 string
	integer lastblank
c
	do while (.true.)
		read(unit, 10, end=20) string
10		format(a)
		print *, string(1:lastblank(string))
	end do
20	continue
c
c all done
c
	return
	end
c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
	subroutine writefile(unit)
c
	implicit none
c
c arguments and local variables
c
	integer unit
	character*256 string
	integer lastblank
c
c read from user and write to a file
c
	do while (.true.)
		print 10
10		format(' ', 'enter a string: ', $)
		read(*, 15, end=20) string
15		format(a)
		write(unit=unit, *) string(1:lastblank(string))
	end do
20	continue
c
c all done
c
	return
	end
