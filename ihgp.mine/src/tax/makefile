all:	utbl usury

usury:	 usury.c 
	cc usury.c -g -o usury -lm
	-rm $(HOME)/bin/usury
	ln usury $(HOME)/bin/usury

utbl:	utbl.c 
	CC utbl.c -g -o utbl -lm
	-rm $(HOME)/bin/utbl
	ln utbl $(HOME)/bin/utbl
