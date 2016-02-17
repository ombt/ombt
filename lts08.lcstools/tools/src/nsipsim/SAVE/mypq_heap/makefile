
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = 
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

all:		mypqueue.pm \
		nsipsimadm \
		mytimer.pm \
		nsipsim \
		$(ENDLIST)
		touch all

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
		-(sed "s?/opt/exp/bin/perl?/opt/OV/bin/Perl/bin/perl?" nsipsimadm.PL >/tmp/remotensipsimadm.PL)
		-(sed "s?/home/lcstools/tools/bin?/NSIPSIM/bin?" /tmp/remotensipsimadm.PL >/tmp/remotensipsimadm.PL)
		-(rcp /tmp/remotensipsimadm.PL root@135.1.235.58:/NSIPSIM/src/nsipsimadm.PL)

mytimer.pm:		mytimer.PM \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/mytimer.pm
		-cp mytimer.PM mytimer.pm
		chmod +x mytimer.pm
		cp mytimer.pm $(SHELLBIN)/mytimer.pm
		-(rcp mytimer.PM root@135.1.235.58:/NSIPSIM/src)

nsipsim:		nsipsim.PL \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/nsipsim
		-cp nsipsim.PL nsipsim
		chmod +x nsipsim
		cp nsipsim $(SHELLBIN)/nsipsim
		-(sed "s?/opt/exp/bin/perl?/opt/OV/bin/Perl/bin/perl?" nsipsim.PL >/tmp/remotensipsim.PL)
		-(sed "s?/home/lcstools/tools/bin?/NSIPSIM/bin?" /tmp/remotensipsim.PL >/tmp/remotensipsim2.PL)
		-(rcp /tmp/remotensipsim2.PL root@135.1.235.58:/NSIPSIM/src/nsipsim.PL)

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f mypqueue.pm
		-rm -f nsipsimadm
		-rm -f mytimer.pm
		-rm -f nsipsim

backup:		nsipsimadm.PL \
		nsipsim.PL \
		mytimer.PM \
		mypqueue.PM \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp nsipsimadm.PL $(BACKUPDIR)/nsipsimadm.PL
		cp nsipsim.PL $(BACKUPDIR)/nsipsim.PL
		cp mytimer.PM $(BACKUPDIR)/mytimer.PM
		cp mypqueue.PM $(BACKUPDIR)/mypqueue.PM
		touch backup

print:		nsipsimadm.PL \
		nsipsim.PL \
		mytimer.PM \
		mypqueue.PM \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			nsipsimadm.PL \
			nsipsim.PL \
			mytimer.PM \
			mypqueue.PM \
			$(ENDLIST)
