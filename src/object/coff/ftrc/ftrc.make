ftrc: ftrc.c 
	-rm $(HOME)/mar/bin/ftrc
	/bin/cc ftrc.c -g -o ftrc -lmalloc
	-ln ftrc $(HOME)/mar/bin/ftrc
