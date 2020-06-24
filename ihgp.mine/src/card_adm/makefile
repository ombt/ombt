
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

all:		card_adm \
		$(ENDLIST)
		touch all

card_adm:		card_adm.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/card_adm
		cp card_adm.sh card_adm
		chmod +x card_adm
		cp card_adm $(SHELLBIN)/card_adm
		rcp card_adm rumore@3gmsclsp:/home/rumore/bin
		rcp card_adm rumore@ihlssb1:/home/rumore/bin

clean:
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f card_adm

backup:		card_adm.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp card_adm.sh $(BACKUPDIR)/card_adm.sh
		touch backup

print:		card_adm.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			card_adm.sh \
			$(ENDLIST)
