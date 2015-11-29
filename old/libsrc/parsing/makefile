
# ROOT = /home/ombt/ombt

ifndef ROOT
$(error ROOT is not set)
endif

include $(ROOT)/build/makefile.common

CXXEXTRAFLAGS = -Wfatal-errors

LIBNAME = parsing

FILES = Tokenizer.cpp

include $(ROOT)/build/makefile.lib2

Tokenizer.o:	Tokenizer.cpp \
		Tokenizer.h
