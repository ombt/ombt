
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = 
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

all:		mymk \
		$(ENDLIST)
		touch all

mymk:		mymk.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/mymk
		cp mymk.sh mymk
		chmod +x mymk
		cp mymk $(SHELLBIN)/mymk
		rcp mymk integ@lts00:/home/integ/bin
		rcp mymk integ@ihlbs1:/home/integ/bin

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f mymk

backup:		mymk.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp mymk.sh $(BACKUPDIR)/mymk.sh
		touch backup

print:		mymk.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			mymk.sh \
			$(ENDLIST)
