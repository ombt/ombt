#
# Copyright (C) 2010, OMBT LLC and Mike A. Rumore
# All rights reserved.
# Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
#
# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../../..
endif

include $(ROOT)/build/makefile.common

BINARYDIR = $(ROOT)/$(RELEASEBINDIR)

LIBRARYDIR = $(ROOT)/$(RELEASELIBDIR)

INCLUDEDIR = $(ROOT)/$(RELEASEHDRDIR)

CXX = g++
CXXFLAGS = -g -O0 -DUSEOSTREAM=1 -I$(INCLUDEDIR) -I. 
CXXFLAGS += -Wfatal-errors -Wno-deprecated

LD = g++
LDFLAGS = -g -O0 -L$(LIBRARYDIR) -lombtadt
LDFLAGS += -Wfatal-errors -Wno-deprecated

LEX = /usr/bin/lex
LEXFLAGS = -n
YACC = /usr/bin/yacc
YACCFLAGS = -d -v


all:	mybc

test:	

depend:

mybc:		mybcyacc.o \
		mybclex.o 
		$(LD) $(LDFLAGS) -o mybc \
			mybcyacc.o \
			mybclex.o \
			-ly \
			-ll \
			-lm


clean:
	-rm -f install
	-rm -f *.o
	-rm -f mybc
	-rm -f $(BINARYDIR)/mybc

install:	mybc
	cp mybc $(BINARYDIR)/mybc
	touch install

backup:
	bkup

backupd:
	bkup -d


mybclex.o:	mybc.l \
		./mybc.h \
		mybc.y 
		$(LEX) $(LEXFLAGS) mybc.l
		cp lex.yy.c mybclex.cpp
		$(CXX) $(CXXFLAGS) -c mybclex.cpp

mybcyacc.o:	mybc.y \
		./mybc.h
		$(YACC) $(YACCFLAGS) mybc.y
		cp y.tab.h mybcyacc.h
		cp y.tab.c mybcyacc.cpp
		$(CXX) $(CXXFLAGS) -c mybcyacc.cpp
