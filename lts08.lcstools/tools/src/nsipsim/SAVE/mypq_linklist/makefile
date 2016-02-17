
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

all:		tpq \
		mypqueue.pm \
		ttm \
		mytimer.pm \
		nsipsim \
		$(ENDLIST)
		touch all

tpq:		tpq.PL \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/tpq
		-cp tpq.PL tpq
		chmod +x tpq
		cp tpq $(SHELLBIN)/tpq

mypqueue.pm:		mypqueue.PM \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/mypqueue.pm
		-cp mypqueue.PM mypqueue.pm
		chmod +x mypqueue.pm
		cp mypqueue.pm $(SHELLBIN)/mypqueue.pm

ttm:		ttm.PL \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/ttm
		-cp ttm.PL ttm
		chmod +x ttm
		cp ttm $(SHELLBIN)/ttm

mytimer.pm:		mytimer.PM \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/mytimer.pm
		-cp mytimer.PM mytimer.pm
		chmod +x mytimer.pm
		cp mytimer.pm $(SHELLBIN)/mytimer.pm

nsipsim:		nsipsim.PL \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/nsipsim
		-cp nsipsim.PL nsipsim
		chmod +x nsipsim
		cp nsipsim $(SHELLBIN)/nsipsim

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f tpq
		-rm -f mypqueue.pm
		-rm -f ttm
		-rm -f mytimer.pm
		-rm -f nsipsim

backup:		nsipsim.PL \
		tpq.PL \
		mytimer.PM \
		ttm.PL \
		mypqueue.PM \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp nsipsim.PL $(BACKUPDIR)/nsipsim.PL
		cp tpq.PL $(BACKUPDIR)/tpq.PL
		cp mytimer.PM $(BACKUPDIR)/mytimer.PM
		cp ttm.PL $(BACKUPDIR)/ttm.PL
		cp mypqueue.PM $(BACKUPDIR)/mypqueue.PM
		touch backup

print:		nsipsim.PL \
		tpq.PL \
		mytimer.PM \
		ttm.PL \
		mypqueue.PM \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			nsipsim.PL \
			tpq.PL \
			mytimer.PM \
			ttm.PL \
			mypqueue.PM \
			$(ENDLIST)
