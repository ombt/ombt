
.SUFFIXES:		.o .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g
LD = CC
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g
INCLDIRS = 
CC = CC
LPRFLAGS = 
AR = /usr/ccs/bin/ar
LPR = mycpr2on1
ENDLIST = 

.c.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<
.cpp.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<

all:		 \
		demangle \
		$(ENDLIST)
		touch all

:		$(ENDLIST)
		$(LD) $(LDFLAGS) -o  \
			$(ENDLIST)

demangle:		demangle.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o demangle \
			demangle.o \
			-ldemangle \
			$(ENDLIST)
		chmod 755 demangle
		cp demangle /home/rumore/bin/demangle
		rcp demangle integ@lts02:/home/integ/bin/demangle
		rcp demangle integ@ihlbs1:/home/integ/bin/demangle

demangle.o:	demangle.c \
		$(ENDLIST)

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-rm -f demangle
		-rm -f demangle.o

backup:		demangle.c \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp demangle.c $(BACKUPDIR)/demangle.c
		touch backup

print:		demangle.c \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			demangle.c \
			$(ENDLIST)
