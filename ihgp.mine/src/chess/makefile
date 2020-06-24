
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g
LD = gcc
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g
INCLDIRS = 
CC = gcc
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

all:		 \
		chess \
		$(ENDLIST)
		touch all

:		$(ENDLIST)
		$(LD) $(LDFLAGS) -o  \
			$(ENDLIST)

chess:		chess.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o chess \
			chess.o \
			-lcurses \
			$(ENDLIST)

chess.o:	chess.c \
		$(ENDLIST)

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f chess
		-rm -f chess.o

backup:		chess.c \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp chess.c $(BACKUPDIR)/chess.c
		touch backup

print:		chess.c \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			chess.c \
			$(ENDLIST)
