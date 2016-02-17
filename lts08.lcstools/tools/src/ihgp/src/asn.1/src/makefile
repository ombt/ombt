
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

all:		asn1cc \
		asn1l \
		asn1pp \
		$(ENDLIST)
		touch all

asn1cc:		asn1cc.sh \
		$(ENDLIST)
		-rm -f $(SHELLBIN)/asn1cc
		cp asn1cc.sh asn1cc
		chmod +x asn1cc
		cp asn1cc $(SHELLBIN)/asn1cc

asn1l:		asn1lex.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o asn1l \
			asn1lex.o \
			-ll \
			$(ENDLIST)
		cp asn1l $(HOME)/bin/asn1l

asn1pp:		asn1pp.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o asn1pp \
			asn1pp.o \
			$(ENDLIST)
		cp asn1pp $(HOME)/bin/asn1pp

asn1pp.o:	asn1pp.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)

asn1lex.o:	asn1.l \
		./asn1.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)
		$(LEX) $(LEXFLAGS) asn1.l
		mv lex.yy.c asn1lex.c
		$(CC) $(CCFLAGS) $(INCLDIRS) -c asn1lex.c

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f asn1cc
		-rm -f asn1l
		-rm -f asn1pp
		-rm -f asn1pp.o
		-rm -f asn1lex.o

backup:		asn1pp.c \
		asn1.l \
		asn1cc.sh \
		./debug.h \
		./asn1yacc.h \
		./returns.h \
		./asn1.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp asn1pp.c $(BACKUPDIR)/asn1pp.c
		cp asn1.l $(BACKUPDIR)/asn1.l
		cp asn1cc.sh $(BACKUPDIR)/asn1cc.sh
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./asn1yacc.h ./$(BACKUPDIR)/asn1yacc.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		cp ./asn1.h ./$(BACKUPDIR)/asn1.h
		touch backup

print:		asn1pp.c \
		asn1.l \
		asn1cc.sh \
		./debug.h \
		./asn1yacc.h \
		./returns.h \
		./asn1.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			asn1pp.c \
			asn1.l \
			asn1cc.sh \
			./debug.h \
			./asn1yacc.h \
			./returns.h \
			./asn1.h \
			$(ENDLIST)
