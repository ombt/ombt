# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../../..
endif

include $(ROOT)/build/makefile.common

BINARYDIR = $(ROOT)/$(RELEASEBINDIR)

LIBRARYDIR = $(ROOT)/$(RELEASELIBDIR)

INCLUDEDIR = $(ROOT)/$(RELEASEHDRDIR)

CXX = g++
CXXFLAGS = -g -DDEBUG -I. -I$(INCLUDEDIR) -L$(LIBRARYDIR) -lombtrandom
CXXFLAGS += -Wfatal-errors

all:	graphoverlap

test:	

graphoverlap:	\
	graphoverlap.o \
	graph.o \
	tokenizer.o
	$(CXX) -o graphoverlap graphoverlap.o graph.o tokenizer.o

graphoverlap.o:	graphoverlap.cpp graph.h debug.h
graph.o:	graph.cpp graph.h tokenizer.h debug.h
tokenizer.o:	tokenizer.cpp tokenizer.h debug.h

clean:
	-rm -f install
	-rm -f *.o
	-rm -f graphoverlap 
	-rm -f $(BINARYDIR)/graphoverlap

install:
	cp graphoverlap $(BINARYDIR)/graphoverlap
	touch install

backup:
	bkup

backupd:
	bkup -d

