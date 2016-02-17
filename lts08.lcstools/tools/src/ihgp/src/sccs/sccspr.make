sccspr: sccspr.c 
	cc sccspr.c -g -o sccspr
	-rm $(HOME)/bin/sccspr
	ln sccspr $(HOME)/bin/sccspr
