
.SUFFIXES:		.o .c

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g -xildoff
LD = CC
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g -DFLEXWARN -xildoff -DDEBUG
INCLDIRS = -I../include
CC = CC
LPRFLAGS = 
AR = ar
LPR = mycpr2on1
ENDLIST = 

.c.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<

all:		chap2 \
		chap3 \
		chap4 \
		chap5 \
		chap6 \
		chap7 \
		$(ENDLIST)
		touch all

chap2:		chap2.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o chap2 \
			chap2.o \
			$(ENDLIST)

chap3:		chap3.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o chap3 \
			chap3.o \
			$(ENDLIST)

chap4:		chap4.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o chap4 \
			chap4.o \
			$(ENDLIST)

chap5:		chap5.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o chap5 \
			chap5.o \
			$(ENDLIST)

chap6:		chap6.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o chap6 \
			chap6.o \
			$(ENDLIST)

chap7:		chap7.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o chap7 \
			chap7.o \
			$(ENDLIST)

chap6.o:	chap6.c \
		../include/aouthdr.h \
		./debug.h \
		../include/filehdr.h \
		../include/linenum.h \
		../include/reloc.h \
		./returns.h \
		../include/scnhdr.h \
		$(ENDLIST)

chap2.o:	chap2.c \
		../include/filehdr.h \
		$(ENDLIST)

chap7.o:	chap7.c \
		../include/aouthdr.h \
		./debug.h \
		../include/filehdr.h \
		../include/linenum.h \
		../include/reloc.h \
		./returns.h \
		../include/scnhdr.h \
		../include/syms.h \
		../include/storclass.h \
		$(ENDLIST)

chap3.o:	chap3.c \
		../include/aouthdr.h \
		../include/filehdr.h \
		$(ENDLIST)

chap4.o:	chap4.c \
		../include/aouthdr.h \
		../include/filehdr.h \
		../include/scnhdr.h \
		$(ENDLIST)

chap5.o:	chap5.c \
		../include/aouthdr.h \
		./debug.h \
		../include/filehdr.h \
		../include/reloc.h \
		./returns.h \
		../include/scnhdr.h \
		$(ENDLIST)

clean:
		-rm -rf Templates.DB
		-rm -f chap2
		-rm -f chap3
		-rm -f chap4
		-rm -f chap5
		-rm -f chap6
		-rm -f chap7
		-rm -f chap6.o
		-rm -f chap2.o
		-rm -f chap7.o
		-rm -f chap3.o
		-rm -f chap4.o
		-rm -f chap5.o

backup:		chap6.c \
		chap2.c \
		chap7.c \
		chap3.c \
		chap4.c \
		chap5.c \
		../include/aouthdr.h \
		./debug.h \
		../include/filehdr.h \
		../include/reloc.h \
		../include/syms.h \
		../include/storclass.h \
		./returns.h \
		../include/scnhdr.h \
		../include/linenum.h \
		$(ENDLIST)
		-([ ! -d "../include/$(BACKUPDIR)" ] && mkdir ../include/$(BACKUPDIR);)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp chap6.c $(BACKUPDIR)/chap6.c
		cp chap2.c $(BACKUPDIR)/chap2.c
		cp chap7.c $(BACKUPDIR)/chap7.c
		cp chap3.c $(BACKUPDIR)/chap3.c
		cp chap4.c $(BACKUPDIR)/chap4.c
		cp chap5.c $(BACKUPDIR)/chap5.c
		cp ../include/aouthdr.h ../include/$(BACKUPDIR)/aouthdr.h
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ../include/filehdr.h ../include/$(BACKUPDIR)/filehdr.h
		cp ../include/reloc.h ../include/$(BACKUPDIR)/reloc.h
		cp ../include/syms.h ../include/$(BACKUPDIR)/syms.h
		cp ../include/storclass.h ../include/$(BACKUPDIR)/storclass.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		cp ../include/scnhdr.h ../include/$(BACKUPDIR)/scnhdr.h
		cp ../include/linenum.h ../include/$(BACKUPDIR)/linenum.h
		touch backup

print:		chap6.c \
		chap2.c \
		chap7.c \
		chap3.c \
		chap4.c \
		chap5.c \
		../include/aouthdr.h \
		./debug.h \
		../include/filehdr.h \
		../include/reloc.h \
		../include/syms.h \
		../include/storclass.h \
		./returns.h \
		../include/scnhdr.h \
		../include/linenum.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			chap6.c \
			chap2.c \
			chap7.c \
			chap3.c \
			chap4.c \
			chap5.c \
			../include/aouthdr.h \
			./debug.h \
			../include/filehdr.h \
			../include/reloc.h \
			../include/syms.h \
			../include/storclass.h \
			./returns.h \
			../include/scnhdr.h \
			../include/linenum.h \
			$(ENDLIST)
