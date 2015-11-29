
# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../..
endif

include $(ROOT)/build/makefile.common

CXXEXTRAFLAGS = -Wfatal-errors

LIBNAME = logic

FILES = LogicTokenizer.cpp

include $(ROOT)/build/makefile.lib2

LogicTokenizer.o:	\
		LogicTokenizer.cpp \
		LogicTokenizer.h
