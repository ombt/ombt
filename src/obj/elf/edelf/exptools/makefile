
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g -DUSEEXCEPTION -DOS5_5 -DEXPTOOLS=1
LD = /opt/SUNWspro/bin/CC
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g -DUSEEXCEPTION -DOS5_5 -DEXPTOOLS=1
INCLDIRS = 
CC = /opt/SUNWspro/bin/CC
LPRFLAGS = 
AR = /usr/ccs/bin/ar
LPR = mycpr2on1
ENDLIST = 

.c.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<
.C.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<

all:		edelf \
		$(ENDLIST)
		touch all

edelf:		edelf.o \
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
		$(LD) $(LDFLAGS) -o edelf \
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
			-lgen \
			$(ENDLIST)

menu.o:	menu.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./elfobj.h \
		./returns.h \
		./stab.h \
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
		./elfobj.h \
		./returns.h \
		./stab.h \
		./tokenize.h \
		./trans.h \
		$(ENDLIST)

edelf.o:	edelf.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./elfobj.h \
		./returns.h \
		./stab.h \
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
		./elfobj.h \
		./returns.h \
		./stab.h \
		./tokenize.h \
		./trans.h \
		$(ENDLIST)

reloc.o:	reloc.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./elfobj.h \
		./returns.h \
		./stab.h \
		./tokenize.h \
		./trans.h \
		$(ENDLIST)

strings.o:	strings.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./elfobj.h \
		./returns.h \
		./stab.h \
		./tokenize.h \
		$(ENDLIST)

fhdr.o:		fhdr.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./elfobj.h \
		./returns.h \
		./stab.h \
		./tokenize.h \
		./trans.h \
		$(ENDLIST)

dynamic.o:	dynamic.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./elfobj.h \
		./returns.h \
		./stab.h \
		./tokenize.h \
		./trans.h \
		$(ENDLIST)

util.o:		util.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./elfobj.h \
		./returns.h \
		./stab.h \
		./tokenize.h \
		$(ENDLIST)

stabs.o:	stabs.c \
		./data.h \
		./edelf.h \
		./debug.h \
		./elfobj.h \
		./returns.h \
		./stab.h \
		./tokenize.h \
		./stabtokenizer.h \
		./spaces.h \
		./trans.h \
		$(ENDLIST)

hash.o:		hash.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./elfobj.h \
		./returns.h \
		./stab.h \
		./tokenize.h \
		./trans.h \
		$(ENDLIST)

dasm.o:		dasm.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./elfobj.h \
		./returns.h \
		./stab.h \
		./tokenize.h \
		./trans.h \
		$(ENDLIST)

versions.o:	versions.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./elfobj.h \
		./returns.h \
		./stab.h \
		./tokenize.h \
		./trans.h \
		$(ENDLIST)

shdrs.o:	shdrs.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./elfobj.h \
		./returns.h \
		./stab.h \
		./tokenize.h \
		./trans.h \
		$(ENDLIST)

phdrs.o:	phdrs.c \
		./edelf.h \
		./data.h \
		./debug.h \
		./elfobj.h \
		./returns.h \
		./stab.h \
		./tokenize.h \
		./trans.h \
		$(ENDLIST)

spaces.o:	spaces.c \
		./spaces.h \
		$(ENDLIST)

data.o:		data.c \
		./data.h \
		$(ENDLIST)

trans.o:	trans.c \
		./tokenize.h \
		./debug.h \
		./returns.h \
		./trans.h \
		./stab.h \
		$(ENDLIST)

backup:
		backup -d

expclean:
		expmake executableTypes=solaris -clean edelf

expmake:
		expmake executableTypes=solaris edelf

exptools:
		expmake executableTypes=solaris edelf
		expprovide edelf
