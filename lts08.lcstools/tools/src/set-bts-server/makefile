
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

all:		set-bts-server \
		$(ENDLIST)
		touch all

set-bts-server:		set-bts-server.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/set-bts-server
		-cp set-bts-server.sh set-bts-server
		chmod +x set-bts-server
		cp set-bts-server $(SHELLBIN)/set-bts-server

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f set-bts-server

backup:		set-bts-server.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp set-bts-server.sh $(BACKUPDIR)/set-bts-server.sh
		touch backup

backupd:		set-bts-server.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		set-bts-server.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			set-bts-server.sh \
			$(ENDLIST)
