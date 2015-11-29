# ROOT = /home/ombt/ombt

ifndef ROOT
$(error ROOT is not set)
endif

include $(ROOT)/build/makefile.common

CXXEXTRAFLAGS = -Wfatal-errors

LIBNAME = random

FILES = Random.cpp

include $(ROOT)/build/makefile.lib2

Random.o:	Random.cpp \
		Random.h
