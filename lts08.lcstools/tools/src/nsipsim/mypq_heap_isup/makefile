
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

all:		nsipsimtcp \
		nsipsimhack \
		nsipsimisup \
		testisupmsgs \
		mypqueue.pm \
		nsipsimadm \
		myisup.pm \
		prtisupmsgs \
		mytimer.pm \
		tisup \
		$(ENDLIST)
		touch all

nsipsimtcp:		nsipsimtcp.PL \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/nsipsimtcp
		-cp nsipsimtcp.PL nsipsimtcp
		chmod +x nsipsimtcp
		cp nsipsimtcp $(SHELLBIN)/nsipsimtcp
		-(rcp nsipsimtcp.PL root@135.1.235.58:/NSIPSIM/src)

nsipsimhack:		nsipsimhack.PL \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/nsipsimhack
		-cp nsipsimhack.PL nsipsimhack
		chmod +x nsipsimhack
		cp nsipsimhack $(SHELLBIN)/nsipsimhack
		-(rcp nsipsimhack.PL root@135.1.235.58:/NSIPSIM/src)

nsipsimisup:		nsipsimisup.PL \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/nsipsimisup
		-cp nsipsimisup.PL nsipsimisup
		chmod +x nsipsimisup
		cp nsipsimisup $(SHELLBIN)/nsipsimisup
		-(rcp nsipsimisup.PL root@135.1.235.58:/NSIPSIM/src)

testisupmsgs:		testisupmsgs.PL \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/testisupmsgs
		-cp testisupmsgs.PL testisupmsgs
		chmod +x testisupmsgs
		cp testisupmsgs $(SHELLBIN)/testisupmsgs
		-(rcp testisupmsgs.PL root@135.1.235.58:/NSIPSIM/src)

mypqueue.pm:		mypqueue.PM \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/mypqueue.pm
		-cp mypqueue.PM mypqueue.pm
		chmod +x mypqueue.pm
		cp mypqueue.pm $(SHELLBIN)/mypqueue.pm
		-(rcp mypqueue.PM root@135.1.235.58:/NSIPSIM/src)

nsipsimadm:		nsipsimadm.PL \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/nsipsimadm
		-cp nsipsimadm.PL nsipsimadm
		chmod +x nsipsimadm
		cp nsipsimadm $(SHELLBIN)/nsipsimadm
		-(rcp nsipsimadm.PL root@135.1.235.58:/NSIPSIM/src)

myisup.pm:		myisup.PM \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/myisup.pm
		-cp myisup.PM myisup.pm
		chmod +x myisup.pm
		cp myisup.pm $(SHELLBIN)/myisup.pm
		-(rcp myisup.PM root@135.1.235.58:/NSIPSIM/src)

prtisupmsgs:		prtisupmsgs.PL \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/prtisupmsgs
		-cp prtisupmsgs.PL prtisupmsgs
		chmod +x prtisupmsgs
		cp prtisupmsgs $(SHELLBIN)/prtisupmsgs
		-(rcp prtisupmsgs.PL root@135.1.235.58:/NSIPSIM/src)

mytimer.pm:		mytimer.PM \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/mytimer.pm
		-cp mytimer.PM mytimer.pm
		chmod +x mytimer.pm
		cp mytimer.pm $(SHELLBIN)/mytimer.pm
		-(rcp mytimer.PM root@135.1.235.58:/NSIPSIM/src)

tisup:		tisup.PL \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/tisup
		-cp tisup.PL tisup
		chmod +x tisup
		cp tisup $(SHELLBIN)/tisup

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f nsipsimtcp
		-rm -f nsipsimhack
		-rm -f nsipsimisup
		-rm -f testisupmsgs
		-rm -f mypqueue.pm
		-rm -f nsipsimadm
		-rm -f myisup.pm
		-rm -f prtisupmsgs
		-rm -f mytimer.pm
		-rm -f tisup

backup:		myisup.PM \
		nsipsimadm.PL \
		testisupmsgs.PL \
		nsipsimhack.PL \
		mytimer.PM \
		tisup.PL \
		nsipsimtcp.PL \
		prtisupmsgs.PL \
		mypqueue.PM \
		nsipsimisup.PL \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp myisup.PM $(BACKUPDIR)/myisup.PM
		cp nsipsimadm.PL $(BACKUPDIR)/nsipsimadm.PL
		cp testisupmsgs.PL $(BACKUPDIR)/testisupmsgs.PL
		cp nsipsimhack.PL $(BACKUPDIR)/nsipsimhack.PL
		cp mytimer.PM $(BACKUPDIR)/mytimer.PM
		cp tisup.PL $(BACKUPDIR)/tisup.PL
		cp nsipsimtcp.PL $(BACKUPDIR)/nsipsimtcp.PL
		cp prtisupmsgs.PL $(BACKUPDIR)/prtisupmsgs.PL
		cp mypqueue.PM $(BACKUPDIR)/mypqueue.PM
		cp nsipsimisup.PL $(BACKUPDIR)/nsipsimisup.PL
		touch backup

backupd:		myisup.PM \
		nsipsimadm.PL \
		testisupmsgs.PL \
		nsipsimhack.PL \
		mytimer.PM \
		tisup.PL \
		nsipsimtcp.PL \
		prtisupmsgs.PL \
		mypqueue.PM \
		nsipsimisup.PL \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		myisup.PM \
		nsipsimadm.PL \
		testisupmsgs.PL \
		nsipsimhack.PL \
		mytimer.PM \
		tisup.PL \
		nsipsimtcp.PL \
		prtisupmsgs.PL \
		mypqueue.PM \
		nsipsimisup.PL \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			myisup.PM \
			nsipsimadm.PL \
			testisupmsgs.PL \
			nsipsimhack.PL \
			mytimer.PM \
			tisup.PL \
			nsipsimtcp.PL \
			prtisupmsgs.PL \
			mypqueue.PM \
			nsipsimisup.PL \
			$(ENDLIST)
