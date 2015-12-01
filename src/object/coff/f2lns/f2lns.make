f2lns: f2lns.c 
	cc f2lns.c -g -o f2lns -DFLEXWARN
	-rm $(HOME)/mar/bin/f2lns
	ln f2lns $(HOME)/mar/bin/f2lns
