# ROOT = /home/ombt/ombt

ifndef ROOT
ROOT = $(PWD)/../../..
endif

include $(ROOT)/build/makefile.common

PRODS = \
	gcalc

CXXEXTRAFLAGS = 
CXXLIBFLAGS = `pkg-config --cflags --libs gtk+-2.0` 

include $(ROOT)/build/makefile.src2

