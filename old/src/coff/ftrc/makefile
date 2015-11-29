
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g
LD = cc
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g
INCLDIRS = -I../include
CC = cc
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

all:		ftrc \
		$(ENDLIST)
		touch all

ftrc:		ftrc.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o ftrc \
			ftrc.o \
			$(ENDLIST)
		cp ftrc $(HOME)/bin/ftrc
		rcp ftrc integ@ihlbs1:/home/integ/bin/ftrc

ftrc.o:	ftrc.c \
		../include/aouthdr.h \
		../include/debug.h \
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
		-rm -f ftrc
		-rm -f ftrc.o

backup:		ftrc.c \
		../include/aouthdr.h \
		../include/debug.h \
		../include/filehdr.h \
		../include/reloc.h \
		../include/syms.h \
		../include/storclass.h \
		../include/scnhdr.h \
		../include/linenum.h \
		$(ENDLIST)
		-([ ! -d "../include/$(BACKUPDIR)" ] && mkdir ../include/$(BACKUPDIR);)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp ftrc.c $(BACKUPDIR)/ftrc.c
		cp ../include/aouthdr.h ../include/$(BACKUPDIR)/aouthdr.h
		cp ../include/debug.h ../include/$(BACKUPDIR)/debug.h
		cp ../include/filehdr.h ../include/$(BACKUPDIR)/filehdr.h
		cp ../include/reloc.h ../include/$(BACKUPDIR)/reloc.h
		cp ../include/syms.h ../include/$(BACKUPDIR)/syms.h
		cp ../include/storclass.h ../include/$(BACKUPDIR)/storclass.h
		cp ../include/scnhdr.h ../include/$(BACKUPDIR)/scnhdr.h
		cp ../include/linenum.h ../include/$(BACKUPDIR)/linenum.h
		touch backup

print:		ftrc.c \
		../include/aouthdr.h \
		../include/debug.h \
		../include/filehdr.h \
		../include/reloc.h \
		../include/syms.h \
		../include/storclass.h \
		../include/scnhdr.h \
		../include/linenum.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			ftrc.c \
			../include/aouthdr.h \
			../include/debug.h \
			../include/filehdr.h \
			../include/reloc.h \
			../include/syms.h \
			../include/storclass.h \
			../include/scnhdr.h \
			../include/linenum.h \
			$(ENDLIST)
