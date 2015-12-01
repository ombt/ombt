#
# Copyright (C) 2010, OMBT LLC and Mike A. Rumore
# All rights reserved.
# Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
#
# ROOT = /home/ombt/ombt

ifndef ROOT
  $(error ROOT is not set; see README)
endif

BINDIR = $(ROOT)/bin

LIBDIR = $(ROOT)/lib

INCLUDEDIR = $(ROOT)/hdr

CXX = g++
CXXFLAGS = -g -O0 -DUSEOSTREAM=1 -I$(INCLUDEDIR) -I. 
CXXFLAGS += -Wfatal-errors -Wno-deprecated

LD = g++
LDFLAGS = -g -O0 -L$(LIBDIR) -lombtadt
LDFLAGS += -Wfatal-errors -Wno-deprecated

LEX = /usr/bin/lex
LEXFLAGS = -n
YACC = /usr/bin/yacc
YACCFLAGS = -d -v


all:	tprop

test:	

tprop:		propyacc.o \
		proplex.o \
		atom.o \
		clause.o \
		semantic.o
		$(LD) $(LDFLAGS) -o tprop \
			propyacc.o \
			proplex.o \
			atom.o \
			clause.o \
			semantic.o \
			-ly \
			-ll


clean:
	-rm -f install
	-rm -f *.o
	-rm -f tprop
	-rm -f $(BINDIR)/tprop

install:
	cp tprop $(BINDIR)/tprop
	touch install

backup:
	bkup

backupd:
	bkup -d


atom.o:	atom.cpp \
		./atom.h 

semantic.o:	semantic.cpp \
		./semantic.h \
		./clause.h \
		./atom.h

clause.o:	clause.cpp \
		./clause.h \
		./atom.h

proplex.o:	prop.l \
		./prop.h \
		./clause.h \
		./atom.h \
		./semantic.h \
		prop.y 
		$(LEX) $(LEXFLAGS) prop.l
		cp lex.yy.c proplex.cpp
		$(CXX) $(CXXFLAGS) -c proplex.cpp

propyacc.o:	prop.y \
		./prop.h \
		./clause.h \
		./atom.h \
		./semantic.h 
		$(YACC) $(YACCFLAGS) prop.y
		cp y.tab.h propyacc.h
		cp y.tab.c propyacc.cpp
		$(CXX) $(CXXFLAGS) -c propyacc.cpp
