
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

all:		lcspicks \
		lcstimer \
		lcslocks \
		spload \
		lcstimer2 \
		$(ENDLIST)
		touch all

lcspicks:		lcspicks.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/lcspicks
		-cp lcspicks.sh lcspicks
		chmod +x lcspicks
		cp lcspicks $(SHELLBIN)/lcspicks

lcstimer:		lcstimer.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/lcstimer
		-cp lcstimer.sh lcstimer
		chmod +x lcstimer
		cp lcstimer $(SHELLBIN)/lcstimer

lcslocks:		lcslocks.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/lcslocks
		-cp lcslocks.sh lcslocks
		chmod +x lcslocks
		cp lcslocks $(SHELLBIN)/lcslocks

spload:		spload.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/spload
		-cp spload.sh spload
		chmod +x spload
		cp spload $(SHELLBIN)/spload

lcstimer2:		lcstimer2.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/lcstimer2
		-cp lcstimer2.sh lcstimer2
		chmod +x lcstimer2
		cp lcstimer2 $(SHELLBIN)/lcstimer2

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f lcspicks
		-rm -f lcstimer
		-rm -f lcslocks
		-rm -f spload
		-rm -f lcstimer2

backup:		lcstimer.sh \
		lcslocks.sh \
		lcspicks.sh \
		spload.sh \
		lcstimer2.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp lcstimer.sh $(BACKUPDIR)/lcstimer.sh
		cp lcslocks.sh $(BACKUPDIR)/lcslocks.sh
		cp lcspicks.sh $(BACKUPDIR)/lcspicks.sh
		cp spload.sh $(BACKUPDIR)/spload.sh
		cp lcstimer2.sh $(BACKUPDIR)/lcstimer2.sh
		touch backup

backupd:		lcstimer.sh \
		lcslocks.sh \
		lcspicks.sh \
		spload.sh \
		lcstimer2.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		lcstimer.sh \
		lcslocks.sh \
		lcspicks.sh \
		spload.sh \
		lcstimer2.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			lcstimer.sh \
			lcslocks.sh \
			lcspicks.sh \
			spload.sh \
			lcstimer2.sh \
			$(ENDLIST)
