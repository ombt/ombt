c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c subroutine to graph a function. 
c
c status = plotf(f, xmin, xmax, xlabel, ymin, ymax, ylabel, ptype)
c
c ptype = 'linear', 'log', 'sqrt'. anything else is interpreted as
c         linear.
c
c plots look something like this ...
c
c       y-min                                                     y-max
c                               y-axis-label
c x-min +-------------------------------------------------------------+
c       |                                |                            |
c       |                                |                            |
c       |                                |                            |
c       |                                |                            |
c       |                                |                            |
c       |                                |                            |
c       |                                |                            |
c     x |                                |                            |
c     | |                                |                            |
c     a |                                |                            |
c     x |                                |                            |
c     i |                                |                            |
c     s |--------------------------------0----------------------------|
c     | |                                |                            |
c     l |                                |                            |
c     a |                                |                            |
c     b |                                |                            |
c     e |                                |                            |
c     l |                                |                            |
c       |                                |                            |
c       |                                |                            |
c       |                                |                            |
c       |                                |                            |
c x-max +--------------------------------|----------------------------+
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c2345678901234567890
c
	logical function plotf(f, xmin, xmax, xlabel, ymin, ymax, ylabel, ptype)
c
	implicit none
c
c function to plot
c
	external f
	real f
c
c other arguments
c
	real xmin, xmax, ymin, ymax
	character*(*) xlabel, ylabel, ptype
c
c local variables
c
	integer ix, yoff, xlabmin, xlabmax, ixl
	character*50 outbuf
	character*1 xch
	real x, dx, dy, plotymin, plotymax
	integer ify, iyaxis, ixaxis
c
c sanity check of arguments
c  
	if (xmin.ge.xmax.or.ymin.ge.ymax) then
		print *, 'xmin >= xmax or ymin >= ymax'
		plotf = .false.
		return
	endif
c
c clear buffer.
c
	call fill(outbuf, ' ')
c
c get scaling for plotting function.
c 
	dx = (xmax-xmin)/50.0
	if (ptype.eq.'log') then
		plotymin = log(ymin)
		plotymax = log(ymax)
	else if (ptype.eq.'sqrt') then
		plotymin = sqrt(ymin)
		plotymax = sqrt(ymax)
	else
		plotymin = ymin
		plotymax = ymax
	endif
	dy = (plotymax-plotymin)/50.0
c
c calculate location of axes
c
	if (xmin.le.0.0.and.0.0.le.xmax) then
		ixaxis = (0.0-xmin)/dx
	else
		ixaxis = -1
	endif
	if (ptype.ne.'log'.and.plotymin.le.0.0.and.0.0.le.plotymax) then
		iyaxis = (0.0-plotymin)/dy + 1
	else
		iyaxis = -1
	endif
c
c print y-axis min and max values
c
	print 100, ymin, ymax
 100	format(/' ', t13, e10.3, t55, e10.3)
c
c print y-axis label
c
	yoff = 50/2 - len(ylabel)/2
	if (ptype.eq.'log') then
		outbuf(yoff-5:) = 'log10 plot: ' // ylabel
	else if (ptype.eq.'sqrt') then
		outbuf(yoff-5:) = 'sqrt plot: ' // ylabel
	else
		outbuf(yoff-6:) = 'linear plot: ' // ylabel
	endif
c
	print 210, outbuf
 210	format(' ', t13, a50)
c
	call fill(outbuf, ' ')
c
c figure out where to print x-label
c
	xlabmin = 50/2 - len(xlabel)/2
	xlabmax = xlabmin + len(xlabel) - 1
	ixl = 1
c
c print y-axis.
c
	print 300
 300	format(' ', t13, 52('-'))
c
c start looping to print graph.
c
	do 1000 ix=1, 50, 1
c
c check if x-label must be printed
c
		if (xlabmin.le.ix.and.ix.le.xlabmax) then
			xch = xlabel(ixl:ixl)
			ixl = ixl + 1
		else
			xch = ' '
		endif
c
c print out axes.
c
		if (ixaxis.eq.ix) then
			call fill(outbuf, '-')
		endif
		if (iyaxis.ne.-1) then
			outbuf(iyaxis:iyaxis) = '|'
		endif
		if (ixaxis.eq.ix.and.iyaxis.ne.-1) then
			outbuf(iyaxis:iyaxis) = '+'
		endif
c
c check if function must be printed.
c
		x = xmin + dx*ix
		if (ptype.eq.'log') then
			ify = (log(f(x))-plotymin)/dy + 1
		else if (ptype.eq.'sqrt') then
			ify = (sqrt(f(x))-plotymin)/dy + 1
		else
			ify = (f(x)-plotymin)/dy + 1
		endif
		if (1.le.ify.and.ify.le.50) then
			outbuf(ify:ify) = '*'
		endif
c
c print out x-axis label, xmin, xmax and outbuf.
c
		if (ix.eq.1) then
			print 400, xmin, outbuf
 400			format(' ', e10.3, t13, '|', a50, '|')
		else if (ix.eq.50) then
			print 500, xmax, outbuf
 500			format(' ', e10.3, t13, '|', a50, '|')
		else
			print 600, xch, outbuf
 600			format(' ', t10, a1, t13, '|', a50, '|')
		endif
		call fill(outbuf, ' ')
 1000	continue
c
c print y-axis.
c
	print 300
c
c all done
c
	plotf = .true.
	return
	end
c
c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c fill a buffer with a given character
c
	subroutine fill(buf, ch)
c
	implicit none
c
	character*1 ch
	character*(*) buf
	integer ic
c
	do 100 ic=1, len(buf), 1
		buf(ic:ic) = ch
 100	continue
c
	return
	end
