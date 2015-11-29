# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../../..
endif

include $(ROOT)/build/makefile.common

BINARYDIR = $(ROOT)/$(RELEASEBINDIR)

LIBRARYDIR = $(ROOT)/$(RELEASELIBDIR)

INCLUDEDIR = $(ROOT)/$(RELEASEHDRDIR)

CXX = g++
CXXFLAGS = -g -DDEBUG -I. -I$(INCLUDEDIR) -L$(LIBRARYDIR) 
# CXXFLAGS += -lombtgraphs -lombtparsing
CXXFLAGS += -Wfatal-errors

all:	graphoverlap3

test:	

graphoverlap3:	\
	graphoverlap3.o 
	$(CXX) $(CXXFLAGS) -o graphoverlap3 graphoverlap3.o \
		-lombtgraphs \
		-lombtparsing
		

graphoverlap3.o:	\
			graphoverlap3.cpp \
			$(INCLUDEDIR)/graphs/Graph.h \
			$(INCLUDEDIR)/Debug.h \
			$(INCLUDEDIR)/Returns.h

clean:
	-rm -f install
	-rm -f *.o
	-rm -f graphoverlap3
	-rm -f $(BINARYDIR)/graphoverlap3

install:
	cp graphoverlap3 $(BINARYDIR)/graphoverlap3
	touch install

backup:
	bkup

backupd:
	bkup -d

