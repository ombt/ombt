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
CXXFLAGS = -g -DUSEOSTREAM=1 -I$(INCLUDEDIR) -L$(LIBRARYDIR) -lombtrandom
CXXFLAGS += -Wfatal-errors

all:	setuid

test:	

setuid:	setuid.o \
	mydebug.o
	$(CXX) -o setuid setuid.o mydebug.o

setuid.o:	setuid.cpp mydebug.h
mydebug.o:	mydebug.cpp mydebug.h

clean:
	-rm -f install
	-rm -f *.o
	-rm -f setuid 
	-rm -f $(BINARYDIR)/setuid

install:
	cp setuid $(BINARYDIR)/setuid
	touch install

depend:

backup:
	bkup

backupd:
	bkup -d

