
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

all:		tarsplogs \
		tarspcores \
		untarcmcores \
		untarcmlogs \
		untarspcores \
		tarcmlogs \
		untarsplogs \
		tarcmcores \
		tariomlogs \
		$(ENDLIST)
		touch all

tarsplogs:		tarsplogs.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/tarsplogs
		-cp tarsplogs.sh tarsplogs
		chmod +x tarsplogs
		cp tarsplogs $(SHELLBIN)/tarsplogs

tarspcores:		tarspcores.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/tarspcores
		-cp tarspcores.sh tarspcores
		chmod +x tarspcores
		cp tarspcores $(SHELLBIN)/tarspcores

untarcmcores:		untarcmcores.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/untarcmcores
		-cp untarcmcores.sh untarcmcores
		chmod +x untarcmcores
		cp untarcmcores $(SHELLBIN)/untarcmcores

untarcmlogs:		untarcmlogs.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/untarcmlogs
		-cp untarcmlogs.sh untarcmlogs
		chmod +x untarcmlogs
		cp untarcmlogs $(SHELLBIN)/untarcmlogs

untarspcores:		untarspcores.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/untarspcores
		-cp untarspcores.sh untarspcores
		chmod +x untarspcores
		cp untarspcores $(SHELLBIN)/untarspcores

tarcmlogs:		tarcmlogs.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/tarcmlogs
		-cp tarcmlogs.sh tarcmlogs
		chmod +x tarcmlogs
		cp tarcmlogs $(SHELLBIN)/tarcmlogs

untarsplogs:		untarsplogs.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/untarsplogs
		-cp untarsplogs.sh untarsplogs
		chmod +x untarsplogs
		cp untarsplogs $(SHELLBIN)/untarsplogs

tarcmcores:		tarcmcores.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/tarcmcores
		-cp tarcmcores.sh tarcmcores
		chmod +x tarcmcores
		cp tarcmcores $(SHELLBIN)/tarcmcores

tariomlogs:		tariomlogs.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/tariomlogs
		-cp tariomlogs.sh tariomlogs
		chmod +x tariomlogs
		cp tariomlogs $(SHELLBIN)/tariomlogs

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f tarsplogs
		-rm -f tarspcores
		-rm -f untarcmcores
		-rm -f untarcmlogs
		-rm -f untarspcores
		-rm -f tarcmlogs
		-rm -f untarsplogs
		-rm -f tarcmcores
		-rm -f tariomlogs

backup:		untarspcores.sh \
		untarcmlogs.sh \
		tarsplogs.sh \
		tarcmcores.sh \
		tariomlogs.sh \
		untarsplogs.sh \
		untarcmcores.sh \
		tarcmlogs.sh \
		tarspcores.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp untarspcores.sh $(BACKUPDIR)/untarspcores.sh
		cp untarcmlogs.sh $(BACKUPDIR)/untarcmlogs.sh
		cp tarsplogs.sh $(BACKUPDIR)/tarsplogs.sh
		cp tarcmcores.sh $(BACKUPDIR)/tarcmcores.sh
		cp tariomlogs.sh $(BACKUPDIR)/tariomlogs.sh
		cp untarsplogs.sh $(BACKUPDIR)/untarsplogs.sh
		cp untarcmcores.sh $(BACKUPDIR)/untarcmcores.sh
		cp tarcmlogs.sh $(BACKUPDIR)/tarcmlogs.sh
		cp tarspcores.sh $(BACKUPDIR)/tarspcores.sh
		touch backup

backupd:		untarspcores.sh \
		untarcmlogs.sh \
		tarsplogs.sh \
		tarcmcores.sh \
		tariomlogs.sh \
		untarsplogs.sh \
		untarcmcores.sh \
		tarcmlogs.sh \
		tarspcores.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		untarspcores.sh \
		untarcmlogs.sh \
		tarsplogs.sh \
		tarcmcores.sh \
		tariomlogs.sh \
		untarsplogs.sh \
		untarcmcores.sh \
		tarcmlogs.sh \
		tarspcores.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			untarspcores.sh \
			untarcmlogs.sh \
			tarsplogs.sh \
			tarcmcores.sh \
			tariomlogs.sh \
			untarsplogs.sh \
			untarcmcores.sh \
			tarcmlogs.sh \
			tarspcores.sh \
			$(ENDLIST)
