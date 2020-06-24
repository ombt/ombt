
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

all:		mybsplit \
		$(ENDLIST)
		touch all

mybsplit:		mybsplit.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o mybsplit \
			mybsplit.o \
			$(ENDLIST)
		cp mybsplit $(HOME)/bin

mybsplit.o:	mybsplit.c \
		$(ENDLIST)

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-rm -f mybsplit
		-rm -f mybsplit.o

backup:		mybsplit.c \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp mybsplit.c $(BACKUPDIR)/mybsplit.c
		touch backup

print:		mybsplit.c \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			mybsplit.c \
			$(ENDLIST)
