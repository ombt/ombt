#
# Copyright (C) 2010, OMBT LLC and Mike A. Rumore
# All rights reserved.
# Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
#

.SUFFIXES:		.o .c

LEXFLAGS = -n
YACC = yacc
YACCFLAGS = -d -v
LEX = lex
MAKE = make
LDFLAGS = -g
LD = g++
BACKUPDIR = BACKUP
SHELLBIN = $(HOME)/bin
ARFLAGS = -r -v
CCFLAGS = -g -DDEBUG
INCLDIRS = -I/opt/x11r5/include
CC = g++
AR = ar
ENDLIST = 

.c.o:
		$(CC) $(CCFLAGS) $(INCLDIRS) -c $<

all:		mygraph \
		$(ENDLIST)
		touch all

mygraph:		mygraph.o \
		$(ENDLIST)
		$(LD) $(LDFLAGS) -o mygraph \
			mygraph.o \
			-lX11 \
			$(ENDLIST)
		chmod 755 mygraph
		cp mygraph /home/rumore/bin/mygraph

mygraph.o:	mygraph.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)

clean:
		-rm -rf Templates.DB
		-rm -f mygraph
		-rm -f mygraph.o

backup:		mygraph.c \
		./debug.h \
		./returns.h \
		$(ENDLIST)
		-([ ! -d "./$(BACKUPDIR)" ] && mkdir ./$(BACKUPDIR);)
		cp mygraph.c $(BACKUPDIR)/mygraph.c
		cp ./debug.h ./$(BACKUPDIR)/debug.h
		cp ./returns.h ./$(BACKUPDIR)/returns.h
		touch backup
