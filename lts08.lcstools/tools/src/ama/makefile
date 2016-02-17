
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

all:		amagen-oper \
		$(ENDLIST)
		touch all

amagen-oper:		amagen-oper.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/amagen-oper
		-cp amagen-oper.sh amagen-oper
		chmod +x amagen-oper
		cp amagen-oper $(SHELLBIN)/amagen-oper
		-(rm $(HOME)/tools/bin/amagen-on)
		-(rm $(HOME)/tools/bin/amagen-off)
		ln $(HOME)/tools/bin/amagen-oper $(HOME)/tools/bin/amagen-on
		ln $(HOME)/tools/bin/amagen-oper $(HOME)/tools/bin/amagen-off

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f amagen-oper

backup:		amagen-oper.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp amagen-oper.sh $(BACKUPDIR)/amagen-oper.sh
		touch backup

backupd:		amagen-oper.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		amagen-oper.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			amagen-oper.sh \
			$(ENDLIST)
