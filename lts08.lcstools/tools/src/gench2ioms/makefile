
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = 
TCLBIN = $(HOME)/bin
LD = cc
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/tools/bin/suidtools
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

all:		gench2ioms \
		$(ENDLIST)
		touch all

gench2ioms:		gench2ioms.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/gench2ioms
		-cp gench2ioms.sh gench2ioms
		chmod +x gench2ioms
		cp gench2ioms $(SHELLBIN)/gench2ioms
		-(rm $(HOME)/tools/bin/lcsgench2ioms)
		ln $(HOME)/tools/bin/suidcmd $(HOME)/tools/bin/lcsgench2ioms

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f gench2ioms

backup:		gench2ioms.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp gench2ioms.sh $(BACKUPDIR)/gench2ioms.sh
		touch backup

backupd:		gench2ioms.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		gench2ioms.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			gench2ioms.sh \
			$(ENDLIST)
