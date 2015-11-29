
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g -DYYDEBUG -DDEBUG -DUSEEXCEPTION
LD = CC
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g -DYYDEBUG -DDEBUG -DUSEEXCEPTION
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

all:		tstabtokenizer \
		tinteger \
		tstab \
		tstring \
		treal \
		stabp \
		tname \
		thexinteger \
		$(ENDLIST)
		touch all

tstabtokenizer:		tstabtokenizer.o \
		spaces.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tstabtokenizer \
			tstabtokenizer.o \
			spaces.o \
			$(ENDLIST)

tinteger:		tinteger.o \
		spaces.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tinteger \
			tinteger.o \
			spaces.o \
			$(ENDLIST)

tstab:		tstab.o \
		spaces.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tstab \
			tstab.o \
			spaces.o \
			$(ENDLIST)

tstring:		tstring.o \
		spaces.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tstring \
			tstring.o \
			spaces.o \
			$(ENDLIST)

treal:		treal.o \
		spaces.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o treal \
			treal.o \
			spaces.o \
			$(ENDLIST)

stabp:		stabyacc.o \
		stablex.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o stabp \
			stabyacc.o \
			stablex.o \
			-ldemangle \
			$(ENDLIST)
		chmod 755 stabp
		rm -f /home/rumore/bin/stabp
		ln -s `pwd`/stabp /home/rumore/bin/stabp

tname:		tname.o \
		spaces.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o tname \
			tname.o \
			spaces.o \
			$(ENDLIST)

thexinteger:		thexinteger.o \
		spaces.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o thexinteger \
			thexinteger.o \
			spaces.o \
			$(ENDLIST)

stabyacc.o:	stab.y \
		$(ENDLIST)
		$(YACC) $(YACCFLAGS) stab.y
		mv y.tab.h stabyacc.h
		mv y.tab.c stabyacc.c
		$(CC) $(CCFLAGS) $(INCLDIRS) -c stabyacc.c

thexinteger.o:	thexinteger.c \
		$(ENDLIST)

tinteger.o:	tinteger.c \
		$(ENDLIST)

treal.o:	treal.c \
		$(ENDLIST)

tstab.o:	tstab.c \
		./spaces.h \
		./stabtokenizer.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

tstabtokenizer.o:	tstabtokenizer.c \
		./stabtokenizer.h \
		./debug.h \
		./returns.h \
		./spaces.h \
		$(ENDLIST)

spaces.o:	spaces.c \
		./spaces.h \
		$(ENDLIST)

tstring.o:	tstring.c \
		$(ENDLIST)

stablex.o:	stab.l \
		stab.y \
		$(ENDLIST)
		$(LEX) $(LEXFLAGS) stab.l
		mv lex.yy.c stablex.c
		$(CC) $(CCFLAGS) $(INCLDIRS) -c stablex.c

tname.o:	tname.c \
		$(ENDLIST)

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f tstabtokenizer
		-rm -f tinteger
		-rm -f tstab
		-rm -f tstring
		-rm -f treal
		-rm -f stabp
		-rm -f tname
		-rm -f thexinteger
		-rm -f stabyacc.o
		-rm -f thexinteger.o
		-rm -f tinteger.o
		-rm -f treal.o
		-rm -f tstab.o
		-rm -f tstabtokenizer.o
		-rm -f spaces.o
		-rm -f tstring.o
		-rm -f stablex.o
		-rm -f tname.o

backup:		stab.y \
		thexinteger.c \
		tinteger.c \
		treal.c \
		tstab.c \
		tstabtokenizer.c \
		spaces.c \
		tstring.c \
		stab.l \
		tname.c \
		./debug.h \
		./stabyacc.h \
		./stabtokenizer.h \
		./returns.h \
		./spaces.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp stab.y $(BACKUPDIR)/stab.y
		cp thexinteger.c $(BACKUPDIR)/thexinteger.c
		cp tinteger.c $(BACKUPDIR)/tinteger.c
		cp treal.c $(BACKUPDIR)/treal.c
		cp tstab.c $(BACKUPDIR)/tstab.c
		cp tstabtokenizer.c $(BACKUPDIR)/tstabtokenizer.c
		cp spaces.c $(BACKUPDIR)/spaces.c
		cp tstring.c $(BACKUPDIR)/tstring.c
		cp stab.l $(BACKUPDIR)/stab.l
		cp tname.c $(BACKUPDIR)/tname.c
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./stabyacc.h ./$(BACKUPDIR)/stabyacc.h
		cp ./stabtokenizer.h ./$(BACKUPDIR)/stabtokenizer.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		cp ./spaces.h ./$(BACKUPDIR)/spaces.h
		touch backup

print:		stab.y \
		thexinteger.c \
		tinteger.c \
		treal.c \
		tstab.c \
		tstabtokenizer.c \
		spaces.c \
		tstring.c \
		stab.l \
		tname.c \
		./debug.h \
		./stabyacc.h \
		./stabtokenizer.h \
		./returns.h \
		./spaces.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			stab.y \
			thexinteger.c \
			tinteger.c \
			treal.c \
			tstab.c \
			tstabtokenizer.c \
			spaces.c \
			tstring.c \
			stab.l \
			tname.c \
			./debug.h \
			./stabyacc.h \
			./stabtokenizer.h \
			./returns.h \
			./spaces.h \
			$(ENDLIST)
