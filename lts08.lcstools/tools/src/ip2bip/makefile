
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g -DDEBUG
LD = gcc
TCLBIN = $(HOME)/bin
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g -DDEBUG
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

all:		ip2bip \
		$(ENDLIST)
		touch all

ip2bip:		ip2bip.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o ip2bip \
			ip2bip.o \
			$(ENDLIST)
		cp ip2bip $(HOME)/tools/bin/ip2bip

ip2bip.o:	ip2bip.c \
		$(ENDLIST)

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f ip2bip
		-rm -f ip2bip.o

backup:		ip2bip.c \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp ip2bip.c $(BACKUPDIR)/ip2bip.c
		touch backup

backupd:		ip2bip.c \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		ip2bip.c \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			ip2bip.c \
			$(ENDLIST)
