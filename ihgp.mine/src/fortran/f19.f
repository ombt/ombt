c
c test of sin and cos function
c
      program trig
c
c variables
c 
      implicit none
c
      real pi
      parameter (pi=3.141593)
c
      integer nterms, iangle, nangles
      real angle, prec, radians
      real mysine, mycosine, value
c
c get number of angles.
c
      print *, 'enter number of angles'
      read *, nangles
c
      do 100 iangle = 1, nangles, 1
c
c get angle and precision
c
          print *, 'enter angle in degrees:'
          read *, angle
          radians = angle*2.0*pi/360.0
          print *, 'enter precision:'
          read *, prec
c
c do cosine calculation
c
          value = mycosine(radians, prec, nterms)
          print *, 'cosine(', angle, ') = ', value
          print *, 'precision used = ', prec
          print *, 'number of terms used = ', nterms
c
c do sine calculation
c
          value = mysine(radians, prec, nterms)
          print *, 'sine(', angle, ') = ', value
          print *, 'precision used = ', prec
          print *, 'number of terms used = ', nterms
c
 100  continue
c
c all done
c
      stop
      end
