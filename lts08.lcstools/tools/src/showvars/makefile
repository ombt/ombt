
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

all:		showvars \
		$(ENDLIST)
		touch all

showvars:		showvars.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/showvars
		-cp showvars.sh showvars
		chmod +x showvars
		cp showvars $(SHELLBIN)/showvars
		-(rm $(HOME)/tools/bin/lcsshowvars)
		ln $(HOME)/tools/bin/suidcmd $(HOME)/tools/bin/lcsshowvars

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f showvars

backup:		showvars.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp showvars.sh $(BACKUPDIR)/showvars.sh
		touch backup

backupd:		showvars.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		showvars.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			showvars.sh \
			$(ENDLIST)
