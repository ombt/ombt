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
LDFLAGS = -g -O0 -L$(LIBRARYDIR)
LDFLAGS += -Wfatal-errors -Wno-deprecated

all:	gp

test:	

gp:	gp.o
	$(LD) $(LDFLAGS) -o gp \
		gp.o \
		-lrt

clean:
	-rm -f install
	-rm -f *.o
	-rm -f gp
	-rm -f $(BINARYDIR)/gp

install:
	cp gp $(BINARYDIR)/gp
	touch install

depend:

gp.o:	gp.cpp 

