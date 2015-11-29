
.SUFFIXES:		.o .C .CPP .c .cpp

LEXFLAGS = -n
YACC = /usr/ccs/bin/yacc
YACCFLAGS = -d -v
LEX = /usr/ccs/bin/lex
MAKE = make
LDFLAGS = -g -DDEBUG -DUSEEXCEPTION
LD = /opt/SUNWspro/bin/CC
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g -DDEBUG -DUSEEXCEPTION
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
.cpp.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<
.CPP.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<

all:		ocelf \
		$(ENDLIST)
		touch all

ocelf:		ocelf.o \
		compare.o \
		readdata.o \
		swabdata.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o ocelf \
			ocelf.o \
			compare.o \
			readdata.o \
			swabdata.o \
			$(ENDLIST)

ocelf.o:	ocelf.c \
		./ocelf.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

compare.o:	compare.c \
		./ocelf.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

readdata.o:	readdata.c \
		./ocelf.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

swabdata.o:	swabdata.c \
		./ocelf.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

exptools:
		expmake executableTypes=solaris ocelf
		expprovide ocelf
expmake:
		expmake executableTypes=solaris ocelf

expprovide:
		expprovide ocelf
