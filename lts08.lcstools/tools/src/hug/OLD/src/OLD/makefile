
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = 
LD = cc
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/tools/bin/hug
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

all:		lcsupsh \
		$(ENDLIST)
		touch all

lcsupsh:		lcsupsh.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/lcsupsh
		-cp lcsupsh.sh lcsupsh
		chmod +x lcsupsh
		cp lcsupsh $(SHELLBIN)/lcsupsh

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f lcsupsh

backup:		lcsupsh.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp lcsupsh.sh $(BACKUPDIR)/lcsupsh.sh
		touch backup

print:		lcsupsh.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			lcsupsh.sh \
			$(ENDLIST)
