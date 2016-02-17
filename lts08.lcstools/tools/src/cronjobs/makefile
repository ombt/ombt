
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

all:		cronjobs \
		$(ENDLIST)
		touch all

cronjobs:		cronjobs.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/cronjobs
		-cp cronjobs.sh cronjobs
		chmod +x cronjobs
		cp cronjobs $(SHELLBIN)/cronjobs
		./cronjobs

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f cronjobs

backup:		cronjobs.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp cronjobs.sh $(BACKUPDIR)/cronjobs.sh
		touch backup

backupd:		cronjobs.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		cronjobs.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			cronjobs.sh \
			$(ENDLIST)
