
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

all:		syslog \
		comparefiles \
		spaudit \
		reboot \
		reload \
		nreload \
		$(ENDLIST)
		touch all

syslog:		syslog.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/syslog
		-cp syslog.sh syslog
		chmod +x syslog
		cp syslog $(SHELLBIN)/syslog
		-(rm $(HOME)/tools/bin/lcssyslog)
		ln $(HOME)/tools/bin/suidcmd $(HOME)/tools/bin/lcssyslog

comparefiles:		comparefiles.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/comparefiles
		-cp comparefiles.sh comparefiles
		chmod +x comparefiles
		cp comparefiles $(SHELLBIN)/comparefiles
		-(rm $(HOME)/tools/bin/comparefiles)
		cp $(HOME)/tools/src/reload/comparefiles $(HOME)/tools/bin/comparefiles

spaudit:		spaudit.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/spaudit
		-cp spaudit.sh spaudit
		chmod +x spaudit
		cp spaudit $(SHELLBIN)/spaudit
		-(rm $(HOME)/tools/bin/lcsspaudit)
		ln $(HOME)/tools/bin/suidcmd $(HOME)/tools/bin/lcsspaudit

reboot:		reboot.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/reboot
		-cp reboot.sh reboot
		chmod +x reboot
		cp reboot $(SHELLBIN)/reboot
		-(rm $(HOME)/tools/bin/lcsreboot)
		ln $(HOME)/tools/bin/suidcmd $(HOME)/tools/bin/lcsreboot

reload:		reload.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/reload
		-cp reload.sh reload
		chmod +x reload
		cp reload $(SHELLBIN)/reload
		-(rm $(HOME)/tools/bin/lcsreload)
		ln $(HOME)/tools/bin/suidcmd $(HOME)/tools/bin/lcsreload

nreload:		nreload.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/nreload
		-cp nreload.sh nreload
		chmod +x nreload
		cp nreload $(SHELLBIN)/nreload
		-(rm $(HOME)/tools/bin/lcsnreload)
		ln $(HOME)/tools/bin/suidcmd $(HOME)/tools/bin/lcsnreload

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f syslog
		-rm -f comparefiles
		-rm -f spaudit
		-rm -f reboot
		-rm -f reload
		-rm -f nreload

backup:		nreload.sh \
		reboot.sh \
		spaudit.sh \
		reload.sh \
		comparefiles.sh \
		syslog.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp nreload.sh $(BACKUPDIR)/nreload.sh
		cp reboot.sh $(BACKUPDIR)/reboot.sh
		cp spaudit.sh $(BACKUPDIR)/spaudit.sh
		cp reload.sh $(BACKUPDIR)/reload.sh
		cp comparefiles.sh $(BACKUPDIR)/comparefiles.sh
		cp syslog.sh $(BACKUPDIR)/syslog.sh
		touch backup

backupd:		nreload.sh \
		reboot.sh \
		spaudit.sh \
		reload.sh \
		comparefiles.sh \
		syslog.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		nreload.sh \
		reboot.sh \
		spaudit.sh \
		reload.sh \
		comparefiles.sh \
		syslog.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			nreload.sh \
			reboot.sh \
			spaudit.sh \
			reload.sh \
			comparefiles.sh \
			syslog.sh \
			$(ENDLIST)
