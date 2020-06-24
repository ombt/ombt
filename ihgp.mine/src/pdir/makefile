
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
INCLDIRS = 
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

all:		pdir \
		$(ENDLIST)
		touch all

pdir:		pdir.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o pdir \
			pdir.o \
			$(ENDLIST)
		cp pdir $(HOME)/bin/pdir

pdir.o:	pdir.c \
		$(ENDLIST)

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f pdir
		-rm -f pdir.o

backup:		pdir.c \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp pdir.c $(BACKUPDIR)/pdir.c
		touch backup

print:		pdir.c \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			pdir.c \
			$(ENDLIST)
