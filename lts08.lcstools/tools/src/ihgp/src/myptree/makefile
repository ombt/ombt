
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

all:		myptree \
		$(ENDLIST)
		touch all

myptree:		myptree.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/myptree
		cp myptree.sh myptree
		chmod +x myptree
		cp myptree $(SHELLBIN)/myptree
		chmod 755 myptree
		cp myptree /home/rumore/bin/myptree
		rcp myptree integ@lts00:/home/integ/bin/myptree
		rcp myptree integ@ihlbs1:/home/integ/bin/myptree

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-rm -f myptree

backup:		myptree.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp myptree.sh $(BACKUPDIR)/myptree.sh
		touch backup

print:		myptree.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			myptree.sh \
			$(ENDLIST)
