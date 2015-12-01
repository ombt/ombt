#
# Copyright (C) 2010, OMBT LLC and Mike A. Rumore
# All rights reserved.
# Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
#
# ROOT = /home/ombt/ombt
#
# old make.in file from Solaris env.
#
# CC = CC
# CCFLAGS = -DDEBUG -DUSEEXCEPTION -g -xildoff
# LD = CC
# LDFLAGS = -DDEBUG -DUSEEXCEPTION -g -xildoff
# #
# atp
# pr.y
# pr.l
# array.c
# binaryTree_AVL.c
# clause.c
# deletion.c
# expandfiles.c
# index.c
# literal.c
# main.c
# map.c
# mylist.c
# mystring.c
# prover.c
# queue_List.c
# semantic.c
# stack_List.c
# substitution.c
# symbol.c
# unification.c
# uniquename.c
# globals.c
# search.c
# bfsnode.c
# options.c
# memoryleak.c
# varnames.c
# -ly
# -ll
# #
# install1 = rm -f $(HOME)/bin/atp
# install2 = ln -s $(PWD)/atp $(HOME)/bin/atp
# install3 = cp $(PWD)/atp $(HOME)/public_html/predcal/cgi-bin/atp
# #

ifndef ROOT
ROOT = $(PWD)/../../../..
endif

include $(ROOT)/build/makefile.common

BINARYDIR = $(ROOT)/$(RELEASEBINDIR)

LIBRARYDIR = $(ROOT)/$(RELEASELIBDIR)

INCLUDEDIR = $(ROOT)/$(RELEASEHDRDIR)

SYSTEMINCLUDEDIR = $(ROOT)/$(RELEASEHDRDIR)/system

CXX = g++
CXXFLAGS = -g -O0 -DUSEOSTREAM=1 -I$(SYSTEMINCLUDEDIR) -I$(INCLUDEDIR) -I. 
CXXFLAGS += -Wfatal-errors -Wno-deprecated

LD = g++
LDFLAGS = -g -O0 -L$(LIBRARYDIR) 
LDFLAGS += -Wfatal-errors -Wno-deprecated

LEX = /usr/bin/lex
LEXFLAGS = -n
YACC = /usr/bin/yacc
YACCFLAGS = -d -v


all:	atp

test:	

atp:		pryacc.o \
		prlex.o \
		bfsnode.o \
		clause.o \
		deletion.o \
		expandfiles.o \
		globals.o \
		index.o \
		literal.o \
		main.o \
		memoryleak.o \
		options.o \
		prover.o \
		search.o \
		semantic.o \
		substitution.o \
		symbol.o \
		unification.o \
		uniquename.o \
		varnames.o
		$(LD) $(LDFLAGS) -o atp \
			pryacc.o \
			prlex.o \
			bfsnode.o \
			clause.o \
			deletion.o \
			expandfiles.o \
			globals.o \
			index.o \
			literal.o \
			main.o \
			memoryleak.o \
			options.o \
			prover.o \
			search.o \
			semantic.o \
			substitution.o \
			symbol.o \
			unification.o \
			uniquename.o \
			varnames.o \
			-lombtadt \
			-ly \
			-ll


clean:
	-rm -f install
	-rm -f *.o
	-rm -f atp
	-rm -f $(BINARYDIR)/atp

install:
	cp atp $(BINARYDIR)/atp
	touch install

backup:
	bkup

backupd:
	bkup -d

prlex.o:	pr.l \
		./pr.h \
		./semantic.h \
		pr.y 
		$(LEX) $(LEXFLAGS) pr.l
		cp lex.yy.c prlex.cpp
		$(CXX) $(CXXFLAGS) -c prlex.cpp

pryacc.o:	pr.y \
		./pr.h \
		./semantic.h 
		$(YACC) $(YACCFLAGS) pr.y
		cp y.tab.h pryacc.h
		cp y.tab.c pryacc.cpp
		$(CXX) $(CXXFLAGS) -c pryacc.cpp

bfsnode.o:	bfsnode.cpp bfsnode.h

clause.o:	clause.cpp clause.h

deletion.o:	deletion.cpp deletion.h

expandfiles.o:	expandfiles.cpp

globals.o:	globals.cpp globals.h

index.o:	index.cpp index.h

literal.o:	literal.cpp literal.h

main.o:		main.cpp

memoryleak.0:	memoryleak.cpp memoryleak.h

options.o:	options.h 

prover.o:	prover.cpp

search.o:	search.cpp search.h

semantic.o:	semantic.cpp semantic.h

substitution.o:	substitution.cpp substitution.h

symbol.o:	symbol.cpp symbol.h

unification.o:	unification.cpp unification.h

uniquename.o:	uniquename.cpp uniquename.h

varnames.o:	varnames.cpp varnames.h


