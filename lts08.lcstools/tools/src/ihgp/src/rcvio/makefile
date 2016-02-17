rcvio: rcvio.c 
	cc -DVAX rcvio.c -g -o rcvio
	-rm $(HOME)/bin/rcvio
	ln rcvio $(HOME)/bin/rcvio
	chmod 700 rcvio
debug: rcvio.c 
	cc -DVAX rcvio.c -g -o rcvio -DDEBUG
	-rm $(HOME)/bin/rcvio
	ln rcvio $(HOME)/bin/rcvio
	chmod 700 rcvio
