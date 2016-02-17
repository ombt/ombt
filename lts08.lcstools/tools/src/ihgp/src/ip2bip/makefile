
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g -DDEBUG
LD = CC
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g -DDEBUG
INCLDIRS = 
CC = CC
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

all:		ip2bip \
		$(ENDLIST)
		touch all

ip2bip:		ip2bip.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o ip2bip \
			ip2bip.o \
			$(ENDLIST)
		chmod 755 ip2bip
		rcp ip2bip rumore@ihlssb1:/home/rumore/bin/ip2bip
		rcp ip2bip rumore@3gmsclsp:/home/rumore/bin/ip2bip

ip2bip.o:	ip2bip.c \
		$(ENDLIST)

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f ip2bip
		-rm -f ip2bip.o

backup:		ip2bip.c \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp ip2bip.c $(BACKUPDIR)/ip2bip.c
		touch backup

print:		ip2bip.c \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			ip2bip.c \
			$(ENDLIST)
