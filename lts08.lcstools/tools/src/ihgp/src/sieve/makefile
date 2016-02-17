
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

all:		sieve \
		$(ENDLIST)
		touch all

sieve:		sieve.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o sieve \
			sieve.o \
			$(ENDLIST)
		cp sieve $(HOME)/bin

sieve.o:	sieve.c \
		$(ENDLIST)

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f sieve
		-rm -f sieve.o

backup:		sieve.c \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp sieve.c $(BACKUPDIR)/sieve.c
		touch backup

print:		sieve.c \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			sieve.c \
			$(ENDLIST)
