
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
SHELLBIN = $(HOME)/bin
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

all:		auto_mkindex \
		$(ENDLIST)
		touch all

auto_mkindex:		auto_mkindex.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/auto_mkindex
		-cp auto_mkindex.TCL auto_mkindex
		chmod +x auto_mkindex
		cp auto_mkindex $(TCLBIN)/auto_mkindex

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f auto_mkindex

backup:		auto_mkindex.TCL \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp auto_mkindex.TCL $(BACKUPDIR)/auto_mkindex.TCL
		touch backup

backupd:		auto_mkindex.TCL \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		auto_mkindex.TCL \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			auto_mkindex.TCL \
			$(ENDLIST)
