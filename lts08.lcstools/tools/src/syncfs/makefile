
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

all:		syncfs \
		syncfscrontab \
		$(ENDLIST)
		touch all

syncfs:		syncfs.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/syncfs
		-cp syncfs.sh syncfs
		chmod +x syncfs
		cp syncfs $(SHELLBIN)/syncfs
		-(rm $(HOME)/tools/bin/lcssyncfs)
		ln $(HOME)/tools/bin/suidcmd $(HOME)/tools/bin/lcssyncfs

syncfscrontab:		syncfscrontab.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/syncfscrontab
		-cp syncfscrontab.sh syncfscrontab
		chmod +x syncfscrontab
		cp syncfscrontab $(SHELLBIN)/syncfscrontab
		-(rm $(HOME)/tools/bin/syncfscrontab)
		cp syncfscrontab $(HOME)/tools/bin/syncfscrontab

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f syncfs
		-rm -f syncfscrontab

backup:		syncfs.sh \
		syncfscrontab.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp syncfs.sh $(BACKUPDIR)/syncfs.sh
		cp syncfscrontab.sh $(BACKUPDIR)/syncfscrontab.sh
		touch backup

backupd:		syncfs.sh \
		syncfscrontab.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		syncfs.sh \
		syncfscrontab.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			syncfs.sh \
			syncfscrontab.sh \
			$(ENDLIST)
