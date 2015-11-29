lnno: lnno.c 
	/bin/cc lnno.c -g -o lnno
	-rm $(HOME)/mar/bin/lnno
	ln lnno $(HOME)/mar/bin/lnno
