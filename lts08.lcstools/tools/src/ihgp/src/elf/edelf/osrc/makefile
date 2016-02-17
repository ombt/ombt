
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g -DDEBUG -DUSEEXCEPTION
LD = CC
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g -DDEBUG -DUSEEXCEPTION
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

all:		OLDedelf58 \
		$(ENDLIST)
		touch all

OLDedelf58:		edelf.o \
		data.o \
		fhdr.o \
		menu.o \
		shdrs.o \
		phdrs.o \
		strings.o \
		symbols.o \
		reloc.o \
		tokenize.o \
		util.o \
		overwriter.o \
		trans.o \
		hash.o \
		versions.o \
		dynamic.o \
		dasm.o \
		stabs.o \
		spaces.o \
		stabtokenizer.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o OLDedelf58 \
			edelf.o \
			data.o \
			fhdr.o \
			menu.o \
			shdrs.o \
			phdrs.o \
			strings.o \
			symbols.o \
			reloc.o \
			tokenize.o \
			util.o \
			overwriter.o \
			trans.o \
			hash.o \
			versions.o \
			dynamic.o \
			dasm.o \
			stabs.o \
			spaces.o \
			stabtokenizer.o \
			-ldemangle \
			$(ENDLIST)
		chmod 755 OLDedelf58
		rm -f /home/rumore/bin/OLDedelf58
		ln -s `pwd`/OLDedelf58 /home/rumore/bin/OLDedelf58

menu.o:	menu.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./returns.h \
		./tokenize.h \
		$(ENDLIST)

tokenize.o:	tokenize.c \
		./tokenize.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

symbols.o:	symbols.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./returns.h \
		./tokenize.h \
		./trans.h \
		$(ENDLIST)

edelf.o:	edelf.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./returns.h \
		./tokenize.h \
		$(ENDLIST)

stabtokenizer.o:	stabtokenizer.c \
		./stabtokenizer.h \
		./debug.h \
		./returns.h \
		./spaces.h \
		$(ENDLIST)

overwriter.o:	overwriter.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./returns.h \
		./tokenize.h \
		./trans.h \
		$(ENDLIST)

reloc.o:	reloc.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./returns.h \
		./tokenize.h \
		./trans.h \
		$(ENDLIST)

strings.o:	strings.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./returns.h \
		./tokenize.h \
		$(ENDLIST)

fhdr.o:	fhdr.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./returns.h \
		./tokenize.h \
		./trans.h \
		$(ENDLIST)

dynamic.o:	dynamic.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./returns.h \
		./tokenize.h \
		./trans.h \
		$(ENDLIST)

util.o:	util.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./returns.h \
		./tokenize.h \
		$(ENDLIST)

stabs.o:	stabs.c \
		./data.h \
		./edelf.h \
		./debug.h \
		./returns.h \
		./tokenize.h \
		./stabtokenizer.h \
		./spaces.h \
		./trans.h \
		$(ENDLIST)

hash.o:	hash.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./returns.h \
		./tokenize.h \
		./trans.h \
		$(ENDLIST)

dasm.o:	dasm.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./returns.h \
		./tokenize.h \
		./trans.h \
		$(ENDLIST)

versions.o:	versions.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./returns.h \
		./tokenize.h \
		./trans.h \
		$(ENDLIST)

shdrs.o:	shdrs.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./returns.h \
		./tokenize.h \
		./trans.h \
		$(ENDLIST)

phdrs.o:	phdrs.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./returns.h \
		./tokenize.h \
		./trans.h \
		$(ENDLIST)

spaces.o:	spaces.c \
		./spaces.h \
		$(ENDLIST)

data.o:	data.c \
		./data.h \
		$(ENDLIST)

trans.o:	trans.c \
		./tokenize.h \
		./debug.h \
		./returns.h \
		./trans.h \
		$(ENDLIST)

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-([ -f "ir.out" ] && rm -rf ir.out;)
		-rm -f OLDedelf58
		-rm -f menu.o
		-rm -f tokenize.o
		-rm -f symbols.o
		-rm -f edelf.o
		-rm -f stabtokenizer.o
		-rm -f overwriter.o
		-rm -f reloc.o
		-rm -f strings.o
		-rm -f fhdr.o
		-rm -f dynamic.o
		-rm -f util.o
		-rm -f stabs.o
		-rm -f hash.o
		-rm -f dasm.o
		-rm -f versions.o
		-rm -f shdrs.o
		-rm -f phdrs.o
		-rm -f spaces.o
		-rm -f data.o
		-rm -f trans.o

backup:		menu.c \
		tokenize.c \
		symbols.c \
		edelf.c \
		stabtokenizer.c \
		overwriter.c \
		reloc.c \
		strings.c \
		fhdr.c \
		dynamic.c \
		util.c \
		stabs.c \
		hash.c \
		dasm.c \
		versions.c \
		shdrs.c \
		phdrs.c \
		spaces.c \
		data.c \
		trans.c \
		./debug.h \
		./tokenize.h \
		./edelf.h \
		./stabtokenizer.h \
		./returns.h \
		./spaces.h \
		./data.h \
		./trans.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp menu.c $(BACKUPDIR)/menu.c
		cp tokenize.c $(BACKUPDIR)/tokenize.c
		cp symbols.c $(BACKUPDIR)/symbols.c
		cp edelf.c $(BACKUPDIR)/edelf.c
		cp stabtokenizer.c $(BACKUPDIR)/stabtokenizer.c
		cp overwriter.c $(BACKUPDIR)/overwriter.c
		cp reloc.c $(BACKUPDIR)/reloc.c
		cp strings.c $(BACKUPDIR)/strings.c
		cp fhdr.c $(BACKUPDIR)/fhdr.c
		cp dynamic.c $(BACKUPDIR)/dynamic.c
		cp util.c $(BACKUPDIR)/util.c
		cp stabs.c $(BACKUPDIR)/stabs.c
		cp hash.c $(BACKUPDIR)/hash.c
		cp dasm.c $(BACKUPDIR)/dasm.c
		cp versions.c $(BACKUPDIR)/versions.c
		cp shdrs.c $(BACKUPDIR)/shdrs.c
		cp phdrs.c $(BACKUPDIR)/phdrs.c
		cp spaces.c $(BACKUPDIR)/spaces.c
		cp data.c $(BACKUPDIR)/data.c
		cp trans.c $(BACKUPDIR)/trans.c
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./tokenize.h ./$(BACKUPDIR)/tokenize.h
		cp ./edelf.h ./$(BACKUPDIR)/edelf.h
		cp ./stabtokenizer.h ./$(BACKUPDIR)/stabtokenizer.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		cp ./spaces.h ./$(BACKUPDIR)/spaces.h
		cp ./data.h ./$(BACKUPDIR)/data.h
		cp ./trans.h ./$(BACKUPDIR)/trans.h
		touch backup

print:		menu.c \
		tokenize.c \
		symbols.c \
		edelf.c \
		stabtokenizer.c \
		overwriter.c \
		reloc.c \
		strings.c \
		fhdr.c \
		dynamic.c \
		util.c \
		stabs.c \
		hash.c \
		dasm.c \
		versions.c \
		shdrs.c \
		phdrs.c \
		spaces.c \
		data.c \
		trans.c \
		./debug.h \
		./tokenize.h \
		./edelf.h \
		./stabtokenizer.h \
		./returns.h \
		./spaces.h \
		./data.h \
		./trans.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			menu.c \
			tokenize.c \
			symbols.c \
			edelf.c \
			stabtokenizer.c \
			overwriter.c \
			reloc.c \
			strings.c \
			fhdr.c \
			dynamic.c \
			util.c \
			stabs.c \
			hash.c \
			dasm.c \
			versions.c \
			shdrs.c \
			phdrs.c \
			spaces.c \
			data.c \
			trans.c \
			./debug.h \
			./tokenize.h \
			./edelf.h \
			./stabtokenizer.h \
			./returns.h \
			./spaces.h \
			./data.h \
			./trans.h \
			$(ENDLIST)
