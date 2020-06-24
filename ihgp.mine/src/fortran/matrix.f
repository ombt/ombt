c
c program to show how to use 2-d matrices.
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c2345678901234567890
c
      program matrices
c
      implicit none
c
c define local variables and any functions
c
      integer maxsz
      parameter(maxsz=5)
      real a(maxsz, maxsz), b(maxsz, maxsz), c(maxsz, maxsz)
      integer arows, acols, brows, bcols, crows, ccols
      integer i, j
      character*1 answer
c
      logical status, mstar, mplus, mminus
c
c read in size of A matrix
c 
 100   print 150
 150  format(' ', 'enter number of rows and columns for matrix A: ')
      read *, arows, acols
      if ((arows.lt.1.or.arows.gt.maxsz).or.
     1    (acols.lt.1.or.acols.gt.maxsz)) then
          print 200, maxsz, maxsz
 200      format(' ', 'ERROR: must have 1 <= rows <= ', I2, 
     2                ' and 1 <= cols <= ', I2)
          goto 100
      end if
c
c read in A matrix
c
      read *, ((a(i, j), j=1, acols), i=1, arows)
c
c read in size of B matrix
c 
 300   print 350
 350  format(' ', 'enter number of rows and columns for matrix B: ')
      read *, brows, bcols
      if ((brows.lt.1.or.brows.gt.maxsz).or.
     1    (bcols.lt.1.or.bcols.gt.maxsz)) then
          print 400, maxsz, maxsz
 400      format(' ', 'ERROR: must have 1 <= rows <= ', I2, 
     2                ' and 1 <= cols <= ', I2)
          goto 300
      end if
c
c read in A matrix
c
      read *, ((b(i, j), j=1, bcols), i=1, brows)
c
c ask user what to do
c
      answer = ' '
 500  if (answer.ne.'q') then
          print 550
 550      format(' enter ''*'' for A*B'/
     1           ' enter ''+'' for A+B'/
     2           ' enter ''-'' for A+B'/
     3           ' enter ''q'' for quit')
          read *, answer
c
c check what was entered: multiply, add or subtract the two matrices.
c
          if (answer.eq.'*') then
c 
c C = A * B
c
              status = mstar(a, arows, acols, 
     1                       b, brows, bcols, 
     2                       c, crows, ccols,
     3                       maxsz, maxsz)
c
c check for an error and print C if success
c
              if (.not.status) then
                  print 600
 600              format(' ', 'A*B failed !!!')
              else
                  print 650, ((c(i, j), j=1, maxsz), i=1, maxsz)
 650              format(' ', 'c = '/5(/5e10.2,1x))
              end if
          else if (answer.eq.'+') then
c 
c C = A + B
c
              status = mplus(a, arows, acols, 
     1                       b, brows, bcols, 
     2                       c, crows, ccols,
     3                       maxsz, maxsz)
c
c check for an error and print C if success
c
              if (.not.status) then
                  print 700
 700              format(' ', 'A+B failed !!!')
              else
                  print 650, ((c(i, j), j=1, maxsz), i=1, maxsz)
              end if
          else if (answer.eq.'-') then
c 
c C = A - B
c
              status = mminus(a, arows, acols, 
     1                       b, brows, bcols, 
     2                       c, crows, ccols,
     3                       maxsz, maxsz)
c
c check for an error and print C if success
c
              if (.not.status) then
                  print 800
 800              format(' ', 'A-B failed !!!')
              else
                  print 650, ((c(i, j), j=1, maxsz), i=1, maxsz)
              end if
          endif
          goto 500
      endif
c
c all done
c
      stop
      end
c
c functions to multiply, add and subtract matrices.c
c
      logical function mstar(a, ar, ac, 
     1                       b, br, bc, 
     2                       c, cr, cc, 
     3                       maxr, maxc)
c
      implicit none
c
      integer ar, ac, br, bc, cr, cc, maxr, maxc, ir, ic, is
      real a(maxr, maxc), b(maxr, maxc), c(maxr, maxc)
c
c check that dimensions match for a multiplication
c
      if (ac.ne.br) then
          print 100
 100      format(' ', 'number of A rows != number of B columns')
          mstar = .false.
          return
      end if
c
c determine number of rows and columns of C matrix
c
      cr = ar
      cc = bc
c
c do multiplication
c
      do 200 ir=1, cr, 1
          do 300 ic=1, cc, 1
              do 400 is=1, ac, 1
                  c(ir, ic) = c(ir, ic) + a(ir, is)*b(is, ic)
 400          continue
 300      continue
 200   continue
c
c all done
c
      mstar = .true.
      return
      end
c
      logical function mplus(a, ar, ac, 
     1                       b, br, bc, 
     2                       c, cr, cc, 
     3                       maxr, maxc)
c
      implicit none
c
      integer ar, ac, br, bc, cr, cc, maxr, maxc, ir, ic
      real a(maxr, maxc), b(maxr, maxc), c(maxr, maxc)
c
c check that dimensions match for addition
c
      if (ar.ne.br.or.ac.ne.bc) then
          print 100
 100      format(' ', 'number of A rows != number of B rows or'/
     1                'number of A columns != number of B columns')
          mplus = .false.
          return
      end if
c
c determine number of rows and columns of C matrix
c
      cr = ar
      cc = ac
c
c do addition
c
      do 200 ir=1, cr, 1
          do 300 ic=1, cc, 1
              c(ir, ic) = a(ir, ic) + b(ir, ic)
 300      continue
 200   continue
c
c all done
c
      mplus = .true.
      return
      end
c
      logical function mminus(a, ar, ac, 
     1                        b, br, bc, 
     2                        c, cr, cc, 
     3                        maxr, maxc)
c
      implicit none
c
      integer ar, ac, br, bc, cr, cc, maxr, maxc, ir, ic
      real a(maxr, maxc), b(maxr, maxc), c(maxr, maxc)
c
c check that dimensions match for a subtraction
c
      if (ar.ne.br.or.ac.ne.bc) then
          print 100
 100      format(' ', 'number of A rows != number of B rows or'/
     1                'number of A columns != number of B columns')
          mminus = .false.
          return
      end if
c
c determine number of rows and columns of C matrix
c
      cr = ar
      cc = ac
c
c do subtraction
c
      do 200 ir=1, cr, 1
          do 300 ic=1, cc, 1
              c(ir, ic) = a(ir, ic) - b(ir, ic)
 300      continue
 200   continue
c
c all done
c
      mminus = .true.
      return
      end
