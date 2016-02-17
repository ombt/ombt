
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

all:		unlock \
		lock \
		lockadm \
		$(ENDLIST)
		touch all

unlock:		unlock.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/unlock
		-cp unlock.sh unlock
		chmod +x unlock
		cp unlock $(SHELLBIN)/unlock
		-(rm $(HOME)/tools/bin/lcsunlock)
		ln $(HOME)/tools/bin/suidcmd $(HOME)/tools/bin/lcsunlock

lock:		lock.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/lock
		-cp lock.sh lock
		chmod +x lock
		cp lock $(SHELLBIN)/lock
		-(rm $(HOME)/tools/bin/lcslock)
		ln $(HOME)/tools/bin/suidcmd $(HOME)/tools/bin/lcslock

lockadm:		lockadm.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/lockadm
		-cp lockadm.sh lockadm
		chmod +x lockadm
		cp lockadm $(SHELLBIN)/lockadm
		-(rm $(HOME)/tools/bin/lcslockadm)
		ln $(HOME)/tools/bin/suidcmd $(HOME)/tools/bin/lcslockadm

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f unlock
		-rm -f lock
		-rm -f lockadm

backup:		unlock.sh \
		lockadm.sh \
		lock.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp unlock.sh $(BACKUPDIR)/unlock.sh
		cp lockadm.sh $(BACKUPDIR)/lockadm.sh
		cp lock.sh $(BACKUPDIR)/lock.sh
		touch backup

backupd:		unlock.sh \
		lockadm.sh \
		lock.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		unlock.sh \
		lockadm.sh \
		lock.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			unlock.sh \
			lockadm.sh \
			lock.sh \
			$(ENDLIST)
