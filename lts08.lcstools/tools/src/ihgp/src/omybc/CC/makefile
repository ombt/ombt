
.SUFFIXES:		.o .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g -DSUN
LD = CC
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g -DSUN
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

all:		omybc \
		$(ENDLIST)
		touch all

omybc:		omybcyacc.o \
		omybclex.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o omybc \
			omybcyacc.o \
			omybclex.o \
			-ll \
			-ly \
			-lm \
			$(ENDLIST)
		chmod 755 omybc
		cp omybc /home/rumore/bin/omybc

omybclex.o:	omybc.l \
		./omybc.h \
		omybc.y \
		$(ENDLIST)
		$(LEX) $(LEXFLAGS) omybc.l
		mv lex.yy.c omybclex.c
		$(CC) $(CCFLAGS) $(INCLDIRS) -c omybclex.c

omybcyacc.o:	omybc.y \
		./omybc.h \
		$(ENDLIST)
		$(YACC) $(YACCFLAGS) omybc.y
		mv y.tab.h omybcyacc.h
		mv y.tab.c omybcyacc.c
		$(CC) $(CCFLAGS) $(INCLDIRS) -c omybcyacc.c

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-rm -f omybc
		-rm -f omybclex.o
		-rm -f omybcyacc.o

backup:		omybc.l \
		omybc.y \
		./omybcyacc.h \
		./omybc.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp omybc.l $(BACKUPDIR)/omybc.l
		cp omybc.y $(BACKUPDIR)/omybc.y
		cp ./omybcyacc.h ./$(BACKUPDIR)/omybcyacc.h
		cp ./omybc.h ./$(BACKUPDIR)/omybc.h
		touch backup

print:		omybc.l \
		omybc.y \
		./omybcyacc.h \
		./omybc.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			omybc.l \
			omybc.y \
			./omybcyacc.h \
			./omybc.h \
			$(ENDLIST)
