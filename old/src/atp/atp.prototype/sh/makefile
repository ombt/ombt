
.SUFFIXES:		.o .c .cpp

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
.cpp.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<

all:		atp \
		$(ENDLIST)
		touch all

atp:		atp.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/atp
		cp atp.sh atp
		chmod +x atp
		cp atp $(SHELLBIN)/atp

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-rm -f atp

backup:		atp.sh \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp atp.sh $(BACKUPDIR)/atp.sh
		touch backup

print:		atp.sh \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			atp.sh \
			$(ENDLIST)
