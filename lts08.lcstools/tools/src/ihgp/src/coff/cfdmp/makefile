
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

all:		cfdmp \
		$(ENDLIST)
		touch all

cfdmp:		cfdmp.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o cfdmp \
			cfdmp.o \
			$(ENDLIST)
		cp cfdmp $(HOME)/bin/cfdmp

cfdmp.o:	cfdmp.c \
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
		-rm -f cfdmp
		-rm -f cfdmp.o

backup:		cfdmp.c \
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
		cp cfdmp.c $(BACKUPDIR)/cfdmp.c
		cp ../include/aouthdr.h ../include/$(BACKUPDIR)/aouthdr.h
		cp ../include/filehdr.h ../include/$(BACKUPDIR)/filehdr.h
		cp ../include/reloc.h ../include/$(BACKUPDIR)/reloc.h
		cp ../include/syms.h ../include/$(BACKUPDIR)/syms.h
		cp ../include/storclass.h ../include/$(BACKUPDIR)/storclass.h
		cp ../include/scnhdr.h ../include/$(BACKUPDIR)/scnhdr.h
		cp ../include/linenum.h ../include/$(BACKUPDIR)/linenum.h
		touch backup

print:		cfdmp.c \
		../include/aouthdr.h \
		../include/filehdr.h \
		../include/reloc.h \
		../include/syms.h \
		../include/storclass.h \
		../include/scnhdr.h \
		../include/linenum.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			cfdmp.c \
			../include/aouthdr.h \
			../include/filehdr.h \
			../include/reloc.h \
			../include/syms.h \
			../include/storclass.h \
			../include/scnhdr.h \
			../include/linenum.h \
			$(ENDLIST)
