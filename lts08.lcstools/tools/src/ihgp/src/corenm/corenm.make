corenm: corenm.c 
	#cc corenm.c -DCC -g -o corenm
	cc corenm.c -g -o corenm
	-rm $(HOME)/bin/corenm
	ln corenm $(HOME)/bin/corenm
