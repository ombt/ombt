
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

all:		pkg_mkindex \
		$(ENDLIST)
		touch all

pkg_mkindex:		pkg_mkindex.TCL \
		$(ENDLIST)
		-rm -f $(TCLBIN)/pkg_mkindex
		-cp pkg_mkindex.TCL pkg_mkindex
		chmod +x pkg_mkindex
		cp pkg_mkindex $(TCLBIN)/pkg_mkindex

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f pkg_mkindex

backup:		pkg_mkindex.TCL \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp pkg_mkindex.TCL $(BACKUPDIR)/pkg_mkindex.TCL
		touch backup

backupd:		pkg_mkindex.TCL \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		pkg_mkindex.TCL \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			pkg_mkindex.TCL \
			$(ENDLIST)
