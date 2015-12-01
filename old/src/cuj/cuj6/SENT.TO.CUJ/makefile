#
# Copyright (C) 2010, OMBT LLC and Mike A. Rumore
# All rights reserved.
# Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
#

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

all:		reader \
		writer \
		$(ENDLIST)
		touch all

reader:		reader.o \
		vsem.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o reader \
			reader.o \
			vsem.o \
			$(ENDLIST)

writer:		writer.o \
		vsem.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o writer \
			writer.o \
			vsem.o \
			$(ENDLIST)

reader.o:	reader.c \
		./vsem.h \
		./debug.h \
		./returns.h \
		./vshm5.h \
		$(ENDLIST)

vsem.o:	vsem.c \
		./vsem.h \
		./debug.h \
		./returns.h \
		$(ENDLIST)

writer.o:	writer.c \
		./vsem.h \
		./debug.h \
		./returns.h \
		./vshm5.h \
		$(ENDLIST)

clean:
		-([ -d "Templates.DB" ] && rm -rf Templates.DB;)
		-([ -d "SunWS_cache" ] && rm -rf SunWS_cache;)
		-rm -f reader
		-rm -f writer
		-rm -f reader.o
		-rm -f vsem.o
		-rm -f writer.o

backup:		reader.c \
		vsem.c \
		writer.c \
		./debug.h \
		./vsem.h \
		./returns.h \
		./vshm5.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp reader.c $(BACKUPDIR)/reader.c
		cp vsem.c $(BACKUPDIR)/vsem.c
		cp writer.c $(BACKUPDIR)/writer.c
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./vsem.h ./$(BACKUPDIR)/vsem.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		cp ./vshm5.h ./$(BACKUPDIR)/vshm5.h
		touch backup

print:		reader.c \
		vsem.c \
		writer.c \
		./debug.h \
		./vsem.h \
		./returns.h \
		./vshm5.h \
		$(ENDLIST)
		$(LPR) $(LPRFLAGS) \
			reader.c \
			vsem.c \
			writer.c \
			./debug.h \
			./vsem.h \
			./returns.h \
			./vshm5.h \
			$(ENDLIST)
