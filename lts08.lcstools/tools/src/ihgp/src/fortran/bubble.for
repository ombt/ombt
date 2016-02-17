c
c use a bubble sort to sort an array.
c
c        1         2         3        4
c23456789012345678901234567890234567890
c
      program bubble
c
c array of data to sort.
c
      integer dsz
      parameter (dsz=20)
c
c declare an array of random numbers.
c
      real d(dsz)
      data d / 2.0, 1.0, 3.0, 4.0, 6.0, 
     1         1.5, -4.0, -5.0, 100.0, 200.0,
     2         34.2, 33.0, 23.0, -300.0, -1000.0,
     3         2.03, 1.05, 44.0, 10.0, 0.0 /
c
c other variables.
c
      real temp
      integer idx1, idx2
      logical done
c
c show user how data looks like now.
c
      print *, '*********** DATA BEFORE SORTING *************'
      do 100 idx1=1, dsz, 1
          print *, 'data[', idx1, '] = ', d(idx1)
 100  continue
c
c use a bubble sort to sort the data
c
      idx1 = 2
      done = .false.
c
 150  if (idx1.le.dsz.and..not.done) then
c
c assume all done
c
          done = .true.
          do 200 idx2 = dsz, idx1, -1
c
c check which entries are out of order.
c
              if (d(idx2).lt.d(idx2-1)) then
c
c switch two entries that are out of order.
c
                  temp = d(idx2)
                  d(idx2) = d(idx2-1)
                  d(idx2-1) = temp
c
c still not done.
c
                  done = .false.
              endif
 200      continue
c
c increment counter
c
          idx1 = idx1 + 1
          goto 150
      end if
c
c show user how data looks like now.
c
      print *, '*********** DATA AFTER SORTING *************'
      do 300 idx1=1, dsz, 1
          print *, 'data[', idx1, '] = ', d(idx1)
 300  continue
c
c all done
c
      stop
      end
