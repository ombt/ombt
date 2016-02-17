
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

all:		lcsksh \
		$(ENDLIST)
		touch all

lcsksh:		lcsksh.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/lcsksh
		-cp lcsksh.sh lcsksh
		chmod +x lcsksh
		cp lcsksh $(SHELLBIN)/lcsksh

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f lcsksh

backup:		lcsksh.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp lcsksh.sh $(BACKUPDIR)/lcsksh.sh
		touch backup

backupd:		lcsksh.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		lcsksh.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			lcsksh.sh \
			$(ENDLIST)
