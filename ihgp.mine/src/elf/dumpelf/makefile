
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

all:		dumpelf \
		$(ENDLIST)
		touch all

dumpelf:		dumpelf.o \
		data.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o dumpelf \
			dumpelf.o \
			data.o \
			-lelf \
			-ldemangle \
			$(ENDLIST)
		chmod 755 dumpelf
		cp dumpelf /home/rumore/bin/dumpelf
		rcp dumpelf integ@ihlbs1:/home/integ/bin/dumpelf

data.o:	data.c \
		./data.h \
		$(ENDLIST)

dumpelf.o:	dumpelf.c \
		./data.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f dumpelf
		-rm -f data.o
		-rm -f dumpelf.o

backup:		data.c \
		dumpelf.c \
		./debug.h \
		./returns.h \
		./data.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp data.c $(BACKUPDIR)/data.c
		cp dumpelf.c $(BACKUPDIR)/dumpelf.c
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		cp ./data.h ./$(BACKUPDIR)/data.h
		touch backup

print:		data.c \
		dumpelf.c \
		./debug.h \
		./returns.h \
		./data.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			data.c \
			dumpelf.c \
			./debug.h \
			./returns.h \
			./data.h \
			$(ENDLIST)
