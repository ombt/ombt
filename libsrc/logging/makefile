# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
endif

include $(ROOT)/build/makefile.common

CXXEXTRAFLAGS = -Wfatal-errors

LIBNAME = logging

FILES = Logging.cpp

include $(ROOT)/build/makefile.lib2

Logging.o:	Logging.cpp \
		Logging.h
