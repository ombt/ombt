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

all:	graphoverlap2

test:	

graphoverlap2:	\
	graphoverlap2.o \
	graph.o \
	tokenizer.o
	$(CXX) -o graphoverlap2 graphoverlap2.o graph.o tokenizer.o

graphoverlap2.o:	graphoverlap2.cpp graph.h debug.h
graph.o:	graph.cpp graph.h tokenizer.h debug.h
tokenizer.o:	tokenizer.cpp tokenizer.h debug.h

clean:
	-rm -f install
	-rm -f *.o
	-rm -f graphoverlap2
	-rm -f $(BINARYDIR)/graphoverlap2

install:
	cp graphoverlap2 $(BINARYDIR)/graphoverlap2
	touch install

backup:
	bkup

backupd:
	bkup -d

