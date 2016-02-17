c
c demonstrate a binary search using a while-loop. 
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c        1         2         3         4
c234567890123456789012345678901234567890
c
      program binary
c
      integer maxsz
      parameter (maxsz=20)
c
c declare and initialize array using a data statement. refer
c to section 4.3.6, page 154 in text book.
c
      real mydata(maxsz), number
      data mydata / 11.0, 12.0, 13.0, 41.0, 15.0, 16.0, 17.0,
     1             18.0, 19.0, 30.0, 31.0, 32.0, 33.0, 
     2             34.0, 35.0, 36.0, 37.0, 38.0, 39.0,
     3             40.0 /
c
c other variables.
c
      logical found
      integer start, stop, middle
c
c get a number from the user and search the array mydata for
c the value given by the user. use a binary search.
c
      print *, 'enter a number'
      read *, number
c
c do a binary search to locate number
c
      start = 1
      stop = maxsz
      found = .false.
c
 100  if (start.le.stop.and..not.found) then
          middle = (start + stop)/2
          if (number.lt.mydata(middle)) then
              stop = middle - 1
          else if (number.gt.mydata(middle)) then
              start = middle + 1
          else
              found = .true.
          endif
          goto 100
      endif
c
c check if number was found
c
      if (found) then
          print *, 'number was found in location ', middle
      else
          print *, 'number was NOT found'
      endif
c
c all done
c
      stop
      end
