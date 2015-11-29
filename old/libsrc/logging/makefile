# ROOT = /home/ombt/ombt

ifndef ROOT
$(error ROOT is not set)
endif

include $(ROOT)/build/makefile.common

CXXEXTRAFLAGS = -Wfatal-errors

LIBNAME = logging

FILES = Logging.cpp

include $(ROOT)/build/makefile.lib2

Logging.o:	Logging.cpp \
		Logging.h
