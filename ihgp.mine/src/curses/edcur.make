edcur: edcur.c curplot.c curplot.h 
	cc edcur.c curplot.c -g -o edcur \
	-lm -lcurses
