
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
SHELLBIN = $(HOME)/tools/bin
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

all:		ptalog.parse \
		$(ENDLIST)
		touch all

ptalog.parse:		ptalog.parse.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/ptalog.parse
		-cp ptalog.parse.sh ptalog.parse
		chmod +x ptalog.parse
		cp ptalog.parse $(SHELLBIN)/ptalog.parse

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f ptalog.parse

backup:		ptalog.parse.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp ptalog.parse.sh $(BACKUPDIR)/ptalog.parse.sh
		touch backup

backupd:		ptalog.parse.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		ptalog.parse.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			ptalog.parse.sh \
			$(ENDLIST)
