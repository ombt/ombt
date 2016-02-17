pdir: pdir.c 
	cc pdir.c -g -o pdir
	-rm $(HOME)/bin/pdir
	ln pdir $(HOME)/bin/pdir
