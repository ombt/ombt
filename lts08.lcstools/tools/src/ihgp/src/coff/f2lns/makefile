
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

all:		f2lns \
		$(ENDLIST)
		touch all

f2lns:		f2lns.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o f2lns \
			f2lns.o \
			$(ENDLIST)
		cp f2lns $(HOME)/bin/f2lns

f2lns.o:	f2lns.c \
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
		-rm -f f2lns
		-rm -f f2lns.o

backup:		f2lns.c \
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
		cp f2lns.c $(BACKUPDIR)/f2lns.c
		cp ../include/aouthdr.h ../include/$(BACKUPDIR)/aouthdr.h
		cp ../include/filehdr.h ../include/$(BACKUPDIR)/filehdr.h
		cp ../include/reloc.h ../include/$(BACKUPDIR)/reloc.h
		cp ../include/syms.h ../include/$(BACKUPDIR)/syms.h
		cp ../include/storclass.h ../include/$(BACKUPDIR)/storclass.h
		cp ../include/scnhdr.h ../include/$(BACKUPDIR)/scnhdr.h
		cp ../include/linenum.h ../include/$(BACKUPDIR)/linenum.h
		touch backup

print:		f2lns.c \
		../include/aouthdr.h \
		../include/filehdr.h \
		../include/reloc.h \
		../include/syms.h \
		../include/storclass.h \
		../include/scnhdr.h \
		../include/linenum.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			f2lns.c \
			../include/aouthdr.h \
			../include/filehdr.h \
			../include/reloc.h \
			../include/syms.h \
			../include/storclass.h \
			../include/scnhdr.h \
			../include/linenum.h \
			$(ENDLIST)
