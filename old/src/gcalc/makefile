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
CXXFLAGS = -g -DUSEOSTREAM=1 -I/usr/include/gtk-1.2/ -I/usr/include/glib-2.0/ -I$(INCLUDEDIR) -L$(LIBDIR) 
CXXFLAGS += -Wfatal-errors

all:	gcalc

test:	

gcalc:	gcalc.o
	$(CXX) -o gcalc gcalc.o 

gcalc.o:	gcalc.cpp 

clean:
	-rm -f install
	-rm -f *.o
	-rm -f gcalc 
	-rm -f $(BINDIR)/bin/gcalc

install:
	cp gcalc $(BINDIR)/gcalc
	touch install

backup:
	bkup

backupd:
	bkup -d

