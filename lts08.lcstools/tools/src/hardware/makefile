
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

all:		hw2ioms \
		mkhwscripts \
		hw2enet \
		hw2chassis \
		cd2route \
		hw2tdm \
		$(ENDLIST)
		touch all

hw2ioms:		hw2ioms.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/hw2ioms
		-cp hw2ioms.sh hw2ioms
		chmod +x hw2ioms
		cp hw2ioms $(SHELLBIN)/hw2ioms

mkhwscripts:		mkhwscripts.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/mkhwscripts
		-cp mkhwscripts.sh mkhwscripts
		chmod +x mkhwscripts
		cp mkhwscripts $(SHELLBIN)/mkhwscripts

hw2enet:		hw2enet.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/hw2enet
		-cp hw2enet.sh hw2enet
		chmod +x hw2enet
		cp hw2enet $(SHELLBIN)/hw2enet

hw2chassis:		hw2chassis.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/hw2chassis
		-cp hw2chassis.sh hw2chassis
		chmod +x hw2chassis
		cp hw2chassis $(SHELLBIN)/hw2chassis

cd2route:		cd2route.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/cd2route
		-cp cd2route.sh cd2route
		chmod +x cd2route
		cp cd2route $(SHELLBIN)/cd2route

hw2tdm:		hw2tdm.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/hw2tdm
		-cp hw2tdm.sh hw2tdm
		chmod +x hw2tdm
		cp hw2tdm $(SHELLBIN)/hw2tdm

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f hw2ioms
		-rm -f mkhwscripts
		-rm -f hw2enet
		-rm -f hw2chassis
		-rm -f cd2route
		-rm -f hw2tdm

backup:		cd2route.sh \
		hw2chassis.sh \
		mkhwscripts.sh \
		hw2enet.sh \
		hw2ioms.sh \
		hw2tdm.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp cd2route.sh $(BACKUPDIR)/cd2route.sh
		cp hw2chassis.sh $(BACKUPDIR)/hw2chassis.sh
		cp mkhwscripts.sh $(BACKUPDIR)/mkhwscripts.sh
		cp hw2enet.sh $(BACKUPDIR)/hw2enet.sh
		cp hw2ioms.sh $(BACKUPDIR)/hw2ioms.sh
		cp hw2tdm.sh $(BACKUPDIR)/hw2tdm.sh
		touch backup

backupd:		cd2route.sh \
		hw2chassis.sh \
		mkhwscripts.sh \
		hw2enet.sh \
		hw2ioms.sh \
		hw2tdm.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		cd2route.sh \
		hw2chassis.sh \
		mkhwscripts.sh \
		hw2enet.sh \
		hw2ioms.sh \
		hw2tdm.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			cd2route.sh \
			hw2chassis.sh \
			mkhwscripts.sh \
			hw2enet.sh \
			hw2ioms.sh \
			hw2tdm.sh \
			$(ENDLIST)
