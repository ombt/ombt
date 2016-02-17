
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

all:		mybc \
		$(ENDLIST)
		touch all

mybc:		mybcyacc.o \
		mybclex.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o mybc \
			mybcyacc.o \
			mybclex.o \
			-ll \
			-ly \
			-lm \
			$(ENDLIST)

mybcyacc.o:	mybc.y \
		./mybc.h \
		$(ENDLIST)
		$(YACC) $(YACCFLAGS) mybc.y
		mv y.tab.h mybcyacc.h
		mv y.tab.c mybcyacc.c
		$(CC) $(CCFLAGS) $(INCLDIRS) -c mybcyacc.c

mybclex.o:	mybc.l \
		./mybc.h \
		mybc.y \
		$(ENDLIST)
		$(LEX) $(LEXFLAGS) mybc.l
		mv lex.yy.c mybclex.c
		$(CC) $(CCFLAGS) $(INCLDIRS) -c mybclex.c

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-rm -f mybc
		-rm -f mybcyacc.o
		-rm -f mybclex.o

backup:		mybc.y \
		mybc.l \
		./mybc.h \
		./mybcyacc.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp mybc.y $(BACKUPDIR)/mybc.y
		cp mybc.l $(BACKUPDIR)/mybc.l
		cp ./mybc.h ./$(BACKUPDIR)/mybc.h
		cp ./mybcyacc.h ./$(BACKUPDIR)/mybcyacc.h
		touch backup

print:		mybc.y \
		mybc.l \
		./mybc.h \
		./mybcyacc.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			mybc.y \
			mybc.l \
			./mybc.h \
			./mybcyacc.h \
			$(ENDLIST)
