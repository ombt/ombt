
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

all:		ftpsp \
		tl1sh \
		mytl1 \
		telnetsp \
		pingsp \
		$(ENDLIST)
		touch all

ftpsp:		ftpsp.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/ftpsp
		-cp ftpsp.sh ftpsp
		chmod +x ftpsp
		cp ftpsp $(SHELLBIN)/ftpsp

tl1sh:		tl1sh.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/tl1sh
		-cp tl1sh.sh tl1sh
		chmod +x tl1sh
		cp tl1sh $(SHELLBIN)/tl1sh

mytl1:		mytl1.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/mytl1
		-cp mytl1.sh mytl1
		chmod +x mytl1
		cp mytl1 $(SHELLBIN)/mytl1

telnetsp:		telnetsp.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/telnetsp
		-cp telnetsp.sh telnetsp
		chmod +x telnetsp
		cp telnetsp $(SHELLBIN)/telnetsp

pingsp:		pingsp.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/pingsp
		-cp pingsp.sh pingsp
		chmod +x pingsp
		cp pingsp $(SHELLBIN)/pingsp

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f ftpsp
		-rm -f tl1sh
		-rm -f mytl1
		-rm -f telnetsp
		-rm -f pingsp

backup:		mytl1.sh \
		ftpsp.sh \
		pingsp.sh \
		tl1sh.sh \
		telnetsp.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp mytl1.sh $(BACKUPDIR)/mytl1.sh
		cp ftpsp.sh $(BACKUPDIR)/ftpsp.sh
		cp pingsp.sh $(BACKUPDIR)/pingsp.sh
		cp tl1sh.sh $(BACKUPDIR)/tl1sh.sh
		cp telnetsp.sh $(BACKUPDIR)/telnetsp.sh
		touch backup

backupd:		mytl1.sh \
		ftpsp.sh \
		pingsp.sh \
		tl1sh.sh \
		telnetsp.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		mytl1.sh \
		ftpsp.sh \
		pingsp.sh \
		tl1sh.sh \
		telnetsp.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			mytl1.sh \
			ftpsp.sh \
			pingsp.sh \
			tl1sh.sh \
			telnetsp.sh \
			$(ENDLIST)
