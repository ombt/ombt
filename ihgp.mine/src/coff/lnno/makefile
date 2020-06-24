
.SUFFIXES:		.o .c

LEXFLAGS = -n
YACC = yacc
YACCFLAGS = -d -v
LEX = lex
MAKE = make
LDFLAGS = -g
LD = cc
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g -DFLEXWARN
INCLDIRS = -I../include
CC = cc
LPRFLAGS = 
AR = ar
LPR = mycpr2on1
ENDLIST = 

.c.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<

all:		lnno \
		$(ENDLIST)
		touch all

lnno:		lnno.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o lnno \
			lnno.o \
			$(ENDLIST)
		cp lnno $(HOME)/bin/lnno

lnno.o:	lnno.c \
		../include/aouthdr.h \
		../include/filehdr.h \
		../include/linenum.h \
		../include/reloc.h \
		../include/scnhdr.h \
		../include/syms.h \
		../include/storclass.h \
		$(ENDLIST)

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-rm -f lnno
		-rm -f lnno.o

backup:		lnno.c \
		../include/aouthdr.h \
		../include/filehdr.h \
		../include/reloc.h \
		../include/syms.h \
		../include/storclass.h \
		../include/scnhdr.h \
		../include/linenum.h \
		$(ENDLIST)
		-([ ! -d "../include/$(BACKUPDIR)" ] && mkdir ../include/$(BACKUPDIR);)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp lnno.c $(BACKUPDIR)/lnno.c
		cp ../include/aouthdr.h ../include/$(BACKUPDIR)/aouthdr.h
		cp ../include/filehdr.h ../include/$(BACKUPDIR)/filehdr.h
		cp ../include/reloc.h ../include/$(BACKUPDIR)/reloc.h
		cp ../include/syms.h ../include/$(BACKUPDIR)/syms.h
		cp ../include/storclass.h ../include/$(BACKUPDIR)/storclass.h
		cp ../include/scnhdr.h ../include/$(BACKUPDIR)/scnhdr.h
		cp ../include/linenum.h ../include/$(BACKUPDIR)/linenum.h
		touch backup

print:		lnno.c \
		../include/aouthdr.h \
		../include/filehdr.h \
		../include/reloc.h \
		../include/syms.h \
		../include/storclass.h \
		../include/scnhdr.h \
		../include/linenum.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			lnno.c \
			../include/aouthdr.h \
			../include/filehdr.h \
			../include/reloc.h \
			../include/syms.h \
			../include/storclass.h \
			../include/scnhdr.h \
			../include/linenum.h \
			$(ENDLIST)
