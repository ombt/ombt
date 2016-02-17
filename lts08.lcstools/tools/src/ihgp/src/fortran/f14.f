c
c crude word count using fortran
c
	program WordCount
c
c local variables
c
	implicit none
	character*256 word
	character tab, blank
	integer nchars, nwords, nchars2tab, nchars2blank
	parameter(tab = char(9), blank = char(32))
c
c initialize
c
	nchars = 0
	nwords = 0
c
c read until done
c
	do while (.true.)
		read(*, 5, err=20, end=10) word
5		format(a)
		print *, 'word is <', word, '>'
		call removeblanks(word)
		print *, 'word is <', word, '>'
		nchars2tab = index(word, tab)
		print *, 'nchars2tab = ', nchars2tab
		nchars2blank = index(word, blank)
		print *, 'nchars2blank = ', nchars2blank
		if (nchars2tab.lt.nchars2blank.and.nchars2tab.gt.0) then
			nchars = nchars + nchars2tab
		else
			nchars = nchars + nchars2blank
		end if
		nwords = nwords + 1
	end do
c
c print out results
c
10	continue
	print *, 'words = ', nwords
	print *, 'chars = ', nchars
	stop
c
c error return
c
20	print *, 'error in read'
	stop
c
c all done
c
	end
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c remove leading white space
c
	subroutine removeblanks(string)
c
c arguments and local variables
c
	character*(*) string
	integer is1
	character tab, blank
	parameter(tab = char(9), blank = char(32))
c
c scan string
c
	is1 = 1
	do while ((string(is1:is1).eq.blank).or.(string(is1:is1).eq.tab))
		is1 = is1 + 1
	end do
	string = string(is1:)
c
c all done
c
	return
	end
