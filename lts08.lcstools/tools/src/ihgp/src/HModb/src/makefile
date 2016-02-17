
CC = CC
# CCFLAGS = -g -DDEBUG -DYYDEBUG
CCFLAGS = -g -DDEBUG 

LEX = lex
LEXFLAGS = -v

YACC = yacc
YACCFLAGS = -d

OTYPP_OBJS = otypp.o
OTYSCAN_OBJS = otyscanyacc.o otyscanlex.o otysemantics.o

all:		otypp \
		otyscan \
		otybld \
		otyssbld
		touch all

otypp:		$(OTYPP_OBJS)
		-rm -f $(HOME)/bin/otypp
		$(CC) $(CFLAGS) -o otypp otypp.o
		ln otypp $(HOME)/bin/otypp

otyscan:	$(OTYSCAN_OBJS)
		-rm -f $(HOME)/bin/otyscan
		$(CC) $(CFLAGS) -o otyscan $(OTYSCAN_OBJS) -ll -ly
		ln otyscan $(HOME)/bin/otyscan

otyscanyacc.o:	otyscan.y \
		otyscan.h \
		returns.h \
		debug.h
		$(YACC) $(YACCFLAGS) otyscan.y
		mv y.tab.h otyscanyacc.h
		mv y.tab.c otyscanyacc.c
		$(CC) $(CCFLAGS) -c otyscanyacc.c

otyscanlex.o:	otyscan.l \
		otyscan.h \
		otyscanyacc.h \
		returns.h \
		debug.h
		$(LEX) $(LEXFLAGS) otyscan.l
		mv lex.yy.c otyscanlex.c
		$(CC) $(CCFLAGS) -c otyscanlex.c

otysemantics.o:	otysemantics.c \
		otyscan.h \
		returns.h \
		debug.h
		$(CC) $(CCFLAGS) -c otysemantics.c

otypp.o:	otypp.c \
		otypp.h \
		returns.h \
		debug.h
		$(CC) $(CCFLAGS) -c otypp.c

otybld:		otybld.sh
		-rm -f $(HOME)/bin/otybld
		-rm -f otybld
		cp otybld.sh otybld
		chmod 755 otybld
		ln otybld $(HOME)/bin/otybld

otyssbld:	otyssbld.sh
		-rm -f $(HOME)/bin/otyssbld
		-rm -f otyssbld
		cp otyssbld.sh otyssbld
		chmod 755 otyssbld
		ln otyssbld $(HOME)/bin/otyssbld





