
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

all:		tl1cmd \
		tl1exec \
		$(ENDLIST)
		touch all

tl1cmd:		tl1cmd.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/tl1cmd
		-cp tl1cmd.sh tl1cmd
		chmod +x tl1cmd
		cp tl1cmd $(SHELLBIN)/tl1cmd

tl1exec:		tl1exec.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/tl1exec
		-cp tl1exec.sh tl1exec
		chmod +x tl1exec
		cp tl1exec $(SHELLBIN)/tl1exec

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f tl1cmd
		-rm -f tl1exec

backup:		tl1cmd.sh \
		tl1exec.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp tl1cmd.sh $(BACKUPDIR)/tl1cmd.sh
		cp tl1exec.sh $(BACKUPDIR)/tl1exec.sh
		touch backup

backupd:		tl1cmd.sh \
		tl1exec.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		tl1cmd.sh \
		tl1exec.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			tl1cmd.sh \
			tl1exec.sh \
			$(ENDLIST)
