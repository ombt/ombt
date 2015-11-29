# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
endif

include $(ROOT)/build/makefile.common

CXXEXTRAFLAGS = -Wfatal-errors

LIBNAME = graphs

FILES = Graph.cpp Algorithms.cpp

include $(ROOT)/build/makefile.lib2

Graph.o:	Graph.cpp \
		Graph.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/parsing/Tokenizer.h

Algorithms.o:	Algorithms.cpp \
		Algorithms.h \
		Graph.h \
		$(INCLUDEDIR)/Returns.h \
		$(INCLUDEDIR)/Debug.h \
		$(INCLUDEDIR)/parsing/Tokenizer.h

