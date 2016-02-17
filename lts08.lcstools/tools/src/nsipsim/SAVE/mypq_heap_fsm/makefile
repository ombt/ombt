
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

all:		nsipsimadm \
		nsipsimfsm \
		$(ENDLIST)
		touch all

nsipsimadm:		nsipsimadm.PL \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/nsipsimadm
		-cp nsipsimadm.PL nsipsimadm
		chmod +x nsipsimadm
		cp nsipsimadm $(SHELLBIN)/nsipsimadm
		-(sed "s?/opt/exp/bin/perl?/opt/OV/bin/Perl/bin/perl?" nsipsimadm.PL >/tmp/remotensipsimadm.PL)
		-(sed "s?/home/lcstools/tools/bin?/NSIPSIM/bin?" /tmp/remotensipsimadm.PL >/tmp/remotensipsim2adm.PL)
		-(rcp /tmp/remotensipsim2adm.PL root@135.1.235.58:/NSIPSIM/src/nsipsimadm.PL)

nsipsimfsm:		nsipsimfsm.PL \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/nsipsimfsm
		-cp nsipsimfsm.PL nsipsimfsm
		chmod +x nsipsimfsm
		cp nsipsimfsm $(SHELLBIN)/nsipsimfsm
		-(sed "s?/opt/exp/bin/perl?/opt/OV/bin/Perl/bin/perl?" nsipsimfsm.PL >/tmp/remotensipsimfsm.PL)
		-(sed "s?/home/lcstools/tools/bin?/NSIPSIM/bin?" /tmp/remotensipsimfsm.PL >/tmp/remotensipsim2fsm.PL)
		-(rcp /tmp/remotensipsim2fsm.PL root@135.1.235.58:/NSIPSIM/src/nsipsimfsm.PL)

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f nsipsimadm
		-rm -f nsipsimfsm

backup:		nsipsimadm.PL \
		nsipsimfsm.PL \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp nsipsimadm.PL $(BACKUPDIR)/nsipsimadm.PL
		cp nsipsimfsm.PL $(BACKUPDIR)/nsipsimfsm.PL
		touch backup

print:		nsipsimadm.PL \
		nsipsimfsm.PL \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			nsipsimadm.PL \
			nsipsimfsm.PL \
			$(ENDLIST)
