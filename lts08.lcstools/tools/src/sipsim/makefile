
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

all:		mypqueue.pm \
		sipsim \
		$(ENDLIST)
		touch all

mypqueue.pm:		mypqueue.PM \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/mypqueue.pm
		-cp mypqueue.PM mypqueue.pm
		chmod +x mypqueue.pm
		cp mypqueue.pm $(SHELLBIN)/mypqueue.pm
		-(rcp mypqueue.PM root@135.1.235.58:/SIPSIM/src)

sipsim:		sipsim.PL \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/sipsim
		-cp sipsim.PL sipsim
		chmod +x sipsim
		cp sipsim $(SHELLBIN)/sipsim
		-(sed "s?/opt/exp/bin/perl?/opt/OV/bin/Perl/bin/perl?" sipsim.PL >/tmp/remotesipsim.PL)
		-(rcp /tmp/remotesipsim.PL root@135.1.235.58:/SIPSIM/src/sipsim.PL)

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f mypqueue.pm
		-rm -f sipsim

backup:		sipsim.PL \
		mypqueue.PM \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp sipsim.PL $(BACKUPDIR)/sipsim.PL
		cp mypqueue.PM $(BACKUPDIR)/mypqueue.PM
		touch backup

backupd:		sipsim.PL \
		mypqueue.PM \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		sipsim.PL \
		mypqueue.PM \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			sipsim.PL \
			mypqueue.PM \
			$(ENDLIST)
