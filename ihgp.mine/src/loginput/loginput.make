loginput: loginput.c 
	-rm $(HOME)/bin/loginput
	cc -g -o loginput loginput.c
	ln loginput $(HOME)/bin/loginput
