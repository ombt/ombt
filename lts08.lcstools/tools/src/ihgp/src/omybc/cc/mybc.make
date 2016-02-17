mybc:	mybc.y mybc.l
	yacc -d mybc.y
	lex mybc.l
	cc -g -o mybc lex.yy.c y.tab.c -ll -ly -lm -DSUN
	-rm $(HOME)/bin/mybc
	ln mybc $(HOME)/bin/mybc
