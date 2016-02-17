
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
SHELLBIN = $(LCSTOOLSFPATH)
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

all:		lcslabid \
		cdtl1 \
		cdbtl1 \
		$(ENDLIST)
		touch all

lcslabid:		lcslabid.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/lcslabid
		-cp lcslabid.sh lcslabid
		chmod +x lcslabid
		cp lcslabid $(SHELLBIN)/lcslabid

cdtl1:		cdtl1.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/cdtl1
		-cp cdtl1.sh cdtl1
		chmod +x cdtl1
		cp cdtl1 $(SHELLBIN)/cdtl1

cdbtl1:		cdbtl1.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/cdbtl1
		-cp cdbtl1.sh cdbtl1
		chmod +x cdbtl1
		cp cdbtl1 $(SHELLBIN)/cdbtl1

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f lcslabid
		-rm -f cdtl1
		-rm -f cdbtl1

backup:		lcslabid.sh \
		cdtl1.sh \
		cdbtl1.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp lcslabid.sh $(BACKUPDIR)/lcslabid.sh
		cp cdtl1.sh $(BACKUPDIR)/cdtl1.sh
		cp cdbtl1.sh $(BACKUPDIR)/cdbtl1.sh
		touch backup

backupd:		lcslabid.sh \
		cdtl1.sh \
		cdbtl1.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		lcslabid.sh \
		cdtl1.sh \
		cdbtl1.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			lcslabid.sh \
			cdtl1.sh \
			cdbtl1.sh \
			$(ENDLIST)
