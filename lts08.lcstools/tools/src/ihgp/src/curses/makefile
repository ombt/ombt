
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = 
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

all:		edcur \
		$(ENDLIST)
		touch all

edcur:		edcur.o \
		curplot.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o edcur \
			edcur.o \
			curplot.o \
			-lm \
			-lcurses \
			$(ENDLIST)

edcur.o:	edcur.c \
		./curplot.h \
		$(ENDLIST)

curplot.o:	curplot.c \
		$(ENDLIST)

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f edcur
		-rm -f edcur.o
		-rm -f curplot.o

backup:		edcur.c \
		curplot.c \
		./curplot.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp edcur.c $(BACKUPDIR)/edcur.c
		cp curplot.c $(BACKUPDIR)/curplot.c
		cp ./curplot.h ./$(BACKUPDIR)/curplot.h
		touch backup

print:		edcur.c \
		curplot.c \
		./curplot.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			edcur.c \
			curplot.c \
			./curplot.h \
			$(ENDLIST)
