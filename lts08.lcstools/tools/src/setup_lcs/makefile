
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

all:		setup_lcshug \
		setup_lcs \
		$(ENDLIST)
		touch all

setup_lcshug:		setup_lcshug.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/setup_lcshug
		-cp setup_lcshug.sh setup_lcshug
		chmod +x setup_lcshug
		cp setup_lcshug $(SHELLBIN)/setup_lcshug

setup_lcs:		setup_lcs.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/setup_lcs
		-cp setup_lcs.sh setup_lcs
		chmod +x setup_lcs
		cp setup_lcs $(SHELLBIN)/setup_lcs

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f setup_lcshug
		-rm -f setup_lcs

backup:		setup_lcs.sh \
		setup_lcshug.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp setup_lcs.sh $(BACKUPDIR)/setup_lcs.sh
		cp setup_lcshug.sh $(BACKUPDIR)/setup_lcshug.sh
		touch backup

backupd:		setup_lcs.sh \
		setup_lcshug.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		setup_lcs.sh \
		setup_lcshug.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			setup_lcs.sh \
			setup_lcshug.sh \
			$(ENDLIST)
