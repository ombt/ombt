
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g
LD = CC
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g -DDEBUG -DUSEEXCEPTION
INCLDIRS = 
CC = CC
LPRFLAGS = 
AR = /usr/ccs/bin/ar
LPR = mycpr2on1
ENDLIST = 

.c.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<
.C.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<
.cpp.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<
.CPP.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<

all:		hfe \
		$(ENDLIST)
		touch all

hfe:		hfe.o \
		array.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o hfe \
			hfe.o \
			array.o \
			$(ENDLIST)

hfe.o:	hfe.c \
		./huffman.h \
		./array.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		./fileio.h \
		$(ENDLIST)

array.o:	array.c \
		./array.h \
		./absIterator.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f hfe
		-rm -f hfe.o
		-rm -f array.o

backup:		hfe.c \
		array.c \
		./debug.h \
		./absIterator.h \
		./fileio.h \
		./huffman.h \
		./array.h \
		./returns.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp hfe.c $(BACKUPDIR)/hfe.c
		cp array.c $(BACKUPDIR)/array.c
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./absIterator.h ./$(BACKUPDIR)/absIterator.h
		cp ./fileio.h ./$(BACKUPDIR)/fileio.h
		cp ./huffman.h ./$(BACKUPDIR)/huffman.h
		cp ./array.h ./$(BACKUPDIR)/array.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		touch backup

print:		hfe.c \
		array.c \
		./debug.h \
		./absIterator.h \
		./fileio.h \
		./huffman.h \
		./array.h \
		./returns.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			hfe.c \
			array.c \
			./debug.h \
			./absIterator.h \
			./fileio.h \
			./huffman.h \
			./array.h \
			./returns.h \
			$(ENDLIST)
