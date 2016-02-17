
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

all:		spectralog \
		$(ENDLIST)
		touch all

spectralog:		spectralog.PL \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/spectralog
		-cp spectralog.PL spectralog
		chmod +x spectralog
		cp spectralog $(SHELLBIN)/spectralog

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f spectralog

backup:		spectralog.PL \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp spectralog.PL $(BACKUPDIR)/spectralog.PL
		touch backup

backupd:		spectralog.PL \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		spectralog.PL \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			spectralog.PL \
			$(ENDLIST)
