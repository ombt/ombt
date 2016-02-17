
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

all:		getspips \
		$(ENDLIST)
		touch all

getspips:		getspips.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/getspips
		-cp getspips.sh getspips
		chmod +x getspips
		cp getspips $(SHELLBIN)/getspips

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f getspips

backup:		getspips.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp getspips.sh $(BACKUPDIR)/getspips.sh
		touch backup

backupd:		getspips.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		backup -d
		touch backupd

mymk:
		mymk

print:		getspips.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			getspips.sh \
			$(ENDLIST)
