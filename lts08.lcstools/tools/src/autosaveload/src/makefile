
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
SHELLBIN = ../bin
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

all:		getloads \
		autosaveload \
		installcrontab \
		$(ENDLIST)
		touch all

getloads:		getloads.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/getloads
		-cp getloads.sh getloads
		chmod +x getloads
		cp getloads $(SHELLBIN)/getloads

autosaveload:		autosaveload.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/autosaveload
		-cp autosaveload.sh autosaveload
		chmod +x autosaveload
		cp autosaveload $(SHELLBIN)/autosaveload

installcrontab:		installcrontab.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/installcrontab
		-cp installcrontab.sh installcrontab
		chmod +x installcrontab
		cp installcrontab $(SHELLBIN)/installcrontab

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f getloads
		-rm -f autosaveload
		-rm -f installcrontab

backup:		autosaveload.sh \
		installcrontab.sh \
		getloads.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp autosaveload.sh $(BACKUPDIR)/autosaveload.sh
		cp installcrontab.sh $(BACKUPDIR)/installcrontab.sh
		cp getloads.sh $(BACKUPDIR)/getloads.sh
		touch backup

backupd:		autosaveload.sh \
		installcrontab.sh \
		getloads.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		autosaveload.sh \
		installcrontab.sh \
		getloads.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			autosaveload.sh \
			installcrontab.sh \
			getloads.sh \
			$(ENDLIST)
