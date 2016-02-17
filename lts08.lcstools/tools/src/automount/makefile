
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

all:		make.auto.spfs \
		$(ENDLIST)
		touch all

make.auto.spfs:		make.auto.spfs.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/make.auto.spfs
		-cp make.auto.spfs.sh make.auto.spfs
		chmod +x make.auto.spfs
		cp make.auto.spfs $(SHELLBIN)/make.auto.spfs

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f make.auto.spfs

backup:		make.auto.spfs.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp make.auto.spfs.sh $(BACKUPDIR)/make.auto.spfs.sh
		touch backup

backupd:		make.auto.spfs.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		make.auto.spfs.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			make.auto.spfs.sh \
			$(ENDLIST)
