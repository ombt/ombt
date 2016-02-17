all: tax njtax
	echo "all done" >all

tax: tax.c 
	cc tax.c -g -o tax -lm -DDEDUCTINCOMETAX
	-rm $(HOME)/bin/tax
	ln tax $(HOME)/bin/tax

njtax: tax.c 
	cc tax.c -g -o njtax -lm -DDEDUCTINCOMETAX -DNEWJERSEY
	-rm $(HOME)/bin/tax
	ln tax $(HOME)/bin/tax
