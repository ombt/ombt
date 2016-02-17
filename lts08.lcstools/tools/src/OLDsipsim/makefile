
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

all:		sndrcvudp \
		mypqueue.pm \
		refersim \
		sendudp \
		sipit \
		sipsim \
		$(ENDLIST)
		touch all

sndrcvudp:		sndrcvudp.pl \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/sndrcvudp
		-cp sndrcvudp.pl sndrcvudp
		chmod +x sndrcvudp
		cp sndrcvudp $(SHELLBIN)/sndrcvudp

mypqueue.pm:		mypqueue.PM \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/mypqueue.pm
		-cp mypqueue.PM mypqueue.pm
		chmod +x mypqueue.pm
		cp mypqueue.pm $(SHELLBIN)/mypqueue.pm

refersim:		refersim.PL \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/refersim
		-cp refersim.PL refersim
		chmod +x refersim
		cp refersim $(SHELLBIN)/refersim

sendudp:		sendudp.pl \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/sendudp
		-cp sendudp.pl sendudp
		chmod +x sendudp
		cp sendudp $(SHELLBIN)/sendudp

sipit:		sipit.PL \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/sipit
		-cp sipit.PL sipit
		chmod +x sipit
		cp sipit $(SHELLBIN)/sipit

sipsim:		sipsim.PL \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/sipsim
		-cp sipsim.PL sipsim
		chmod +x sipsim
		cp sipsim $(SHELLBIN)/sipsim

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f sndrcvudp
		-rm -f mypqueue.pm
		-rm -f refersim
		-rm -f sendudp
		-rm -f sipit
		-rm -f sipsim

backup:		sndrcvudp.pl \
		sipsim.PL \
		sendudp.pl \
		mypqueue.PM \
		sipit.PL \
		refersim.PL \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp sndrcvudp.pl $(BACKUPDIR)/sndrcvudp.pl
		cp sipsim.PL $(BACKUPDIR)/sipsim.PL
		cp sendudp.pl $(BACKUPDIR)/sendudp.pl
		cp mypqueue.PM $(BACKUPDIR)/mypqueue.PM
		cp sipit.PL $(BACKUPDIR)/sipit.PL
		cp refersim.PL $(BACKUPDIR)/refersim.PL
		touch backup

print:		sndrcvudp.pl \
		sipsim.PL \
		sendudp.pl \
		mypqueue.PM \
		sipit.PL \
		refersim.PL \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			sndrcvudp.pl \
			sipsim.PL \
			sendudp.pl \
			mypqueue.PM \
			sipit.PL \
			refersim.PL \
			$(ENDLIST)
