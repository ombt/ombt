cfdmp: cfdmp.c 
	/bin/cc cfdmp.c -g -o cfdmp
	-rm $(HOME)/mar/bin/cfdmp
	ln cfdmp $(HOME)/mar/bin/cfdmp
