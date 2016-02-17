
.SUFFIXES:		.o .c

LEXFLAGS = -n
YACC = yacc
YACCFLAGS = -d -v
LEX = lex
MAKE = make
LDFLAGS = -g
LD = CC
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g -DDEBUG
INCLDIRS = -I/opt/x11r5/include
CC = CC
AR = ar
ENDLIST = 

.c.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<

all:		mygraph \
		$(ENDLIST)
		touch all

mygraph:		mygraph.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o mygraph \
			mygraph.o \
			-lX11 \
			$(ENDLIST)
		chmod 755 mygraph
		cp mygraph /home/rumore/bin/mygraph

mygraph.o:	mygraph.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)

clean:
		-rm -rf Templates.DB
		-rm -f mygraph
		-rm -f mygraph.o

backup:		mygraph.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp mygraph.c $(BACKUPDIR)/mygraph.c
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		touch backup
