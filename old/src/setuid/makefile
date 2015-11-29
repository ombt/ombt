# ROOT = /home/ombt/ombt

ifndef ROOT
  $(error ROOT is not set; see README)
endif

BINDIR = $(ROOT)/bin

LIBDIR = $(ROOT)/lib

INCLUDEDIR = $(ROOT)/hdr

CXX = g++
CXXFLAGS = -g -DUSEOSTREAM=1 -I$(INCLUDEDIR) -L$(LIBDIR) -lombtrandom
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
	-rm -f $(BINDIR)/bin/setuid

install:
	cp setuid $(BINDIR)/setuid
	touch install

backup:
	bkup

backupd:
	bkup -d

