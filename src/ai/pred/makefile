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
LDFLAGS = -g -O0 -L$(LIBRARYDIR) 
LDFLAGS += -Wfatal-errors -Wno-deprecated

LEX = /usr/bin/lex
LEXFLAGS = -n
YACC = /usr/bin/yacc
YACCFLAGS = -d -v

all:	tpred

test:	

tpred:		predyacc.o \
		predlex.o \
		clause.o \
		irclause.o \
		semantic.o \
		substitution.o \
		symbol.o \
		term.o \
		term_syntax.o \
		term_tree.o \
		unification.o
		$(LD) $(LDFLAGS) -o tpred \
			predyacc.o \
			predlex.o \
			clause.o \
			irclause.o \
			semantic.o \
			substitution.o \
			symbol.o \
			term.o \
			term_syntax.o \
			term_tree.o \
			unification.o \
			-lombtadt \
			-ly \
			-ll


clean:
	-rm -f install
	-rm -f *.o
	-rm -f tpred
	-rm -f $(BINARYDIR)/tpred

install:
	cp tpred $(BINARYDIR)/tpred
	touch install

backup:
	bkup

backupd:
	bkup -d

clause.o:	clause.cpp \
		clause.h

irclause.o:	irclause.cpp \
		irclause.h

predlex.o:	pred.l \
		./pred.h \
		pred.y 
		$(LEX) $(LEXFLAGS) pred.l
		cp lex.yy.c predlex.cpp
		$(CXX) $(CXXFLAGS) -c predlex.cpp


predyacc.o:	pred.y \
		./pred.h
		$(YACC) $(YACCFLAGS) pred.y
		cp y.tab.h predyacc.h
		cp y.tab.c predyacc.cpp
		$(CXX) $(CXXFLAGS) -c predyacc.cpp

semantic.o:	semantic.cpp \
		semantic.h

substitution.o:	substitution.cpp \
		substitution.h

symbol.o:	symbol.cpp \
		symbol.h

term.o:		term.cpp \
		term.h

term_syntax.o:	term_syntax.cpp \
		term_syntax.h

term_tree.o:	term_tree.cpp \
		term_tree.h

unification.o:	unification.cpp \
		unification.h

