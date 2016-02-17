
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
CCFLAGS = -g
INCLDIRS = 
CC = CC
AR = ar
ENDLIST = 

.c.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<

all:		tnode \
		tsearch \
		$(ENDLIST)
		touch all

tnode:		tnode.o \
		list.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tnode \
			tnode.o \
			list.o \
			$(ENDLIST)

tsearch:		tsearch.o \
		search.o \
		list.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tsearch \
			tsearch.o \
			search.o \
			list.o \
			$(ENDLIST)

tnode.o:	tnode.c \
		./node.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		./nodeptr.h \
		./refcount.h \
		$(ENDLIST)

list.o:	list.c \
		./list.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

tsearch.o:	tsearch.c \
		./search.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		$(ENDLIST)

search.o:	search.c \
		./search.h \
		./debug.h \
		./list.h \
		./absIterator.h \
		./returns.h \
		$(ENDLIST)

clean:
		-rm -rf Templates.DB
		-rm -f tnode
		-rm -f tsearch
		-rm -f tnode.o
		-rm -f list.o
		-rm -f tsearch.o
		-rm -f search.o

backup:		tnode.c \
		list.c \
		tsearch.c \
		search.c \
		./search.h \
		./debug.h \
		./absIterator.h \
		./nodeptr.h \
		./node.h \
		./returns.h \
		./list.h \
		./refcount.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp tnode.c $(BACKUPDIR)/tnode.c
		cp list.c $(BACKUPDIR)/list.c
		cp tsearch.c $(BACKUPDIR)/tsearch.c
		cp search.c $(BACKUPDIR)/search.c
		cp ./search.h ./$(BACKUPDIR)/search.h
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./absIterator.h ./$(BACKUPDIR)/absIterator.h
		cp ./nodeptr.h ./$(BACKUPDIR)/nodeptr.h
		cp ./node.h ./$(BACKUPDIR)/node.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		cp ./list.h ./$(BACKUPDIR)/list.h
		cp ./refcount.h ./$(BACKUPDIR)/refcount.h
		touch backup
