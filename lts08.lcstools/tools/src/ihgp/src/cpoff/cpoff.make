cpoff: cpoff.c 
	cc cpoff.c -g -o cpoff
	-rm $(HOME)/bin/cpoff
	ln cpoff $(HOME)/bin/cpoff
